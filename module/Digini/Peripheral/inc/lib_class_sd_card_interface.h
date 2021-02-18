//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_sd_card_interface.h
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

//#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------
// enum(s)
//-------------------------------------------------------------------------------------------------

enum ResponseType_e
{
    NO_RESPONSE,
    SHORT_RESPONSE,
    LONG_RESPONSE,
    R1_RESPONSE,
    R1_RESPONSE_B,
    R2_RESPONSE,
    R3_RESPONSE,
    R4_RESPONSE,
    R5_RESPONSE,
    R6_RESPONSE,
    R7_RESPONSE,
};

 enum SD_StatusFlag_e
{
    SD_FLAG_COMMAND_CRC_FAIL = 0,
    SD_FLAG_DATA_CRC_FAIL,
    SD_FLAG_COMMAND_TIME_OUT,
    SD_FLAG_DATA_TIME_OUT,
    SD_FLAG_TX_UNDERRUN,
    SD_FLAG_RX_OVERRUN,
    SD_FLAG_COMMAND_RESPONSE_END,
    SD_FLAG_COMMAND_SENT,
    SD_FLAG_DATA_END,
    SD_FLAG_START_BIT_ERROR,
    SD_FLAG_DATA_BLOCK_END,
    SD_FLAG_COMMAND_ACTIVE,
};

enum SD_TransferType_e
{
    SD_CARD_TRANSFER_TO_CARD,
    SD_CARD_TRANSFER_FROM_CARD
};

enum SD_CARD_BlockSize_e
{
    SD_CARD_DATABLOCK_SIZE_1B   = 0,
    SD_CARD_DATABLOCK_SIZE_2B,
    SD_CARD_DATABLOCK_SIZE_4B,
    SD_CARD_DATABLOCK_SIZE_8B,
    SD_CARD_DATABLOCK_SIZE_16B,
    SD_CARD_DATABLOCK_SIZE_32B,
    SD_CARD_DATABLOCK_SIZE_64B,
    SD_CARD_DATABLOCK_SIZE_128B,
    SD_CARD_DATABLOCK_SIZE_256B,
    SD_CARD_DATABLOCK_SIZE_512B,
    SD_CARD_DATABLOCK_SIZE_1024B,
    SD_CARD_DATABLOCK_SIZE_2048B,
    SD_CARD_DATABLOCK_SIZE_4096B,
    SD_CARD_DATABLOCK_SIZE_8192B,
    SD_CARD_DATABLOCK_SIZE_16384B,
};

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class CSD_CARD_Interface
{
    public:

        virtual SystemState_e       GetResponse             (uint32_t* pResponse)                                                                = 0;
        virtual uint32_t            GetTransfertStatus      (void)                                                                               = 0;
        virtual bool                IsFlagSet               (uint32_t Status, SD_StatusFlag_e Flag)                                              = 0;
        virtual void                PowerOn                 (void)                                                                               = 0;
        virtual void                PowerOff                (void)                                                                               = 0;
        virtual void                TickHook                (void)                                                                               = 0;
        virtual void                TransmitCommand         (uint8_t Command, uint32_t Argument, ResponseType_e ResponseType)                    = 0;
        virtual void                SetSpeed                (DeviceSpeed_e DeviceSpeed)                                                          = 0;
        virtual void                SetBusWidth             (BusSize_e BusSize)                                                                  = 0;
        virtual void                StartBlockTransfert     (const uint8_t *pBuffer, size_t Size, SD_TransferType_e TransfertDir)                = 0;
        virtual SystemState_e       WaitBlockTransfertEnd   (void)                                                                               = 0;
        virtual SystemState_e       FindSCR                 (uint32_t RCA, uint32_t* SCR)                                                        = 0;
        virtual SystemState_e       SendCommand             (uint8_t Command, uint32_t Argument, ResponseType_e ResponseType, uint32_t* pBuffer) = 0;
        virtual DRESULT             Read                    (uint8_t* pBuffer, uint32_t Sector, uint8_t NumberOfBlocks)                          = 0;
};

//-------------------------------------------------------------------------------------------------

