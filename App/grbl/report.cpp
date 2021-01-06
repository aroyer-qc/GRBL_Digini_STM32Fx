/*
  Report.c - reporting and messaging methods
  Part of Grbl-Advanced

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c)	2017-2019 Patrick F.

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

/*
  This file functions as the primary feedback interface for Grbl. Any outgoing data, such
  as the protocol status messages, feedback messages, and status reports, are stored here.
  For the most part, these functions primarily are called from protocol.c methods. If a
  different style feedback is desired (i.e. JSON), then a user can change these following
  methods to accomodate their needs.
*/

#include "grbl.h"



static void report_util_axis_values(float *axis_value) {
	uint8_t idx;

	for(idx = 0; idx < N_LINEAR_AXIS; idx++) {
		printFloat_CoordValue(axis_value[idx]);

		if(idx < (N_LINEAR_AXIS-1)) { Putc(',');	}
	}
}


// Handles the primary confirmation protocol response for streaming interfaces and human-feedback.
// For every incoming line, this method responds with an 'ok' for a successful command or an
// 'error:'  to indicate some error event with the line or some critical system error during
// operation. Errors events can originate from the g-code parser, settings module, or asynchronously
// from a critical error, such as a triggered hard limit. Interface should always monitor for these
// responses.
void report_status_message(uint8_t status_code)
{
	switch(status_code) {
	case STATUS_OK: // STATUS_OK
		printf("ok\r\n"); break;

	default:
		printf("error:%d\r\n", status_code);
	}
}

// Prints alarm messages.
void report_alarm_message(uint8_t alarm_code)
{
	printf("ALARM:%d\r\n", alarm_code);
}

// Prints feedback messages. This serves as a centralized method to provide additional
// user feedback for things that are not of the status/alarm message protocol. These are
// messages such as setup warnings, switch toggling, and how to exit alarms.
// NOTE: For interfaces, messages are always placed within brackets. And if silent mode
// is installed, the message number codes are less than zero.
void report_feedback_message(uint8_t message_code)
{
	printf("[MSG:");

	switch(message_code)
	{
	  case MESSAGE_CRITICAL_EVENT:
	    printf("Reset to continue"); break;
	  case MESSAGE_ALARM_LOCK:
	    printf("'$H'|'$X' to unlock"); break;
	  case MESSAGE_ALARM_UNLOCK:
	    printf("Caution: Unlocked"); break;
	  case MESSAGE_ENABLED:
	    printf("Enabled"); break;
	  case MESSAGE_DISABLED:
	    printf("Disabled"); break;
	  case MESSAGE_SAFETY_DOOR_AJAR:
	    printf("Check Door"); break;
	  case MESSAGE_CHECK_LIMITS:
	    printf("Check Limits"); break;
	  case MESSAGE_PROGRAM_END:
	    printf("Pgm End"); break;
	  case MESSAGE_RESTORE_DEFAULTS:
	    printf("Restoring defaults"); break;
	  case MESSAGE_SPINDLE_RESTORE:
	    printf("Restoring spindle"); break;
	  case MESSAGE_SLEEP_MODE:
	    printf("Sleeping"); break;
	}

    printf("]\r\n");
}


// Welcome message
void report_init_message()
{
	printf("\r\nGRBL-Advanced %s ['$' for help]\r\n", GRBL_VERSION);
}

// Grbl help message
void report_grbl_help() {
  printf("[HLP:$$ $# $G $I $N $x=val $Nx=line $J=line $SLP $C $X $H ~ ! ? ctrl-x]\r\n");
}


// Grbl global settings print out.
// NOTE: The numbering scheme here must correlate to storing in settings.c
void report_grbl_settings() {
	// Print Grbl settings.
	printf("$0=%d\r\n", settings.system_flags);
	printf("$1=%d\r\n", settings.stepper_idle_lock_time);
	printf("$2=%d\r\n", settings.step_invert_mask);
	printf("$3=%d\r\n", settings.dir_invert_mask);
	printf("$4=%d\r\n", bit_istrue(settings.flags, BITFLAG_INVERT_ST_ENABLE));
	printf("$5=%d\r\n", bit_istrue(settings.flags, BITFLAG_INVERT_LIMIT_PINS));
	printf("$6=%d\r\n", bit_istrue(settings.flags, BITFLAG_INVERT_PROBE_PIN));
	printf("$10=%d\r\n", settings.status_report_mask);
	printf("$11=%." N_DECIMAL_SETTING_STR "f\r\n", settings.junction_deviation);
    printf("$12=%." N_DECIMAL_SETTING_STR "f\r\n", settings.arc_tolerance);
	printf("$13=%d\r\n", bit_istrue(settings.flags, BITFLAG_REPORT_INCHES));
	printf("$14=%d\r\n", settings.tool_change);
	printf("$20=%d\r\n", bit_istrue(settings.flags, BITFLAG_SOFT_LIMIT_ENABLE));
	printf("$21=%d\r\n", bit_istrue(settings.flags, BITFLAG_HARD_LIMIT_ENABLE));
	printf("$22=%d\r\n", bit_istrue(settings.flags, BITFLAG_HOMING_ENABLE));
	printf("$23=%d\r\n", settings.homing_dir_mask);
    printf("$24=%." N_DECIMAL_SETTING_STR "f\r\n", settings.homing_feed_rate);
    printf("$25=%." N_DECIMAL_SETTING_STR "f\r\n", settings.homing_seek_rate);
	printf("$26=%d\r\n", settings.homing_debounce_delay);
    printf("$27=%." N_DECIMAL_SETTING_STR "f\r\n", settings.homing_pulloff);
    printf("$30=%." N_DECIMAL_RPM_STR "f\r\n", settings.rpm_max);
    printf("$31=%." N_DECIMAL_RPM_STR "f\r\n", settings.rpm_min);
	printf("$32=%d\r\n", bit_istrue(settings.flags,BITFLAG_LASER_MODE));

	// Print axis settings
	uint8_t idx, set_idx;
	uint8_t val = AXIS_SETTINGS_START_VAL;
	for(set_idx = 0; set_idx < AXIS_N_SETTINGS; set_idx++) {
		for(idx = 0; idx < N_AXIS; idx++) {
			switch(set_idx)  {
			case 0: printf("$%d=%." N_DECIMAL_SETTING_STR "f\r\n", val+idx, settings.steps_per_mm[idx]); break;
			case 1: printf("$%d=%." N_DECIMAL_SETTING_STR "f\r\n", val+idx, settings.max_rate[idx]); break;
			case 2: printf("$%d=%." N_DECIMAL_SETTING_STR "f\r\n", val+idx, settings.acceleration[idx] / (60 * 60)); break;
			case 3: printf("$%d=%." N_DECIMAL_SETTING_STR "f\r\n", val+idx, -settings.max_travel[idx]); break;
            case 4: printf("$%d=%." N_DECIMAL_SETTING_STR "f\r\n", val+idx, settings.backlash[idx]);  break;

			default:
				break;
			}
		}
		val += AXIS_SETTINGS_INCREMENT;
	}
}


// Prints current probe parameters. Upon a probe command, these parameters are updated upon a
// successful probe or upon a failed probe with the G38.3 without errors command (if supported).
// These values are retained until Grbl is power-cycled, whereby they will be re-zeroed.
void report_probe_parameters()
{
  // Report in terms of machine position.
	float print_position[N_AXIS];

	// Report in terms of machine position.
	printf("[PRB:");
	system_convert_array_steps_to_mpos(print_position, sys_probe_position);
	report_util_axis_values(print_position);
	printf(":%d]\r\n", sys.probe_succeeded);
}


void report_TLS_params()
{
    float print_position[N_AXIS];
    uint8_t idx = 0;

    // Report in terms of machine position.
	printf("[TLS:");
	system_convert_array_steps_to_mpos(print_position, settings.tls_position);

	for(idx = 0; idx < 3; idx++) {
		printFloat_CoordValue(print_position[idx]);

		if(idx < (3-1)) {
			Putc(',');
		}
	}

	printf(":%d]\r\n", settings.tls_valid);
}

// Prints Grbl NGC parameters (coordinate offsets, probing)
void report_ngc_parameters()
{
	float coord_data[N_AXIS];
	uint8_t coord_select;

	for(coord_select = 0; coord_select <= SETTING_INDEX_NCOORD; coord_select++) {
		if(!(settings_read_coord_data(coord_select,coord_data))) {
			report_status_message(STATUS_SETTING_READ_FAIL);
			return;
		}

		printf("[G");
		switch(coord_select)
		{
		case 6:	printf("28"); break;
		case 7:	printf("30"); break;

		default: printf("%d", coord_select+54);	break; // G54-G59
		}
		Putc(':');
		report_util_axis_values(coord_data);
		printf("]\r\n");
	}
	printf("[G92:");        // Print G92,G92.1 which are not persistent in memory
	report_util_axis_values(gc_state.coord_offset);
    printf("]\r\n");
	printf("[TLO:");        // Print tool length offset value
	printFloat_CoordValue(gc_state.tool_length_offset);
	printf("]\r\n");
	report_probe_parameters();   // Print probe parameters. Not persistent in memory.
	report_TLS_params();     // Print tls position. Persistent in memory.
}


// Print current gcode parser mode state
void report_gcode_modes()
{
	printf("[GC:G");

	if(gc_state.modal.motion >= MOTION_MODE_PROBE_TOWARD) {
		printf("38.%d", gc_state.modal.motion - (MOTION_MODE_PROBE_TOWARD-2));
	}
	else {
		printf("%d", gc_state.modal.motion);
	}

	printf(" G%d", gc_state.modal.coord_select+54);
	printf(" G%d", gc_state.modal.plane_select+17);
	printf(" G%d", 21-gc_state.modal.units);
	printf(" G%d", gc_state.modal.distance+90);
	printf(" G%d", 94-gc_state.modal.feed_rate);
	printf(" G%d", 98+gc_state.modal.retract);

	if(gc_state.modal.program_flow) {
		printf(" M");

		switch(gc_state.modal.program_flow)	{
		  case PROGRAM_FLOW_PAUSED: Putc('0');	break;
		// case PROGRAM_FLOW_OPTIONAL_STOP : Putc('1'); break; // M1 is ignored and not supported.
		case PROGRAM_FLOW_COMPLETED_M2:
		case PROGRAM_FLOW_COMPLETED_M30:
			printf("%d", gc_state.modal.program_flow);
			break;

		default:
			break;
		}
	}

	printf(" M");

	switch(gc_state.modal.spindle)
	{
	case SPINDLE_ENABLE_CW:	Putc('3'); break;
	case SPINDLE_ENABLE_CCW: Putc('4');	break;
	case SPINDLE_DISABLE: Putc('5'); break;
	}

#ifdef ENABLE_M7
	if(gc_state.modal.coolant) { // Note: Multiple coolant states may be active at the same time.
		if (gc_state.modal.coolant & PL_COND_FLAG_COOLANT_MIST) { printf(" M7"); }
		if (gc_state.modal.coolant & PL_COND_FLAG_COOLANT_FLOOD) { printf(" M8"); }
	} else { printf(" M9");	}
#else
	if(gc_state.modal.coolant) { printf(" M8"); }
	else { printf(" M9"); }
#endif

#ifdef ENABLE_PARKING_OVERRIDE_CONTROL
	if(sys.override_ctrl == OVERRIDE_PARKING_MOTION) {
		printf(" M%d", 56);
	}
#endif

	printf(" T%d F", gc_state.tool);
	printFloat_RateValue(gc_state.feed_rate);
	printf(" S");
	printFloat(gc_state.spindle_speed, 0);
	printf("]\r\n");
}

// Prints specified startup line
void report_startup_line(uint8_t n, char *line)
{
	printf("$N%d=%s]\r\n", n, line);
}

void report_execute_startup_message(char *line, uint8_t status_code)
{
	printf(">%s:", line);
	report_status_message(status_code);
}

// Prints build info line
void report_build_info(char *line)
{
	printf("[VER: %s %s:%s]\r\nOPT:V", GRBL_VERSION, GRBL_VERSION_BUILD, line);
  #ifdef VARIABLE_SPINDLE
    Putc('V');
  #endif
  #ifdef USE_LINE_NUMBERS
	Putc('N');
  #endif
  #ifdef ENABLE_M7
	Putc('M');
  #endif
  #ifdef COREXY
	Putc('C');
  #endif
  #ifdef PARKING_ENABLE
	Putc('P');
  #endif
  #ifdef HOMING_FORCE_SET_ORIGIN
	Putc('Z');
  #endif
  #ifdef HOMING_SINGLE_AXIS_COMMANDS
	Putc('H');
  #endif
  #ifdef LIMITS_TWO_SWITCHES_ON_AXES
	Putc('T');
  #endif
  #ifdef ALLOW_FEED_OVERRIDE_DURING_PROBE_CYCLES
	Putc('A');
  #endif
  #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
    Putc('D');
  #endif
  #ifdef SPINDLE_ENABLE_OFF_WITH_ZERO_SPEED
	Putc('0');
  #endif
  #ifdef ENABLE_SOFTWARE_DEBOUNCE
	Putc('S');
  #endif
  #ifdef ENABLE_PARKING_OVERRIDE_CONTROL
	Putc('R');
  #endif
  #ifndef HOMING_INIT_LOCK
    Putc('L');
  #endif
  #ifdef ENABLE_SAFETY_DOOR_INPUT_PIN
    Putc('+');
  #endif
  #ifndef ENABLE_RESTORE_EEPROM_WIPE_ALL // NOTE: Shown when disabled.
	Putc('*');
  #endif
  #ifndef ENABLE_RESTORE_EEPROM_DEFAULT_SETTINGS // NOTE: Shown when disabled.
	Putc('$');
  #endif
  #ifndef ENABLE_RESTORE_EEPROM_CLEAR_PARAMETERS // NOTE: Shown when disabled.
	Putc('#');
  #endif
  #ifndef ENABLE_BUILD_INFO_WRITE_COMMAND // NOTE: Shown when disabled.
	Putc('I');
  #endif
  #ifndef FORCE_BUFFER_SYNC_DURING_EEPROM_WRITE // NOTE: Shown when disabled.
	Putc('E');
  #endif
  #ifndef FORCE_BUFFER_SYNC_DURING_WCO_CHANGE // NOTE: Shown when disabled.
	Putc('W');
  #endif
  #ifdef ENABLE_DUAL_AXIS
    Putc('2');
  #endif
  #ifdef USE_MULTI_AXIS
    Putc('X');
  #endif
	// NOTE: Compiled values, like override increments/max/min values, may be added at some point later.
	printf(",%d,%d]\r\n", BLOCK_BUFFER_SIZE - 1, LINE_BUFFER_SIZE);
}


// Prints the character string line Grbl has received from the user, which has been pre-parsed,
// and has been sent into protocol_execute_line() routine to be executed by Grbl.
void report_echo_line_received(char *line)
{
	printf("[echo: %s]\r\n", line);
}


 // Prints real-time data. This function grabs a real-time snapshot of the stepper subprogram
 // and the actual location of the CNC machine. Users may change the following function to their
 // specific needs, but the desired real-time data report must be as short as possible. This is
 // requires as it minimizes the computational overhead and allows grbl to keep running smoothly,
 // especially during g-code programs with fast, short line segments and high frequency reports (5-20Hz).
void report_realtime_status()
{
	uint8_t idx;
	int32_t current_position[N_AXIS]; // Copy current state of the system position variable
	float print_position[N_AXIS];

	memcpy(current_position,sys_position,sizeof(sys_position));
	system_convert_array_steps_to_mpos(print_position, current_position);

	// Report current machine state and sub-states
	Putc('<');
	switch(sys.state) {
	case STATE_IDLE: printf("Idle"); break;
	case STATE_CYCLE: printf("Run"); break;
	case STATE_HOLD:
		if(!(sys.suspend & SUSPEND_JOG_CANCEL)) {
			printf("Hold:");
			if(sys.suspend & SUSPEND_HOLD_COMPLETE) { Putc('0'); } // Ready to resume
    		else { Putc('1'); } // Actively holding
			break;
		} // Continues to print jog state during jog cancel.
	case STATE_JOG:         printf("Jog"); break;
	case STATE_HOMING:      printf("Home"); break;
	case STATE_ALARM:       printf("Alarm"); break;
	case STATE_CHECK_MODE:  printf("Check"); break;
	case STATE_SAFETY_DOOR:
		printf("Door:");
		if (sys.suspend & SUSPEND_INITIATE_RESTORE) {
			Putc('3'); // Restoring
		} else {
			if(sys.suspend & SUSPEND_RETRACT_COMPLETE) {
				if(sys.suspend & SUSPEND_SAFETY_DOOR_AJAR) {
					Putc('1'); // Door ajar
				} else {
					Putc('0');
				} // Door closed and ready to resume
			} else {
				Putc('2'); // Retracting
			}
		}
		break;

	case STATE_SLEEP:
		printf("Sleep");
		break;

    case STATE_FEED_DWELL:
        printf("Dwell");
        break;

    case STATE_TOOL_CHANGE:
        printf("Tool");
        break;

	default:
		break;
	}

	float wco[N_AXIS];
	if(bit_isfalse(settings.status_report_mask,BITFLAG_RT_STATUS_POSITION_TYPE) ||
	   (sys.report_wco_counter == 0) ) {
		for (idx = 0; idx <  N_AXIS; idx++) {
			// Apply work coordinate offsets and tool length offset to current position.
			wco[idx] = gc_state.coord_system[idx]+gc_state.coord_offset[idx];
			if(idx == TOOL_LENGTH_OFFSET_AXIS) { wco[idx] += gc_state.tool_length_offset; }
			if(bit_isfalse(settings.status_report_mask, BITFLAG_RT_STATUS_POSITION_TYPE)) {
				print_position[idx] -= wco[idx];
			}
		}
	}

	// Report machine position
	if(bit_istrue(settings.status_report_mask, BITFLAG_RT_STATUS_POSITION_TYPE)) {
		printf("|MPos:");
	} else {
		printf("|WPos:");
	}
	report_util_axis_values(print_position);

	// Returns planner and serial read buffer states.
#ifdef REPORT_FIELD_BUFFER_STATE
	if(bit_istrue(settings.status_report_mask, BITFLAG_RT_STATUS_BUFFER_STATE)) {
		printf("|Bf:%d,", plan_get_block_buffer_available());
		//Printf("%d", FifoUsart_Available(STDOUT_NUM));   argo use my stuff   this inform system of available buffer
	}
#endif

  #ifdef USE_LINE_NUMBERS
   #ifdef REPORT_FIELD_LINE_NUMBERS
	// Report current line number
	plan_block_t * cur_block = plan_get_current_block();
	if(cur_block != NULL) {
		uint32_t ln = cur_block->line_number;

		if(ln > 0) {
			printf("|Ln:%ld", ln);
		}
	}
   #endif
  #endif

	// Report realtime feed speed
#ifdef REPORT_FIELD_CURRENT_FEED_SPEED
  #ifdef VARIABLE_SPINDLE
	printf("|FS:");
	printFloat_RateValue(st_get_realtime_rate());
	Putc(',');
	printFloat(sys.spindle_speed, N_DECIMAL_RPM_VALUE);
  #else
    printf("|F:");
    printFloat_RateValue(st_get_realtime_rate());
  #endif
#endif

#ifdef REPORT_FIELD_PIN_STATE
	uint8_t lim_pin_state = limits_get_state();
	uint8_t ctrl_pin_state = system_control_get_state();
	uint8_t prb_pin_state = probe_get_state();
	if(lim_pin_state | ctrl_pin_state | prb_pin_state) {
		printf("|Pn:");
		if (prb_pin_state) { Putc('P'); }
		if (lim_pin_state) {
	     #ifdef ENABLE_DUAL_AXIS
          #if (DUAL_AXIS_SELECT == X_AXIS)
            if (bit_istrue(lim_pin_state,(bit(X_AXIS)|bit(N_AXIS)))) { Putc('X'); }
            if (bit_istrue(lim_pin_state,bit(Y_AXIS))) { Putc('Y'); }
          #endif
          #if (DUAL_AXIS_SELECT == Y_AXIS)
            if (bit_istrue(lim_pin_state,bit(X_AXIS))) { Putc('X'); }
            if (bit_istrue(lim_pin_state,(bit(Y_AXIS)|bit(N_AXIS)))) { Putc('Y'); }
          #endif
          if (bit_istrue(lim_pin_state,bit(Z_AXIS))) { Putc('Z'); }
         #else
			if (bit_istrue(lim_pin_state, bit(X_AXIS))) { Putc('X'); }
			if (bit_istrue(lim_pin_state, bit(Y_AXIS))) { Putc('Y'); }
			if (bit_istrue(lim_pin_state, bit(Z_AXIS))) { Putc('Z'); }
			if (bit_istrue(lim_pin_state, bit(A_AXIS))) { Putc('A'); }
			if (bit_istrue(lim_pin_state, bit(B_AXIS))) { Putc('B'); }
         #endif
		}
		if(ctrl_pin_state) {
            #ifdef ENABLE_SAFETY_DOOR_INPUT_PIN
			  if (bit_istrue(ctrl_pin_state, CONTROL_PIN_INDEX_SAFETY_DOOR)) { Putc('D'); }
            #endif
			if (bit_istrue(ctrl_pin_state, CONTROL_PIN_INDEX_RESET)) { Putc('R'); }
			if (bit_istrue(ctrl_pin_state, CONTROL_PIN_INDEX_FEED_HOLD)) { Putc('H'); }
			if (bit_istrue(ctrl_pin_state, CONTROL_PIN_INDEX_CYCLE_START)) { Putc('S'); }
		}
	}
#endif

#ifdef REPORT_FIELD_WORK_COORD_OFFSET
	if(sys.report_wco_counter > 0) { sys.report_wco_counter--; 	}
	else {
		if(sys.state & (STATE_HOMING | STATE_CYCLE | STATE_HOLD | STATE_JOG | STATE_SAFETY_DOOR)) {
			sys.report_wco_counter = (REPORT_WCO_REFRESH_BUSY_COUNT-1); // Reset counter for slow refresh
		} else { sys.report_wco_counter = (REPORT_WCO_REFRESH_IDLE_COUNT-1); }
		if(sys.report_ovr_counter == 0) { sys.report_ovr_counter = 1; } // Set override on next report.
		printf("|WCO:");
		report_util_axis_values(wco);
	}
#endif

#ifdef REPORT_FIELD_OVERRIDES
	if(sys.report_ovr_counter > 0) { sys.report_ovr_counter--; }
	else {
		if(sys.state & (STATE_HOMING | STATE_CYCLE | STATE_HOLD | STATE_JOG | STATE_SAFETY_DOOR)) {
			sys.report_ovr_counter = (REPORT_OVR_REFRESH_BUSY_COUNT-1); // Reset counter for slow refresh
		} else { sys.report_ovr_counter = (REPORT_OVR_REFRESH_IDLE_COUNT-1); }

		printf("|Ov:%d,%d,%d", sys.f_override, sys.r_override, sys.spindle_speed_ovr);

		uint8_t sp_state = spindle_get_state();
		uint8_t cl_state = coolant_get_state();

		if(sp_state || cl_state) {
			printf("|A:");
			if(sp_state) { // != SPINDLE_STATE_DISABLE
              #ifdef VARIABLE_SPINDLE
               #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
			    Putc('S'); // CW
               #else
				if(sp_state == SPINDLE_STATE_CW) { Putc('S'); } // CW
    			else { Putc('C'); } // CCW
		       #endif
              #else
				if(sp_state & SPINDLE_STATE_CW) { Putc('S'); } // CW
    			else { Putc('C'); } // CCW
              #endif
			}
			if(cl_state & COOLANT_STATE_FLOOD) { Putc('F');	}
            #ifdef ENABLE_M7
			  if(cl_state & COOLANT_STATE_MIST) { Putc('M'); }
            #endif
		}
	}
#endif

	printf(">\r\n");
}


#ifdef DEBUG
  void report_realtime_debug()
  {

  }
#endif
