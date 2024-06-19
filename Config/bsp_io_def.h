//-------------------------------------------------------------------------------------------------
//
//  File : bsp_io_def.h
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
// Define(s)
//-------------------------------------------------------------------------------------------------

// in original code
/**
 * Timer 1
 * Outputs 10 KHz on D11
 * Used for Variable Spindle PWM
 **/
/**
 * Timer 2
 * Used for Encoder
 **/
/**
 * Timer 3
 * Used for Input capture
 **/
/**
 * Timer 4
 * Used for Encoder
 **/
/**
 * Timer 9
 * Base clock: 24 MHz
 * Used for Stepper Interrupt
 * On CC1, Main Stepper Interrupt is called
 * On Update, Stepper Port Reset is called
 **/

//-------------------------------------------------------------------------------------------------
//
//          Parameter 1:  This is the ID of the IO pin
//          Parameter 2:  Port for the PIN
//                          GPIOx
//          Parameter 3:  IO number for the PIN
//          Parameter 4:  MODE for the pin
//                          GPIO_MODE_INPUT
//                          GPIO_MODE_OUTPUT
//                          GPIO_MODE_ALTERNATE
//                          GPIO_MODE_ANALOG                   TODO TODO TODO TODO
//          Parameter 5:  TYPE for the pin
//                          GPIO_TYPE_PIN_DRIVE_MASK
//                          GPIO_TYPE_PIN_DRIVE_SHIFT
//                          GPIO_TYPE_PIN_PP
//                          GPIO_TYPE_PIN_OD
//          Parameter 6:  SPEED for the pin
//                          GPIO_SPEED_FREQ_LOW
//                          GPIO_SPEED_FREQ_MEDIUM
//                          GPIO_SPEED_FREQ_HIGH
//                          GPIO_SPEED_FREQ_VERY_HIGH
//          Parameter 7:  EXTRA info for the pin
//                          If mode is GPIO_MODE_INPUT,        This is not used. Put 0 there
//                          If mode is GPIO_MODE_OUTPUT,       This is the default level on the pin at initialization. Put level HALIO_LEVEL_0 or HALIO_LEVEL_1 or HALIO_LEVEL_HIGH_Z
//                          If mode is GPIO_MODE_ALTERNATE,    This is the alternate setting
//                                                               Example: GPIO_AF7_USART3  -  See hal_io.h for your CPU
//                          If mode is GPIO_MODE_ANALOG,       This is used to select the ADC input
//
//-------------------------------------------------------------------------------------------------

#define IO_DEF(X_IO) \
/*        ENUM ID of the IO,    IO Port,    IO Pin, Mode,                 Pin output type,      IO Speed,                   Pin Option        */\
/* Input IO's --------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO( IO_USER_BUTTON,       GPIOI,      11,     IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,  IO_SPEED_FREQ_LOW,        0)                    \
    X_IO( IO_USB1_VBUS,         GPIOJ,      12,     IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,  IO_SPEED_FREQ_LOW,        0)                    \
    X_IO( IO_USB1_OVR,          GPIOD,      4,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,  IO_SPEED_FREQ_LOW,        0)                    \
/* Output IO's -------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO( IO_LED1,              GPIOI,      1,      IO_MODE_OUTPUT,       IO_TYPE_PIN_NO_PULL,  IO_SPEED_FREQ_LOW,        0)                    \
    X_IO( IO_USB1_PWR,          GPIOD,      54,     IO_MODE_OUTPUT,       IO_TYPE_PIN_NO_PULL,  IO_SPEED_FREQ_LOW,        0)                    \
/* QSPI Serial Flash IO's --------------------------------------------------------------------------------------------------------------------*/\
    X_IO( IO_QSPI_CLK,          GPIOB,      2,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,       IO_SPEED_FREQ_MEDIUM,     IO_AF9_QSPI)          \
    X_IO( IO_QSPI_BK1_IO0,      GPIOD,      11,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_NO_PULL,  IO_SPEED_FREQ_MEDIUM,     IO_AF9_QSPI)          \
    X_IO( IO_QSPI_BK1_IO1,      GPIOD,      12,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_NO_PULL,  IO_SPEED_FREQ_MEDIUM,     IO_AF9_QSPI)          \
    X_IO( IO_QSPI_BK1_IO2,      GPIOE,      2,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_NO_PULL,  IO_SPEED_FREQ_MEDIUM,     IO_AF9_QSPI)          \
    X_IO( IO_QSPI_BK1_IO3,      GPIOD,      13,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_NO_PULL,  IO_SPEED_FREQ_MEDIUM,     IO_AF9_QSPI)          \
    X_IO( IO_QSPI_CS,           GPIOB,      6,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,       IO_SPEED_FREQ_HIGH,       IO_AF10_QSPI)         \
/*X_IO( IO_FLASH_RESET,  GPIOC,      12,     IO_MODE_OUTPUT,       GPIO_TYPE_PIN_PP,     IO_SPEED_FREQ_LOW,        0)                */         \
/* I2C IO's ----------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO( IO_I2C1_SCL,          GPIOB,      8,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_OD,       IO_SPEED_FREQ_MEDIUM,     IO_AF4_I2C1)          \
    X_IO( IO_I2C1_SDA,          GPIOB,      9,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_OD,       IO_SPEED_FREQ_MEDIUM,     IO_AF4_I2C1)          \
    X_IO( IO_I2C3_SCL,          GPIOH,      7,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_OD,       IO_SPEED_FREQ_MEDIUM,     IO_AF4_I2C3)          \
    X_IO( IO_I2C3_SDA,          GPIOH,      8,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_OD,       IO_SPEED_FREQ_MEDIUM,     IO_AF4_I2C3)          \
/* UART IO's ---------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO( IO_UART1_RX,          GPIOB,      7,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_NO_PULL,  IO_SPEED_FREQ_MEDIUM,     IO_AF7_USART1)        \
    X_IO( IO_UART1_TX,          GPIOA,      9,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,       IO_SPEED_FREQ_MEDIUM,     IO_AF7_USART1)        \
/* SDRAM IO's --------------------------------------------------------------------------------------------------------------------------------*/\
    /*+--------------------+--------------------+---------------------+---------------------+*/\
    /*+                                SDRAM pins assignment                                +*/\
    /*+--------------------+--------------------+---------------------+---------------------+*/\
    /*| PD0  <-> FMC_D2    | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0     | PG0  <-> FMC_A10    |*/\
    /*| PD1  <-> FMC_D3    | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1     | PG1  <-> FMC_A11    |*/\
    /*| PD8  <-> FMC_D13   | PE7  <-> FMC_D4    | PF2  <-> FMC_A2     | PG4  <-> FMC_BA0    |*/\
    /*| PD9  <-> FMC_D14   | PE8  <-> FMC_D5    | PF3  <-> FMC_A3     | PG5  <-> FMC_BA1    |*/\
    /*| PD10 <-> FMC_D15   | PE9  <-> FMC_D6    | PF4  <-> FMC_A4     | PG8  <-> FMC_SDCLK  |*/\
    /*| PD14 <-> FMC_D0    | PE10 <-> FMC_D7    | PF5  <-> FMC_A5     | PG15 <-> FMC_SDNCAS |*/\
    /*| PD15 <-> FMC_D1    | PE11 <-> FMC_D8    | PF11 <-> FMC_SDNRAS |---------------------+*/\
    /*+--------------------| PE12 <-> FMC_D9    | PF12 <-> FMC_A6     |                      */\
    /*                     | PE13 <-> FMC_D10   | PF13 <-> FMC_A7     |                      */\
    /*                     | PE14 <-> FMC_D11   | PF14 <-> FMC_A8     |                      */\
    /*                     | PE15 <-> FMC_D12   | PF15 <-> FMC_A9     |                      */\
    /*+--------------------+--------------------+---------------------+                      */\
    /*| PC3 <-> FMC_SDCKE0 |                                                                 */\
    /*| PH3 <-> FMC_SDNE0  |                                                                 */\
    /*| PH5 <-> FMC_SDNWE  |                                                                 */\
    /*+--------------------+                                                                 */\
    X_IO( IO_FMC_D0,            GPIOD,      14,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D1,            GPIOD,      15,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D2,            GPIOD,      0,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D3,            GPIOD,      1,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D4,            GPIOE,      7,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D5,            GPIOE,      8,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D6,            GPIOE,      9,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D7,            GPIOE,      10,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D8,            GPIOE,      11,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D9,            GPIOE,      12,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D10,           GPIOE,      13,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D11,           GPIOE,      14,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D12,           GPIOE,      15,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D13,           GPIOD,      8,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D14,           GPIOD,      9,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_D15,           GPIOD,      10,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A0,            GPIOF,      0,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A1,            GPIOF,      1,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A2,            GPIOF,      2,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A3,            GPIOF,      3,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A4,            GPIOF,      4,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A5,            GPIOF,      5,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A6,            GPIOF,      12,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A7,            GPIOF,      13,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A8,            GPIOF,      14,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A9,            GPIOF,      15,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A10,           GPIOG,      0,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_A11,           GPIOG,      1,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_B0,            GPIOG,      4,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_B1,            GPIOG,      5,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_NBL0,          GPIOE,      0,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_NBL1,          GPIOE,      1,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_SNDRAS,        GPIOF,      11,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_SNDCAS,        GPIOG,      15,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_SNDCLK,        GPIOG,      8,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_SDCKE0,        GPIOC,      3,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_SDNE0,         GPIOH,      3,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    X_IO( IO_FMC_SDNWE,         GPIOH,      5,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_FMC)      \
    /* +------------------------+-----------------------+----------------------------+*/\
    /* +                       LCD pins assignment                                   +*/\
    /* +------------------------+-----------------------+----------------------------+*/\
    /* |  LCD_TFT R0 <-> PI.15  |  LCD_TFT G0 <-> PJ.07 |  LCD_TFT B0 <-> PE.04      |*/\
    /* |  LCD_TFT R1 <-> PJ.00  |  LCD_TFT G1 <-> PJ.08 |  LCD_TFT B1 <-> PJ.13      |*/\
    /* |  LCD_TFT R2 <-> PJ.01  |  LCD_TFT G2 <-> PJ.09 |  LCD_TFT B2 <-> PJ.14      |*/\
    /* |  LCD_TFT R3 <-> PJ.02  |  LCD_TFT G3 <-> PJ.10 |  LCD_TFT B3 <-> PJ.15      |*/\
    /* |  LCD_TFT R4 <-> PJ.03  |  LCD_TFT G4 <-> PJ.11 |  LCD_TFT B4 <-> PG.12      |*/\
    /* |  LCD_TFT R5 <-> PJ.04  |  LCD_TFT G5 <-> PK.00 |  LCD_TFT B5 <-> PK.04      |*/\
    /* |  LCD_TFT R6 <-> PJ.05  |  LCD_TFT G6 <-> PK.01 |  LCD_TFT B6 <-> PK.05      |*/\
    /* |  LCD_TFT R7 <-> PJ.06  |  LCD_TFT G7 <-> PK.02 |  LCD_TFT B7 <-> PK.06      |*/\
    /* -------------------------------------------------------------------------------*/\
    /* |  LCD_TFT HSYNC <-> PI.10  | LCDTFT VSYNC <->  PI.09 |                        */\
    /* |  LCD_TFT CLK   <-> PI.14  | LCD_TFT DE   <->  PK.07 |                        */\
    /* -------------------------------------------------------                        */\
    /* |  LCD_DISP      <-> PI.12  |                                                  */\
    /* |  LCD_BL_CTRL   <-> PK.03  |                                                  */\
    /* -----------------------------                                                  */\
    X_IO( IO_LCD_TFT_R0,        GPIOI,      15,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_R1,        GPIOJ,      0,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_R2,        GPIOJ,      1,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_R3,        GPIOJ,      2,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_R4,        GPIOJ,      3,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_R5,        GPIOJ,      4,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_R6,        GPIOJ,      5,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_R7,        GPIOJ,      6,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_G0,        GPIOJ,      7,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_G1,        GPIOJ,      8,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_G2,        GPIOJ,      9,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_G3,        GPIOJ,      10,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_G4,        GPIOJ,      11,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_G5,        GPIOK,      0,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_G6,        GPIOK,      1,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_G7,        GPIOK,      2,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_B0,        GPIOE,      4,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_B1,        GPIOJ,      13,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_B2,        GPIOJ,      14,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_B3,        GPIOJ,      15,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_B4,        GPIOG,      12,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF9_LTDC)      \
    X_IO( IO_LCD_TFT_B5,        GPIOK,      4,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_B6,        GPIOK,      5,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_B7,        GPIOK,      6,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_HSYNC,     GPIOI,      10,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_VSYNC,     GPIOI,      9,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_CLK,       GPIOI,      14,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_DE,        GPIOK,      7,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF14_LTDC)     \
    X_IO( IO_LCD_TFT_DISPLAY,   GPIOI,      12,     IO_MODE_OUTPUT,       IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    0)                \
    X_IO( IO_LCD_TFT_BL_CTRL,   GPIOK,      3,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    0)                \
/* Ethernet IO's -----------------------------------------------------------------------------------------------------------------------------*/\
    X_IO( IO_MX_ETH_MDC,        GPIOC,      1,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_VERY_HIGH,    IO_AF11_ETH)      \
    X_IO( IO_MX_ETH_MDIO,       GPIOA,      2,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_VERY_HIGH,    IO_AF11_ETH)      \
    X_IO( IO_MX_ETH_TXD0,       GPIOG,      13,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_VERY_HIGH,    IO_AF11_ETH)      \
    X_IO( IO_MX_ETH_TXD1,       GPIOG,      14,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_VERY_HIGH,    IO_AF11_ETH)      \
    X_IO( IO_MX_ETH_RXD0,       GPIOC,      4,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_VERY_HIGH,    IO_AF11_ETH)      \
    X_IO( IO_MX_ETH_RXD1,       GPIOC,      5,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_VERY_HIGH,    IO_AF11_ETH)      \
    X_IO( IO_MX_ETH_TX_EN,      GPIOG,      11,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_VERY_HIGH,    IO_AF11_ETH)      \
    X_IO( IO_MX_ETH_RX_ER,      GPIOG,      2,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_VERY_HIGH,    IO_AF11_ETH)      \
    X_IO( IO_MX_ETH_CRS_DV,     GPIOA,      7,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_VERY_HIGH,    IO_AF11_ETH)      \
    X_IO( IO_MX_ETH_REF_CLK,    GPIOA,      1,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_VERY_HIGH,    IO_AF11_ETH)      \
/* SDIO IO's ---------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO( IO_SD_D0,             GPIOC,      8,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_SDMMC1)   \
    X_IO( IO_SD_D1,             GPIOC,      9,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_SDMMC1)   \
    X_IO( IO_SD_D2,             GPIOC,      10,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_SDMMC1)   \
    X_IO( IO_SD_D3,             GPIOC,      11,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_SDMMC1)   \
    X_IO( IO_SD_CLK,            GPIOC,      12,     IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_SDMMC1)   \
    X_IO( IO_SD_CMD,            GPIOD,      2,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_VERY_HIGH,    IO_AF12_SDMMC1)   \
    X_IO( IO_DETECT_SD_CARD,    GPIOC,      13,     IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
/* GRBL IO's ---------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO( IO_ENABLE_DRIVE,      GPIOI,      2,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_STEP_X,            GPIOG,      6,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_STEP_Y,            GPIOB,      5,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_STEP_Z,            GPIOG,      7,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_STEP_A,            GPIOA,      4,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_STEP_B,            GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_STEP_C,            GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIR_X,             GPIOI,      0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIR_Y,             GPIOH,      6,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIR_Z,             GPIOI,      3,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIR_A,             GPIOC,      6,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIR_B,             GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIR_C,             GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_X1,          GPIOA,      15,     IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_Y1,          GPIOA,      8,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_Z1,          GPIOB,      15,     IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_A1,          GPIOG,      9,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_B1,          GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_C1,          GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_X2,          GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_Y2,          GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_Z2,          GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_A2,          GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_B2,          GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_LIMIT_C2,          GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_OUT_1,     GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_OUT_2,     GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_OUT_3,     GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_OUT_4,     GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_OUT_5,     GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_OUT_6,     GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_OUT_7,     GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_OUT_8,     GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_IN_1,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_IN_2,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_IN_3,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_IN_4,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_IN_5,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_IN_6,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_IN_7,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_DIGITAL_IN_8,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_ANALOG_OUT_1,      GPIOA,      5,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_ANALOG_OUT_2,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_ANALOG_OUT_3,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_ANALOG_OUT_4,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_ANALOG_OUT_5,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_ANALOG_OUT_6,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_ANALOG_OUT_7,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_ANALOG_OUT_8,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_ANALOG_OUT_9,      GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_ANALOG_OUT_10,     GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_SPINDLE_ENABLE,    GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_SPINDLE_PWM,       GPIOxx,     0,      IO_MODE_ALTERNATE,    IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          IO_AF1_TIM1)      \
    X_IO( IO_SPINDLE_DIRECTION, GPIOxx,     14,     IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_COOLANT,           GPIOC,      13,     IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_COOLANT_FLOOD,     GPIOF,      8,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_COOLANT_MIST,      GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_PROBE,             GPIOC,      7,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_SAFETY_DOOR,       GPIOxx,     0,      IO_MODE_INPUT,        IO_TYPE_PIN_NO_PULL,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_CONTROL_RESET,     GPIOA,      0,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_CONTROL_FEED,      GPIOF,      10,     IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
    X_IO( IO_CONTROL_START,     GPIOF,      9,      IO_MODE_INPUT,        IO_TYPE_PIN_PULL_UP,    IO_SPEED_FREQ_LOW,          0)                \
/* Misc --------------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO( IO_E2_WP,             GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          1)                \
    X_IO( IO_W5500_RESET,       GPIOxx,     0,      IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          1)                \
    X_IO( IO_DEBUG_PIN,         GPIOG,      13,     IO_MODE_OUTPUT,       IO_TYPE_PIN_PP,         IO_SPEED_FREQ_LOW,          0)                \


//-------------------------------------------------------------------------------------------------
//
//          Parameter 1:  This is the ID of the IO IRQ pin
//
//          Parameter 2:  This is the ID of the IO pin used for this IRQ IO
//                          GPIOx
//
//          Parameter 3:    EXTI0_IRQn                  EXTI Line0 Interrupt
//                          EXTI1_IRQn                  EXTI Line1 Interrupt
//                          EXTI2_IRQn                  EXTI Line2 Interrupt
//                          EXTI3_IRQn                  EXTI Line3 Interrupt
//                          EXTI4_IRQn                  EXTI Line4 Interrupt
//                          EXTI9_5_IRQn                External Line[9:5] Interrupts
//                          EXTI15_10_IRQn              External Line[15:10] Interrupts
//
//          Parameter 4:    IO_EXTI_TRIGGER_RISING
//                          IO_EXTI_TRIGGER_FALLING
//                          IO_EXTI_TRIGGER_RISING_FALLING
//
//-------------------------------------------------------------------------------------------------

#define IO_IRQ_DEF(X_IO_IRQ) \
  X_IO_IRQ(IO_ISR_LIMIT_X1,        IO_LIMIT_X1,        EXTI15_10_IRQn, IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_Y1,        IO_LIMIT_Y1,        EXTI9_5_IRQn,   IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_Z1,        IO_LIMIT_Z1,        EXTI15_10_IRQn, IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_A1,        IO_LIMIT_A1,        EXTI9_5_IRQn,   IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_B1,        IO_LIMIT_B1,        EXTI15_10_IRQn, IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_C1,        IO_LIMIT_C1,        EXTI9_5_IRQn,   IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_X2,        IO_LIMIT_X1,        EXTI15_10_IRQn, IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_Y2,        IO_LIMIT_Y1,        EXTI9_5_IRQn,   IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_Z2,        IO_LIMIT_Z1,        EXTI15_10_IRQn, IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_A2,        IO_LIMIT_A1,        EXTI9_5_IRQn,   IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_B2,        IO_LIMIT_B1,        EXTI15_10_IRQn, IO_EXTI_TRIGGER_RISING_FALLING)  \
  X_IO_IRQ(IO_ISR_LIMIT_C2,        IO_LIMIT_C1,        EXTI9_5_IRQn,   IO_EXTI_TRIGGER_RISING_FALLING)  \

// IO_LIMIT_A is a fake limit switch


//-------------------------------------------------------------------------------------------------
