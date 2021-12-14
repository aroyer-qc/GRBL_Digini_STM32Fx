//-------------------------------------------------------------------------------------------------
//
//  File : clock_cfg.h
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
// Pre define(s)
//-------------------------------------------------------------------------------------------------

#define HSE_VALUE                                    25000000
#define HSI_VALUE                                    16000000
#define LSE_VALUE                                    32768
#define LSI_VALUE                                    32768
#define SYSTICK_RATE                                 1000

#define LIB_TIMING_CFG_DELAY_LOOP_VALUE_FOR_1_USEC   1000 // TODO need to be set and fine tuned

//-------------------------------------------------------------------------------------------------
// Include(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include "stm32f746xx.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define FLASH_LATENCY_0                FLASH_ACR_LATENCY_0WS    //!< FLASH Zero Latency cycle      */
#define FLASH_LATENCY_1                FLASH_ACR_LATENCY_1WS    //*!< FLASH One Latency cycle       */
#define FLASH_LATENCY_2                FLASH_ACR_LATENCY_2WS    //*!< FLASH Two Latency cycles      */
#define FLASH_LATENCY_3                FLASH_ACR_LATENCY_3WS    //*!< FLASH Three Latency cycles    */
#define FLASH_LATENCY_4                FLASH_ACR_LATENCY_4WS    //*/!< FLASH Four Latency cycles     */
#define FLASH_LATENCY_5                FLASH_ACR_LATENCY_5WS    //*!< FLASH Five Latency cycles     */
#define FLASH_LATENCY_6                FLASH_ACR_LATENCY_6WS    //*!< FLASH Six Latency cycles      */
#define FLASH_LATENCY_7                FLASH_ACR_LATENCY_7WS    //*!< FLASH Seven Latency cycles    */
#define FLASH_LATENCY_8                FLASH_ACR_LATENCY_8WS    //*!< FLASH Eight Latency cycles    */
#define FLASH_LATENCY_9                FLASH_ACR_LATENCY_9WS    //*!< FLASH Nine Latency cycles     */
#define FLASH_LATENCY_10               FLASH_ACR_LATENCY_10WS   //*!< FLASH Ten Latency cycles      */
#define FLASH_LATENCY_11               FLASH_ACR_LATENCY_11WS   //*!< FLASH Eleven Latency cycles   */
#define FLASH_LATENCY_12               FLASH_ACR_LATENCY_12WS   //*!< FLASH Twelve Latency cycles   */
#define FLASH_LATENCY_13               FLASH_ACR_LATENCY_13WS   //*!< FLASH Thirteen Latency cycles */
#define FLASH_LATENCY_14               FLASH_ACR_LATENCY_14WS   //*!< FLASH Fourteen Latency cycles */
#define FLASH_LATENCY_15               FLASH_ACR_LATENCY_15WS   //*!< FLASH Fifteen Latency cycles  */

// User clock selection
#define SYS_PLL_SOURCE_MUX                            RCC_PLLCFGR_PLLSRC_HSE                      // RCC_PLLCFGR_PLLSRC_HSE or
                                                                                                // RCC_PLLCFGR_PLLSRC_HSI
#define SYS_CLOCK_MUX                                  RCC_CFGR_SW_PLL                             // RCC_CFGR_SW_HSI or
                                                                                                // RCC_CFGR_SW_HSE or
                                                                                                // RCC_CFGR_SW_PLL

#define SYS_CLOCK_PLL_SOURCE

// User PLL configuration
#if (SYS_CLOCK_MUX == RCC_CFGR_SW_PLL)
  // HSE
  #define HSE_PLLM_SOURCE                             HSE_VALUE
  #define HSE_PLLM_DIVIDER                            25
  #define HSE_PLLM_N_MULTIPLIER                     432
  #define HSE_PLLM_P_DIVIDER                        2
  #define HSE_PLLM_Q_DIVIDER                        5
  #define HSE_PLLM_R_DIVIDER                        2

  // HSI
  #define HSI_PLLM_SOURCE                             HSI_VALUE
  #define HSI_PLLM_DIVIDER                            8
  #define HSI_PLLM_N_MULTIPLIER                     100
  #define HSI_PLLM_P_DIVIDER                        2
  #define HSI_PLLM_Q_DIVIDER                        2
  #define HSI_PLLM_R_DIVIDER                        2
#endif

//#define CORTEX_SYSTEM_TIMER                         LL_SYSTICK_CLKSOURCE_HCLK_DIV8               // LL_SYSTICK_CLKSOURCE_HCLK (/1) or LL_SYSTICK_CLKSOURCE_HCLK_DIV8

// User clock and divider
#define HCLK_DIVIDER                                RCC_CFGR_HPRE_DIV1                         // (AHB Prescaler) 1,2,4,8,16,64,128,256,512
#define APB1_CLK_DIVIDER                               RCC_CFGR_PPRE1_DIV4                        // 1,2,4,8,16
#define APB2_CLK_DIVIDER                               RCC_CFGR_PPRE2_DIV2                        // 1,2,4,8,16

//-------------------------------------------------------------------------------------------------
//
//     W   W    A    RRRR   N   N  III  N   N   GGG
//     W   W  A   A  R   R  NN  N   I   NN  N  G
//     W   W  AAAAA  RRRR   N N N   I   N N N  G  GG
//     W W W  A   A  R  R   N  NN   I   N  NN  G   G
//     W   W  A   A  R   R  N   N  III  N   N   GGG
//
//    WARNING:        Don't modified passed this point, everything else is automatic
//
//-------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------
// Autoconfig value for PLL

#if (SYS_CLOCK_MUX == RCC_CFGR_SW_PLL)

  // -----------------
  // HSI Check
  #define SYS_HSI_PLL_CLK_FREQUENCY                    (((HSI_PLLM_SOURCE / HSI_PLLM_DIVIDER) * HSI_PLLM_N_MULTIPLIER) / HSI_PLLM_P_DIVIDER)
  #define SYS_HSI_PLL_Q_FREQUENCY                    (((HSI_PLLM_SOURCE / HSI_PLLM_DIVIDER) * HSI_PLLM_N_MULTIPLIER) / HSI_PLLM_Q_DIVIDER)
  #define SYS_HSI_PLL_R_FREQUENCY                    (((HSI_PLLM_SOURCE / HSI_PLLM_DIVIDER) * HSI_PLLM_N_MULTIPLIER) / HSI_PLLM_R_DIVIDER)

  #if (HSI_PLLM_DIVIDER < 2) || (HSI_PLLM_DIVIDER > 63)
    #pragma message XSTR(HSI_PLLM_DIVIDER)
    #error PLLM for HSI is out of range
  #else
    #define RCC_PLL_CFGR_HSI_PLLM_CFG               (HSI_PLLM_DIVIDER << RCC_PLLCFGR_PLLM_Pos)
  #endif

  #if (HSI_PLLM_N_MULTIPLIER < 50) || (HSI_PLLM_N_MULTIPLIER > 432)
    #pragma message "XSTR(HSI_PLLM_N_MULTIPLIER)"
    #error PLLN for HSI is out of range
  #else
    #define RCC_PLL_CFGR_HSI_PLLN_CFG               (HSI_PLLM_N_MULTIPLIER << RCC_PLLCFGR_PLLN_Pos)
  #endif

  #if ((HSI_PLLM_P_DIVIDER / 2) < 1) || ((HSI_PLLM_P_DIVIDER / 2) > 4)   // (tested for 2,4,6,8)
    #pragma message "XSTR(HSI_PLLM_P_DIVIDER)"
    #error PLLP for HSI is out of range
  #else
    #define RCC_PLL_CFGR_HSI_PLLP_CFG               (((HSI_PLLM_P_DIVIDER / 2) - 1) << RCC_PLLCFGR_PLLP_Pos)
  #endif

  #if (HSI_PLLM_Q_DIVIDER < 2) || (HSI_PLLM_Q_DIVIDER > 15)
    #pragma message "XSTR(HSI_PLLM_Q_DIVIDER)"
    #error PLLQ for HSI is out of range
  #else
    #define RCC_PLL_CFGR_HSI_PLLQ_CFG               (HSI_PLLM_Q_DIVIDER << RCC_PLLCFGR_PLLQ_Pos)
  #endif

  #if (HSI_PLLM_R_DIVIDER < 2) || (HSI_PLLM_R_DIVIDER > 7)
    #pragma message "XSTR(HSI_PLLM_R_DIVIDER)"
    #error PLLR for HSI is out of range
  #else
    #define RCC_PLL_CFGR_HSI_PLLR_CFG               (HSI_PLLM_R_DIVIDER << RCC_PLLCFGR_PLLR_Pos)
  #endif

  #define RCC_HSI_PLL_CFGR_CFG                      (RCC_PLL_CFGR_HSI_PLLM_CFG |   \
                                                     RCC_PLL_CFGR_HSI_PLLN_CFG |   \
                                                     RCC_PLL_CFGR_HSI_PLLP_CFG |   \
                                                     RCC_PLL_CFGR_HSI_PLLQ_CFG |   \
                                                     RCC_PLL_CFGR_HSI_PLLR_CFG |   \
                                                     RCC_PLLCFGR_PLLSRC_HSI)

  // -----------------
  // HSE Check
  #define SYS_HSE_PLL_CLK_FREQUENCY                    (((HSE_PLLM_SOURCE / HSE_PLLM_DIVIDER) * HSE_PLLM_N_MULTIPLIER) / HSE_PLLM_P_DIVIDER)
  #define SYS_HSE_PLL_Q_FREQUENCY                    (((HSE_PLLM_SOURCE / HSE_PLLM_DIVIDER) * HSE_PLLM_N_MULTIPLIER) / HSE_PLLM_Q_DIVIDER)
  #define SYS_HSE_PLL_R_FREQUENCY                    (((HSE_PLLM_SOURCE / HSE_PLLM_DIVIDER) * HSE_PLLM_N_MULTIPLIER) / HSE_PLLM_R_DIVIDER)

  #if (HSE_PLLM_DIVIDER < 2) || (HSE_PLLM_DIVIDER > 63)
    #pragma message XSTR(HSE_PLLM_DIVIDER)
    #error PLLM for HSE is out of range
  #else
    #define RCC_PLL_CFGR_HSE_PLLM_CFG               (HSE_PLLM_DIVIDER << RCC_PLLCFGR_PLLM_Pos)
  #endif

  #if (HSE_PLLM_N_MULTIPLIER < 50) || (HSE_PLLM_N_MULTIPLIER > 432)
    #pragma message "XSTR(HSE_PLLM_N_MULTIPLIER)"
    #error PLLN for HSE is out of range
  #else
    #define RCC_PLL_CFGR_HSE_PLLN_CFG               (HSE_PLLM_N_MULTIPLIER << RCC_PLLCFGR_PLLN_Pos)
  #endif

  #if ((HSE_PLLM_P_DIVIDER / 2) < 1) || ((HSE_PLLM_P_DIVIDER / 2) > 4)   // (tested for 2,4,6,8)
    #pragma message "XSTR(HSE_PLLM_P_DIVIDER)"
    #error PLLP for HSE is out of range
  #else
    #define RCC_PLL_CFGR_HSE_PLLP_CFG               (((HSE_PLLM_P_DIVIDER / 2) - 1) << RCC_PLLCFGR_PLLP_Pos)
  #endif

  #if (HSE_PLLM_Q_DIVIDER < 2) || (HSE_PLLM_Q_DIVIDER > 15)
    #pragma message "XSTR(HSE_PLLM_Q_DIVIDER)"
    #error PLLQ for HSE is out of range
  #else
    #define RCC_PLL_CFGR_HSE_PLLQ_CFG               (HSE_PLLM_Q_DIVIDER << RCC_PLLCFGR_PLLQ_Pos)
  #endif

  #if (HSE_PLLM_R_DIVIDER < 2) || (HSE_PLLM_R_DIVIDER > 7)
    #pragma message "XSTR(HSE_PLLM_R_DIVIDER)"
    #error PLLR for HSE is out of range
  #else
    #define RCC_PLL_CFGR_HSE_PLLR_CFG               (HSE_PLLM_R_DIVIDER << RCC_PLLCFGR_PLLR_Pos)
  #endif

  #define RCC_HSE_PLL_CFGR_CFG                      (RCC_PLL_CFGR_HSE_PLLM_CFG |   \
                                                     RCC_PLL_CFGR_HSE_PLLN_CFG |   \
                                                     RCC_PLL_CFGR_HSE_PLLP_CFG |   \
                                                     RCC_PLL_CFGR_HSE_PLLQ_CFG |   \
                                                     RCC_PLL_CFGR_HSE_PLLR_CFG |   \
                                                     RCC_PLLCFGR_PLLSRC_HSE)

// do if else for HSE or HSE
    #define SYS_PLL_CLK_FREQUENCY                  SYS_HSE_PLL_CLK_FREQUENCY

#endif // SYS_CLOCK_MUX == RCC_CFGR_SW_PLL

// --------------------------------------------------------------------------------------------------------------------------------

#if (SYS_CLOCK_MUX == RCC_CFGR_SW_PLL)
  #define SYS_CPU_CORE_CLOCK_FREQUENCY              SYS_PLL_CLK_FREQUENCY
#elif (SYS_CLOCK_MUX == RCC_CFGR_SW_HSE)
  #define SYS_CPU_CORE_CLOCK_FREQUENCY              HSE_VALUE
#else //(SYS_CLOCK_MUX == RCC_CFGR_SW_HSI)
  #define SYS_CPU_CORE_CLOCK_FREQUENCY              HSI_VALUE
#endif

// --------------------------------------------------------------------------------------------------------------------------------

#define SYS_HCLK_CFG                                HCLK_DIVIDER

#if   HCLK_DIVIDER == RCC_CFGR_HPRE_DIV1
    #define SYS_HCLK_CLOCK_FREQUENCY                 SYS_CPU_CORE_CLOCK_FREQUENCY
#elif HCLK_DIVIDER == RCC_CFGR_HPRE_DIV2
    #define SYS_HCLK_CLOCK_FREQUENCY                 (SYS_CPU_CORE_CLOCK_FREQUENCY / 2)
#elif HCLK_DIVIDER == RCC_CFGR_HPRE_DIV4
    #define SYS_HCLK_CLOCK_FREQUENCY                 (SYS_CPU_CORE_CLOCK_FREQUENCY / 4)
#elif HCLK_DIVIDER == RCC_CFGR_HPRE_DIV8
    #define SYS_HCLK_CLOCK_FREQUENCY                 (SYS_CPU_CORE_CLOCK_FREQUENCY / 8)
#elif HCLK_DIVIDER == RCC_CFGR_HPRE_DIV16
    #define SYS_HCLK_CLOCK_FREQUENCY                 (SYS_CPU_CORE_CLOCK_FREQUENCY / 16)
#elif HCLK_DIVIDER == RCC_CFGR_HPRE_DIV64
    #define SYS_HCLK_CLOCK_FREQUENCY                 (SYS_CPU_CORE_CLOCK_FREQUENCY / 64)
#elif HCLK_DIVIDER == RCC_CFGR_HPRE_DIV128
    #define SYS_HCLK_CLOCK_FREQUENCY                 (SYS_CPU_CORE_CLOCK_FREQUENCY / 128)
#elif HCLK_DIVIDER == RCC_CFGR_HPRE_DIV256
    #define SYS_HCLK_CLOCK_FREQUENCY                 (SYS_CPU_CORE_CLOCK_FREQUENCY / 256)
#elif HCLK_DIVIDER == RCC_CFGR_HPRE_DIV512
    #define SYS_HCLK_CLOCK_FREQUENCY                 (SYS_CPU_CORE_CLOCK_FREQUENCY / 512)
#endif

#define SYSTEM_CORE_CLOCK                           SYS_HCLK_CLOCK_FREQUENCY

// --------------------------------------------------------------------------------------------------------------------------------

#define SYS_APB1_CFG                                APB1_CLK_DIVIDER

#if   APB1_CLK_DIVIDER == RCC_CFGR_PPRE1_DIV1
    #define SYS_APB1_CLOCK_FREQUENCY                 SYS_HCLK_CLOCK_FREQUENCY
#elif APB1_CLK_DIVIDER == RCC_CFGR_PPRE1_DIV2
    #define SYS_APB1_CLOCK_FREQUENCY                 (SYS_HCLK_CLOCK_FREQUENCY / 2)
#elif APB1_CLK_DIVIDER == RCC_CFGR_PPRE1_DIV4
    #define SYS_APB1_CLOCK_FREQUENCY                 (SYS_HCLK_CLOCK_FREQUENCY / 4)
#elif APB1_CLK_DIVIDER == RCC_CFGR_PPRE1_DIV8
    #define SYS_APB1_CLOCK_FREQUENCY                 (SYS_HCLK_CLOCK_FREQUENCY / 8)
#elif APB1_CLK_DIVIDER == RCC_CFGR_PPRE1_DIV16
    #define SYS_APB1_CLOCK_FREQUENCY                 (SYS_HCLK_CLOCK_FREQUENCY / 16)
#endif

#define SYS_APB1_TIMER_CLOCK_FREQUENCY                (SYS_APB1_CLOCK_FREQUENCY * 2)

// --------------------------------------------------------------------------------------------------------------------------------

#define SYS_APB2_CFG                                APB2_CLK_DIVIDER

#if   APB2_CLK_DIVIDER == RCC_CFGR_PPRE2_DIV1
    #define SYS_APB2_CLOCK_FREQUENCY                 SYS_HCLK_CLOCK_FREQUENCY
#elif APB2_CLK_DIVIDER == RCC_CFGR_PPRE2_DIV2
    #define SYS_APB2_CLOCK_FREQUENCY                 (SYS_HCLK_CLOCK_FREQUENCY / 2)
#elif APB2_CLK_DIVIDER == RCC_CFGR_PPRE2_DIV4
    #define SYS_APB2_CLOCK_FREQUENCY                 (SYS_HCLK_CLOCK_FREQUENCY / 4)
#elif APB2_CLK_DIVIDER == RCC_CFGR_PPRE2_DIV8
    #define SYS_APB2_CLOCK_FREQUENCY                 (SYS_HCLK_CLOCK_FREQUENCY / 8)
#elif APB2_CLK_DIVIDER == RCC_CFGR_PPRE2_DIV16
    #define SYS_APB2_CLOCK_FREQUENCY                 (SYS_HCLK_CLOCK_FREQUENCY / 16)
#endif

#define SYS_APB2_TIMER_CLOCK_FREQUENCY                (SYS_APB2_CLOCK_FREQUENCY * 2)

// --------------------------------------------------------------------------------------------------------------------------------

// Power scaling according to speed
#if (SYS_CPU_CORE_CLOCK_FREQUENCY <= 151000000)
#define POWER_REGULATOR_CFG                         PWR_CR1_VOS_0   // Voltage scale 3
#elif (SYS_CPU_CORE_CLOCK_FREQUENCY <= 180000000)
#define POWER_REGULATOR_CFG                         PWR_CR1_VOS_1   // Voltage scale 2
#elif (SYS_CPU_CORE_CLOCK_FREQUENCY <= 216000000)
#define POWER_REGULATOR_CFG                         PWR_CR1_VOS     // Voltage scale 1
#endif


// Flash Latency configuration for Voltage from 2.7V to 3.6V
#if   (SYS_CPU_CORE_CLOCK_FREQUENCY <= 30000000)
#define FLASH_LATENCY_CFG                           FLASH_ACR_LATENCY_0WS
#elif (SYS_CPU_CORE_CLOCK_FREQUENCY <= 60000000)
#define FLASH_LATENCY_CFG                           FLASH_ACR_LATENCY_1WS
#elif (SYS_CPU_CORE_CLOCK_FREQUENCY <= 90000000)
#define FLASH_LATENCY_CFG                           FLASH_ACR_LATENCY_2WS
#elif (SYS_CPU_CORE_CLOCK_FREQUENCY <= 120000000)
#define FLASH_LATENCY_CFG                           FLASH_ACR_LATENCY_3WS
#elif (SYS_CPU_CORE_CLOCK_FREQUENCY <= 150000000)
#define FLASH_LATENCY_CFG                           FLASH_ACR_LATENCY_4WS
#elif (SYS_CPU_CORE_CLOCK_FREQUENCY <= 180000000)
#define FLASH_LATENCY_CFG                           FLASH_ACR_LATENCY_5WS
#elif (SYS_CPU_CORE_CLOCK_FREQUENCY <= 210000000)
#define FLASH_LATENCY_CFG                           FLASH_ACR_LATENCY_6WS
#elif (SYS_CPU_CORE_CLOCK_FREQUENCY <= 216000000)
#define FLASH_LATENCY_CFG                           FLASH_ACR_LATENCY_7WS
#endif


// Verification
#if SYS_HCLK_CLOCK_FREQUENCY > 216000000
 #pragma message "XSTR(SYS_HCLK_CLOCK_FREQUENCY)"
 #error CPU Core frequency exceed maximum allowed!
#endif

#if SYS_APB1_CLOCK_FREQUENCY > 54000000
 #pragma message "XSTR(SYS_APB1_CLOCK_FREQUENCY)"
 #error APB1 frequency exceed maximum allowed!
#endif

#if SYS_APB2_CLOCK_FREQUENCY > 108000000
 #pragma message "XSTR(SYS_APB2_CLOCK_FREQUENCY)"
 #error APB2 frequency exceed maximum allowed!
#endif



