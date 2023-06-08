//-------------------------------------------------------------------------------------------------
//
//  File : task_network.h
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

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "nOS.h"
#include "lwip/err.h"

//-------------------------------------------------------------------------------------------------
// Global Macro
//-------------------------------------------------------------------------------------------------

#ifdef TASK_NETWORK_GLOBAL
    #define TASK_NETWORK_EXTERN
#else
    #define TASK_NETWORK_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define TASK_WEBSERVER_STACK_SIZE            500
#define TASK_WEBSERVER_PRIO                  6

#define TASK_NETWORK_STACK_SIZE              500
#define TASK_NETWORK_PRIO                    4

//#define TASK_NETWORK_STACK_SIZE              1024
//#define TASK_NETWORK_PRIO                    4

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class ClassNetwork
{
  public:


    // Task
    void            Network                     (void);
    void            WebServer                   (void);



    nOS_Error       Initialize                  (void);


  private:

    void            WebServer_Serve             (void);
    void            WebServer_DynamicPage       (void);

    void            TCP_EchoServer_Initialize   (void);
    err_t           TCP_EchoServer_Accept       (void* arg, struct tcp_pcb* newpcb, err_t err);

    static nOS_Thread      m_WebServerHandle;
    static nOS_Stack       m_WebServerStack     [TASK_WEBSERVER_STACK_SIZE];
    static nOS_Thread      m_NetworkHandle;
    static nOS_Stack       m_NetworkStack       [TASK_NETWORK_STACK_SIZE];

    struct netconn*        m_WebServerConn;
    struct netconn*        m_WebServerNewConn;

};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

TASK_NETWORK_EXTERN class ClassNetwork  Network;

#ifdef TASK_NETWORK_GLOBAL
                 class ClassNetwork* pNetwork = &Network;
#else
    extern       class ClassNetwork* pNetwork;
#endif

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

extern "C" void TaskWebServer_Wrapper       (void* pvParameters);
extern "C" void TaskNetwork_Wrapper         (void* pvParameters);

//-------------------------------------------------------------------------------------------------
