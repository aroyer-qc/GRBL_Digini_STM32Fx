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

#include "bsp.h"

//-------------------------------------------------------------------------------------------------

extern "C"
{

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
  #if (DIGINI_FATFS_USE_SD_CARD == DEF_ENABLED)
   #if (SD_CARD_USE_DETECT_SIGNAL == DEF_ENABLED)
    if((EXTI->PR & IO_PIN_MASK_6) != 0)
    {
        EXTI->PR = IO_PIN_MASK_6;

// TODO need to handle the insert card detection!!!


      #if (SD_CARD_USE_DETECT_SIGNAL == DEF_ENABLED)
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
        IO_CallBack(IO_ISR_LIMIT_Y1);
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
        IO_CallBack(IO_ISR_LIMIT_X1);    // It also serve IO_ISR_LIMIT_Z as they share EXTI15 line
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
#if (USE_SDIO_DRIVER == DEF_ENABLED)
NOS_ISR(SDMMC1_IRQHandler)
{
    // Process All SDIO Interrupt Sources
    mySDIO.SDMMC1_IRQHandler();
}

NOS_ISR(DMA2_Stream3_IRQHandler)
{
    // Process DMA2 Stream3
    mySDIO.DMA_Stream3IRQHandler();
}

NOS_ISR(DMA2_Stream6_IRQHandler)
{
    // Process DMA2 Stream6
    mySDIO.DMA_Stream6IRQHandler();
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

#if (USE_RTC_DRIVER  == DEF_ENABLED)
NOS_ISR(RTC_WKUP_IRQHandler)
{
    myRTC.WakeUp_IRQ_Handler();
}

NOS_ISR(TAMP_STAMP_IRQHandler)
{
    myRTC.Stamp_IRQ_Handler();
}

NOS_ISR(RTC_Alarm_IRQHandler)
{
    myRTC.Alarm_IRQ_Handler();
}
#endif // USE_RTC_DRIVER

//-------------------------------------------------------------------------------------------------
//
//  Handler:        USBH_OTG_ISR_IRQHandler
//
//  Description:    Handle interrupt for USART module
//
//  Note(s):        Wrapper C++ for handler
//
//-------------------------------------------------------------------------------------------------
#if (USE_USB_DRIVER == DEF_ENABLED)
NOS_ISR(OTG_HS_IRQHandler)
{
    //USB.IRQHandler();
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

#if (USE_TIM_DRIVER == DEF_ENABLED)

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
   // myTIM_GRBL_LimitsDebounce.CallBack(true);
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

#endif // USE_TIM_DRIVER


//-------------------------------------------------------------------------------------------------
//
//  IIII  2222   CCCC
//   II      22 CC
//   II   2222  CC
//   II   22    CC
//  IIII  22222  CCCC
//
//-------------------------------------------------------------------------------------------------

#if (USE_I2C_DRIVER == DEF_ENABLED)

//-------------------------------------------------------------------------------------------------
//  Handler:        I2Cx_EV_IRQHandler and I2Cx_ER_IRQHandler
//
//  Description:    Handle interrupt I2Cx Event handler
//
//  Note(s):        Wrapper for C++ for handler
//
//-------------------------------------------------------------------------------------------------

// Interrupt Handler assign to I2C1
#if (I2C_DRIVER_SUPPORT_I2C1_CFG == DEF_ENABLED)

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
#if (I2C_DRIVER_SUPPORT_I2C2_CFG == DEF_ENABLED)

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
#if (I2C_DRIVER_SUPPORT_I2C3_CFG == DEF_ENABLED)

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
#if (I2C_DRIVER_SUPPORT_I2C4_CFG == DEF_ENABLED)

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

#endif // USE_I2C_DRIVER

//-------------------------------------------------------------------------------------------------
//
//  UU  UU  AAAA  RRRRR  TTTTTT
//  UU  UU AA  AA RR  RR   TT
//  UU  UU AAAAAA RRRRR    TT
//  UU  UU AA  AA RR RR    TT
//   UUUU  AA  AA RR  RR   TT
//
//-------------------------------------------------------------------------------------------------

#if (USE_UART_DRIVER == DEF_ENABLED)

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

#if (UART_DRIVER_SUPPORT_UART1_CFG == DEF_ENABLED)
NOS_ISR(USART1_IRQHandler)
{
    myUART_Terminal.IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

#if (UART_DRIVER_SUPPORT_UART2_CFG == DEF_ENABLED)
NOS_ISR(USART2_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

#if (UART_DRIVER_SUPPORT_UART3_CFG == DEF_ENABLED)
NOS_ISR(USART3_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

#if (UART_DRIVER_SUPPORT_UART4_CFG == DEF_ENABLED)
NOS_ISR(UART4_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

#if (UART_DRIVER_SUPPORT_UART5_CFG == DEF_ENABLED)
NOS_ISR(UART5_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

#if (UART_DRIVER_SUPPORT_UART6_CFG == DEF_ENABLED)
NOS_ISR(USART6_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

#if (UART_DRIVER_SUPPORT_UART7_CFG == DEF_ENABLED)
NOS_ISR(UART7_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

#if (UART_DRIVER_SUPPORT_UART8_CFG == DEF_ENABLED)
NOS_ISR(UART8_IRQHandler)
{
    // _your_class_object_here__->IRQ_Handler();
}
#endif

//-------------------------------------------------------------------------------------------------

#if (SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
//Example -> NOS_ISR(SPDIF_RX_IRQHandler)
{
    // _your_class_object_here__->VirtualUartRX_IRQHandler();
}
#endif

//-------------------------------------------------------------------------------------------------

#if (SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
//Example -> NOS_ISR(SPDIF_TX_IRQHandler)
{
    // _your_class_object_here__->VirtualUartTX_IRQHandler();
}
#endif

//-------------------------------------------------------------------------------------------------

#endif // USE_UART_DRIVER

//-------------------------------------------------------------------------------------------------

void UsageFault_Handler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void SVC_Handler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}


void WWDG_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void PVD_IRQHandler                 (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void TAMP_STAMP_IRQHandler          (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*//*
void RTC_WKUP_IRQHandler            (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void FLASH_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void RCC_IRQHandler                 (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void EXTI0_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void EXTI1_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void EXTI2_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void EXTI3_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void EXTI4_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA1_Stream0_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA1_Stream1_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA1_Stream2_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA1_Stream3_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA1_Stream4_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA1_Stream5_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA1_Stream6_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void ADC_IRQHandler                 (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void CAN1_TX_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void CAN1_RX0_IRQHandler            (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void CAN1_RX1_IRQHandler            (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void CAN1_SCE_IRQHandler            (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void EXTI9_5_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*//*
void TIM1_BRK_TIM9_IRQHandler       (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*//*
void TIM1_UP_TIM10_IRQHandler       (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void TIM1_TRG_COM_TIM11_IRQHandler  (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void TIM1_CC_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void TIM2_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void TIM3_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void TIM4_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void I2C1_EV_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*//*
void I2C1_ER_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void I2C2_EV_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void I2C2_ER_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void SPI1_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void SPI2_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void USART1_IRQHandler              (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void USART2_IRQHandler              (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void USART3_IRQHandler              (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void EXTI15_10_IRQHandler           (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*//*
void RTC_Alarm_IRQHandler           (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void OTG_FS_WKUP_IRQHandler         (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void TIM8_BRK_TIM12_IRQHandler      (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void TIM8_UP_TIM13_IRQHandler       (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void TIM8_TRG_COM_TIM14_IRQHandler  (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void TIM8_CC_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA1_Stream7_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void FMC_IRQHandler                 (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void SDMMC1_IRQHandler              (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void TIM5_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void SPI3_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void UART4_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void UART5_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void TIM6_DAC_IRQHandler            (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void TIM7_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA2_Stream0_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA2_Stream1_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA2_Stream2_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void DMA2_Stream3_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void DMA2_Stream4_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void ETH_IRQHandler                 (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void ETH_WKUP_IRQHandler            (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void CAN2_TX_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void CAN2_RX0_IRQHandler            (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void CAN2_RX1_IRQHandler            (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void CAN2_SCE_IRQHandler            (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void OTG_FS_IRQHandler              (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA2_Stream5_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void DMA2_Stream6_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void DMA2_Stream7_IRQHandler        (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void USART6_IRQHandler              (void)
{
    while(1)
    {
        __asm("nop");
    };
}
/*
void I2C3_EV_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void I2C3_ER_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void OTG_HS_EP1_OUT_IRQHandler      (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void OTG_HS_EP1_IN_IRQHandler       (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void OTG_HS_WKUP_IRQHandler         (void)
{
    while(1)
    {
        __asm("nop");
    };
}

/*
void OTG_HS_IRQHandler              (void)
{
    while(1)
    {
        __asm("nop");
    };
}
*/
void DCMI_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void RNG_IRQHandler                 (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void FPU_IRQHandler                 (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void UART7_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void UART8_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void SPI4_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void SPI5_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void SPI6_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void SAI1_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void LTDC_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void LTDC_ER_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void DMA2D_IRQHandler               (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void SAI2_IRQHandler                (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void QUADSPI_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void LPTIM1_IRQHandler              (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void CEC_IRQHandler                 (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void I2C4_EV_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void I2C4_ER_IRQHandler             (void)
{
    while(1)
    {
        __asm("nop");
    };
}

void SPDIF_RX_IRQHandler            (void)
{
    while(1)
    {
        __asm("nop");
    };
}







} // extern "C"

//-------------------------------------------------------------------------------------------------
