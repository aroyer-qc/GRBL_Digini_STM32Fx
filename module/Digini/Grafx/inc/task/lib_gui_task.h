//-------------------------------------------------------------------------------------------------
//
//  File : lib_gui_task.h
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
#include "nOS.h"

//-------------------------------------------------------------------------------------------------
// Global Define(s)
//-------------------------------------------------------------------------------------------------

#ifdef LIB_GUI_TASK_GLOBAL
  #define LIB_GUI_TASK_EXTERN
#else
  #define LIB_GUI_TASK_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define GUI_TASK_STACK_SIZE                     4096
#define GUI_TASK_PRIO                           6
#define GUI_NUMBER_OF_MSG                       4

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class GUI_myClassTask
{
    public:

        void                        Run                 (void);
        nOS_Error                   Initialize          (void);

      #ifdef GRAFX_USE_SLIDING_PAGE
        void                        SetSlidingDirection (SlideDir_e SlidingDirection) {m_SlidingDir = SlidingDirection;};
        void                        SetSlidingRange     (PageSlideRange_t* pPageSlideOverride);
      #endif

        nOS_Queue                   m_Q_Msg;

    private:

        Link_e                      CreateAllWidget     (void);
        Link_e                      RefreshAllWidget    (MsgRefresh_t* pMsg);
        void                        FinalizeAllWidget   (void);

      #ifdef GRAFX_USE_SLIDING_PAGE
        bool                        SlidingPage         (void);
      #endif

        Link_e                      m_Link;
        PageWidget_t*               m_pPage;
        uint8_t                     m_WidgetCount;
        class CWidgetInterface**    m_pWidgetList;
        nOS_Thread                  m_Handle;
        nOS_Stack                   m_Stack[GUI_TASK_STACK_SIZE];
        MsgRefresh_t                m_ArrayMsg[GUI_NUMBER_OF_MSG];

      #ifdef GRAFX_USE_SLIDING_PAGE
        SlideDir_e                  m_SlidingDir;
        PageSlideRange_t            m_SlideRange;
      #endif
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

LIB_GUI_TASK_EXTERN class   GUI_myClassTask         GUI_Task;
LIB_GUI_TASK_EXTERN         Widget_e*               GUI_pPagePrevious;
LIB_GUI_TASK_EXTERN         void*                   GUI_pMailBox;           // Pointer that can be used only before calling ServiceCall, allow struct pointer to be communicated to Service

#ifdef LIB_GUI_TASK_GLOBAL
                    class   GUI_myClassTask*        GUI_pTask = &GUI_Task;
#else
    extern          class   GUI_myClassTask*        GUI_pTask;
#endif

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

extern "C" void GUI_TaskWrapper         (void* pvParameters);
void            GUI_FilterServiceState  (ServiceEvent_e* pServiceState, MsgRefresh_t* pMsg, Widget_e Widget, uint16_t Filter, ServiceEvent_e DefaultState);
void            GUI_ClearWidgetLayer    (void);

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

