//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_graph.cpp
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2021 Alain Royer.
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
#ifdef GRAPH_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CGraph
//
//  Parameter(s):   Graph_t       pGraph         Pointer to Graph_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CGraph::CGraph(Graph_t* pGraph)
{
    m_pGraph = pGraph;
    m_DrawX  = pGraph->Draw_X;          // Initialize X drawing position
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
Link_e CGraph::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;
    
    if((pService = ServiceCall(&m_pGraph->Service, &m_ServiceState)) != nullptr)
    {
        Draw(pService, true);
        FreeServiceStruct(&pService);
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
Link_e CGraph::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_REFRESH;

    if((pService = ServiceCall(&m_pGraph->Service, &m_ServiceState)) != nullptr)        // Invoke the application service for this icon (It might change by itself the m_ServiceState)
    {
        Draw(pService);
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
void CGraph::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;
    pService = ServiceCall(&m_pGraph->Service, &m_ServiceState);
    FreeServiceStruct(&pService);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Draw
//
//  Parameter(s):   ServiceReturn_t* pService
//                  bool             IsItDrawingGrid
//  Return:         None
//
//
//  Description:    Draw the Graph on display according to state.
//
//-------------------------------------------------------------------------------------------------
void CGraph::Draw(ServiceReturn_t* pService, bool IsItDrawingGrid)
{
    uint16_t X;
    uint16_t EndX;
    uint16_t Y;
    
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

/*
Box.Pos.X;
Box.Pos.Y;
Box.Size.Width

struct Graph_t
{
    Service_t      Service;
    Box_t          Box;
    uint32_t       GridColor;               // Grid color
    uint16_t       GridSpacing_X;           // Pixel grid X
    uint16_t       GridSpacing_Y;           // Pixel grid Y
    uint16_t       Scale_X;                 // Scale value for X
    uint16_t       Scale_Y;                 // Scale value for Y
    uint16_t       ScrollStep;              // Number of step for each slot
    uint16_t       Draw_X;                  // Start drawing position
    uint16_t       Options;
};
*/

    // Put here drawing function

    if(IsItDrawingGrid == true)
    {
        EndX = (m_pGraph->Box.Pos.X + )
        
        for(X = m_pGraph->Box.Pos.X; X < EndX; x += m_pGraph->Scale_X)
        // Do iteration to draw the vertical grid

        // Do iteration to draw the horizontal grid
    }

    // Check if its time to shift the graph do it here by copy!!
    {
        // Check if we if we need to add a new vertical line do it..
        {
        }
        
        // Add every horizontal line pixel of the graph
    }

    // Plot the pixel X and Y
    // Update all variables for next iteration

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------

#endif // GRAPH_DEF
#endif // DIGINI_USE_GRAFX
