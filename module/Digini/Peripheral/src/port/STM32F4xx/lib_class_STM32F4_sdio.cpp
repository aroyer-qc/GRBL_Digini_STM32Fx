//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_sdio.cpp
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

//------ Note(s) ----------------------------------------------------------------------------------
//
//  MMCv4.2/SDv2 (in native mode via SDIO) control module
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
//
// Warning  : The STM32F4 in Revision-A generates a
//            "SDIO_IT_DCRCFAIL" in function "SD_ProcessIRQSrc"
//            To avoid this set the Define "SDIO_STM32F4_REV" to 0
//            (check the STM-Errata-Sheet for more information)
//
//-------------------------------------------------------------------------------------------------

#include "lib_class_STM32F4_sdio.h"

//-------------------------------------------------------------------------------------------------

#if (USE_SDIO_DRIVER == DEF_ENABLED)

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CSDIO
//
//   Parameter(s):  sSDIO* pSDIO_Info
//
//   Description:   Initializes the SDIO peripheral according to the specified parameters
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CSDIO::CSDIO(sSDIO* pSDIO)
{
    m_pSDIO = pSDIO;
}

//-------------------------------------------------------------------------------------------------
//
//   Destructor:   CSDIO
//
//   Parameter(s):
//
//   Description:    Deinitializes the SDIO peripheral
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CSDIO::~CSDIO()
{
}

//-------------------------------------------------------------------------------------------------
//
//
//  Function:       Initialize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Initializes the SDIO peripheral according to the specified Parameters
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CSDIO::Initialize(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    uint32_t            Priority;

    this->Lock();

    m_TransfertError    = SD_OK;
    m_DMA_EndOfTransfer = false;
    m_TransfertEnd      = false;
    m_StopCondition     = false;

    if(m_pSDIO->pDMA_Stream == DMA2_Stream3)
    {
        m_DMA_IRQn      = DMA2_Stream3_IRQn;
        m_DMA_FlagFEIF  = DMA_FLAG_FEIF3;
        m_DMA_FlagDMEIF = DMA_FLAG_DMEIF3;
        m_DMA_FlagTEIF  = DMA_FLAG_TEIF3;
        m_DMA_FlagHTIF  = DMA_FLAG_HTIF3;
        m_DMA_FlagTCIF  = DMA_FLAG_TCIF3;
    }
    else
    {
        m_DMA_IRQn      = DMA2_Stream6_IRQn;
        m_DMA_FlagFEIF  = DMA_FLAG_FEIF6;
        m_DMA_FlagDMEIF = DMA_FLAG_DMEIF6;
        m_DMA_FlagTEIF  = DMA_FLAG_TEIF6;
        m_DMA_FlagHTIF  = DMA_FLAG_HTIF6;
        m_DMA_FlagTCIF  = DMA_FLAG_TCIF6;
    }

    Priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), m_pSDIO->PreempPrio, 0x00);
    NVIC_SetPriority(SDIO_IRQn, Priority);
    NVIC_EnableIRQ(SDIO_IRQn);
    Priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), m_pSDIO->DMA_PreempPrio, 0x00);
    NVIC_SetPriority(m_DMA_IRQn, Priority);
    NVIC_EnableIRQ(m_DMA_IRQn);

    // GPIO Periph clock enable
    RCC_AHB1PeriphClockCmd(m_pSDIO->CLK_Clock |
                           m_pSDIO->CMD_Clock |
                           m_pSDIO->D0_Clock  |
                           m_pSDIO->D1_Clock  |
                           m_pSDIO->D2_Clock  |
                           m_pSDIO->D3_Clock,
                           ENABLE);

    GPIO_PinAFConfig(m_pSDIO->pCLK_Port, m_pSDIO->CLK_PinSource, GPIO_AF_SDIO);
    GPIO_PinAFConfig(m_pSDIO->pCMD_Port, m_pSDIO->CMD_PinSource, GPIO_AF_SDIO);
    GPIO_PinAFConfig(m_pSDIO->pD0_Port,  m_pSDIO->D0_PinSource,  GPIO_AF_SDIO);
    GPIO_PinAFConfig(m_pSDIO->pD1_Port,  m_pSDIO->D1_PinSource,  GPIO_AF_SDIO);
    GPIO_PinAFConfig(m_pSDIO->pD2_Port,  m_pSDIO->D2_PinSource,  GPIO_AF_SDIO);
    GPIO_PinAFConfig(m_pSDIO->pD3_Port,  m_pSDIO->D3_PinSource,  GPIO_AF_SDIO);

    // General setting for all pin
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    // Configure pin
    // Configure CLK, CMD, D0, D1, D2, D3 pins
    GPIO_InitStructure.GPIO_Pin = m_pSDIO->CMD_Pin;
    GPIO_Init(m_pSDIO->pCMD_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = m_pSDIO->D0_Pin;
    GPIO_Init(m_pSDIO->pD0_Port,  &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = m_pSDIO->D1_Pin;
    GPIO_Init(m_pSDIO->pD1_Port,  &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = m_pSDIO->D2_Pin;
    GPIO_Init(m_pSDIO->pD2_Port,  &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = m_pSDIO->D3_Pin;
    GPIO_Init(m_pSDIO->pD3_Port,  &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = m_pSDIO->CLK_Pin;
    GPIO_Init(m_pSDIO->pCLK_Port, &GPIO_InitStructure);

    // Enable the SDIO APB2 Clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, ENABLE);

    // Enable the DMA2 Clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    this->Config(LOW_SPEED);
    SDIO_SetPowerState(SDIO_PowerState_ON);
    SDIO_ClockCmd(ENABLE);

    this->Unlock();
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
SystemState_e CSDIO::GetResponse(uint32_t* pResponse)
{
    pResponse[0] = SDIO_GetResponse(SDIO_RESP1);
    pResponse[1] = SDIO_GetResponse(SDIO_RESP2);
    pResponse[2] = SDIO_GetResponse(SDIO_RESP3);
    pResponse[3] = SDIO_GetResponse(SDIO_RESP4);

    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       GetTransfertStatus
//
//  Parameter(s):   None
//  Return:         uint32_t    Status
//
//  Description:    Get the Status
//
//  Note(s):        STM32F4 status match definition of the library, so no translation necessary
//
//-------------------------------------------------------------------------------------------------
uint32_t CSDIO::GetTransfertStatus(void)
{
    uint32_t Status;

    Status  = SDIO->STA;

    // For a bug in ST SDIO module
    if((m_LastCommand == CMD1) || (m_LastCommand == CMD12) || (m_LastCommand == CMD41) || (m_LastCommand == ACMD41))
    {
        SDIO->ICR =  SDIO_FLAG_CCRCFAIL;
        Status   &= ~SDIO_FLAG_CCRCFAIL;
        Status   |=  SDIO_FLAG_CMDREND;
    }

    return Status;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: IsFlagSet
//
//   Parameter(s):  uint32_t         Status
//                  SD_StatusFlag_e  Flag
//   Return value:  bool             true or false
//
//   Description:   Test the SD flag for this driver and return the state
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
bool CSDIO::IsFlagSet(uint32_t Status, SD_StatusFlag Flag_e)
{
    /*
    0x00000001,     // SD_FLAG_COMMAND_CRC_FAIL
    0x00000002,     // SD_FLAG_DATA_CRC_FAIL
    0x00000004,     // SD_FLAG_COMMAND_TIME_OUT
    0x00000008,     // SD_FLAG_DATA_TIME_OUT
    0x00000010,     // SD_FLAG_TX_UNDERRUN
    0x00000020,     // SD_FLAG_RX_OVERRUN
    0x00000040,     // SD_FLAG_COMMAND_RESPONSE_END
    0x00000080,     // SD_FLAG_COMMAND_SENT
    0x00000100,     // SD_FLAG_DATA_END
    0x00000200,     // SD_FLAG_START_BIT_ERROR
    0x00000400,     // SD_FLAG_DATA_BLOCK_END
    0x00000800,     // SD_FLAG_CMD_ACTIVE
    */

    return ((((uint32_t)1 << Flag) & Status) != 0) ? true : false;
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
void CSDIO::TickHook(void)
{
    if(m_TimeOut > 0)
    {
        m_TimeOut--;
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: TransmitCommand
//
//   Parameter(s):  uint8_t         Command
//                  uint32_t        Argument
//   Return value:  None
//
//   Description:   Send command to the SD Card
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CSDIO::TransmitCommand(uint8_t Command, uint32_t Argument, ResponseType_e ResponseType)
{
    uint32_t            PortResponse;
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
    uint32_t            Status;
    uint32_t            Flag;

    m_LastCommand = Command;
    PortResponse  = SDIO_Response_Short;
    if(ResponseType == R2_RESPONSE) PortResponse = SDIO_Response_Long;
    if(ResponseType == NO_RESPONSE) PortResponse = SDIO_Response_No;

    // Clear the Command Flags
    SDIO->ICR = (SDIO_ICR_CCRCFAIL | SDIO_ICR_CTIMEOUT | SDIO_ICR_CMDREND | SDIO_ICR_CMDSENT);

    // ----------------------------------
    // ---------- Send Command ----------
    SDIO_CmdInitStructure.SDIO_CmdIndex = Command;
    SDIO_CmdInitStructure.SDIO_Argument = Argument;
    SDIO_CmdInitStructure.SDIO_Response = PortResponse;
    SDIO_CmdInitStructure.SDIO_Wait     = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM     = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    m_TimeOut = SDIO_SOFTWARE_COMMAND_TIMEOUT;

    if(ResponseType == NO_RESPONSE)      Flag = (SDIO_STA_CTIMEOUT | SDIO_STA_CMDSENT);
    else                                 Flag = (SDIO_STA_CTIMEOUT | SDIO_STA_CMDREND | SDIO_FLAG_CCRCFAIL);

    do
    {
        Status = SDIO->STA;
        if(m_TimeOut == 0) Status = SDIO_STA_CTIMEOUT;
    }
    while((Status & Flag) == 0);
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       InitWrite
//
//  Parameter(s):   const uint8_t*  pBuffer
//                  uint32_t        Count
//  Return:         void
//
//  Description:    Write data
//
//  Note(s):        this driver use DMA
//
//-------------------------------------------------------------------------------------------------
void CSDIO::StartBlockTransfert(const uint8_t *pBuffer, uint32_t Count, uint32_t TransfertDir)
{
    m_TransfertError    = BUSY;
    m_DMA_EndOfTransfer = false;
    m_TransfertEnd      = false;
    m_StopCondition     = true;
    SDIO->DCTRL         = 0;

    SDIO_DataInit(TransfertDir, Count);
    DMA_Config((uint32_t *)pBuffer, Count, SDIO_TransferDir_ToCard);
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND |SDIO_IT_RXOVERR | SDIO_IT_TXUNDERR | SDIO_IT_STBITERR, ENABLE);
    SDIO_DMACmd(ENABLE);
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       WaitTransfertEnd
//
//  Parameter(s):   None
//
//  Return:         SystemState_e
//
//  Description:    Wait Transfert End
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e CSDIO::WaitBlockTransfertEnd(void)
{
    m_TimeOut = SDIO_DATA_TIMEOUT;

    while(((m_DMA_EndOfTransfer == false )  &&
          ( m_TransfertEnd      == false )  &&
          ( m_TransfertError    == BUSY ))  &&
          ( m_TimeOut           != 0     ));

    m_DMA_EndOfTransfer = false;
    m_TimeOut           = SDIO_DATA_TIMEOUT;

    while(((SDIO->STA & SDIO_FLAG_TXACT)) && (m_TimeOut != 0))
    {
        OS_TaskYield();
    };

    if(m_StopCondition == true)
    {
        m_StopCondition = false;
        TransmitCommand(CMD12, 0, R1_RESPONSE);                             // CMD_CMD_STOP_TRANSMISSION
    }

    SDIO_ClearFlag(SDIO_STATIC_FLAGS);                                      // Clear all the static flags
    if(m_TransfertError != SD_OK)   return m_TransfertError;
    if(m_TimeOut        == 0)       return TIME_OUT;
    return READY;

}

//-------------------------------------------------------------------------------------------------
//
//  Function:       Config
//
//  Parameter(s):   eDeviceSpeed DeviceSpeed
//  Return:         None
//
//  Description:    Set the speed of the interface and the basic normal config for the SD module
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CSDIO::Config(DeviceSpeed_e DeviceSpeed)
{
    SDIO_InitTypeDef    SDIO_InitStructure;

    // Power ON Sequence
    // Configure the SDIO peripheral
    // SDIO_CK = SDIOCLK / (SDIO_INIT_CLK_DIV + 2)
    // on STM32F4xx devices, SDIOCLK is fixed to 48MHz
    // SDIO_CK for initialization should not exceed 400 KHz
    SDIO_InitStructure.SDIO_ClockDiv            = (DeviceSpeed == LOW_SPEED) ? SDIO_SPEED_400_KHZ : SDIO_SPEED_24_MHZ;
    SDIO_InitStructure.SDIO_ClockEdge           = SDIO_ClockEdge_Rising;
    SDIO_InitStructure.SDIO_ClockBypass         = SDIO_ClockBypass_Disable;
    SDIO_InitStructure.SDIO_ClockPowerSave      = SDIO_ClockPowerSave_Disable;
    SDIO_InitStructure.SDIO_BusWide             = (DeviceSpeed == LOW_SPEED) ? SDIO_BusWide_1b : SDIO_BusWide_4b;
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
    SDIO_Init(&SDIO_InitStructure);
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
void CSDIO::Lock(void)
{
    while(xSemaphoreTakeRecursive(*m_pSDIO->pMutex, portMAX_DELAY) != true){};
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
void CSDIO::Unlock(void)
{
    xSemaphoreGiveRecursive(*m_pSDIO->pMutex);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: DMA_DataInit
//
//   Parameter(s):  uint32_t TransfertDir
//                  size_t   Size
//   Return value:  None
//
//   Description:   Configuration for SDIO Data using SDIO_DataInitTypeDef
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CSDIO::SDIO_DataInit(uint32_t TransfertDir, size_t Size)
{
    SDIO_DataInitTypeDef SDIO_DataInitStructure;

    SDIO_DataInitStructure.SDIO_DataTimeOut   = SDIO_DATA_TIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength    = uint32_t(Size);
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DMA_BLOCK_SIZE_512;
    SDIO_DataInitStructure.SDIO_TransferDir   = TransfertDir;
    SDIO_DataInitStructure.SDIO_TransferMode  = SDIO_TransferMode_Block;
    SDIO_DataInitStructure.SDIO_DPSM          = SDIO_DPSM_Enable;
    SDIO_DataConfig(&SDIO_DataInitStructure);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: DMA_Config
//
//   Parameter(s):  uint32_t*   pBuffer
//                  uint32_t    BufferSize
//                              Mode
//   Return value:  None
//
//   Description:   Configuration of the DMA in RX
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CSDIO::DMA_Config(uint32_t* pBuffer, uint32_t BufferSize, uint32_t Direction)
{
    DMA_InitTypeDef SDDMA_InitStructure;

    DMA_ClearFlag(m_pSDIO->pDMA_Stream, m_DMA_FlagFEIF | m_DMA_FlagDMEIF | m_DMA_FlagTEIF | m_DMA_FlagHTIF | m_DMA_FlagTCIF);

    // DMA2 Stream disable
    DMA_Cmd(m_pSDIO->pDMA_Stream, DISABLE);

    // DMA2 Stream Config
    DMA_DeInit(m_pSDIO->pDMA_Stream);

    SDDMA_InitStructure.DMA_Channel            = DMA_Channel_4;                            // Only channel 4 available for SDIO
    SDDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SDIO->FIFO;
    SDDMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)pBuffer;
    SDDMA_InitStructure.DMA_DIR                = Direction;
    SDDMA_InitStructure.DMA_BufferSize         = BufferSize;
    SDDMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    SDDMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    SDDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    SDDMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;
    SDDMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    SDDMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
    SDDMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Enable;
    SDDMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    SDDMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_INC4;
    SDDMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_INC4;
    DMA_Init(m_pSDIO->pDMA_Stream, &SDDMA_InitStructure);
    DMA_ITConfig(m_pSDIO->pDMA_Stream, DMA_IT_TC, ENABLE);
    DMA_FlowControllerConfig(m_pSDIO->pDMA_Stream, DMA_FlowCtrl_Peripheral);

    // DMA2 Stream enable
    DMA_Cmd(m_pSDIO->pDMA_Stream, ENABLE);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: DMA_StreamIRQHandler
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   DMA IRQ
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CSDIO::DMA_StreamIRQHandler(void)
{
    if(DMA2->LISR & DMA_FLAG_TCIF3_7)
    {
        DMA2->LIFCR = DMA_FLAG_TCIF3_7;
        m_DMA_EndOfTransfer = true;
    }
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
void CSDIO::SDIO_IRQHandler(void)
{
    // Check for SDMMC interrupt flags
    if((SDMMC1->STA & SDMMC_IT_DATAEND) != RESET)
    {
        SDMMC1->ICR = SDMMC_IT_DATAEND;
        m_TransfertError = SYS_READY;   // No transfer error
    }
    else if((SDMMC1->STA & SDMMC_IT_DCRCFAIL) != RESET)
    {
        SDMMC1->ICR = SDMMC_FLAG_DCRCFAIL;
        m_TransfertError = SYS_SD_DATA_CRC_FAIL;
    }
    else if((SDMMC1->STA & SDMMC_IT_DTIMEOUT) != RESET)
    {
        SDMMC1->ICR = SDMMC_FLAG_DTIMEOUT;
        m_TransfertError = SYS_SD_DATA_TIMEOUT;
    }
    else if((SDMMC1->STA & SDMMC_IT_RXOVERR) != RESET)
    {
        SDMMC1->ICR = SDMMC_FLAG_RXOVERR;
        m_TransfertError = SYS_SD_RX_OVERRUN;
    }
    else if((SDMMC1->STA & SDMMC_IT_TXUNDERR) != RESET)
    {
        SDMMC1->ICR = SDMMC_FLAG_TXUNDERR;
        m_TransfertError = SYS_SD_TX_UNDERRUN;
    }
    else if(SDIO_GetITStatus(SDIO_IT_STBITERR) != RESET)
    {
        SDIO_ClearITPendingBit(SDIO_IT_STBITERR);
        m_TransfertError = START_BIT_ERROR;
    }

    SDMMC1->MASK &= ~(SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_DATAEND  |
                      SDMMC_IT_TXFIFOHE | SDMMC_IT_RXFIFOHF | SDMMC_IT_TXUNDERR |
                      SDMMC_IT_RXOVERR);

    m_TransfertEnd = true;
}

//-------------------------------------------------------------------------------------------------

#endif // USE_SDIO_DRIVER == DEF_ENABLED

