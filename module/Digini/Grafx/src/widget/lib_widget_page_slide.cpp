//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_page_slide.cpp
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

// TODO TODO TODO TODO TODO TODO TODO

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "widget_cfg.h"
#ifdef PAGE_SLIDE_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"
#include "lib_class_database.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CPageSlide
//
//  Parameter(s):   PageSlide_t         pPageSlide         Pointer to PageSlide_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CPageSlide::CPageSlide(PageSlide_t* pPageSlide)
{
    m_pPageSlide = pPageSlide;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Create
//
//  Parameter(s):   PageWidget_t* pPageWidget
//  Return:         Link_e        Link
//
//  Description:    This function call service and create the widget
//
//  Note(s)         Return a link or INVALID_LINK
//
//-------------------------------------------------------------------------------------------------
Link_e CPageSlide::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_START;
    m_pPageWidget  = pPageWidget;
    if((pService = ServiceCall(&m_pPageSlide->Service, &m_ServiceState)) != nullptr)
    {
        this->Draw(pService);
        FreeServiceStruct(&pService);
    }

    if((m_pPageSlide->Options & GRAFX_OPTION_DRAW_ONCE_AND_REMOVE) != 0)
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
Link_e CPageSlide::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_REFRESH;

    // Invoke the application service for this background (It might change by itself the m_ServiceState)
    if((pService = ServiceCall(&m_pPageSlide->Service, &m_ServiceState)) != nullptr)
    {
        this->Draw(pService);
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
void CPageSlide::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;

    if((pService = ServiceCall(&m_pPageSlide->Service, &m_ServiceState)) != nullptr)
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
//  Description:    Draw the Background on display according to state.
//
//-------------------------------------------------------------------------------------------------
void CPageSlide::Draw(ServiceReturn_t* pService)
{
    ImageInfo_t ImageInfo;
    CLayer*     pLayer;


    CLayer::PushDrawing();

  #ifdef GRAFX_DEBUG_GUI
        CLayer::SetDrawing(BACKGROUND_DISPLAY_LAYER_0);
        pLayer = &LayerTable[BACKGROUND_DISPLAY_LAYER_0];
  #else
    if(SKIN_pTask->IsSkinLoaded() == true)
    {
        CLayer::SetDrawing(CONSTRUCTION_BACKGROUND_LAYER);
        pLayer = &LayerTable[CONSTRUCTION_BACKGROUND_LAYER];
    }
    else
    {
        CLayer::SetDrawing(BACKGROUND_DISPLAY_LAYER_0);
        pLayer = &LayerTable[BACKGROUND_DISPLAY_LAYER_0];
    }
  #endif

/*
    myGrafx->BlockCopy(ImageInfo.pPointer,
                       m_pPageSlide->Pos.X,
                       m_pPageSlide->Pos.Y,
                       ImageInfo.Size.Width,
                       ImageInfo.Size.Height,
                       m_pPageSlide->Pos.X,
                       m_pPageSlide->Pos.Y,
                       pLayer->GetPixelFormat(),
                       CLEAR_BLEND);
*/

  #ifndef GRAFX_DEBUG_GUI
    if(SKIN_pTask->IsSkinLoaded() == true)
    {
        myGrafx->WaitFor_V_Sync();
        myGrafx->CopyLayerToLayer(CONSTRUCTION_BACKGROUND_LAYER, BACKGROUND_DISPLAY_LAYER_0, 0, 0, GRAFX_SIZE_X, GRAFX_SIZE_Y);
    }
  #endif

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // PAGE_SLIDE_DEF
#endif // DIGINI_USE_GRAFX
