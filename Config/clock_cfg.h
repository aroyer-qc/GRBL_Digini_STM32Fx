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
#define CFG_LSE_VALUE                               32768
#define CFG_SYSTICK_RATE                            1000

#define CFG_DELAY_TIMING_LOOP_VALUE_FOR_1_USEC      1000 // TODO need to be set and fine tuned

// User clock selection
#define CFG_SYS_PLL_MUX                             CFG_RCC_PLLCFGR_PLLSRC_HSE          // CFG_RCC_PLLCFGR_PLLSRC_HSE or
                                                                                        // CFG_RCC_PLLCFGR_PLLSRC_HSI
#define CFG_SYS_CLOCK_MUX                           RCC_CFGR_SW_PLL                     // RCC_CFGR_SW_HSI or
                                                                                        // RCC_CFGR_SW_HSE or
                                                                                        // RCC_CFGR_SW_PLL
// User PLL configuration
// For xxx MHz HCLK on HSE
#define CFG_PLL_SOURCE                              CFG_HSE_VALUE
#define CFG_PLL_M_DIVIDER                           25
#define CFG_PLL_N_MULTIPLIER                        432
#define CFG_PLL_P_DIVIDER                           2
#define CFG_PLL_Q_DIVIDER                           9

// PLLSAI
#define CFG_PLLSAI_N_MULTIPLIER                     192
#define CFG_PLLSAI_P_DIVIDER                        4                                   // 48 MHz for USB
#define CFG_PLLSAI_Q_DIVIDER                        2
#define CFG_PLLSAI_R_DIVIDER                        5                                   // 5 for 9.6 MHz for LTDC

#define CFG_PLLSAI_DIV_Q                            CFG_RCC_DCKCFGR1_PLLSAI_Q_DIV(1)    // External divider from PLLSAI_Q
#define CFG_PLLSAI_DIV_R                            CFG_RCC_DCKCFGR1_PLLSAI_R_DIV4      // External divider from PLLSAI_R. This feed the LCD

// PLLI2S
#define CFG_PLLI2S_N_MULTIPLIER                     100
#define CFG_PLLI2S_P_DIVIDER                        2
#define CFG_PLLI2S_Q_DIVIDER                        2
#define CFG_PLLI2S_R_DIVIDER                        2

#define CFG_PLLI2S_DIV_R                            CFG_RCC_DCKCFGR1_PLLI2S_Q_DIV(1)    // External divider from PLLI2S_Q.

#define CFG_ENABLE_PLLSAI                           1
#define CFG_ENABLE_PLLI2S                           0


//#define CORTEX_SYSTEM_TIMER                         LL_SYSTICK_CLKSOURCE_HCLK_DIV8               // LL_SYSTICK_CLKSOURCE_HCLK (/1) or LL_SYSTICK_CLKSOURCE_HCLK_DIV8

// User clock and divider
#define CFG_HCLK_DIVIDER                            CFG_RCC_CFGR_HPRE_DIV1              // (AHB Prescaler) 1,2,4,8,16,64,128,256,512
#define CFG_APB1_CLK_DIVIDER                        CFG_RCC_CFGR_PPRE1_DIV4             // 1,2,4,8,16
#define CFG_APB2_CLK_DIVIDER                        CFG_RCC_CFGR_PPRE2_DIV2             // 1,2,4,8,16

#define CFG_MCO_1                                   (CFG_RCC_CFGR_MCO1_PLL | CFG_RCC_CFGR_MCO1PRE_DIV5)
#define CFG_MCO_2                                   (CFG_RCC_CFGR_MCO2_HSE | CFG_RCC_CFGR_MCO2PRE_DIV5)

//-------------------------------------------------------------------------------------------------
// multiplexer clock source

// Define the MUX for module that you will need. See lib_STM32F7_system_clock for clock selection option.

#define CFG_CEC_SOURCE_MUX                          CFG_CEC_LSE
#define CFG_CK48M_SOURCE_MUX                        CFG_CK48M_PLLQ
#define CFG_I2C1_SOURCE_MUX                         CFG_I2C1_PCLK1
#define CFG_I2C2_SOURCE_MUX                         CFG_I2C2_PCLK1
#define CFG_I2C3_SOURCE_MUX                         CFG_I2C3_PCLK1
#define CFG_I2C4_SOURCE_MUX                         CFG_I2C4_PCLK1
#define CFG_LPTIM1_SOURCE_MUX                       CFG_LPTIM1_LSE
#define CFG_SAI1_SOURCE_MUX                         CFG_SAI1_PLLSAIQ
#define CFG_SAI2_SOURCE_MUX                         CFG_SAI2_PLLSAIQ
#define CFG_SDMMC1_SOURCE_MUX                       CFG_SDMMC1_PLL48_CLOCK
#define CFG_USART1_SOURCE_MUX                       CFG_USART1_SYS_CLOCK
#define CFG_USART2_SOURCE_MUX                       CFG_USART2_PCLK1
#define CFG_USART3_SOURCE_MUX                       CFG_USART3_PCLK1
#define CFG_UART4_SOURCE_MUX                        CFG_UART4_PCLK1
#define CFG_UART5_SOURCE_MUX                        CFG_UART5_PCLK1
#define CFG_USART6_SOURCE_MUX                       CFG_USART6_PCLK2
#define CFG_UART7_SOURCE_MUX                        CFG_UART7_PCLK1
#define CFG_UART8_SOURCE_MUX                        CFG_UART8_PCLK1
#define CFG_USB_SOURCE_MUX                          CFG_USB_PLLQ

//-------------------------------------------------------------------------------------------------
