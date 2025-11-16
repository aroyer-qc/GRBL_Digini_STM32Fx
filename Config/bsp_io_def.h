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
//          IO_CFG_DEF(X_IO_CFG)
//
//          This is common configuration for IO_DEF pin declaration.
//          It help reduce code size by sharing configuration.
//
//          Parameter 1:  ENUM ID of the configuration
//
//          Parameter 2:  MODE for the pin
//                          IO_MODE_INPUT
//                          IO_MODE_OUTPUT
//                          IO_MODE_ALTERNATE
//                          IO_MODE_ANALOG
//          Parameter 3:  TYPE for the pin
//                          IO_TYPE_PIN_DRIVE_MASK
//                          IO_TYPE_PIN_DRIVE_SHIFT
//                          IO_TYPE_PIN_PP
//                          IO_TYPE_PIN_OD
//          Parameter 4:  SPEED for the pin
//                          IO_SPEED_FREQ_LOW
//                          IO_SPEED_FREQ_MEDIUM
//                          IO_SPEED_FREQ_HIGH
//                          IO_SPEED_FREQ_VERY_HIGH
//          Parameter 5:  EXTRA info for the pin
//                          If mode is IO_MODE_INPUT,        This is not used. Put 0 there
//                          If mode is IO_MODE_OUTPUT,       This is the default level on the pin at initialization. Put level 0 or 1
//                          If mode is IO_MODE_ALTERNATE,    This is the alternate setting
//                                                               Example: GPIO_AF7_USART3  -  See lib_STM3F4_io.h
//                          If mode is IO_MODE_ANALOG,       This is used to select the ADC input               // to be validated!!!
//
//-------------------------------------------------------------------------------------------------

#define IO_CFG_DEF(X_IO_CFG) \
/*            ENUM ID IO CFG,                           Mode,               Pin type,              IO Speed,                 Pin Option           */\
/* input IO's CFG --------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO_CFG( IO_CFG_INPUT_NP_LS,                       IO_MODE_INPUT,      IO_TYPE_PIN_NO_PULL,   IO_SPEED_FREQ_LOW,        0)                     \
    X_IO_CFG( IO_CFG_INPUT_PU_LS,                       IO_MODE_INPUT,      IO_TYPE_PIN_PULL_UP,   IO_SPEED_FREQ_LOW,        0)                     \
/* Output IO's CFG -------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO_CFG( IO_CFG_OUTPUT_PP_LS_DEF0,                 IO_MODE_OUTPUT,     IO_TYPE_PIN_PP,        IO_SPEED_FREQ_LOW,        0)                     \
    X_IO_CFG( IO_CFG_OUTPUT_PP_LS_DEF1,                 IO_MODE_OUTPUT,     IO_TYPE_PIN_PP,        IO_SPEED_FREQ_LOW,        1)                     \
/* ETH IO's CFG ----------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO_CFG( IO_CFG_ETH_AF11,                          IO_MODE_ALTERNATE,  IO_TYPE_PIN_PP,        IO_SPEED_FREQ_VERY_HIGH,  IO_AF11_ETH)           \
/* UART IO's CFG ---------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO_CFG( IO_CFG_UART_TX_AF7,                       IO_MODE_ALTERNATE,  IO_TYPE_PIN_PP,        IO_SPEED_FREQ_MEDIUM,     IO_AF7_USART1)         \
    X_IO_CFG( IO_CFG_UART_RX_AF7,                       IO_MODE_ALTERNATE,  IO_TYPE_PIN_NO_PULL,   IO_SPEED_FREQ_MEDIUM,     IO_AF7_USART1)         \
/* I2C IO's CFG ----------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO_CFG( IO_CFG_I2C_AF4,                           IO_MODE_ALTERNATE,  IO_TYPE_PIN_OD,        IO_SPEED_FREQ_MEDIUM,     IO_AF4_I2C)            \
/* LCD IO's CFG ----------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO_CFG( IO_CFG_LCD_AF9,                           IO_MODE_ALTERNATE,  IO_TYPE_PIN_PULL_UP,   IO_SPEED_FREQ_VERY_HIGH,  IO_AF9_LTDC)           \
    X_IO_CFG( IO_CFG_LCD_AF14,                          IO_MODE_ALTERNATE,  IO_TYPE_PIN_PULL_UP,   IO_SPEED_FREQ_VERY_HIGH,  IO_AF14_LTDC)          \
/* QSPI IO's CFG ---------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO_CFG( IO_CFG_QSPI_NP_AF9,                       IO_MODE_ALTERNATE,  IO_TYPE_PIN_NO_PULL,   IO_SPEED_FREQ_VERY_HIGH,  IO_AF9_QSPI)           \
    X_IO_CFG( IO_CFG_QSPI_PP_AF9,                       IO_MODE_ALTERNATE,  IO_TYPE_PIN_PP,        IO_SPEED_FREQ_VERY_HIGH,  IO_AF9_QSPI)           \
    X_IO_CFG( IO_CFG_QSPI_PP_AF10,                      IO_MODE_ALTERNATE,  IO_TYPE_PIN_PP,        IO_SPEED_FREQ_HIGH,       IO_AF10_QSPI)          \
/* SDMMC IO's CFG --------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO_CFG( IO_CFG_SDMMC_AF12,                        IO_MODE_ALTERNATE,  IO_TYPE_PIN_PULL_UP,   IO_SPEED_FREQ_VERY_HIGH,  IO_AF12_SDMMC1)        \
/* SDRAM IO's CFG --------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO_CFG( IO_CFG_SDRAM_AF12,                        IO_MODE_ALTERNATE,  IO_TYPE_PIN_PULL_UP,   IO_SPEED_FREQ_VERY_HIGH,  IO_AF12_FMC)           \
/* TIM IO's CFG ----------------------------------------------------------------------------------------------------------------------------------*/\
    X_IO_CFG( IO_CFG_TIM1_PP_LS,                        IO_MODE_ALTERNATE,  IO_TYPE_PIN_PP,        IO_SPEED_FREQ_LOW,        IO_AF1_TIM)            \
/* -----------------------------------------------------------------------------------------------------------------------------------------------*/


//-------------------------------------------------------------------------------------------------
//
//          IO_DEF(X_IO)
//
//          Parameter 1:  This is the ID of the IO pin
//          Parameter 2:  Port for the PIN
//                          GPIOx
//          Parameter 3:  IO number for the PIN
//          Parameter 4:  IO CFG to used for this pin
//
//-------------------------------------------------------------------------------------------------

#define IO_DEF(X_IO) \
/* ---------------------------------------------------------------------------------------------*/\
/*        ENUM ID of the IO,    IO Port,    IO Pin, 	IO Config                               */\
/* Input IO's ----------------------------------------------------------------------------------*/\
    X_IO( IO_USER_BUTTON,       GPIOI,      IO_PIN_11,  IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_USB1_VBUS,         GPIOJ,      IO_PIN_12,  IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_USB1_OVR,          GPIOD,      IO_PIN_4,   IO_CFG_INPUT_NP_LS)                       \
/* Output IO's ---------------------------------------------------------------------------------*/\
    X_IO( IO_LED1,              GPIOI,      IO_PIN_1,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_USB1_PWR,          GPIOD,      IO_PIN_5,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
/* QSPI Serial Flash IO's ----------------------------------------------------------------------*/\
/*   X_IO( IO_FLASH_RESET,      GPIOC,      IO_PIN_12,  IO_CFG_OUTPUT_PP_LS_DEF0)               */\
/* UART IO's -----------------------------------------------------------------------------------*/\
    X_IO( IO_UART1_RX,          GPIOB,      IO_PIN_7,   IO_CFG_UART_RX_AF7)                       \
    X_IO( IO_UART1_TX,          GPIOA,      IO_PIN_9,   IO_CFG_UART_TX_AF7)                       \
/* SD Card IO's --------------------------------------------------------------------------------*/\
    X_IO( IO_DETECT_SD_CARD,    GPIOC,      IO_PIN_13,  IO_CFG_INPUT_PU_LS)                       \
/* LCD IO's ------------------------------------------------------------------------------------*/\
    X_IO( IO_LCD_TFT_DISPLAY,   GPIOI,      IO_PIN_12,  IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_LCD_TFT_BL_CTRL,   GPIOK,      IO_PIN_3,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
/* GRBL IO's -----------------------------------------------------------------------------------*/\
    X_IO( IO_ENABLE_DRIVE,      GPIOI,      IO_PIN_2,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_STEP_X,            GPIOG,      IO_PIN_6,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_STEP_Y,            GPIOB,      IO_PIN_5,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_STEP_Z,            GPIOG,      IO_PIN_7,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_STEP_A,            GPIOA,      IO_PIN_4,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_STEP_B,            GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_STEP_C,            GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIR_X,             GPIOI,      IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIR_Y,             GPIOH,      IO_PIN_6,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIR_Z,             GPIOI,      IO_PIN_3,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIR_A,             GPIOC,      IO_PIN_6,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIR_B,             GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIR_C,             GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_LIMIT_X1,          GPIOA,      IO_PIN_15,  IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_Y1,          GPIOA,      IO_PIN_8,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_Z1,          GPIOB,      IO_PIN_15,  IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_A1,          GPIOG,      IO_PIN_9,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_B1,          GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_C1,          GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_X2,          GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_Y2,          GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_Z2,          GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_A2,          GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_B2,          GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_LIMIT_C2,          GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_DIGITAL_OUT_1,     GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIGITAL_OUT_2,     GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIGITAL_OUT_3,     GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIGITAL_OUT_4,     GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIGITAL_OUT_5,     GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIGITAL_OUT_6,     GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIGITAL_OUT_7,     GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIGITAL_OUT_8,     GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_DIGITAL_IN_1,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_DIGITAL_IN_2,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_DIGITAL_IN_3,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_DIGITAL_IN_4,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_DIGITAL_IN_5,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_DIGITAL_IN_6,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_DIGITAL_IN_7,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_DIGITAL_IN_8,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_ANALOG_OUT_1,      GPIOA,      IO_PIN_5,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_ANALOG_OUT_2,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_ANALOG_OUT_3,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_ANALOG_OUT_4,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_ANALOG_OUT_5,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_ANALOG_OUT_6,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_ANALOG_OUT_7,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_ANALOG_OUT_8,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_ANALOG_OUT_9,      GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_ANALOG_OUT_10,     GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_SPINDLE_ENABLE,    GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_SPINDLE_PWM,       GPIOxx,     IO_PIN_0,   IO_CFG_TIM1_PP_LS)                        \
    X_IO( IO_SPINDLE_DIRECTION, GPIOxx,     IO_PIN_14,  IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_COOLANT,           GPIOC,      IO_PIN_13,  IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_COOLANT_FLOOD,     GPIOF,      IO_PIN_8,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_COOLANT_MIST,      GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF0)                 \
    X_IO( IO_PROBE,             GPIOC,      IO_PIN_7,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_SAFETY_DOOR,       GPIOxx,     IO_PIN_0,   IO_CFG_INPUT_NP_LS)                       \
    X_IO( IO_CONTROL_RESET,     GPIOA,      IO_PIN_0,   IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_CONTROL_FEED,      GPIOF,      IO_PIN_10,  IO_CFG_INPUT_PU_LS)                       \
    X_IO( IO_CONTROL_START,     GPIOF,      IO_PIN_9,   IO_CFG_INPUT_PU_LS)                       \
/* Misc ----------------------------------------------------------------------------------------*/\
    X_IO( IO_E2_WP,             GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF1)                 \
    X_IO( IO_W5500_RESET,       GPIOxx,     IO_PIN_0,   IO_CFG_OUTPUT_PP_LS_DEF1)                 \
    X_IO( IO_DEBUG_PIN,         GPIOG,      IO_PIN_13,  IO_CFG_OUTPUT_PP_LS_DEF0)                 \
/*----------------------------------------------------------------------------------------------*/


//-------------------------------------------------------------------------------------------------
//
//  IO_GROUP is useful for pin sharing configuration and they are not used individually at run time
//  because they are control by module.
//
//    Examples: LCD, SDRAM, ETH, etc...
//
//  Notes : Some group might share same config
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//                        ETH pins assignment                                   |
//-------------------------------------------------------------------------------
//|  PA1 <-> REF_CLK       |  PC1 <-> MDC          |  PG2  <-> RX_ER            |
//|  PA2 <-> MDIO          |  PC4 <-> RXD0         |  PG11 <-> TX_EN            |
//|  PA7 <-> CRS_DV        |  PC5 <-> RXD1         |  PG13 <-> TXD0             |
//|                        |                       |  PG14 <-> TXD1             |
//|------------------------------------------------------------------------------

#define ETH_PIN_ON_PORT_A       (IO_PIN_1  | IO_PIN_2  | IO_PIN_7)
#define ETH_PIN_ON_PORT_C       (IO_PIN_1  | IO_PIN_4  | IO_PIN_5)
#define ETH_PIN_ON_PORT_G       (IO_PIN_2  | IO_PIN_11 | IO_PIN_13 | IO_PIN_14)

//---------------------------------------------------------------------------------------
//                                I2C pins assignment                                   |
//---------------------------------------------------------------------------------------

#define I2C1_PIN_ON_PORT_B      (IO_PIN_8  | IO_PIN_9)
#define I2C3_PIN_ON_PORT_H      (IO_PIN_7  | IO_PIN_8)

//---------------------------------------------------------------------
//                        LCD pins assignment                         |
//--------------------------------------------------------------------|
//| PI15  <-> LCD_TFT R0 | PJ7  <-> LCD_TFT G0  | PE4  <-> LCD_TFT B0 |
//| PJ0   <-> LCD_TFT R1 | PJ8  <-> LCD_TFT G1  | PJ13 <-> LCD_TFT B1 |
//| PJ1   <-> LCD_TFT R2 | PJ9  <-> LCD_TFT G2  | PJ14 <-> LCD_TFT B2 |
//| PJ2   <-> LCD_TFT R3 | PJ10 <-> LCD_TFT G3  | PJ15 <-> LCD_TFT B3 |
//| PJ3   <-> LCD_TFT R4 | PJ11 <-> LCD_TFT G4  | PG12 <-> LCD_TFT B4 |
//| PJ4   <-> LCD_TFT R5 | PK0  <-> LCD_TFT G5  | PK4  <-> LCD_TFT B5 |
//| PJ5   <-> LCD_TFT R6 | PK1  <-> LCD_TFT G6  | PK5  <-> LCD_TFT B6 |
//| PJ6   <-> LCD_TFT R7 | PK2  <-> LCD_TFT G7  | PK6  <-> LCD_TFT B7 |
//|--------------------------------------------------------------------
//|  LCD_TFT HSYNC <-> PI.10  | LCDTFT VSYNC <->  PI.09 |
//|  LCD_TFT CLK   <-> PI.14  | LCD_TFT DE   <->  PK.07 |
//|---------------------------|--------------------------
//|  LCD_DISP      <-> PI.12  |
//|  LCD_BL_CTRL   <-> PK.03  |
//-----------------------------

#define LCD_PIN_ON_PORT_E         (IO_PIN_4)
#define LCD_PIN_ON_PORT_G         (IO_PIN_12)
#define LCD_PIN_ON_PORT_I         (IO_PIN_9  | IO_PIN_10 | IO_PIN_14 | IO_PIN_15)
#define LCD_PIN_ON_PORT_J         (IO_PIN_0  | IO_PIN_1  | IO_PIN_2  | IO_PIN_3  | IO_PIN_4  | IO_PIN_5  | IO_PIN_6  | IO_PIN_7  | \
                                   IO_PIN_8  | IO_PIN_9  | IO_PIN_10 | IO_PIN_11 | IO_PIN_12 | IO_PIN_13 | IO_PIN_14 | IO_PIN_15)
#define LCD_PIN_ON_PORT_K         (IO_PIN_0  | IO_PIN_1  | IO_PIN_2  | IO_PIN_4  | IO_PIN_5  | IO_PIN_6  | IO_PIN_7)

//---------------------------------------------------------------------------------------
//                                QSPI pins assignment                                  |
//---------------------------------------------------------------------------------------

#define QSPI_PIN_ON_PORT_B_AF9    (IO_PIN_2)
#define QSPI_PIN_ON_PORT_B_AF10   (IO_PIN_6)
#define QSPI_PIN_ON_PORT_D        (IO_PIN_11 | IO_PIN_12 | IO_PIN_13)
#define QSPI_PIN_ON_PORT_E        (IO_PIN_2)

//---------------------------------------------------------------------------------------
//                                SDMMC pins assignment                                 |
//---------------------------------------------------------------------------------------

#define SDMMC_PIN_ON_PORT_C       (IO_PIN_8  | IO_PIN_9  | IO_PIN_10 | IO_PIN_11 | IO_PIN_12)
#define SDMMC_PIN_ON_PORT_D       (IO_PIN_2)

//---------------------------------------------------------------------------------------
//                                SDRAM pins assignment                                 |
//---------------------------------------------------------------------------------------
//| PC3 <-> FMC_SDCKE0 | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0     | PG0  <-> FMC_A10    |
//|--------------------| PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1     | PG1  <-> FMC_A11    |
//| PD0  <-> FMC_D2    | PE7  <-> FMC_D4    | PF2  <-> FMC_A2     | PG4  <-> FMC_BA0    |
//| PD1  <-> FMC_D3    | PE8  <-> FMC_D5    | PF3  <-> FMC_A3     | PG5  <-> FMC_BA1    |
//| PD8  <-> FMC_D13   | PE9  <-> FMC_D6    | PF4  <-> FMC_A4     | PG8  <-> FMC_SDCLK  |
//| PD9  <-> FMC_D14   | PE10 <-> FMC_D7    | PF5  <-> FMC_A5     | PG15 <-> FMC_SDNCAS |
//| PD10 <-> FMC_D15   | PE11 <-> FMC_D8    | PF11 <-> FMC_SDNRAS |---------------------|
//| PD14 <-> FMC_D0    | PE12 <-> FMC_D9    | PF12 <-> FMC_A6     | PH3 <-> FMC_SDNE0   |
//| PD15 <-> FMC_D1    | PE13 <-> FMC_D10   | PF13 <-> FMC_A7     | PH5 <-> FMC_SDNWE   |
//---------------------| PE14 <-> FMC_D11   | PF14 <-> FMC_A8     |----------------------
//                     | PE15 <-> FMC_D12   | PF15 <-> FMC_A9     |
//                     --------------------------------------------

#define SDRAM_PIN_ON_PORT_C         (IO_PIN_3)
#define SDRAM_PIN_ON_PORT_D         (IO_PIN_0  | IO_PIN_1  | IO_PIN_8  | IO_PIN_9  | IO_PIN_10 | IO_PIN_14 | IO_PIN_15)
#define SDRAM_PIN_ON_PORT_E         (IO_PIN_0  | IO_PIN_1  | IO_PIN_7  | IO_PIN_8  | IO_PIN_9  | IO_PIN_10 | IO_PIN_11 | IO_PIN_12 | IO_PIN_13 | IO_PIN_14 | IO_PIN_15)
#define SDRAM_PIN_ON_PORT_F         (IO_PIN_0  | IO_PIN_1  | IO_PIN_2  | IO_PIN_3  | IO_PIN_4  | IO_PIN_5  | IO_PIN_11 | IO_PIN_12 | IO_PIN_13 | IO_PIN_14 | IO_PIN_15)
#define SDRAM_PIN_ON_PORT_G         (IO_PIN_0  | IO_PIN_1  | IO_PIN_4  | IO_PIN_5  | IO_PIN_8  | IO_PIN_15)
#define SDRAM_PIN_ON_PORT_H         (IO_PIN_3  | IO_PIN_5)

//---------------------------------------------------------------------------------------

#define IO_GROUP_DEF(X_IO_GROUP) \
/* ----------------------------------------------------------------------------------------------------------*/\
/*              ENUM ID of the Group,       IO Port,    IO Group Pin,              IO ConfigMode             */\
/* ETH ------------------------------------------------------------------------------------------------------*/\
    X_IO_GROUP( IO_ETH_ON_PORTA,            GPIOA,      ETH_PIN_ON_PORT_A,         IO_CFG_ETH_AF11)            \
    X_IO_GROUP( IO_ETH_ON_PORTC,            GPIOC,      ETH_PIN_ON_PORT_C,         IO_CFG_ETH_AF11)            \
    X_IO_GROUP( IO_ETH_ON_PORTG,            GPIOG,      ETH_PIN_ON_PORT_G,         IO_CFG_ETH_AF11)            \
/* I2C ------------------------------------------------------------------------------------------------------*/\
    X_IO_GROUP( IO_I2C1_ON_PORTB,           GPIOB,      I2C1_PIN_ON_PORT_B,        IO_CFG_I2C_AF4)             \
    X_IO_GROUP( IO_I2C3_ON_PORTH,           GPIOH,      I2C3_PIN_ON_PORT_H,        IO_CFG_I2C_AF4)             \
/* LCD ------------------------------------------------------------------------------------------------------*/\
    X_IO_GROUP( IO_LCD_ON_PORT_E,           GPIOE,      LCD_PIN_ON_PORT_E,         IO_CFG_LCD_AF14)            \
    X_IO_GROUP( IO_LCD_ON_PORT_G,           GPIOG,      LCD_PIN_ON_PORT_G,         IO_CFG_LCD_AF9)             \
    X_IO_GROUP( IO_LCD_ON_PORT_I,           GPIOI,      LCD_PIN_ON_PORT_I,         IO_CFG_LCD_AF14)            \
    X_IO_GROUP( IO_LCD_ON_PORT_J,           GPIOJ,      LCD_PIN_ON_PORT_J,         IO_CFG_LCD_AF14)            \
    X_IO_GROUP( IO_LCD_ON_PORT_K,           GPIOK,      LCD_PIN_ON_PORT_K,         IO_CFG_LCD_AF14)            \
/* QSPI -----------------------------------------------------------------------------------------------------*/\
    X_IO_GROUP( IO_QSPI_ON_PORT_B_PP_AF9,   GPIOB,      QSPI_PIN_ON_PORT_B_AF9,    IO_CFG_QSPI_PP_AF9)         \
    X_IO_GROUP( IO_QSPI_ON_PORT_B_PP_AF10,  GPIOB,      QSPI_PIN_ON_PORT_B_AF10,   IO_CFG_QSPI_PP_AF10)        \
    X_IO_GROUP( IO_QSPI_ON_PORT_D_NP_AF9,   GPIOD,      QSPI_PIN_ON_PORT_D,        IO_CFG_QSPI_NP_AF9)         \
    X_IO_GROUP( IO_QSPI_ON_PORT_E_NP_AF9,   GPIOE,      QSPI_PIN_ON_PORT_E,        IO_CFG_QSPI_NP_AF9)         \
/* SDMMC ----------------------------------------------------------------------------------------------------*/\
    X_IO_GROUP( IO_SDMMC_ON_PORT_C,         GPIOC,      SDMMC_PIN_ON_PORT_C,       IO_CFG_SDMMC_AF12)          \
    X_IO_GROUP( IO_SDMMC_ON_PORT_D,         GPIOD,      SDMMC_PIN_ON_PORT_D,       IO_CFG_SDMMC_AF12)          \
/* SDRAM ----------------------------------------------------------------------------------------------------*/\
    X_IO_GROUP( IO_SDRAM_ON_PORT_C,         GPIOC,      SDRAM_PIN_ON_PORT_C,       IO_CFG_SDRAM_AF12)          \
    X_IO_GROUP( IO_SDRAM_ON_PORT_D,         GPIOD,      SDRAM_PIN_ON_PORT_D,       IO_CFG_SDRAM_AF12)          \
    X_IO_GROUP( IO_SDRAM_ON_PORT_E,         GPIOE,      SDRAM_PIN_ON_PORT_E,       IO_CFG_SDRAM_AF12)          \
    X_IO_GROUP( IO_SDRAM_ON_PORT_F,         GPIOF,      SDRAM_PIN_ON_PORT_F,       IO_CFG_SDRAM_AF12)          \
    X_IO_GROUP( IO_SDRAM_ON_PORT_G,         GPIOG,      SDRAM_PIN_ON_PORT_G,       IO_CFG_SDRAM_AF12)          \
    X_IO_GROUP( IO_SDRAM_ON_PORT_H,         GPIOH,      SDRAM_PIN_ON_PORT_H,       IO_CFG_SDRAM_AF12)          \
/* ----------------------------------------------------------------------------------------------------------*/

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
//          Parameter 4:    Priority
//
//          Parameter 5:    IO_EXTI_TRIGGER_RISING
//                          IO_EXTI_TRIGGER_FALLING
//                          IO_EXTI_TRIGGER_RISING_FALLING
//
//          Parameter 6:    Callback function pointer
//
//          Parameter 7:    Argument pointer for callback
//
//-------------------------------------------------------------------------------------------------

#define IO_IRQ_DEF(X_IO_IRQ) \
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------*/\
/*         Enum ID                 Pin ID              EXTI Pin Source  Priority    Trigger                             CallBack                Argument    */\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------*/\
  X_IO_IRQ(IO_ISR_LIMIT_X1,        IO_LIMIT_X1,        EXTI15_10_IRQn,  6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_Y1,        IO_LIMIT_Y1,        EXTI9_5_IRQn,    6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_Z1,        IO_LIMIT_Z1,        EXTI15_10_IRQn,  6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_A1,        IO_LIMIT_A1,        EXTI9_5_IRQn,    6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_B1,        IO_LIMIT_B1,        EXTI15_10_IRQn,  6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_C1,        IO_LIMIT_C1,        EXTI9_5_IRQn,    6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_X2,        IO_LIMIT_X1,        EXTI15_10_IRQn,  6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_Y2,        IO_LIMIT_Y1,        EXTI9_5_IRQn,    6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_Z2,        IO_LIMIT_Z1,        EXTI15_10_IRQn,  6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_A2,        IO_LIMIT_A1,        EXTI9_5_IRQn,    6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_B2,        IO_LIMIT_B1,        EXTI15_10_IRQn,  6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
  X_IO_IRQ(IO_ISR_LIMIT_C2,        IO_LIMIT_C1,        EXTI9_5_IRQn,    6,          IO_EXTI_TRIGGER_RISING_FALLING,     nullptr,                nullptr)      \
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------*/\

// IO_LIMIT_A is a fake limit switch


//-------------------------------------------------------------------------------------------------
