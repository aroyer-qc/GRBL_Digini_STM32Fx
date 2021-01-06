/*
  ToolChange.c - Changing tool
  Part of Grbl-Advanced

  Copyright (c)	2018-2019 Patrick F.

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

#include "grbl.h"





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
	gc_state.tool_length_offset = 0.0;
}


void TC_ChangeCurrentTool(void)
{
    plan_line_data_t pl_data = {0};
    float position[N_AXIS] = {0.0};


    if(sys.state == STATE_CHECK_MODE)
    {
		return;
	}

    // Wait until queue is processed
    protocol_buffer_synchronize();

    // Don't move XY. Go to Z 0
	system_convert_array_steps_to_mpos(position, sys_position);
	position[Z_AXIS] = 0.0;
	memcpy(tc_pos, position, sizeof(float)*N_AXIS);

    //system_set_exec_state_flag(EXEC_TOOL_CHANGE);

    pl_data.feed_rate = 0.0;
    pl_data.condition |= PL_COND_FLAG_RAPID_MOTION; // Set rapid motion condition flag.
    pl_data.backlash_motion = 0;
    pl_data.spindle_speed = 0;
  #ifdef USE_LINE_NUMBERS
    pl_data.line_number = gc_state.line_number;
  #endif

	mc_line(position, &pl_data);
	delay_ms(20);

	spindle_stop();

	// Wait until queue is processed
    protocol_buffer_synchronize();

    // Wait until move is finished
    while(sys.state != STATE_IDLE);

    sys.state = STATE_TOOL_CHANGE;

    gc_sync_position();
}


void TC_ProbeTLS(void)
{
    plan_line_data_t pl_data = {0};
    float position[N_AXIS] = {0.0};
    uint8_t flags = 0;


    if(sys.state == STATE_CHECK_MODE || settings.tls_valid == 0)
    {
		return;
	}

	// Move to XY position of TLS
	system_convert_array_steps_to_mpos(position, settings.tls_position);
	position[Z_AXIS] = 0.0;

    // Set-up planer
    pl_data.feed_rate = 0.0;
	pl_data.condition |= PL_COND_FLAG_RAPID_MOTION; // Set rapid motion condition flag.
    pl_data.backlash_motion = 0;
    pl_data.spindle_speed = 0;
    #ifdef USE_LINE_NUMBERS
    pl_data.line_number = gc_state.line_number;
    #endif

    // Move to X/Y position of TLS
	mc_line(position, &pl_data);

    // Move down with offset (for tool)
	position[Z_AXIS] = (settings.tls_position[Z_AXIS] / settings.steps_per_mm[Z_AXIS]) + TOOL_SENSOR_OFFSET;
	mc_line(position, &pl_data);

	// Wait until queue is processed
    protocol_buffer_synchronize();

    // Set up fast probing
    pl_data.feed_rate = 200.0;
	pl_data.condition = 0; // Reset rapid motion condition flag.

    // Probe TLS fast
    position[Z_AXIS] -= 200.0;
    uint8_t ret = mc_probe_cycle(position, &pl_data, flags);
    if(ret != GC_PROBE_FOUND)
    {
        // Error
        return;
    }

    // Get current position
    system_convert_array_steps_to_mpos(position, sys_position);
    position[Z_AXIS] += 1.8;

    // Move up a little bit for slow probing
    pl_data.feed_rate = 200.0;
    mc_line(position, &pl_data);

    // Probe TLS slow
    pl_data.feed_rate = 12.0;
    position[Z_AXIS] -= 200;
    ret = mc_probe_cycle(position, &pl_data, flags);
    if(ret != GC_PROBE_FOUND)
    {
        // Error
        return;
    }

    if(isFirstTC)
    {
        // Save first tool as reference
        isFirstTC = 0;
        toolReferenz = sys_probe_position[Z_AXIS];
    }
    else
    {
        // Calculate tool offset
        toolOffset = sys_probe_position[Z_AXIS] - toolReferenz;

        // Apply offset as dynamic tool length offset
        gc_state.modal.tool_length = TOOL_LENGTH_OFFSET_ENABLE_DYNAMIC;
        gc_state.tool_length_offset = toolOffset / settings.steps_per_mm[Z_AXIS];
    }

    delay_ms(5);

    // Move Z up
    position[Z_AXIS] = 0.0;
    pl_data.condition |= PL_COND_FLAG_RAPID_MOTION; // Set rapid motion condition flag.

    mc_line(position, &pl_data);

    // Move back to initial tc position
    mc_line(tc_pos, &pl_data);

    // Wait until queue is processed
    protocol_buffer_synchronize();

    gc_sync_position();
}
