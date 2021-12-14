//-------------------------------------------------------------------------------------------------
//
//  File : grbl_task.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2020 Alain Royer.
// Email: aroyer.qc@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
// AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "grbl.h"
#include "ff.h"

//-------------------------------------------------------------------------------------------------
// Global Macro
//-------------------------------------------------------------------------------------------------

#ifdef TASK_GRBL_GLOBAL
    #define TASK_GRBL_EXTERN
#else
    #define TASK_GRBL_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class ClassTaskGRBL
{
  public:

    void            Run                (void);
    nOS_Error       Initialize         (void);
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

TASK_GRBL_EXTERN system_t             sys;
TASK_GRBL_EXTERN int32_t              sys_position[N_AXIS];             // Real-time machine (aka home) position vector in steps.
TASK_GRBL_EXTERN int32_t              sys_probe_position[N_AXIS];       // Last probe position in machine coordinates and steps.
TASK_GRBL_EXTERN volatile uint8_t     sys_probe_state;                  // Probing state value.  Used to coordinate the probing cycle with stepper ISR.
TASK_GRBL_EXTERN volatile uint16_t    sys_rt_exec_state;                // Global realtime executor bitflag variable for state management. See EXEC bitmasks.
TASK_GRBL_EXTERN volatile uint8_t     sys_rt_exec_alarm;                // Global realtime executor bitflag variable for setting various alarms.
TASK_GRBL_EXTERN volatile uint8_t     sys_rt_exec_motion_override;      // Global realtime executor bitflag variable for motion-based overrides.
TASK_GRBL_EXTERN volatile uint8_t     sys_rt_exec_accessory_override;   // Global realtime executor bitflag variable for spindle/coolant overrides.
TASK_GRBL_EXTERN class ClassTaskGRBL  TaskGRBL;

#ifdef TASK_GRBL_GLOBAL
                 class ClassTaskGRBL* pTaskGRBL = &TaskGRBL;
#else
    extern       class ClassTaskGRBL* pTaskGRBL;
#endif

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

extern      bool GRBL_RealTimeCommand(char);

//-------------------------------------------------------------------------------------------------
