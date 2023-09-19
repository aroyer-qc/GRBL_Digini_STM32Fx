//-------------------------------------------------------------------------------------------------
//
//  File : task_grbl.cpp
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

// info http://tomsrobotics.com/product/grbl32-6-axis-cnc-controller-g6f1/#:~:text=F16%20enhanced%20GRBL32%20features%3A,30KHz%20for%20Arduino%20based%20GRBL).

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#define TASK_GRBL_GLOBAL
#include "Task_grbl.h"
#undef TASK_GRBL_GLOBAL
#include "lib_digini.h"

#if 0
// for the real time command
#include "MotionControl.h"
#include "Settings.h"

#include "Print.h"
#include "ComIf.h"
#endif

//-------------------------------------------------------------------------------------------------
//
//   Static Variables
//
//-------------------------------------------------------------------------------------------------

nOS_Thread   ClassTaskGRBL::m_Handle;
nOS_Stack    ClassTaskGRBL::m_Stack[TASK_GRBL_STACK_SIZE];

//-------------------------------------------------------------------------------------------------
//
//  Name:           TaskGRBL_Wrapper
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//  Description:    main() for the taskGRBL
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
extern "C" void TaskGRBL_Wrapper(void* pvParameters)
{
    (static_cast<ClassTaskGRBL*>(pvParameters))->Run();
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   void
//  Return:         nOS_Error
//
//  Description:    Initialize
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
nOS_Error ClassTaskGRBL::Initialize(void)
{
    nOS_Error Error = NOS_OK;

  #ifdef ENABLE_DUAL_AXIS
    IO_PinInit(IO_STEP_DUAL_AXIS);
    IO_PinInit(IO_DIR_DUAL_AXIS);
  #endif

    // ------------------------
    // Coolant IO
    IO_PinInit(IO_COOLANT_FLOOD);
    IO_PinInit(IO_COOLANT_MIST);

    // ------------------------
    // Probe IO
    IO_PinInit(IO_PROBE);

    // ------------------------
    // Spindle IO
 #ifdef VARIABLE_SPINDLE
  #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
    IO_PinInit(IO_SPINDLE_ENABLE);
  #else
   #ifndef ENABLE_DUAL_AXIS
    #if USE_SPINDLE_DIRECTION_PIN
    IO_PinInit(IO_SPINDLE_DIRECTION);
    #endif
   #endif
  #endif
 #else
    IO_PinInit(IO_SPINDLE_ENABLE);
  #ifndef ENABLE_DUAL_AXIS
   #if USE_SPINDLE_DIRECTION_PIN
    IO_PinInit(IO_SPINDLE_DIRECTION);
   #endif
  #endif
 #endif

    // ------------------------
    // User Control Input
    IO_PinInit(IO_CONTROL_RESET);
    IO_PinInit(IO_CONTROL_FEED);
    IO_PinInit(IO_CONTROL_START);
    IO_PinInit(IO_SAFETY_DOOR);

    // ------------------------
    // Stepper Control Output
    IO_PinInit(IO_STEP_X);
    IO_PinInit(IO_STEP_Y);
    IO_PinInit(IO_STEP_Z);
    IO_PinInit(IO_STEP_A);
    IO_PinInit(IO_STEP_B);
    //IO_PinInit(IO_STEP_C);
    IO_PinInit(IO_DIR_X);
    IO_PinInit(IO_DIR_Y);
    IO_PinInit(IO_DIR_Z);
    IO_PinInit(IO_DIR_A);
    IO_PinInit(IO_DIR_B);
    IO_PinInit(IO_DIR_C);
    IO_PinInit(IO_ENABLE_DRIVE);

    // ------------------------
    // Limit switch Input
    IO_PinInit(IO_LIMIT_X1);
    IO_PinInit(IO_LIMIT_Y1);
    IO_PinInit(IO_LIMIT_Z1);
    IO_PinInit(IO_LIMIT_A1);
    IO_PinInit(IO_LIMIT_B1);
    IO_PinInit(IO_LIMIT_C1);
    IO_PinInit(IO_LIMIT_X2);
    IO_PinInit(IO_LIMIT_Y2);
    IO_PinInit(IO_LIMIT_Z2);
    IO_PinInit(IO_LIMIT_A2);
    IO_PinInit(IO_LIMIT_B2);
    IO_PinInit(IO_LIMIT_C2);

    // TODO AR fix this TERM_Initialize();         // Init terminal (UART or Virtual)
    Stepper_Initialize();       // timer!!
    Settings_Initialize();
    System_ResetPosition();

    // Initialize GrIP protocol
//argo    GrIP_Initialize();

    if(BIT_IS_TRUE(Settings.flags, BITFLAG_HOMING_ENABLE))
    {
        System.State = STATE_ALARM;
    }
    else
    {
        System.State = STATE_IDLE;
    }

    // Reset system variables.
    uint16_t PriorState = System.State;
    bool     HomeState = System.IsHomed;

    System_LoadConfig();
    System_Clear();
    System.State   = PriorState;
    System.IsHomed = HomeState;

    Probe_Reset();

    sys_probe_state = 0;
    sys_rt_exec_state = 0;
    sys_rt_exec_alarm = 0;
    sys_rt_exec_motion_override = 0;
    sys_rt_exec_accessory_override = 0;

    // Reset Grbl-Advanced primary systems.
    GC_Initialize();
    Planner_Initialize();
    MC_Initialize();
    TC_Initialize();

    Coolant_Initialize();
    Limits_Initialize();
    Probe_Initialize();
    Spindle_Initialize();
    Stepper_Reset();

    // Sync cleared gcode and planner positions to current system position.
    Planner_SyncPosition();
    GC_SyncPosition();

    // Print welcome message. Indicates an initialization has occured at power-up or with a reset.
   // not sure if it is needed
    Report_InitializeMessage();

    if(IsItInitialize == false)
    {
        Error = nOS_ThreadCreate(&m_Handle,
                                 TaskGRBL_Wrapper,
                                 this,
                                 &m_Stack[0],
                                 TASK_GRBL_STACK_SIZE,
                                 TASK_GRBL_PRIO);
    }

  #if (DIGINI_USE_STACKTISTIC == DEF_ENABLED)
    myStacktistic.Register(&m_Stack[0], TASK_GRBL_STACK_SIZE, "GRBL");
  #endif

    return Error;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Run
//
//  Parameter(s):   void
//  Return:         void
//
//  Description:    main() loop of GRBL
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void ClassTaskGRBL::Run(void)
{

  #if (DIGINI_USE_GRAFX == DEF_ENABLED)
    while(SKIN_pTask->IsSkinLoaded() == false)
    {
        nOS_Sleep(100);
    };
  #endif

    for(;;)
    {
        // Grbl-Advanced initialization loop upon power-up or a system abort. For the latter, all processes
        // will return to this loop to be cleanly re-initialized.
        Initialize();

        // Start Grbl-Advanced main loop. Processes program inputs and executes them.
        //Protocol_MainLoop();

        nOS_Sleep(100);
    }
}

