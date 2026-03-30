//-------------------------------------------------------------------------------------------------
//
//  File : ip_cfg.h
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
// Define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Driver

// Include file for the ETH PHY Driver
#define PHY_DRIVER_INCLUDE                      "./NanoIP/inc/driver/MAC/lib_class_PHY_LAN8742A.h"

// Enable the STM32 Ethernet MAC-to-LinkDriver adapter.
// When enabled, the network stack uses the STM32 MAC + PHY through the ETH_STM32_Adapter to expose a unified ETH_LinkDriver interface to the IP layer.
#define ETH_USE_STM32_ADAPTER                   DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// Debug
#define IP_DBG_DHCP                             DEF_ENABLED
#define IP_DBG_DNS								DEF_DISABLED
#define IP_DBG_ARP								DEF_DISABLED
#define IP_DBG_TCP 								DEF_DISABLED
#define IP_DBG_MQTT								DEF_ENABLED
#define IP_DBG_ARP_RETRY_MSG					DEF_DISABLED

//-------------------------------------------------------------------------------------------------
// Driver configuration  (may vary according to driver)

// for PHY_LAN8742A_Driver

#define ETH_USED_INTERFACE                      ETH_INTERFACE_RMII
#define ETH_USE_TIME_STAMP                      DEF_DISABLED
#define ETH_USE_CHECKSUM_OFFLOAD                DEF_DISABLED
#define ETH_DEBUG_PACKET_COUNT                  DEF_ENABLED
#define ETH_USE_PHY_LINK_IRQ                    DEF_DISABLED
#define ETH_PHY_LINK_IO                         IO_NOT_DEFINED  // Put here the IO ID if supported

//-------------------------------------------------------------------------------------------------
// Protocol supported by the application

#define IP_USE_DHCP							    DEF_DISABLED           // Need UDP
#define IP_USE_DNS							    DEF_DISABLED           // Need UDP
#define IP_USE_HTTP                             DEF_DISABLED
#define IP_USE_ICMP							    DEF_DISABLED
#define IP_USE_MQTT 						    DEF_DISABLED
#define IP_USE_NTP							    DEF_DISABLED
#define IP_USE_SNTP							    DEF_DISABLED
#define IP_USE_SOAP 						    DEF_DISABLED
#define IP_USE_TCP_CLIENT                       DEF_DISABLED
#define IP_USE_TCP_SERVER                       DEF_DISABLED
#define IP_USE_RAW                              DEF_DISABLED

//---------------------------------------------------------
// External server URL

#define IP_DEFAULT_NTP_SERVER_1                 "0.ca.pool.ntp.org"
#define IP_DEFAULT_NTP_SERVER_2                 "1.ca.pool.ntp.org"

//---------------------------------------------------------
// ARP Configuration

#define IP_ARP_TIME_OUT							120
#define IP_ARP_TABLE_SIZE						8                           // how many address in the ARP table

//---------------------------------------------------------
// MQTT Configuration

#define MQTT_BROKER_IP                          IP_ADDRESS(192,168,1,186)
#define MQTT_RX_BUFFER_SIZE                     256
#define MQTT_USE_UNSUBSCRIBE                    DEF_ENABLED

//---------------------------------------------------------
// TCP Configuration

#define TCP_MAX_TX_SEGMENTS                     2
#define TCP_MSS                                 1460
#define TCP_MAX_RETRY                           5

//-------------------------------------------------------------------------------------------------

#define IP_PACKET_Q_SIZE  			            8

#define IP_TCP_MAX_LISTEN                       2

//-------------------------------------------------------------------------------------------------
// Interface configuration

#define IP_IF_WIRED_PROTOCOL                    (IP_FLAG_USE_ARP | IP_FLAG_USE_DHCP | IP_FLAG_USE_ICMP | IP_FLAG_USE_TCP | IP_FLAG_USE_UDP)

// MAC address configuration using GUID of the CPU.
#define MAC_ADDR0                               0x00//(((char*)0x1FF0F420)[0])
#define MAC_ADDR1                               0x19//(((char*)0x1FF0F420)[2])
#define MAC_ADDR2                               0x04//(((char*)0x1FF0F420)[4])
#define MAC_ADDR3                               (((char*)0x1FF0F420)[6])
#define MAC_ADDR4                               (((char*)0x1FF0F420)[8])
#define MAC_ADDR5                               (((char*)0x1FF0F420)[10])
#define IP_MAC_ADDRESS_WIRED                    {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5}


#define SOCKET_MAX_COUNT                        10
#define DNS_MAX_PENDING_COUNT                   4
#define TCP_DEFAULT_WINDOW_SIZE                 HTONS(4 * TCP_MSS)
#define VENDOR_CLASS                            "Digini"

#define IF_ETH_DEF(X_IF) \
/*        ENUM ID       Hostname           Protocol Flag         Default static IP,         Default Gateway,         Default subnet,            Default Static DNS,      MAC Address,          ETH LinkDriver          */ \
/* Interface 1 ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/ \
    X_IF( IF_WIRED,     "LAN1 Digini",     IP_IF_WIRED_PROTOCOL, IP_ADDRESS(192,168,1,199), IP_ADDRESS(192,168,1,1), IP_ADDRESS(255,255,255,0), IP_ADDRESS(192,168,1,1), IP_MAC_ADDRESS_WIRED, pSTM32_LinkDriver       )  \

#define IP_NET_IF_MTU                           1536

//-------------------------------------------------------------------------------------------------
