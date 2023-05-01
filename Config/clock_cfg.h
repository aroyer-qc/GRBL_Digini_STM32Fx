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

#define HSE_VALUE                                    25000000
#define HSI_VALUE                                    16000000
#define LSE_VALUE                                    32768
#define LSI_VALUE                                    32768
#define SYSTICK_RATE                                 1000

#define LIB_TIMING_CFG_DELAY_LOOP_VALUE_FOR_1_USEC   1000 // TODO need to be set and fine tuned

// User clock selection
#define SYS_PLL_SOURCE_MUX                          RCC_PLLCFGR_PLLSRC_HSE      // RCC_PLLCFGR_PLLSRC_HSE or
                                                                                // RCC_PLLCFGR_PLLSRC_HSI
#define SYS_CLOCK_MUX                               RCC_CFGR_SW_PLL             // RCC_CFGR_SW_HSI or
                                                                                // RCC_CFGR_SW_HSE or
                                                                                // RCC_CFGR_SW_PLL

#define SYS_CLOCK_PLL_SOURCE

// User PLL configuration

// For xxx MHz HCLK on HSI
#define HSI_PLLM_SOURCE                             HSI_VALUE
#define HSI_PLLM_DIVIDER                            8
#define HSI_PLLM_N_MULTIPLIER                       100
#define HSI_PLLM_P_DIVIDER                          2
#define HSI_PLLM_Q_DIVIDER                          2
//#define HSI_PLLM_R_DIVIDER                          2

// For xxx MHz HCLK on HSE
#define HSE_PLLM_SOURCE                             HSE_VALUE
#define HSE_PLLM_DIVIDER                            25
#define HSE_PLLM_N_MULTIPLIER                       432
#define HSE_PLLM_P_DIVIDER                          2
#define HSE_PLLM_Q_DIVIDER                          9
//#define HSE_PLLM_R_DIVIDER                          2

//#define CORTEX_SYSTEM_TIMER                         LL_SYSTICK_CLKSOURCE_HCLK_DIV8               // LL_SYSTICK_CLKSOURCE_HCLK (/1) or LL_SYSTICK_CLKSOURCE_HCLK_DIV8

// User clock and divider
#define HCLK_DIVIDER                                RCC_CFGR_HPRE_DIV1          // (AHB Prescaler) 1,2,4,8,16,64,128,256,512
#define APB1_CLK_DIVIDER                            RCC_CFGR_PPRE1_DIV4         // 1,2,4,8,16
#define APB2_CLK_DIVIDER                            RCC_CFGR_PPRE2_DIV2         // 1,2,4,8,16

//-------------------------------------------------------------------------------------------------
