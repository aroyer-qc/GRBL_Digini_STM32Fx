/*
  util.h - Header file for shared definitions, variables, and functions
  Part of Grbl-Advanced

  Copyright (c) 2014-2016 Sungeun K. Jeon for Gnea Research LLC
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
#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <stdint.h>
#include <math.h>
#include <stdbool.h>


#define F_TIMER_STEPPER             24000000UL

#define N_AXIS                      5
#define N_LINEAR_AXIS               3

#define X_AXIS                      0 // Axis indexing value.
#define Y_AXIS                      1
#define Z_AXIS                      2
#define A_AXIS                      3
#define B_AXIS                      4

#define AXIS_MASK(A)                (1<<A)

/*
#define X_STEP_BIT                  0
#define Y_STEP_BIT                  1
#define Z_STEP_BIT                  2
#define A_STEP_BIT                  3
#define B_STEP_BIT                  4

#define X_DIRECTION_BIT             0
#define Y_DIRECTION_BIT             1
#define Z_DIRECTION_BIT             2
#define A_DIRECTION_BIT             3
#define B_DIRECTION_BIT             4
*/

#define X1_LIMIT_BIT                0
#define Y1_LIMIT_BIT                1
#define Z1_LIMIT_BIT                2
#define X2_LIMIT_BIT                3
#define Y2_LIMIT_BIT                4
#define Z2_LIMIT_BIT                5
#define LIMIT_MASK                  ((1<<X1_LIMIT_BIT) | (1<<Y1_LIMIT_BIT) | (1<<Z1_LIMIT_BIT) | (1<<X2_LIMIT_BIT) | (1<<Y2_LIMIT_BIT) | (1<<Z2_LIMIT_BIT))

#define SPINDLE_ENABLE_BIT          0
#define SPINDLE_DIRECTION_BIT       1

#define CONTROL_RESET_BIT           0
#define CONTROL_FEED_HOLD_BIT       1
#define CONTROL_CYCLE_START_BIT     2
#define CONTROL_SAFETY_DOOR_BIT     3
#define CONTROL_MASK                ((1 << CONTROL_RESET_BIT)       | \
                                     (1 << CONTROL_FEED_HOLD_BIT)   | \
                                     (1 << CONTROL_CYCLE_START_BIT) | \
                                     (1 << CONTROL_SAFETY_DOOR_BIT))


#define DELAY_MODE_DWELL            0
#define DELAY_MODE_SYS_SUSPEND      1


// CoreXY motor assignments. DO NOT ALTER.
// NOTE: If the A and B motor axis bindings are changed, this effects the CoreXY equations.
#define A_MOTOR                    X_AXIS // Must be X_AXIS
#define B_MOTOR                    Y_AXIS // Must be Y_AXIS


// Conversions
#define TICKS_PER_MICROSECOND       (24UL)   // todo AR (SYSTEM_CORE_CLOCK/1000000)??  to do change this


#define SOME_LARGE_VALUE            1.0E+38


#define ACCEL_TICKS_PER_SECOND      100


#define clear_vector(a)                 (memset(a,0,sizeof(a)))
#define clear_vector_f(a)               (memset(a, 0.0, sizeof(a)))
#define copy_vector(d,s)                (memcpy(d,s,sizeof(d)))

#define isequal_position_vector(a,b)    !(memcmp(a, b, sizeof(float)*N_AXIS))


// Delays variable-defined milliseconds.
#define delay_ms(a)                             nOS_Sleep(a)

// Delays variable-defined microseconds. Compiler compatibility fix for _delay_us().
#define delay_us(a)                             nOS_Yield()     // ARGO todo use timer
//void delay_us(uint32_t us);


// Read a floating point value from a string. Line points to the input buffer, char_counter
// is the indexer pointing to the current character of the line, while float_ptr is
// a pointer to the result variable. Returns true when it succeeds
bool Read_Float(char *line, uint8_t *char_counter, float *float_ptr);

uint8_t ExtractFloat(char *line, int start_idx, char *float_char);

void PrintFloat_CoordValue(float n);

void PrintFloat_RateValue(float n);

// Non-blocking delay function used for general operation and suspend features.
//void Delay_sec(float seconds, uint8_t mode);

// Computes hypotenuse, avoiding avr-gcc's bloated version and the extra error checking.
float hypot_f(float x, float y);
bool isEqual_f(float a, float b);
float convert_delta_vector_to_unit_vector(float *vector);
float limit_value_by_axis_maximum(float *max_value, float *unit_vec);


#endif // UTIL_H_INCLUDED
