//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_lcd_480x272.cpp
//
//-------------------------------------------------------------------------------------------------

#ifdef DIGINI_USE_GRAFX
#ifdef STM32F429_439xx
#ifdef DIGINI_USE_DRV_STM32F4XX_480X272

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "stm32f4xx.h"
#include "lib_class_STM32F4_lcd_480X272.h"

//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_Config
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    GPIO configuration specific for the LCD and processor used by this driver
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GRAFX_DRV_Config()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Enable GPIOA, GPIOI, GPIOJ, GPIOJ AHB Clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOJ | RCC_AHB1Periph_GPIOK, ENABLE);

    // +------------------------+-----------------------+----------------------------+
    // +                       LCD pins assignment                                   +
    // +------------------------+-----------------------+----------------------------+
    // |  LCD_TFT R0 <-> PI.15  |  LCD_TFT G0 <-> PJ.07 |  LCD_TFT B0 <-> PJ.12      |
    // |  LCD_TFT R1 <-> PJ.00  |  LCD_TFT G1 <-> PJ.08 |  LCD_TFT B1 <-> PJ.13      |
    // |  LCD_TFT R2 <-> PJ.01  |  LCD_TFT G2 <-> PJ.09 |  LCD_TFT B2 <-> PJ.14      |
    // |  LCD_TFT R3 <-> PJ.02  |  LCD_TFT G3 <-> PJ.10 |  LCD_TFT B3 <-> PJ.15      |
    // |  LCD_TFT R4 <-> PJ.03  |  LCD_TFT G4 <-> PJ.11 |  LCD_TFT B4 <-> PK.03      |
    // |  LCD_TFT R5 <-> PJ.04  |  LCD_TFT G5 <-> PK.00 |  LCD_TFT B5 <-> PK.04      |
    // |  LCD_TFT R6 <-> PJ.05  |  LCD_TFT G6 <-> PK.01 |  LCD_TFT B6 <-> PK.05      |
    // |  LCD_TFT R7 <-> PJ.06  |  LCD_TFT G7 <-> PK.02 |  LCD_TFT B7 <-> PK.06      |
    // -------------------------------------------------------------------------------
    // |  LCD_TFT HSYNC <-> PI.12  | LCDTFT VSYNC <->  PI.13 |
    // |  LCD_TFT CLK   <-> PI.14  | LCD_TFT DE   <->  PK.07 |
    // -------------------------------------------------------
    // |  LCD_BL_CTRL   <-> PA.08  |
    // -----------------------------

    // ---- GPIOs Configuration ----

    // Configure the LCD_BL_CTRL pin
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin   = LCD_BL_CTRL_PIN;
    GPIO_Init(LCD_BL_CTRL_PORT, &GPIO_InitStruct);

    // GPIOs All other pin are configure the same
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    // GPIOI configuration
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource12, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource13, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource14, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource15, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOI, &GPIO_InitStruct);

    // GPIOJ configuration
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource0,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource1,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource2,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource3,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource4,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource5,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource6,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource7,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource8,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource9,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource12, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource13, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource14, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOJ, GPIO_PinSource15, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
    GPIO_Init(GPIOJ, &GPIO_InitStruct);

    // GPIOK configuration
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource0,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource1,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource2,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource3,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource4,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource5,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource6,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOK, GPIO_PinSource7,  GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOK, &GPIO_InitStruct);
}


//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_DRV_STM32F4XX_480X272
#endif // STM32F429_439xx
#endif // DIGINI_USE_GRAFX
