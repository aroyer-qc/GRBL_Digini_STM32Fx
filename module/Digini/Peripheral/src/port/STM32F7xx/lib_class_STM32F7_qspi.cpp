//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_qspi.cpp
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

#include "digini_cfg.h"
#ifdef DIGINI_USE_QSPI
#define STM32F7_QSPI_GLOBAL
#include "lib_class_STM32F7_qspi.h"
#undef  STM32F7_QSPI_GLOBAL
#include "lib_STM32F7_dma.h"
#include "lib_memory.h"
#include "lib_utility.h"
#include "lib_define.h"
#include "lib_macro.h"
#include "clock_cfg.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#if   (QSPI_MEMORY_SIZE_MBYTE_CFG == 2)
  #define QSPI_MEMORY_SIZE_DCR_CFG     20
#elif (QSPI_MEMORY_SIZE_MBYTE_CDF == 4)
  #define QSPI_MEMORY_SIZE_DCR_CFG     21
#elif (QSPI_MEMORY_SIZE_MBYTE_CFG == 8)
  #define QSPI_MEMORY_SIZE_DCR_CFG     22
#elif (QSPI_MEMORY_SIZE_MBYTE_CFG == 16)
  #define QSPI_MEMORY_SIZE_DCR_CFG     23
#elif (QSPI_MEMORY_SIZE_MBYTE_CFG == 32)
  #define QSPI_MEMORY_SIZE_DCR_CFG     24
#endif

#define QSPI_DUMMY_CYCLES_READ_DISCOVERY    8

#define QSPI_ADDRESS_32_BITS                ((uint32_t)(QUADSPI_CCR_ADSIZE_1 | QUADSPI_CCR_ADSIZE_0))   // 24-bit address
#define QSPI_STATUS_BYTE_SIZE               1

#define QSPI_SAMPLE_SHIFTING_NONE           ((uint32_t)0x00000000)                                      // No clock cycle shift to sample data
#define QSPI_SAMPLE_SHIFTING_HALFCYCLE      ((uint32_t)QUADSPI_CR_SSHIFT)                               // 1/2 clock cycle shift to sample data

#define QSPI_CS_HIGH_TIME_1_CYCLE           ((uint32_t)0x00000000)                                      // nCS stay high for at least 1 clock cycle between commands.
#define QSPI_CS_HIGH_TIME_2_CYCLE           ((uint32_t)QUADSPI_DCR_CSHT_0)                              // ... for 2 clock cycles.
#define QSPI_CS_HIGH_TIME_3_CYCLE           ((uint32_t)QUADSPI_DCR_CSHT_1)                              // ... for 3 clock cycles.
#define QSPI_CS_HIGH_TIME_4_CYCLE           ((uint32_t)QUADSPI_DCR_CSHT_0 | QUADSPI_DCR_CSHT_1)         // ... for 4 clock cycles.
#define QSPI_CS_HIGH_TIME_5_CYCLE           ((uint32_t)QUADSPI_DCR_CSHT_2)                              // ... for 5 clock cycles.
#define QSPI_CS_HIGH_TIME_6_CYCLE           ((uint32_t)QUADSPI_DCR_CSHT_2 | QUADSPI_DCR_CSHT_0)         // ... for 6 clock cycles.
#define QSPI_CS_HIGH_TIME_7_CYCLE           ((uint32_t)QUADSPI_DCR_CSHT_2 | QUADSPI_DCR_CSHT_1)         // ... for 7 clock cycles.
#define QSPI_CS_HIGH_TIME_8_CYCLE           ((uint32_t)QUADSPI_DCR_CSHT)                                // ... for 8 clock cycles.

#define QSPI_CLOCK_MODE_0                   ((uint32_t)0x00000000)                                      // Clk stays low while nCS is released
#define QSPI_CLOCK_MODE_3                   ((uint32_t)QUADSPI_DCR_CKMODE)                              // Clk goes high while nCS is released

#define QSPI_FLASH_ID_1                     ((uint32_t)0x00000000)                                      // FLASH 1 selected
#define QSPI_FLASH_ID_2                     ((uint32_t)QUADSPI_CR_FSEL)                                 // FLASH 2 selected

#define QSPI_DUAL_FLASH_DISABLE             ((uint32_t)0x00000000)                                      // Dual-flash mode disabled
#define QSPI_DUAL_FLASH_ENABLE              ((uint32_t)QUADSPI_CR_DFM)                                  // Dual-flash mode enabled

#define QSPI_ALTERNATE_BYTES_8_BITS         ((uint32_t)0x00000000)                                      // 8-bit alternate bytes
#define QSPI_ALTERNATE_BYTES_16_BITS        ((uint32_t)QUADSPI_CCR_ABSIZE_0)                            // 16-bit alternate bytes
#define QSPI_ALTERNATE_BYTES_24_BITS        ((uint32_t)QUADSPI_CCR_ABSIZE_1)                            // 24-bit alternate bytesdeltarockwell beaver
#define QSPI_ALTERNATE_BYTES_32_BITS        ((uint32_t)QUADSPI_CCR_ABSIZE)                              // 32-bit alternate bytes

#define QSPI_ALTERNATE_BYTES_NONE           ((uint32_t)0x00000000)                                      // No alternate bytes
#define QSPI_ALTERNATE_BYTES_1_LINE         ((uint32_t)QUADSPI_CCR_ABMODE_0)                            // Alternate bytes on a single line
#define QSPI_ALTERNATE_BYTES_2_LINES        ((uint32_t)QUADSPI_CCR_ABMODE_1)                            // Alternate bytes on two lines
#define QSPI_ALTERNATE_BYTES_4_LINES        ((uint32_t)QUADSPI_CCR_ABMODE)                              // Alternate bytes on four lines

#define QSPI_DDR_MODE_DISABLE               0                                                           // Double data rate mode disabled
#define QSPI_DDR_MODE_ENABLE                ((uint32_t)QUADSPI_CCR_DDRM)                                // Double data rate mode enabled

#define QSPI_DDR_HHC_ANALOG_DELAY           ((uint32_t)0x00000000)                                      // Delay the data output using analog delay in DDR mode
#define QSPI_DDR_HHC_HALF_CLK_DELAY         ((uint32_t)QUADSPI_CCR_DHHC)                                // Delay the data output by 1/2 clock cycle in DDR mode

#define QSPI_SIOO_INST_EVERY_CMD            ((uint32_t)0x00000000)                                      // Send instruction on every transaction
#define QSPI_SIOO_INST_ONLY_FIRST_CMD       ((uint32_t)QUADSPI_CCR_SIOO)                                // Send instruction only for the first command

#define QSPI_MATCH_MODE_AND                 ((uint32_t)0x00000000)                                      // AND match mode between unmasked bits
#define QSPI_MATCH_MODE_OR                  ((uint32_t)QUADSPI_CR_PMM)                                  // OR match mode between unmasked bits

#define QSPI_AUTOMATIC_STOP_DISABLE         ((uint32_t)0x00000000)                                      // AutoPolling stops only with abort or QSPI disabling
#define QSPI_AUTOMATIC_STOP_ENABLE          ((uint32_t)QUADSPI_CR_APMS)                                 // AutoPolling stops as soon as there is a match

#define QSPI_TIMEOUT_COUNTER_DISABLE        ((uint32_t)0x00000000)                                      // Timeout counter disabled, nCS remains active
#define QSPI_TIMEOUT_COUNTER_ENABLE         ((uint32_t)QUADSPI_CR_TCEN)                                 // Timeout counter enabled, nCS released when timeout expires

//--------------------

#if (QSPI_SUPPORT_DUAL_FLASH_MODE == DEF_ENABLED)
#define QSPI_DUAL_FLASH_MODE_CFG            QSPI_DUAL_FLASH_ENABLE
#else
#define QSPI_DUAL_FLASH_MODE_CFG            QSPI_DUAL_FLASH_DISABLE
#endif

#if (QSPI_SUPPORT_DOUBLE_DATA_RATE == DEF_ENABLED)
#define QSPI_DOUBLE_DATA_RATE_CFG           QSPI_DDR_MODE_ENABLE
#else
#define QSPI_DOUBLE_DATA_RATE_CFG           QSPI_DDR_MODE_DISABLE
#endif

#define QSPI_SR_BUSY_BIT                    0x01
#define QSPI_SR_WRITE_ENABLE_BIT            0x02
#define QSPI_SR_QUAD_ENABLE_BIT             0x40
#define QSPI_SR_WRITE_MASK_BIT              0x42                                   // Do not protect block or status register...

#define QSPI_MAX_DELAY                      5000
#define QSPI_TIMEOUT_DEFAULT_VALUE          5000                        	        // 5 Sec
#define QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE    2                        	            // 2 mSec
#define QSPI_TIMEOUT_READ_COMMAND           20                       	            // 20 mSec
#define QSPI_TIMEOUT_DMA_ABORT              5                           	        // 5 mSec
#define QSPI_SECTOR_ERASE_TIME
#define QSPI_AUTOMATIC_POLLING_INTERVAL

#define QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE ((uint32_t)0x00000000)                 // Indirect write mode
#define QSPI_FUNCTIONAL_MODE_INDIRECT_READ  ((uint32_t)QUADSPI_CCR_FMODE_0)        // Indirect read mode
#define QSPI_FUNCTIONAL_MODE_AUTO_POLLING   ((uint32_t)QUADSPI_CCR_FMODE_1)        // Automatic polling mode

#define QSPI_COMMON_CONFIG                  (QSPI_DOUBLE_DATA_RATE_CFG | \
                                             QSPI_DDR_HHC_ANALOG_DELAY | \
                                             QSPI_SIOO_INST_EVERY_CMD  | \
                                             QSPI_ALTERNATE_BYTES_NONE)

#define QSPI_PRESCALER                      ((QSPI_PRESCALER_CFG - 1) << QUADSPI_CR_PRESCALER_Pos)

#define QSPI_DMA_READ_DIRECTION             ((uint32_t)0x00000000)                 // DMA is in Read direction
#define QSPI_DMA_WRITE_DIRECTION            ((uint32_t)DMA_SxCR_DIR_0)             // DMA is in Write direction

//-------------------------------------------------------------------------------------------------
//
//   Class: QSPI_Driver
//
//
//   Description:   Class to handle QSPI
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Private Constants and Macros
//-------------------------------------------------------------------------------------------------

const QSPI_Command_t QSPI_Driver::m_Cmd[QSPI_NB_OF_CMD] =
{
//                                             Instruction                        InstructionMode           AddressMode           DataMode            DummyCycles
    /* QSPI_CMD_READ_STATUS_SINGLE       */  { QSPI_READ_STATUS_REG_CMD,          QSPI_INSTRUCTION_1_LINE,  QSPI_ADDRESS_NONE,    QSPI_DATA_1_LINE,   0                                 },
    /* QSPI_CMD_WRITE_STATUS_SINGLE      */  { QSPI_WRITE_STATUS_REG_CMD,         QSPI_INSTRUCTION_1_LINE,  QSPI_ADDRESS_NONE,    QSPI_DATA_1_LINE,   0                                 },
    /* QSPI_CMD_READ_ID_SINGLE           */  { QSPI_READ_ID_CMD,                  QSPI_INSTRUCTION_1_LINE,  QSPI_ADDRESS_NONE,    QSPI_DATA_1_LINE,   0                                 },
    /* QSPI_CMD_WRITE_ENABLE_SINGLE      */  { QSPI_WRITE_ENABLE_CMD,             QSPI_INSTRUCTION_1_LINE,  QSPI_ADDRESS_NONE,    QSPI_DATA_NONE,     0                                 },
    /* QSPI_CMD_READ_STATUS_QUAD         */  { QSPI_READ_STATUS_REG_CMD,          QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE,    QSPI_DATA_4_LINES,  0                                 },
    /* QSPI_CMD_WRITE_STATUS_QUAD        */  { QSPI_WRITE_STATUS_REG_CMD,         QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE,    QSPI_DATA_4_LINES,  0                                 },
    /* QSPI_CMD_READ_ID_QUAD             */  { QSPI_READ_ID_CMD,                  QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE,    QSPI_DATA_4_LINES,  0                                 },
    /* QSPI_CMD_WRITE_ENABLE_QUAD        */  { QSPI_WRITE_ENABLE_CMD,             QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE,    QSPI_DATA_NONE,     0                                 },
    /* QSPI_CMD_SECTOR_ERASE             */  { QSPI_SECTOR_ERASE_CMD,             QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_4_LINES, QSPI_DATA_NONE,     0                                 },
    /* QSPI_CMD_BLOCK_ERASE              */  { QSPI_BLOCK_ERASE_CMD,              QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_4_LINES, QSPI_DATA_NONE,     0                                 },
    /* QSPI_CMD_CHIP_ERASE               */  { QSPI_CHIP_ERASE_CMD,               QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE,    QSPI_DATA_NONE,     0                                 },
    /* QSPI_CMD_ENTER_QUAD_MODE          */  { QSPI_ENTER_QUAD_MODE_CMD,          QSPI_INSTRUCTION_1_LINE,  QSPI_ADDRESS_NONE,    QSPI_DATA_NONE,     0                                 },
    /* QSPI_CMD_EXIT_QUAD_MODE           */  { QSPI_EXIT_QUAD_MODE_CMD,           QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_NONE,    QSPI_DATA_NONE,     0                                 },
    /* QSPI_CMD_PAGE_PROG_QUAD           */  { QSPI_QUAD_PAGE_PROG_CMD,           QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_4_LINES, QSPI_DATA_4_LINES,  0                                 },
  #if (QSPI_READ_DISCOVERY_PARAMETER == DEF_ENABLED)
    /* QSPI_CMD_FAST_READ_QUAD           */  { QSPI_QUAD_OUT_FAST_READ_CMD,       QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_4_LINES, QSPI_DATA_4_LINES,  0                                 },
    /* QSPI_CMD_READ_DISCOVERY_PARAMETER */  { QSPI_READ_DISCOVERY_PARAMATER_CMD, QSPI_INSTRUCTION_4_LINES,  QSPI_ADDRESS_NONE,   QSPI_DATA_4_LINES,  QSPI_DUMMY_CYCLES_READ_DISCOVERY  },
  #else
    /* QSPI_CMD_FAST_READ_QUAD           */  { QSPI_QUAD_OUT_FAST_READ_CMD,       QSPI_INSTRUCTION_4_LINES, QSPI_ADDRESS_4_LINES, QSPI_DATA_4_LINES,  QSPI_DUMMY_CLOCK_CYCLES_READ_CFG  },
  #endif
};



//-------------------------------------------------------------------------------------------------
//
//   Constructor:   QSPI_Driver
//
//   Parameter(s):  None
//
//   Description:   Initializes the QSPI peripheral according to the specified Parameters
//
//   Note(s):
//
//					QSPI_InitStruct: pointer to a QSPI_InitTypeDef structure that contains
//                  the configuration information for the specified QSPI peripheral.
//
//-------------------------------------------------------------------------------------------------
QSPI_Driver::QSPI_Driver()
{
    m_IsItInitialize  = false;
    m_KickAutoPolling = false;
    m_IsItBusy        = false;
    m_State           = QSPI_STATE_RESET;
    m_ErrorCode       = SYS_READY;
    m_DMA_ErrorCode   = SYS_READY;

  #if (QSPI_READ_DISCOVERY_PARAMETER == DEF_ENABLED)
    m_DiscoveryDummyCycles = 0;
  #endif
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Initialize
//
//   Parameter(s):  None
//   Return Value:  SystemState_e
//
//   Description:   Initialize the QSPI module
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::Initialize(void)
{
    nOS_Error       Error;
    TickCount_t     TickStart;
    SystemState_e   State;
    ISR_Prio_t      ISR_Prio;
    uint32_t        PriorityGroup;

    if(m_IsItInitialize == false)
    {
    	m_IsItInitialize = true;
    	Error = nOS_MutexCreate(&m_Mutex, NOS_MUTEX_RECURSIVE, NOS_MUTEX_PRIO_INHERIT);
    	VAR_UNUSED(Error);
    }

  #if (QSPI_SUPPORT_FLASH_RESET_PIN == DEF_ENABLED)
    IO_PinInit(IO_FLASH_RESET);                                     // Init pin reset for Flash at low level (RESET state)
  #endif

    // Enable clock on QSPI module and DMA
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    RCC->AHB3ENR |= RCC_AHB3ENR_QSPIEN;

    // Reset the QuadSPI memory interface
    RCC->AHB3RSTR |=   RCC_AHB3RSTR_QSPIRST;
    RCC->AHB3RSTR &= ~(RCC_AHB3RSTR_QSPIRST);

    // IO level initialization
    for(uint32_t IO_Id = uint32_t(IO_QSPI_BK1_IO0); IO_Id <= uint32_t(IO_QSPI_BK1_IO3); IO_Id++)
    {
        IO_PinInit(IO_ID_e(IO_Id));
    }

    IO_PinInit(IO_QSPI_CLK);
    IO_PinInit(IO_QSPI_CS);

  #if (QSPI_SUPPORT_DUAL_FLASH_MODE == DEF_ENABLED)                             // Not tested (future use)
    for(uint32_t IO_Id = uint32_t(IO_QSPI_BK2_IO0); IO_Id <= uint32_t(IO_QSPI_BK2_IO3); IO_Id++)
    {
        IO_PinInit(IO_ID_e(IO_Id));
    }

    IO_PinInit(IO_QSPI_CS2); // ??
    // todo support configuration for share pin

  #endif

    // Configure the DMA channel -----------------------------------------------

    DMA2_Stream7->CR &= ~DMA_SxCR_EN;                                           // Disable the DMA channel

    TickStart = GetTick();

    while((DMA2_Stream7->CR & DMA_SxCR_EN) != 0)                                // Check if the DMA channel is effectively disabled
    {
        if(TickHasTimeOut(TickStart, QSPI_TIMEOUT_DMA_ABORT) == true)           // Check for the Timeout
        {
            m_ErrorCode = SYS_TIME_OUT;                                         // Update error code
            m_IsItBusy  = false;
            return SYS_ERROR;
        }
    }

    DMA2_Stream7->PAR  = (uint32_t)&QUADSPI->DR;                                // Set QUADSPI register address
    DMA2_Stream7->NDTR = 0;                                                     // Reset NDTR Register

    // Write DMA Channel configuration
    DMA2_Stream7->CR = DMA_MODE_NORMAL          |
                       DMA_MEMORY_INCREMENT     |
                       DMA_PERIPH_NO_INCREMENT  |
                       DMA_P_DATA_ALIGN_BYTE    |
                       DMA_M_DATA_ALIGN_BYTE    |
                       DMA_P_BURST_SINGLE       |
                       DMA_M_BURST_SINGLE       |
                       DMA_PRIORITY_HIGH        |
                       DMA_CHANNEL_3;

    // clear all flag
    DMA2->HIFCR = (DMA_HIFCR_CTCIF7   |
                   DMA_HIFCR_CHTIF7   |
                   DMA_HIFCR_CTEIF7   |
                   DMA_HIFCR_CDMEIF7  |
                   DMA_HIFCR_CFEIF7);

    m_DMA_ErrorCode = SYS_READY;                                                // Initialize the error code

    // Initialize QuadSPI ------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// define(s) and macro(s)
//-------------------------------------------------------------------------------------------------

    // Configure QSPI FIFO Threshold
    MODIFY_REG(QUADSPI->CR,
               QUADSPI_CR_FTHRES,                                               // Bit to modify
               ((QSPI_FIFO_THRESHOLD_CFG - 1) << QUADSPI_CR_FTHRES_Pos));       // Configure QSPI FIFO Threshold

    State = this->WaitFlagStateUntilTimeout(QUADSPI_SR_BUSY,
                                            0,
                                            QSPI_TIMEOUT_DEFAULT_VALUE);        // Wait till BUSY flag reset

    if(State == SYS_READY)
    {
        // Configure QSPI Clock Prescaler and Sample Shift
        MODIFY_REG(QUADSPI->CR,
                   (QUADSPI_CR_PRESCALER |
                    QUADSPI_CR_SSHIFT    |
                    QUADSPI_CR_FSEL      |
                    QUADSPI_CR_DFM),                                            // Bit to modify
                   ((QSPI_PRESCALER)          |                                 // Config prescaler
                     QSPI_SAMPLE_SHIFTING_CFG |                                 // Config sample shifting
                     QSPI_FLASH_ID_CFG        |                                 // Config Flash ID bank 1 or bank 2
                     QSPI_DUAL_FLASH_MODE_CFG));                                // Config QSPI single or dual flash

        // Configure QSPI Flash Size, CS High Time and Clock Mode
        MODIFY_REG(QUADSPI->DCR,
                   (QUADSPI_DCR_FSIZE |
                    QUADSPI_DCR_CSHT  |
                    QUADSPI_DCR_CKMODE),                                        // Bit to modify
                   ((QSPI_MEMORY_SIZE_DCR_CFG << QUADSPI_DCR_FSIZE_Pos) |
                     QSPI_CS_HIGH_TIME_CFG                              |
                     QSPI_CLOCK_MODE_CFG));

        SET_BIT(QUADSPI->CR, QUADSPI_CR_EN);                                    // Enable the QSPI peripheral.
        m_State     = QSPI_STATE_READY;                                         // Initialize the QSPI state
        m_ErrorCode = SYS_READY;                                                // Set QSPI error code to none

      #if (QSPI_SUPPORT_FLASH_RESET_PIN == DEF_ENABLED)
        IO_SetPinHigh(FLASH_RESET);                                         // release RESET on serial flash
      #endif

        // Configure interrupt priority for QUADSPI
        PriorityGroup = NVIC_GetPriorityGrouping();
        ISR_Prio.PriorityGroup = PriorityGroup;
        ISR_Prio.SubPriority   = 0;

        // NVIC configuration for QuadSPI interrupt
        ISR_Prio.PremptionPriority = QSPI_ISR_PRIORITY_CFG;
        ISR_Init(QUADSPI_IRQn, &ISR_Prio);
        // NVIC configuration for DMA interrupt
        ISR_Prio.PremptionPriority = QSPI_DMA_ISR_PRIORITY_CFG;
        ISR_Init(DMA2_Stream7_IRQn, &ISR_Prio);

        m_State = QSPI_STATE_READY;
    }

    m_IsItBusy = false;

    if(State == SYS_READY)
    {
        State = this->QuadEnable();

      #if (QSPI_READ_DISCOVERY_PARAMETER == DEF_ENABLED)
        if(State == SYS_READY)
        {
            State = this->ReadCommand(QSPI_CMD_READ_DISCOVERY_PARAMETER,
                                      0,                                            // No address for this command
                                      &m_DiscoveryDummyCycles,
                                      1,                                            // Number of byte to read
                                      QSPI_TIMEOUT_DEFAULT_VALUE);
        }
      #endif
    }

    if(State == SYS_READY)
    {

        m_Status = SYS_READY;
    }

    return State;                                                                   // Return function status
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Read
//
//   Parameter(s):  Address     Read start address
//                  pBuffer     Pointer to data to be read
//                  Size        Size of data to read
//   Return Value:  SystemState_e
//
//   Description:   Reads an amount of data from the QSPI memory.
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::Read(uint32_t Address, uint8_t* pBuffer, uint32_t Size)
{
    SystemState_e State = SYS_BUSY;

    this->Lock();
    State = this->ReadCommand(QSPI_CMD_FAST_READ_QUAD, Address, pBuffer, Size, QSPI_TIMEOUT_DEFAULT_VALUE);
    this->Unlock();
    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Write
//
//   Parameter(s):  Address     Write start address
//                  pBuffer     Pointer to data to be written
//                  Size        Size of data to write
//   Return Value:  SystemState_e
//
//   Description:   Write an amount of data to the QSPI memory.
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::Write(uint32_t Address, uint8_t* pBuffer, uint32_t Size)
{
    SystemState_e State = SYS_BUSY;
    uint32_t InSectorSize;              // Size of new data in the sector
    uint8_t* PBuf;

    this->Lock();
    PBuf = (uint8_t*)pBuffer;

    do // Sector loop
    {
        InSectorSize = QSPI_FLASH_SECTOR_SIZE - (Address % QSPI_FLASH_SECTOR_SIZE);

        if(Size < InSectorSize)
        {
            InSectorSize = Size;
        }

        if((State = this->WriteInSector(Address, PBuf, InSectorSize)) == SYS_ALREADY_BLANK)
        {
            State = SYS_READY;
        }

        // Update address and length left as per sector update
        Address += InSectorSize;
        Size    -= InSectorSize;
        PBuf    += InSectorSize;
    }
    while((Size != 0) && (State == SYS_READY));

    this->Unlock();
    return State;
}

/**********************************************************************************************************************************
 * @brief           Erases the specified sector of the QSPI memory.
 *
 * @param           SectorAddress   Sector address to erase
 *
 * @return          error_t         Error
 *
 *********************************************************************************************************************************/
SystemState_e QSPI_Driver::EraseSector(uint32_t SectorAddress)
{
    SystemState_e State;

    this->Lock();
    State = this->InternalEraseSector(SectorAddress);
    this->Unlock();
    return State;
}

/**********************************************************************************************************************************
 * @brief           Erases the entire QSPI memory.
 *
 * @param           None
 *
 * @return          error_t         Error
 *
 *********************************************************************************************************************************/
SystemState_e QSPI_Driver::EraseChip(void)
{
    SystemState_e State;

    this->Lock();

    // Enable write operations
    if((State = this->WriteEnable()) == SYS_READY)
    {

        while(this->WaitFlashReady() == SYS_BUSY)
        {
            nOS_Yield();
        };

        if((State = this->Command_IT(QSPI_CMD_CHIP_ERASE, 0, 0)) == SYS_READY)
        {
            while(this->WaitFlashReady() == SYS_BUSY)
            {
                nOS_Yield();
            };

            while(this->AutoPolling(QSPI_CMD_READ_STATUS_QUAD,
                                    0,
                                    QSPI_SR_BUSY_BIT,
                                    QSPI_TIMEOUT_DEFAULT_VALUE) != SYS_READY) { };
        }
    }
    this->Unlock();

    return State;
}

/**********************************************************************************************************************************
 * @brief           This function return the status on the flash, and kick the auto polling if necessary.
 *
 * @param           None
 *
 * @return          error_t         Error
 *
 *********************************************************************************************************************************/
SystemState_e QSPI_Driver::FlashReady(void)
{
    SystemState_e State = SYS_BUSY;

    this->Lock();
    State = this->WaitFlashReady();
    this->Unlock();
    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      GetStatus
//
//   Parameter(s):
//   Return Value:
//
//   Description:    SystemState_e  Return general status of the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::GetStatus(void)
{
    return m_Status;
}

//-------------------------------------------------------------------------------------------------
//
//  ****  ****  * *   *   *  ***** *****
//  *   * *   * * *   *  * *   *   *
//  ****  ****  * *   * *   *  *   ****
//  *     *  *  *  * *  *****  *   *
//  *     *   * *   *   *   *  *   *****
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Function:       WaitReady
//
//  Parameter(s):   Timeout      Duration of the time out
//  Return Value:
//
//  Description:    Wait for the QSPI module to be ready.
//
//  Note(s):        return  SYS_READY     if available
//                          SYS_TIME_OUT  if busy pass timeout
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::WaitReady(uint32_t TimeOut)
{
    SystemState_e State = SYS_READY;
    TickCount_t TickStart = GetTick();

    while(((QUADSPI->SR & QUADSPI_SR_BUSY) != 0) && (State == SYS_READY))   // Wait until flag busy is cleared or time out
    {
        nOS_Yield();

        if(TickHasTimeOut(TickStart, TimeOut) == true)
        {
            State = SYS_TIME_OUT;
        }
    }

    while((m_State != QSPI_STATE_READY) && (State == SYS_READY))            // Wait until flag is in expected state
    {
        if(TimeOut != QSPI_MAX_DELAY)                                       // Check for the Timeout
        {
            if(TickHasTimeOut(TickStart, TimeOut) == true)
            {
                State = SYS_TIME_OUT;
            }
        }
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       WaitReady
//
//  Parameter(s):   None
//  Return Value:   SYS_READY
//
//  Description:    Wait for the write enable to be ready, stay in if busy.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::WriteEnable(void)
{
    SystemState_e State;

    do
    {
        // Enable write operations
        if((State = this->Command(QSPI_CMD_WRITE_ENABLE_QUAD, 0, 0, QSPI_TIMEOUT_DEFAULT_VALUE)) == SYS_READY)
        {
            this->WaitReady(QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE);
        }
    }
    while(State == SYS_BUSY);

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       WaitFlashReady
//
//  Parameter(s):   None
//  Return Value:   SystemState_e
//
//  Description:    Return the status on the flash, and kick the auto polling if necessary.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::WaitFlashReady(void)
{
    SystemState_e State = SYS_READY;

    if(m_KickAutoPolling == true)
    {
        if(m_State == QSPI_STATE_READY)
        {
            m_ErrorCode = SYS_READY;
            m_State = QSPI_STATE_BUSY_AUTO_POLLING;                                     // Update state
            State = this->WaitFlagStateUntilTimeout(QUADSPI_SR_BUSY,
                                                    0,
                                                    QSPI_TIMEOUT_DEFAULT_VALUE);        // Wait till BUSY flag reset

            if(State == SYS_READY)
            {
                m_KickAutoPolling = false;                                              // Reset flag
                QUADSPI->PSMAR = 0;                                                     // PSMAR register with the Status match value
                QUADSPI->PSMKR = QSPI_SR_BUSY_BIT;                                      // PSMKR register with the Status mask value
                QUADSPI->PIR   = QSPI_AUTOMATIC_POLLING_INTERVAL_CFG;                   // PIR register with the interval value
                MODIFY_REG(QUADSPI->CR,
                           (QUADSPI_CR_PMM | QUADSPI_CR_APMS),
                           (QSPI_MATCH_MODE_AND | QSPI_AUTOMATIC_STOP_ENABLE));         // CR register with Match mode and Automatic stop mode
                QUADSPI->FCR = (QUADSPI_FCR_CTEF | QUADSPI_FCR_CSMF);                   // Clear interrupt
                this->Config(QSPI_CMD_READ_STATUS_QUAD,
                             0,
                             QSPI_STATUS_BYTE_SIZE,
                             QSPI_FUNCTIONAL_MODE_AUTO_POLLING);                        // Call the configuration function
                SET_BIT(QUADSPI->CR, (QUADSPI_CR_TEIE | QUADSPI_CR_SMIE));              // Enable the QSPI Transfer Error and Status match Interrupt
            }
        }
    }

    if(State == SYS_READY)
    {
        while(m_State != QSPI_STATE_READY)
        {
            nOS_Yield();
        }
    }

    return State;                                                                       // Return function status
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       WaitFlagStateUntilTimeout
//
//  Parameter(s):   Flag        Flag checked.
//                  State       Value of the flag expected.
//                  TimeOut     Duration of the timeout.
//  Return Value:   SystemState_e
//
//  Description:    Wait for a flag state until timeout.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::WaitFlagStateUntilTimeout(uint32_t Flag, uint32_t State, TickCount_t TimeOut)
{
    TickCount_t TickStart;
    TickStart = GetTick();

    while(((READ_BIT(QUADSPI->SR, Flag) != 0) ? 1 : 0) != State)          // Wait until flag is in expected state
    {
        if((TickHasTimeOut(TickStart, TimeOut) == true) || (TimeOut == 0))
        {
            m_State     = QSPI_STATE_ERROR;
            m_ErrorCode = SYS_TIME_OUT;
            return SYS_ERROR;
        }
    }
    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       InternalEraseSector
//
//  Parameter(s):   SectorAddress   Erase sector address
//  Return Value:   SystemState_e
//
//  Description:    Wait for a flag state until timeout.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::InternalEraseSector(uint32_t SectorAddress)
{
    SystemState_e State;

    // Enable write operations
    if((State = this->WriteEnable()) == SYS_READY)
    {
        while(this->WaitFlashReady() == SYS_BUSY)
        {
            nOS_Yield();
        };

        if((State = this->Command_IT(QSPI_CMD_SECTOR_ERASE, SectorAddress, 0)) == SYS_READY)
        {
            while(this->WaitFlashReady() == SYS_BUSY)
            {
                nOS_Yield();
            };

            while(this->AutoPolling(QSPI_CMD_READ_STATUS_QUAD,
                                    0,
                                    QSPI_SR_BUSY_BIT,
                                    QSPI_TIMEOUT_DEFAULT_VALUE) != SYS_READY) { };
        }
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       PageProgram
//
//  Parameter(s):   Address     Write start address
//                  pBuffer     Pointer to data to be written
//                  Size        Size of data to write
//  Return Value:   SystemState_e
//
//  Description:    Program a page of data in flash.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::PageProgram(uint32_t Address, uint8_t* pBuffer, uint32_t Size)
{
    // Loop all data, if one is found not blank, then the page is written, if not skip writing
    for(uint32_t i = 0; i < Size; i++)
    {
        if(pBuffer[i] != 0xFF)
        {
            SystemState_e State;

            // Enable write operations
            if((State = this->WriteEnable()) == SYS_READY)
            {
                while(this->WaitFlashReady() == SYS_READY)
                {
                    nOS_Yield();
                };

                if((State = this->WriteCommand(QSPI_CMD_PAGE_PROG_QUAD, Address, pBuffer, Size, QSPI_TIMEOUT_DEFAULT_VALUE)) == SYS_READY)
                {
                    while((State = this->AutoPolling(QSPI_CMD_READ_STATUS_QUAD,
                                                     0,
                                                     QSPI_SR_BUSY_BIT,
                                                     QSPI_TIMEOUT_DEFAULT_VALUE)) != SYS_READY) { };
                }
            }

            return State;
        }
    }

    return SYS_ALREADY_BLANK;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       WriteInSector
//
//  Parameter(s):   uint32_t Address    Address in flash for data write
//                  uint8_t* pData      Pointer on buffer data
//                  uint32_t Size       Length of data
//  Return Value:   SystemState_e       Succeed to wait for flash readiness (SYS_READY)
//                                      Other are error
//
//  Description:    Write in a sector.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::WriteInSector(uint32_t Address, uint8_t* pBuffer, uint32_t Size)
{
    SystemState_e State;
    uint32_t      SectorAddress;             // First addrSYS_READYess in the sector
    uint32_t      Offset;                    // This is the offset for the internal working buffer
    uint8_t*      pBuf;                      // Pointer on data to flash
    uint8_t*      pBufTp;                    // Pointer on temporary sector data
    uint32_t      PageAddress;               // Address of first page
    uint32_t      PageSize;                  // Number of data for the page
    uint32_t      NumberOfPage;              // Number of page to update for this sector.
    bool          ProcessThisSector;
    uint8_t*      pSectorBuffer;
    uint32_t      LeftTowriteInLastPage;

    //---------------------------------------------------------------------------------------------
    // Get necessary memory to read sector
    if((pSectorBuffer = (uint8_t*)pMemory->Alloc(QSPI_FLASH_SECTOR_SIZE)) == nullptr)
    {
        return SYS_FAIL_MEMORY_ALLOCATION;
    }

    //---------------------------------------------------------------------------------------------
    // Calculate Address and size for current sector
    SectorAddress = Address - (Address % QSPI_FLASH_SECTOR_SIZE);
    Offset        = Address - SectorAddress;

    //---------------------------------------------------------------------------------------------
    // Get the actual data in the sector
    if((State = this->ReadCommand(QSPI_CMD_FAST_READ_QUAD,
                                  SectorAddress,
                                  &pSectorBuffer[0],
                                  QSPI_FLASH_SECTOR_SIZE,
                                  QSPI_TIMEOUT_DEFAULT_VALUE)) != SYS_READY)
    {
        pMemory->Free((void**)&pSectorBuffer);
        return State;
    }

    //---------------------------------------------------------------------------------------------
    // Check if data to write is the same

    pBuf   = pBuffer;
    pBufTp = &pSectorBuffer[Offset];
    ProcessThisSector = false;

    // Check each byte
    for(uint32_t i = 0; i < Size; i++)
    {
        if(*pBufTp != *pBuf)
        {
            ProcessThisSector = true;
            i = Size;
        }
        else
        {
            pBuf++;
            pBufTp++;
        }
    }

    if(ProcessThisSector == false)
    {
        pMemory->Free((void**)&pSectorBuffer);
        return SYS_READY;
    }

    //---------------------------------------------------------------------------------------------
    // Check if we need to erase the sector

    // 1 - Drop all bit in the range we want to update
    pBuf   = pBuffer;
    pBufTp = &pSectorBuffer[Offset];
    ProcessThisSector = false;

    // Drop bit's
    for(uint32_t i = 0; i < Size; i++)
    {
        if((*pBufTp & *pBuf) != *pBuf)
        {
            ProcessThisSector = true;          // Proceed with erase
            i = Size;
        }
        else
        {
            pBuf++;
            pBufTp++;
        }
    }

    // 2 - If after dropping bit's, data is not the same we need to erase the sector
    if(ProcessThisSector == true)
    {
        // Erase Sector
        if((State = this->InternalEraseSector(SectorAddress)) != SYS_READY)
        {
            pMemory->Free((void**)&pSectorBuffer);
            return State;
        }

        // Wait for erase to be completed
        if((State = this->WaitFlashReady(QSPI_SECTOR_ERASE_TIME)) != SYS_READY)
        {
            pMemory->Free((void**)&pSectorBuffer);
            return State;
        }

        PageAddress           = SectorAddress;
        pBufTp                = &pSectorBuffer[0];
        NumberOfPage          = QSPI_FLASH_SECTOR_SIZE / QSPI_FLASH_PAGE_SIZE;
        LeftTowriteInLastPage = 0;
    }
    else
    {
        PageAddress  = Address;
        pBufTp       = &pSectorBuffer[Offset];
        NumberOfPage = Size / QSPI_FLASH_PAGE_SIZE;
        LeftTowriteInLastPage = Size % QSPI_FLASH_PAGE_SIZE;

        if(LeftTowriteInLastPage != 0)
        {
            NumberOfPage++;
        }
    }

    // 3 - Copy data into buffer

    // Copy data directly
    memcpy(&pSectorBuffer[Offset], pBuffer, Size);

    //---------------------------------------------------------------------------------------------
    // Program page
    do
    {
        PageSize = QSPI_FLASH_PAGE_SIZE;

        if((NumberOfPage == 1) && (LeftTowriteInLastPage != 0))
        {
            PageSize = LeftTowriteInLastPage;
        }

        State = this->PageProgram(PageAddress, pBufTp, PageSize);

        pBufTp      += PageSize;
        PageAddress += PageSize;
        NumberOfPage--;
    }
    while((NumberOfPage != 0) && (State == SYS_READY));

    pMemory->Free((void**)&pSectorBuffer);
    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       QuadEnable
//
//  Parameter(s):   None
//  Return Value:   SystemState_e
//
//  Description:    Set the QUAD Enable bit.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::QuadEnable(void)
{
    SystemState_e State;
    uint8_t       Status;

    // Make sure we are not in already enable!
    if((State = this->Command(QSPI_CMD_EXIT_QUAD_MODE, 0, 0, QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE)) == SYS_READY)
    {
        if((State = this->ReadCommand(QSPI_CMD_READ_STATUS_SINGLE, 0, &Status, 1, QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE)) == SYS_READY)
        {
            this->WaitReady(QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE);    // Wait operation to be completed as Status is modified in DMA

            if((Status & QSPI_SR_QUAD_ENABLE_BIT) == 0)
            {
                if((State = this->Command(QSPI_CMD_WRITE_ENABLE_SINGLE, 0, 0, QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE)) == SYS_READY)
                {
                    // Writing New Status
                    Status &= QSPI_SR_WRITE_MASK_BIT;
                    Status |= QSPI_SR_QUAD_ENABLE_BIT;

                    if((State = this->WriteCommand(QSPI_CMD_WRITE_STATUS_SINGLE, 0, &Status, 1, QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE)) == SYS_READY)
                    {
                        this->WaitReady(QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE);
                    }
                    else
                    {
                       return State;
                    }
                }
            }

            if((State = this->Command(QSPI_CMD_ENTER_QUAD_MODE, 0, 0, QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE)) == SYS_READY)
            {
                if((State = this->ReadCommand(QSPI_CMD_READ_STATUS_QUAD, 0, &Status, 1, QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE)) == SYS_READY)
                {
                    if((Status & QSPI_SR_QUAD_ENABLE_BIT) == 0)
                    {
                        State = SYS_INITIALIZATION_FAIL;
                    }
                }
            }
        }
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       AutoPolling
//
//  Parameter(s):   CmdID        The command ID for structure that contains configuration information.
//                  Match        Value to match.
//                  Mask         Bits to check.
//                  Timeout      Timeout duration.
//  Return Value:   SystemState_e
//
//  Description:    Configure the QSPI Automatic Polling Mode in blocking mode.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::AutoPolling(QSPI_CmdList_e CmdID, uint32_t Match, uint32_t Mask, TickCount_t TimeOut)
{
    SystemState_e State;

    if(m_State == QSPI_STATE_READY)
    {
        m_ErrorCode = SYS_READY;
        m_State     = QSPI_STATE_BUSY_AUTO_POLLING;                     // Update state
        State       = this->WaitFlagStateUntilTimeout(QUADSPI_SR_BUSY,
                                                      0,
                                                      TimeOut);         // Wait till BUSY flag reset

        if(State == SYS_READY)
        {
            QUADSPI->PSMAR = Match;                                     // Config PSMAR register with the status match value
            QUADSPI->PSMKR = Mask;                                      // Config PSMKR register with the status mask value
            QUADSPI->PIR   = QSPI_AUTOMATIC_POLLING_INTERVAL_CFG;       // Config PIR register with the interval value

            // Config CR register with Match mode and Automatic stop enabled (otherwise there will be an infinite loop in blocking mode)
            MODIFY_REG(QUADSPI->CR,
                       (QUADSPI_CR_PMM | QUADSPI_CR_APMS),
                       (QSPI_MATCH_MODE_AND | QSPI_AUTOMATIC_STOP_ENABLE));
            this->Config(CmdID,
                         0,
                         QSPI_STATUS_BYTE_SIZE,
                         QSPI_FUNCTIONAL_MODE_AUTO_POLLING);            // Call the configuration function
            State = this->WaitFlagStateUntilTimeout(QUADSPI_SR_SMF,
                                                    1,
                                                    TimeOut);           // Wait until SM flag is set to go back in idle state
            if(State == SYS_READY)
            {
                QUADSPI->FCR  = QUADSPI_FCR_CSMF;
                m_State = QSPI_STATE_READY;                             // Update state
            }
        }
    }
    else
    {
        State = SYS_BUSY;
    }

    return State;                                                       // Return function status
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       Config
//
//  Parameter(s):   CmdID            The command ID for structure that contains configuration information.
//                  Address          Address to send.
//                  Size             Size of the data
//                  FunctionalMode   Functional mode to configured
//                                      This parameter can be one of the following values:
//                                          QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE:    Indirect write mode
//                                          QSPI_FUNCTIONAL_MODE_AUTO_POLLING:      Automatic polling mode
//  Return Value:   None
//
//  Description:    Configure the communication registers.
//
//-------------------------------------------------------------------------------------------------
void QSPI_Driver::Config(QSPI_CmdList_e CmdID, uint32_t Address, uint32_t Size, uint32_t FunctionalMode)
{
  #if (QSPI_READ_DISCOVERY_PARAMETER == DEF_ENABLED)
    uint8_t DummyCycles;
  #endif

    QSPI_Command_t* pCmd = (QSPI_Command_t*)&m_Cmd[CmdID];

    QUADSPI->FCR = (QUADSPI_FCR_CTEF | QUADSPI_FCR_CTCF);               // Clear interrupt: Transmit error and transmit complete

    if(pCmd->DataMode != QSPI_DATA_NONE)
    {
        QUADSPI->DLR = Size - 1;                                        // Configure QSPI: DLR register with the number of data to read or write
    }

  #if (QSPI_READ_DISCOVERY_PARAMETER == DEF_ENABLED)
    DummyCycles = (CmdID == QSPI_CMD_READ_DISCOVERY_PARAMETER) ? m_DiscoveryDummyCycles : pCmd->DummyCycles;
  #endif

    uint32_t Config = (QSPI_COMMON_CONFIG    |
                       pCmd->DataMode        |
                       pCmd->Instruction     |
                       pCmd->InstructionMode |
                       pCmd->AddressMode     |
                       FunctionalMode        |
                     #if (QSPI_READ_DISCOVERY_PARAMETER == DEF_ENABLED)
                       (DummyCycles << QUADSPI_CCR_DCYC_Pos));
                     #else
                       (pCmd->DummyCycles << QUADSPI_CCR_DCYC_Pos));
                     #endif

    //---- Preset command with common config ----
    if(pCmd->AddressMode != QSPI_ADDRESS_NONE)
    {
        //---- Command with instruction and address ----
        // Configure QSPI: CCR register with all communications parameters
        QUADSPI->CCR = (Config | QSPI_ADDRESS_32_BITS);
        QUADSPI->AR  = Address;                                         // Configure QSPI: AR register with address value
    }
    else
    {
        //---- Command with only instruction ----
        // Configure QSPI: CCR register with all communications parameters
        QUADSPI->CCR = Config;
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       ReadCommand
//
//  Parameter(s):   CmdID        The command ID for structure that contains configuration information.
//                  Address      Address in flash for this command
//                  pData        Pointer on buffer to return data
//                  Size         Size of data to read
//                  Timeout      Timeout duration
//  Return Value:   SystemState_e
//
//  Description:    Read data from specific command.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::ReadCommand(QSPI_CmdList_e CmdID, uint32_t Address, uint8_t* pData, uint32_t Size, TickCount_t TimeOut)
{
    SystemState_e State;

    if(pData != nullptr)
    {
        if((State = this->Command(CmdID, Address, Size, TimeOut)) == SYS_READY)
        {
            if((State = this->DMA_Receive(pData, Size)) == SYS_READY)
            {
                // Wait operation to be completed as Status is modified in DMA
                State = this->WaitReady(QSPI_TIMEOUT_READ_COMMAND);
            }
        }
    }
    else
    {
        State = SYS_READY;
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       WriteCommand
//
//  Parameter(s):   CmdID        The command ID for structure that contains configuration information.
//                  Address      Address in flash for this command
//                  pData        Pointer on buffer to return data
//                  Size         Size of data to read
//                  Timeout      Timeout duration
//  Return Value:   SystemState_e
//
//  Description:    Write data for specific command.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::WriteCommand(QSPI_CmdList_e CmdID, uint32_t Address, uint8_t* pData, uint32_t Size, TickCount_t TimeOut)
{
    SystemState_e State;
    TickCount_t   TickStart;

    if(pData != nullptr)
    {
        TickStart = GetTick();

        if((State = this->Command(CmdID, Address, Size, TimeOut)) == SYS_READY)
        {
            if((State = this->DMA_Transmit(pData, Size)) == SYS_READY)
            {
                while((this->WaitFlashReady() == SYS_BUSY) && (State != SYS_TIME_OUT))
                {
                    nOS_Yield();

                    if(TickHasTimeOut(TickStart, TimeOut) == true)
                    {
                        State = SYS_TIME_OUT;
                    }
                }

                // Wait operation to be completed as Status is modified in DMA
                State = this->WaitReady(QSPI_TIMEOUT_SHORT_TIMEOUT_VALUE);
            }
        }
    }
    else
    {
        State = SYS_ERROR;
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       Command
//
//  Parameter(s):   CmdID        The command ID for structure that contains configuration information.
//                  Address      Address in flash for this command
//                  Size         Size of data to read
//                  Timeout      Timeout duration
//  Return Value:   SystemState_e
//
//  Description:    Set the command configuration.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::Command(QSPI_CmdList_e CmdID, uint32_t Address, uint32_t Size, TickCount_t Timeout)
{
    SystemState_e State;

    if(m_State == QSPI_STATE_READY)
    {
        m_ErrorCode = SYS_READY;
        m_State     = QSPI_STATE_BUSY;                                          // Update QSPI state
        State = this->WaitFlagStateUntilTimeout(QUADSPI_SR_BUSY, 0, Timeout);   // Wait till BUSY flag reset

        if(State == SYS_READY)
        {
            this->Config(CmdID,
                         Address,
                         Size,
                         QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);                  // Call the configuration function

            if(m_Cmd[CmdID].DataMode == QSPI_DATA_NONE)
            {
                State = this->WaitFlagStateUntilTimeout(QUADSPI_SR_TCF,
                                                        1,
                                                        QSPI_TIMEOUT_DEFAULT_VALUE);

                if(State == SYS_READY)
                {
                    m_State = QSPI_STATE_READY;                                 // Update QSPI state
                }
            }
            else
            {
                m_State = QSPI_STATE_READY;                                     // Update QSPI state
            }
        }
    }
    else
    {
        State = SYS_BUSY;
    }

    return State;                                                               // Return function status
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       Command_IT
//
//  Parameter(s):   CmdID        The command ID for structure that contains configuration information.
//                  Address      Address in flash for this command
//                  Size         Size of data to read
//  Return Value:   SystemState_e
//
//  Description:    Set the command configuration in interrupt mode.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::Command_IT(QSPI_CmdList_e CmdID, uint32_t Address, uint32_t Size)
{
    SystemState_e   State;
    QSPI_DataMode_e DataMode  = m_Cmd[CmdID].DataMode;

    if(m_State == QSPI_STATE_READY)
    {
        m_ErrorCode = SYS_READY;
        m_State     = QSPI_STATE_BUSY;                                          // Update QSPI state
        State = this->WaitFlagStateUntilTimeout(QUADSPI_SR_BUSY,
                                                0,
                                                QSPI_TIMEOUT_DEFAULT_VALUE);    // Wait till BUSY flag reset
        if(State == SYS_READY)
        {
            if(DataMode == QSPI_DATA_NONE)
            {
                SET_BIT(QUADSPI->FCR, (QUADSPI_FCR_CTEF | QUADSPI_FCR_CTCF));   // Clear interrupt: Transmit error and transmit complete
            }

            this->Config(CmdID,
                         Address,
                         Size,
                         QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);                  // Call the configuration function

            if(DataMode == QSPI_DATA_NONE)
            {
                // When there is no data phase, the transfer start as soon as the configuration is done so activate TC and TE interrupts
                SET_BIT(QUADSPI->CR, QUADSPI_CR_TEIE | QUADSPI_CR_TCIE);        // Enable the QSPI Transfer Error Interrupt
            }
            else
            {
                m_State = QSPI_STATE_READY;                                     // Update QSPI state
            }
        }
    }
    else
    {
        State = SYS_BUSY;
    }

    return State;                                                               // Return function status
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       Abort_IT
//
//  Parameter(s):   None
//  Return Value:   SystemState_e
//
//  Description:    Abort the current transmission (non-blocking function)
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::Abort_IT(void)
{
    SystemState_e State = SYS_READY;

    if((m_State & QSPI_STATE_BUSY) != 0)                                // Check if the state is in one of the busy states
    {
        m_State = QSPI_STATE_ABORT;                                     // Update QSPI state
        CLEAR_BIT(QUADSPI->CR, (QUADSPI_CR_TOIE |
                                QUADSPI_CR_SMIE |
                                QUADSPI_CR_FTIE |
                                QUADSPI_CR_TCIE |
                                QUADSPI_CR_TEIE));                      // Disable all interrupts

        if(READ_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN) != 0)
        {
            CLEAR_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN);                   // Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register

            if(this->DMA_Abort() != SYS_READY)
            {
                m_ErrorCode = SYS_ERROR;                                // Set error code to DMA
                State       = SYS_ERROR;
            }
        }
        else
        {
            QUADSPI->FCR = QUADSPI_FCR_CTCF;                            // Clear interrupt
            SET_BIT(QUADSPI->CR, (QUADSPI_CR_TCIE |                     // Enable the QSPI Transfer Complete Interrupt and
                                  QUADSPI_CR_ABORT));                   // Configure QSPI with Abort request
        }
    }
    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Lock
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Lock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void QSPI_Driver::Lock(void)
{
    while(nOS_MutexLock(&m_Mutex, NOS_WAIT_INFINITE) != NOS_OK) {};
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Unlock
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Unlock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void QSPI_Driver::Unlock(void)
{
    nOS_MutexUnlock(&m_Mutex);
}

/**********************************************************************************************************************************
 * @brief           This function handles QUADSPI interrupt request.
 *
 * @param           None
 *
 * @return          None
 *
 *********************************************************************************************************************************/
extern "C" void QUADSPI_IRQHandler(void)
{
    uint32_t Flag      = QUADSPI->SR;
    uint32_t IT_Source = QUADSPI->CR;

    // QSPI Transfer Complete interrupt occurred -------------------------------
    if(((Flag & QUADSPI_SR_TCF) != 0) && ((IT_Source & QUADSPI_CR_TCIE) != 0))
    {
        QUADSPI->FCR = QUADSPI_FCR_CTCF;                            // Clear interrupt
        CLEAR_BIT(QUADSPI->CR, (QUADSPI_CR_TCIE |
                                QUADSPI_CR_TEIE ));                 // Disable the QSPI Transfer Error and Transfer complete Interrupts

        // Transfer complete callback
        if(QSPI.m_State == QSPI_STATE_BUSY_INDIRECT_TX)
        {
            if(READ_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN) != 0)
            {
                CLEAR_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN);           // Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register
                DMA2_Stream7->CR &= ~DMA_SxCR_EN;                   // Disable the DMA channel
            }

            QSPI.m_State = QSPI_STATE_READY;                        // Change state of QSPI
            QSPI.m_KickAutoPolling = true;                          // When transfer is completed
        }
        else if(QSPI.m_State == QSPI_STATE_BUSY_INDIRECT_RX)
        {
            if(READ_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN) != 0)
            {
                CLEAR_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN);           // Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register
                DMA2_Stream7->CR &= ~DMA_SxCR_EN;                   // Disable the DMA channel
            }

            QSPI.m_State = QSPI_STATE_READY;                        // Change state of QSPI
        }
        else if(QSPI.m_State == QSPI_STATE_BUSY)
        {
            QSPI.m_State = QSPI_STATE_READY;                        // Change state of QSPI
        }
        else if(QSPI.m_State == QSPI_STATE_ABORT)
        {
            CLEAR_BIT(QUADSPI->CCR, QUADSPI_CCR_FMODE);             // Reset functional mode configuration to indirect write mode by default
            QSPI.m_State = QSPI_STATE_READY;                        // Change state of QSPI

            if(QSPI.m_ErrorCode == SYS_READY)
            {
                // Abort called by the user
                //QSPI.AbortCpltCallback();                           // Abort Complete callback
            }
            else
            {
                // Abort due to an error (eg :  DMA error)
                //QSPI.ErrorCallback();                              // Error callback

            }
        }
    }

    // QSPI Status Match interrupt occurred ------------------------------------
    else if(((Flag & QUADSPI_SR_SMF) != 0) && ((IT_Source & QUADSPI_CR_SMIE) != 0))
    {
        QUADSPI->FCR = QUADSPI_FCR_CSMF;                            // Clear interrupt

        if(READ_BIT(QUADSPI->CR, QUADSPI_CR_APMS) != 0)             // Check if the automatic poll mode stop is activated
        {
            CLEAR_BIT(QUADSPI->CR, (QUADSPI_CR_SMIE |               // Disable Status Match Interrupts
                                    QUADSPI_CR_TEIE));              //    and Transfer Error
            QSPI.m_State = QSPI_STATE_READY;                        // Change state of QSPI
        }
    }

    // QSPI Transfer Error interrupt occurred ---------------------------------
    else if(((Flag & QUADSPI_SR_TEF) != 0) && ((IT_Source & QUADSPI_CR_TEIE) != 0))
    {
        QUADSPI->FCR = QUADSPI_FCR_CTEF;                            // Clear interrupt
        CLEAR_BIT(QUADSPI->CR, (QUADSPI_CR_SMIE |
                                QUADSPI_CR_TCIE |
                                QUADSPI_CR_TEIE |
                                QUADSPI_CR_FTIE));                  // Disable all the QSPI Interrupts
        QSPI.m_ErrorCode = SYS_TRANSFER_ERROR;                      // Set error code

        if(READ_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN) != 0)
        {
            CLEAR_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN);               // Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register
            //QSPI.XferAbortCallback = QSPI.DMA_AbortCplt;

            if(QSPI.DMA_Abort_IT() != SYS_READY)
            {
                QSPI.m_ErrorCode = SYS_DMA_ERROR;                   // Set error code to DMA
                QSPI.m_State     = QSPI_STATE_READY;                // Change state of QSPI
              //  QSPI.ErrorCallback();                               // Error callback
            }
        }
        else
        {
            QSPI.m_State = QSPI_STATE_READY;                        // Change state of QSPI
            //QSPI.ErrorCallback();                                  // Error callback
        }
    }
}

//-------------------------------------------------------------------------------------------------
//  DDDDDD  MM     MM  AAAAA
//  DD   DD MMMM MMMM AA   AA
//  DD   DD MM MMM MM AAAAAAA
//  DD   DD MM  M  MM AA   AA
//  DDDDDD  MM     MM AA   AA
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_Transmit
//
//  Parameter(s):   pData        Pointer to data buffer.
//                  Size         Size of the DMA TX.
//  Return Value:   SystemState_e
//
//  Description:    Send an amount of data in non-blocking mode with DMA.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::DMA_Transmit(uint8_t* pData, uint32_t Size)
{
    SystemState_e State = SYS_READY;

    if(m_State == QSPI_STATE_READY)
    {
        m_ErrorCode = SYS_READY;                        // Clear the error code
        m_State     = QSPI_STATE_BUSY_INDIRECT_TX;      // Update state
        MODIFY_REG(QUADSPI->CCR,
                   QUADSPI_CCR_FMODE,
                   QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);
        SET_BIT(QUADSPI->CR,
                (QUADSPI_CR_TEIE |                      // Enable the QSPI transfer error
                 QUADSPI_CR_TCIE));                     // Enable TC Interrupts: Workaround for QSPI low kernel clock frequency
        MODIFY_REG(DMA2_Stream7->CR,
                   DMA_SxCR_DIR_Msk,
                   QSPI_DMA_WRITE_DIRECTION);           // DMA Direction : memory to peripheral
        this->DMA_Start(pData, Size + 1);
        SET_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN);
    }
    else
    {
        State = SYS_BUSY;
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_Receive
//
//  Parameter(s):   pData        Pointer to data buffer.
//                  Size         Size to received.
//  Return Value:   SystemState_e
//
//  Description:    Receive an amount of data in non-blocking mode with DMA.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::DMA_Receive(uint8_t *pData, uint32_t Size)
{
    SystemState_e State;

    if(m_State == QSPI_STATE_READY)
    {
        m_ErrorCode = SYS_READY;                                            // Clear the error code
        m_State     = QSPI_STATE_BUSY_INDIRECT_RX;                          // Update state

        // Note: QSPI need to be configured to indirect mode before starting the DMA to avoid premature triggering for the DMA transfer
        MODIFY_REG(QUADSPI->CCR,                                            // Configure QSPI: CCR register with functional as indirect read
                   QUADSPI_CCR_FMODE,
                   QSPI_FUNCTIONAL_MODE_INDIRECT_READ);
        MODIFY_REG(DMA2_Stream7->CR,                                        // DMA Direction : peripheral to memory
                   DMA_SxCR_DIR_Msk,
                   QSPI_DMA_READ_DIRECTION);

        if((State = this->DMA_Start(pData, Size + 1)) == SYS_READY)
        {
            SET_BIT(QUADSPI->CR, (QUADSPI_CR_TEIE | QUADSPI_CR_TCIE));      // Enable the QSPI transfer error and transmit complete interrupt
            SET_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN);                         // Enable the QSPI DMA
            QUADSPI->AR = QUADSPI->AR;                                      // Start the transfer by re-writing the address in AR register
        }

        return State;
    }

    return SYS_BUSY;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_Start
//
//  Parameter(s):   pData               Data buffer to transfer
//                  BlockDataLength     The length of a block transfer in bytes.
//  Return Value:   SystemState_e
//
//  Description:    Starts the DMA Transfer with interrupts enabled.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::DMA_Start(uint8_t* pData, uint32_t BlockDataLength)
{
    m_DMA_ErrorCode = SYS_READY;                                // Initialize the error code
    DMA2_Stream7->NDTR = BlockDataLength;                       // Configure the DMA Channel data length
    DMA2->HIFCR = (DMA_HIFCR_CTEIF7 | DMA_HIFCR_CTCIF7);        // Clear interrupt flags
    DMA2_Stream7->M0AR = (uint32_t)pData;                       // Configure DMA Channel Source address
    DMA2_Stream7->CR |= DMA_SxCR_EN;                            // Enable the Peripheral

    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_Abort
//
//  Parameter(s):   pData               Data buffer to transfer
//                  BlockDataLength     The length of a block transfer in bytes.
//  Return Value:   SystemState_e
//
//  Description:    Aborts the DMA Transfer.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::DMA_Abort(void)
{
    TickCount_t TickStart =  GetTick();

    DMA2_Stream7->CR &= ~(DMA_SxCR_TEIE |                                   // Disable all the transfer interrupts
                          DMA_SxCR_EN);                                     // Disable the channel

    while((DMA2_Stream7->CR & DMA_SxCR_EN) != 0)                            // Check if the DMA Channel is effectively disabled
    {
        if(TickHasTimeOut(TickStart, QSPI_TIMEOUT_DMA_ABORT) == true)
        {
            m_DMA_ErrorCode = SYS_TIME_OUT;                                 // Update error code
            return SYS_ERROR;
        }
    }

    DMA2->HIFCR = DMA_HIFCR_CTEIF7;                                         // Clear all interrupt flags

    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_Abort_IT
//
//  Parameter(s):   None
//  Return Value:   SystemState_e
//
//  Description:    Aborts the DMA Transfer in Interrupt mode.
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::DMA_Abort_IT(void)
{
    DMA2_Stream7->CR &= ~DMA_SxCR_EN;             // Disable the DMA channel
    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA2_Stream7_IRQHandler
//
//  Parameter(s):   None
//  Return Value:   None
//
//  Description:    handle the DMA IRQ for QSPI.
//
//-------------------------------------------------------------------------------------------------
extern "C" NOS_ISR(DMA2_Stream7_IRQHandler)
{
    uint32_t Count = 0;
    uint32_t TimeOut = SYSTEM_CORE_CLOCK / 9600U;
    uint32_t Flag;

    Flag = DMA2->HISR;

    // Transfer Error Interrupt management
    if((Flag & DMA_HISR_TEIF7) != 0)
    {
        if((DMA2_Stream7->CR & QUADSPI_CR_TEIE) != 0)
        {
            DMA2_Stream7->CR &= ~QUADSPI_CR_TEIE;               // Disable the transfer error interrupt
            QSPI.m_DMA_ErrorCode = SYS_DMA_ERROR;               // Update error code : Read Transfer error
            DMA2->HIFCR = DMA_HIFCR_CTEIF7;                     // Clear the transfer error flags
        }
    }

    if(QSPI.m_DMA_ErrorCode != SYS_READY)                       // Manage error case
    {
        DMA2_Stream7->CR &= ~DMA_SxCR_EN;                       // Disable the channel

        do
        {
            if(++Count > TimeOut)
            {
                break;
            }
        }
        while((DMA2_Stream7->CR & DMA_SxCR_EN) != 0);

        QSPI.DMA_Error();
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_Error
//
//  Parameter(s):   None
//  Return Value:   None
//
//  Description:    DMA QSPI communication error callback.
//
//-------------------------------------------------------------------------------------------------
void QSPI_Driver::DMA_Error(void)
{
    m_ErrorCode = SYS_DMA_ERROR;
    CLEAR_BIT(QUADSPI->CR, QUADSPI_CR_DMAEN);       // Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register
    this->Abort_IT();                               // Abort the QSPI
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       DMA_AbortCplt
//
//  Parameter(s):   None
//  Return Value:   None
//
//  Description:    DMA QSPI abort complete callback.
//
//-------------------------------------------------------------------------------------------------
void QSPI_Driver::DMA_AbortCplt(void)
{
    if(m_State == QSPI_STATE_ABORT)                     // DMA Abort called by QSPI abort
    {
        QUADSPI->FCR = QUADSPI_FCR_CTCF;                // Clear interrupt
        SET_BIT(QUADSPI->CR, QUADSPI_CR_TCEN);          // Enable the QSPI Transfer Complete Interrupt
        SET_BIT(QUADSPI->CR, QUADSPI_CR_ABORT);         // Configure QSPI: CR register with Abort request
    }
    else                                                // DMA Abort called due to a transfer error interrupt
    {
        m_State = QSPI_STATE_READY;                     // Change state of QSPI
        //this->ErrorCallback();                          // Error callback
    }
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_QSPI



































#if 0


//-------------------------------------------------------------------------------------------------
//
//   Function:      MemoryMapped
//
//   Parameter(s):  Command    Structure that contains the command configuration information
//                  Config     Structure that contains the memory mapped configuration information.
//   Return Value:  SystemState_e
//
//   Description:   Configure the Memory Mapped mode.
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::MemoryMapped(QSPI_CommandTypeDef* Command, QSPI_MemoryMappedTypeDef* Config)
{
    SystemState_e State;

    State = this->WaitReady(QSPI_FLAG_BUSY, 0, 100);   // TODO found the right timeout

    if(State == SYS_READY)
    {
        // Configure QSPI: CR register with time out counter enable
        QUADSPI->CR &= ~(uint32_t)QuadSPI_CR_TCEN;
        QUADSPI->CR = Config->TimeOutActivation);

        if(Config->TimeOutActivation == QSPI_TIMEOUT_COUNTER_ENABLE)
        {
            // Configure QSPI: LPTR register with the low-power time out value
            QUADSPI->LPTR = Config->TimeOutPeriod;

            // Enable the QSPI TimeOut Interrupt
            QUADSPI->CR |= QSPI_IT_TO;
        }

        // Call the configuration function
        this->Config(Command, QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED);
    }
    else
    {
        State = SYS_BUSY;
    }

    return State;
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      Config
//
//   Parameter(s):  Command         Structure that contains the command configuration information
//                  FunctionalMode  Functional mode to configured
//   Return Value:  None
//
//   Description:   This function configures the communication registers
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void QSPI_Driver::Config(QSPI_CommandTypeDef *Command, uint32_t FunctionalMode)
{
    if((Command->DataMode != QSPI_DATA_NONE) && (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED))
    {
        // Configure QSPI: DLR register with the number of data to read or write
        QUADSPI->DLR, (Command->NbData - 1);
    }

    if(Command->InstructionMode != QSPI_INSTRUCTION_NONE)
    {
        if(Command->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)
        {
            // Configure QSPI: ABR register with alternate bytes value
            QUADSPI->ABR, Command->AlternateBytes);

            if(Command->AddressMode != QSPI_ADDRESS_NONE)
            {
                //---- Command with instruction, address and alternate bytes ----
                // Configure QSPI: CCR register with all communications parameters
                QUADSPI->CCR, (Command->DdrMode           | Command->DdrHoldHalfCycle    | Command->SIOOMode           |
                               Command->DataMode          | (Command->DummyCycles << 18) | Command->AlternateBytesSize |
                               Command->AlternateByteMode | Command->AddressSize         | Command->AddressMode        |
                               Command->InstructionMode   | Command->Instruction         | FunctionalMode));

                if(FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)
                {
                    // Configure QSPI: AR register with address value
                    QUADSPI->AR, Command->Address;
                }
            }
            else
            {
                //---- Command with instruction and alternate bytes ----
                // Configure QSPI: CCR register with all communications parameters
                QUADSPI->CCR, (Command->DdrMode           | Command->DdrHoldHalfCycle    | Command->SIOOMode           |
                               Command->DataMode          | (Command->DummyCycles << 18) | Command->AlternateBytesSize |
                               Command->AlternateByteMode | Command->AddressMode         | Command->InstructionMode    |
                               Command->Instruction       | FunctionalMode));
            }
        }
        else
        {
            if(Command->AddressMode != QSPI_ADDRESS_NONE)
            {
                //---- Command with instruction and address ----
                // Configure QSPI: CCR register with all communications parameters
                QUADSPI->CCR, (Command->DdrMode     | Command->DdrHoldHalfCycle    | Command->SIOOMode          |
                               Command->DataMode    | (Command->DummyCycles << 18) | Command->AlternateByteMode |
                               Command->AddressSize | Command->AddressMode         | Command->InstructionMode   |
                               Command->Instruction | FunctionalMode));

                if(FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)
                {
                    // Configure QSPI: AR register with address value
                    QUADSPI->AR, Command->Address;
                }
            }
            else
            {
                //---- Command with only instruction ----
                // Configure QSPI: CCR register with all communications parameters
                QUADSPI->CCR, (Command->DdrMode     | Command->DdrHoldHalfCycle    | Command->SIOOMode          |
                               Command->DataMode    | (Command->DummyCycles << 18) | Command->AlternateByteMode |
                               Command->AddressMode | Command->InstructionMode     | Command->Instruction       |
                               FunctionalMode));
            }
        }
    }
    else
    {
        if(Command->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)
        {
            // Configure QSPI: ABR register with alternate bytes value
            QUADSPI->ABR, Command->AlternateBytes;

            if(Command->AddressMode != QSPI_ADDRESS_NONE)
            {
                //---- Command with address and alternate bytes ----
                // Configure QSPI: CCR register with all communications parameters
                QUADSPI->CCR, (Command->DdrMode           | Command->DdrHoldHalfCycle    | Command->SIOOMode           |
                               Command->DataMode          | (Command->DummyCycles << 18) | Command->AlternateBytesSize |
                               Command->AlternateByteMode | Command->AddressSize         | Command->AddressMode        |
                               Command->InstructionMode   | FunctionalMode));

                if(FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)
                {
                    // Configure QSPI: AR register with address value
                    QUADSPI->AR, Command->Address;
                }
            }
            else
            {
                //---- Command with only alternate bytes ----
                // Configure QSPI: CCR register with all communications parameters
                QUADSPI->CCR, (Command->DdrMode           | Command->DdrHoldHalfCycle    | Command->SIOOMode           |
                               Command->DataMode          | (Command->DummyCycles << 18) | Command->AlternateBytesSize |
                               Command->AlternateByteMode | Command->AddressMode         | Command->InstructionMode    |
                               FunctionalMode));
            }
        }
        else
        {
            if(Command->AddressMode != QSPI_ADDRESS_NONE)
            {
                //---- Command with only address ----
                // Configure QSPI: CCR register with all communications parameters
                QUADSPI->CCR, (Command->DdrMode     | Command->DdrHoldHalfCycle    | Command->SIOOMode          |
                               Command->DataMode    | (Command->DummyCycles << 18) | Command->AlternateByteMode |
                               Command->AddressSize | Command->AddressMode         | Command->InstructionMode   |
                               FunctionalMode));

                if(FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)
                {
                    // Configure QSPI: AR register with address value
                    QUADSPI->AR, Command->Address;
                }
            }
            else
            {
                //---- Command with only data phase ----
                if(Command->DataMode != QSPI_DATA_NONE)
                {
                    // Configure QSPI: CCR register with all communications parameters
                    QUADSPI->CCR, (Command->DdrMode     | Command->DdrHoldHalfCycle    | Command->SIOOMode          |
                                   Command->DataMode    | (Command->DummyCycles << 18) | Command->AlternateByteMode |
                                   Command->AddressMode | Command->InstructionMode     | FunctionalMode));
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Command
//
//   Parameter(s):  Command     Structure that contains the command configuration information
//                  Timeout     Time out duration
//
//   Return Value:  SystemState_e
//
//   Description:   This function is used only in Indirect Read or Write Modes
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::Command(QSPI_CommandTypeDef* Command, uint32_t Timeout)
{
    SystemState_e State;

    // Wait till BUSY flag reset
    State = this->WaitReady(QSPI_FLAG_BUSY, 0, Timeout)

    if(State == SYS_READY)
    {
        // Call the configuration function
        this->Config(Command, QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);

        if(Command->DataMode == QSPI_DATA_NONE)
        {
            // When there is no data phase, the transfer start as soon as the configuration is done
            // so wait until TC flag is set to go back in idle state
            if(this->WaitReady(QSPI_FLAG_TC, 1, Timeout) != SYS_READY)
            {
                State = SYS_TIME_OUT;
            }
            else
            {
                QUADSPI->FCR = QSPI_FLAG_TC;
            }
        }
    }

  return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Receive
//
//   Parameter(s):  pData       Pointer to data buffer.
//
//   Return Value:  SystemState_e
//
//   Description:   Receives an amount of data in non blocking mode with DMA.
//
//   Note(s):       This function is used only in Indirect Read Mode
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::Receive(uint8_t *pData)
{
    SystemState_e State;
    uint32_t *tmp;
    //uint32_t addr_reg = READ_REG(QUADSPI->AR);

    if(pData != nullptr)
    {
        //remove that crap!!
        // Configure counters and size of the handle
        hqspi->RxXferCount = READ_REG(hqspi->Instance->DLR) + 1;
        hqspi->RxXferSize = READ_REG(hqspi->Instance->DLR) + 1;
        hqspi->pRxBuffPtr = pData;
        //hqspi->hdma->XferCpltCallback = QSPI_DMARxCplt;         // Set the QSPI DMA transfer complete callback
        //hqspi->hdma->XferHalfCpltCallback = QSPI_DMARxHalfCplt; // Set the QSPI DMA Half transfer complete callback
        //hqspi->hdma->XferErrorCallback = QSPI_DMAError;         // Set the DMA error callback

        m_pDMA->CR &= ~(uint32_t)DMA_SxCR_DIR;
        m_pDMA->CR |=  DMA_PERIPH_TO_MEMORY;                   // Configure the direction of the DMA

        tmp = (uint32_t*)&pData;        // Enable the DMA Channel
        DMA_Start_IT(hqspi->hdma, (uint32_t)&QUADSPI->DR, *(uint32_t*)tmp, hqspi->RxXferSize);

        QUADSPI->CCR &= ~(uint32_t)QuadSPI_CCR_FMODE;
        QUADSPI->CCR |=  QSPI_FUNCTIONAL_MODE_INDIRECT_READ;    // Configure QSPI: CCR register with functional as indirect read
        // probably it wont work!!
        QUADSPI->AR = QUADSPI->AR;                              // Start the transfer by re-writing the address in AR register
        QUADSPI->CR = QuadSPI_CR_DMAEN;                         // Enable the DMA transfer by setting the DMAEN bit in the QSPI CR register
    }
    else
    {
        State = SYS_ERROR;
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Transmit
//
//   Parameter(s):  pData       Pointer to data buffer.
//
//   Return Value:  SystemState_e
//
//   Description:   Sends an amount of data in non blocking mode with DMA.
//
//   Note(s):       This function is used only in Indirect Read Mode
//
//-------------------------------------------------------------------------------------------------
SystemState_e QSPI_Driver::Transmit(uint8_t* pData)
{
  SystemState_e State;
  uint32_t *tmp;

    if(pData != nullptr)
    {
        // Configure counters and size of the handle
        hqspi->TxXferCount = READ_REG(hqspi->Instance->DLR) + 1;
        hqspi->TxXferSize = READ_REG(hqspi->Instance->DLR) + 1;
        hqspi->pTxBuffPtr = pData;

        // Configure QSPI: CCR register with functional mode as indirect write
        QUADSPI->CCR &= ~(uint32_t)QuadSPI_CCR_FMODE;
        QUADSPI->CCR |=  QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE;

        //hqspi->hdma->XferCpltCallback = QSPI_DMATxCplt;           // Set the QSPI DMA transfer complete callback
        //hqspi->hdma->XferHalfCpltCallback = QSPI_DMATxHalfCplt;   // Set the QSPI DMA Half transfer complete callback
        //hqspi->hdma->XferErrorCallback = QSPI_DMAError;           // Set the DMA error callback

        m_pDMA->CR &= ~(uint32_t)DMA_SxCR_DIR;
        m_pDMA->CR |=  DMA_MEMORY_TO_PERIPH;                      // Configure the direction of the DMA

        // Enable the QSPI transmit DMA Channel
        tmp = (uint32_t*)&pData;
        DMA_Start_IT(hqspi->hdma, *(uint32_t*)tmp, (uint32_t)&hqspi->Instance->DR, hqspi->TxXferSize);

        // Enable the DMA transfer by setting the DMAEN bit in the QSPI CR register
        QUADSPI->CR = QuadSPI_CR_DMAEN;
    }
    else
    {
      State = SYS_OK;
    }

    return State;
}

#if 0
//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:    IRQHandler
//
//  Description:    This function handles SPIx interrupt request.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void QSPI_Driver::IRQHandler(void)
{
    if(m_pPort->CallBackISR != nullptr)
    {
        m_pPort->CallBackISR();
    }
}
#endif

#endif
