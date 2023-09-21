//-------------------------------------------------------------------------------------------------
//
//  File :  spi_var.h
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
//
//  Notes : SPI variables for STM32F4xx
//
//-------------------------------------------------------------------------------------------------

// miht not be compatible with F7 come from F4

//-------------------------------------------------------------------------------------------------
// Constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef SPI_DRIVER_GLOBAL

SPI_Info_t SPI_Info[NB_OF_SPI_DRIVER] =
{
  #if (SPI_DRIVER_SUPPORT_SPI1_CFG == DEF_ENABLED)
	{
	    DRIVER_SPI1_ID,
            SPI1,                       // SPIx
            IO_SPI1_CLK,                // CLK
            IO_SPI1_MOSI,               // MOSI
            IO_SPI1_MISO,               // MISO
            IO_SPI1_NSS,				// NSS if no IO exist user can use IO_NOT_DEFINED
            SPI_FULL_DUPLEX,            // SPI_FULL_DUPLEX or SPI_HALF_DUPLEX
            SPI_SPEED_FCLK_DIV64,
            RCC_APB2ENR_SPI1EN,         // RCC_APBxENR
            &RCC->APB2ENR,              // Register
            SPI1_IRQn,                  // IRQn_Channel

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == DEF_ENABLED)
        DMA2,                       // DMA Module
		RCC_APB1ENR_DMA2EN,         // RCC_APBxENR for DMA
        DMA_CHANNEL_3               // DMA_Channel RX
        DMA_LIFCR_CTCIF0,           // RX_IT_Flag
        DMA2_Stream0,               // RX_DMA_TypeDef
        DMA2_Stream0_IRQn,          // RX_IRQn
        //DMA_LIFCR_CTCIF2,         // RX_IT_Flag
        //DMA1_Stream2,             // RX_DMA_TypeDef
        //DMA1_Stream2_IRQn,        // RX_IRQn
        DMA_CHANNEL_3,              // DMA_Channel TX
        DMA_LIFCR_CTCIF3,           // TX_IT_Flag
        DMA2_Stream3,               // TX_DMA_TypeDef
        DMA2_Stream3_IRQn,          // TX_IRQn
        //DMA_HIFCR_CTCIF5,         // TX_IT_Flag
        //DMA2_Stream5,             // TX_DMA_TypeDef
        //DMA2_Stream5_IRQn,        // TX_IRQn
        RCC_AHB1ENR_DMA1EN,         // RCC_APBxPeriph
      #endif
	},
  #endif

  #if (SPI_DRIVER_SUPPORT_SPI2_CFG == DEF_ENABLED)
	{
	    DRIVER_SPI2_ID,
		SPI2,                       // SPIx
        IO_SPI2_CLK,                // CLK
        IO_SPI2_MOSI,               // MOSI
        IO_NOT_DEFINED,             // MISO
		IO_NOT_DEFINED,				// NSS if no IO exist user can use IO_NOT_DEFINED
		SPI_HALF_DUPLEX,            // SPI_FULL_DUPLEX or SPI_HALF_DUPLEX
		SPI_SPEED_FCLK_DIV64,
		RCC_APB1ENR_SPI2EN,         // RCC_APBxENR
		&RCC->APB1ENR,              // Register
		SPI2_IRQn,                  // IRQn_Channel

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == DEF_ENABLED)
        DMA1,                       // DMA Module
        DMA_CHANNEL_0,              // DMA_Channel RX
        DMA_LIFCR_CTCIF3,           // RX_IT_Flag
        DMA1_Stream3,               // RX_DMA_TypeDef
        DMA1_Stream3_IRQn,          // RX_IRQn
        DMA_CHANNEL_0,              // DMA_Channel TX
        DMA_HIFCR_CTCIF4,           // TX_IT_Flag
        DMA1_Stream4,               // TX_DMA_TypeDef
        DMA1_Stream4_IRQn,          // TX_IRQn
        RCC_AHB1ENR_DMA1EN,         // RCC_APBxPeriph
      #endif
 	},
  #endif

  #if (SPI_DRIVER_SUPPORT_SPI3_CFG == DEF_ENABLED)
	{
	    DRIVER_SPI3_ID,
		SPI3,                       // SPIx
        IO_SPI3_CLK,                // CLK
        IO_SPI3_MOSI,               // MOSI
        IO_SPI3_MISO,               // MISO
		IO_SPI3_NSS,				// NSS if no IO exist user can use IO_NOT_DEFINED
		SPI_FULL_DUPLEX,            // SPI_FULL_DUPLEX or SPI_HALF_DUPLEX
		SPI_SPEED_FCLK_DIV64,
		RCC_APB1ENR_SPI3EN,         // RCC_APBxENR
		&RCC->APB1ENR,              // Register
		SPI3_IRQn,                  // IRQn_Channel

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == DEF_ENABLED)
        DMA1,                       // DMA Module
        DMA_CHANNEL_0,              // DMA_Channel RX
        DMA_LIFCR_CTCIF0,           // RX_IT_Flag
        DMA1_Stream0,               // RX_DMA_TypeDef
        DMA1_Stream0_IRQn,          // RX_IRQn
        //DMA_LIFCR_CTCIF2,         // RX_IT_Flag
        //DMA1_Stream2,             // RX_DMA_TypeDef
        //DMA1_Stream2_IRQn,        // RX_IRQn
        DMA_CHANNEL_0,              // DMA_Channel TX
        DMA_HIFCR_CTCIF5,           // TX_IT_Flag
        DMA1_Stream5,               // TX_DMA_TypeDef
        DMA1_Stream5_IRQn,          // TX_IRQn
        //DMA_HIFCR_CTCIF7,         // TX_IT_Flag
        //DMA1_Stream7,             // TX_DMA_TypeDef
        //DMA1_Stream7_IRQn,        // TX_IRQn
        RCC_AHB1ENR_DMA1EN,         // RCC_APBxPeriph
      #endif
	},
  #endif
};

class SPI_Driver mySPI_ForLCD(DRIVER_SPI2_ID);

#else // SPI_DRIVER_GLOBAL

extern       SPI_Info_t     SPI_Info[NB_OF_SPI_DRIVER];
extern class SPI_Driver     mySPI_ForLCD;

#endif // SPI_DRIVER_GLOBAL
