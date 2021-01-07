//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_sdio.cpp
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
//
// Notes:
//
//          SD_Detect-Pin
//          SDIO_D0  = SD-Card DAT0
//          SDIO_D1  = SD-Card DAT1
//          SDIO_D2  = SD-Card DAT2
//          SDIO_D3  = SD-Card DAT3/CD
//          SDIO_CK  = SD-Card Clock
//          SDIO_CMD = SD-Card CMD
//
//          DMA : either  DMA2_STREAM3_CHANNEL4
//                   or   DMA2_STREAM6_CHANNEL4
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_SDIO
#include <stdint.h>
#define STM32F7_SDIO_GLOBAL
#include "lib_class_sdio.h"
#undef  STM32F7_SDIO_GLOBAL
#include "lib_dma.h"
#include "nOS.h"
#include "stm32f7xx_ll_sdmmc.h"
#include "string.h"
#include "lib_io.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define SD_DATA_TIMEOUT                 ((uint32_t)0xFFFFFFFF)
#define SD_SOFTWARE_COMMAND_TIMEOUT     ((uint32_t)0x00010000)

// stuff to change
#define SD_CARD_RETRY_TIMER             (1000 / SD_CARD_TICK_PERIOD)
#define SD_CARD_TICK_PERIOD             1


#define ClearAllFlag()                  { SDMMC1->ICR = (SDMMC_FLAG_CCRCFAIL | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_CTIMEOUT | \
                                                         SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_TXUNDERR | SDMMC_FLAG_RXOVERR  | \
                                                         SDMMC_FLAG_CMDREND  | SDMMC_FLAG_CMDSENT  | SDMMC_FLAG_DATAEND  | \
                                                         SDMMC_FLAG_DBCKEND); }

// ----- MMC/SDC command -----
#define ACMD                            0x80
#define CMD0                            (0)                             // GO_IDLE_STATE
#define CMD1                            (1)                             // SEND_OP_COND (MMC)
#define CMD2                            (2)                             // ALL_SEND_CID
#define CMD3                            (3)                             // SEND_RELATIVE_ADDR
#define CMD6                            (6)                             // SET_BUS_WIDTH (SDC)
#define CMD7                            (7)                             // SELECT_CARD
#define CMD8                            (8)                             // SEND_IF_COND
#define CMD9                            (9)                             // SEND_CSD
#define CMD10                           (10)                            // SEND_CID
#define CMD12                           (12)                            // STOP_TRANSMISSION
#define CMD13                           (13)                            // SEND_STATUS
// #define ACMD13                       (55)(13)                        // SD_STATUS (SDC)
#define CMD16                           (16)                            // SET_BLOCKLEN
#define CMD17                           (17)                            // READ_SINGLE_BLOCK
#define CMD18                           (18)                            // READ_MULTIPLE_BLOCK
#define CMD23                           (23)                            // SET_BLK_COUNT (MMC)
//#define ACMD23                        (55)(23)                        // SET_WR_BLK_ERASE_COUNT (SDC)
#define CMD24                           (24)                            // WRITE_BLOCK
#define CMD25                           (25)                            // WRITE_MULTIPLE_BLOCK
#define CMD32                           (32)                            // ERASE_ER_BLK_START
#define CMD33                           (33)                            // ERASE_ER_BLK_END
#define CMD38                           (38)                            // ERASE
#define CMD41                           (41)                            // SEND_OP_COND (SDC)
//#define ACMD41                        (55)(41)                        // SEND_OP_COND (SDC)
#define CMD51                           (51)                            // SEND_SCR
//#define ACMD51                        (55)(51)                        // SEND_SCR
#define CMD55                           (55)                            // APP_CMD


#define SD_OCR_ADDR_OUT_OF_RANGE        ((uint32_t)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((uint32_t)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((uint32_t)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((uint32_t)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((uint32_t)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((uint32_t)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((uint32_t)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((uint32_t)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((uint32_t)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((uint32_t)0x00200000)
#define SD_OCR_CC_ERROR                 ((uint32_t)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((uint32_t)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((uint32_t)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((uint32_t)0x00020000)
#define SD_OCR_CID_CSD_OVERWRITE        ((uint32_t)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((uint32_t)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((uint32_t)0x00004000)
#define SD_OCR_ERASE_RESET              ((uint32_t)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((uint32_t)0x00000008)
#define SD_OCR_ERRORBITS                ((uint32_t)0xFDFFE008)
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((uint32_t)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((uint32_t)0x00008000)


#define SD_SHORT_RESPONSE               SDMMC_CMD_WAITRESP_0
#define SD_LONG_RESPONSE                SDMMC_CMD_WAITRESP


#define SD_BUS_WIDE_1B                  ((uint32_t)0x00000000)
#define SD_BUS_WIDE_4B                  SDMMC_CLKCR_WIDBUS_0
#define SD_BUS_WIDE_8B                  SDMMC_CLKCR_WIDBUS_1



#define SD_DETECT_GPIO_PORT             GPIOC
#define SD_DETECT_GPIO_CLOCK            RCC_AHB1ENR_GPIOCEN
#define SD_DETECT_PIN             		GPIO_PIN_MASK_13

#define SD_WIDE_BUS_SUPPORT             ((uint32_t)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((uint32_t)0x00010000)
#define SD_CARD_LOCKED                  ((uint32_t)0x02000000)
#define SD_VOLTAGE_WINDOW               ((uint32_t)0x80100000)
#define SD_RESP_HIGH_CAPACITY           ((uint32_t)0x40000000)
#define SD_RESP_STD_CAPACITY            ((uint32_t)0x00000000)
#define SD_MAX_VOLT_TRIAL               ((uint32_t)0x0000FFFF)
#define SD_ALLZERO                      ((uint32_t)0x00000000)
#define SD_CHECK_PATTERN                ((uint32_t)0x000001AA)


#define SD_DATABLOCK_SIZE_8B            (SDMMC_DCTRL_DBLOCKSIZE_0|SDMMC_DCTRL_DBLOCKSIZE_1)
#define SD_DATABLOCK_SIZE_64B           (SDMMC_DCTRL_DBLOCKSIZE_1|SDMMC_DCTRL_DBLOCKSIZE_2)
#define SD_DATABLOCK_SIZE_512B          (SDMMC_DCTRL_DBLOCKSIZE_0|SDMMC_DCTRL_DBLOCKSIZE_3)

#define BLOCK_SIZE                      512
//#define DMA_CHANNEL_4                   ((uint32_t)0x08000000)                // already define
//#define DMA_MEMORY_TO_PERIPH            ((uint32_t)DMA_SxCR_DIR_0)
//#define DMA_MINC_ENABLE                 ((uint32_t)DMA_SxCR_MINC)
//#define DMA_MDATAALIGN_WORD             ((uint32_t)DMA_SxCR_MSIZE_1)
//#define DMA_PDATAALIGN_WORD             ((uint32_t)DMA_SxCR_PSIZE_1)
//#define DMA_PRIORITY_VERY_HIGH          ((uint32_t)DMA_SxCR_PL)
//#define DMA_MBURST_INC4                 ((uint32_t)DMA_SxCR_MBURST_0)
//#define DMA_PBURST_INC4                 ((uint32_t)DMA_SxCR_PBURST_0)

#define IFCR_CLEAR_MASK_STREAM3         (DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3)
#define IFCR_CLEAR_MASK_STREAM6         (DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CFEIF6)

//-------------------------------------------------------------------------------------------------
// typedef(s)
//-------------------------------------------------------------------------------------------------

enum SD_CardState_t
{
    SD_CARD_READY                  = ((uint32_t)0x00000001),  // Card state is ready
    SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),  // Card is in identification state
    SD_CARD_STANDBY                = ((uint32_t)0x00000003),  // Card is in standby state
    SD_CARD_TRANSFER               = ((uint32_t)0x00000004),  // Card is in transfer state
    SD_CARD_SENDING                = ((uint32_t)0x00000005),  // Card is sending an operation
    SD_CARD_RECEIVING              = ((uint32_t)0x00000006),  // Card is receiving operation information
    SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),  // Card is in programming state
    SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),  // Card is disconnected
    SD_CARD_ERROR                  = ((uint32_t)0x000000FF)   // Card is in error state
};

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   SDIO_Driver
//
//   Parameter(s):  None
//
//   Description:   Initializes the IO and the peripheral
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SDIO_Driver::SDIO_Driver()
{
    uint32_t PriorityGroup;

    nOS_MutexCreate(&m_Mutex, NOS_MUTEX_RECURSIVE, NOS_MUTEX_PRIO_INHERIT);


    // Reset SDIO Module
    RCC->APB2RSTR |=  RCC_APB2RSTR_SDMMC1RST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_SDMMC1RST;

    // Enable SDIO clock
    RCC->APB2ENR |= RCC_APB2ENR_SDMMC1EN;

    // Enable DMA2 clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;


    /// ---- GPIOs Configuration ----
    for(uint32_t IO_Id = uint32_t(IO_SD_D0); IO_Id <= uint32_t(IO_SD_CMD); IO_Id++)
    {
        IO_PinInit(IO_ID_e(IO_Id));
    }

  #if SD_CARD_USE_DETECT_SIGNAL == 1
    IO_PinInit(IO_DETECT_SD_CARD);
  #endif

  #if SD_CARD_USE_POWER_CONTROL == 1
    IO_OutputInit(IO_POWER_SD_CARD);
  #endif


    // Initialize DMA2 channel 3 for RX from SD CARD
    DMA2_Stream3->CR   = 0;                                                 // Reset DMA Stream control register
    DMA2_Stream3->PAR  = (uint32_t)&SDMMC1->FIFO;
    DMA2->LIFCR        = IFCR_CLEAR_MASK_STREAM3;                           // Clear all interrupt flags
    DMA2_Stream3->CR   = (DMA_CHANNEL_4         | DMA_SxCR_PFCTRL        |  // Prepare the DMA Stream configuration
                          DMA_MEMORY_INCREMENT  | DMA_P_DATA_ALIGN_WORD  |  // And write to DMA Stream CR register
                          DMA_M_DATA_ALIGN_WORD | DMA_PRIORITY_VERY_HIGH |
                          DMA_M_BURST_INC4      | DMA_P_BURST_INC4);

    DMA2_Stream3->FCR  = (DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);                 // Configuration FIFO control register

    // Initialize DMA2 channel 6 for TX to SD CARD
    DMA2_Stream6->CR   = 0;                                                 // Reset DMA Stream control register
    DMA2_Stream6->PAR  = (uint32_t)&SDMMC1->FIFO;
    DMA2->HIFCR        = IFCR_CLEAR_MASK_STREAM6;                           // Clear all interrupt flags
    DMA2_Stream6->CR   = (DMA_CHANNEL_4         | DMA_SxCR_PFCTRL        |  // Prepare the DMA Stream configuration
                          DMA_MEMORY_INCREMENT  | DMA_P_DATA_ALIGN_WORD  |  // And write to DMA Stream CR register
                          DMA_M_DATA_ALIGN_WORD | DMA_PRIORITY_VERY_HIGH |
                          DMA_M_BURST_INC4      | DMA_P_BURST_INC4       |
                          DMA_MEMORY_TO_PERIPH);
    DMA2_Stream6->FCR  = (DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);                 // Configuration FIFO control register

    m_CardStatus       = STA_NOINIT;
    m_TransferError    = SYS_READY;
    m_DMA_XferComplete = false;
    m_TransferComplete = false;

    //m_SendStopTransfer = false;
    // m_TickPeriod   = SD_CARD_TICK_PERIOD;

    PriorityGroup = NVIC_GetPriorityGrouping();

    // NVIC configuration for SDIO interrupts
    NVIC_SetPriority(SDMMC1_IRQn, NVIC_EncodePriority(PriorityGroup, 5, 0));
    NVIC_EnableIRQ(SDMMC1_IRQn);

    // NVIC configuration for DMA transfer complete interrupt
    NVIC_SetPriority(DMA2_Stream3_IRQn, NVIC_EncodePriority(PriorityGroup, 6, 0));
    NVIC_EnableIRQ(DMA2_Stream3_IRQn);

    NVIC_SetPriority(DMA2_Stream6_IRQn, NVIC_EncodePriority(PriorityGroup, 6, 0));
    NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: GetStatus
//
//   Parameter(s):  uint32_t*       pResponse
//   Return value:  SystemState_e
//
//   Description:   Get response from SD device
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::GetStatus(void)
{
    SystemState_e  State;
    uint32_t       Response1;
    SD_CardState_t CardState;

    if((State = this->IsDetected()) == SYS_READY)
    {
        if((State = this->TransmitCommand((CMD13 | SD_SHORT_RESPONSE), m_RCA, 1)) == SYS_READY)
        {
            Response1 = SDMMC1->RESP1;
            CardState = (SD_CardState_t)((Response1 >> 9) & 0x0F);

            // Find SD status according to card state
            if     (CardState == SD_CARD_TRANSFER)  State = SYS_READY;
            else if(CardState == SD_CARD_ERROR)     State = SYS_DEVICE_ERROR;
            else                                    State = SYS_DEVICE_BUSY;
        }
        else
        {
            State = SYS_DEVICE_ERROR;
        }
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: FatFS_Status
//
//   Parameter(s):  None
//   Return value:  DSTATUS
//
//   Description:   Get Status from SD Card
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DSTATUS SDIO_Driver::FatFS_DiskStatus(void)
{
	// Check SDCARD status
	m_CardStatus = (this->GetStatus() == SYS_READY) ? SYS_READY : STA_NOINIT;
	return (DSTATUS)m_CardStatus;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: FatFS_Initialize
//
//   Parameter(s):  None
//   Return value:  DSTATUS
//
//   Description:   Initialize SD Card
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DSTATUS SDIO_Driver::FatFS_Initialize(void)
{
    SystemState_e  State;

    this->PowerOFF();

	// Check if SD card is present
	if(this->IsDetected() == SYS_DEVICE_NOT_PRESENT)
    {
        return STA_NOINIT;
    }

    // Initialize SDMMC peripheral interface with default configuration for SD card initialization
    SDMMC1->CLKCR &= ~(uint32_t)CLKCR_CLEAR_MASK;
    SDMMC1->CLKCR |=  (uint32_t)SDMMC_INIT_CLK_DIV;

    if((State = this->PowerON()) == SYS_READY)                                              // Identify card operating voltage
    {
        if((State = this->InitializeCard()) == SYS_READY)                                   // Initialize the present card and put them in idle state
        {
            if((State = this->GetCardInfo()) == SYS_READY)                                  // Read CSD/CID MSD registers
            {
                State = this->TransmitCommand((CMD7 | SD_SHORT_RESPONSE), m_RCA, 1);        // Select the Card
                SDMMC1->CLKCR &= ~(uint32_t)CLKCR_CLEAR_MASK;                               // Configure SDMMC peripheral interface
            }
        }
    }

	// Configure SD Bus width
	if(State == SYS_READY)
	{
		// Enable wide operation
		State = this->SetBusWidth(SD_BUS_WIDE_4B);
	}

	// Configure the SDCARD device
	m_CardStatus = (State == SYS_READY) ? SYS_READY : STA_NOINIT;
	return (DSTATUS)m_CardStatus;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:       FatFS_Read
//
//   Parameter(s):  uint8_t*  pBuffer
//                  uint32_t  Sector
//                  uint8_t   NumberOfBlocks
//   Return value:  DRESULT
//
//   Description:   Read From SD Card
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DRESULT SDIO_Driver::FatFS_Read(uint8_t* pBuffer, uint32_t Sector, uint8_t NumberOfBlocks)
{
	// Prepare the DMA Transfer
    this->StartBlockTransfert(DMA2_Stream3, (uint32_t *)pBuffer, BLOCK_SIZE, NumberOfBlocks);

    // Read block(s) in DMA transfer mode
    if(this->ReadBlocks((uint64_t)(Sector * BLOCK_SIZE), BLOCK_SIZE, NumberOfBlocks) == SYS_READY)
    {
        // Wait until transfer is complete
        if(this->CheckOperation(SDMMC_STA_RXACT) == SYS_READY)
        {
            return RES_OK;
        }
    }

	return RES_ERROR;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: FatFS_Write
//
//   Parameter(s):  const uint8_t*  pBuffer
//                  uint32_t        Sector
//                  uint8_t         NumberOfBlocks
//   Return value:  DRESULT
//
//   Description:   Write to the SD Card
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DRESULT SDIO_Driver::FatFS_Write(const uint8_t* pBuffer, uint32_t Sector, uint8_t NumberOfBlocks)
{
    // Prepare the DMA Transfer
    this->StartBlockTransfert(DMA2_Stream6, (uint32_t *)pBuffer, BLOCK_SIZE, NumberOfBlocks);

    // Write block(s) in DMA transfer mode
    if(this->WriteBlocks((uint64_t)(Sector * BLOCK_SIZE), BLOCK_SIZE, NumberOfBlocks) == SYS_READY)
    {
        // Wait until transfer is complete
        if(this->CheckOperation(SDMMC_STA_TXACT) == SYS_READY)
        {
            return RES_OK;
        }
    }

	return RES_ERROR;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: FatFS_IO_Control
//
//   Parameter(s):  uint8_t    Control		Control code
//	                void*      pBuffer		Buffer to send/receive control data
//   Return value:  DRESULT
//
//   Description:   Control
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
#if _USE_IOCTL == 1
DRESULT SDIO_Driver::FatFS_IO_Control(uint8_t Control, void *pBuffer)
{
    DRESULT     Result;
//  uint8_t     b;
    //uint8_t*    pPtr = (uint8_t*)pBuffer;
    //uint32_t    Data;
    //uint32_t*   dp;
    //uint32_t    st;
    //uint32_t    ed;


    if(m_CardStatus == STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    Result = RES_ERROR;

    switch(Control)
    {
        // Make sure that all data has been written on the media
        case CTRL_SYNC:
        {
            Result = RES_OK;
            break;
        }

        // Get number of sectors on the disk (uint32_t)
        case GET_SECTOR_COUNT:
        {
			this->GetCardInfo();
			*(DWORD *)pBuffer = m_CardCapacity / BLOCK_SIZE;
			Result = RES_OK;
            break;
        }

        // Get sectors on the disk (uint16_t)
        case GET_SECTOR_SIZE:
        {
			*(WORD *)pBuffer = BLOCK_SIZE;
			Result = RES_OK;
            break;
        }

        // Get erase block size in unit of sectors (uint32_t)
        case GET_BLOCK_SIZE:
        {
			*(DWORD*)pBuffer = BLOCK_SIZE;
            break;
        }

/*        // Erase a block of sectors
        case CTRL_ERASE_SECTOR:
        {

            if(!(m_CardType & HIGH_CAPACITY_SD_CARD) || (((m_CardCSD[0].u_8.u0 >> 6) == 0) && ((m_CardCSD[2].u_8.u2 & 0x40) == 0)))
            {
                break;                                                              // Check if sector erase can be applied to the card
            }

            dp = (uint32_t*)pBuffer;
            st = dp[0];
            ed = dp[1];

/SDIO_Driver/ TODO need to check this
        //    if(!(m_CardType & CT_BLOCK))
        //    {
        //       st *= 512;
        //       ed *= 512;
        //    }

            if((this->SendCommand((CMD32 | SD_SHORT_RESPONSE), st, R1_RESPONSE, &m_Response[0]) == SYS_READY) &&
               (this->SendCommand((CMD33 | SD_SHORT_RESPONSE), ed, R1_RESPONSE, &m_Response[0]) == SYS_READY) &&
               (this->SendCommand((CMD38 | SD_SHORT_RESPONSE), 0,  R1_RESPONSE, &m_Response[0]) == SYS_READY) &&
               (WaitReady(30000) == SYS_READY))
            {
                Result = RES_OK;
            }
            break;
        }

        // Get card type flags (1 byte)
        case MMC_GET_TYPE:
        {
            *pPtr = m_CardType;
            Result = RES_OK;
            break;
        }

        case GET_CSD:                                                               // Get CSD (16 bytes)
        {
            memcpy(pPtr, m_CardCSD, 16);
            Result = RES_OK;
            break;
        }

        case MMC_GET_CID:                                                           // Get CID (16 bytes)
        {
            memcpy(pPtr, m_CardCID, 16);
            Result = RES_OK;
            break;
        }

        case MMC_GET_OCR:                                                           // Get OCR (4 bytes)
        {
            memcpy(pPtr, &m_OCR, 4);
            Result = RES_OK;
            break;
        }

        // Receive SD status as a data block (64 bytes)

        case MMC_GET_SDSTAT:
            if(m_CardType & CT_SDC)                                                 // SDC
            {
                if(WaitReady(500))
                {

                    m_pDriver->ReadyReception(1, 64);                                          // Ready to receive data blocks
                    if((This->SendCommand((ACMD13 | SD_SHORT_RESPONSE), 0, R1_RESPONSE, &m_Response[0]) == true) &&
                       (m_Response[0] & 0xC0580000) == 0)                           // Start to read
                    {
                        while((MCI_dwXferWp == 0) && ((MCI_byXferStat & 0xC) == 0) && (m_Detect == DEVICE_PRESENT));

                        if((MCI_byXferStat & 0xC) == 0)
                        {
                            LIB_memcpy(pBuffer, DmaBuff[0], 64);                   // Copy_al2un(buff, DmaBuff[0], 64);
                            Result = RES_OK;
                        }
                    }

                }
                //StopTransfert();                                                // Close data path
            }
            break;
*/
        default:
            Result = RES_PARERR;
    }

    return Result;
}
#endif

//-------------------------------------------------------------------------------------------------
//
//   Function name: FATFS_GetDriveSize
//
//   Parameter(s):  char*           pDriveName
//   Return value:  FATFS_Size_t*   SizeStruct
//   Return value:  FRESULT
//
//   Description:   Get response from SD device
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
FRESULT SDIO_Driver::FatFS_GetDriveSize(char* pDriveName, FatFS_Size_t* SizeStruct)
{
	FATFS*  pFS;
    DWORD   FreeCluster;
	FRESULT Result;

    // Get volume information and free clusters of drive
    if((Result = f_getfree(pDriveName, &FreeCluster, &pFS)) == FR_OK)
    {
        // Get total sectors and free sectors
        SizeStruct->Total = (pFS->n_fatent - 2) * pFS->csize * 0.5;
        SizeStruct->Free = FreeCluster * pFS->csize * 0.5;
    }

    return Result;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: GetResponse
//
//   Parameter(s):  uint32_t*       pResponse
//   Return value:  SystemState_e
//
//   Description:   Get response from SD device
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::GetResponse(uint32_t* pResponse)
{
    pResponse[0] = SDMMC1->RESP1;
    pResponse[1] = SDMMC1->RESP2;
    pResponse[2] = SDMMC1->RESP3;
    pResponse[3] = SDMMC1->RESP4;

    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: TickHook
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Call by the high level SD Card class
//
//   Note(s):       Do not put in OS tick hook
//
//-------------------------------------------------------------------------------------------------
void SDIO_Driver::TickHook(void)
{
    m_TickPeriod--;
    if(m_TickPeriod == 0)
    {
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       SetBusWidth
//
//  Parameter(s):   uint32_t        WideMode
//  Return:         SystemState_e   State
//
//  Description:    Set the bus width of the SDMMC module
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::SetBusWidth(uint32_t WideMode)
{
    SystemState_e State;
    uint32_t      Temp;
    uint32_t      SCR[2] = {0, 0};

    // MMC Card does not support this feature
    if(m_CardType == SD_MULTIMEDIA)
    {
        State = SYS_UNSUPPORTED_FEATURE;
    }
    else if((m_CardType == SD_STD_CAPACITY_V1_1) || (m_CardType == SD_STD_CAPACITY_V2_0) ||\
            (m_CardType == SD_HIGH_CAPACITY))
    {
        if(WideMode == SD_BUS_WIDE_8B)
        {
            State = SYS_UNSUPPORTED_FEATURE;
        }
        else if((WideMode == SD_BUS_WIDE_4B) ||
                (WideMode == SD_BUS_WIDE_1B))
        {
            if((SDMMC1->RESP1 & SD_CARD_LOCKED) != SD_CARD_LOCKED)
            {
                // Get SCR Register
                if((State = this->FindSCR(SCR)) == SYS_READY)
                {
                    Temp = (WideMode == SD_BUS_WIDE_4B) ? SD_WIDE_BUS_SUPPORT : SD_SINGLE_BUS_SUPPORT;

                    // If requested card supports wide bus operation
                    if((SCR[1] & Temp) != SD_ALLZERO)
                    {
                        // Send CMD55 APP_CMD with argument as card's RCA.
                        if((State = this->TransmitCommand((CMD55 | SD_SHORT_RESPONSE), m_RCA, 1)) == SYS_READY)
                        {
                            Temp = (WideMode == SD_BUS_WIDE_4B) ? 2 : 0;

                            // Send ACMD6 APP_CMD with argument as 2 for wide bus mode
                            State =  this->TransmitCommand((CMD6 | SD_SHORT_RESPONSE), Temp, 1);
                        }
                    }
                    else
                    {
                        State = SYS_REQUEST_NOT_APPLICABLE;
                    }
                }
            }
            else
            {
                State = SYS_LOCK_UNLOCK_FAILED;
            }
        }
        else
        {
            State = SYS_INVALID_PARAMETER;  // WideMode is not a valid argument
        }

        if(State == SYS_READY)
        {
            // Configure the SDMMC peripheral
            SDMMC1->CLKCR &= ~(uint32_t)CLKCR_CLEAR_MASK;
            SDMMC1->CLKCR |=  (uint32_t)WideMode;
        }
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: TransmitCommand
//
//   Parameter(s):  uint8_t         Command
//	                uint32_t        Argument
//	                ResponseType_e  ResponseType
//   Return value:  SystemState_e
//
//   Description:   Send command to the SD Card
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::TransmitCommand(uint8_t Command, uint32_t Argument, int32_t ResponseType)
{
    SystemState_e   State;

    ClearAllFlag();                                                     // Clear the Command Flags
    SDMMC1->ARG = (uint32_t)Argument;                                   // Set the SDMMC Argument value
    SDMMC1->CMD = (uint32_t)(Command | SDMMC_CMD_CPSMEN);               // Set SDMMC command parameters
    m_LastCommand = Command;
    if((Argument == 0) && (ResponseType == 0)) ResponseType = -1;       // Go idle command
    State = this->CmdResponse(Command & SDMMC_CMD_CMDINDEX, ResponseType);
    ClearAllFlag();                                                     // Clear the Command Flags

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: CheckOCR_Response
//
//   Parameter(s):  uint32_t        Response_R1      OCR Response code
//   Return value:  SystemState_e
//
//   Description:   Analyze the OCR response and return the appropriate error code
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::CheckOCR_Response(uint32_t Response_R1)
{
    if((Response_R1 & SD_OCR_ERRORBITS)             == SD_ALLZERO)                  return SYS_READY;
    if((Response_R1 & SD_OCR_ADDR_OUT_OF_RANGE)     == SD_OCR_ADDR_OUT_OF_RANGE)    return SYS_OUT_OF_RANGE;
    if((Response_R1 & SD_OCR_ADDR_MISALIGNED)       == SD_OCR_ADDR_MISALIGNED)      return SYS_ADDRESS_MISALIGNED;
    if((Response_R1 & SD_OCR_BLOCK_LEN_ERR)         == SD_OCR_BLOCK_LEN_ERR)        return SYS_BLOCK_LENGTH_ERROR;
    if((Response_R1 & SD_OCR_ERASE_SEQ_ERR)         == SD_OCR_ERASE_SEQ_ERR)        return SD_ERASE_SEQ_ERR;
    if((Response_R1 & SD_OCR_BAD_ERASE_PARAM)       == SD_OCR_BAD_ERASE_PARAM)      return SD_BAD_ERASE_PARAM;
    if((Response_R1 & SD_OCR_WRITE_PROT_VIOLATION)  == SD_OCR_WRITE_PROT_VIOLATION) return SD_WRITE_PROT_VIOLATION;
    if((Response_R1 & SD_OCR_LOCK_UNLOCK_FAILED)    == SD_OCR_LOCK_UNLOCK_FAILED)   return SYS_LOCK_UNLOCK_FAILED;
    if((Response_R1 & SD_OCR_COM_CRC_FAILED)        == SD_OCR_COM_CRC_FAILED)       return SYS_CRC_FAIL;
    if((Response_R1 & SD_OCR_ILLEGAL_CMD)           == SD_OCR_ILLEGAL_CMD)          return SYS_ILLEGAL_COMMAND;
    if((Response_R1 & SD_OCR_CARD_ECC_FAILED)       == SD_OCR_CARD_ECC_FAILED)      return SD_CARD_ECC_FAILED;
    if((Response_R1 & SD_OCR_CC_ERROR)              == SD_OCR_CC_ERROR)             return SD_CC_ERROR;
    if((Response_R1 & SD_OCR_GENERAL_UNKNOWN_ERROR) == SD_OCR_GENERAL_UNKNOWN_ERROR)return SYS_GENERAL_UNKNOWN_ERROR;
    if((Response_R1 & SD_OCR_STREAM_READ_UNDERRUN)  == SD_OCR_STREAM_READ_UNDERRUN) return SYS_UNDERRUN;
    if((Response_R1 & SD_OCR_STREAM_WRITE_OVERRUN)  == SD_OCR_STREAM_WRITE_OVERRUN) return SYS_OVERRUN;
    if((Response_R1 & SD_OCR_CID_CSD_OVERWRITE)     == SD_OCR_CID_CSD_OVERWRITE)    return SD_CID_CSD_OVERWRITE;
    if((Response_R1 & SD_OCR_WP_ERASE_SKIP)         == SD_OCR_WP_ERASE_SKIP)        return SD_WP_ERASE_SKIP;
    if((Response_R1 & SD_OCR_CARD_ECC_DISABLED)     == SD_OCR_CARD_ECC_DISABLED)    return SD_CARD_ECC_DISABLED;
    if((Response_R1 & SD_OCR_ERASE_RESET)           == SD_OCR_ERASE_RESET)          return SD_ERASE_RESET;
    if((Response_R1 & SD_OCR_AKE_SEQ_ERROR)         == SD_OCR_AKE_SEQ_ERROR)        return SD_AKE_SEQ_ERROR;

    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: CmdResponse
//
//   Parameter(s):  uint8_t             Command
//	                uint32_t            Argument
//	                SD_ResponseType_e   ResponseType
//   Return value:  SystemState_e
//
//   Description:   Checks for error conditions for any response.
//                              - R0, R1, R6, R7
//                              - R2 (CID or CSD) response.
//                              - R3 (OCR) response.
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::CmdResponse(uint8_t Command, int32_t ResponseType)
{
    uint32_t Response_R1;
    uint32_t TimeOut;
    uint32_t Flag;

    if(ResponseType == -1) Flag = SDMMC_STA_CMDSENT;
    else                   Flag = SDMMC_STA_CCRCFAIL | SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT;

    TimeOut = SD_SOFTWARE_COMMAND_TIMEOUT;
    do
    {
        TimeOut--;
    }
    while(((SDMMC1->STA & Flag) == 0) && (TimeOut > 0));

    if(ResponseType <= 0)
    {
        if(TimeOut == 0)                            return SYS_COMMAND_TIME_OUT;
        else                                        return SYS_READY;
    }

    if((SDMMC1->STA & SDMMC_STA_CTIMEOUT) != 0)     return SYS_COMMAND_TIME_OUT;
    if(ResponseType == 3)
    {
        if(TimeOut == 0)                            return SYS_COMMAND_TIME_OUT;    // Card is not V2.0 compliant or card does not support the set voltage range
        else                                        return SYS_READY;               // Card is SD V2.0 compliant
    }
    if((SDMMC1->STA & SDMMC_STA_CCRCFAIL) != 0)     return SYS_CRC_FAIL;
    if(ResponseType == 2)                           return SYS_READY;
    if((uint8_t)SDMMC1->RESPCMD != Command)         return SYS_ILLEGAL_COMMAND;     // Check if response is of desired command

    Response_R1 = SDMMC1->RESP1;                                                    // We have received response, retrieve it for analysis

    if(ResponseType == 1)
    {
        return this->CheckOCR_Response(Response_R1);
    }
    else if(ResponseType == 6)
    {
        if((Response_R1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED)) == SD_ALLZERO)
        {
            m_RCA = Response_R1;
        }
        if((Response_R1 & SD_R6_GENERAL_UNKNOWN_ERROR) == SD_R6_GENERAL_UNKNOWN_ERROR)      return SYS_GENERAL_UNKNOWN_ERROR;
        if((Response_R1 & SD_R6_ILLEGAL_CMD)           == SD_R6_ILLEGAL_CMD)                return SYS_ILLEGAL_COMMAND;
        if((Response_R1 & SD_R6_COM_CRC_FAILED)        == SD_R6_COM_CRC_FAILED)             return SYS_CRC_FAIL;
    }

    return SYS_READY;
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
//   Function:      IsDetected
//
//   Parameter(s):  None
//   Return Value:  SystemState_e       Return
//
//   Description:   Test if card is present
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::IsDetected(void)
{
  #if SD_CARD_USE_DETECT_SIGNAL == 1
    return (IO_Input(DIGINI_SD_CARD_DETECT_IO) == IO_RESET) ? SYS_READY : SYS_DEVICE_NOT_PRESENT;
  #else
    return SYS_READY;
  #endif
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Lock
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Lock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SDIO_Driver::Lock(void)
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
void SDIO_Driver::Unlock(void)
{
    nOS_MutexUnlock(&m_Mutex);
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      SD_PowerON
//
//   Parameter(s):  None
//   Return Value:  SystemState_e
//
//   Description:   Inquires cards about their operating voltage and configures clock
//                  controls and stores SD information that will be needed in future.
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::PowerON(void)
{
    SystemState_e   State;
    uint32_t        Response;
    uint32_t        Count        = 0;
    uint32_t        ValidVoltage = 0;
    uint32_t        SD_Type       = SD_RESP_STD_CAPACITY;
    nOS_TickCounter TickStart;

    // Power ON Sequence -------------------------------------------------------
    SDMMC1->CLKCR &= ~SDMMC_CLKCR_CLKEN;        // Disable SDMMC Clock
    SDMMC1->POWER  = SDMMC_POWER_PWRCTRL;       // Set Power State to ON

    // 1ms: required power up waiting time before starting the SD initialization sequence
    TickStart = nOS_GetTickCount();
    while((nOS_GetTickCount() - TickStart) < 1){}

    SDMMC1->CLKCR |= SDMMC_CLKCR_CLKEN;           // Enable SDMMC Clock

    // CMD0: GO_IDLE_STATE -----------------------------------------------------
    // No CMD Response required
    if((State = this->TransmitCommand(CMD0, 0, 0)) != SYS_READY)
    {
        // CMD Response Timeout (wait for CMDSENT flag)
        return State;
    }

    // CMD8: SEND_IF_COND ------------------------------------------------------
    // Send CMD8 to verify SD card interface operating condition
    // Argument: - [31:12]: Reserved (shall be set to '0')
    //- [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
    //- [7:0]: Check Pattern (recommended 0xAA)
    // CMD Response: R7
    if((State = this->TransmitCommand((CMD8 | SD_SHORT_RESPONSE), SD_CHECK_PATTERN, 7)) == SYS_READY)
    {
        // SD Card 2.0
        m_CardType = SD_STD_CAPACITY_V2_0;
        SD_Type    = SD_RESP_HIGH_CAPACITY;
    }

    // Send CMD55
    // If State is Command Timeout, it is a MMC card
    // If State is SYS_READY it is a SD card: SD card 2.0 (voltage range mismatch) or SD card 1.x
    if((State = this->TransmitCommand((CMD55 | SD_SHORT_RESPONSE), 0, 1)) == SYS_READY)
    {
        // SD CARD
        // Send ACMD41 SD_APP_OP_COND with Argument 0x80100000
        while((ValidVoltage == false) && (Count < SD_MAX_VOLT_TRIAL))
        {

            // SEND CMD55 APP_CMD with RCA as 0
            if((State = this->TransmitCommand((CMD55 | SD_SHORT_RESPONSE), 0, 1)) != SYS_READY)
            {
                return State;
            }

            // Send CMD41
            if((State = this->TransmitCommand((CMD41 | SD_SHORT_RESPONSE), SD_VOLTAGE_WINDOW | SD_Type, 3)) != SYS_READY)
            {
                return State;
            }

            Response = SDMMC1->RESP1;                               // Get command response
            ValidVoltage = (((Response >> 31) == 1) ? 1 : 0);       // Get operating voltage
            Count++;
        }

        if(Count >= SD_MAX_VOLT_TRIAL)
        {
            return SYS_INVALID_VOLTAGE_RANGE;
        }

        if((Response & SD_RESP_HIGH_CAPACITY) == SD_RESP_HIGH_CAPACITY)
        {
            m_CardType = SD_HIGH_CAPACITY;
        }
    } // else MMC Card

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      SD_PowerON
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Put interface in power OFF
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SDIO_Driver::PowerOFF(void)
{
    // Set Power State to OFF
    SDMMC1->POWER = (uint32_t)0;

    // Reset SDIO Module
    RCC->APB2RSTR |=  RCC_APB2RSTR_SDMMC1RST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_SDMMC1RST;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      SD_InitializeCard
//
//   Parameter(s):  None
//   Return Value:  SystemState_e
//
//   Description:   Initializes all cards or single card as the case may be Card(s) come into standby state.
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::InitializeCard(void)
{
    SystemState_e State;

    if((SDMMC1->POWER & SDMMC_POWER_PWRCTRL) != 0) // Power off
    {
        if(m_CardType != SD_SECURE_DIGITAL_IO)
        {
            // Send CMD2 ALL_SEND_CID
            if((State = this->TransmitCommand((CMD2 | SD_LONG_RESPONSE), 0, 2)) != SYS_READY)
            {
                return State;
            }

            // Get Card identification number data
            this->GetResponse(m_CardCID);
        }

        if((m_CardType == SD_STD_CAPACITY_V1_1)       || (m_CardType == SD_STD_CAPACITY_V2_0) ||
           (m_CardType == SD_SECURE_DIGITAL_IO_COMBO) || (m_CardType == SD_HIGH_CAPACITY))
        {
            // Send CMD3 SET_REL_ADDR with argument 0
            // SD Card publishes its RCA.
            if((State = this->TransmitCommand((CMD3 | SD_SHORT_RESPONSE), 0, 6)) != SYS_READY)
            {
                return State;
            }
        }

        if(m_CardType != SD_SECURE_DIGITAL_IO)
        {
            // Send CMD9 SEND_CSD with argument as card's RCA
            if((State = this->TransmitCommand((CMD9 | SD_LONG_RESPONSE), m_RCA, 2)) == SYS_READY)
            {
                // Get Card Specific Data
                this->GetResponse(m_CardCSD);
            }
        }
    }
    else
    {
        State = SYS_REQUEST_NOT_APPLICABLE;
    }

    // Card are initialized
    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: DataInit
//
//   Parameter(s):  uint32_t       Size
//                  uint32_t       DataBlockSize
//                  bool           IsItReadFromCard
//   Return value:  None
//
//   Description:   Configuration for SDIO Data using SDIO_DataInitTypeDef
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SDIO_Driver::DataInit(uint32_t Size, uint32_t DataBlockSize, bool IsItReadFromCard)
{
    uint32_t Direction;

    SDMMC1->DTIMER = SD_DATA_TIMEOUT;       // Set the SDMMC Data TimeOut value
    SDMMC1->DLEN   = Size;                  // Set the SDMMC DataLength value
    Direction      = (IsItReadFromCard == true) ? SDMMC_DCTRL_DTDIR : 0;
    SDMMC1->DCTRL |=  (uint32_t)(DataBlockSize | Direction | SDMMC_DCTRL_DTEN);
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      FindSCR
//
//   Parameter(s):  pSCR: pointer to the buffer that will contain the SCR value
//   Return Value:
//
//   Description:   Finds the SD card SCR register value.
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::FindSCR(uint32_t* pSCR)
{
    SystemState_e State;
    int           Index;
    uint32_t      SCR[2];

    // Set Block Size To 8 Bytes
    // Send CMD16 Set Block length
    if((State = this->TransmitCommand((CMD16 | SD_SHORT_RESPONSE), 8, 1)) == SYS_READY)
    {
        // Send CMD55 APP_CMD with argument as card's RCA
        if((State = this->TransmitCommand((CMD55 | SD_SHORT_RESPONSE), m_RCA, 1)) == SYS_READY)
        {
            this->DataInit(8, SD_DATABLOCK_SIZE_8B, true);

            // Send ACMD51 SD_APP_SEND_SCR with argument as 0
            if((State = this->TransmitCommand((CMD51 | SD_SHORT_RESPONSE), 0, 1)) == SYS_READY)
            {
                Index = 0;
                while(((SDMMC1->STA & (SDMMC_STA_RXOVERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DBCKEND)) == 0) &&
                      (Index < 2))
                {
                    if((SDMMC1->STA & SDMMC_STA_RXDAVL) != 0)
                    {
                      *(SCR + Index) = SDMMC1->FIFO;
                      Index++;
                    }
                }

                if     ((SDMMC1->STA & SDMMC_STA_DTIMEOUT) != 0) State = SYS_DATA_TIME_OUT;
                else if((SDMMC1->STA & SDMMC_STA_DCRCFAIL) != 0) State = SYS_CRC_FAIL;
                else if((SDMMC1->STA & SDMMC_STA_RXOVERR)  != 0) State = SYS_OVERRUN;
                else if((SDMMC1->STA & SDMMC_STA_RXDAVL)   != 0) State = SYS_OUT_OF_BOUND;
                else
                {
                    *(pSCR + 1) = ((SCR[0] & 0x000000FF) << 24) | ((SCR[0] & 0x0000FF00) << 8) |
                                  ((SCR[0] & 0x00FF0000) >> 8)  | ((SCR[0] & 0xFF000000) >> 24);

                    *(pSCR)     = ((SCR[1] & 0x000000FF) << 24) | ((SCR[1] & 0x0000FF00) << 8) |
                                  ((SCR[1] & 0x00FF0000) >> 8)  | ((SCR[1] & 0xFF000000) >> 24);
                }
            }
        }
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: GetStatus
//
//   Parameter(s):  None
//   Return value:  SystemState_e
//
//   Description:   Returns information about specific card.
//                  Contains all SD card information
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::GetCardInfo(void)
{
    SystemState_e State;
    uint32_t      Temp;

    State = SYS_READY;

    // Byte 0
    Temp = (m_CardCSD[0] & 0xFF000000) >> 24;
    m_CSD.CSDStruct      = (uint8_t)((Temp & 0xC0) >> 6);
    m_CSD.SysSpecVersion = (uint8_t)((Temp & 0x3C) >> 2);
    m_CSD.Reserved1      = Temp & 0x03;

    // Byte 1
    Temp = (m_CardCSD[0] & 0x00FF0000) >> 16;
    m_CSD.TAAC = (uint8_t)Temp;

    // Byte 2
    Temp = (m_CardCSD[0] & 0x0000FF00) >> 8;
    m_CSD.NSAC = (uint8_t)Temp;

    // Byte 3
    Temp = m_CardCSD[0] & 0x000000FF;
    m_CSD.MaxBusClkFrec = (uint8_t)Temp;

    // Byte 4
    Temp = (m_CardCSD[1] & 0xFF000000) >> 24;
    m_CSD.CardComdClasses = (uint16_t)(Temp << 4);

    // Byte 5
    Temp = (m_CardCSD[1] & 0x00FF0000) >> 16;
    m_CSD.CardComdClasses |= (uint16_t)((Temp & 0xF0) >> 4);
    m_CSD.RdBlockLen       = (uint8_t)(Temp & 0x0F);

    // Byte 6
    Temp = (m_CardCSD[1] & 0x0000FF00) >> 8;
    m_CSD.PartBlockRead   = (uint8_t)((Temp & 0x80) >> 7);
    m_CSD.WrBlockMisalign = (uint8_t)((Temp & 0x40) >> 6);
    m_CSD.RdBlockMisalign = (uint8_t)((Temp & 0x20) >> 5);
    m_CSD.DSRImpl         = (uint8_t)((Temp & 0x10) >> 4);
    m_CSD.Reserved2       = 0;

    if((m_CardType == SD_STD_CAPACITY_V1_1) || (m_CardType == SD_STD_CAPACITY_V2_0))
    {
        m_CSD.DeviceSize = (Temp & 0x03) << 10;

        // Byte 7
        Temp = (uint8_t)(m_CardCSD[1] & 0x000000FF);
        m_CSD.DeviceSize |= (Temp) << 2;

        // Byte 8
        Temp = (uint8_t)((m_CardCSD[2] & 0xFF000000) >> 24);
        m_CSD.DeviceSize |= (Temp & 0xC0) >> 6;

        m_CSD.MaxRdCurrentVDDMin = (Temp & 0x38) >> 3;
        m_CSD.MaxRdCurrentVDDMax = (Temp & 0x07);

        // Byte 9
        Temp = (uint8_t)((m_CardCSD[2] & 0x00FF0000) >> 16);
        m_CSD.MaxWrCurrentVDDMin = (Temp & 0xE0) >> 5;
        m_CSD.MaxWrCurrentVDDMax = (Temp & 0x1C) >> 2;
        m_CSD.DeviceSizeMul      = (Temp & 0x03) << 1;

        // Byte 10
        Temp = (uint8_t)((m_CardCSD[2] & 0x0000FF00) >> 8);
        m_CSD.DeviceSizeMul |= (Temp & 0x80) >> 7;

        m_CardCapacity  = (m_CSD.DeviceSize + 1) ;
        m_CardCapacity *= (1 << (m_CSD.DeviceSizeMul + 2));
        m_CardBlockSize = 1 << (m_CSD.RdBlockLen);
        m_CardCapacity *= m_CardBlockSize;
    }
    else if(m_CardType == SD_HIGH_CAPACITY)
    {
        // Byte 7
        Temp = (uint8_t)(m_CardCSD[1] & 0x000000FF);
        m_CSD.DeviceSize = (Temp & 0x3F) << 16;

        // Byte 8
        Temp = (uint8_t)((m_CardCSD[2] & 0xFF000000) >> 24);

        m_CSD.DeviceSize |= (Temp << 8);

        // Byte 9
        Temp = (uint8_t)((m_CardCSD[2] & 0x00FF0000) >> 16);

        m_CSD.DeviceSize |= (Temp);

        // Byte 10
        Temp = (uint8_t)((m_CardCSD[2] & 0x0000FF00) >> 8);

        m_CardCapacity  = ((m_CSD.DeviceSize + 1)) * 512 * 1024;
        m_CardBlockSize = 512;
    }
    else
    {
        // Not supported card type
        State = SYS_ERROR;
    }

    m_CSD.EraseGrSize = (Temp & 0x40) >> 6;
    m_CSD.EraseGrMul  = (Temp & 0x3F) << 1;

    // Byte 11
    Temp = (uint8_t)(m_CardCSD[2] & 0x000000FF);
    m_CSD.EraseGrMul     |= (Temp & 0x80) >> 7;
    m_CSD.WrProtectGrSize = (Temp & 0x7F);

    // Byte 12
    Temp = (uint8_t)((m_CardCSD[3] & 0xFF000000) >> 24);
    m_CSD.WrProtectGrEnable = (Temp & 0x80) >> 7;
    m_CSD.ManDeflECC        = (Temp & 0x60) >> 5;
    m_CSD.WrSpeedFact       = (Temp & 0x1C) >> 2;
    m_CSD.MaxWrBlockLen     = (Temp & 0x03) << 2;

    // Byte 13
    Temp = (uint8_t)((m_CardCSD[3] & 0x00FF0000) >> 16);
    m_CSD.MaxWrBlockLen      |= (Temp & 0xC0) >> 6;
    m_CSD.WriteBlockPaPartial = (Temp & 0x20) >> 5;
    m_CSD.Reserved3           = 0;
    m_CSD.ContentProtectAppli = (Temp & 0x01);

    // Byte 14
    Temp = (uint8_t)((m_CardCSD[3] & 0x0000FF00) >> 8);
    m_CSD.FileFormatGrouop = (Temp & 0x80) >> 7;
    m_CSD.CopyFlag         = (Temp & 0x40) >> 6;
    m_CSD.PermWrProtect    = (Temp & 0x20) >> 5;
    m_CSD.TempWrProtect    = (Temp & 0x10) >> 4;
    m_CSD.FileFormat       = (Temp & 0x0C) >> 2;
    m_CSD.ECC              = (Temp & 0x03);

    // Byte 15
    Temp = (uint8_t)(m_CardCSD[3] & 0x000000FF);
    m_CSD.CSD_CRC   = (Temp & 0xFE) >> 1;
    m_CSD.Reserved4 = 1;

    // Byte 0
    Temp = (uint8_t)((m_CardCID[0] & 0xFF000000) >> 24);
    m_CID.ManufacturerID = Temp;

    // Byte 1
    Temp = (uint8_t)((m_CardCID[0] & 0x00FF0000) >> 16);
    m_CID.OEM_AppliID = Temp << 8;

    // Byte 2
    Temp = (uint8_t)((m_CardCID[0] & 0x000000FF00) >> 8);
    m_CID.OEM_AppliID |= Temp;

    // Byte 3
    Temp = (uint8_t)(m_CardCID[0] & 0x000000FF);
    m_CID.ProdName1 = Temp << 24;

    // Byte 4
    Temp = (uint8_t)((m_CardCID[1] & 0xFF000000) >> 24);
    m_CID.ProdName1 |= Temp << 16;

    // Byte 5
    Temp = (uint8_t)((m_CardCID[1] & 0x00FF0000) >> 16);
    m_CID.ProdName1 |= Temp << 8;

    // Byte 6
    Temp = (uint8_t)((m_CardCID[1] & 0x0000FF00) >> 8);
    m_CID.ProdName1 |= Temp;

    // Byte 7
    Temp = (uint8_t)(m_CardCID[1] & 0x000000FF);
    m_CID.ProdName2 = Temp;

    // Byte 8
    Temp = (uint8_t)((m_CardCID[2] & 0xFF000000) >> 24);
    m_CID.ProdRev = Temp;

    // Byte 9
    Temp = (uint8_t)((m_CardCID[2] & 0x00FF0000) >> 16);
    m_CID.ProdSN = Temp << 24;

    // Byte 10
    Temp = (uint8_t)((m_CardCID[2] & 0x0000FF00) >> 8);
    m_CID.ProdSN |= Temp << 16;

    // Byte 11
    Temp = (uint8_t)(m_CardCID[2] & 0x000000FF);
    m_CID.ProdSN |= Temp << 8;

    // Byte 12
    Temp = (uint8_t)((m_CardCID[3] & 0xFF000000) >> 24);
    m_CID.ProdSN |= Temp;

    // Byte 13
    Temp = (uint8_t)((m_CardCID[3] & 0x00FF0000) >> 16);
    m_CID.Reserved1   |= (Temp & 0xF0) >> 4;
    m_CID.ManufactDate = (Temp & 0x0F) << 8;

    // Byte 14
    Temp = (uint8_t)((m_CardCID[3] & 0x0000FF00) >> 8);
    m_CID.ManufactDate |= Temp;

    // Byte 15
    Temp = (uint8_t)(m_CardCID[3] & 0x000000FF);
    m_CID.CID_CRC   = (Temp & 0xFE) >> 1;
    m_CID.Reserved2 = 1;

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: StartBlockTransfert
//
//   Parameter(s):  DMA_Stream_TypeDef* pDMA            DMA Stream to use for the DMA operation
//                  uint64_t            pBuffer         Pointer to/for the buffer the data
//                  uint32_t            BlockSize       The SD card Data block size
//                  uint32_t            NumberOfBlocks  Number of blocks to access
//   Return value:  SystemState_e
//
//   Description:   Prepare the DMA transfer
//
//   note(s):       This should be preceding the functions WriteBlocks() or ReadBlocks()
//                  completion of the write process (by SD current status polling).
//
//-------------------------------------------------------------------------------------------------
void SDIO_Driver::StartBlockTransfert(DMA_Stream_TypeDef* pDMA, uint32_t* pBuffer, uint32_t BlockSize, uint32_t NumberOfBlocks)
{
    SDMMC1->DCTRL      = 0;                                                                 // Initialize data control register
    m_TransferComplete = false;                                                                 // Initialize handle flags
    m_DMA_XferComplete = false;
    m_TransferError    = SYS_READY;
    m_Operation        = (NumberOfBlocks > 1) ? SD_MULTIPLE_BLOCK : SD_SINGLE_BLOCK;        // Initialize SD Read operation
    SDMMC1->MASK      |= (SDMMC_MASK_DCRCFAILIE | SDMMC_MASK_DTIMEOUTIE |                   // Enable transfer interrupts
                          SDMMC_MASK_DATAENDIE  | SDMMC_MASK_RXOVERRIE);
    SDMMC1->DCTRL     |= SDMMC_DCTRL_DMAEN;                                                 // Enable SDMMC DMA transfer
    pDMA->CR          &= ~DMA_SxCR_EN;                                                      // Disable the Peripheral
    pDMA->NDTR         = (uint32_t)(BlockSize * NumberOfBlocks) / 4;                        // Configure DMA Stream data length
    pDMA->M0AR         = (uint32_t)pBuffer;                                                 // Configure DMA Stream memory address
    pDMA->CR          |= DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE;    // Enable all interrupts
    pDMA->FCR         |= DMA_SxFCR_FEIE;
    pDMA->CR          |= DMA_SxCR_EN;                                                       // Enable the Peripheral
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: ReadBlocks
//
//   Parameter(s):  uint64_t ReadAddress
//                  uint32_t BlockSize
//                  uint32_t NumberOfBlocks
//   Return value:  SystemState_e
//
//   Description:   Reads block(s) from a specified address in a card. The Data transfer is managed
//                  by DMA mode.
//
//   note(s):       This API should be followed by the function CheckOperation() to check the
//                  completion of the write process (by SD current status polling).
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::ReadBlocks(uint64_t ReadAddress, uint32_t BlockSize, uint32_t NumberOfBlocks)
{
    SystemState_e State;
    uint32_t      CmdIndex;

    if(m_CardType == SD_HIGH_CAPACITY)
    {
        BlockSize    = 512;
        ReadAddress /= 512;
    }

    // Set Block Size for Card
    if((State = this->TransmitCommand((CMD16 | SD_SHORT_RESPONSE), BlockSize, 1)) == SYS_READY)
    {
        // Configure the SD DPSM (Data Path State Machine)
        this->DataInit(BlockSize * NumberOfBlocks, SD_DATABLOCK_SIZE_512B, true);

    }

    // Send CMD18 READ_MULT_BLOCK with argument data address
    // or send CMD17 READ_SINGLE_BLOCK depending on number of block
    CmdIndex = (NumberOfBlocks > 1) ? CMD18 : CMD17;
    State    = this->TransmitCommand((CmdIndex | SD_SHORT_RESPONSE), (uint32_t)ReadAddress, 1);

    // Update the SD transfer error in SD handle
    m_TransferError = State;

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: WriteBlocks
//
//   Parameter(s):  uint64_t WriteAddress
//                  uint32_t BlockSize
//                  uint32_t NumberOfBlocks
//   Return value:  SystemState_e
//
//   Description:   Writes block(s) to a specified address in a card. The Data transfer is managed
//                  by DMA mode.
//
//   note(s):       This API should be followed by the function CheckOperation() to check the
//                  completion of the write process (by SD current status polling).
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::WriteBlocks(uint64_t WriteAddress, uint32_t BlockSize, uint32_t NumberOfBlocks)
{
    SystemState_e State;
    uint32_t      CmdIndex;

    if(m_CardType == SD_HIGH_CAPACITY)
    {
        BlockSize    = 512;
        WriteAddress /= 512;
    }

    // i think block size is missing


    // Check number of blocks command
    // Send CMD24 WRITE_SINGLE_BLOCK
    // Send CMD25 WRITE_MULT_BLOCK with argument data address
    CmdIndex = (NumberOfBlocks > 1) ? CMD25 : CMD24;

    // Set Block Size for Card
    if((State = this->TransmitCommand((CmdIndex | SD_SHORT_RESPONSE), (uint32_t)WriteAddress, 1)) != SYS_READY)
    {
        return State;
    }

    // Configure the SD DPSM (Data Path State Machine)
    this->DataInit(BlockSize * NumberOfBlocks, SD_DATABLOCK_SIZE_512B, false);

    m_TransferError = State;

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: CheckOperation
//
//   Parameter(s):  uint32_t       Flag
//   Return value:  SystemState_e
//
//   Description:   This function waits until the SD DMA data wirte or read transfer is finished.
//                  This should be called after WriteBlocks_DMA or SD_ReadBlocks_DMA() function
//                  to insure that all data sent is already transferred by the DMA controller.
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::CheckOperation(uint32_t Flag)
{
    SystemState_e  State;
    uint32_t       TimeOut;

    // Wait for DMA/SD transfer end or SD error variables
    TimeOut = SD_DATA_TIMEOUT;
    while((m_DMA_XferComplete == false)     &&
          (m_TransferComplete == false)     &&
          (m_TransferError    == SYS_READY) &&
          (TimeOut > 0))
    {
        TimeOut--;
    }

    // Wait until the Rx transfer is no longer active
    TimeOut = SD_DATA_TIMEOUT;
    while(((SDMMC1->STA & Flag) != 0) && (TimeOut > 0))
    {
        TimeOut--;
    }

    // Send stop command in multi block read
    if(m_Operation == SD_MULTIPLE_BLOCK)
    {
        State = this->TransmitCommand((CMD12 | SD_SHORT_RESPONSE), 0, 1);
    }

    if((TimeOut == 0) && (State == SYS_READY))
    {
        State = SYS_DATA_TIME_OUT;
    }

    // Return error state
    if(m_TransferError != SYS_READY)
    {
        return m_TransferError;
    }

    return State;
}

/*
//-------------------------------------------------------------------------------------------------
//
//   Function name: WaitReady
//
//   Parameter(s):  uint32_t        Timer            Timeout in unit of 1 msec
//   Return value:  SystemState_e
//
//   Description:   Wait card ready
//
//-------------------------------------------------------------------------------------------------
SystemState_e SDIO_Driver::WaitReady(uint32_t Timer)
{
    uint32_t Response;

    m_TimeOut = Timer;

    do
    {
        this->TransmitCommand(CMD13, m_RCA, 1);
        SDMMC1->RESP1);

        if((Response & 0x00001E00) == 0x00000800)
        {
            return SYS_READY;
        }

        if(this->IsDetected() == SYS_DEVICE_NOT_PRESENT)                   // if card is Remove while waiting
        {
            return SYS_DEVICE_NOT_PRESENT;
        }
    }
    while(m_TimeOut != 0);

    return SYS_TIME_OUT;
}
*/

//-------------------------------------------------------------------------------------------------
//
//   Function name: SD DMA transfer complete RX and TX callback.
//
//   Parameter(s):  pDMA_Stream
//   Return value:  None
//
//   Description:   DMA IRQ
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SDIO_Driver::DMA_Complete(DMA_Stream_TypeDef* pDMA_Stream)
{
    m_DMA_XferComplete = 1;             // DMA transfer is complete
    while(m_TransferComplete == 0){}    // Wait until SD transfer is complete
    pDMA_Stream->CR &= ~DMA_SxCR_EN;    // Disable the stream
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: SDIO_IRQHandler
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   SDIO IRQ
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SDIO_Driver::SDMMC1_IRQHandler(void)
{
    // Check for SDMMC interrupt flags
    if((SDMMC1->STA & SDMMC_STA_DATAEND) != 0)
    {
        m_TransferError    = SYS_READY;
        m_TransferComplete = true;
    }
    else if((SDMMC1->STA & SDMMC_STA_DCRCFAIL) != 0)    m_TransferError = SYS_CRC_FAIL;
    else if((SDMMC1->STA & SDMMC_STA_DTIMEOUT) != 0)    m_TransferError = SYS_DATA_TIME_OUT;
    else if((SDMMC1->STA & SDMMC_STA_RXOVERR) != 0)     m_TransferError = SYS_OVERRUN;
    else if((SDMMC1->STA & SDMMC_STA_TXUNDERR) != 0)    m_TransferError = SYS_UNDERRUN;

    ClearAllFlag();

    // Disable all SDMMC peripheral interrupt sources
    SDMMC1->MASK &= ~(SDMMC_MASK_DCRCFAILIE | SDMMC_MASK_DTIMEOUTIE | SDMMC_MASK_DATAENDIE  |
                      SDMMC_MASK_TXFIFOHEIE | SDMMC_MASK_RXFIFOHFIE | SDMMC_MASK_TXUNDERRIE |
                      SDMMC_MASK_RXOVERRIE);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: DMA_Stream3IRQHandler
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   DMA IRQ
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SDIO_Driver::DMA_Stream3IRQHandler(void)
{
    // Transfer Error Interrupt management
    if((DMA2->LISR & DMA_LISR_TEIF3) != 0)
    {
        if((DMA2_Stream3->CR & DMA_SxCR_TEIE) != 0)
        {
            DMA2_Stream3->CR   &= ~DMA_SxCR_TEIE;       // Disable the transfer error interrupt
            DMA2->LIFCR = DMA_LIFCR_CTEIF3;             // Clear the transfer error flag
        }
    }

    // FIFO Error Interrupt management
    if((DMA2->LISR & DMA_LISR_FEIF3) != 0)
    {
        if((DMA2_Stream3->FCR & DMA_SxFCR_FEIE) != 0)
        {
            DMA2_Stream3->FCR   &= ~DMA_SxFCR_FEIE;     // Disable the FIFO Error interrupt
            DMA2->LIFCR = DMA_LIFCR_CFEIF3;             // Clear the FIFO error flag
        }
    }

    // Direct Mode Error Interrupt management
    if((DMA2->LISR & DMA_LISR_DMEIF3) != 0)
    {
        if((DMA2_Stream3->CR & DMA_SxCR_DMEIE) != 0)
        {
            DMA2_Stream3->CR   &= ~DMA_SxCR_DMEIE;       // Disable the direct mode Error interrupt
            DMA2->LIFCR = DMA_LIFCR_CDMEIF3;             // Clear the FIFO error flag
        }
    }

    // Half Transfer Complete Interrupt management
    if((DMA2->LISR & DMA_LISR_HTIF3) != 0)
    {
        if((DMA2_Stream3->CR & DMA_SxCR_HTIE) != 0)
        {
            if(((DMA2_Stream3->CR) & (uint32_t)(DMA_SxCR_DBM)) != 0)    // Multi_Buffering mode enabled
            {
                DMA2->LIFCR = DMA_LIFCR_CHTIF3;                         // Clear the half transfer complete flag
            }
            else
            {
                if((DMA2_Stream3->CR & DMA_SxCR_CIRC) == 0)             // Disable the half transfer interrupt if the DMA mode is not CIRCULAR
                {
                    DMA2_Stream3->CR   &= ~DMA_SxCR_HTIE;               // Disable the half transfer interrupt
                }

                DMA2->LIFCR = DMA_LIFCR_CHTIF3;                         // Clear the half transfer complete flag
            }
        }
    }

    // Transfer Complete Interrupt management
    if((DMA2->LISR & DMA_LISR_TCIF3) != 0)
    {
        if((DMA2_Stream3->CR & DMA_SxCR_TCIE) != 0)
        {
            if((DMA2_Stream3->CR & (uint32_t)(DMA_SxCR_DBM)) != 0)
            {
                DMA2->LIFCR = DMA_LIFCR_CTCIF3;                         // Clear the transfer complete flag
            }
            else //Disable the transfer complete interrupt if the DMA mode is not CIRCULAR
            {
                if((DMA2_Stream3->CR & DMA_SxCR_CIRC) == 0)
                {
                    DMA2_Stream3->CR &= ~DMA_SxCR_TCIE;                 // Disable the transfer complete interrupt
                }

                DMA2->LIFCR = DMA_LIFCR_CTCIF3;                         // Clear the transfer complete flag
                DMA_Complete(DMA2_Stream3);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: DMA_Stream6IRQHandler
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   DMA IRQ
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SDIO_Driver::DMA_Stream6IRQHandler(void)
{
    // Transfer Error Interrupt management
    if((DMA2->HISR & DMA_HISR_TEIF6) != 0)
    {
        if((DMA2_Stream6->CR & DMA_SxCR_TEIE) != 0)
        {
            DMA2_Stream6->CR   &= ~DMA_SxCR_TEIE;       // Disable the transfer error interrupt
            DMA2->HIFCR = DMA_HIFCR_CTEIF6;             // Clear the transfer error flag
        }
    }

    // FIFO Error Interrupt management
    if((DMA2->HISR & DMA_HISR_FEIF6) != 0)
    {
        if((DMA2_Stream6->FCR & DMA_SxFCR_FEIE) != 0)
        {
            DMA2_Stream6->FCR   &= ~DMA_SxFCR_FEIE;     // Disable the FIFO Error interrupt
            DMA2->HIFCR = DMA_HIFCR_CFEIF6;             // Clear the FIFO error flag
        }
    }

    // Direct Mode Error Interrupt management
    if((DMA2->HISR & DMA_HISR_DMEIF6) != 0)
    {
        if((DMA2_Stream6->CR & DMA_SxCR_DMEIE) != 0)
        {
            DMA2_Stream6->CR   &= ~DMA_SxCR_DMEIE;       // Disable the direct mode Error interrupt
            DMA2->HIFCR = DMA_HIFCR_CDMEIF6;             // Clear the FIFO error flag
        }
    }

    // Half Transfer Complete Interrupt management
    if((DMA2->HISR & DMA_HISR_HTIF6) != 0)
    {
        if((DMA2_Stream6->CR & DMA_SxCR_HTIE) != 0)
        {
            if(((DMA2_Stream6->CR) & (uint32_t)(DMA_SxCR_DBM)) != 0)    // Multi_Buffering mode enabled
            {
                DMA2->HIFCR = DMA_HIFCR_CHTIF6;                         // Clear the half transfer complete flag
            }
            else
            {
                if((DMA2_Stream6->CR & DMA_SxCR_CIRC) == 0)             // Disable the half transfer interrupt if the DMA mode is not CIRCULAR
                {
                    DMA2_Stream6->CR &= ~DMA_SxCR_HTIE;                 // Disable the half transfer interrupt
                }

                DMA2->HIFCR = DMA_HIFCR_CHTIF6;                         // Clear the half transfer complete flag
            }
        }
    }

    // Transfer Complete Interrupt management
    if((DMA2->HISR & DMA_HISR_TCIF6) != 0)
    {
        if((DMA2_Stream6->CR & DMA_SxCR_TCIE) != 0)
        {
            if((DMA2_Stream6->CR & (uint32_t)(DMA_SxCR_DBM)) != 0)
            {
                DMA2->HIFCR = DMA_HIFCR_CTCIF6;                         // Clear the transfer complete flag
            }
            else //Disable the transfer complete interrupt if the DMA mode is not CIRCULAR
            {
                if((DMA2_Stream6->CR & DMA_SxCR_CIRC) == 0)
                {
                    DMA2_Stream6->CR   &= ~DMA_SxCR_TCIE;               // Disable the transfer complete interrupt
                }

                DMA2->HIFCR = DMA_HIFCR_CTCIF6;                         // Clear the transfer complete flag
                DMA_Complete(DMA2_Stream6);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: CardDetectIRQ_Handler
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   IRQ Handler for the card detect
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
#if (SD_CARD_USE_DETECT_SIGNAL == 1)
void SDIO_Driver::CardDetectIRQ_Handler(void)
{
    m_Detect = SYS_DEVICE_NOT_PRESENT;

    if(IO_Input(DETECT_SD_CARD) == IO_RESET)
    {
        m_Detect = SYS_READY;
    }
}
#endif

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_SDIO
