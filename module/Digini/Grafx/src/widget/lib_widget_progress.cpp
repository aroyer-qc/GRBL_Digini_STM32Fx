//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_progress.cpp
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
#ifdef PROGRESS_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"
//#include "lib_class_database.h"  // TODO should be in lib_digini.h

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CProgress
//
//  Parameter(s):   Progress_t         pProgress         Pointer to Progress_t structure
//
//  Description:    Initialize widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CProgress::CProgress(Progress_t* pProgress)
{
    m_pProgress = pProgress;
    m_Value     = 0;
    m_pProgress->Text.Box.Pos.X  += m_pProgress->Box.Pos.X;
    m_pProgress->Text.Blend = CLEAR_BLEND;
    m_Range = m_pProgress->EndValue - m_pProgress->StartValue;
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
Link_e CProgress::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;

    if((pService = ServiceCall(&m_pProgress->Service, &m_ServiceState)) != nullptr)
    {
        if(pService->ServiceType == SERVICE_RETURN_TYPE1)
        {
            m_Value = ((ServiceType1_t*)pService)->Data;
        }

        DrawOnce(pService);
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
Link_e CProgress::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_REFRESH;

    // Invoke the application service for this Progress (It might change by itself the m_ServiceState)
    if((pService = ServiceCall(&m_pProgress->Service, &m_ServiceState)) != nullptr)
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
//  Name:           Finalize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    This function call service associated with widget to finalize it properly.
//
//-------------------------------------------------------------------------------------------------
void CProgress::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;
    pService = ServiceCall(&m_pProgress->Service, &m_ServiceState);

    if(pService != nullptr)
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
//  Description:    Draw the Progress on display.
//
//-------------------------------------------------------------------------------------------------
void CProgress::Draw(ServiceReturn_t* pService)
{
  #ifndef GRAFX_DEBUG_GUI
    Layer_e BackLayerToDraw;
    Layer_e ForeLayerToDraw;
  #endif
    Cartesian_t  CursorPos;
    int16_t      Position;

    CursorPos.X = m_pProgress->Box.Pos.X;
    CursorPos.Y = m_pProgress->Box.Pos.Y;
    Position = (m_Value * m_pProgress->PixelRange) / m_Range;

// TODO can i put this into a support function
    CLayer::PushDrawing();

  #ifdef GRAFX_DEBUG_GUI
    CLayer::SetDrawing(((m_pProgress->Options & GRAFX_OPTION_DRAW_ON_BACK) != 0) ? BACKGROUND_DISPLAY_LAYER_0 : FOREGROUND_DISPLAY_LAYER_0);
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
        CLayer::SetDrawing(((m_pProgress->Options & GRAFX_OPTION_DRAW_ON_BACK) != 0) ? BackLayerToDraw : ForeLayerToDraw);
    }
  #endif

  // CLayer::SetColor(TRANSPARENT);

    // this erase the old bar...  do we put a option for redraw full widget?
    // DrawRectangle(&m_pProgress->Box);                          // Erase previous draw

  #ifdef GRAFX_DEBUG_GUI
    CLayer::SetColor(LIGHT_RED);
    DrawDebugBox(&m_pProgress->Box);
  #endif

    //CLayer::SetColor(LIGHT_RED);
    //DrawDLine(10, 100, 450, 110);  not working


    if((m_pProgress->Options & GRAFX_OPTION_ORIENTATION_MASK) == GRAFX_OPTION_ORIENTATION_HORIZONTAL)
    {
        CursorPos.X += (Position + m_pProgress->CursorOffset.X);
        CursorPos.Y += m_pProgress->CursorOffset.Y;
    }
    else // Vertical
    {
        CursorPos.X += m_pProgress->CursorOffset.X;
        CursorPos.Y += ((m_pProgress->PixelRange - Position) - m_pProgress->CursorOffset.Y);
    }

    if(m_pProgress->Cursor != INVALID_IMAGE)
    {
        CopyLinear(m_pProgress->Cursor, CursorPos, ALPHA_BLEND);                        // Put cursor if priority is disable
    }

    if(m_pProgress->Bar != INVALID_IMAGE)
    {
        if(m_Value != 0)
        {
            if((m_pProgress->Options & GRAFX_OPTION_ORIENTATION_MASK) == GRAFX_OPTION_ORIENTATION_HORIZONTAL)
            {
                while(m_CursorPos.X != CursorPos.X)
                {
                    CopyLinear(m_pProgress->Bar, m_CursorPos, ALPHA_BLEND);
                    m_CursorPos.X++;
                }
            }
            else // Vertical
            {
                while(m_CursorPos.Y != CursorPos.Y)
                {
                    CopyLinear(m_pProgress->Bar, m_CursorPos, ALPHA_BLEND);
                    m_CursorPos.Y--;
                }
            }
        }
        m_CursorPos = CursorPos;
    }

    WidgetPrint(&m_pProgress->Text, pService);

    #ifdef GRAFX_PAINT_BOX_DEBUG
      if((m_pProgress->Box.Size.Width != 0) && (m_pProgress->Box.Size.Height != 0))
      {
          CLayer::SetColor(GRAFX_PAINT_BOX_DEBUG_COLOR);
          DrawBox(&m_pProgress->Box, 1);
      }
    #endif

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
void CProgress::DrawOnce(ServiceReturn_t* pService)
{

    CLayer::PushDrawing();

    if(m_pProgress->Background != INVALID_IMAGE)
    {
        CopyLinear(m_pProgress->Background, m_pProgress->Box.Pos, ALPHA_BLEND);
    }

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // PROGRESS_DEF
#endif // DIGINI_USE_GRAFX
