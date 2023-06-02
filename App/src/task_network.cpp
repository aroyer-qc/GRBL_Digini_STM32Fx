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

#define TASK_NETWORK_GLOBAL
#include "task_network.h"
#undef TASK_NETWORK_GLOBAL
#include "lib_digini.h"

#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernetif.h"

//#include "Ethernet.h"      TODO modify for lwip and lan8742...
//#include "librairiGrIP.h"
//#include "ServerTCP.h"

//-------------------------------------------------------------------------------------------------
// Private variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef ETH_IF
    uint8_t MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

    // IP Address of tcp server
    IPAddress_t IP         = {{192, 168, 1, 20}};
    IPAddress_t GatewayIP  = {{192, 168, 1, 1}};
    IPAddress_t SubnetMask = {{255, 255, 255, 0}};
    IPAddress_t MyDns      = {{8, 8, 8, 8}};
#endif

struct netif gnetif;


static void Netif_Config(void);

//-------------------------------------------------------------------------------------------------
//
//   Static Variables
//
//-------------------------------------------------------------------------------------------------

nOS_Thread ClassTaskNetwork::m_Handle;
nOS_Stack  ClassTaskNetwork::m_Stack[TASK_NETWORK_STACK_SIZE];

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
    (static_cast<ClassTaskNetwork*>(pvParameters))->Run();
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
nOS_Error ClassTaskNetwork::Initialize(void)
{
    nOS_Error Error;

    Error = nOS_ThreadCreate(&m_Handle,
                             TaskNetwork_Wrapper,
                             this,
                             &m_Stack[0],
                             TASK_NETWORK_STACK_SIZE,
                             TASK_NETWORK_PRIO);

  // Initialize the LwIP stack
  lwip_init();

  // Configure the Network interface
  Netif_Config();

  // tcp echo server Init
//  udp_echoserver_init();


    //Error = nOS_FlagCreate(&this->m_Flag, 0);
    return Error;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Run
//
//  Parameter(s):   void
//  Return:         void
//
//  Description:    main() loop of Network task
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void ClassTaskNetwork::Run(void)
{
    #ifdef ETH_IF
        // Initialize W5500
    //argo    Ethernet_Initialize(MAC, &IP, &MyDns, &GatewayIP, &SubnetMask);

        // Initialize TCP server
    //argo    ServerTCP_Initialize(ETH_SOCK, ETH_PORT);
    #endif

    for(;;)
    {
        /* Read a received packet from the Ethernet buffers and send it
        to the lwIP for handling */
        //ethernetif_input(&gnetif);

        nOS_Sleep(1);
    }
}


/**
  * @brief  Configurates the network interface
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

#if LWIP_DHCP
  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else

  /* IP address default setting */
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

#endif

  /* add the network interface */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /*  Registers the default network interface */
  netif_set_default(&gnetif);

 // ethernet_link_status_updated(&gnetif);

#if LWIP_NETIF_LINK_CALLBACK
  netif_set_link_callback(&gnetif, ethernet_link_status_updated);
#endif
}
