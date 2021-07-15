//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_irq.cpp
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

// TODO move specific usr or BSP stuff outside of this file

// Example use function call to user IRQ handler


//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#include <stdint.h>
#include "bsp.h"
#include "STM32F4xx.h"
//#include "lib_class_sd_card.h"
#include "lib_class_STM32F4_i2c.h"
#include "lib_class_STM32F4_sdio.h"
#include "lib_class_STM32F4_rtc.h"
#include "lib_class_STM32F4_usart.h"
#include "lib_class_STM32F4_usb.h"

//-------------------------------------------------------------------------------------------------
// Prototype(s)
//-------------------------------------------------------------------------------------------------
extern "C" void EXTI9_5_IRQHandler      ();

extern "C" void I2C1_EV_IRQHandler      ();             // I2C1 Event
//extern "C" void I2C2_EV_IRQHandler    ();             // I2C2 Event
//extern "C" void I2C3_EV_IRQHandler    ();             // I2C3 Event

extern "C" void I2C1_ER_IRQHandler      ();             // I2C2 Error
//extern "C" void I2C2_ER_IRQHandler    ();             // I2C1 Error
//extern "C" void I2C3_ER_IRQHandler    ();             // I2C3 Error

extern "C" void SDIO_IRQHandler         ();
extern "C" void DMA2_Stream3_IRQHandler ();
extern "C" void DMA2_Stream6_IRQHandler ();

extern "C" void RTC_WKUP_IRQHandler     ();
extern "C" void TAMP_STAMP_IRQHandler   ();
extern "C" void RTC_Alarm_IRQHandler    ();

//-------------------------------------------------------------------------------------------------
//
//  Handler:        EXTI9_5_IRQHandler
//
//  Description:   Handle interrupt from line 5-9
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
extern "C" void EXTI9_5_IRQHandler()
{
    // portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
    //    BSP_pUSB->OverCurrent_IRQHandler();
    }


    // For SD Card Detect
  #ifdef DIGINI_FATFS_USE_SD_CARD
   #if SD_CARD_USE_DETECT_SIGNAL == 1
    if(EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line6);

      #if (SD_CARD_USE_DETECT_SIGNAL == 1) && (SD_CARD_USE_POLLING_DETECT_SIGNAL == 0)
//        if(BSP_pSD_Card != nullptr)
//        {
//            BSP_pSD_Card->CardDetectIRQ_Handler();
//        }
      #endif
    }
   #endif
  #endif

    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {


        //xSemaphoreGiveFromISR(xF.Handle.FlagTouchDetected, &xHigherPriorityTaskWoken);
        EXTI_ClearITPendingBit(EXTI_Line8);
        //portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}


//-------------------------------------------------------------------------------------------------
//
//  Handler:        I2C1_EV_IRQHandler
//
//  Description:    Handle interrupt I2C1 Event handler
//
//  Note(s):        Wrapper for C++ for handler
//
//-------------------------------------------------------------------------------------------------
#if USE_I2C_DRIVER == DEF_ENABLED
extern "C" void I2C1_EV_IRQHandler()             // I2C1 Event
{
    I2C_Port1.EV_IRQHandler();
}

//-------------------------------------------------------------------------------------------------
//
//  Handler:        I2C1_ER_IRQHandler
//
//  Description:    Handle interrupt I2C1 Error handler
//
//  Note(s):        Wrapper C++ for handler
//
//-------------------------------------------------------------------------------------------------
extern "C" void I2C1_ER_IRQHandler()
{
    I2C_Port1.ER_IRQHandler();
}

//-------------------------------------------------------------------------------------------------
//
//  Handler:        I2C2_EV_IRQHandler
//
//  Description:    Handle interrupt I2C2 Event handler
//
//  Note(s):        Wrapper C++ for handler
//
//-------------------------------------------------------------------------------------------------
/*
void I2C2_EV_IRQHandler()
{
    if(pI2C2_EV_IRQHandler != nullptr)
    {
        pI2C2_EV_IRQHandler();
    }
}
*/


//-------------------------------------------------------------------------------------------------
//
//  Handler:        I2C2_ER_IRQHandler
//
//  Description:    Handle interrupt I2C2 Error handler
//
//  Note(s):        Wrapper C++ for handler
//
//-------------------------------------------------------------------------------------------------
/*
void I2C2_ER_IRQHandler()             // I2C2 Error
{
    if(pI2C2_ER_IRQHandler != NULL)
    {
        pI2C2_ER_IRQHandler();
    }
}
*/


//-------------------------------------------------------------------------------------------------
//
//  Handler:        I2C3_EV_IRQHandler
//
//  Description:    Handle interrupt I2C3 Event handler
//
//  Note(s):        Wrapper C++ for handler
//
//-------------------------------------------------------------------------------------------------
/*
void I2C3_EV_IRQHandler()
{
    if(pI2C3_EV_IRQHandler != nullptr)
    {
        pI2C3_EV_IRQHandler();
    }
}
*/


//-------------------------------------------------------------------------------------------------
//
//  Handler:        I2C3_ER_IRQHandler
//
//  Description:    Handle interrupt I2C3 Error handler
//
//  Note(s):        Wrapper C++ for handler
//
//-------------------------------------------------------------------------------------------------
/*
void I2C3_ER_IRQHandler()             // I2C3 Error
{
    if(pI2C3_ER_IRQHandler != nullptr)
    {
        pI2C3_ER_IRQHandler();
    }
}
*/

#endif // USE_I2C_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
//
//  Handler:        SDIO_IRQHandler
//  Handler:        DMA2_Stream3_IRQHandler
//  Handler:        DMA2_Stream6_IRQHandler
//
//  Description:    Handle interrupt for SDIO module
//
//  Note(s):        Wrapper C++ for handler
//
//-------------------------------------------------------------------------------------------------
#ifdef DIGINI_FATFS_USE_SD_CARD
/*
extern "C" void SDIO_IRQHandler()
{
    // Process All SDIO Interrupt Sources
    SDIO_Port.SDIO_IRQHandler();
}

extern "C" void DMA2_Stream3_IRQHandler()
{
    // Process DMA2 Stream3
    SDIO_Port.DMA_StreamIRQHandler();
}

extern "C" void DMA2_Stream6_IRQHandler()
{
    // Process DMA2 Stream6
    SDIO_Port.DMA_StreamIRQHandler();
}
*/
#endif
//-------------------------------------------------------------------------------------------------
//
//  Handler:        RTC_WKUP_IRQHandler
//  Handler:        TAMP_STAMP_IRQHandler
//  Handler:        RTC_Alarm_IRQHandler
//
//  Description:    Handle interrupt for RTC module
//
//  Note(s):        Wrapper C++ for handler
//
//-------------------------------------------------------------------------------------------------
#if USE_RTC_DRIVER == DEF_ENABLED

extern "C" void RTC_WKUP_IRQHandler()
{
    if(BSP_pRTC != nullptr)
    {
        BSP_pRTC->WakeUp_IRQ_Handler();
    }
}

extern "C" void TAMP_STAMP_IRQHandler()
{
    if(BSP_pRTC != nullptr)
    {
        BSP_pRTC->Stamp_IRQ_Handler();
    }
}

extern "C" void RTC_Alarm_IRQHandler()
{
    if(BSP_pRTC != nullptr)
    {
        BSP_pRTC->Alarm_IRQ_Handler();
    }
}

#endif

//-------------------------------------------------------------------------------------------------
//
//  Handler:        USART1_IRQHandler
//
//  Description:    Handle interrupt for USART module
//
//  Note(s):        Wrapper C++ for handler
//
//-------------------------------------------------------------------------------------------------
#if USE_UART_DRIVER == DEF_ENABLED

extern "C" void USART1_IRQHandler()
{
    USART_Port1.IRQHandler();
}

#endif

//-------------------------------------------------------------------------------------------------
//
//  Handler:        USBH_OTG_ISR_IRQHandler
//
//  Description:    Handle interrupt for USART module
//
//  Note(s):        Wrapper C++ for handler
//
//-------------------------------------------------------------------------------------------------
#if USE_USB_DRIVER == DEF_ENABLED

extern "C" void OTG_HS_IRQHandler()
{
    USB.IRQHandler();
}

#endif
