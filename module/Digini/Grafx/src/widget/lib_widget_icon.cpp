//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_icon.cpp
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
#ifdef ICON_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CIcon
//
//  Parameter(s):   Icon_t         pIcon         Pointer to Icon_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CIcon::CIcon(Icon_t* pIcon)
{
    m_pIcon = pIcon;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Create
//
//  Parameter(s):   PageWidget_t* pPageWidget
//  Return:         Link_e        INVALID_LINK
//
//  Description:    This function call service and create the widget
//
//  Note(s)         No link, Always return INVALID_LINK
//
//-------------------------------------------------------------------------------------------------
Link_e CIcon::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;
    if((pService = ServiceCall(&m_pIcon->Service, &m_ServiceState)) != nullptr)
    {
        this->Draw(pService);
        FreeServiceStruct(&pService);
    }

    if((m_pIcon->Options & GRAFX_OPTION_DRAW_ONCE_AND_REMOVE) != 0)
    {
        return REMOVE_WIDGET;
    }

    return INVALID_LINK;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Refresh
//
//  Parameter(s):   MsgRefresh_t*    pMsg
//  Return:         Link_e           INVALID_LINK
//
//  Description:    This function call service to refresh widget
//
//  Note(s)         No link, Always return INVALID_LINK
//
//-------------------------------------------------------------------------------------------------
Link_e CIcon::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_REFRESH;

    if((pService = ServiceCall(&m_pIcon->Service, &m_ServiceState)) != nullptr)        // Invoke the application service for this icon (It might change by itself the m_ServiceState)
    {
        if((m_pIcon->Options & GRAFX_OPTION_DRAW_ONCE) != GRAFX_OPTION_DRAW_ONCE)
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
void CIcon::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;
    if((pService = ServiceCall(&m_pIcon->Service, &m_ServiceState)) != nullptr)
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
//  Description:    Draw the Icon on display according to state.
//
//-------------------------------------------------------------------------------------------------
void CIcon::Draw(ServiceReturn_t* pService)
{
  #ifndef GRAFX_DEBUG_GUI
    Layer_e BackLayerToDraw;
    Layer_e ForeLayerToDraw;
  #endif

    CLayer::PushDrawing();

/*
  #ifdef GRAFX_DEBUG_GUI
    CLayer::SetDrawing(FOREGROUND_DISPLAY_LAYER_0);
  #else
   #ifdef GRAFX_USE_CONSTRUCTION_FOREGROUND_LAYER
    CLayer::SetDrawing(CONSTRUCTION_FOREGROUND_LAYER);
   #else
    CLayer::SetDrawing(FOREGROUND_DISPLAY_LAYER_0);
   #endif
  #endif
*/

 #ifdef GRAFX_DEBUG_GUI
    CLayer::SetDrawing(((m_pIcon->Options & GRAFX_OPTION_DRAW_ON_BACK) != 0) ? BACKGROUND_DISPLAY_LAYER_0 : FOREGROUND_DISPLAY_LAYER_0);
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

    CLayer::SetDrawing(((m_pIcon->Options & GRAFX_OPTION_DRAW_ON_BACK) != 0) ? BackLayerToDraw : ForeLayerToDraw);
  #endif

    if((m_pIcon->Options & GRAFX_OPTION_CLEAR) != 0)
    {
        CLayer::SetColor(BLACK);
        DrawRectangle(&m_pIcon->Box);
    }

    if(pService->ServiceType == SERVICE_RETURN_TYPE5)
    {
        CopyLinear(((ServiceType5_t*)pService)->pAddress,
                   m_pIcon->Box.Pos.X,
                   m_pIcon->Box.Pos.Y,
                   ((ServiceType5_t*)pService)->Size.Width,
                   ((ServiceType5_t*)pService)->Size.Height,
                   ((ServiceType5_t*)pService)->PixelFormat,
                   CLEAR_BLEND);
    }
    else if((m_pIcon->Options & GRAFX_OPTION_BLEND_CLEAR) != 0)
    {
        CopyLinear(m_pIcon->Image.ID_List[pService->IndexState], m_pIcon->Box.Pos, CLEAR_BLEND);
    }
    else
    {
        CopyLinear(m_pIcon->Image.ID_List[pService->IndexState], m_pIcon->Box.Pos, ALPHA_BLEND);
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
void CIcon::DrawOnce(ServiceReturn_t* pService)
{

    CLayer::PushDrawing();

    // Copy merge stuff

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // ICON_DEF
#endif // DIGINI_USE_GRAFX
