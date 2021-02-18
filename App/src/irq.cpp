//-------------------------------------------------------------------------------------------------
//
//  File : irq.cpp
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
#include "digini_cfg.h"
#include "bsp.h"
#include "STM32F7xx.h"
#include "lib_class_sdio.h"
#include "lib_class_rtc.h"
#include "lib_class_uart.h"
#include "lib_class_i2c.h"
#include "lib_class_tim.h"
//#include "lib_class_STM32F7_usb.h"
#include "lib_pdi_task.h"

//-------------------------------------------------------------------------------------------------

extern "C"
{

//-------------------------------------------------------------------------------------------------
// Prototype(s)
//-------------------------------------------------------------------------------------------------

//void EXTI9_5_IRQHandler      ();
//void EXTI15_10_IRQHandler    ();
//void SDMMC1_IRQHandler       ();
//void DMA2_Stream3_IRQHandler ();
//void DMA2_Stream6_IRQHandler ();
//void RTC_WKUP_IRQHandler     ();
//void TAMP_STAMP_IRQHandler   ();
//void RTC_Alarm_IRQHandler    ();
//void I2C1_EV_IRQHandler      ();
//void I2C1_ER_IRQHandler      ();
//void I2C3_EV_IRQHandler      ();
//void I2C3_ER_IRQHandler      ();

//-------------------------------------------------------------------------------------------------
//
//  EEEEE XX   XX TTTTTT IIII
//  EE     XX XX    TT    II
//  EEEEE   XXX     TT    II
//  EE     XX XX    TT    II
//  EEEEE XX   XX   TT   IIII
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Handler:        EXTI9_5_IRQHandler
//
//  Description:   Handle interrupt from line 5-9
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
NOS_ISR(EXTI9_5_IRQHandler)
{
    //if((EXTI->PR & IO_PIN_MASK_5) != 0)
    //{
    //    EXTI->PR = IO_PIN_MASK_5;
        //    BSP_pUSB->OverCurrent_IRQHandler();
    //}

    // For SD Card Detect  // todo this need rework to follow new IRQ handling by IO function
  #ifdef DIGINI_FATFS_USE_SD_CARD
   #if SD_CARD_USE_DETECT_SIGNAL == 1
    if((EXTI->PR & IO_PIN_MASK_6) != 0)
    {
        EXTI->PR = IO_PIN_MASK_6;

// TODO need to handle the insert card detection!!!


      #if (SD_CARD_USE_DETECT_SIGNAL == 1)
//        if(BSP_pSD_Card != nullptr)
//        {
//            BSP_pSD_Card->CardDetectIRQ_Handler();
//        }
      #endif
    }
   #endif
  #endif

    if((EXTI->PR & IO_PIN_MASK_8) != 0)
    {
        EXTI->PR = IO_PIN_MASK_8;
        IO_CallBack(IO_ISR_LIMIT_Y);
    }


        EXTI->PR = IO_PIN_MASK_9;  // bug
}


//-------------------------------------------------------------------------------------------------
//
//  Handler:        EXTI15_10_IRQHandler
//
//  Description:   Handle interrupt from line 10-15
//                      - LCD touch interrupt           GPIO13
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
NOS_ISR(EXTI15_10_IRQHandler)
{
  #ifdef GRAFX_PDI_INTERRUPT_IO
    if((EXTI->PR & IO_PIN_MASK_13) != 0)
    {
        EXTI->PR = IO_PIN_MASK_13;
        PDI_pTask->IRQ_Handler();
    }
  #endif

    if((EXTI->PR & IO_PIN_MASK_15) != 0)
    {
        EXTI->PR = IO_PIN_MASK_15;
        IO_CallBack(IO_ISR_LIMIT_X);    // It also serve IO_ISR_LIMIT_Z as they share EXTI15 line
    }
}

//-------------------------------------------------------------------------------------------------
//
//   SSSSS  DDDDD  IIII  OOOO
//  SS      DD  DD  II  OO  OO
//   SSSSS  DD  DD  II  OO  OO
//       SS DD  DD  II  OO  OO
//   SSSSS  DDDDD  IIII  OOOO
//
//-------------------------------------------------------------------------------------------------

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
#ifdef DIGINI_USE_SDIO
NOS_ISR(SDMMC1_IRQHandler)
{
    // Process All SDIO Interrupt Sources
    pSDIO->SDMMC1_IRQHandler();
}

NOS_ISR(DMA2_Stream3_IRQHandler)
{
    // Process DMA2 Stream3
    pSDIO->DMA_Stream3IRQHandler();
}

NOS_ISR(DMA2_Stream6_IRQHandler)
{
    // Process DMA2 Stream6
    pSDIO->DMA_Stream6IRQHandler();
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  RRRRR  TTTTTT  CCCC
//  RR  RR   TT   CC
//  RRRRR    TT   CC
//  RR RR    TT   CC
//  RR  RR   TT    CCCC
//
//-------------------------------------------------------------------------------------------------

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
#ifdef DIGINI_USE_RTC
NOS_ISR(RTC_WKUP_IRQHandler)
{
    if(BSP_pRTC != nullptr)
    {
        BSP_pRTC->WakeUp_IRQ_Handler();
    }
}

NOS_ISR(TAMP_STAMP_IRQHandler)
{
    if(BSP_pRTC != nullptr)
    {
        BSP_pRTC->Stamp_IRQ_Handler();
    }
}

NOS_ISR(RTC_Alarm_IRQHandler)
{
    if(BSP_pRTC != nullptr)
    {#ifdef DIGINI_USE_RTC

        BSP_pRTC->Alarm_IRQ_Handler();
    }
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
#ifdef DIGINI_USE_USB
NOS_ISR(OTG_HS_IRQHandler)
{
    //USB.IRQHandler();
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  IIII  2222   CCCC
//   II      22 CC
//   II   2222  CC
//   II   22    CC
//  IIII  22222  CCCC
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Handler:        I2Cx_EV_IRQHandler and I2Cx_ER_IRQHandler
//
//  Description:    Handle interrupt I2Cx Event handler
//
//  Note(s):        Wrapper for C++ for handler
//
//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to I2C1
#if (I2C_DRIVER_SUPPORT_I2C1 == DEF_ENABLED)

NOS_ISR(I2C1_EV_IRQHandler)
{
    myI2C_External.EV_IRQHandler();
}

NOS_ISR(I2C1_ER_IRQHandler)
{
    myI2C_External.ER_IRQHandler();
}

#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to I2C2
#if (I2C_DRIVER_SUPPORT_I2C2 == DEF_ENABLED)

NOS_ISR(I2C2_EV_IRQHandler)
{
    // <Your c++ class object>.EV_IRQHandler();
}

NOS_ISR(I2C2_ER_IRQHandler)
{
    // <Your c++ class object>.ER_IRQHandler();
}

#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to I2C3
#if (I2C_DRIVER_SUPPORT_I2C3 == DEF_ENABLED)

NOS_ISR(I2C3_EV_IRQHandler)
{
    myI2C_Control.EV_IRQHandler();
}

NOS_ISR(I2C3_ER_IRQHandler)
{
    myI2C_Control.ER_IRQHandler();
}

#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to I2C4
#if (I2C_DRIVER_SUPPORT_I2C4 == DEF_ENABLED)

NOS_ISR(I2C4_EV_IRQHandler)
{
    // <Your c++ class object>.EV_IRQHandler();
}

NOS_ISR(I2C4_ER_IRQHandler)
{
    // <Your c++ class object>.ER_IRQHandler();
}

#endif

//-------------------------------------------------------------------------------------------------
//
//  UU  UU  AAAA  RRRRR  TTTTTT
//  UU  UU AA  AA RR  RR   TT
//  UU  UU AAAAAA RRRRR    TT
//  UU  UU AA  AA RR RR    TT
//   UUUU  AA  AA RR  RR   TT
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:   External IRQ handler
//
//  Description:   Individual handle for each enable UART
//
//  Note(s):       There is virtual UART IRQ handler (Hijack unused IRQ from the system)
//                 and use software interrupt trigger
//
//-------------------------------------------------------------------------------------------------


// Interrupt Handler assign to USART1
#if (UART_DRIVER_SUPPORT_UART1_CFG == DEF_ENABLED)
NOS_ISR(USART1_IRQHandler)
{
    myUART_Terminal.IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to USART2
#if (UART_DRIVER_SUPPORT_UART2_CFG == DEF_ENABLED)
NOS_ISR(USART2_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to USART3
#if (UART_DRIVER_SUPPORT_UART3_CFG == DEF_ENABLED)
NOS_ISR(USART3_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to UART4
#if (UART_DRIVER_SUPPORT_UART4 == DEF_ENABLED)
NOS_ISR(UART4_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to UART5
#if (UART_DRIVER_SUPPORT_UART5 == DEF_ENABLED)
NOS_ISR(UART5_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to USART6
#if (UART_DRIVER_SUPPORT_UART6 == DEF_ENABLED)
NOS_ISR(USART6_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to UART7
#if (UART_DRIVER_SUPPORT_UART7 == DEF_ENABLED)
NOS_ISR(UART7_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to UART8
#if (UART_DRIVER_SUPPORT_UART8 == DEF_ENABLED)
NOS_ISR(UART8_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to Virtual RX UART
#if (SUPPORT_VIRTUAL_UART == DEF_ENABLED)
//Example -> NOS_ISR(SPDIF_RX_IRQHandler)
{
    // _your_class_object_here__->VirtualUartRX_IRQHandler();
}
#endif

//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to Virtual TX UART
#if (SUPPORT_VIRTUAL_UART == DEF_ENABLED)
//Example -> NOS_ISR(SPDIF_TX_IRQHandler)
{
    // _your_class_object_here__->VirtualUartTX_IRQHandler();
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  TTTTTT IIII MM    MM
//    TT    II  MMM  MMM
//    TT    II  MMMMMMMM
//    TT    II  MM MM MM
//    TT   IIII MM    MM
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:   External IRQ handler
//
//  Description:   Individual handle for each enable TIM
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------


#if (TIM_DRIVER_SUPPORT_TIM1_CFG == DEF_ENABLED) || (TIM_DRIVER_SUPPORT_TIM10_CFG == DEF_ENABLED)
NOS_ISR(TIM1_UP_TIM10_IRQHandler)
{
  #if (TIM_DRIVER_SUPPORT_TIM1_CFG == DEF_ENABLED)
    if((TIM1->SR & TIM_SR_UIF) != 0)
    {
        if(myTIM_GRBL_Spindle.m_pCallBack != nullptr)
        {
            myTIM_GRBL_Spindle.m_pCallBack(TIM_MATCH_UPDATE);
            CLEAR_BIT(TIM1->SR, TIM_SR_UIF);
        }
    }
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM10_CFG == DEF_ENABLED)
    /* Your class objet here */.CallBack(true);
  #endif
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM1_CFG == DEF_ENABLED) && (TIM_DRIVER_SUPPORT_TIM1_COMPARE_CFG == DEF_ENABLED)
NOS_ISR(TIM1_CC_IRQHandler)
{
    /* Your class objet here */.CallBack(false);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM2_CFG == DEF_ENABLED)
NOS_ISR(TIM2_IRQHandler)
{
    myTIM_GRBL_LimitsDebounce.CallBack(true);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM3_CFG == DEF_ENABLED)
NOS_ISR(TIM3_IRQHandler)
{
    /* Your class objet here */.CallBack(true);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM4_CFG == DEF_ENABLED)
NOS_ISR(TIM4_IRQHandler)
{
    /* Your class objet here */.CallBack(true);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM5_CFG == DEF_ENABLED)
NOS_ISR(TIM5_IRQHandler)
{
    /* Your class objet here */.CallBack(true);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM6_CFG == DEF_ENABLED)
NOS_ISR(TIM6_DAC_IRQHandler)
{
    /* Your class objet here */.CallBack(true);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM7_CFG == DEF_ENABLED)
NOS_ISR(TIM7_IRQHandler)
{
    /* Your class objet here */.CallBack(true);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM8_CFG == DEF_ENABLED) || (TIM_DRIVER_SUPPORT_TIM13_CFG == DEF_ENABLED)
NOS_ISR(TIM8_UP_TIM13_IRQHandler)
{
   #if (TIM_DRIVER_SUPPORT_TIM8_CFG == DEF_ENABLED)
     if((TIM8->SR & TIM_SR_UIF) != 0)
     {
        if(/* Your class objet here */.m_pCallBack!= nullptr)
        {
            /* Your class objet here */.m_pCallBack(TIM_MATCH_UPDATE);
            CLEAR_BIT(TIM8->SR, TIM_SR_UIF);
        }
     }
    #endif

  #if (TIM_DRIVER_SUPPORT_TIM13_CFG == DEF_ENABLED)
    /* Your class objet here */.CallBack(true);
  #endif
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM8_CFG == DEF_ENABLED) && (TIM_DRIVER_SUPPORT_TIM8_COMPARE_CFG == DEF_ENABLED)
NOS_ISR(TIM8_CC_IRQHandler)
{
    /* Your class objet here */.CallBack(false);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM9_CFG == DEF_ENABLED)
NOS_ISR(TIM1_BRK_TIM9_IRQHandler)
{
    myTIM_GRBL_Stepper.CallBack(true);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM11_CFG == DEF_ENABLED)
NOS_ISR(TIM1_TRG_COM_TIM11_IRQHandler)
{
    /* Your class objet here */.CallBack(true);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM12_CFG == DEF_ENABLED)
NOS_ISR(TIM8_BRK_TIM12_IRQHandler)
{
    /* Your class objet here */.m_pCallBack(TIM_MATCH_UPDATE);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_TIM14_CFG == DEF_ENABLED)
NOS_ISR(TIM8_TRG_COM_TIM14_IRQHandler)
{
    /* Your class objet here */.CallBack(true);
}
#endif

//-------------------------------------------------------------------------------------------------

#if (TIM_DRIVER_SUPPORT_LPTIM1_CFG == DEF_ENABLED)
NOS_ISR(LPTIM1_IRQHandler)
{
    uint32_t Status = LPTIM1->ISR;
    uint32_t Enable = LPTIM1->IER;

    if(/* Your class objet here */.m_pCallBack != nullptr)
    {
      #if (TIM_DRIVER_SUPPORT_LPTIM1_COMPARE_CFG == DEF_ENABLED)
        if(((Status & LPTIM_ISR_CMPM) != 0) && ((Enable & LPTIM_ISR_CMPM) != 0))
        {
            /* Your class objet here */.m_pCallBack(TIM_MATCH_CH1);
           LPTIM1->ICR = LPTIM_ICR_CMPMCF;
        }

        if(((Status & LPTIM_ISR_ARRM) != 0) && ((Enable & LPTIM_ISR_ARRM) != 0))
      #endif
        {
             /* Your class objet here */.m_pCallBack(TIM_MATCH_UPDATE);
            LPTIM1->ICR = LPTIM_ICR_ARRMCF;
        }
    }
}
#endif

//-------------------------------------------------------------------------------------------------

} // extern "C"

//-------------------------------------------------------------------------------------------------