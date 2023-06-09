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

// MAC address configuration.
#define MAC_ADDR0	                            (((char *)0x1FFF7A10)[0])
#define MAC_ADDR1	                            (((char *)0x1FFF7A10)[2])
#define MAC_ADDR2	                            (((char *)0x1FFF7A10)[4])
#define MAC_ADDR3	                            (((char *)0x1FFF7A10)[6])
#define MAC_ADDR4	                            (((char *)0x1FFF7A10)[8])
#define MAC_ADDR5	                            (((char *)0x1FFF7A10)[10])

#define netifMTU                                1500
#define netifGUARD_BLOCK_TIME			        250
#define ETHERNET_FRAME_SIZE                     1514
#define BLOCK_TIME_WAITING_FOR_INPUT            250             // mSec

#define TASK_ETHERNET_IF_STACK_SIZE             512
#define TASK_ETHERNET_IF_PRIO                   4

// Define those to better describe your network interface.
#define IFNAME0 'e'
#define IFNAME1 'n'

//-------------------------------------------------------------------------------------------------
