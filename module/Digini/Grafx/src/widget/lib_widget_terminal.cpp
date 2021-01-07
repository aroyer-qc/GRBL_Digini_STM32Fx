//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_terminal.cpp
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
#ifdef TERMINAL_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define TERM_MINIMUM_INTERLINE           2
#define TERM_EXTRA_END_OF_LINE_NULL_SZ   1

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CTerminal
//
//  Parameter(s):   Terminal_t           pTerminal         Pointer to Terminal_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------

CTerminal::CTerminal(Terminal_t* pTerminal)
{
    FontInfo_t FontInfo;
    uint32_t   VirtualScreenSize;

    m_pTerminal = pTerminal;

    // Calculate the number of line and starting vertical offset
    DB_Central.Get(&FontInfo, GFX_FONT_INFO, pTerminal->Font);
    m_LineSize = (FontInfo.Interline < TERM_MINIMUM_INTERLINE) ? TERM_MINIMUM_INTERLINE : FontInfo.Interline;
    m_LineSize += FontInfo.Height;
    m_NumberOfLine = (pTerminal->Box.Size.Height / m_LineSize);
    m_OffsetLine   = (pTerminal->Box.Size.Height % m_LineSize) / 2;

    // Number of character per line
    m_NbOfCharPerLine = (pTerminal->Box.Size.Width / FontInfo.Width) + TERM_EXTRA_END_OF_LINE_NULL_SZ;

    // Reserve virtual screen memory and clear it's space
    VirtualScreenSize = m_NumberOfLine * m_NbOfCharPerLine;
    m_pScreen = (uint8_t*)pMemory->AllocAndClear(VirtualScreenSize);

    m_CurrentLine = 0;
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
Link_e CTerminal::Create(PageWidget_t* pPageWidget)
{
  #ifdef DIGINI_USE_POINTING_DEVICE
    EventArea_t      EventArea;
  #endif

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;
  #ifdef DIGINI_USE_POINTING_DEVICE
    EventArea.Rectangle.Box = m_pTerminal->Box;
    PDI_pTask->CreateZone(&EventArea, m_pTerminal->Options, pPageWidget->ID);       // Create the zone on the touch sense virtual screen
  #endif

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
Link_e CTerminal::Refresh(MsgRefresh_t* pMsg)
{
    uint8_t*         pScreenPtr;
    size_t           SizeStr;
    uint32_t         CopySize;
    ServiceReturn_t* pService;

    // If it is our touch zone that is pressed then change service state to process the touch
    GUI_FilterServiceState(&m_ServiceState, pMsg, m_pPageWidget->ID, m_pTerminal->ServiceFilter, SERVICE_IDLE);

    // Invoke the application service for this button (It might change by itself the m_ServiceState)
    if((pService = ServiceCall(&m_pTerminal->Service, &m_ServiceState)) != nullptr)
    {
        if((pService->ServiceType == SERVICE_RETURN) && (m_ServiceState == SERVICE_RELEASED))
        {
            if(m_pPageWidget->Link != INVALID_LINK)
            {
                FreeServiceStruct(&pService);
                return m_pPageWidget->Link;                                                     // Stop here, useless to continue because we will switch screen
            }
        }

        if(pService->ServiceType == SERVICE_RETURN_TYPE3)
        {
            Link_e Link = ((ServiceType3_t*)pService)->Link;

            if(Link != INVALID_LINK)
            {
                FreeServiceStruct(&pService);
                return Link;                                                                    // Stop here, useless to continue because we will switch screen
            }
        }

        //if(m_ServiceState != SERVICE_IDLE)
        {
            if(pService->ServiceType == SERVICE_RETURN_TYPE4)
            {
                if(m_CurrentLine >= m_NumberOfLine)
                {
                    m_CurrentLine--;
                    pScreenPtr = m_pScreen + m_NbOfCharPerLine;
                    CopySize = (m_NumberOfLine * m_NbOfCharPerLine) - m_NbOfCharPerLine;
                    memcpy(m_pScreen, pScreenPtr, CopySize);
                }

                pScreenPtr = m_pScreen + (m_CurrentLine * m_NbOfCharPerLine);                   // Get offset of the line in virtual screen
                SizeStr = strlen((const char*)((ServiceType4_t*)pService)->pString[0]);                      // Check size of print
                memset(pScreenPtr, ' ', m_NbOfCharPerLine - TERM_EXTRA_END_OF_LINE_NULL_SZ);    // Make sure line is clear
                memcpy(pScreenPtr,
                       ((ServiceType4_t*)pService)->pString[0],
                       (SizeStr < m_NbOfCharPerLine) ? SizeStr : (m_NbOfCharPerLine - TERM_EXTRA_END_OF_LINE_NULL_SZ));
                Draw(pService);
                m_CurrentLine++;
            }

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
void CTerminal::Finalize()
{
    pMemory->Free((void**)&m_pScreen);
    ServiceReturn_t* pService;
    m_ServiceState = SERVICE_FINALIZE;

    if((pService = ServiceCall(&m_pTerminal->Service, &m_ServiceState)) != nullptr)
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
void CTerminal::Draw(ServiceReturn_t* pService)
{
    Box_t    Box;
    GPrintf  Printf;
    uint8_t* pScreenPtr;

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

    CLayer::SetColor(BLACK);
    DrawRectangle(&m_pTerminal->Box);

    CLayer::SetColor(m_pTerminal->TextColor);
    FontDefault.Set(m_pTerminal->Font);
    Box.Pos.X       = m_pTerminal->Box.Pos.X;
    Box.Pos.Y       = m_pTerminal->Box.Pos.Y;
    Box.Size.Width  = m_pTerminal->Box.Size.Width;
    Box.Size.Height = m_LineSize;

    for(uint8_t i = 0; i < m_NumberOfLine; i++)
    {
        pScreenPtr = (m_pScreen + ( i * m_NbOfCharPerLine));
        Printf.Draw(&Box, (const char*)pScreenPtr);
        Box.Pos.Y += (m_LineSize + m_OffsetLine);
    }

    // Uncomment to debug and see window size
    //CLayer::SetColor(OPAQUE_RED);
    //DrawBox(&m_pTerminal->Box, 2);

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // TERMINAL_DEF
#endif // DIGINI_USE_GRAFX
