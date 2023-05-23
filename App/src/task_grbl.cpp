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
    nOS_Error Error;

    // ------------------------
    // Stepper drive IO
    IO_PinInit(IO_ENABLE_DRIVE);
    IO_PinInit(IO_STEP_X);
    IO_PinInit(IO_DIR_X);

  #ifdef ENABLE_DUAL_AXIS
    IO_PinInit(IO_STEP_DUAL_AXIS);
    IO_PinInit(IO_DIR_DUAL_AXIS);
  #endif

  #ifdef GRBL_USE_AXIS_Y
    IO_PinInit(IO_STEP_Y);
    IO_PinInit(IO_DIR_Y);
  #endif
  #ifdef GRBL_USE_AXIS_Z
    IO_PinInit(IO_STEP_Z);
    IO_PinInit(IO_DIR_Z);
  #endif
  #ifdef GRBL_USE_AXIS_A
    IO_PinInit(IO_STEP_A);
    IO_PinInit(IO_DIR_A);
  #endif
  #ifdef GRBL_USE_AXIS_B
    IO_PinInit(IO_STEP_B);
    IO_PinInit(IO_DIR_B);
  #endif

    // ------------------------
    // Limit switch IO
    IO_PinInit(IO_LIMIT_X);
  #ifdef GRBL_USE_AXIS_Y
    IO_PinInit(IO_LIMIT_Y);
  #endif
  #ifdef GRBL_USE_AXIS_Z
    IO_PinInit(IO_LIMIT_Z);
  #endif

  #ifdef GRBL_USE_AXIS_A
    IO_PinInit(IO_LIMIT_A);
  #endif

  #ifdef GRBL_USE_AXIS_B
    IO_PinInit(IO_LIMIT_B);
  #endif

    // ------------------------
    // Coolant IO
  #ifdef GRBL_USE_COOLANT_FLOOD
    IO_PinInit(IO_COOLANT_FLOOD);
  #endif
  #ifdef ENABLE_M7
    IO_PinInit(IO_COOLANT_MIST);
  #endif

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

    Error = nOS_ThreadCreate(&this->m_Handle,
                             TaskGRBL_Wrapper,
                             this,
                             &this->m_Stack[0],
                             TASK_GRBL_STACK_SIZE,
                             TASK_GRBL_PRIO);

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
    this->Initialize();         // Needed if task is not at the idle level, otherwise remove it

    while(SKIN_pTask->IsSkinLoaded() == false)
    {
        nOS_Sleep(100);
    };

    // --------------------------------------------------------------------------------------------

    // TODO AR fix this ERM_Initialize();         // Init terminal (UART or Virtual)
    System_Initialize();
    Stepper_Initialize();
    Settings_Initialize();
    System_ResetPosition();

    // Initialize GrIP protocol
//argo    GrIP_Initialize();

    if(BIT_IS_TRUE(Settings.flags, BITFLAG_HOMING_ENABLE))
    {
        System.state = STATE_ALARM;
    }
    else
    {
        System.state = STATE_IDLE;
    }

    // Grbl-Advanced initialization loop upon power-up or a system abort. For the latter, all processes
    // will return to this loop to be cleanly re-initialized.

    for(;;)
    {
        // Reset system variables.
        uint16_t prior_state = System.state;
        uint8_t home_state = System.is_homed;

        System_Clear();
        System.state = prior_state;
        System.is_homed = home_state;

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
        Report_InitializeMessage();

        //-- Start Grbl-Advanced main loop. Processes program inputs and executes them. --//
        Protocol_MainLoop();
        //--------------------------------------------------------------------------------//

        // Clear serial buffer after soft reset to prevent undefined behavior
        //  FifoUsart_Initialize(); it's DMA... so not needed!! no buffer allocated
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           GRBL_RealTimeCommand
//
//  Parameter(s):   RealTimeCommand             Character to be check
//  Return:         IsItProcessed               - true      If character was a real time command
//                                              - false     If not a valid real time command
//
//  Description:    Interpreter for GRBL real time command
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
bool GRBL_RealTimeCommand(char RealTimeCommand)
{
    bool IsItRealTime = true;

    switch(RealTimeCommand)
    {
        case CMD_RESET:         MC_Reset();                                     break; // Call motion control reset routine.
        case CMD_STATUS_REPORT: System_SetExecStateFlag(EXEC_STATUS_REPORT); break; // Set as true
        case CMD_CYCLE_START:   System_SetExecStateFlag(EXEC_CYCLE_START);   break; // Set as true
        case CMD_FEED_HOLD:     System_SetExecStateFlag(EXEC_FEED_HOLD);     break; // Set as true

        // Real-time control characters extended ASCII only.
        case CMD_SAFETY_DOOR:   System_SetExecStateFlag(EXEC_SAFETY_DOOR);   break; // Set as true
        case CMD_JOG_CANCEL:
        {
            if(System.state & STATE_JOG)    // Block all other states from invoking motion cancel.
            {
                System_SetExecStateFlag(EXEC_MOTION_CANCEL);
            }
        }
        break;

        case CMD_FEED_OVR_RESET:            System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_RESET);              break;
        case CMD_FEED_OVR_COARSE_PLUS:      System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_COARSE_PLUS);        break;
        case CMD_FEED_OVR_COARSE_MINUS:     System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_COARSE_MINUS);       break;
        case CMD_FEED_OVR_FINE_PLUS:        System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_FINE_PLUS);          break;
        case CMD_FEED_OVR_FINE_MINUS:       System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_FINE_MINUS);         break;
        case CMD_RAPID_OVR_RESET:           System_SetExecMotionOverrideFlag(EXEC_RAPID_OVR_RESET);             break;
        case CMD_RAPID_OVR_MEDIUM:          System_SetExecMotionOverrideFlag(EXEC_RAPID_OVR_MEDIUM);            break;
        case CMD_RAPID_OVR_LOW:             System_SetExecMotionOverrideFlag(EXEC_RAPID_OVR_LOW);               break;
        case CMD_SPINDLE_OVR_RESET:         System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_RESET);        break;
        case CMD_SPINDLE_OVR_COARSE_PLUS:   System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_COARSE_PLUS);  break;
        case CMD_SPINDLE_OVR_COARSE_MINUS:  System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_COARSE_MINUS); break;
        case CMD_SPINDLE_OVR_FINE_PLUS:     System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_FINE_PLUS);    break;
        case CMD_SPINDLE_OVR_FINE_MINUS:    System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_FINE_MINUS);   break;
        case CMD_SPINDLE_OVR_STOP:          System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_STOP);         break;
        case CMD_COOLANT_FLOOD_OVR_TOGGLE:  System_SetExecAccessoryOverrideFlag(EXEC_COOLANT_FLOOD_OVR_TOGGLE); break;
      #ifdef ENABLE_M7
        case CMD_COOLANT_MIST_OVR_TOGGLE:   System_SetExecAccessoryOverrideFlag(EXEC_COOLANT_MIST_OVR_TOGGLE);  break;
      #endif

        default:
        {
            if(RealTimeCommand < 0x80)
            {
                // Throw away any unfound extended-ASCII character by not passing it to the serial buffer.
                // keeping only character that are not extended
                IsItRealTime = false;
            }
        }
        break;
    }

    return IsItRealTime;
}
