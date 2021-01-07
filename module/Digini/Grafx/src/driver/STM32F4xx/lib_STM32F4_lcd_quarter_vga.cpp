//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_lcd_quarter_vga.cpp
//
//-------------------------------------------------------------------------------------------------

#ifdef DIGINI_USE_GRAFX
#ifdef STM32F429_439xx
#ifdef DIGINI_USE_DRV_STM32F4XX_QUARTER_VGA

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "stm32f4xx.h"
#include "lib_STM32F4_lcd_quarter_vga.h"
// TO DO use an lib_Port for this
#include "lib_class_STM32F4_gpio.h"
#include "lib_class_STM32F4_spi.h"

//-------------------------------------------------------------------------------------------------
// Prototype(s)
//-------------------------------------------------------------------------------------------------

static void GRAFX_DRV_PowerOn();

//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_SPI_Config
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Configures the LCD_SPI interface.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
/*
static void DRV_SPI_Config()
{
    SPI_InitTypeDef    SPI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;

    // Enable LCD_SPI_SCK_GPIO_CLK, LCD_SPI_MISO_GPIO_CLK and LCD_SPI_MOSI_GPIO_CLK clock
    RCC_AHB1PeriphClockCmd(LCD_SPI_SCK_GPIO_CLK | LCD_SPI_MISO_GPIO_CLK | LCD_SPI_MOSI_GPIO_CLK, ENABLE);

    // Enable LCD_SPI and SYSCFG clock
    RCC_APB2PeriphClockCmd(LCD_SPI_CLK, ENABLE);

    // Configure LCD_SPI SCK pin
    GPIO_InitStructure.GPIO_Pin   = LCD_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(LCD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    // Configure LCD_SPI MISO pin
    GPIO_InitStructure.GPIO_Pin = LCD_SPI_MISO_PIN;
    GPIO_Init(LCD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

    // Configure LCD_SPI MOSI pin
    GPIO_InitStructure.GPIO_Pin = LCD_SPI_MOSI_PIN;
    GPIO_Init(LCD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    // Connect SPI SCK
    GPIO_PinAFConfig(LCD_SPI_SCK_GPIO_PORT, LCD_SPI_SCK_SOURCE, LCD_SPI_SCK_AF);

    // Connect SPI MISO
    GPIO_PinAFConfig(LCD_SPI_MISO_GPIO_PORT, LCD_SPI_MISO_SOURCE, LCD_SPI_MISO_AF);

    // Connect SPI MOSI
    GPIO_PinAFConfig(LCD_SPI_MOSI_GPIO_PORT, LCD_SPI_MOSI_SOURCE, LCD_SPI_MOSI_AF);

    SPI_I2S_DeInit(LCD_SPI);

    // SPI Config
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex; //SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    SPI_Init(LCD_SPI, &SPI_InitStructure);

    // SPI enable
    SPI_Cmd(LCD_SPI, ENABLE);
}
*/

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

    // Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                           RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

    // GPIOs Configuration
    //
    // +------------------------+-----------------------+----------------------------+
    // +                       LCD pins assignment                                   +
    // +------------------------+-----------------------+----------------------------+
    // |  LCD_TFT R2 <-> PC.12  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
    // |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
    // |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
    // |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
    // |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
    // |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
    // -------------------------------------------------------------------------------
    // |  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
    // |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
    // -------------------------------------------------------

    // GPIOA configuration
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // GPIOB configuration
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // GPIOC configuration
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // GPIOD configuration
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    // GPIOF configuration
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOF, &GPIO_InitStruct);

    // GPIOG configuration
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource6,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource7,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOG, &GPIO_InitStruct);

    // Control lines IL9340    
    IO_OutputInit(LCD_NCS);
    IO_OutputInit(LCD_WRX);

    // Init the LCD controller IL9340
    if(SPI_Port5.LockToDevice(&SPI_DeviceInfo[SPI_DEVICE_LCD_CONTROL]) != READY) return;
    GRAFX_DRV_PowerOn();
    SPI_Port5.UnlockFromDevice(&SPI_DeviceInfo[SPI_DEVICE_LCD_CONTROL]);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_WriteCommand
//
//  Parameter(s):   uint8_t LCD_Reg    Address of the selected register.
//  Return:         None
//
//  Description:    Writes command to select the LCD register.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
static void GRAFX_DRV_WriteCommand(uint8_t LCD_Reg)
{
    // Reset WRX to send command
    IO_Output(LCD_WRX, IO_SET);

    // Reset LCD control line(/CS) and Send command
    IO_Output(LCD_NCS, IO_SET);
    SPI_Port5.Write(LCD_Reg);

    // Wait until a data is sent(not busy), before config /CS HIGH
    SPI_Port5.WaitReady();
    //BSP_Delay(20000);
    IO_Output(LCD_NCS, IO_RESET);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_WriteData
//
//  Parameter(s):   uint8_t value          Data to write to the selected register.
//  Return:         None
//
//  Description:    Writes data to select the LCD register.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
static void GRAFX_DRV_WriteData(uint8_t byValue)
{
    // Set WRX to send data
    IO_Output(LCD_WRX, IO_SET);

    // Reset LCD control line(/CS) and Send data
    IO_Output(LCD_NCS, IO_SET);
    SPI_Port5.Write(byValue);

    // Wait until a data is sent(not busy), before config /CS HIGH
    SPI_Port5.WaitReady();
    //BSP_Delay(20000);
    IO_Output(LCD_NCS, IO_RESET);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_PowerOn
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Setup LCD device controller
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
static void GRAFX_DRV_PowerOn()
{
    GRAFX_DRV_WriteCommand(0xCA);
    GRAFX_DRV_WriteData(0xC3);
    GRAFX_DRV_WriteData(0x08);
    GRAFX_DRV_WriteData(0x50);
    GRAFX_DRV_WriteCommand(LCD_POWERB);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0xC1);
    GRAFX_DRV_WriteData(0x30);
    GRAFX_DRV_WriteCommand(LCD_POWER_SEQ);
    GRAFX_DRV_WriteData(0x64);
    GRAFX_DRV_WriteData(0x03);
    GRAFX_DRV_WriteData(0x12);
    GRAFX_DRV_WriteData(0x81);
    GRAFX_DRV_WriteCommand(LCD_DTCA);
    GRAFX_DRV_WriteData(0x85);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0x78);
    GRAFX_DRV_WriteCommand(LCD_POWERA);
    GRAFX_DRV_WriteData(0x39);
    GRAFX_DRV_WriteData(0x2C);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0x34);
    GRAFX_DRV_WriteData(0x02);
    GRAFX_DRV_WriteCommand(LCD_PRC);
    GRAFX_DRV_WriteData(0x20);
    GRAFX_DRV_WriteCommand(LCD_DTCB);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteCommand(LCD_FRC);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0x1B);
    GRAFX_DRV_WriteCommand(LCD_DFC);
    GRAFX_DRV_WriteData(0x0A);
    GRAFX_DRV_WriteData(0xA2);
    GRAFX_DRV_WriteCommand(LCD_POWER1);
    GRAFX_DRV_WriteData(0x10);
    GRAFX_DRV_WriteCommand(LCD_POWER2);
    GRAFX_DRV_WriteData(0x10);
    GRAFX_DRV_WriteCommand(LCD_VCOM1);
    GRAFX_DRV_WriteData(0x45);
    GRAFX_DRV_WriteData(0x15);
    GRAFX_DRV_WriteCommand(LCD_VCOM2);
    GRAFX_DRV_WriteData(0x90);
    GRAFX_DRV_WriteCommand(LCD_MAC);
    GRAFX_DRV_WriteData(0xC8);
    GRAFX_DRV_WriteCommand(LCD_3GAMMA_EN);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteCommand(LCD_RGB_INTERFACE);
    GRAFX_DRV_WriteData(0xC2);
    GRAFX_DRV_WriteCommand(LCD_DFC);
    GRAFX_DRV_WriteData(0x0A);
    GRAFX_DRV_WriteData(0xA7);
    GRAFX_DRV_WriteData(0x27);
    GRAFX_DRV_WriteData(0x04);

    GRAFX_DRV_WriteCommand(LCD_COLUMN_ADDR);                  // Colomn address set
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0xEF);
    GRAFX_DRV_WriteCommand(LCD_PAGE_ADDR);                    // Page address set
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0x01);
    GRAFX_DRV_WriteData(0x3F);
    GRAFX_DRV_WriteCommand(LCD_INTERFACE);
    GRAFX_DRV_WriteData(0x01);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0x06);

    GRAFX_DRV_WriteCommand(LCD_GRAM);
    //BSP_Delay(200000);

    GRAFX_DRV_WriteCommand(LCD_GAMMA);
    GRAFX_DRV_WriteData(0x01);

    GRAFX_DRV_WriteCommand(LCD_PGAMMA);
    GRAFX_DRV_WriteData(0x0F);
    GRAFX_DRV_WriteData(0x29);
    GRAFX_DRV_WriteData(0x24);
    GRAFX_DRV_WriteData(0x0C);
    GRAFX_DRV_WriteData(0x0E);
    GRAFX_DRV_WriteData(0x09);
    GRAFX_DRV_WriteData(0x4E);
    GRAFX_DRV_WriteData(0x78);
    GRAFX_DRV_WriteData(0x3C);
    GRAFX_DRV_WriteData(0x09);
    GRAFX_DRV_WriteData(0x13);
    GRAFX_DRV_WriteData(0x05);
    GRAFX_DRV_WriteData(0x17);
    GRAFX_DRV_WriteData(0x11);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteCommand(LCD_NGAMMA);
    GRAFX_DRV_WriteData(0x00);
    GRAFX_DRV_WriteData(0x16);
    GRAFX_DRV_WriteData(0x1B);
    GRAFX_DRV_WriteData(0x04);
    GRAFX_DRV_WriteData(0x11);
    GRAFX_DRV_WriteData(0x07);
    GRAFX_DRV_WriteData(0x31);
    GRAFX_DRV_WriteData(0x33);
    GRAFX_DRV_WriteData(0x42);
    GRAFX_DRV_WriteData(0x05);
    GRAFX_DRV_WriteData(0x0C);
    GRAFX_DRV_WriteData(0x0A);
    GRAFX_DRV_WriteData(0x28);
    GRAFX_DRV_WriteData(0x2F);
    GRAFX_DRV_WriteData(0x0F);

    GRAFX_DRV_WriteCommand(LCD_SLEEP_OUT);
    //BSP_Delay(200000);
    GRAFX_DRV_WriteCommand(LCD_DISPLAY_ON);
    GRAFX_DRV_WriteCommand(LCD_GRAM);                         // GRAM start writing
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_DRV_STM32F4XX_QUARTER_VGA
#endif // STM32F429_439xx
#endif // DIGINI_USE_GRAFX
