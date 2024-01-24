//-------------------------------------------------------------------------------------------------
//
//  File : ethernet_config.h
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

#pragma once

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define ETH_USED_INTERFACE                      ETH_INTERFACE_RMII

#define ETH_USE_TIME_STAMP                      DEF_DISABLED
#define ETH_USE_CHECKSUM_OFFLOAD                DEF_DISABLED
#define ETH_DEBUG_PACKET_COUNT                  DEF_ENABLED

// MAC address configuration using GUID of the CPU.
#define MAC_ADDR0	                            (((char *)0x1FF0F420)[0])
#define MAC_ADDR1	                            (((char *)0x1FF0F420)[2])
#define MAC_ADDR2	                            (((char *)0x1FF0F420)[4])
#define MAC_ADDR3	                            (((char *)0x1FF0F420)[6])
#define MAC_ADDR4	                            (((char *)0x1FF0F420)[8])
#define MAC_ADDR5	                            (((char *)0x1FF0F420)[10])

#define ETH_IP_ADDR0                            192
#define ETH_IP_ADDR1                            168
#define ETH_IP_ADDR2                            0
#define ETH_IP_ADDR3                            252

#define ETH_SUBNET_MASK_ADDR0                   255
#define ETH_SUBNET_MASK_ADDR1                   255
#define ETH_SUBNET_MASK_ADDR2                   255
#define ETH_SUBNET_MASK_ADDR3                   0

#define ETH_GATEWAY_IP_ADDR0                    192
#define ETH_GATEWAY_IP_ADDR1                    168
#define ETH_GATEWAY_IP_ADDR2                    0
#define ETH_GATEWAY_IP_ADDR3                    1

#define ETH_USE_PHY_LINK_IRQ                    DEF_DISABLED
#define ETH_PHY_LINK_IO                         IO_NOT_DEFINED

#define netifMTU                                1500
#define netifGUARD_BLOCK_TIME			        250
#define ETHERNET_FRAME_SIZE                     1514
#define BLOCK_TIME_WAITING_FOR_INPUT            250             // mSec

#define TASK_ETHERNET_IF_STACK_SIZE             512
#define TASK_ETHERNET_IF_PRIO                   4

// Define those to better describe your network interface.
#define IFNAME0 'd'
#define IFNAME1 'g'
#define IF_NAME "dg0"

// ETH PHY Driver
#define PHY_DRIVER_INCLUDE                      "lib_class_PHY_LAN8742A.h"
#define PHY_DRIVER_INTERFACE                    PHY_LAN8742A_Driver

//-------------------------------------------------------------------------------------------------
