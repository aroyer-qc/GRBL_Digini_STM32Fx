//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_label.cpp
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

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "widget_cfg.h"
#ifdef LABEL_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CLabel
//
//  Parameter(s):   Label_t         pLabel         Pointer to Label_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CLabel::CLabel(Label_t* pLabel)
{
    m_pLabel = pLabel;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Create
//
//  Parameter(s):   PageWidget_t* pPageWidget
//  Return:         Link_e        link
//
//  Description:    This function call service and create the widget
//
//  Note(s)         No link on creation of label, always invalid
//
//-------------------------------------------------------------------------------------------------
Link_e CLabel::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_START;
    m_pPageWidget  = pPageWidget;
    if((pService = ServiceCall(&m_pLabel->Service, &m_ServiceState)) != nullptr)
    {
        if(m_ServiceState == SERVICE_REFRESH)
        {
            Draw(pService);
        }
        FreeServiceStruct(&pService);
    }

    if((m_pLabel->Options & GRAFX_OPTION_DRAW_ONCE_AND_REMOVE) != 0)
    {
        return REMOVE_WIDGET;
    }

    return INVALID_LINK;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Refresh
//
//  Parameter(s):   MsgRefresh_t*        pMsg
//  Return:         Link_e               Link            Link to switch page if it's needed
//
//  Description:    This function call service to refresh widget
//
//-------------------------------------------------------------------------------------------------
Link_e CLabel::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_REFRESH;

    // Invoke the application service for this label (It might change by itself the m_ServiceState)
    if((pService = ServiceCall(&m_pLabel->Service, &m_ServiceState)) != nullptr)
    {
        // TODO replace by a link return type handle
        if(pService->ServiceType == SERVICE_RETURN)
        {
            if(m_pPageWidget->Link != INVALID_LINK)
            {
                FreeServiceStruct(&pService);
                return m_pPageWidget->Link;        // Stop here, useless to continue because we will switch screen
            }
        }

        if(m_ServiceState == SERVICE_REFRESH)
        {
            if((m_pLabel->Options & GRAFX_OPTION_DRAW_ONCE) != GRAFX_OPTION_DRAW_ONCE)
            {
                Draw(pService);
            }
        }

        FreeServiceStruct(&pService);
    }

    return INVALID_LINK;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Finalize
//
//  Parameter(s):   none
//  Return:         void
//
//  Description:    This function call service associated with widget to finalize it properly.
//
//-------------------------------------------------------------------------------------------------
void CLabel::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;

    if((pService = ServiceCall(&m_pLabel->Service, &m_ServiceState)) != nullptr)
    {
        FreeServiceStruct(&pService);
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Draw
//
//  Parameter(s):   ServiceReturn_t* pService
//  Return:         None
//
//
//  Description:    Draw the label on display according to state.
//
//-------------------------------------------------------------------------------------------------
void CLabel::Draw(ServiceReturn_t* pService)
{
  #ifndef GRAFX_DEBUG_GUI
    Layer_e BackLayerToDraw;
    Layer_e ForeLayerToDraw;
  #endif

    CLayer::PushDrawing();

  #ifdef GRAFX_DEBUG_GUI
    CLayer::SetDrawing(((m_pLabel->Options & GRAFX_OPTION_DRAW_ON_BACK) != 0) ? BACKGROUND_DISPLAY_LAYER_0 : FOREGROUND_DISPLAY_LAYER_0);
  #else
   #ifdef GRAFX_USE_CONSTRUCTION_FOREGROUND_LAYER
    ForeLayerToDraw = CONSTRUCTION_FOREGROUND_LAYER;
   #else
    ForeLayerToDraw = FOREGROUND_DISPLAY_LAYER_0;
   #endif

   #ifdef GRAFX_USE_BACKGROUND_LAYER
    #ifdef GRAFX_USE_CONSTRUCTION_BACKGROUND_LAYER
     BackLayerToDraw = CONSTRUCTION_BACKGROUND_LAYER;
    #else
     BackLayerToDraw = BACKGROUND_DISPLAY_LAYER_0;
    #endif
    if(SKIN_pTask->IsSkinLoaded() != true)
    {
        CLayer::SetDrawing(FOREGROUND_DISPLAY_LAYER_0);   // On loading with do print directly on foreground layer
    }
    else
    {
        CLayer::SetDrawing(((m_pLabel->Options & GRAFX_OPTION_DRAW_ON_BACK) != 0) ? BackLayerToDraw : ForeLayerToDraw);
    }
   #else
    CLayer::SetDrawing(ForeLayerToDraw);
   #endif
  #endif

    WidgetPrint(&m_pLabel->Text, pService);
    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // LABEL_DEF
#endif // DIGINI_USE_GRAFX
