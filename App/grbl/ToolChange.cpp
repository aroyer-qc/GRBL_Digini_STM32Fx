/*
  ToolChange.c - Changing tool
  Part of Grbl-Advanced

  Copyright (c) 2018-2020 Patrick F.

  Grbl-Advanced is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl-Advanced is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl-Advanced.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <string.h>
#include "ToolChange.h"
#include "GCode.h"
#include "MotionControl.h"
#include "Protocol.h"
#include "System.h"
#include "System32.h"
#include "SpindleControl.h"
#include "Settings.h"
#include "Config.h"
#include "ToolTable.h"
#include "defaults.h"


#define TOOL_SENSOR_OFFSET        70.0  // mm


static uint8_t isFirstTC = 1;
static int32_t toolOffset = 0;
static int32_t toolReferenz = 0;
static float tc_pos[N_AXIS] = {0};


void TC_Init(void)
{
    isFirstTC = 1;
    toolOffset = 0;
    toolReferenz = 0;

    memset(tc_pos, 0, sizeof(float)*N_AXIS);

    gc_state.modal.tool_length = TOOL_LENGTH_OFFSET_CANCEL;

    for(uint8_t i = 0; i < N_AXIS; i++)
    {
        gc_state.tool_length_offset[i] = 0.0;
    }
}


void TC_ChangeCurrentTool(void)
{
    Planner_LineData_t pl_data = {0};
    float position[N_AXIS] = {0.0};


    if(sys.state == STATE_CHECK_MODE)
    {
        return;
    }

    // Wait until queue is processed
    Protocol_BufferSynchronize();

    // Move TOOL_LENGTH_OFFSET_AXIS to 0
    System_ConvertArraySteps2Mpos(position, sys_position);
    position[TOOL_LENGTH_OFFSET_AXIS] = 0.0;
    memcpy(tc_pos, position, sizeof(float)*N_AXIS);

    //System_SetExecStateFlag(EXEC_TOOL_CHANGE);

    pl_data.feed_rate = 0.0;
    pl_data.condition |= PL_COND_FLAG_RAPID_MOTION; // Set rapid motion condition flag.
    pl_data.backlash_motion = 0;
    pl_data.spindle_speed = 0;
    pl_data.line_number = gc_state.line_number;

    MC_Line(position, &pl_data);
    Delay_ms(20);

    Spindle_Stop();

    // Wait until queue is processed
    Protocol_BufferSynchronize();

    // Wait until move is finished
    while(sys.state != STATE_IDLE)
    {
        Protocol_ExecuteRealtime(); // Check for any run-time commands

        if(sys.abort)
        {
            // Bail, if system abort.
            return;
        }
    }


    sys.state = STATE_TOOL_CHANGE;

    GC_SyncPosition();
}


void TC_ProbeTLS(void)
{
    Planner_LineData_t pl_data = {0};
    float position[N_AXIS] = {0.0};
    uint8_t flags = 0;


    if(sys.state == STATE_CHECK_MODE || settings.tls_valid == 0)
    {
        return;
    }

    // Move to XY position of TLS
    System_ConvertArraySteps2Mpos(position, settings.tls_position);
    position[TOOL_LENGTH_OFFSET_AXIS] = 0.0;

    // Set-up planer
    pl_data.feed_rate = 0.0;
    pl_data.condition |= PL_COND_FLAG_RAPID_MOTION; // Set rapid motion condition flag.
    pl_data.backlash_motion = 0;
    pl_data.spindle_speed = 0;
    pl_data.line_number = gc_state.line_number;

    // Move to X/Y position of TLS
    MC_Line(position, &pl_data);

    // Move down with offset (for tool)
    position[TOOL_LENGTH_OFFSET_AXIS] = (settings.tls_position[TOOL_LENGTH_OFFSET_AXIS] / settings.steps_per_mm[TOOL_LENGTH_OFFSET_AXIS]) + TOOL_SENSOR_OFFSET;
    MC_Line(position, &pl_data);

    // Wait until queue is processed
    Protocol_BufferSynchronize();

    // Set up fast probing
    pl_data.feed_rate = 220.0;
    pl_data.condition = 0; // Reset rapid motion condition flag.

    // Probe TLS fast
    position[TOOL_LENGTH_OFFSET_AXIS] -= 200.0;
    uint8_t ret = MC_ProbeCycle(position, &pl_data, flags);
    if(ret != GC_PROBE_FOUND)
    {
        // Error
        return;
    }

    // Get current position
    System_ConvertArraySteps2Mpos(position, sys_position);
    position[TOOL_LENGTH_OFFSET_AXIS] += 1.8;

    // Move up a little bit for slow probing
    pl_data.feed_rate = 200.0;
    MC_Line(position, &pl_data);

    // Probe TLS slow
    pl_data.feed_rate = 12.0;
    position[TOOL_LENGTH_OFFSET_AXIS] -= 200;
    ret = MC_ProbeCycle(position, &pl_data, flags);
    if(ret != GC_PROBE_FOUND)
    {
        // Error
        return;
    }

    if(isFirstTC)
    {
        // Save first tool as reference
        isFirstTC = 0;
        toolReferenz = sys_probe_position[TOOL_LENGTH_OFFSET_AXIS];
    }
    else
    {
        // Calculate tool offset
        toolOffset = sys_probe_position[TOOL_LENGTH_OFFSET_AXIS] - toolReferenz;

        // Apply offset as dynamic tool length offset
        gc_state.modal.tool_length = TOOL_LENGTH_OFFSET_ENABLE_DYNAMIC;
        gc_state.tool_length_offset[TOOL_LENGTH_OFFSET_AXIS] = toolOffset / settings.steps_per_mm[TOOL_LENGTH_OFFSET_AXIS];
    }

    Delay_ms(5);

    // Move Z up
    position[TOOL_LENGTH_OFFSET_AXIS] = 0.0;
    pl_data.condition |= PL_COND_FLAG_RAPID_MOTION; // Set rapid motion condition flag.

    MC_Line(position, &pl_data);

    // Move back to initial tc position
    MC_Line(tc_pos, &pl_data);

    // Wait until queue is processed
    Protocol_BufferSynchronize();

    GC_SyncPosition();
}


void TC_ApplyToolOffset(void)
{
    ToolParams_t params = {};

    TT_GetToolParams(gc_state.tool, &params);

    // Apply offset as dynamic tool length offset
    gc_state.modal.tool_length = TOOL_LENGTH_OFFSET_ENABLE_DYNAMIC;

    gc_state.tool_length_offset[X_AXIS] = params.x_offset;
    gc_state.tool_length_offset[Y_AXIS] = params.y_offset;
    gc_state.tool_length_offset[Z_AXIS] = params.z_offset;
}
