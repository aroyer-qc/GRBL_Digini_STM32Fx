//-------------------------------------------------------------------------------------------------
//
//  File : system_stm32f7xx.c
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

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "stm32f7xx.h"
#include "clock_cfg.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define RCC_PLLCFGR_RESET_VALUE             0x24003010


// PLL_VCO = (HSE_VALUE / PLL_M) * PLL_N
#define PLL_M      25
#define PLL_N      432

// SYSCLK = PLL_VCO / PLL_P
#define PLL_P      2
// USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ
#define PLL_Q      5

// Vector Table base offset field. This value must be a multiple of 0x200.
#define VECT_TAB_OFFSET  0x00

//uint32_t SystemCoreClock;
//uint32_t APB1_Clock;

//-------------------------------------------------------------------------------------------------
//
//  Name:           SystemInit
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Reset the CPU Setting
//                  Configures : Vector table relocation
//                               System clock source
//                               PLL Multiplier and Divider factors
//                               AHB/APBx prescalers and Flash settings
//
//  Note(s):        This function should be called only once the RCC clock configuration is reset
//                  to the default reset state (done in BSP_SystemReset function).
//
//-------------------------------------------------------------------------------------------------
void SystemInit(void)
{
    // Reset the RCC clock configuration to the default reset state
    SET_BIT(RCC->CR, RCC_CR_HSION);                                         // Set HSION bit
    RCC->CFGR = 0;                                                          // Reset CFGR register
    CLEAR_BIT(RCC->CR, (RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON));       // Reset HSEON, CSSON and PLLON bits
    RCC->PLLCFGR = RCC_PLLCFGR_RESET_VALUE;                                 // Reset PLLCFGR register
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);                                      // Reset HSEBYP bit
    RCC->CIR = 0;                                                           // Disable all interrupts

    // FPU settings
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SET_BIT(SCB->CPACR, ((3UL << 10 * 2) | (3UL << 11 * 2)));               // Set CP10 and CP11 Full Access
  #endif

    //----------------------------- HSE Configuration --------------------------

  #if (SYS_PLL_SOURCE_MUX == RCC_PLLCFGR_PLLSRC_HSE) || (SYS_CLOCK_MUX == RCC_CFGR_SW_HSE)
    SET_BIT(RCC->CR, RCC_CR_HSEON);                                         // Enable the External High Speed oscillator (HSE).
    while((RCC->CR & RCC_CR_HSERDY) == 0);                                  // Wait till HSE is ready
  #endif

    //-------------------------------- Power Control ---------------------------

    // Enable Power Control clock
    RCC->APB1ENR = RCC_APB1ENR_PWREN;

    // Activate the OverDrive to reach the 216 Mhz Frequency
    SET_BIT(PWR->CR1, PWR_CR1_ODEN);
    while((PWR->CSR1 & PWR_CSR1_ODRDY) == 0);
    SET_BIT(PWR->CR1, PWR_CR1_ODSWEN);
    while((PWR->CSR1 & PWR_CSR1_ODSWRDY) == 0);


    // The voltage scaling allows optimizing the power consumption when the device is
    // clocked below the maximum system frequency, to update the voltage scaling value
    // regarding system frequency refer to product datasheet.
    SET_BIT(PWR->CR1, POWER_REGULATOR_CFG);

    //-------------------------------- PLL Configuration -----------------------

  #if (SYS_CLOCK_MUX == RCC_CFGR_SW_PLL)
    // Disable the main PLL.
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);

   #if (SYS_PLL_SOURCE_MUX == RCC_PLLCFGR_PLLSRC_HSE)
    // Configure the main PLL clock source, multiplication and division factors.
    RCC->PLLCFGR = HSE_PLLM_DIVIDER                                           |
                   (HSE_PLLM_N_MULTIPLIER << RCC_PLLCFGR_PLLN_Pos)            |
                   (((HSE_PLLM_P_DIVIDER >> 1) - 1) << RCC_PLLCFGR_PLLP_Pos)  |
                   (SYS_PLL_SOURCE_MUX)                                       |
                    (HSE_PLLM_Q_DIVIDER << RCC_PLLCFGR_PLLQ_Pos);
   #else
    RCC->PLLCFGR = HSI_PLLM_DIVIDER                                           |
                   (HSI_PLLM_N_MULTIPLIER << RCC_PLLCFGR_PLLN_Pos)            |
                   (((HSI_PLLM_P_DIVIDER >> 1) - 1) << RCC_PLLCFGR_PLLP_Pos)  |
                   (SYS_PLL_SOURCE_MUX)                                       |
                    (HSI_PLLM_Q_DIVIDER << RCC_PLLCFGR_PLLQ_Pos);
   #endif

    SET_BIT(RCC->CR, RCC_CR_PLLON);                             // Enable the main PLL.
    while((RCC->CR & RCC_CR_PLLRDY) == 0);                      // Wait till PLL is ready
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_LATENCY_CFG;          // Configure Flash prefetch and wait state
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL);    // Select the main PLL as system clock source
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);      // Wait till the main PLL is used as system clock source
  #endif

    SET_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL);

    // AHB,APB1,APB2 CLOCK
    SET_BIT(RCC->CFGR, (SYS_HCLK_CFG | SYS_APB1_CFG | SYS_APB2_CFG));

    // Configure the Vector Table location add offset address
  #ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM1_BASE | VECT_TAB_OFFSET; // Vector Table Relocation in Internal SRAM
  #else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; // Vector Table Relocation in Internal FLASH
  #endif
}

//-------------------------------------------------------------------------------------------------

