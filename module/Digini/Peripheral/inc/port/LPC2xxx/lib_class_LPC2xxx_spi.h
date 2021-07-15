//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_LPC2xxx.h
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

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include "stm32f4xx.h"
#include "nOS.h"
#include "lib_class_STM32F4_gpio.h"
#include "lib_typedef.h"
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------

#if USE_SPI_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define SPI_USE_DMA_TRANSFERT       0

#define SPI_PORT_QTY                6
#define SPI_TICK_HOOK_TIME          portTICK_RATE_MS            // in mSec

#define SPI_CR1_DFF_8_BITS          0
#define SPI_CR1_DFF_16_BITS         SPI_CR1_DFF

#define SPI_CR1_CPOL_LOW            0
#define SPI_CR1_CPOL_HIGH           SPI_CR1_CPOL

#define SPI_CR1_CPHA_1_EDGE         0
#define SPI_CR1_CPHA_2_EDGE         SPI_CR1_CPHA

#define SPI_CR1_SSM_DISABLE         0
#define SPI_CR1_SSM_ENABLE          SPI_CR1_SSM

#define SPI_CR1_MSB_FIRST           0                    //SPI_FirstBit_MSB
#define SPI_CR1_LSB_FIRST           SPI_CR1_LSBFIRST    //SPI_FirstBit_LSB

#define SPI_PRESCALER_MASK          0x0038

#define SPI_TRANSFERT_TIME_OUT      2

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

enum SPI_IRQ_Number_e
{
    SPI1_HARD_PORT = 0,
    SPI2_HARD_PORT,
    SPI3_HARD_PORT,
    SPI4_HARD_PORT,
    SPI5_HARD_PORT,
    SPI6_HARD_PORT,
};


// To found how many port ther is
enum SPI_PortQty_e
{
  #define X_SPI( ENUM_ID, SPI_TYPEDEF, CLOCK, HARDWARE_PORT, ALTERNATE_FUNCTION, PORT_CFG, PREEMP_PRIO, IRQ_NB, IRQ_SOURCE, CALLBACK_ISR, DMA, DMA_STREAM_RX, DMA_CHANNEL_RX, DMA_FLAG_TC_RX, DMA_STREAM_TX, DMA_CHANNEL_TX, DMA_FLAG_TC_TX,  \
                 GPIO_SCLK, SCLK_PIN, SCLK_PIN_SOURCE,  SCLK_CLOCK,  \
                 GPIO_MOSI, MOSI_PIN, MOSI_PIN_SOURCE,  MOSI_CLOCK,  \
                 GPIO_MISO, MISO_PIN, MISO_PIN_SOURCE,  MISO_CLOCK,  \
                 GPIO_CS,   CS_PIN,   CS_PIN_SOURCE,    CS_CLOCK     \
               ) ENUM_ID,
    SPI_DEF
  #undef X_SPI
    SPI_NB_PORT_CONST
};

// To found how many device there is
enum SPI_DeviceQty_e
{
 #ifdef SPI_DEVICE_DEF
  #define X_SPI_DEVICE(ENUM_ID, DEFINITION, SLOW_SPEED, FAST_SPEED, TIMEOUT) ENUM_ID,
    SPI_DEVICE_DEF
  #undef X_SPI_DEVICE
 #endif
    SPI_NB_DEVICE_CONST
};


struct SPI_PortInfo_t
{
    SPI_TypeDef*        pSPIx;
    uint32_t            Clock;
    uint8_t             HardwarePort;
    uint8_t             AlternateFunction;
    uint16_t            portConfig;
    uint8_t             PreempPrio;
    IRQn_Type           IRQn;
    uint16_t            IRQ_Source;
    void                (*CallBackISR)();
    DMA_TypeDef*        pDMA;
    DMA_Stream_TypeDef* pDMA_RX;
    uint32_t            DMA_ChannelRX;
    uint32_t            DMA_Flag_TC_RX;
    DMA_Stream_TypeDef* pDMA_TX;
    uint32_t            DMA_ChannelTX;
    uint32_t            DMA_Flag_TC_TX;
    GPIO_TypeDef*       pGPIO_SCLK;
    uint16_t            SCLK_Pin;
    uint8_t             SCLK_PinSource;
    uint32_t            SCLK_Clock;
    GPIO_TypeDef*       pGPIO_MISO;
    uint16_t            MISO_Pin;
    uint8_t             MISO_PinSource;
    uint32_t            MISO_Clock;
    GPIO_TypeDef*       pGPIO_MOSI;
    uint16_t            MOSI_Pin;
    uint8_t             MOSI_PinSource;
    uint32_t            MOSI_Clock;
    GPIO_TypeDef*       pGPIO_CS;
    uint16_t            CS_Pin;
    uint8_t             CS_PinSource;
    uint32_t            CS_Clock;
};

struct SPI_DeviceInfo_t
{
    uint32_t            SlowSpeed;
    uint32_t            FastSpeed;
    uint8_t             TimeOut;
    uint32_t            ChipSelectClock;
    GPIO_TypeDef*       pChipSelect;
    uint16_t            ChipSelectPin;
};

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class CSPI
{
    public:
                        CSPI                    (SPI_PortInfo_t* pPort);
                        ~CSPI                    ();

        SystemState_e   GetStatus               (void);
        void            Initialize              (void);
        SystemState_e   LockToDevice               (SPI_DeviceInfo_t* pDevice);                                             // Set SPI to this device and lock
        SystemState_e   UnlockFromDevice         (SPI_DeviceInfo_t* pDevice);                                             // Unlock SPI from device

        // Read function (overloaded)
        SystemState_e   Read                    (uint8_t* pBuffer, size_t Size);
        SystemState_e   Read                    (uint8_t*  Data);
        SystemState_e   Read                    (uint16_t* Data);
        SystemState_e   Read                    (uint32_t* Data);
        SystemState_e   Read                    (uint8_t* pBuffer, size_t Size, SPI_DeviceInfo_t* pDevice);
        SystemState_e   Read                    (uint8_t*  pData, SPI_DeviceInfo_t* pDevice);
        SystemState_e   Read                    (uint16_t* pData, SPI_DeviceInfo_t* pDevice);
        SystemState_e   Read                    (uint32_t* pData, SPI_DeviceInfo_t* pDevice);

        // Write function (overloaded)
        SystemState_e   Write                    (const uint8_t* pBuffer, size_t Size);
        SystemState_e   Write                    (uint8_t  Data);
        SystemState_e   Write                    (uint16_t Data);
        SystemState_e   Write                    (uint32_t Data);
        SystemState_e   Write                   (const uint8_t* pBuffer, size_t Size, SPI_DeviceInfo_t* pDevice);
        SystemState_e   Write                   (uint8_t  Data, SPI_DeviceInfo_t* pDevice);
        SystemState_e   Write                   (uint16_t Data, SPI_DeviceInfo_t* pDevice);
        SystemState_e   Write                   (uint32_t Data, SPI_DeviceInfo_t* pDevice);

        void            IRQHandler              (void);
        SystemState_e   Request                 (AccessRequest_e Request, uint8_t *pBuffer, size_t Size);
        void            Config                  (DeviceSpeed_e Speed);
        void            TickHook                (void);
        void            RegisterDriverTick      (void (*CallBackTick)());
        SystemState_e   WaitReady               (void);

    private:

        uint16_t        GetPrescalerFromSpeed   (uint32_t speed);
        void            Lock                    (void);
        void            Unlock                  (void);

        SPI_PortInfo_t*                         m_pPort;
        SPI_DeviceInfo_t*                       m_pDevice;
        nOS_Mutex                               m_Mutex;
        AccessRequest_e                            m_Request;
        uint32_t                                m_SlowSpeed;
        uint32_t                                m_FastSpeed;
        volatile size_t                            m_Size;
        volatile SystemState_e                    m_Status;
        volatile uint8_t                        m_Timeout;
        void                                    (*m_pCallBackTick)();

};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef STM32F4_SPI_GLOBAL

  SPI_PortInfo_t SPI_PortInfo[SPI_NB_PORT_CONST] =
  {
    #define X_SPI( ENUM_ID, SPI_TYPEDEF, CLOCK, HARDWARE_PORT, ALTERNATE_FUNCTION, PORT_CFG, PREEMP_PRIO,            IRQ_NB, IRQ_SOURCE,           CALLBACK_ISR, DMA,                      DMA_STREAM_RX, DMA_CHANNEL_RX, DMA_FLAG_TC_RX,                      DMA_STREAM_TX, DMA_CHANNEL_TX, DMA_FLAG_TC_TX, \
                                  GPIO_SCLK, SCLK_PIN, SCLK_PIN_SOURCE,            SCLK_CLOCK,  \
                                  GPIO_MISO, MISO_PIN, MISO_PIN_SOURCE,            MISO_CLOCK,  \
                                  GPIO_MOSI, MOSI_PIN, MOSI_PIN_SOURCE,            MOSI_CLOCK,  \
                                  GPIO_CS,   CS_PIN,   CS_PIN_SOURCE,              CS_CLOCK     \
                 ) {        SPI_TYPEDEF, CLOCK, HARDWARE_PORT, ALTERNATE_FUNCTION, PORT_CFG, PREEMP_PRIO, (IRQn_Type)IRQ_NB, IRQ_SOURCE, (uint32_t)CALLBACK_ISR, DMA, (DMA_Stream_TypeDef*)DMA_STREAM_RX, DMA_CHANNEL_RX, DMA_FLAG_TC_RX, (DMA_Stream_TypeDef*)DMA_STREAM_TX, DMA_CHANNEL_TX, DMA_FLAG_TC_TX, \
                   (GPIO_TypeDef*)GPIO_SCLK, SCLK_PIN, SCLK_PIN_SOURCE,  (uint32_t)SCLK_CLOCK,  \
                   (GPIO_TypeDef*)GPIO_MISO, MISO_PIN, MISO_PIN_SOURCE,  (uint32_t)MISO_CLOCK,  \
                   (GPIO_TypeDef*)GPIO_MOSI, MOSI_PIN, MOSI_PIN_SOURCE,  (uint32_t)MOSI_CLOCK,  \
                   (GPIO_TypeDef*)GPIO_CS,   CS_PIN,   CS_PIN_SOURCE,    (uint32_t)CS_CLOCK     },
      SPI_DEF
    #undef X_SPI
  };

 #ifdef SPI_DEVICE_DEF
  SPI_DeviceInfo_t SPI_DeviceInfo[SPI_NB_DEVICE_CONST] =
  {
    #define X_SPI_DEVICE(ENUM_ID, DEFINITION, SLOW_SPEED, FAST_SPEED, TIMEOUT) \
                                            { SLOW_SPEED, FAST_SPEED, TIMEOUT},
      SPI_DEVICE_DEF
    #undef X_SPI_DEVICE
  };
 #endif

// TODO (Alain#1#): use combination of enum##to create on the fly all SPI object
   //CSPI SPI_Port1(&SPI_PortInfo[SPI1_PORT]);
  //CSPI SPI_Port2(&SPI_PortInfo[SPI2_PORT]);
  //CSPI SPI_Port3(&SPI_PortInfo[SPI3_PORT]);
  //CSPI SPI_Port4(&SPI_PortInfo[SPI4_PORT]);
  CSPI SPI_Port5(&SPI_PortInfo[LCD_PORT]);
  //CSPI SPI_Port6(&SPI_PortInfo[SPI6_PORT]);

#else

  extern SPI_PortInfo_t SPI_PortInfo[SPI_NB_PORT_CONST];
 #ifdef SPI_DEVICE_DEF
  extern SPI_DeviceInfo_t SPI_DeviceInfo[SPI_NB_DEVICE_CONST];
 #endif
  //extern CSPI SPI_Port1;
  //extern CSPI SPI_Port2;
  //extern CSPI SPI_Port3;
  //extern CSPI SPI_Port4;
  extern CSPI SPI_Port5;
  //extern CSPI SPI_Port6;

#endif


#endif // USE_SPI_DRIVER == DEF_ENABLED



