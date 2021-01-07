//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_basic_box.cpp
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
#ifdef BASIC_BOX_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CBasicBox
//
//  Parameter(s):   BasicBox_t          pBasicBox        Pointer to BasicBox_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CBasicBox::CBasicBox(BasicBox_t* pBasicBox)
{
    ImageInfo_t ImageInfo;

    m_pBasicBox = pBasicBox;

    DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, pBasicBox->ImageTL, 0);
    m_TopLeft.X     = pBasicBox->Box.Pos.X;
    m_X_VerLeft     = pBasicBox->Box.Pos.X;
    m_X_Hor         = m_TopLeft.X + ImageInfo.Size.Width;

    m_TopLeft.Y     = pBasicBox->Box.Pos.Y;
    m_Y_HorTop      = pBasicBox->Box.Pos.Y;
    m_Y_Ver         = pBasicBox->Box.Pos.Y + ImageInfo.Size.Height;

    DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, pBasicBox->ImageTR, 0);
    m_TopRight.X    = ((pBasicBox->Box.Pos.X + pBasicBox->Box.Size.Width) - 1) - ImageInfo.Size.Width;
    m_TopRight.Y    = pBasicBox->Box.Pos.Y;

    DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, pBasicBox->ImageBL, 0);
    m_BottomLeft.X  = pBasicBox->Box.Pos.X;
    m_BottomLeft.Y  = ((pBasicBox->Box.Pos.Y + pBasicBox->Box.Size.Height) - 1) - ImageInfo.Size.Height;

    DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, pBasicBox->ImageBR, 0);
    m_BottomRight.X = ((pBasicBox->Box.Pos.X + pBasicBox->Box.Size.Width) - 1) - ImageInfo.Size.Width;
    m_BottomRight.Y = ((pBasicBox->Box.Pos.Y + pBasicBox->Box.Size.Height) - 1) - ImageInfo.Size.Height;

    DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, pBasicBox->ImageH, 0);
    m_Y_HorBot      = ((pBasicBox->Box.Pos.Y + pBasicBox->Box.Size.Height) - 1) - ImageInfo.Size.Height;

    DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, pBasicBox->ImageV, 0);
    m_X_VerRight    = ((pBasicBox->Box.Pos.X + pBasicBox->Box.Size.Width) - 1) - ImageInfo.Size.Width;
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
Link_e CBasicBox::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;
    EventArea_t      EventArea;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;

    if((pService = ServiceCall(&m_pBasicBox->Service, &m_ServiceState)) != nullptr)
    {
        EventArea.Rectangle.Box = m_pBasicBox->Box;
      #ifdef DIGINI_USE_POINTING_DEVICE
        PDI_pTask->CreateZone(&EventArea, m_pBasicBox->Options, pPageWidget->ID);       // Create the zone on the touch sense virtual screen
      #endif
        Draw(pService);
        FreeServiceStruct(&pService);
    }

    if((m_pBasicBox->Options & GRAFX_OPTION_DRAW_ONCE_AND_REMOVE) != 0)
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
Link_e CBasicBox::Refresh(MsgRefresh_t* pMsg)
{
     ServiceReturn_t* pService;

    m_ServiceState = SERVICE_REFRESH;

   // If it is our touch zone that is pressed then change service state to process the touch
    GUI_FilterServiceState(&m_ServiceState, pMsg, m_pPageWidget->ID, m_pBasicBox->ServiceFilter, SERVICE_IDLE);

    // Invoke the application service for this basic box (It might change by itself the m_ServiceState)
    if((pService = ServiceCall(&m_pBasicBox->Service, &m_ServiceState)) != nullptr)
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
                return Link;                                            // Stop here, useless to continue because we will switch screen
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
void CBasicBox::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;

    if((pService = ServiceCall(&m_pBasicBox->Service, &m_ServiceState)) != nullptr)
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
void CBasicBox::Draw(ServiceReturn_t* pService)
{
  #ifndef GRAFX_DEBUG_GUI
    Layer_e BackLayerToDraw;
    Layer_e ForeLayerToDraw;
  #endif

    CLayer::PushDrawing();

  #ifdef GRAFX_DEBUG_GUI
    CLayer::SetDrawing(((m_pBasicBox->Options & GRAFX_OPTION_DRAW_ON_BACK) != 0) ? BACKGROUND_DISPLAY_LAYER_0 : FOREGROUND_DISPLAY_LAYER_0);
  #else

   #ifdef GRAFX_USE_CONSTRUCTION_FOREGROUND_LAYER
    ForeLayerToDraw = CONSTRUCTION_FOREGROUND_LAYER;
   #else
    ForeLayerToDraw = FOREGROUND_DISPLAY_LAYER_0;
   #endif

   #ifdef GRAFX_USE_CONSTRUCTION_BACKGROUND_LAYER
    BackLayerToDraw = CONSTRUCTION_BACKGROUND_LAYER;
   #else
    BackLayerToDraw = BACKEGROUND_DISPLAY_LAYER_0;
   #endif

    if(SKIN_pTask->IsSkinLoaded() != true)
    {
        CLayer::SetDrawing(FOREGROUND_DISPLAY_LAYER_0);   // On loading with do print directly on foreground layer
    }
    else
    {
        CLayer::SetDrawing(((m_pBasicBox->Options & GRAFX_OPTION_DRAW_ON_BACK) != 0) ? BackLayerToDraw : ForeLayerToDraw);
    }
  #endif

    if((m_pBasicBox->Options & GRAFX_OPTION_CLEAR) != 0)
    {
        CLayer::SetColor(BLACK);
        DrawRectangle(&m_pBasicBox->Box);
    }

    CopyLinear(m_pBasicBox->ImageTL, m_TopLeft,     ALPHA_BLEND);
    CopyLinear(m_pBasicBox->ImageTR, m_TopRight,    ALPHA_BLEND);
    CopyLinear(m_pBasicBox->ImageBL, m_BottomLeft,  ALPHA_BLEND);
    CopyLinear(m_pBasicBox->ImageBR, m_BottomRight, ALPHA_BLEND);

    Cartesian_t Top;
    Cartesian_t Bot;

    Top.Y = m_Y_HorTop;
    Bot.Y = m_Y_HorBot;

    for(int16_t i = m_X_Hor; i < m_TopRight.X; i++)
    {
        Top.X = i;
        Bot.X = i;

        CopyLinear(m_pBasicBox->ImageH, Top, ALPHA_BLEND);
        CopyLinear(m_pBasicBox->ImageH, Bot, ALPHA_BLEND);
    }

    Cartesian_t Left;
    Cartesian_t Right;

    Left.X  = m_X_VerLeft;
    Right.X = m_X_VerRight;

    for(int16_t i = m_Y_Ver; i < m_BottomLeft.Y; i++)
    {
        Left.Y  = i;
        Right.Y = i;

        CopyLinear(m_pBasicBox->ImageV, Left,  ALPHA_BLEND);
        CopyLinear(m_pBasicBox->ImageV, Right, ALPHA_BLEND);
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
void CBasicBox::DrawOnce(ServiceReturn_t* pService)
{

    CLayer::PushDrawing();

    // Copy merge stuff

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // BASIC_BOX_DEF
#endif // DIGINI_USE_GRAFX
