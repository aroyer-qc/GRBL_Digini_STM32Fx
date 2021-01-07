//-------------------------------------------------------------------------------------------------
//
//  File : lib_define.h
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

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define TickCount_t          nOS_TickCounter

// ---------- General define ----------
#ifdef DEBUG
#define debug                   static
#define debug_point             asm("nop");
#else
#define debug
#define debug_point             (void)
#endif

#ifndef DEF_NO
#define DEF_NO                  0
#endif

#ifndef DEF_YES
#define DEF_YES                 1
#endif

#ifndef DEF_OFF
#define DEF_OFF                 (unsigned char)0
#endif

#ifndef DEF_ON
#define DEF_ON                  (unsigned char)1
#endif

#ifndef DEF_TOGGLE
#define DEF_TOGGLE              (unsigned char)2
#endif

#ifndef DEF_FAIL
#define DEF_FAIL                0
#endif

#ifndef DEF_OK
#define DEF_OK                  1
#endif

#ifndef DEF_LOW
#define DEF_LOW                 0
#endif

#ifndef DEF_HIGH
#define DEF_HIGH                1
#endif

#ifndef DEF_DISABLED
#define DEF_DISABLED            0
#endif

#ifndef DEF_ENABLED
#define DEF_ENABLED             1
#endif

#ifndef DEF_NO
#define DEF_NO                  0
#endif

#ifndef DEF_YES
#define DEF_YES                 1
#endif

#ifndef DEF_DISABLE
#define DEF_DISABLE             0
#endif

#ifndef DEF_ENABLE
#define DEF_ENABLE              1
#endif

#ifndef DEF_FALLING_EDGE
#define DEF_FALLING_EDGE        0
#endif

#ifndef DEF_RISING_EDGE
#define DEF_RISING_EDGE         1
#endif

#ifndef DEF_DIR_IN
#define DEF_DIR_IN              0
#endif

#ifndef DEF_DIR_OUT
#define DEF_DIR_OUT             1
#endif

#ifndef DEF_CLEAR
#define DEF_CLEAR               0
#endif

//#ifndef DEF_ACK
//#define DEF_ACK                 0
//#endif

#ifndef DEF_NACK
#define DEF_NACK                1
#endif

//#ifndef ERR_NONE
//#define ERR_NONE                0
//#endif

#ifndef KHz
#define KHz                     000UL
#endif

#ifndef MHz
#define MHz                     000000UL
#endif

#define keep                    static volatile
