/*
  USART.c - USART implementation
  Part of STM32F4_HAL

  Copyright (c)	2017 Patrick F.

  STM32F4_HAL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  STM32F4_HAL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with STM32F4_HAL.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include "Usart.h"
#include "FIFO_USART.h"


static uint8_t FifoInit = 0;


void Usart_Init(USART_TypeDef *usart, uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	if(!FifoInit) {
		// Initialize fifo once
		FifoUsart_Init();
		FifoInit = 1;
	}

	if(usart == USART1) {
		/* Enable GPIO clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		GPIO_PinAFConfig(GPIOA, GPIO_PIN_9, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOA, GPIO_PIN_10, GPIO_AF_USART1);

		/* Configure USART Tx RX as alternate function push-pull */
		GPIO_InitStructure.Mode = GPIO_Mode_AF;
		GPIO_InitStructure.Pin = GPIO_PIN_9 | GPIO_PIN_10;
		GPIO_InitStructure.Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.OType = GPIO_OType_PP;
		GPIO_InitStructure.PuPd = GPIO_PuPd_UP;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

		USART_OverSampling8Cmd(USART1, ENABLE);

		USART_InitStructure.USART_BaudRate = baud;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		/* USART configuration */
		USART_Init(USART1, &USART_InitStructure);

		NVIC_InitTypeDef NVIC_InitStructure;

		/* Enable the USARTx Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

	} else if(usart == USART2) {
		/* Enable GPIO clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		GPIO_PinAFConfig(GPIOA, GPIO_PIN_2, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PIN_3, GPIO_AF_USART2);

		/* Configure USART Tx as alternate function push-pull */
		GPIO_InitStructure.Mode = GPIO_Mode_AF;
		GPIO_InitStructure.Pin = GPIO_PIN_2 | GPIO_PIN_3;
		GPIO_InitStructure.Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.OType = GPIO_OType_PP;
		GPIO_InitStructure.PuPd = GPIO_PuPd_UP;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* Enable the USART OverSampling by 8 */
		USART_OverSampling8Cmd(USART2, ENABLE);

		USART_InitStructure.USART_BaudRate = baud;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		/* USART configuration */
		USART_Init(USART2, &USART_InitStructure);

		NVIC_InitTypeDef NVIC_InitStructure;

		/* Enable the USARTx Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

	} else if(usart == USART6) {
		/* Enable GPIO clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		GPIO_PinAFConfig(GPIOC, GPIO_PIN_6, GPIO_AF_USART6);
		GPIO_PinAFConfig(GPIOC, GPIO_PIN_7, GPIO_AF_USART6);

		/* Configure USART Tx as alternate function push-pull */
		GPIO_InitStructure.Mode = GPIO_Mode_AF;
		GPIO_InitStructure.Pin = GPIO_PIN_6;
		GPIO_InitStructure.Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.OType = GPIO_OType_PP;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

		/* Configure USART Rx as input floating */
		GPIO_InitStructure.Mode = GPIO_Mode_AF;
		GPIO_InitStructure.Pin = GPIO_PIN_7;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

		USART_InitStructure.USART_BaudRate = baud;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		/* USART configuration */
		USART_Init(USART6, &USART_InitStructure);

		NVIC_InitTypeDef NVIC_InitStructure;

		/* Enable the USARTx Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}

	/* Enable the Receive interrupt*/
	USART_ITConfig(usart, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(usart, ENABLE);
}

void Usart_Put(USART_TypeDef *usart, bool buffered, char c)
{
    uint8_t num = 0;

    if(usart == USART1)
    {
        num = USART1_NUM;
    }
    else if(usart == USART2)
    {
        num = USART2_NUM;
    }
    else if(usart == USART6)
    {
        num = USART6_NUM;
    }

	if(buffered)
    {
        FifoUsart_Insert(num, USART_DIR_TX, c);

        // Enable sending via interrupt
        Usart_TxInt(usart, true);
    }
    else
    {
        while(USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET);
		USART_SendData(usart, c);
    }
}

void Usart_Write(USART_TypeDef *usart, bool buffered, char *data, uint8_t len)
{
	uint8_t i = 0;
	uint8_t num = 0;

    if(usart == USART1)
    {
        num = USART1_NUM;
    }
    else if(usart == USART2)
    {
        num = USART2_NUM;
    }
    else if(usart == USART6)
    {
        num = USART6_NUM;
    }

    if(buffered)
    {
        while(len--)
        {
            FifoUsart_Insert(num, USART_DIR_TX, data[i++]);
        }

        // Enable sending via interrupt
        Usart_TxInt(usart, true);
    }
    else
    {
		while(len--)
        {
            while(USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET);
            USART_SendData(usart, data[i++]);
        }
    }
}

void Usart_TxInt(USART_TypeDef *usart, bool enable)
{
	if(enable)
	{
		USART_ITConfig(usart, USART_IT_TXE, ENABLE);
	}
	else
	{
		USART_ITConfig(usart, USART_IT_TXE, DISABLE);
	}
}

void Usart_RxInt(USART_TypeDef *usart, bool enable)
{
	if(enable)
	{
		USART_ITConfig(usart, USART_IT_RXNE, ENABLE);
	}
	else
	{
		USART_ITConfig(usart, USART_IT_RXNE, DISABLE);
	}
}

