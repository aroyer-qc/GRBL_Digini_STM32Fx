//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_qspi.h
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

#ifdef STM32F7_QSPI_GLOBAL
    #define STM32F7_QSPI_EXTERN
#else
    #define STM32F7_QSPI_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

//#include <stdint.h>
#include "stm32f7xx.h"
#include "nOS.h"
#include "lib_isr.h"
#include "lib_io.h"
#include "lib_define.h"
#include "qspi_cfg.h"
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------

#if USE_QSPI_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// enum
//-------------------------------------------------------------------------------------------------

enum QSPI_CmdList_e
{
    QSPI_CMD_READ_STATUS_SINGLE,
    QSPI_CMD_WRITE_STATUS_SINGLE,
    QSPI_CMD_READ_ID_SINGLE,
    QSPI_CMD_WRITE_ENABLE_SINGLE,
    QSPI_CMD_READ_STATUS_QUAD,
    QSPI_CMD_WRITE_STATUS_QUAD,
    QSPI_CMD_READ_ID_QUAD,
    QSPI_CMD_WRITE_ENABLE_QUAD,
    QSPI_CMD_SECTOR_ERASE,
    QSPI_CMD_BLOCK_ERASE,
    QSPI_CMD_CHIP_ERASE,
    QSPI_CMD_ENTER_QUAD_MODE,
    QSPI_CMD_EXIT_QUAD_MODE,
    QSPI_CMD_PAGE_PROG_QUAD,
    QSPI_CMD_FAST_READ_QUAD,
  #if (QSPI_READ_DISCOVERY_PARAMETER == DEF_ENABLED)
    QSPI_CMD_READ_DISCOVERY_PARAMETER,
  #endif
    QSPI_NB_OF_CMD,
};

enum QSPI_Instruction_e
{
    QSPI_READ_STATUS_REG_CMD          = 0x05,
    QSPI_WRITE_STATUS_REG_CMD         = 0x01,
    QSPI_WRITE_ENABLE_CMD             = 0x06,
    QSPI_SECTOR_ERASE_CMD             = QSPI_SECTOR_ERASE_CMD_CFG,
    QSPI_BLOCK_ERASE_CMD              = QSPI_BLOCK_ERASE_CMD_CFG,
    QSPI_CHIP_ERASE_CMD               = QSPI_CHIP_ERASE_CMD_CFG,
    QSPI_ENTER_QUAD_MODE_CMD          = 0x35,
    QSPI_EXIT_QUAD_MODE_CMD           = 0xF5,
    QSPI_QUAD_PAGE_PROG_CMD           = QSPI_QUAD_PAGE_PROGRAM_CMD_CFG,
    QSPI_QUAD_OUT_FAST_READ_CMD       = QSPI_QUAD_OUT_FAST_READ_CMD_CFG,
    QSPI_READ_ID_CMD                  = 0x9F,
    QSPI_ENABLE_RESET_CMD             = 0x66,
    QSPI_SEND_RESET_CMD               = 0x99,
  #if (QSPI_READ_DISCOVERY_PARAMETER == DEF_ENABLED)
    QSPI_READ_DISCOVERY_PARAMATER_CMD = 0x5A,
  #endif
};

enum QSPI_State_e
{
    QSPI_STATE_RESET                 = 0x00,    // Peripheral not initialized
    QSPI_STATE_READY                 = 0x01,    // Peripheral initialized and ready for use
    QSPI_STATE_BUSY                  = 0x02,    // Peripheral in indirect mode and busy
    QSPI_STATE_BUSY_INDIRECT_TX      = 0x12,    // Peripheral in indirect mode with transmission ongoing
    QSPI_STATE_BUSY_INDIRECT_RX      = 0x22,    // Peripheral in indirect mode with reception ongoing
    QSPI_STATE_BUSY_AUTO_POLLING     = 0x42,    // Peripheral in auto polling mode ongoing
    QSPI_STATE_ABORT                 = 0x08,    // Peripheral with abort request ongoing
    QSPI_STATE_ERROR                 = 0x04,    // Peripheral in error

    QSPI_STATE_DMA_READY             = 0x81,
    QSPI_STATE_DMA_BUSY              = 0x82,
    QSPI_STATE_DMA_ERROR             = 0x84,
    QSPI_STATE_DMA_ABORT             = 0x88,
};

enum QSPI_InstructionMode_e
{
    QSPI_INSTRUCTION_NONE    = ((uint32_t)0x00000000),                               // No instruction
    QSPI_INSTRUCTION_1_LINE  = ((uint32_t)QUADSPI_CCR_IMODE_0),                      // Instruction on a single line
    QSPI_INSTRUCTION_2_LINES = ((uint32_t)QUADSPI_CCR_IMODE_1),                      // Instruction on two lines
    QSPI_INSTRUCTION_4_LINES = ((uint32_t)QUADSPI_CCR_IMODE),                        // Instruction on four lines
};

enum QSPI_AddressMode_e
{
    QSPI_ADDRESS_NONE        = ((uint32_t)0x00000000),                               // No address
    QSPI_ADDRESS_1_LINE      = ((uint32_t)QUADSPI_CCR_ADMODE_0),                     // Address on a single line
    QSPI_ADDRESS_2_LINES     = ((uint32_t)QUADSPI_CCR_ADMODE_1),                     // Address on two lines
    QSPI_ADDRESS_4_LINES     = ((uint32_t)QUADSPI_CCR_ADMODE),                       // Address on four lines
};

enum QSPI_DataMode_e
{
    QSPI_DATA_NONE           = ((uint32_t)0X00000000),                               // No data
    QSPI_DATA_1_LINE         = ((uint32_t)QUADSPI_CCR_DMODE_0),                      // Data on a single line
    QSPI_DATA_2_LINES        = ((uint32_t)QUADSPI_CCR_DMODE_1),                      // Data on two lines
    QSPI_DATA_4_LINES        = ((uint32_t)QUADSPI_CCR_DMODE),                        // Data on four lines
};

struct QSPI_Command_t
{
    QSPI_Instruction_e     Instruction;        // Specifies the Instruction to be sent.
    QSPI_InstructionMode_e InstructionMode;    // Specifies the Instruction Mode.
    QSPI_AddressMode_e     AddressMode;        // Specifies the Address Mode.
    QSPI_DataMode_e        DataMode;           // Specifies the Data Mode (used for dummy cycles and data phases).
    uint32_t               DummyCycles;        // Specifies the Number of Dummy Cycles. This parameter can be a number between 0 and 31
};

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class QSPI_Driver
{
    public:
                        QSPI_Driver                    ();

        SystemState_e   GetStatus                   (void);
        SystemState_e   Initialize                  (void);
        SystemState_e   Read                        (uint32_t Address, uint8_t* pBuffer, uint32_t Size);
        SystemState_e   Write                        (uint32_t Address, uint8_t* pBuffer, uint32_t Size);
        SystemState_e   EraseSector                 (uint32_t SectorAddress);
        SystemState_e   EraseChip                   (void);
        SystemState_e   FlashReady                  (void);
        SystemState_e   DMA_Abort_IT                (void);
        void            DMA_Error                   (void);

        volatile bool                               m_KickAutoPolling;
        volatile QSPI_State_e                       m_State;
        volatile SystemState_e                      m_ErrorCode;
        volatile SystemState_e                      m_DMA_ErrorCode;

    private:

    // ---- QSPI section ----

        // Access function
        SystemState_e   InternalEraseSector         (uint32_t SectorAddress);
        SystemState_e   PageProgram                 (uint32_t Address, uint8_t* pData, uint32_t Size);
        SystemState_e   WriteInSector               (uint32_t Address, uint8_t* pData, uint32_t Size);


        // Wait function
        SystemState_e   WaitReady                   (TickCount_t TimeOut);
        SystemState_e   WaitFlagStateUntilTimeout   (uint32_t Flag, uint32_t State, TickCount_t TimeOut);
        SystemState_e   WaitFlashReady              (void);
        SystemState_e   AutoPolling                 (QSPI_CmdList_e CmdID, uint32_t Match, uint32_t Mask, TickCount_t TimeOut);

        // Control function
        SystemState_e   QuadEnable                  (void);
        SystemState_e   WriteEnable                 (void);
        void            Config                      (QSPI_CmdList_e CmdID, uint32_t Address, uint32_t Size, uint32_t FunctionalMode);
        SystemState_e   Abort_IT                    (void);
        SystemState_e   ReadCommand                 (QSPI_CmdList_e CmdID, uint32_t Address, uint8_t* pData, uint32_t Size, TickCount_t TimeOut);
        SystemState_e   WriteCommand                (QSPI_CmdList_e CmdID, uint32_t Address, uint8_t* pData, uint32_t Size, TickCount_t TimeOut);
        SystemState_e   Command                     (QSPI_CmdList_e CmdID, uint32_t Address, uint32_t Size, TickCount_t TimeOut);
        SystemState_e   Command_IT                  (QSPI_CmdList_e CmdID, uint32_t Address, uint32_t Size);

        // ---- DMA section ----
        SystemState_e   DMA_Transmit                (uint8_t* pData, uint32_t Size);
        SystemState_e   DMA_Receive                 (uint8_t* pData, uint32_t Size);
        SystemState_e   DMA_Start                   (uint8_t* pData, uint32_t BlockDataLength);
        SystemState_e   DMA_Abort                   (void);

        void            DMA_AbortCplt               (void);

        void            Lock                        (void);
        void            Unlock                      (void);

        volatile bool                               m_IsItBusy;
        SystemState_e                               m_Status;
        nOS_Mutex                                   m_Mutex;
        static const QSPI_Command_t                 m_Cmd[QSPI_NB_OF_CMD];
        bool                                        m_IsItInitialize;

      #if (QSPI_READ_DISCOVERY_PARAMETER == DEF_ENABLED)
        uint8_t                                     m_DiscoveryDummyCycles;
      #endif
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef STM32F7_QSPI_GLOBAL

  QSPI_Driver           QSPI;

#else // STM32F7_QSPI_GLOBAL

  extern QSPI_Driver    QSPI;

#endif // STM32F7_QSPI_GLOBAL

//-------------------------------------------------------------------------------------------------

#endif // USE_QSPI_DRIVER == DEF_ENABLED



