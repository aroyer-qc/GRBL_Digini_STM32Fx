//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_spectrum.cpp
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
#ifdef SPECTRUM_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"
#include "lib_class_database.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CSpectrum
//
//  Parameter(s):   Spectrum_t         pSpectrum         Pointer to Spectrum_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//-------------------------------------------------------------------------------------------------
CSpectrum::CSpectrum(Spectrum_t* pSpectrum)
{
   ImageInfo_t  ImageInfo;

    m_pSpectrum = pSpectrum;
    DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, uint16_t(m_pSpectrum->Bar), 0);
    m_BarSize.Width  = ImageInfo.Size.Width;
    m_BarSize.Height = ImageInfo.Size.Height;
    m_DotSize        = m_BarSize.Height / m_pSpectrum->Resolution.Height;
    m_BarAddress     = ImageInfo.pPointer;
    m_PixelFormat    = ImageInfo.PixelFormat;
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
Link_e CSpectrum::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;
    if((pService = ServiceCall(&m_pSpectrum->Service, &m_ServiceState)) != nullptr)
    {
        Draw(pService);
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
Link_e CSpectrum::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_REFRESH;
    if((pService = ServiceCall(&m_pSpectrum->Service, &m_ServiceState)) != nullptr)
    {
        Draw(pService);
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
void CSpectrum::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;
    pService = ServiceCall(&m_pSpectrum->Service, &m_ServiceState);
    FreeServiceStruct(&pService);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Draw
//
//  Parameter(s):   ServiceReturn_t* pService
//  Return:         None
//
//
//  Description:    Draw the meter on display.
//
//-------------------------------------------------------------------------------------------------
void CSpectrum::Draw(ServiceReturn_t* pService)
{
    int       NumberOfBar;
    uint8_t*  pData;
    uint16_t  Width;
    uint16_t  Height;
    int16_t   PosX;
    int16_t   PosY;

    if(pService->ServiceType == SERVICE_RETURN_TYPE7)
    {
        pData = (uint8_t*)((ServiceType7_t*)pService)->pData;
    }

  #ifdef GRAFX_DEBUG_GUI
    CLayer::SetDrawing(FOREGROUND_DISPLAY_LAYER_0);
  #else
   #ifdef GRAFX_USE_CONSTRUCTION_FOREGROUND_LAYER
    CLayer::SetDrawing(CONSTRUCTION_FOREGROUND_LAYER);
   #else
    CLayer::SetDrawing(FOREGROUND_DISPLAY_LAYER_0);
   #endif
  #endif

    NumberOfBar = m_pSpectrum->Resolution.Width;
    Width       = m_BarSize.Width;
    PosX        = m_pSpectrum->Box.Pos.X;

    CLayer::SetColor(TRANSPARENT);
    DrawRectangle(&m_pSpectrum->Box);

    for(int i = 0; i < NumberOfBar; i++)
    {
        if(i != 0)
        {
            PosX += (m_BarSize.Width + m_pSpectrum->Spacing);
        }
        Height = ((*(pData + i) * m_DotSize));
        PosY   = m_pSpectrum->Box.Pos.Y;
        PosY  += m_BarSize.Height - Height;
        CopyLinear(m_BarAddress, PosX, PosY, Width, Height, m_PixelFormat, CLEAR_BLEND);
    }

    CLayer::PopDrawing();
    FreeServiceStruct(&pService);
}

//-------------------------------------------------------------------------------------------------

#endif // SPECTRUM_DEF
#endif // DIGINI_USE_GRAFX

