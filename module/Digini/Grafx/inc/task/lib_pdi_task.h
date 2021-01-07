//-------------------------------------------------------------------------------------------------
//
//  File : lib_pdi_task.h
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

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#ifdef DIGINI_USE_POINTING_DEVICE
#include "lib_grafx.h"
#include "nOS.h"

//-------------------------------------------------------------------------------------------------
// Global Define(s)
//-------------------------------------------------------------------------------------------------

#ifdef LIB_PDI_TASK_GLOBAL
  #define LIB_PDI_TASK_EXTERN
#else
  #define LIB_PDI_TASK_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------
#define TOUCH_LOOP_DELAY                    10 //BSP_TICK_PER_SECOND / 100
#define TOUCH_PRESS_COUNT                   1
#define TOUCH_SUPERKEY_COUNT                30
#define TOUCH_TYPEMATIC_PRECOUNT            30
#define TOUCH_TYPEMATIC_COUNT               2

#define PDI_TASK_STACK_SIZE                 1024
#define PDI_TASK_PRIO                       4

#define PDI_SWAP_NONE                       ((uint8_t) 0x01)
#define PDI_SWAP_X                          ((uint8_t) 0x02)
#define PDI_SWAP_Y                          ((uint8_t) 0x04)
#define PDI_SWAP_XY                         ((uint8_t) 0x08)

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class PDI_myClassTask
{
    public:

        void                Run                 (void);
        SystemState_e       Initialize          (PointingDeviceInterface* pDriver, uint16_t SizeX, uint16_t SizeY, uint8_t Orientation);
        SystemState_e       GetState            (void);

      #ifdef DIGINI_USE_PDI_MULTI_EVENT
        ServiceEvent_e      GetGestureID        (void);
      #endif

        void                ClearAllZone        (void);
        void                CreateZone          (EventArea_t* pEventArea, uint16_t Options, Widget_e Widget);


      #ifdef GRAFX_PDI_INTERRUPT_IO
        void                IRQ_Handler         (void);
        SystemState_e       EnableIT            (void);
        SystemState_e       DisableIT           (void);
        SystemState_e       GetIT_Status        (void);
        SystemState_e       ClearIT             (void);
      #endif

    private:

        void                ResetEventData      (void);
        Widget_e            GetZoneID           (void);


        nOS_Thread                          m_Handle;
      #ifdef GRAFX_PDI_INTERRUPT_IO
        nOS_Sem                             m_FlagTouchDetected;
      #endif
        PointingDeviceInterface*            m_pDriver;
        nOS_Stack                           m_Stack[PDI_TASK_STACK_SIZE];
        Cartesian_t                         m_EventPosition;
        uint16_t                            m_ConfigSizeX;
        uint16_t                            m_ConfigSizeY;
        uint8_t                             m_Orientation;
        uint8_t                             m_EventDetected;                        // Total number of active events detected at last scan

      #ifdef DIGINI_USE_PDI_MULTI_EVENT
        ServiceEvent_e                      m_EventX      [PDI_NUMBER_OF_EVENT];
        ServiceEvent_e                      m_EventY      [PDI_NUMBER_OF_EVENT];
        uint8_t                             m_EventWeight [PDI_NUMBER_OF_EVENT];    // Weight property of events
        ServiceEvent_e                      m_EventID     [PDI_NUMBER_OF_EVENT];    // Take value of type
        uint8_t                             m_EventArea   [PDI_NUMBER_OF_EVENT];    // Event area of each Event
        ServiceEvent_e                      m_GestureID;                            // Type of gesture detected
      #else
        ServiceEvent_e                      m_EventX;
        ServiceEvent_e                      m_EventY;
      #endif
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

extern class   PDI_myClassTask  PDI_Task;
extern class   PDI_myClassTask* PDI_pTask;

#ifdef LIB_PDI_TASK_GLOBAL
 class   PDI_myClassTask        PDI_Task;
 class   PDI_myClassTask*       PDI_pTask = &PDI_Task;
#endif

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

extern "C" void     PDI_TaskWrapper     (void* pvParameters);

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_POINTING_DEVICE

#endif // DIGINI_USE_GRAFX
