//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_virtual_window.cpp
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
#ifdef VIRTUAL_WINDOW_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CVirtualWindow
//
//  Parameter(s):   VirtualWindow_t      pVirtualWindow       Pointer to VirtualWindow_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CVirtualWindow::CVirtualWindow(VirtualWindow_t* pVirtualWindow)
{
    m_pVirtualWindow = pVirtualWindow;
    m_pAddress       = nullptr;
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
//  Note(s)         Return INVALID_LINK
//
//-------------------------------------------------------------------------------------------------
Link_e CVirtualWindow::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;
    if((pService = ServiceCall(&m_pVirtualWindow->Service, &m_ServiceState)) != nullptr)
    {
        if(pService->ServiceType == SERVICE_RETURN_TYPE5)
        {
            m_pAddress    = ((ServiceType5_t*)pService)->Address;
            m_PixelFormat = ((ServiceType5_t*)pService)->PixelFormat;
            Draw();
        }
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
//  Note(s)         Return INVALID_LINK
//
//-------------------------------------------------------------------------------------------------
Link_e CVirtualWindow::Refresh(MsgRefresh_t* pMsg)
{
    Draw();
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
void CVirtualWindow::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;
    if((pService   = ServiceCall(&m_pVirtualWindow->Service, &m_ServiceState)) != nullptr)
    {
        m_pAddress = nullptr;
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
//  Description:    Copy the window to the display.
//
//-------------------------------------------------------------------------------------------------
void CVirtualWindow::Draw()
{
    if(m_pAddress != nullptr)
    {
        CLayer::PushDrawing();
      #ifdef GRAFX_DEBUG_GUI
        CLayer::SetDrawing(BACKGROUND_DISPLAY_LAYER_0);
      #else
        CLayer::SetDrawing(CONSTRUCTION_BACKGROUND_LAYER);
      #endif
        CopyLinear(m_pAddress, &m_pVirtualWindow->Box, m_PixelFormat, CLEAR_BLEND);
        CLayer::PopDrawing();
    }
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
void CVirtualWindow::DrawOnce(ServiceReturn_t* pService)
{

    CLayer::PushDrawing();

    // Copy merge stuff

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // VIRTUAL_WINDOW_DEF
#endif // DIGINI_USE_GRAFX