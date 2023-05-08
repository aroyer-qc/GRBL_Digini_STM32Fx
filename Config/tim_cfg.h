//-------------------------------------------------------------------------------------------------
//
//  File :  tim_cfg.h
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
// Include(s)
//-------------------------------------------------------------------------------------------------

#include "clock_cfg.h"
#include "config.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define TIM_USE_PWM_FEATURE_CFG                         DEF_ENABLED

#define TIM_DRIVER_SUPPORT_TIM1_CFG                     DEF_ENABLED                 // Timer for spindle
#define TIM_DRIVER_SUPPORT_TIM2_CFG                     DEF_ENABLED                 // De bounced limit switch
#define TIM_DRIVER_SUPPORT_TIM3_CFG                     DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM4_CFG                     DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM5_CFG                     DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM6_CFG                     DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM7_CFG                     DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM8_CFG                     DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM9_CFG                     DEF_ENABLED                 // Timer for stepper
#define TIM_DRIVER_SUPPORT_TIM10_CFG                    DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM11_CFG                    DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM12_CFG                    DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM13_CFG                    DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM14_CFG                    DEF_DISABLED
#define TIM_DRIVER_SUPPORT_LPTIM1_CFG                   DEF_DISABLED

// Note:
//       1 - if compare not used, code for retrieving source IRQ will be discarded
//       2 - This setting is effective only TIM_DRIVER_SUPPORT_TIMx_CFG is DEF_ENABLED
//       3 - No compare register for TIM6 and TIM7
//       4 - Even if timer is disable other timer can enable the feature, if code is shared
#define TIM_DRIVER_SUPPORT_TIM1_COMPARE_CFG             DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM2_TO_TIM5_COMPARE_CFG     DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM8_COMPARE_CFG             DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM9_OR_TIM12_COMPARE_CFG    DEF_ENABLED
#define TIM_DRIVER_SUPPORT_TIM10_COMPARE_CFG            DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM11_COMPARE_CFG            DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM13_COMPARE_CFG            DEF_DISABLED
#define TIM_DRIVER_SUPPORT_TIM14_COMPARE_CFG            DEF_DISABLED
#define TIM_DRIVER_SUPPORT_LPTIM1_COMPARE_CFG           DEF_DISABLED

//-------------------------------------------------------------------------------------------------

// Use TIM_OP_MODE_SINGLE or TIM_OP_MODE_REPETITIVE

#define GRBL_SPINDLE                            TIM_DRIVER_ID_1
#define GRBL_SPINDLE_USE_UPDATE_IRQ             false
#define GRBL_SPINDLE_MODE                       TIM_OP_MODE_REPETITIVE_UP_COUNT
#define GRBL_SPINDLE_PRESCALER                  1000
#define GRBL_SPINDLE_RELOAD                     100

#define GRBL_STEPPER                            TIM_DRIVER_ID_9
#define GRBL_STEPPER_USE_UPDATE_IRQ             true
#define GRBL_STEPPER_MODE                       TIM_OP_MODE_REPETITIVE_UP_COUNT
#define GRBL_STEPPER_PRESCALER                  SYS_APB2_TIMER_CLOCK_FREQUENCY / 10 /*(DEBOUNCE_TIME_CHECK)  TODO AR check previous version of GRBL*/  / 100
#define GRBL_STEPPER_RELOAD                     100

#define GRBL_LIMITS_DEBOUNCE                    TIM_DRIVER_ID_2
#define GRBL_LIMITS_DEBOUNCE_USE_UPDATE_IRQ     true
#define GRBL_LIMITS_DEBOUNCE_MODE               TIM_OP_MODE_SINGLE_UP_COUNT
#define GRBL_LIMITS_DEBOUNCE_PRESCALER          SYS_APB2_TIMER_CLOCK_FREQUENCY / (MAX_STEP_RATE_HZ) / 1000
#define GRBL_LIMITS_DEBOUNCE_RELOAD             1000

//-------------------------------------------------------------------------------------------------
