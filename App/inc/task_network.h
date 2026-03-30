//-------------------------------------------------------------------------------------------------
//
//  File : task_network.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2026 Alain Royer.
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

#if (DIGINI_USE_ETHERNET == DEF_ENABLED)

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

//#define TASK_WEBSERVER_STACK_SIZE            500
//#define TASK_WEBSERVER_PRIO                  6

#define TASK_NETWORK_STACK_SIZE              128
#define TASK_NETWORK_PRIO                    4

#define MQTT_Q_TEST_BUFFER                   4

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class ClassNetwork
{
    public:

                        ClassNetwork                ()                      {};
                       ~ClassNetwork                ()                      {};

        // Task
        void            Network                     (void);
      //void            WebServer                   (void);

        SystemState_e   Initialize                  (void);

        NetworkContext* GetContext                  (void)                       { return &m_NetworkContext; }

      #if (IP_USE_SNTP == DEF_ENABLED)
        void            SetNTP_ResolveIP            (IP_Address_t ResolveIP)     { m_NTP_ResolveIP = ResolveIP; }
      #endif

    private:

      #if (IP_USE_SNTP == DEF_ENABLED)
        static void     DNS_NTP_Callback            (void* pContext, bool Success, IP_Address_t IP);
      #endif

      //void            WebServer_Serve             (void);
      //void            WebServer_DynamicPage       (void);

      //void            TCP_EchoServer_Initialize   (void);
      //rr_t            TCP_EchoServer_Accept       (void* arg, struct tcp_pcb* newpcb, err_t err);

      #if (IP_USE_SNTP == DEF_ENABLED)
        //SNTP_Client                     m_SNTP;                                                 // Simple Network Transport Protocol
        //IP_Address_t                    m_NTP_ResolveIP;
        //bool                            m_NTP_DNS_Resolved;
        //TickCount_t                     m_LastDNS_Request;
      #endif

      #if (IP_USE_SOAP == DEF_ENABLED)
        SOAP_Client                     m_SOAP                                                  // Simple Object Access Protocol
        uint8_t                         m_SOAP_Server_1[IP_MAX_URL_SIZE];                       // Messaging protocol specification for exchanging structured information.
        uint8_t                         m_SOAP_Server_2[IP_MAX_URL_SIZE];
      #endif

      // nOS_Thread                     m_WebServerHandle;
      //nOS_Stack                       m_WebServerStack            [TASK_WEBSERVER_STACK_SIZE]     NOS_STACK_LOCATION;
        nOS_Thread                      m_Handle;
        nOS_Stack                       m_Stack                     [TASK_NETWORK_STACK_SIZE];

        nOS_Queue                       m_MQTT_TestQ_1;
        nOS_Queue                       m_MQTT_TestQ_2;
        nOS_Queue                       m_MQTT_TestQ_3;
        MQTT_Message_t*                 m_pQ_Buffer1[MQTT_Q_TEST_BUFFER];
        MQTT_Message_t*                 m_pQ_Buffer2[MQTT_Q_TEST_BUFFER];
        MQTT_Message_t*                 m_pQ_Buffer3[MQTT_Q_TEST_BUFFER];

        class NetworkContext            m_NetworkContext;
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef TASK_NETWORK_GLOBAL
    class ClassNetwork  TaskNetwork;
    class ClassNetwork* pTaskNetwork = &TaskNetwork;
#else
    extern       class ClassNetwork* pTaskNetwork;
    extern ETH_LinkDriver* pSTM32_LinkDriver;
#endif

//-------------------------------------------------------------------------------------------------

#endif // (DIGINI_USE_ETHERNET == DEF_ENABLED)
