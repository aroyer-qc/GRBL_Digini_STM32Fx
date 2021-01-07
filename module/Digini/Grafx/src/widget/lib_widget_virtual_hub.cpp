//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_virtual_hub.cpp
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
#ifdef VIRTUAL_HUB_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CVirtualHub
//
//  Parameter(s):   VirtualHub_t         pVirtualHub         Pointer to VirtualHub_t structure
//
//  Description:    Initialize widget's service and build widget.
//
//-------------------------------------------------------------------------------------------------
CVirtualHub::CVirtualHub(VirtualHub_t* pVirtualHub)
{
    m_pVirtualHub = pVirtualHub;
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
Link_e CVirtualHub::Create(PageWidget_t* pPageWidget)
{
    ServiceReturn_t* pService;

    m_pPageWidget  = pPageWidget;
    m_ServiceState = SERVICE_START;
    if((pService = ServiceCall(&m_pVirtualHub->Service, &m_ServiceState)) != nullptr)
    {
        if(pService->ServiceType == SERVICE_RETURN)
        {
            Link_e Link = m_pVirtualHub->LinkID[pService->IndexState];
            FreeServiceStruct(&pService);
            return Link;                                                // Stop here, useless to continue because we will switch screen
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
//  Note(s)         No link, Always return INVALID_LINK
//
//-------------------------------------------------------------------------------------------------
Link_e CVirtualHub::Refresh(MsgRefresh_t* pMsg)
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_REFRESH;

    if((pService = ServiceCall(&m_pVirtualHub->Service, &m_ServiceState)) != nullptr)          // Invoke the application service for this VirtualHub (It might change by itself the m_ServiceState)
    {
        if(pService->ServiceType == SERVICE_RETURN)
        {
            Link_e Link = m_pVirtualHub->LinkID[pService->IndexState];
            FreeServiceStruct(&pService);
            return Link;                                                // Stop here, useless to continue because we will switch screen
        }

        if(pService->ServiceType == SERVICE_RETURN_TYPE3)
        {
            Link_e Link = ((ServiceType3_t*)pService)->Link;

            if(Link != INVALID_LINK)
            {
                FreeServiceStruct(&pService);
                return Link;                                                                // Stop here, useless to continue because we will switch screen
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
void CVirtualHub::Finalize()
{
    ServiceReturn_t* pService;

    m_ServiceState = SERVICE_FINALIZE;
    if((pService = ServiceCall(&m_pVirtualHub->Service, &m_ServiceState)) !=nullptr)
    {
        FreeServiceStruct(&pService);
    }
}

//-------------------------------------------------------------------------------------------------

#endif // VIRTUAL_HUB_DEF
#endif // DIGINI_USE_GRAFX
