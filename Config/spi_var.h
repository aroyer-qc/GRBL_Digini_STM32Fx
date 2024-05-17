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
// maybe not be compatible with F7 come from F4
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define SPI_STANDARD_CONFIGURATION_RX ( DMA_MODE_NORMAL             | \
                                        DMA_PERIPHERAL_NO_INCREMENT | \
                                        DMA_MEMORY_INCREMENT        | \
                                        DMA_PERIPHERAL_SIZE_8_BITS  | \
                                        DMA_MEMORY_SIZE_8_BITS      | \
                                        DMA_PERIPHERAL_BURST_SINGLE | \
                                        DMA_MEMORY_BURST_SINGLE     | \
                                        DMA_PRIORITY_MEDIUM)

#define SPI_STANDARD_CONFIGURATION_TX ( DMA_MODE_NORMAL             | \
                                        DMA_PERIPHERAL_NO_INCREMENT | \
                                        DMA_MEMORY_INCREMENT        | \
                                        DMA_PERIPHERAL_SIZE_8_BITS  | \
                                        DMA_MEMORY_SIZE_8_BITS      | \
                                        DMA_PERIPHERAL_BURST_SINGLE | \
                                        DMA_MEMORY_BURST_SINGLE     | \
                                        DMA_PRIORITY_MEDIUM)

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
        SPI_SPEED_FCLK_DIV64,


        SPI_FULL_DUPLEX,            // SPI_FULL_DUPLEX or SPI_HALF_DUPLEX
        4,
        SPI1_IRQn,
        0,                  // IRQn_Channel
        nullptr,

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == DEF_ENABLED)
        // DMA_RX
        {
            SPI_STANDARD_CONFIGURATION_RX | DMA_CHANNEL_3, // Configuration + DMA_Channel RX
            DMA_LIFCR_CTCIF0,                              // RX_IT_Flag
            DMA2_Stream0,                                  // RX_DMA_TypeDef
            DMA2_Stream0_IRQn,                             // TX_IRQn

            // Other choice
         // UART_STANDARD_CONFIGURATION_RX | DMA_CHANNEL_3, // DMA_Channel RX
         // DMA_LIFCR_CTCIF2,                               // RX_IT_Flag
         // DMA1_Stream2,                                   // RX_DMA_TypeDef
         // DMA1_Stream2_IRQn,                              // TX_IRQn
        },

        // DMA_TX
        {
            SPI_STANDARD_CONFIGURATION_RX | DMA_CHANNEL_3, // Configuration + DMA_Channel RX
            DMA_LIFCR_CTCIF3,                              // RX_IT_Flag
            DMA2_Stream3,                                  // RX_DMA_TypeDef
            DMA2_Stream3_IRQn,                             // TX_IRQn

            // Other choice
         // UART_STANDARD_CONFIGURATION_RX | DMA_CHANNEL_3, // DMA_Channel RX
         // DMA_HIFCR_CTCIF5,                               // RX_IT_Flag
         // DMA2_Stream5,                                   // RX_DMA_TypeDef
         // DMA2_Stream5_IRQn,                              // TX_IRQn
        },
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
		SPI_SPEED_FCLK_DIV64,

		SPI_HALF_DUPLEX,            // SPI_FULL_DUPLEX or SPI_HALF_DUPLEX
		SPI2_IRQn,                  // IRQn_Channel

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == DEF_ENABLED)
        // DMA_RX
        {
            SPI_STANDARD_CONFIGURATION_RX | DMA_CHANNEL_0, // Configuration + DMA_Channel RX
            DMA_LIFCR_CTCIF3,                              // RX_IT_Flag
            DMA1_Stream3,                                  // RX_DMA_TypeDef
            DMA1_Stream3_IRQn,                             // TX_IRQn
        },

        // DMA_TX
        {
            SPI_STANDARD_CONFIGURATION_RX | DMA_CHANNEL_0, // Configuration + DMA_Channel RX
            DMA_HIFCR_CTCIF4,                              // RX_IT_Flag
            DMA1_Stream4,                                  // RX_DMA_TypeDef
            DMA1_Stream4_IRQn,                             // TX_IRQn
        },
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
		SPI_SPEED_FCLK_DIV64,
		SPI_FULL_DUPLEX,            // SPI_FULL_DUPLEX or SPI_HALF_DUPLEX

		SPI3_IRQn,                  // IRQn_Channel

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == DEF_ENABLED)
        // DMA_RX
        {
            SPI_STANDARD_CONFIGURATION_RX | DMA_CHANNEL_0, // Configuration + DMA_Channel RX
            DMA_LIFCR_CTCIF0,                              // RX_IT_Flag
            DMA1_Stream0,                                  // RX_DMA_TypeDef
            DMA1_Stream0_IRQn,                             // TX_IRQn

            // Other choice
         // UART_STANDARD_CONFIGURATION_RX | DMA_CHANNEL_0, // DMA_Channel RX
         // DMA_LIFCR_CTCIF3,                               // RX_IT_Flag
         // DMA1_Stream3,                                   // RX_DMA_TypeDef
         // DMA1_Stream3_IRQn,                              // TX_IRQn
        },

        // DMA_TX
        {
            SPI_STANDARD_CONFIGURATION_RX | DMA_CHANNEL_0, // Configuration + DMA_Channel RX
            DMA_HIFCR_CTCIF5,                              // RX_IT_Flag
            DMA1_Stream5,                                  // RX_DMA_TypeDef
            DMA1_Stream5_IRQn,                             // TX_IRQn

            // Other choice
         // UART_STANDARD_CONFIGURATION_RX | DMA_CHANNEL_0, // DMA_Channel RX
         // DMA_HIFCR_CTCIF7,                               // RX_IT_Flag
         // DMA1_Stream7,                                   // RX_DMA_TypeDef
         // DMA1_Stream7_IRQn,                              // TX_IRQn
        },
      #endif
	},
  #endif
};


// TODO need to add 4,5,6

class SPI_Driver mySPI_ForLCD(DRIVER_SPI1_ID);

#else // SPI_DRIVER_GLOBAL

extern       SPI_Info_t     SPI_Info[NB_OF_SPI_DRIVER];
extern class SPI_Driver     mySPI_ForLCD;

#endif // SPI_DRIVER_GLOBAL
