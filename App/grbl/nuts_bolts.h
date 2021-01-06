/*
  nuts_bolts.h - Header file for shared definitions, variables, and functions
  Part of Grbl-Advanced

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud
  Copyright (c)	2017 Patrick F.

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

#ifndef nuts_bolts_h
#define nuts_bolts_h

#include "nOS.h"

#ifndef M_PI
	#define M_PI		                        3.14159265358979323846
#endif // M_PI

#define F_CPU						            SYSTEM_CORE_CLOCK
#define F_TIMER_STEPPER                         SYS_APB2_TIMER_CLOCK_FREQUENCY

// Bit field and masking macros
#define bit(n) 					                (1 << n)
#define bit_true(x,mask) 			            (x) |= (mask)
#define bit_false(x,mask) 			            (x) &= ~(mask)
#define bit_istrue(x,mask) 		                ((x & mask) != 0)
#define bit_isfalse(x,mask) 		            ((x & mask) == 0)

#ifndef NULL
#define NULL                                    nullptr
#endif

#define SOME_LARGE_VALUE                        1.0E+38

// Axis array index values. Must start with 0 and be continuous.
#define X_AXIS						            0 // Axis indexing value.
#define Y_AXIS						            1
#define Z_AXIS						            2
#define A_AXIS						            3
#define B_AXIS						            4
#define N_AXIS                                  5   // Number of axes
#define N_LINEAR_AXIS                           3

#define X_STEP_MASK					            1
#define Y_STEP_MASK					            2
#define Z_STEP_MASK					            4
#define A_STEP_MASK                             8
#define B_STEP_MASK                             16
#define DUAL_STEP_MASK                          32
#define STEP_MASK                               (X_STEP_MASK | Y_STEP_MASK | Z_STEP_MASK | A_STEP_MASK | B_STEP_MASK | DUAL_STEP_MASK) // All step bits

#define X_DIRECTION_MASK			            1
#define Y_DIRECTION_MASK			            2
#define Z_DIRECTION_MASK			            4
#define A_DIRECTION_MASK                        8
#define B_DIRECTION_MASK                        16
#define DUAL_DIRECTION_MASK                     32

#define X_LIMIT_MASK				            0
#define Y_LIMIT_MASK				            1
#define Z_LIMIT_MASK				            2
#define A_LIMIT_MASK				            3
#define B_LIMIT_MASK    			            4
#define LIMIT_MASK					            (X_LIMIT_MASK | Y_LIMIT_MASK | Z_LIMIT_MASK | A_LIMIT_MASK | B_LIMIT_MASK)

//#define SPINDLE_ENABLE_BIT			        0
//#define SPINDLE_DIRECTION_BIT		            1

#define CONTROL_RESET_BIT	                    0
#define CONTROL_FEED_BIT		                1
#define CONTROL_CYCLE_START_BIT	                2
#define CONTROL_SAFETY_DOOR_BIT		            3
#define CONTROL_MASK				            ((1 << CONTROL_RESET_BIT)       | \
                                                 (1 << CONTROL_FEED_BIT)        | \
                                                 (1 << CONTROL_CYCLE_START_BIT) | \
                                                 (1 << CONTROL_SAFETY_DOOR_BIT))


#define ACCEL_TICKS_PER_SECOND 		            100



// CoreXY motor assignments. DO NOT ALTER.
// NOTE: If the A and B motor axis bindings are changed, this effects the CoreXY equations.
#ifdef COREXY
 #define A_MOTOR					            X_AXIS // Must be X_AXIS
 #define B_MOTOR					            Y_AXIS // Must be Y_AXIS
#endif

// Conversions
#define MM_PER_INCH                             (25.40)
#define INCH_PER_MM                             (0.0393701)
#define TICKS_PER_MICROSECOND                   (F_CPU/1000000)

#define DELAY_MODE_DWELL       		            0
#define DELAY_MODE_SYS_SUSPEND 		            1
#define PROBE_MASK                              1

// Useful macros
#define clear_vector(a) 			            memset(a,0,sizeof(a))
#define clear_vector_float(a)                   memset(a, 0.0, sizeof(float)*N_AXIS)

// #define clear_vector_long(a) memset(a, 0.0, sizeof(long)*N_AXIS)
#define max(a,b)                                (((a) > (b)) ? (a) : (b))
#define min(a,b)                                (((a) < (b)) ? (a) : (b))
#define isequal_position_vector(a,b)            !(memcmp(a, b, sizeof(float)*N_AXIS))

// Bit field and masking macros
#define bit(n)                                  (1 << n)
#define bit_true(x,mask)                        (x) |= (mask)
#define bit_false(x,mask)                       (x) &= ~(mask)
#define bit_istrue(x,mask)                      ((x & mask) != 0)
#define bit_isfalse(x,mask)                     ((x & mask) == 0)

// Read a floating point value from a string. Line points to the input buffer, char_counter
// is the indexer pointing to the current character of the line, while float_ptr is
// a pointer to the result variable. Returns true when it succeeds
uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr);

// Non-blocking delay function used for general operation and suspend features.
void delay_sec(float seconds, uint8_t mode);

// Delays variable-defined milliseconds.
#define delay_ms(a)                             nOS_Sleep(a)

// Delays variable-defined microseconds. Compiler compatibility fix for _delay_us().
#define delay_us(a)                             nOS_Yield()     // ARGO todo use timer
//void delay_us(uint32_t us);

// Computes hypotenuse, avoiding avr-gcc's bloated version and the extra error checking.
float hypot_f(float x, float y);

float convert_delta_vector_to_unit_vector(float *vector);
float limit_value_by_axis_maximum(float *max_value, float *unit_vec);

#endif



