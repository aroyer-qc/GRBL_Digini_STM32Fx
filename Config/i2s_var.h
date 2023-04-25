//-------------------------------------------------------------------------------------------------
//
//  File :  i2s_var.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2022 Alain Royer.
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
//
//  Notes: I2S variables for STM32F4xx
//
//
//  use one of the define as communication standard
//
//      Specify I2S Standard
//            I2S_STANDARD_PHILLIPS
//            I2S_STANDARD_MSB
//            I2S_STANDARD_LSB
//
//-------------------------------------------------------------------------------------------------

// copied from F4, might not be compatible

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef I2S_DRIVER_GLOBAL

I2S_Info_t I2S_Info[NB_OF_I2S_DRIVER] =
{
  #if (I2S_DRIVER_SUPPORT_I2S2_CFG == DEF_ENABLED)
    {
        DRIVER_I2S2_ID,
        I2S2,                       // I2Sx
        RCC_APB1ENR_SPI2EN,         // Enable I2Sx module bit
        IO_I2S2_MCK,                // MCK
        IO_I2S2_CK,                 // CK
        IO_I2S2_SD,                 // SD
        IO_I2S2_WS,                 // WS
        I2S_MODE_MASTER_RX,         // Mode
        I2S_DATAFORMAT_16B,         // Data format
        I2S_STANDARD_PHILLIPS,      // Standard to be used on Communication
        I2S_48000_HZ,               // Default frequency
        I2S_MCLK_OUTPUT_ENABLE,     // MCLK Output
        I2S_CPOL_LOW,               // Clock polarity
        DMA_CHANNEL_0,              // DMA Channel
        DMA1_Stream7,               // DMA Stream
        DMA1_Stream7_IRQn,          // DMA Stream IRQn

        DMA_HIFCR_CHTIF7 |
        DMA_HIFCR_CTCIF7,           // IT_Flag
    },
  #endif

  #if (I2S_DRIVER_SUPPORT_I2S3_CFG == DEF_ENABLED)
    {
        DRIVER_I2S3_ID,
        I2S3,                       // I2Sx
        RCC_APB1ENR_SPI3EN,         // Enable I2Sx module bit
        IO_I2S3_MCK,                // MCK
        IO_I2S3_CK,                 // CK
        IO_I2S3_SD,                 // SD
        IO_I2S3_WS,                 // WS
        I2S_MODE_MASTER_TX,         // Mode
        I2S_DATAFORMAT_16B,         // Data format
        I2S_STANDARD_PHILLIPS,       // Standard to be used on Communication
        I2S_48000_HZ,               // Default frequency
        I2S_MCLK_OUTPUT_ENABLE,     // MCLK Output
        I2S_CPOL_LOW,               // Clock polarity
        DMA_CHANNEL_0,              // DMA Channel
        DMA1_Stream7,               // DMA Stream
        DMA1_Stream7_IRQn,          // DMA Stream IRQn

        DMA_HIFCR_CHTIF7 |
        DMA_HIFCR_CTCIF7,           // IT_Flag
    },
  #endif
};

//class I2S_Driver myI2S_Bluetooth    (DRIVER_I2S2_ID);
class I2S_Driver myI2S_Output       (DRIVER_I2S3_ID);

#else // I2S_DRIVER_GLOBAL

extern const I2S_Info_t I2S_PortInfo[NB_OF_I2S_DRIVER];

//extern class I2S_Driver myI2S_Bluetooth;
extern class I2S_Driver myI2S_Output;

#endif // I2S_DRIVER_GLOBAL

//-------------------------------------------------------------------------------------------------
