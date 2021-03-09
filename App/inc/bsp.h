//-------------------------------------------------------------------------------------------------
//
//  File : bsp.h
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

#include "lib_class_database.h"
#include "nOS.h"
#include "lib_memory.h"
#include "lib_time_typedef.h"
#include "project_def.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define BSP_TICK_PER_SECOND                         1000

//-------------------------------------------------------------------------------------------------
// Global Macro
//-------------------------------------------------------------------------------------------------

#ifdef BSP_GLOBAL
    #define BSP_EXTERN
#else
    #define BSP_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

BSP_EXTERN class CRTC*                      BSP_pRTC;                   // Pointer on the 'Real Time Clock class
//BSP_EXTERN class CTimer*                    BSP_pTimer;


BSP_EXTERN int32_t     TestInput;    // TODO (Alain#1#) remove this

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

void                BSP_Initialize          (void);
void                BSP_IRQ_Init            (void);
SystemState_e       BSP_PostOS_Initialize   (void);
void                BSP_Delay_uSec          (uint32_t Delay);
void                BSP_Delay               (uint32_t Delay);
void                BSP_SetBacklightLevel   (uint8_t Level);

// dummy user function for time until class are develop
void                BSP_GetTime             (Time_t* pData);
void                BSP_SetTime             (Time_t* pData);
void                BSP_GetDate             (Date_t* pData);
void                BSP_SetDate             (Date_t* pData);

//-------------------------------------------------------------------------------------------------




