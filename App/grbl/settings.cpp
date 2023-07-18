/*
  Settings.c - eeprom configuration handling
  Part of Grbl-Advanced

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud
  Copyright (c) 2017-2020 Patrick F.

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

#include "lib_digini.h"
#include "Config.h"
#include "GCode.h"
#include "Limits.h"
#include "Protocol.h"
#include "Probe.h"
#include "Report.h"
#include "Settings.h"
#include "SpindleControl.h"
#include "System.h"
#include "Stepper.h"
#include "defaults.h"
#include <stdint.h>
#include <string.h>


Settings_t Settings;


// Method to store startup lines into EEPROM
void Settings_StoreStartupLine(uint8_t n, char *line)
{
#ifdef FORCE_BUFFER_SYNC_DURING_EEPROM_WRITE
    Protocol_BufferSynchronize(); // A startup line may contain a motion and be executing.
#endif
    DB_Central.Set((uint8_t*)line, GRBL_STARTUP_BLOCK, n);
    // why there is no checksum here?
}


// Method to store build info into EEPROM
// NOTE: This function can only be called in IDLE state.
void Settings_StoreBuildInfo(char *line)
{
    // Build info can only be stored when state is IDLE.
    DB_Central.Set((uint8_t*)line, GRBL_BUILD_INFO);
    // why there is no checksum here?
}


// Method to store coord data parameters into EEPROM
void Settings_WriteCoordData(uint8_t CoordSelect, float *coord_data)
{
  #ifdef FORCE_BUFFER_SYNC_DURING_EEPROM_WRITE
    Protocol_BufferSynchronize();
  #endif

    DB_Central.Set((uint8_t*)coord_data, GRBL_GLOBAL_SETTINGS, CoordSelect);
}


// Method to store Grbl global settings struct and version number into EEPROM
// NOTE: This function can only be called in IDLE state. // TODO don't expect this to be true with digini... need to be check
void WriteGlobalSettings(void)
{
    uint8_t Version = SETTINGS_VERSION;
    DB_Central.Set(&Version, GRBL_SETTINGS_VERSION);
    DB_Central.Set(&Settings, GRBL_GLOBAL_SETTINGS);
}


// Method to restore EEPROM-saved Grbl global settings back to defaults.
void Settings_Restore(uint8_t restore_flag)
{
    if(restore_flag & SETTINGS_RESTORE_DEFAULTS)
    {
        Settings.system_flags = DEFAULT_SYSTEM_INVERT_MASK;
        Settings.stepper_idle_lock_time = DEFAULT_STEPPER_IDLE_LOCK_TIME;
        Settings.step_invert_mask = DEFAULT_STEPPING_INVERT_MASK;
        Settings.dir_invert_mask = DEFAULT_DIRECTION_INVERT_MASK;
        Settings.status_report_mask = DEFAULT_STATUS_REPORT_MASK;
        Settings.junction_deviation = DEFAULT_JUNCTION_DEVIATION;
        Settings.arc_tolerance = DEFAULT_ARC_TOLERANCE;

        Settings.rpm_max = DEFAULT_SPINDLE_RPM_MAX;
        Settings.rpm_min = DEFAULT_SPINDLE_RPM_MIN;

        Settings.homing_dir_mask = DEFAULT_HOMING_DIR_MASK;
        Settings.homing_feed_rate = DEFAULT_HOMING_FEED_RATE;
        Settings.homing_seek_rate = DEFAULT_HOMING_SEEK_RATE;
        Settings.homing_debounce_delay = DEFAULT_HOMING_DEBOUNCE_DELAY;
        Settings.homing_pulloff = DEFAULT_HOMING_PULLOFF;

        // Flags
        Settings.flags = 0;
        if(DEFAULT_REPORT_INCHES)
        {
            Settings.flags |= BITFLAG_REPORT_INCHES;
        }
        if(DEFAULT_LASER_MODE)
        {
            Settings.flags |= BITFLAG_LASER_MODE;
        }
        if(DEFAULT_INVERT_ST_ENABLE)
        {
            Settings.flags |= BITFLAG_INVERT_ST_ENABLE;
        }
        if(DEFAULT_HARD_LIMIT_ENABLE)
        {
            Settings.flags |= BITFLAG_HARD_LIMIT_ENABLE;
        }
        if(DEFAULT_HOMING_ENABLE)
        {
            Settings.flags |= BITFLAG_HOMING_ENABLE;
        }
        if(DEFAULT_SOFT_LIMIT_ENABLE)
        {
            Settings.flags |= BITFLAG_SOFT_LIMIT_ENABLE;
        }
        if(DEFAULT_INVERT_LIMIT_PINS)
        {
            Settings.flags |= BITFLAG_INVERT_LIMIT_PINS;
        }
        if(DEFAULT_INVERT_PROBE_PIN)
        {
            Settings.flags |= BITFLAG_INVERT_PROBE_PIN;
        }

        // Flags2
        Settings.flags2 = 0;
        if(DEFAULT_LATHE_MODE)
        {
            Settings.flags2 |= BITFLAG_LATHE_MODE;
        }

        Settings.steps_per_mm[X_AXIS] = DEFAULT_X_STEPS_PER_MM;
        Settings.steps_per_mm[Y_AXIS] = DEFAULT_Y_STEPS_PER_MM;
        Settings.steps_per_mm[Z_AXIS] = DEFAULT_Z_STEPS_PER_MM;
        Settings.steps_per_mm[A_AXIS] = DEFAULT_A_STEPS_PER_DEG;
        Settings.steps_per_mm[B_AXIS] = DEFAULT_B_STEPS_PER_DEG;

        Settings.max_rate[X_AXIS] = DEFAULT_X_MAX_RATE;
        Settings.max_rate[Y_AXIS] = DEFAULT_Y_MAX_RATE;
        Settings.max_rate[Z_AXIS] = DEFAULT_Z_MAX_RATE;
        Settings.max_rate[A_AXIS] = DEFAULT_A_MAX_RATE;
        Settings.max_rate[B_AXIS] = DEFAULT_B_MAX_RATE;

        Settings.acceleration[X_AXIS] = DEFAULT_X_ACCELERATION;
        Settings.acceleration[Y_AXIS] = DEFAULT_Y_ACCELERATION;
        Settings.acceleration[Z_AXIS] = DEFAULT_Z_ACCELERATION;
        Settings.acceleration[A_AXIS] = DEFAULT_A_ACCELERATION;
        Settings.acceleration[B_AXIS] = DEFAULT_B_ACCELERATION;

        Settings.max_travel[X_AXIS] = (-DEFAULT_X_MAX_TRAVEL);
        Settings.max_travel[Y_AXIS] = (-DEFAULT_Y_MAX_TRAVEL);
        Settings.max_travel[Z_AXIS] = (-DEFAULT_Z_MAX_TRAVEL);
        Settings.max_travel[A_AXIS] = (-DEFAULT_A_MAX_TRAVEL);
        Settings.max_travel[B_AXIS] = (-DEFAULT_B_MAX_TRAVEL);

        Settings.backlash[X_AXIS] = DEFAULT_X_BACKLASH;
        Settings.backlash[Y_AXIS] = DEFAULT_Y_BACKLASH;
        Settings.backlash[Z_AXIS] = DEFAULT_Z_BACKLASH;

        Settings.ToolChange = DEFAULT_TOOL_CHANGE_MODE;
        Settings.TLS_Valid = false;
        Settings.tls_position[X_AXIS] = 0;
        Settings.tls_position[Y_AXIS] = 0;
        Settings.tls_position[Z_AXIS] = 0;

        WriteGlobalSettings();
    }

    if(restore_flag & SETTINGS_RESTORE_PARAMETERS)
    {
        float coord_data[N_AXIS];

        memset(&coord_data, 0, sizeof(coord_data));

        for(int idx = 0; idx <= SETTING_INDEX_NCOORD; idx++)
        {
            Settings_WriteCoordData(idx, coord_data);
        }
    }

    if(restore_flag & SETTINGS_RESTORE_STARTUP_LINES)
    {
        for(int idx = 0; idx <= N_STARTUP_LINE; idx++)
        {
            DB_Central.Fill(0, GRBL_STARTUP_BLOCK, idx);
            DB_Central.Fill(0, GRBL_STARTUP_BLOCK_CHKSUM, idx);
        }
    }

    if(restore_flag & SETTINGS_RESTORE_BUILD_INFO)
    {
        DB_Central.Fill(0, GRBL_BUILD_INFO);
        DB_Central.Fill(0, GRBL_BUILD_INFO_CHKSUM);
    }

    if(restore_flag & SETTINGS_RESTORE_TOOLS)
    {
        TT_Reset();
    }
}


// Reads startup line from EEPROM. Updated pointed line string data.
bool Settings_ReadStartupLine(uint8_t n, char *line)
{


    if(DB_Central.Get((uint8_t*)line, GRBL_STARTUP_BLOCK, n) != SYS_READY)              // TODO check this because Actually i don't know how this happen!! AR
    {
        // Reset line with default value
        line[0] = 0; // Empty line
        Settings_StoreStartupLine(n, line);

        return false;
    }

    return true;
}


void Settings_StoreToolTable(ToolTable_t* pTable)
{
    DB_Central.SetAll(pTable, GRBL_TOOL_TABLE);
}


void Settings_StoreToolParams(uint8_t tool_nr, ToolParams_t *params)
{
    DB_Central.Set(params, GRBL_TOOL_TABLE, tool_nr);
}


bool Settings_ReadToolTable(ToolTable_t* pTable)
{
    return (DB_Central.Get(pTable, GRBL_TOOL_TABLE) == SYS_READY) ? true : false;
}


// Reads startup line from EEPROM. Updated pointed line string data.
bool Settings_ReadBuildInfo(char *line)
{
    if(DB_Central.Get((uint8_t*)line, GRBL_BUILD_INFO) != SYS_READY)
    {
        // Reset line with default value
        line[0] = 0; // Empty line
        Settings_StoreBuildInfo(line);

        return false;
    }

    return true;
}


// Read selected coordinate data from EEPROM. Updates pointed coord_data value.
uint8_t Settings_ReadCoordData(uint8_t CoordSelect, float *coord_data)
{
    if(DB_Central.Get((uint8_t*)coord_data, GRBL_GLOBAL_SETTINGS, CoordSelect) != SYS_READY)
    {
        // Reset with default zero vector
        memset(&coord_data, 0.0, sizeof(coord_data));
        DB_Central.Set((uint8_t*)coord_data, GRBL_GLOBAL_SETTINGS, CoordSelect);
        return false;
    }

    return true;
}


// Reads Grbl global settings struct from EEPROM.
uint8_t ReadGlobalSettings()
{
    // Check version-byte of eeprom
    uint8_t Version;

    DB_Central.Get(&Version, GRBL_SETTINGS_VERSION);

    if(Version == SETTINGS_VERSION)
    {
        // Read settings-record and check checksum
        if(DB_Central.Get((uint8_t*)&Settings, GRBL_GLOBAL_SETTINGS) != SYS_READY)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}


// A helper method to set settings from command line
uint8_t Settings_StoreGlobalSetting(uint8_t parameter, float value)
{
    if(value < 0.0)
    {
        return STATUS_NEGATIVE_VALUE;
    }

    if(parameter >= AXIS_SETTINGS_START_VAL)
    {
        // Store axis configuration. Axis numbering sequence set by AXIS_SETTING defines.
        // NOTE: Ensure the setting index corresponds to the report.c settings printout.
        parameter -= AXIS_SETTINGS_START_VAL;
        uint8_t set_idx = 0;

        while(set_idx < AXIS_N_SETTINGS)
        {
            if(parameter < N_AXIS)
            {
                // Valid axis setting found.
                switch (set_idx)
                {
                    case 0:
                      #ifdef MAX_STEP_RATE_HZ
                        if (value*Settings.max_rate[parameter] > (MAX_STEP_RATE_HZ*60.0))
                        {
                            return STATUS_MAX_STEP_RATE_EXCEEDED;
                        }
                      #endif
                        Settings.steps_per_mm[parameter] = value;
                        break;

                    case 1:
                      #ifdef MAX_STEP_RATE_HZ
                        if (value*Settings.steps_per_mm[parameter] > (MAX_STEP_RATE_HZ*60.0))
                        {
                            return STATUS_MAX_STEP_RATE_EXCEEDED;
                        }
                      #endif
                        Settings.max_rate[parameter] = value;
                        break;

                    case 2:
                        Settings.acceleration[parameter] = value*60*60;
                        break; // Convert to mm/min^2 for grbl internal use.
                    case 3:
                        Settings.max_travel[parameter] = -value;
                        break;  // Store as negative for grbl internal use.
                    case 4:
                        Settings.backlash[parameter] = value;
                        break;
                }
                break; // Exit while-loop after setting has been configured and proceed to the EEPROM write call.
            }
            else
            {
                set_idx++;
                // If axis index greater than N_AXIS or setting index greater than number of axis settings, error out.
                if ((parameter < AXIS_SETTINGS_INCREMENT) || (set_idx == AXIS_N_SETTINGS))
                {
                    return STATUS_INVALID_STATEMENT;
                }
                parameter -= AXIS_SETTINGS_INCREMENT;
            }
        }
    }
    else
    {
        // Store non-axis Grbl settings
        uint8_t int_value = trunc(value);

        switch(parameter)
        {
            case 0:
                //Settings.system_flags = int_value;
                break;

            case 1:
                Settings.stepper_idle_lock_time = int_value;
                break;

            case 2:
                Settings.step_invert_mask = int_value;
                Stepper_GenerateStepDirInvertMasks(); // Regenerate step and direction port invert masks.
                break;

            case 3:
                Settings.dir_invert_mask = int_value;
                Stepper_GenerateStepDirInvertMasks(); // Regenerate step and direction port invert masks.
                break;

            case 4: // Reset to ensure change. Immediate re-init may cause problems.
                if (int_value)
                {
                    Settings.flags |= BITFLAG_INVERT_ST_ENABLE;
                }
                else
                {
                    Settings.flags &= ~BITFLAG_INVERT_ST_ENABLE;
                }
                break;

            case 5: // Reset to ensure change. Immediate re-init may cause problems.
                if (int_value)
                {
                    Settings.flags |= BITFLAG_INVERT_LIMIT_PINS;
                }
                else
                {
                    Settings.flags &= ~BITFLAG_INVERT_LIMIT_PINS;
                }
                break;

            case 6: // Reset to ensure change. Immediate re-init may cause problems.
                if (int_value)
                {
                    Settings.flags |= BITFLAG_INVERT_PROBE_PIN;
                }
                else
                {
                    Settings.flags &= ~BITFLAG_INVERT_PROBE_PIN;
                }
                Probe_ConfigureInvertMask(false);
                break;

            case 10:
                Settings.status_report_mask = int_value;
                break;

            case 11:
                Settings.junction_deviation = value;
                break;

            case 12:
                Settings.arc_tolerance = value;
                break;

            case 13:
                if (int_value)
                {
                    Settings.flags |= BITFLAG_REPORT_INCHES;
                }
                else
                {
                    Settings.flags &= ~BITFLAG_REPORT_INCHES;
                }
                System_FlagWcoChange(); // Make sure WCO is immediately updated.
                break;

            case 14:
                Settings.ToolChange = int_value;
                break;   // Check for range?

            case 20:
                if (int_value)
                {
                    if (BIT_IS_FALSE(Settings.flags, BITFLAG_HOMING_ENABLE))
                    {
                        return STATUS_SOFT_LIMIT_ERROR;
                    }
                    Settings.flags |= BITFLAG_SOFT_LIMIT_ENABLE;
                }
                else
                {
                    Settings.flags &= ~BITFLAG_SOFT_LIMIT_ENABLE;
                }
                break;

            case 21:
                if (int_value)
                {
                    Settings.flags |= BITFLAG_HARD_LIMIT_ENABLE;
                }
                else
                {
                    Settings.flags &= ~BITFLAG_HARD_LIMIT_ENABLE;
                }
                Limits_Initialize(); // Re-init to immediately change. NOTE: Nice to have but could be problematic later.
                break;

            case 22:
                if (int_value)
                {
                    Settings.flags |= BITFLAG_HOMING_ENABLE;
                }
                else
                {
                    Settings.flags &= ~BITFLAG_HOMING_ENABLE;
                    Settings.flags &= ~BITFLAG_SOFT_LIMIT_ENABLE; // Force disable soft-limits.
                }
                break;

            case 23:
                Settings.homing_dir_mask = int_value;
                break;

            case 24:
                Settings.homing_feed_rate = value;
                break;

            case 25:
                Settings.homing_seek_rate = value;
                break;

            case 26:
                Settings.homing_debounce_delay = int_value;
                break;

            case 27:
                Settings.homing_pulloff = value;
                break;

            case 30:
                Settings.rpm_max = value;
                Spindle_Initialize();
                break; // Re-initialize Spindle rpm calibration

            case 31:
                Settings.rpm_min = value;
                Spindle_Initialize();
                break; // Re-initialize Spindle rpm calibration

            case 32:
                if (int_value)
                {
                    Settings.flags |= BITFLAG_LASER_MODE;
                }
                else
                {
                    Settings.flags &= ~BITFLAG_LASER_MODE;
                }
                break;

            case 33:
                if (int_value)
                {
                    Settings.flags2 |= BITFLAG_LATHE_MODE;
                }
                else
                {
                    Settings.flags2 &= ~BITFLAG_LATHE_MODE;
                }
                break;

            default:
                return STATUS_INVALID_STATEMENT;
        }
    }

    WriteGlobalSettings();

    return STATUS_OK;
}


void Settings_StoreTlsPosition(void)
{
    memcpy(Settings.tls_position, sys_position, sizeof(float) * N_AXIS);
    Settings.TLS_Valid = true;

    WriteGlobalSettings();
}


// Initialize the config subsystem
void Settings_Initialize(void)
{
    if(!ReadGlobalSettings())
    {
        Report_StatusMessage(STATUS_SETTING_READ_FAIL);
        Settings_Restore(SETTINGS_RESTORE_ALL); // Force restore all EEPROM data.
        Report_GrblSettings();
    }

    // Read tool table
    TT_Initialize();
}

