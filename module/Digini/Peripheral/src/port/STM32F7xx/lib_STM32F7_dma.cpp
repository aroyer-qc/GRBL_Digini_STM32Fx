//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F7_dma.cpp
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

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#define STM32F7_DMA_GLOBAL
#include "lib_STM32F7_dma.h"
#undef  STM32F7_DMA_GLOBAL

//-------------------------------------------------------------------------------------------------
// Public Function
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_ClearFlag
//
//  Parameter(s):   pDMA        DMA stream to modify
//                  Flag        to clear
//  Return:         None
//
//  Description:    Clear flag for specific DMA stream.
//-------------------------------------------------------------------------------------------------
void DMA_ClearFlag(DMA_Stream_TypeDef* pDMA, uint32_t Flag)
{
    volatile uint32_t* pRegister;

    switch((uint32_t)pDMA)
    {
        case (uint32_t)DMA1_Stream0:
        case (uint32_t)DMA1_Stream1:
        case (uint32_t)DMA1_Stream2:
        case (uint32_t)DMA1_Stream3: pRegister = &DMA1->LIFCR; break;

        case (uint32_t)DMA1_Stream4:
        case (uint32_t)DMA1_Stream5:
        case (uint32_t)DMA1_Stream6:
        case (uint32_t)DMA1_Stream7: pRegister = &DMA1->HIFCR; break;

        case (uint32_t)DMA2_Stream0:
        case (uint32_t)DMA2_Stream1:
        case (uint32_t)DMA2_Stream2:
        case (uint32_t)DMA2_Stream3: pRegister = &DMA2->LIFCR; break;

        case (uint32_t)DMA2_Stream4:
        case (uint32_t)DMA2_Stream5:
        case (uint32_t)DMA2_Stream6:
        case (uint32_t)DMA2_Stream7: pRegister = &DMA2->HIFCR; break;
    }

    SET_BIT(*pRegister, Flag);
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_CheckFlag
//
//  Parameter(s):   pDMA        DMA stream to modify
//                  Flag        To check
//  Return:         None
//
//  Description:    Check flag for specific DMA stream.
//-------------------------------------------------------------------------------------------------
uint32_t DMA_CheckFlag(DMA_Stream_TypeDef* pDMA, uint32_t Flag)
{
    volatile uint32_t Register;
    uint32_t          Result     = 0;

    switch((uint32_t)pDMA)
    {
        case (uint32_t)DMA1_Stream0:
        case (uint32_t)DMA1_Stream1:
        case (uint32_t)DMA1_Stream2:
        case (uint32_t)DMA1_Stream3: Register = DMA1->LISR; break;

        case (uint32_t)DMA1_Stream4:
        case (uint32_t)DMA1_Stream5:
        case (uint32_t)DMA1_Stream6:
        case (uint32_t)DMA1_Stream7: Register = DMA1->HISR; break;

        case (uint32_t)DMA2_Stream0:
        case (uint32_t)DMA2_Stream1:
        case (uint32_t)DMA2_Stream2:
        case (uint32_t)DMA2_Stream3: Register = DMA2->LISR; break;

        case (uint32_t)DMA2_Stream4:
        case (uint32_t)DMA2_Stream5:
        case (uint32_t)DMA2_Stream6:
        case (uint32_t)DMA2_Stream7: Register = DMA2->HISR; break;
    }

    if((Register & Flag) != 0)
    {
        Result = 1;
    }

    return Result;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_Enable
//
//  Parameter(s):   pDMA        DMA stream to enable
//  Return:         None
//
//  Description:    Enable a specific DMA stream.
//-------------------------------------------------------------------------------------------------
void DMA_Enable(DMA_Stream_TypeDef* pDMA)
{
    SET_BIT(pDMA->CR, DMA_SxCR_EN);
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_Disable
//
//  Parameter(s):   pDMA        DMA stream to enable
//  Return:         None
//
//  Description:    Disable a specific DMA stream.
//-------------------------------------------------------------------------------------------------
void DMA_Disable(DMA_Stream_TypeDef* pDMA)
{
    CLEAR_BIT(pDMA->CR, DMA_SxCR_EN);
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_EnableTransmitCompleteInterrupt
//
//  Parameter(s):   pDMA        DMA stream to enable
//  Return:         None
//
//  Description:    Enable the transmit complete interrupt for a specific DMA stream.
//-------------------------------------------------------------------------------------------------
void DMA_EnableTransmitCompleteInterrupt(DMA_Stream_TypeDef* pDMA)
{
    SET_BIT(pDMA->CR, DMA_SxCR_TCIE);
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_DisableTransmitCompleteInterrupt
//
//  Parameter(s):   pDMA        DMA stream to enable
//  Return:         None
//
//  Description:    Disable the transmit complete interrupt for a specific DMA stream.
//-------------------------------------------------------------------------------------------------
void DMA_DisableTransmitCompleteInterrupt(DMA_Stream_TypeDef* pDMA)
{
    CLEAR_BIT(pDMA->CR, DMA_SxCR_TCIE);
}

//---------------------------------------------------------------------------------------------------------------------------------
