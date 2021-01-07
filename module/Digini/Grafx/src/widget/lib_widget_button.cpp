//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_button.cpp
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
#ifdef BTN_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CButton
//
//  Parameter(s):   Button_t         pButton         Pointer to Button_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CButton::CButton(Button_t* pButton)
{
    m_pButton = pButton;
    m_pButton->Text.Blend      = ALPHA_BLEND;
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
//  Note(s)         No link on creation of button, always invalid
//
//-------------------------------------------------------------------------------------------------
Link_e CButton::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;
    if((pService = ServiceCall(&m_pButton->Service, &m_ServiceState)) != nullptr)
    {
      #ifdef DIGINI_USE_POINTING_DEVICE
        PDI_pTask->CreateZone(&m_pButton->EventArea, m_pButton->Options, pPageWidget->ID);         // Create the zone on the touch sense virtual screen
      #endif
        this->Draw(pService);
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
Link_e CButton::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    // If it is our touch zone that is pressed then change service state to process the touch
    GUI_FilterServiceState(&m_ServiceState, pMsg, m_pPageWidget->ID, m_pButton->ServiceFilter, SERVICE_IDLE);

    // Invoke the application service for this button (It might change by itself the m_ServiceState)
    if((pService = ServiceCall(&m_pButton->Service, &m_ServiceState)) != nullptr)
    {
        if((pService->ServiceType == SERVICE_RETURN) && (m_ServiceState == SERVICE_RELEASED))
        {
            if(m_pPageWidget->Link != INVALID_LINK)
            {
                FreeServiceStruct(&pService);
                return m_pPageWidget->Link;                                 // Stop here, useless to continue because we will switch screen
            }
        }

        if(pService->ServiceType == SERVICE_RETURN_TYPE3)
        {
            Link_e Link = ((ServiceType3_t*)pService)->Link;
            if(Link != INVALID_LINK)
            {
                FreeServiceStruct(&pService);
                return Link;                                                // Stop here, useless to continue because we will switch screen
            }
        }

        if(m_ServiceState != SERVICE_IDLE)
        {
            this->Draw(pService);
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
void CButton::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;
    if((pService = ServiceCall(&m_pButton->Service, &m_ServiceState)) != nullptr)
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
//  Description:    Draw the button on display according to state.
//
//-------------------------------------------------------------------------------------------------
void CButton::Draw(ServiceReturn_t* pService)
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

    //if(m_ServiceState == SERVICE_MOVE_UP)// TODO (Alain#1#) what this do ... i have no idea... missing comment
    //{
    //    pService->IndexState = 2;
    //}

    if(m_pButton->Image.ID_List[pService->IndexState] != INVALID_IMAGE)
    {
        CopyLinear(m_pButton->Image.ID_List[pService->IndexState], m_pButton->Pos, CLEAR_BLEND);

        // Need image button for label
        if(m_pButton->Text.Label != INVALID_LABEL)
        {
            WidgetPrint(&m_pButton->Text, pService);
        }
    }

    // Glyph can be a button
    if(m_pButton->Glyph.ID_List[pService->IndexState] != INVALID_IMAGE)
    {
        CopyLinear(m_pButton->Glyph.ID_List[pService->IndexState], m_pButton->Glyph.Pos, ALPHA_BLEND);
    }

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
void CButton::DrawOnce(ServiceReturn_t* pService)
{

    CLayer::PushDrawing();

    // Copy merge stuff

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // BTN_DEF
#endif // DIGINI_USE_GRAFX
