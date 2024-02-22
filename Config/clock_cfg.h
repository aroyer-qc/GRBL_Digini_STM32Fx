//-------------------------------------------------------------------------------------------------
//
//  File : clock_cfg.h
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
// Define(s)
//-------------------------------------------------------------------------------------------------

#define CFG_HSE_VALUE                               25000000
#define CFG_HSI_VALUE                               16000000
#define CFG_LSE_VALUE                               32768
#define CFG_LSI_VALUE                               32768
#define CFG_SYSTICK_RATE                            1000

#define CFG_DELAY_TIMING_LOOP_VALUE_FOR_1_USEC      1000 // TODO need to be set and fine tuned

// User clock selection
#define CFG_SYS_PLL_MUX                             CFG_RCC_PLLCFGR_PLLSRC_HSE  // CFG_RCC_PLLCFGR_PLLSRC_HSE or
                                                                                // CFG_RCC_PLLCFGR_PLLSRC_HSI
#define CFG_SYS_CLOCK_MUX                           RCC_CFGR_SW_PLL             // RCC_CFGR_SW_HSI or
                                                                                // RCC_CFGR_SW_HSE or
                                                                                // RCC_CFGR_SW_PLL
// User PLL configuration

// For xxx MHz HCLK on HSI
//#define  CFG_PLLM_SOURCE                          CFG_HSI_VALUE
//#define  CFG_PLLM_DIVIDER                         8
//#define  CFG_PLLM_N_MULTIPLIER                    100
//#define  CFG_PLLM_P_DIVIDER                       2
//#define  CFG_PLLM_Q_DIVIDER                       2
//#define  CFG_PLLM_R_DIVIDER                       2

// For xxx MHz HCLK on HSE
#define CFG_PLLM_SOURCE                             CFG_HSE_VALUE
#define CFG_PLLM_DIVIDER                            25
#define CFG_PLLM_N_MULTIPLIER                       432
#define CFG_PLLM_P_DIVIDER                          2
#define CFG_PLLM_Q_DIVIDER                          9
//#define HSE_PLLM_R_DIVIDER                          2

//#define CORTEX_SYSTEM_TIMER                         LL_SYSTICK_CLKSOURCE_HCLK_DIV8               // LL_SYSTICK_CLKSOURCE_HCLK (/1) or LL_SYSTICK_CLKSOURCE_HCLK_DIV8

// User clock and divider
#define CFG_HCLK_DIVIDER                            CFG_RCC_CFGR_HPRE_DIV1          // (AHB Prescaler) 1,2,4,8,16,64,128,256,512
#define CFG_APB1_CLK_DIVIDER                        CFG_RCC_CFGR_PPRE1_DIV4         // 1,2,4,8,16
#define CFG_APB2_CLK_DIVIDER                        CFG_RCC_CFGR_PPRE2_DIV2         // 1,2,4,8,16

//-------------------------------------------------------------------------------------------------
