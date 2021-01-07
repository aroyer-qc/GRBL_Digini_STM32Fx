//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_basic_button.cpp
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
#ifdef BASIC_BTN_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CBasicButton
//
//  Parameter(s):   BasicButton_t         pBasicButton         Pointer to Button_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CBasicButton::CBasicButton(BasicButton_t* pBasicButton)
{
    m_pBasicButton = pBasicButton;

    m_pBasicButton->Text.Font[1]    = m_pBasicButton->Text.Font[0];
    m_pBasicButton->Text.Blend      = ALPHA_BLEND;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Create
//
//  Parameter(s):   sPageWidget* pPageWidget
//  Return:         Link_e        link
//
//  Description:    This function call service and create the widget
//
//  Note(s)         No link on creation of button, always invalid
//
//-------------------------------------------------------------------------------------------------
Link_e CBasicButton::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;
    EventArea_t      EventArea;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;
    if((pService = ServiceCall(&m_pBasicButton->Service, &m_ServiceState)) != nullptr)
    {
        EventArea.Rectangle.Box = m_pBasicButton->Box;
      #ifdef DIGINI_USE_POINTING_DEVICE
        PDI_pTask->CreateZone(&EventArea, m_pBasicButton->Options, pPageWidget->ID);       // Create the zone on the touch sense virtual screen
      #endif
        Draw(pService);
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
Link_e CBasicButton::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    // If it is our touch zone that is pressed then change service state to process the touch
    GUI_FilterServiceState(&m_ServiceState, pMsg, m_pPageWidget->ID, m_pBasicButton->ServiceFilter, SERVICE_IDLE);

    // Invoke the application service for this button (It might change by itself the m_ServiceState)
    if((pService = ServiceCall(&m_pBasicButton->Service, &m_ServiceState)) != nullptr)
    {
        if((pService->ServiceType == SERVICE_RETURN) && (m_ServiceState == SERVICE_RELEASED))
        {
            if(m_pPageWidget->Link != INVALID_LINK)
            {
                FreeServiceStruct(&pService);
                return m_pPageWidget->Link;                             // Stop here, useless to continue because we will switch screen
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
            Draw(pService);
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
void CBasicButton::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;
    if((pService = ServiceCall(&m_pBasicButton->Service, &m_ServiceState)) != nullptr)
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
void CBasicButton::Draw(ServiceReturn_t* pService)
{
    uint32_t BackColor;
    uint32_t BoxColor;

    CLayer::PushDrawing();

  #ifdef GRAFX_DEBUG_GUI
    CLayer::SetDrawing(BACKGROUND_DISPLAY_LAYER_0);
  #else
    CLayer::SetDrawing(CONSTRUCTION_BACKGROUND_LAYER);
  #endif

    if(m_ServiceState == SERVICE_PRESSED || m_ServiceState == SERVICE_TYPEMATIC || m_ServiceState == SERVICE_SUPERKEY)
    {
        pService->IndexState++;
        BackColor = m_pBasicButton->PressedBackColor;
        BoxColor  = m_pBasicButton->PressedBoxColor;
    }
    else
    {
        BackColor = m_pBasicButton->BackColor;
        BoxColor  = m_pBasicButton->BoxColor;
    }

    CLayer::SetColor(BackColor);
    DrawRectangle(&m_pBasicButton->Box);

    CLayer::SetColor(BoxColor);
    DrawBox(&m_pBasicButton->Box, m_pBasicButton->Thickness);

    WidgetPrint(&m_pBasicButton->Text, pService);
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
void CBasicButton::DrawOnce(ServiceReturn_t* pService)
{

    CLayer::PushDrawing();

    // Copy merge stuff

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // BASIC_BTN_DEF
#endif // DIGINI_USE_GRAFX
