//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_sdio.h
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

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "diskio.h"
//#include "lib_class_sd_card_interface.h"
//#include "lib_class_sd_card.h"
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------

#if (USE_SDIO_DRIVER == DEF_ENABLED)

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define SDIO_SPEED_400_KHZ                          0x7F
#define SDIO_SPEED_48_MHZ                           0x08
#define SDIO_SPEED_24_MHZ                           0x00

#define SDIO_SOFTWARE_COMMAND_TIMEOUT               10
#define SDIO_DATA_TIMEOUT                           0xFFFFFFFF

#define SDIO_STATIC_FLAGS                           0x000005FF

#define SDIO_DMA_BLOCK_SIZE_512                     0x00000090

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

// To found how many port there is (Only ONE on STM32F4, but we keep this driver generic)
typedef enum SDIO_PortQty_e
{
    #define X_SDIO( ENUM_ID, MUTEX, PREEMP_PRIO, DMA_STREAM, DMA_PREEMP_PRIO, GPIO_CLK,    CLK_PIN,     CLK_PIN_SOURCE,    CLK_CLOCK,   \
                                                                              GPIO_CMD,    CMD_PIN,     CMD_PIN_SOURCE,    CMD_CLOCK,   \
                                                                              GPIO_D0,     D0_PIN,      D0_PIN_SOURCE,     D0_CLOCK,    \
                                                                              GPIO_D1,     D1_PIN,      D1_PIN_SOURCE,     D1_CLOCK,    \
                                                                              GPIO_D2,     D2_PIN,      D2_PIN_SOURCE,     D2_CLOCK,    \
                                                                              GPIO_D3,     D3_PIN,      D3_PIN_SOURCE,     D3_CLOCK) ENUM_ID,
    SDIO_DEF
  #undef X_SDIO
    SDIO_NB_PORT_CONST
} SDIO_ePortQty;


typedef struct SDIO_s
{
    xSemaphoreHandle*   pMutex;
    uint8_t             PreempPrio;
    DMA_Stream_TypeDef* pDMA_Stream;
    uint8_t             DMA_PreempPrio;
    GPIO_TypeDef*       pCLK_Port;
    uint16_t            CLK_Pin;
    uint8_t             CLK_PinSource;
    uint32_t            CLK_Clock;
    GPIO_TypeDef*       pCMD_Port;
    uint16_t            CMD_Pin;
    uint8_t             CMD_PinSource;
    uint32_t            CMD_Clock;
    GPIO_TypeDef*       pD0_Port;
    uint16_t            D0_Pin;
    uint8_t             D0_PinSource;
    uint32_t            D0_Clock;
    GPIO_TypeDef*       pD1_Port;
    uint16_t            D1_Pin;
    uint8_t             D1_PinSource;
    uint32_t            D1_Clock;
    GPIO_TypeDef*       pD2_Port;
    uint16_t            D2_Pin;
    uint8_t             D2_PinSource;
    uint32_t            D2_Clock;
    GPIO_TypeDef*       pD3_Port;
    uint16_t            D3_Pin;
    uint8_t             D3_PinSource;
    uint32_t            D3_Clock;
} sSDIO;

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class CSDIO : public CSD_CardInterface
{
    public:
                            CSDIO                   (sSDIO* pSDIO);
                            ~CSDIO                  ();

        eSystemState        GetResponse             (uint32_t* pResponse);
        uint32_t            GetTransfertStatus      ();
        void                Init                    ();
        bool                IsFlagSet               (uint32_t Status, eSD_StatusFlag Flag);
        void                PowerOff                ()                                      { SDIO_SetPowerState(SDIO_PowerState_OFF); }
        void                PowerOn                 ()                                      { SDIO_SetPowerState(SDIO_PowerState_ON);  }
        void                SetBusSize              (eBusSize BusSize)
        void                SetSpeed                (eDeviceSpeed DeviceSpeed);
        void                TickHook                ();
        void                TransmitCommand         (uint8_t Command, uint32_t Argument, eResponseType ResponseType);
        void                StartBlockTransfert     (const uint8_t *pBuffer, uint32_t Count, uint32_t TransfertDir);
        void                SDIO_IRQHandler         ();
        void                DMA_StreamIRQHandler    ();
        eSystemState        WaitBlockTransfertEnd   ();

    private:

        void                SDIO_DataInit           (uint32_t TransfertDir, size_t Size);
        void                DMA_Config              (uint32_t* pBuffer, size_t BufferSize, uint32_t Direction);
        void                Lock                    ();
        void                Unlock                  ();

        sSDIO*                  m_pSDIO;
        IRQn_Type               m_DMA_IRQn;
        uint32_t                m_DMA_FlagFEIF;
        uint32_t                m_DMA_FlagDMEIF;
        uint32_t                m_DMA_FlagTEIF;
        uint32_t                m_DMA_FlagHTIF;
        uint32_t                m_DMA_FlagTCIF;
        uint8_t                 m_LastCommand;
        bool                    m_StopCondition;
        volatile uint32_t       m_TimeOut;
        volatile eSystemState   m_TransfertError;
        volatile bool           m_TransfertEnd;
        volatile bool           m_DMA_EndOfTransfer;
};





//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef STM32F4_SDIO_GLOBAL

  sSDIO SDIO_Info[SDIO_NB_PORT_CONST] =
  {
    #define X_SDIO( ENUM_ID, MUTEX, PREEMP_PRIO, DMA_STREAM, DMA_PREEMP_PRIO, GPIO_CLK,    CLK_PIN,     CLK_PIN_SOURCE,    CLK_CLOCK,     \
                                                                              GPIO_CMD,    CMD_PIN,     CMD_PIN_SOURCE,    CMD_CLOCK,     \
                                                                              GPIO_D0,     D0_PIN,      D0_PIN_SOURCE,     D0_CLOCK,      \
                                                                              GPIO_D1,     D1_PIN,      D1_PIN_SOURCE,     D1_CLOCK,      \
                                                                              GPIO_D2,     D2_PIN,      D2_PIN_SOURCE,     D2_CLOCK,      \
                                                                              GPIO_D3,     D3_PIN,      D3_PIN_SOURCE,     D3_CLOCK )     \
      {                     &MUTEX, PREEMP_PRIO, DMA_STREAM, DMA_PREEMP_PRIO, GPIO_CLK,    CLK_PIN,     CLK_PIN_SOURCE,    CLK_CLOCK,     \
                                                                              GPIO_CMD,    CMD_PIN,     CMD_PIN_SOURCE,    CMD_CLOCK,     \
                                                                              GPIO_D0,     D0_PIN,      D0_PIN_SOURCE,     D0_CLOCK,      \
                                                                              GPIO_D1,     D1_PIN,      D1_PIN_SOURCE,     D1_CLOCK,      \
                                                                              GPIO_D2,     D2_PIN,      D2_PIN_SOURCE,     D2_CLOCK,      \
                                                                              GPIO_D3,     D3_PIN,      D3_PIN_SOURCE,     D3_CLOCK       \
      },
      SDIO_DEF
    #undef X_SDIO
  };

  CSDIO SDIO_Port(&SDIO_Info[SDIO_PORT]);




#else

  extern sSDIO SDIO_Info[SDIO_NB_PORT_CONST];
  extern CSDIO SDIO_Port;

#endif

#endif // USE_SDIO_DRIVER == DEF_ENABLED
