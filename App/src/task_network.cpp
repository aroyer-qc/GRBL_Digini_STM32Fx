//-------------------------------------------------------------------------------------------------
//
//  File : task_network.cpp
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2023 Alain Royer.
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

#include "lib_digini.h"

#if (DIGINI_USE_ETHERNET == DEF_ENABLED)

#define TASK_NETWORK_GLOBAL
#include "task_network.h"
#undef TASK_NETWORK_GLOBAL

//-------------------------------------------------------------------------------------------------
// Private variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

u32_t nPageHits = 0;

// Format of dynamic web page: the page header
static const unsigned char PAGE_START[] =
{
0x3c,0x21,0x44,0x4f,0x43,0x54,0x59,0x50,0x45,0x20,0x68,0x74,0x6d,0x6c,0x20,0x50,
0x55,0x42,0x4c,0x49,0x43,0x20,0x22,0x2d,0x2f,0x2f,0x57,0x33,0x43,0x2f,0x2f,0x44,
0x54,0x44,0x20,0x48,0x54,0x4d,0x4c,0x20,0x34,0x2e,0x30,0x31,0x2f,0x2f,0x45,0x4e,
0x22,0x20,0x22,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x77,0x33,
0x2e,0x6f,0x72,0x67,0x2f,0x54,0x52,0x2f,0x68,0x74,0x6d,0x6c,0x34,0x2f,0x73,0x74,
0x72,0x69,0x63,0x74,0x2e,0x64,0x74,0x64,0x22,0x3e,0x0d,0x0a,0x3c,0x68,0x74,0x6d,
0x6c,0x3e,0x0d,0x0a,0x3c,0x68,0x65,0x61,0x64,0x3e,0x0d,0x0a,0x20,0x20,0x3c,0x74,
0x69,0x74,0x6c,0x65,0x3e,0x53,0x54,0x4d,0x33,0x32,0x46,0x37,0x78,0x78,0x54,0x41,
0x53,0x4b,0x53,0x3c,0x2f,0x74,0x69,0x74,0x6c,0x65,0x3e,0x0d,0x0a,0x20,0x20,0x3c,
0x6d,0x65,0x74,0x61,0x20,0x68,0x74,0x74,0x70,0x2d,0x65,0x71,0x75,0x69,0x76,0x3d,
0x22,0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x54,0x79,0x70,0x65,0x22,0x0d,0x0a,
0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,0x22,0x74,0x65,0x78,0x74,0x2f,0x68,
0x74,0x6d,0x6c,0x3b,0x20,0x63,0x68,0x61,0x72,0x73,0x65,0x74,0x3d,0x77,0x69,0x6e,
0x64,0x6f,0x77,0x73,0x2d,0x31,0x32,0x35,0x32,0x22,0x3e,0x0d,0x0a,0x20,0x20,0x3c,
0x6d,0x65,0x74,0x61,0x20,0x68,0x74,0x74,0x70,0x2d,0x65,0x71,0x75,0x69,0x76,0x3d,
0x22,0x72,0x65,0x66,0x72,0x65,0x73,0x68,0x22,0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,
0x74,0x3d,0x22,0x31,0x22,0x3e,0x0d,0x0a,0x20,0x20,0x3c,0x6d,0x65,0x74,0x61,0x20,
0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,0x22,0x4d,0x53,0x48,0x54,0x4d,0x4c,0x20,
0x36,0x2e,0x30,0x30,0x2e,0x32,0x38,0x30,0x30,0x2e,0x31,0x35,0x36,0x31,0x22,0x20,
0x6e,0x61,0x6d,0x65,0x3d,0x22,0x47,0x45,0x4e,0x45,0x52,0x41,0x54,0x4f,0x52,0x22,
0x3e,0x0d,0x0a,0x20,0x20,0x3c,0x73,0x74,0x79,0x6c,0x65,0x20,0x3d,0x22,0x66,0x6f,
0x6e,0x74,0x2d,0x77,0x65,0x69,0x67,0x68,0x74,0x3a,0x20,0x6e,0x6f,0x72,0x6d,0x61,
0x6c,0x3b,0x20,0x66,0x6f,0x6e,0x74,0x2d,0x66,0x61,0x6d,0x69,0x6c,0x79,0x3a,0x20,
0x56,0x65,0x72,0x64,0x61,0x6e,0x61,0x3b,0x22,0x3e,0x3c,0x2f,0x73,0x74,0x79,0x6c,
0x65,0x3e,0x0d,0x0a,0x3c,0x2f,0x68,0x65,0x61,0x64,0x3e,0x0d,0x0a,0x3c,0x62,0x6f,
0x64,0x79,0x3e,0x0d,0x0a,0x3c,0x68,0x34,0x3e,0x3c,0x73,0x6d,0x61,0x6c,0x6c,0x20,
0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,0x74,0x2d,0x66,0x61,0x6d,0x69,
0x6c,0x79,0x3a,0x20,0x56,0x65,0x72,0x64,0x61,0x6e,0x61,0x3b,0x22,0x3e,0x3c,0x73,
0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x62,0x69,0x67,0x3e,0x3c,0x62,0x69,0x67,0x3e,0x3c,
0x62,0x69,0x67,0x0d,0x0a,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,
0x74,0x2d,0x77,0x65,0x69,0x67,0x68,0x74,0x3a,0x20,0x62,0x6f,0x6c,0x64,0x3b,0x22,
0x3e,0x3c,0x62,0x69,0x67,0x3e,0x3c,0x73,0x74,0x72,0x6f,0x6e,0x67,0x3e,0x3c,0x65,
0x6d,0x3e,0x3c,0x73,0x70,0x61,0x6e,0x0d,0x0a,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,
0x22,0x66,0x6f,0x6e,0x74,0x2d,0x73,0x74,0x79,0x6c,0x65,0x3a,0x20,0x69,0x74,0x61,
0x6c,0x69,0x63,0x3b,0x22,0x3e,0x53,0x54,0x4d,0x33,0x32,0x46,0x37,0x78,0x78,0x20,
0x4c,0x69,0x73,0x74,0x20,0x6f,0x66,0x20,0x74,0x61,0x73,0x6b,0x73,0x20,0x61,0x6e,
0x64,0x0d,0x0a,0x74,0x68,0x65,0x69,0x72,0x20,0x73,0x74,0x61,0x74,0x75,0x73,0x3c,
0x2f,0x73,0x70,0x61,0x6e,0x3e,0x3c,0x2f,0x65,0x6d,0x3e,0x3c,0x2f,0x73,0x74,0x72,
0x6f,0x6e,0x67,0x3e,0x3c,0x2f,0x62,0x69,0x67,0x3e,0x3c,0x2f,0x62,0x69,0x67,0x3e,
0x3c,0x2f,0x62,0x69,0x67,0x3e,0x3c,0x2f,0x62,0x69,0x67,0x3e,0x3c,0x2f,0x73,0x6d,
0x61,0x6c,0x6c,0x3e,0x3c,0x2f,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x2f,0x68,0x34,
0x3e,0x0d,0x0a,0x3c,0x68,0x72,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x77,0x69,
0x64,0x74,0x68,0x3a,0x20,0x31,0x30,0x30,0x25,0x3b,0x20,0x68,0x65,0x69,0x67,0x68,
0x74,0x3a,0x20,0x32,0x70,0x78,0x3b,0x22,0x3e,0x3c,0x73,0x70,0x61,0x6e,0x0d,0x0a,
0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,0x74,0x2d,0x77,0x65,0x69,
0x67,0x68,0x74,0x3a,0x20,0x62,0x6f,0x6c,0x64,0x3b,0x22,0x3e,0x0d,0x0a,0x3c,0x2f,
0x73,0x70,0x61,0x6e,0x3e,0x3c,0x73,0x70,0x61,0x6e,0x20,0x73,0x74,0x79,0x6c,0x65,
0x3d,0x22,0x66,0x6f,0x6e,0x74,0x2d,0x77,0x65,0x69,0x67,0x68,0x74,0x3a,0x20,0x62,
0x6f,0x6c,0x64,0x3b,0x22,0x3e,0x0d,0x0a,0x3c,0x74,0x61,0x62,0x6c,0x65,0x20,0x73,
0x74,0x79,0x6c,0x65,0x3d,0x22,0x77,0x69,0x64,0x74,0x68,0x3a,0x20,0x39,0x36,0x31,
0x70,0x78,0x3b,0x20,0x68,0x65,0x69,0x67,0x68,0x74,0x3a,0x20,0x33,0x30,0x70,0x78,
0x3b,0x22,0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x3d,0x22,0x31,0x22,0x0d,0x0a,0x20,
0x63,0x65,0x6c,0x6c,0x70,0x61,0x64,0x64,0x69,0x6e,0x67,0x3d,0x22,0x32,0x22,0x20,
0x63,0x65,0x6c,0x6c,0x73,0x70,0x61,0x63,0x69,0x6e,0x67,0x3d,0x22,0x32,0x22,0x3e,
0x0d,0x0a,0x20,0x20,0x3c,0x74,0x62,0x6f,0x64,0x79,0x3e,0x0d,0x0a,0x20,0x20,0x20,
0x20,0x3c,0x74,0x72,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x74,0x64,
0x0d,0x0a,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,0x74,0x2d,0x66,
0x61,0x6d,0x69,0x6c,0x79,0x3a,0x20,0x56,0x65,0x72,0x64,0x61,0x6e,0x61,0x3b,0x20,
0x66,0x6f,0x6e,0x74,0x2d,0x77,0x65,0x69,0x67,0x68,0x74,0x3a,0x20,0x62,0x6f,0x6c,
0x64,0x3b,0x20,0x66,0x6f,0x6e,0x74,0x2d,0x73,0x74,0x79,0x6c,0x65,0x3a,0x20,0x69,
0x74,0x61,0x6c,0x69,0x63,0x3b,0x20,0x62,0x61,0x63,0x6b,0x67,0x72,0x6f,0x75,0x6e,
0x64,0x2d,0x63,0x6f,0x6c,0x6f,0x72,0x3a,0x20,0x72,0x67,0x62,0x28,0x35,0x31,0x2c,
0x20,0x35,0x31,0x2c,0x20,0x32,0x35,0x35,0x29,0x3b,0x20,0x74,0x65,0x78,0x74,0x2d,
0x61,0x6c,0x69,0x67,0x6e,0x3a,0x20,0x63,0x65,0x6e,0x74,0x65,0x72,0x3b,0x22,0x3e,
0x3c,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x61,0x0d,0x0a,0x20,0x68,0x72,0x65,0x66,
0x3d,0x22,0x2f,0x53,0x54,0x4d,0x33,0x32,0x46,0x37,0x78,0x78,0x2e,0x68,0x74,0x6d,
0x6c,0x22,0x3e,0x3c,0x73,0x70,0x61,0x6e,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,
0x63,0x6f,0x6c,0x6f,0x72,0x3a,0x20,0x77,0x68,0x69,0x74,0x65,0x3b,0x22,0x3e,0x48,
0x6f,0x6d,0x65,0x0d,0x0a,0x70,0x61,0x67,0x65,0x3c,0x2f,0x73,0x70,0x61,0x6e,0x3e,
0x3c,0x2f,0x61,0x3e,0x3c,0x2f,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x2f,0x74,0x64,
0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x74,0x64,0x0d,0x0a,0x20,0x73,
0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,0x74,0x2d,0x66,0x61,0x6d,0x69,0x6c,
0x79,0x3a,0x20,0x56,0x65,0x72,0x64,0x61,0x6e,0x61,0x3b,0x20,0x66,0x6f,0x6e,0x74,
0x2d,0x77,0x65,0x69,0x67,0x68,0x74,0x3a,0x20,0x62,0x6f,0x6c,0x64,0x3b,0x20,0x66,
0x6f,0x6e,0x74,0x2d,0x73,0x74,0x79,0x6c,0x65,0x3a,0x20,0x69,0x74,0x61,0x6c,0x69,
0x63,0x3b,0x20,0x62,0x61,0x63,0x6b,0x67,0x72,0x6f,0x75,0x6e,0x64,0x2d,0x63,0x6f,
0x6c,0x6f,0x72,0x3a,0x20,0x72,0x67,0x62,0x28,0x35,0x31,0x2c,0x20,0x35,0x31,0x2c,
0x20,0x32,0x35,0x35,0x29,0x3b,0x20,0x74,0x65,0x78,0x74,0x2d,0x61,0x6c,0x69,0x67,
0x6e,0x3a,0x20,0x63,0x65,0x6e,0x74,0x65,0x72,0x3b,0x22,0x3e,0x3c,0x61,0x0d,0x0a,
0x20,0x68,0x72,0x65,0x66,0x3d,0x22,0x53,0x54,0x4d,0x33,0x32,0x46,0x37,0x78,0x78,
0x41,0x44,0x43,0x2e,0x68,0x74,0x6d,0x6c,0x22,0x3e,0x3c,0x73,0x70,0x61,0x6e,0x20,
0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,0x74,0x2d,0x77,0x65,0x69,0x67,
0x68,0x74,0x3a,0x20,0x62,0x6f,0x6c,0x64,0x3b,0x22,0x3e,0x3c,0x2f,0x73,0x70,0x61,
0x6e,0x3e,0x3c,0x2f,0x61,0x3e,0x3c,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x61,0x0d,
0x0a,0x20,0x68,0x72,0x65,0x66,0x3d,0x22,0x2f,0x53,0x54,0x4d,0x33,0x32,0x46,0x37,
0x78,0x78,0x54,0x41,0x53,0x4b,0x53,0x2e,0x68,0x74,0x6d,0x6c,0x22,0x3e,0x3c,0x73,
0x70,0x61,0x6e,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x63,0x6f,0x6c,0x6f,0x72,
0x3a,0x20,0x77,0x68,0x69,0x74,0x65,0x3b,0x22,0x3e,0x4c,0x69,0x73,0x74,0x0d,0x0a,
0x6f,0x66,0x20,0x74,0x61,0x73,0x6b,0x73,0x3c,0x2f,0x73,0x70,0x61,0x6e,0x3e,0x3c,
0x2f,0x61,0x3e,0x3c,0x2f,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x2f,0x74,0x64,0x3e,
0x0d,0x0a,0x20,0x20,0x20,0x20,0x3c,0x2f,0x74,0x72,0x3e,0x0d,0x0a,0x20,0x20,0x3c,
0x2f,0x74,0x62,0x6f,0x64,0x79,0x3e,0x0d,0x0a,0x3c,0x2f,0x74,0x61,0x62,0x6c,0x65,
0x3e,0x0d,0x0a,0x3c,0x62,0x72,0x3e,0x0d,0x0a,0x3c,0x2f,0x73,0x70,0x61,0x6e,0x3e,
0x3c,0x73,0x70,0x61,0x6e,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,
0x74,0x2d,0x77,0x65,0x69,0x67,0x68,0x74,0x3a,0x20,0x62,0x6f,0x6c,0x64,0x3b,0x22,
0x3e,0x3c,0x2f,0x73,0x70,0x61,0x6e,0x3e,0x3c,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,
0x73,0x70,0x61,0x6e,0x0d,0x0a,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,
0x6e,0x74,0x2d,0x66,0x61,0x6d,0x69,0x6c,0x79,0x3a,0x20,0x56,0x65,0x72,0x64,0x61,
0x6e,0x61,0x3b,0x22,0x3e,0x4e,0x75,0x6d,0x62,0x65,0x72,0x20,0x6f,0x66,0x20,0x70,
0x61,0x67,0x65,0x20,0x68,0x69,0x74,0x73,0x3a,0x0d,0x0a,0x00};

//-------------------------------------------------------------------------------------------------
//
//   Static Variables
//
//-------------------------------------------------------------------------------------------------

nOS_Thread ClassNetwork::m_WebServerHandle;
nOS_Stack  ClassNetwork::m_WebServerStack[TASK_WEBSERVER_STACK_SIZE];
nOS_Thread ClassNetwork::m_NetworkHandle;
nOS_Stack  ClassNetwork::m_NetworkStack[TASK_NETWORK_STACK_SIZE];

//-------------------------------------------------------------------------------------------------
//
//  Name:           TaskNetwork_Wrapper
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//  Description:    main() for the taskNetwork
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
extern "C" void TaskNetwork_Wrapper(void* pvParameters)
{
    (static_cast<ClassNetwork*>(pvParameters))->Network();
}

extern "C" void TaskWebServer_Wrapper(void* pvParameters)
{
    (static_cast<ClassNetwork*>(pvParameters))->WebServer();
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   void
//  Return:         nOS_Error
//
//  Description:    Initialize
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
nOS_Error ClassNetwork::Initialize(void)
{
    nOS_Error Error;

    lwipPlatformDiag("Initializing ClassNetwork\n");

    /*Error = nOS_ThreadCreate(&m_NetworkHandle,
                             TaskNetwork_Wrapper,
                             this,
                             &m_NetworkStack[0],
                             TASK_NETWORK_STACK_SIZE,
                             TASK_NETWORK_PRIO);

    */
    // Webserver task

    /*Error = nOS_ThreadCreate(&m_WebServerHandle,
                             TaskWebServer_Wrapper,
                             this,
                             &m_WebServerStack[0],
                             TASK_WEBSERVER_STACK_SIZE,
                             TASK_WEBSERVER_PRIO);
*/

    // tcp echo server Init
    //TCP_EchoServerInitialize();

  #if (DIGINI_USE_STACKTISTIC == DEF_ENABLED)
memset(&m_NetworkStack[0], 0xFF, TASK_NETWORK_STACK_SIZE * 4);
//memset(&m_WebServerStack[0], 0xFF, TASK_WEBSERVER_STACK_SIZE * 4);

    myStacktistic.Register(&m_NetworkStack[0],   TASK_NETWORK_STACK_SIZE,   "Network");
    myStacktistic.Register(&m_WebServerStack[0], TASK_WEBSERVER_STACK_SIZE, "WEB Server");
  #endif


    //Error = nOS_FlagCreate(&this->m_Flag, 0);
    return Error;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Network
//
//  Parameter(s):   void
//  Return:         void
//
//  Description:    main() loop of Network task for TCPIP, ARP, DHCP
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void ClassNetwork::Network(void)
{
    struct netconn* conn;
    struct netconn* newconn;
    err_t           err;
    err_t           accept_err;
    struct netbuf*  buf;
    void*           data;
    u16_t           len;
    err_t           recv_err;


for(;;)
{ nOS_Sleep(100);}


    // Create a new connection identifier.
    conn = netconn_new_with_proto_and_callback(NETCONN_TCP, 0, nullptr);            // maybe move this and not create the task if ethernet is not working

    if(conn != nullptr)
    {
        // Bind connection to well known port number 7.
        err = netconn_bind(conn, NULL, 7);

        if(err == ERR_OK)
        {
            // Tell connection to go into listening mode.
            netconn_listen(conn);

            for(;;)
            {
                // Grab new connection.
                accept_err = netconn_accept(conn, &newconn);

                // Process the new connection.
                if(accept_err == ERR_OK)
                {
                    while((recv_err = netconn_recv(newconn, &buf)) == ERR_OK)
                    {
                        do
                        {
                            netbuf_data(buf, &data, &len);
                            netconn_write(newconn, data, len, NETCONN_COPY);
                        }
                        while(netbuf_next(buf) >= 0);

                        netbuf_delete(buf);
                    }

                    // Close connection and discard connection identifier.
                    netconn_close(newconn);
                    netconn_delete(newconn);
                }
            }
        }
        else
        {
            netconn_delete(newconn);
            printf(" can not bind TCP netconn");
        }
    }
    else
    {
        printf("can not create TCP netconn");
    }



    #ifdef ETH_IF
        // Initialize W5500
    //argo    Ethernet_Initialize(MAC, &IP, &MyDns, &GatewayIP, &SubnetMask);

        // Initialize TCP server
    //argo    ServerTCP_Initialize(ETH_SOCK, ETH_PORT);
    #endif

 //   for(;;)
  //  {
        /* Read a received packet from the Ethernet buffers and send it
        to the lwIP for handling */
        //ethernetif_input(&gnetif);

  //      nOS_Sleep(1);
  //  }
}

//-------------------------------------------------------------------------------------------------
//
//  WEB SERVER
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           WebServer
//
//  Parameter(s):   void
//  Return:         void
//
//  Description:    HTTP Web Server
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void ClassNetwork::WebServer(void)
{
    err_t  err;
    err_t  accept_err;
for(;;)
{ nOS_Sleep(100);}

    m_WebServerConn = netconn_new(NETCONN_TCP);                                     // Create a new TCP connection handle

    if(m_WebServerConn != nullptr)
    {
        err = netconn_bind(m_WebServerConn, nullptr, 80);                           // Bind to port 80 (HTTP) with default IP address

        if(err == ERR_OK)
        {
            netconn_listen(m_WebServerConn);                                        // Put the connection into LISTEN state

            for(;;)
            {
                accept_err = netconn_accept(m_WebServerConn, &m_WebServerNewConn);  // Accept any incoming connection

                if(accept_err == ERR_OK)
                {
                    WebServer_Serve();                                              // Serve connection
                    netconn_delete(m_WebServerNewConn);                             // Delete connection
                }
            }
        }
    }
}

/**
  * @brief serve tcp connection
  * @param conn: pointer on connection structure
  * @retval None
  */
void ClassNetwork::WebServer_Serve(void)
{
    struct netbuf*  inbuf;
    err_t           recv_err;
    char*           buf;
    u16_t           buflen;
    //struct fs_file file;

    /* Read the data from the port, blocking if nothing yet there. We assume the request (the part we care about) is in one netbuf */
    recv_err = netconn_recv(m_WebServerNewConn, &inbuf);

    if(recv_err == ERR_OK)
    {
        if(netconn_err(m_WebServerNewConn) == ERR_OK)
        {
            netbuf_data(inbuf, (void**)&buf, &buflen);

            /* Is this an HTTP GET command? (only check the first 5 chars, since there are other formats for GET, and we're keeping it very simple )*/
            if((buflen >=5) && (strncmp(buf, "GET /", 5) == 0))
            {
                /* Check if request to get ST.gif */
                if(strncmp((char const *)buf,"GET /STM32F7xx_files/ST.gif",27)==0)
                {
                    //fs_open(&file, "/STM32F7xx_files/ST.gif");
                    //netconn_write(m_WebServerNewConn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
                    //fs_close(&file);
                }
                /* Check if request to get stm32.jpeg */
                else if(strncmp((char const *)buf,"GET /STM32F7xx_files/stm32.jpg",30)==0)
                {
                    //fs_open(&file, "/STM32F7xx_files/stm32.jpg");
                    //netconn_write(m_WebServerNewConn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
                    //fs_close(&file);
                }
                else if (strncmp((char const *)buf,"GET /STM32F7xx_files/logo.jpg", 29) == 0)
                {
                    /* Check if request to get ST logo.jpg */
                    //fs_open(&file, "/STM32F7xx_files/logo.jpg");
                    //netconn_write(m_WebServerNewConn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
                    //fs_close(&file);
                }
                else if(strncmp(buf, "GET /STM32F7xxTASKS.html", 24) == 0)
                {
                    /* Load dynamic page */
                    WebServer_DynamicPage();
                }
                else if((strncmp(buf, "GET /STM32F7xx.html", 19) == 0)||(strncmp(buf, "GET / ", 6) == 0))
                {
                    /* Load STM32F7xx page */
                    //fs_open(&file, "/STM32F7xx.html");
                    //netconn_write(m_WebServerNewConn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
                    //fs_close(&file);
                }
                else
                {
                    /* Load Error page */
                    //fs_open(&file, "/404.html");
                    //netconn_write(m_WebServerNewConn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
                    //fs_close(&file);
                }
            }
        }
    }

    /* Close the connection (server closes in HTTP) */
    netconn_close(m_WebServerNewConn);

    /* Delete the buffer (netconn_recv gives us ownership, so we have to make sure to deallocate the buffer) */
    netbuf_delete(inbuf);
}

/**
  * @brief  Create and send a dynamic Web Page. This page contains the list of
  *         running tasks and the number of page hits.
  * @param  conn pointer on connection structure
  * @retval None
  */
void ClassNetwork::WebServer_DynamicPage(void)
{
    char* pPageBody;
    char* pPageHits;

// TODO Add protection
    pPageBody = (char*)pMemoryPool->AllocAndClear(512);
    pPageHits = (char*)pMemoryPool->AllocAndClear(10);

    // Update the hit count
    nPageHits++;
    sprintf(pPageHits, "%d", (int)nPageHits);
    strcat(pPageBody, pPageHits);
    strcat((char *)pPageBody, "<pre><br>Name          State  Priority  Stack   Num" );
    strcat((char *)pPageBody, "<br>---------------------------------------------<br>");

    // The list of tasks and their status
    //osThreadList((unsigned char *)(pPageBody + strlen(pPageBody)));
    strcat((char *)pPageBody, "<br><br>---------------------------------------------");
    strcat((char *)pPageBody, "<br>B : Blocked, R : Ready, D : Deleted, S : Suspended<br>");

    // Send the dynamically generated page
    netconn_write(m_WebServerNewConn, PAGE_START, strlen((char*)PAGE_START), NETCONN_COPY);
    netconn_write(m_WebServerNewConn, pPageBody, strlen(pPageBody), NETCONN_COPY);

    pMemoryPool->Free((void**)&pPageBody);
    pMemoryPool->Free((void**)&pPageHits);
}

//-------------------------------------------------------------------------------------------------

#endif
