//-------------------------------------------------------------------------------------------------
//
//  File :  lib_gui_task.cpp
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

#include <stdint.h>
#include <string.h>
#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#define LIB_GUI_TASK_GLOBAL
#include "lib_grafx.h"
#undef  LIB_GUI_TASK_GLOBAL
#include "lib_memory.h"

//-------------------------------------------------------------------------------------------------
//
//  Name:           GUI_TaskWrapper
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//  Description:    C++ Wrapper for the task
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
extern "C" void GUI_TaskWrapper(void* pvParameters)
{
    (static_cast<GUI_myClassTask*>(pvParameters))->Run();
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   void
//  Return:         nOS_Error
//
//  Description:    Initialize the task
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
nOS_Error GUI_myClassTask::Initialize(void)
{
    nOS_Error Error;

    if((Error = nOS_QueueCreate(&this->m_Q_Msg, &this->m_ArrayMsg[0], sizeof(MsgRefresh_t), GUI_NUMBER_OF_MSG)) == NOS_OK)
    {
        Error = nOS_ThreadCreate(&this->m_Handle,
                                 GUI_TaskWrapper,
                                 this,
                                 &this->m_Stack[0],
                                 GUI_TASK_STACK_SIZE,
                                 GUI_TASK_PRIO);
    }

    return Error;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           run
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//-------------------------------------------------------------------------------------------------
void GUI_myClassTask::Run()
{
    nOS_Error       Error;
    MsgRefresh_t    Msg;
    Link_e          NewLink;
    Link_e          PreviousLink;
    bool            SlidePage;
    int16_t         SlidePos;
    nOS_TickCounter WaitTick;

    // Start at home page
    m_Link    = INVALID_LINK;
    NewLink   = LINK_MAIN_LOADING;
    SlidePage = false;

  #if defined(DIGINI_USE_A_SKIN) && defined(STATIC_SKIN_DEF)
    // Static skin must be loaded
    while(SKIN_pTask->IsStaticSkinLoaded() != true)
    {
        nOS_Sleep(10);
    };

    nOS_Sleep(10); // Allow other task to run   TODO: (Alain#1#) maybe delay is not the best option
  #endif

    for(;;)
    {
      #ifdef GRAFX_USE_SLIDING_PAGE
        WaitTick = (SlidePage == false) ? GRAFX_TICK_WAIT_BETWEEN_REFRESH_LOOP : 2;
      #else
      #endif

        if((Error = nOS_QueueRead(&this->m_Q_Msg, &Msg, WaitTick)) == NOS_E_TIMEOUT)
        {
            Msg.Type     = MESSAGE_TYPE_PDI_EVENT_INFO;
            Msg.Touch    = SERVICE_IDLE;
            Msg.WidgetID = INVALID_WIDGET;
            Error        = NOS_OK;
        }
        else
        {
            __asm("nop");
        }

        if(Error == NOS_OK)
        {
          #ifdef GRAFX_USE_SLIDING_PAGE
            if(SlidePage == false)
            {
          #endif
                if(NewLink != INVALID_LINK)                                             // If we have a new link (switch to new page)
                {                                                                       // Finalize old page then create new page
                    if(m_Link != INVALID_LINK)                                          // finalize the actual page if it exist
                    {
                        FinalizeAllWidget();
                    }
                  #ifdef DIGINI_USE_POINTING_DEVICE
                 //   PDI_pTask->ClearAllZone();
                  #endif
                    GUI_ClearWidgetLayer();

                    // TODO (Alain#2#) maybe do a stack of previous link... now only one level is active
                    PreviousLink = m_Link;
                    m_Link  = NewLink;
                    NewLink = CreateAllWidget();

                   #ifdef GRAFX_USE_SLIDING_PAGE
                   #ifdef DIGINI_USE_LOAD_SKIN
                    if(SKIN_pTask->IsSkinLoaded() == true)
                   #endif
                    {
                        if(NewLink == INVALID_LINK)
                        {
                            SlidePage = true;
                            SlidePos  = GRAFX_SIZE_X;               // Use sliding direction... also need a sliding windows size bigger than real display, now just for testing
                        }
                    }
                  #endif
                }
                else
                {
                    NewLink = RefreshAllWidget(&Msg);
                }

                if(NewLink == PREVIOUS_LINK)
                {
                    NewLink = PreviousLink;
                }
          #ifdef GRAFX_USE_SLIDING_PAGE
            }
          #endif

         #ifndef GRAFX_DEBUG_GUI
          #ifdef GRAFX_USE_CONSTRUCTION_FOREGROUND_LAYER
           #ifdef DIGINI_USE_LOAD_SKIN
            if(SKIN_pTask->IsSkinLoaded() == true)
           #endif
            {
               //myGrafx->WaitFor_V_Sync();
              #ifdef GRAFX_USE_SLIDING_PAGE
                if(SlidePage == true)
                {
                    SlidePos -= GRAFX_SLIDING_PAGE_GRANULARITY;
                    myGrafx->CopyLayerToLayer(FOREGROUND_DISPLAY_LAYER_0, FOREGROUND_DISPLAY_LAYER_0, GRAFX_SLIDING_PAGE_GRANULARITY, 0, 0, 0, GRAFX_SIZE_X - GRAFX_SLIDING_PAGE_GRANULARITY, GRAFX_SIZE_Y);
                    myGrafx->CopyLayerToLayer(CONSTRUCTION_FOREGROUND_LAYER, FOREGROUND_DISPLAY_LAYER_0, 0, 0, SlidePos, 0, GRAFX_SIZE_X - SlidePos, GRAFX_SIZE_Y);

                    if(SlidePos == 0)
                    {
                        SlidePage = false;

                     #ifndef GRAFX_DEBUG_GUI
                      #ifdef GRAFX_USE_CONSTRUCTION_BACKGROUND_LAYER
                       #ifdef DIGINI_USE_LOAD_SKIN
                        if(SKIN_pTask->IsSkinLoaded() == true)
                       #endif
                        {
                            myGrafx->WaitFor_V_Sync();
                            myGrafx->CopyLayerToLayer(CONSTRUCTION_BACKGROUND_LAYER, BACKGROUND_DISPLAY_LAYER_0, 0, 0, GRAFX_SIZE_X, GRAFX_SIZE_Y);
                        }
                      #endif
                     #endif
                    }
                }
                else if(NewLink == INVALID_LINK) /// not sure
              #endif
                {
                    myGrafx->CopyLayerToLayer(CONSTRUCTION_FOREGROUND_LAYER, FOREGROUND_DISPLAY_LAYER_0, 0, 0, GRAFX_SIZE_X, GRAFX_SIZE_Y);
                }
            }
          #endif
         #endif
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           CreateAllWidget
//
//  Parameter(s):   None
//  Return:         Link_e NewLink
//
//  Description:    Allocated all widget in the memory block
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
Link_e GUI_myClassTask::CreateAllWidget()
{
    CWidgetInterface*   pWidget;
    CWidgetInterface**  pWidgetListPointer;
    PageWidget_t*       pPage;
    Widget_e            Widget;
    Link_e              NewLink = INVALID_LINK;
    int                 Count;                                                          // Widget count in page
    int                 CountCopy;
    void*               pWidgetTemp;

    m_pPage = (PageWidget_t*)PageWidget[m_Link];                                        // Get the page pointer
    pPage   = m_pPage;
    m_WidgetCount = 0;
    while((pPage + m_WidgetCount)->ID != INVALID_WIDGET)                                // Get the widget count in the page
    {
        m_WidgetCount++;
    }

    // Reserve memory for CWidgetInterface pointer for the number of widget in the page
    m_pWidgetList = (CWidgetInterface**)pMemory->Alloc(m_WidgetCount * sizeof(CWidgetInterface*));

    Count               = 0;
    pWidgetListPointer  = m_pWidgetList;
    while(Count < m_WidgetCount)
    {
        Widget = pPage->ID;

        if(Widget == INVALID_WIDGET)
        {
            // Do Nothing
        }

// todo remap for the more frequent first
       #ifdef BACK_DEF
        else if((Widget > APP_START_BACK_CONST) && (Widget < APP_END_BACK_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CBackground));                   // Get the memory block for the widget class object
            *pWidgetListPointer = pWidget;                                                                  // copy this pointer to our list of widget pointer for delete later
            pWidgetTemp         = new CBackground(&Background[Widget - (APP_START_BACK_CONST + 1)]);        // Get a class object of the widget
            memcpy(pWidget, pWidgetTemp, sizeof(CBackground));                                              // copy the class object to new reserve location
            delete (CBackground*)pWidgetTemp;                                                               // delete the dynamically allocated object
        }
       #endif

       #ifdef BASIC_BOX_DEF
        else if((Widget > APP_START_BASIC_BOX_CONST) && (Widget < APP_END_BASIC_BOX_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CBasicBox));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CBasicBox(&BasicBox[Widget - (APP_START_BASIC_BOX_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CBasicBox));
            delete (CBasicBox*)pWidgetTemp;
        }
       #endif

       #ifdef BASIC_BTN_DEF
        else if((Widget > APP_START_BASIC_BTN_CONST) && (Widget < APP_END_BASIC_BTN_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CBasicButton));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CBasicButton(&BasicButton[Widget - (APP_START_BASIC_BTN_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CBasicButton));
            delete (CBasicButton*)pWidgetTemp;
        }
       #endif

       #ifdef BASIC_RECT_DEF
        else if((Widget > APP_START_BASIC_RECT_CONST) && (Widget < APP_END_BASIC_RECT_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CBasicRect));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CBasicRect(&BasicRect[Widget - (APP_START_BASIC_RECT_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CBasicRect));
            delete (CBasicRect*)pWidgetTemp;
        }
       #endif

       #ifdef BTN_DEF
        else if((Widget > APP_START_BTN_CONST) && (Widget < APP_END_BTN_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CButton));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CButton(&Button[Widget - (APP_START_BTN_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CButton));
            delete (CButton*)pWidgetTemp;
        }
       #endif

       #ifdef GIF_DEF
        else if((Widget > APP_START_GIF_CONST) && (Widget < APP_END_GIF_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CGif));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CGif(&Gif[Widget - (APP_START_GIF_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CGif));
            delete (CGif*)pWidgetTemp;
        }
       #endif

       #ifdef ICON_DEF
        else if((Widget > APP_START_ICON_CONST) && (Widget < APP_END_ICON_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CIcon));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CIcon(&Icon[Widget - (APP_START_ICON_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CIcon));
            delete (CIcon*)pWidgetTemp;
        }
       #endif

       #ifdef LABEL_DEF
        else if((Widget > APP_START_LABEL_CONST) && (Widget < APP_END_LABEL_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CLabel));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CLabel(&Label[Widget - (APP_START_LABEL_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CLabel));
            delete (CLabel*)pWidgetTemp;
        }
       #endif

       #ifdef LABEL_LIST_DEF
        else if((Widget > APP_START_LABEL_LIST_CONST)  && (Widget < APP_END_LABEL_LIST_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CLabelList));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CLabelList(&LabelList[Widget - (APP_START_LABEL_LIST_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CLabelList));
            delete (CLabelList*)pWidgetTemp;
        }
       #endif

       #ifdef METER_DEF
        else if((Widget > APP_START_METER_CONST) && (Widget < APP_END_METER_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CMeter));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CMeter(&Meter[Widget - (APP_START_METER_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CMeter));
            delete (CMeter*)pWidgetTemp;
        }
       #endif

       #ifdef PAGE_SLIDE_DEF
        else if((Widget > APP_START_PAGE_SLIDE_CONST) && (Widget < APP_END_PAGE_SLIDE_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CPageSlide));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CPageSlide(&PageSlide[Widget - (APP_START_PAGE_SLIDE_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CPageSlide));
            delete (CPageSlide*)pWidgetTemp;
        }
       #endif

       #ifdef PANEL_DEF
        else if((Widget > APP_START_PANEL_CONST) && (Widget < APP_END_PANEL_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CPanel));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CPanel(&Panel[Widget - (APP_START_PANEL_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CPanel));
            delete (CPanel*)pWidgetTemp;
        }
       #endif

       #ifdef PROGRESS_DEF
        else if((Widget > APP_START_PROGRESS_CONST) && (Widget < APP_END_PROGRESS_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CProgress));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CProgress(&Progress[Widget - (APP_START_PROGRESS_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CProgress));
            delete (CProgress*)pWidgetTemp;
        }
       #endif

       #ifdef TERMINAL_DEF
        else if((Widget > APP_START_TERMINAL_CONST) && (Widget < APP_END_TERMINAL_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CTerminal));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CTerminal(&Terminal[Widget - (APP_START_TERMINAL_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CTerminal));
            delete (CTerminal*)pWidgetTemp;
        }
       #endif

       #ifdef SPECTRUM_DEF
        else if((Widget > APP_START_SPECTRUM_CONST) && (Widget < APP_END_SPECTRUM_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CSpectrum));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CSpectrum(&Spectrum[Widget - (APP_START_SPECTRUM_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CSpectrum));
            delete (CSpectrum*)pWidgetTemp;
        }
       #endif

       #ifdef SPRITE_DEF
        else if((Widget > APP_START_SPRITE_CONST) && (Widget < APP_END_SPRITE_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CSprite));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CSprite(&Sprite[Widget - (APP_START_SPRITE_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CSprite));
            delete (CSprite*)pWidgetTemp;
        }
       #endif

        #ifdef TERMINAL_DEF
        else if((Widget > APP_START_TERMINAL_CONST) && (Widget < APP_END_TERMINAL_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CTerminal));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CTerminal(&Terminal[Widget - (APP_START_TERMINAL_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CTerminal));
            delete (CTerminal*)pWidgetTemp;
        }
       #endif

       #ifdef VIRTUAL_HUB_DEF
        else if((Widget > APP_START_VIRTUAL_HUB_CONST) && (Widget < APP_END_VIRTUAL_HUB_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CVirtualHub));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CVirtualHub(&VirtualHub[Widget - (APP_START_VIRTUAL_HUB_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CVirtualHub));
            delete (CVirtualHub*)pWidgetTemp;
        }
       #endif

       #ifdef VIRTUAL_WINDOW_DEF
        else if((Widget > APP_START_VIRTUAL_WINDOW_CONST) && (Widget < APP_END_VIRTUAL_WINDOW_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CVirtualWindow));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CVirtualWindow(&VirtualWindow[Widget - (APP_START_VIRTUAL_WINDOW_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CVirtualWindow));
            delete (CVirtualWindow*)pWidgetTemp;
        }
       #endif
/*  // TODO (Alain#1#)  remove if the new method of exchange data work!!
       #ifdef XCHANGE_DEF
        else if((Widget > APP_START_XCHANGE_CONST) && (Widget < APP_END_XCHANGE_CONST))
        {
            pWidget             = (CWidgetInterface*)pMemory->Alloc(sizeof(CExchange));
            *pWidgetListPointer = pWidget;
            pWidgetTemp         = new CExchange(&Exchange[Widget - (APP_START_XCHANGE_CONST + 1)]);
            memcpy(pWidget, pWidgetTemp, sizeof(CExchange));
            delete (CExchange*)pWidgetTemp;
        }
       #endif
*/
        Count++;
        pPage++;
        pWidgetListPointer++;
    }

    // Reset all pointer and count to beginning of the list
    pPage               = m_pPage;
    Count               = 0;
    pWidgetListPointer  = m_pWidgetList;

    while((Count < m_WidgetCount) && (NewLink == INVALID_LINK))
    {
        // TO DO init a timer for each widget contained in the page and according to his refresh rate

        NewLink = (*pWidgetListPointer)->Create(pPage);

        // If a widget is a draw only once
        if(NewLink == REMOVE_WIDGET)
        {
            // Free this widget
            pMemory->Free((void**)&(*pWidgetListPointer));

            // Move all widget in the list up one position
            CountCopy = Count;
            while(CountCopy < m_WidgetCount)
            {
               //*(pWidgetListPointer + CountCopy) = *(pWidgetListPointer + CountCopy + 1);
                *(m_pWidgetList + CountCopy) = *(m_pWidgetList + CountCopy + 1);            // FIX  ?????  for crashing remove widget
                CountCopy++;
            }

            NewLink = INVALID_LINK;
            m_WidgetCount--;
        }
        else
        {
            pWidgetListPointer++;
            Count++;
        }
        pPage++;
    }

    return NewLink;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           RefreshAllWidget
//
//  Parameter(s):   MsgRefresh_t*    pMsg
//  Return:         Link_e           Link
//
//  Description:    Refresh all widget
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
Link_e GUI_myClassTask::RefreshAllWidget(MsgRefresh_t* pMsg)
{
    CWidgetInterface**  pWidgetListPointer;
    Link_e              NewLink;
    uint8_t             Count;

    NewLink = INVALID_LINK;
    pWidgetListPointer = m_pWidgetList;
    Count = 0;

    while((Count < m_WidgetCount) && (NewLink == INVALID_LINK))
    {
        // TO DO  Check widget timer in the page to see if it need to be refreshed
        NewLink = (*pWidgetListPointer)->Refresh(pMsg);
        Count++;
        pWidgetListPointer++;
        nOS_Yield();
    }

    return NewLink;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           FinalizeAllWidget
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Finalize all widget
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GUI_myClassTask::FinalizeAllWidget()
{
    CWidgetInterface**  pWidgetListPointer;
    uint8_t             Count;

    pWidgetListPointer = m_pWidgetList;
    Count = 0;
    while(Count < m_WidgetCount)
    {
        (*pWidgetListPointer)->Finalize();
        pMemory->Free((void**)&(*pWidgetListPointer));
        pWidgetListPointer++;
        Count++;
        nOS_Yield();
    }

    pMemory->Free((void**)&m_pWidgetList);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           GUI_FilterServiceState
//
//  Parameter(s):   ServiceEvent_e* pServiceState
//                  MsgRefresh_t*   pMsg
//                  Widget_e        Widget
//                  uint16_t        Filter              Filter option
//                  ServiceEvent_e  DefaultState
//  Return:         None
//
//  Description:    Will return only value in filter option otherwise it return default
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GUI_FilterServiceState(ServiceEvent_e* pServiceState, MsgRefresh_t* pMsg, Widget_e Widget, uint16_t Filter, ServiceEvent_e DefaultState)
{
    uint16_t State;
    *pServiceState = DefaultState;

    if(pMsg->WidgetID == Widget)
    {
        State = Filter & pMsg->Touch;
        if(State != 0)
        {
            *pServiceState = ServiceEvent_e(State);
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           GUI_ClearWidgetLayer
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Will clear the widget layer for a new layer to be draw
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
// TODO (Alain#1#): it might already be handle by the class
void GUI_ClearWidgetLayer()
{
    Box_t Box = {{0, 0},{GRAFX_SIZE_X, GRAFX_SIZE_Y}};
    CLayer::PushDrawing();

  #ifdef GRAFX_DEBUG_GUI
    CLayer::SetDrawing(FOREGROUND_DISPLAY_LAYER_0);
  #else
   #ifdef GRAFX_USE_CONSTRUCTION_FOREGROUND_LAYER
    CLayer::SetDrawing(CONSTRUCTION_FOREGROUND_LAYER);
   #else
    CLayer::SetDrawing(FOREGROUND_DISPLAY_LAYER_0);
   #endif
  #endif

    CLayer::SetColor(TRANSPARENT);
    DrawRectangle(&Box);
    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
