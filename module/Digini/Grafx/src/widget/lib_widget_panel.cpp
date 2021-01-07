//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_panel.cpp
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
#ifdef PANEL_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CPanel
//
//  Parameter(s):   Panel_t         pPanel         Pointer to Panel_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CPanel::CPanel(Panel_t* pPanel)
{
    m_pPanel = pPanel;
    m_pPanel->Text.Box.Pos.X  += m_pPanel->Box.Pos.X;
    m_pPanel->Text.Box.Pos.Y  += m_pPanel->Box.Pos.Y;
    m_pPanel->Text.Blend       = ALPHA_BLEND;
    m_pPanel->Options         |= GRAFX_INIT_DONE;
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
//-------------------------------------------------------------------------------------------------
Link_e CPanel::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;
    if((pService = ServiceCall(&m_pPanel->Service, &m_ServiceState)) != nullptr)
    {
        if(pService->IndexState < SERVICE_ID_STATE_SIZE)                                                // Protect array boundary
        {
          #ifdef DIGINI_USE_POINTING_DEVICE
            PDI_pTask->CreateZone((EventArea_t*)&m_pPanel->Box, OPTION_TOUCH_RECTANGLE, pPageWidget->ID);  // Create the zone on the touch sense virtual screen
          #endif
            Draw(pService);
        }

        FreeServiceStruct(&pService);
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
Link_e CPanel::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    GUI_FilterServiceState(&m_ServiceState, pMsg, m_pPageWidget->ID, m_pPanel->ServiceFilter, m_LastServiceState);  // If is is our touch zone that is pressed than change service state to process the touch

    if((pService = ServiceCall(&m_pPanel->Service, &m_ServiceState)) != nullptr)                                   // Invoke the app service for this panel (It might change by itself the m_ServiceState)
    {
        if((pService->ServiceType == SERVICE_RETURN) && (m_ServiceState == SERVICE_RELEASED))
        {
            if(m_pPageWidget->Link != INVALID_LINK)
            {
                FreeServiceStruct(&pService);
                return m_pPageWidget->Link;                                                                     // Stop here, useless to continue because we will switch screen
            }
        }
        if(pService->ServiceType == SERVICE_RETURN_TYPE3)
        {
            Link_e Link = ((ServiceType3_t*)pService)->Link;
            if(Link != INVALID_LINK)
            {
                FreeServiceStruct(&pService);
                return Link;                                                                                    // Stop here, useless to continue because we will switch screen
            }
        }

        Draw(pService);
        FreeServiceStruct(&pService);
    }

    return INVALID_LINK;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Finalize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    This function call service associated with widget to finalize it properly.
//
//-------------------------------------------------------------------------------------------------
void CPanel::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;

    if((pService = ServiceCall(&m_pPanel->Service, &m_ServiceState)) != nullptr)
    {
        FreeServiceStruct(&pService);
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Draw
//
//  Parameter(s):   ServiceEvent_e    ServiceState
//                  ServiceReturn_t*  pService
//  Return:         None
//
//
//  Description:    Draw the panel on display.
//
//-------------------------------------------------------------------------------------------------
void CPanel::Draw(ServiceReturn_t* pService)
{
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

    if((m_ServiceState == SERVICE_PRESSED)   ||
       (m_ServiceState == SERVICE_TYPEMATIC) ||
       (m_ServiceState == SERVICE_SUPERKEY))
    {
        pService->IndexState++;
    }
    m_LastServiceState = m_ServiceState;

    if((m_ServiceState == SERVICE_RELEASED) && (m_LastServiceState == SERVICE_RELEASED))
    {
        m_LastServiceState = SERVICE_IDLE;
    }
    CopyLinear(m_pPanel->Image.ID_List[pService->IndexState], m_pPanel->Box.Pos, CLEAR_BLEND);
    WidgetPrint(&m_pPanel->Text, pService);
    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DrawOnce
//
//  Parameter(s):   ServiceReturn_t* pService
//  Return:         None
//
//
//  Description:    Draw only once what does not need to be refreshed
//
//-------------------------------------------------------------------------------------------------
void CPanel::DrawOnce(ServiceReturn_t* pService)
{

    CLayer::PushDrawing();

    // Copy merge stuff

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // PANEL_DEF
#endif // DIGINI_USE_GRAFX
