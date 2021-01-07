//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_meter.cpp
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
#ifdef METER_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CMeter
//
//  Parameter(s):   Meter_t         pMeter         Pointer to Meter_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//-------------------------------------------------------------------------------------------------
CMeter::CMeter(Meter_t* pMeter)
{
    m_pMeter = pMeter;
    m_Value  = 0;
    m_pMeter->Text.Blend = ALPHA_BLEND;
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
Link_e CMeter::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;
    if((pService = ServiceCall(&m_pMeter->Service, &m_ServiceState)) != nullptr)
    {
        if(pService->ServiceType == SERVICE_RETURN_TYPE1)
        {
            m_Value = ((ServiceType1_t*)pService)->Data;
        }

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
Link_e CMeter::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_REFRESH;

    // Invoke the application service for this meter (It might change by itself the m_ServiceState)
    if((pService = ServiceCall(&m_pMeter->Service, &m_ServiceState)) != nullptr)
    {
        if(pService->ServiceType == SERVICE_RETURN_TYPE1)
        {
            m_Value = ((ServiceType1_t*)pService)->Data;
        }

        if(pService->ServiceType == SERVICE_RETURN_TYPE2)
        {
            m_Value = ((ServiceType2_t*)pService)->Data[0].u_16;
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
void CMeter::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;

    if((pService = ServiceCall(&m_pMeter->Service, &m_ServiceState)) != nullptr)
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
//  Description:    Draw the meter on display.
//
//-------------------------------------------------------------------------------------------------
void CMeter::Draw(ServiceReturn_t* pService)
{
    uint16_t    Angle;
    uint16_t    EndAngle;
    BlendMode_e  BlendMode;

    BlendMode = ((m_pMeter->Options & OPTION_BLEND_CLEAR) != 0) ? CLEAR_BLEND : ALPHA_BLEND;

    EndAngle = m_pMeter->EndAngle;
    if(EndAngle < m_pMeter->StartAngle) EndAngle += 360;
    Angle = (m_Value * (EndAngle - m_pMeter->StartAngle)) / m_pMeter->Range;

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

     CopyLinear(m_pMeter->Minimum, m_pMeter->Box.Pos, BlendMode);
  //  CursorOnCircle(m_pMeter->Cursor, &m_pMeter->Box.Pos, m_pMeter->Radius, 0);                              // Put a cursor at the start

    if((m_pMeter->Options & OPTION_CURSOR_PRIORITY) != 0)
    {
   VAR_UNUSED(Angle);
  //      CursorOnCircle(m_pMeter->Cursor, &m_pMeter->Box.Pos, m_pMeter->Radius, Angle);                      // Put cursor at angle if priority is enable
    }

 //   Pie(m_pMeter->Maximum, m_pMeter->Box.Pos, m_pMeter->Radius, m_pMeter->StartAngle, Angle, ALPHA_BLEND);  // Merge meter according to value

    if((m_pMeter->Options & OPTION_CURSOR_PRIORITY) == 0)
    {
//        CursorOnCircle(m_pMeter->Cursor, &m_pMeter->Box.Pos, m_pMeter->Radius, Angle);                      // Put cursor at angle if priority is disable
    }
    WidgetPrint(&m_pMeter->Text, pService);
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
void CMeter::DrawOnce(ServiceReturn_t* pService)
{

    CLayer::PushDrawing();

    // Copy merge stuff

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // METER_DEF
#endif // DIGINI_USE_GRAFX
