//-------------------------------------------------------------------------------------------------
//
//  File :  spi_var.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2024 Alain Royer.
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

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define SPI_STANDARD_CONFIG               ( SPI_MODE_MASTER       | \
                                            SPI_DATA_WIDTH_8_BIT  | \
                                            SPI_POLARITY_LOW      | \
                                            SPI_PHASE_1_EDGE      | \
                                            SPI_NSS_SOFT          | \
                                            SPI_MSB_FIRST         | \
                                            SPI_FULL_DUPLEX)


#define SPI_DMA_STANDARD_CONFIG_RX        ( DMA_MODE_NORMAL             | \
                                            DMA_PERIPHERAL_TO_MEMORY    | \
                                            DMA_PERIPHERAL_NO_INCREMENT | \
                                            DMA_MEMORY_INCREMENT        | \
                                            DMA_PERIPHERAL_SIZE_8_BITS  | \
                                            DMA_MEMORY_SIZE_8_BITS      | \
                                            DMA_PERIPHERAL_BURST_SINGLE | \
                                            DMA_MEMORY_BURST_SINGLE     | \
                                            DMA_PRIORITY_MEDIUM)

#define SPI_DMA_STANDARD_CONFIG_TX        ( DMA_MODE_NORMAL             | \
                                            DMA_MEMORY_TO_PERIPHERAL    | \
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
        SPI1,                       // SPIx
        SPI_STANDARD_CONFIG,        // Configuration
		24000000,                   // The DAC chip is 12 inche away from CPU on 790A, so reduced comm speed
        IO_SPI1_CLK,                // CLK
        IO_SPI1_MOSI,               // MOSI
        IO_SPI1_MISO,               // MISO
        IO_NOT_DEFINED,             // NSS  N/U
        SPI1_IRQn,                  // IRQn_Channel

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == ENABLED)
        // DMA_RX
        {
            SPI_DMA_STANDARD_CONFIG_RX | DMA_CHANNEL_3,    // Configuration + DMA_Channel RX
            DMA_LIFCR_CTCIF0,                              // RX_IT_Flag
            DMA2_Stream0,                                  // RX_DMA_TypeDef
            DMA2_Stream0_IRQn,                             // RX_IRQn

            // Other choice
         // SPI_DMA_STANDARD_CONFIG_RX | DMA_CHANNEL_3,    // Configuration + DMA_Channel RX
         // DMA_LIFCR_CTCIF2,                              // RX_IT_Flag
         // DMA1_Stream2,                                  // RX_DMA_TypeDef
         // DMA1_Stream2_IRQn,                             // RX_IRQn
        },

        // DMA TX
        {
            SPI_DMA_STANDARD_CONFIG_TX | DMA_CHANNEL_3,    // Configuration + DMA_Channel TX
            DMA_LIFCR_CTCIF3,                              // TX_IT_Flag
            DMA2_Stream3,                                  // TX_DMA_TypeDef
            DMA2_Stream3_IRQn,                             // TX_IRQn

            // Other choice
         // SPI_DMA_STANDARD_CONFIG_TX | DMA_CHANNEL_3,    // Configuration + DMA_Channel TX
         // DMA_HIFCR_CTCIF5,                              // TX_IT_Flag
         // DMA2_Stream5,                                  // TX_DMA_TypeDef
         // DMA2_Stream5_IRQn,                             // TX_IRQn
        },
      #endif
	},
  #endif

  #if (SPI_DRIVER_SUPPORT_SPI2_CFG == DEF_ENABLED)
	{
		SPI2,                       // SPIx
        SPI_STANDARD_CONFIG,        // Configuration
		24000000,                   // The DAC chip is 12 inche away from CPU on 790A, so reduced comm speed
        IO_SPI2_CLK,                // CLK
        IO_SPI2_MOSI,               // MOSI
        IO_NOT_DEFINED,             // MISO
        IO_NOT_DEFINED,             // NSS  N/U
		SPI2_IRQn,                  // IRQn_Channel

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == ENABLED)
        // DMA_RX
        {
            SPI_DMA_STANDARD_CONFIG_RX | DMA_CHANNEL_0,    // Configuration + DMA_Channel RX
            DMA_LIFCR_CTCIF3,                              // RX_IT_Flag
            DMA1_Stream3,                                  // RX_DMA_TypeDef
            DMA1_Stream3_IRQn,                             // RX_IRQn
        },

        // DMA_TX
        {
            SPI_DMA_STANDARD_CONFIG_TX | DMA_CHANNEL_0,    // Configuration + DMA_Channel TX
            DMA_HIFCR_CTCIF4,                              // TX_IT_Flag
            DMA1_Stream4,                                  // TX_DMA_TypeDef
            DMA1_Stream4_IRQn,                             // TX_IRQn
        },
      #endif
 	},
  #endif

  #if (SPI_DRIVER_SUPPORT_SPI3_CFG == DEF_ENABLED)
	{
		SPI3,                       // SPIx
        SPI_STANDARD_CONFIG,        // Configuration
		24000000,                   // The DAC chip is 12 inche away from CPU on 790A, so reduced comm speed
        IO_SPI3_CLK,                // CLK
        IO_SPI3_MOSI,               // MOSI
        IO_SPI3_MISO,               // MISO
        IO_NOT_DEFINED,             // NSS  N/U
		SPI3_IRQn,                  // IRQn_Channel

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == ENABLED)
        // DMA_RX
        {
            SPI_DMA_STANDARD_CONFIG_RX | DMA_CHANNEL_0,    // Configuration + DMA_Channel RX
            DMA_LIFCR_CTCIF0,                              // RX_IT_Flag
            DMA1_Stream0,                                  // RX_DMA_TypeDef
            DMA1_Stream0_IRQn,                             // RX_IRQn

            // Other choice
         // SPI_DMA_STANDARD_CONFIG_RX | DMA_CHANNEL_0,    // Configuration + DMA_Channel RX
         // DMA_LIFCR_CTCIF3,                              // RX_IT_Flag
         // DMA1_Stream3,                                  // RX_DMA_TypeDef
         // DMA1_Stream3_IRQn,                             // RX_IRQn
        },

        // DMA_TX
        {
            SPI_DMA_STANDARD_CONFIG_TX | DMA_CHANNEL_0,    // Configuration + DMA_Channel TX
            DMA_HIFCR_CTCIF5,                              // TX_IT_Flag
            DMA1_Stream5,                                  // TX_DMA_TypeDef
            DMA1_Stream5_IRQn,                             // TX_IRQn

            // Other choice
         // SPI_DMA_STANDARD_CONFIG_TX | DMA_CHANNEL_0,    // Configuration + DMA_Channel TX
         // DMA_HIFCR_CTCIF7,                              // TX_IT_Flag
         // DMA1_Stream7,                                  // TX_DMA_TypeDef
         // DMA1_Stream7_IRQn,                             // TX_IRQn
        },
      #endif
 	},
  #endif

  #if (SPI_DRIVER_SUPPORT_SPI4_CFG == DEF_ENABLED)
	{
		SPI4,                       // SPIx
        SPI_STANDARD_CONFIG,        // Configuration
		24000000,                   // The DAC chip is 12 inche away from CPU on 790A, so reduced comm speed
        IO_SPI4_CLK,                // CLK
        IO_SPI4_MOSI,               // MOSI
        IO_SPI4_MISO,               // MISO
        IO_NOT_DEFINED,             // NSS  N/U
		SPI4_IRQn,                  // IRQn_Channel

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == ENABLED)
        // DMA_RX
        {
            SPI_DMA_STANDARD_CONFIG_RX | DMA_CHANNEL_5,    // Configuration + DMA_Channel RX
            DMA_LIFCR_CTCIF3,                              // RX_IT_Flag
            DMA1_Stream3,                                  // RX_DMA_TypeDef
            DMA1_Stream3_IRQn,                             // TX_IRQn
            // Other choice
         // SPI_DMA_STANDARD_CONFIG_RX | DMA_CHANNEL_4,    // Configuration + DMA_Channel RX
         // DMA_LIFCR_CTCIF0,                              // RX_IT_Flag
         // DMA2_Stream0,                                  // RX_DMA_TypeDef
         // DMA2_Stream0_IRQn,                             // RX_IRQn
        },

        // DMA_TX
        {
            SPI_DMA_STANDARD_CONFIG_TX | DMA_CHANNEL_5,    // Configuration + DMA_Channel TX
            DMA_HIFCR_CTCIF4,                              // TX_IT_Flag
            DMA1_Stream4,                                  // TX_DMA_TypeDef
            DMA1_Stream4_IRQn,                             // TX_IRQn

            // Other choice
         // SPI_DMA_STANDARD_CONFIG_TX | DMA_CHANNEL_4,    // Configuration + DMA_Channel TX
         // DMA_LIFCR_CTCIF1,                              // TX_IT_Flag
         // DMA2_Stream1,                                  // TX_DMA_TypeDef
         // DMA2_Stream1_IRQn,                             // TX_IRQn
        },
      #endif
	},
  #endif

  #if (SPI_DRIVER_SUPPORT_SPI5_CFG == DEF_ENABLED)
	{
		SPI5,                       // SPIx
        SPI_STANDARD_CONFIG,        // Configuration
		24000000,                   // The DAC chip is 12 inche away from CPU on 790A, so reduced comm speed
        IO_SPI5_CLK,                // CLK
        IO_SPI5_MOSI,               // MOSI
        IO_SPI5_MISO,               // MISO
        IO_NOT_DEFINED,             // NSS  N/U
		SPI5_IRQn,                  // IRQn_Channel

      #if (SPI_DRIVER_SUPPORT_DMA_CFG == ENABLED)
        // DMA_RX
        {
            SPI_DMA_STANDARD_CONFIG_RX | DMA_CHANNEL_2,    // Configuration + DMA_Channel RX
            DMA_LIFCR_CTCIF3,                              // RX_IT_Flag
            DMA2_Stream3,                                  // RX_DMA_TypeDef
            DMA2_Stream3_IRQn,                             // RX_IRQn

            // Other choice
         // SPI_DMA_STANDARD_CONFIG_RX | DMA_CHANNEL_7,    // Configuration + DMA_Channel RX
         // DMA_HIFCR_CTCIF5,                              // RX_IT_Flag
         // DMA2_Stream5,                                  // RX_DMA_TypeDef
         // DMA2_Stream5_IRQn,                             // RX_IRQn
        },

        // DMA_TX
        {
            SPI_DMA_STANDARD_CONFIG_TX | DMA_CHANNEL_2,    // Configuration + DMA_Channel TX
            DMA_HIFCR_CTCIF4,                              // TX_IT_Flag
            DMA2_Stream4,                                  // TX_DMA_TypeDef
            DMA2_Stream4_IRQn,                             // TX_IRQn

            // Other choice
         // SPI_DMA_STANDARD_CONFIG_TX | DMA_CHANNEL_7,    // Configuration + DMA_Channel TX
         // DMA_HIFCR_CTCIF6,                              // TX_IT_Flag
         // DMA2_Stream6,                                  // TX_DMA_TypeDef
         // DMA2_Stream6_IRQn,                             // TX_IRQn
        },
      #endif
	},
  #endif

  #if (SPI_DRIVER_SUPPORT_SPI6_CFG == DEF_ENABLED)
	{
		SPI6,                       // SPIx
        SPI_STANDARD_CONFIG,        // Configuration
		24000000,                   // The DAC chip is 12 inche away from CPU on 790A, so reduced comm speed
        IO_SPI6_CLK,                // CLK
        IO_SPI6_MOSI,               // MOSI
        IO_SPI6_MISO,               // MISO
        IO_NOT_DEFINED,             // NSS  N/U
		SPI6_IRQn,                  // IRQn_Channel

      #endif
        // DMA_RX
        {
            SPI_DMA_STANDARD_CONFIG_RX | DMA_CHANNEL_1,    // Configuration + DMA_Channel RX
            DMA_HIFCR_CTCIF6,                              // RX_IT_Flag
            DMA2_Stream6,                                  // RX_DMA_TypeDef
            DMA2_Stream6_IRQn,                             // RX_IRQn
        },

        // DMA_TX
        {
            SPI_DMA_STANDARD_CONFIG_TX | DMA_CHANNEL_1,    // Configuration + DMA_Channel TX
            DMA_HIFCR_CTCIF5,                              // TX_IT_Flag
            DMA2_Stream5,                                  // TX_DMA_TypeDef
            DMA2_Stream5_IRQn,                             // TX_IRQn
        },
      #endif
	},
  #endif
};

//class SPI_Driver mySPI_ForLCD(DRIVER_SPI1_ID);        // Example

#else // SPI_DRIVER_GLOBAL

//extern class SPI_Driver     mySPI_ForLCD;             // Example

#endif // SPI_DRIVER_GLOBAL
