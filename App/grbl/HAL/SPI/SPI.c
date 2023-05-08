#include "SPI.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_gpio.h"


void Spi_Init(SPI_TypeDef *SPIx, SPI_Mode mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	switch(mode)
	{
	case SPI_MODE1:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		break;

	case SPI_MODE2:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		break;

	case SPI_MODE3:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		break;

	default:
		// Mode0 default
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		break;
	}

	if(SPI1 == SPIx) {
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		// Periph clock enable
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

		GPIO_PinAFConfig(GPIOA, GPIO_PIN_5, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PIN_6, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PIN_7, GPIO_AF_SPI1);

		// Configure SPI pins: SCK, MOSI
		GPIO_InitStructure.Pin = GPIO_PIN_5 | GPIO_PIN_7;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FAST// GPIO_Speed_100MHz;
		GPIO_InitStructure.PuPd  = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

		// Configure pins: MISO
		GPIO_InitStructure.Pin = GPIO_PIN_6;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

		// Reset SPI Interface
		SPI_I2S_DeInit(SPIx);

		// SPI configuration
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPIx, &SPI_InitStructure);
	}
	else if(SPI2 == SPIx) {
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		// Periph clock enable
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

		GPIO_PinAFConfig(GPIOB, GPIO_PIN_13, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PIN_14, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PIN_15, GPIO_AF_SPI2);

		// Configure SPI pins: SCK, MOSI
		GPIO_InitStructure.Pin = GPIO_PIN_13 | GPIO_PIN_15;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
		GPIO_InitStructure.PuPd  = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

		// Configure pins: MISO
		GPIO_InitStructure.Pin = GPIO_PIN_14;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

		// Reset SPI Interface
		SPI_I2S_DeInit(SPIx);

		// SPI configuration
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPIx, &SPI_InitStructure);
	}
	else if(SPI3 == SPIx) {
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		// Periph clock enable
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

		GPIO_PinAFConfig(GPIOC, GPIO_PIN_10, GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOC, GPIO_PIN_11, GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOC, GPIO_PIN_12, GPIO_AF_SPI3);

		// Configure SPI pins: SCK, MOSI
		GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_12;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
		GPIO_InitStructure.PuPd  = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

		// Configure pins: MISO
		GPIO_InitStructure.Pin = GPIO_PIN_11;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

		// Reset SPI Interface
		SPI_I2S_DeInit(SPIx);

		// SPI configuration
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPIx, &SPI_InitStructure);

		// Initialize chip select
		RCC_AHB1PeriphClockCmd(SPI3_CS_GPIO_CLK, ENABLE);

        // Configure CS pin
        GPIO_InitStructure.Pin = SPI3_CS_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
		GPIO_InitStructure.PuPd  = GPIO_NOPULL;
		HAL_GPIO_Init(SPI3_CS_GPIO_PORT, &GPIO_InitStructure);

        // Deselect chip
        HAL_GPIO_WritePin(SPI3_CS_GPIO_PORT, SPI3_CS_PIN, GPIO_PIN_SET);
	}

	SPI_CalculateCRC(SPIx, DISABLE);

	// Enable the SPI
	SPI_Cmd(SPIx, ENABLE);
}


uint8_t Spi_ReadByte(SPI_TypeDef *SPIx)
{
	return Spi_WriteByte(SPIx, 0xFF);
}


uint8_t Spi_WriteByte(SPI_TypeDef *SPIx, uint8_t _data)
{
    uint16_t timeout = 0xFFF;

	// Loop while DR register is not empty
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	// Send byte through the SPIx peripheral
	SPI_I2S_SendData(SPIx, _data);

	while((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) && timeout--);

	// Return the byte read from the SPI bus
	return (uint8_t)SPI_I2S_ReceiveData(SPIx);
}


void Spi_ReadByteArray(SPI_TypeDef *SPIx, uint8_t *_buffer, uint8_t _len)
{
	uint8_t i = 0;
	uint16_t timeout = 0xFFF;

	for(i = 0; i < _len; ++i)
    {
		// Loop while DR register is not empty
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

		// Send byte through the SPIx peripheral
		SPI_I2S_SendData(SPIx, 0xFF);

		while((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) && timeout--);

		_buffer[i] = (uint8_t)SPI_I2S_ReceiveData(SPIx);
	}
}

void Spi_WriteDataArray(SPI_TypeDef *SPIx, uint8_t *_data, uint8_t _len)
{
	uint8_t i = 0;

	for(i = 0; i < _len; ++i)
    {
		// Loop while DR register is not empty
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

		// Send byte through the SPIx peripheral
		SPI_I2S_SendData(SPIx, _data[i]);
	}
}


void Spi_SetPrescaler(SPI_TypeDef *SPIx, uint16_t prescaler)
{
    SPI_Cmd(SPIx, DISABLE);

    // Read CR1 and clear baud control
    uint16_t tmpreg = SPIx->CR1 & 0xFFC7;

    tmpreg |= prescaler;

    SPIx->CR1 = tmpreg;

    SPI_Cmd(SPIx, ENABLE);
}


void Spi_ChipSelect(SPI_TypeDef *SPIx, bool select)
{
    if(select)
    {
        if(SPIx == SPI1)
        {
            HAL_GPIO_WritePin(SPI1_CS_GPIO_PORT, SPI1_CS_PIN, GPIO_PIN_RESET);
        }
        else if(SPIx == SPI2)
        {
            HAL_GPIO_WritePin(SPI2_CS_GPIO_PORT, SPI2_CS_PIN, GPIO_PIN_RESET);
        }
        else if(SPIx == SPI3)
        {
            HAL_GPIO_WritePin(SPI3_CS_GPIO_PORT, SPI3_CS_PIN, GPIO_PIN_RESET);
        }
    }
    else
    {
        if(SPIx == SPI1)
        {
            HAL_GPIO_WritePin(SPI1_CS_GPIO_PORT, SPI1_CS_PIN, GPIO_PIN_SET);
        }
        else if(SPIx == SPI2)
        {
            HAL_GPIO_WritePin(SPI2_CS_GPIO_PORT, SPI2_CS_PIN, GPIO_PIN_SET);
        }
        else if(SPIx == SPI3)
        {
            HAL_GPIO_WritePin(SPI3_CS_GPIO_PORT, SPI3_CS_PIN, GPIO_PIN_SET);
        }
    }
}
