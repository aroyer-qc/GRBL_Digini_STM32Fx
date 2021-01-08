//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_spi_sd_card.h
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

#ifdef SPI_SD_CARD_GLOBAL
    #define SPI_SD_CARD_EXTERN
#else
    #define SPI_SD_CARD_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "STM32F4_SPI.h"
#include "diskio.h"

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class CSPI_SD_Card : public CSPI
{
    public:

                                    CSPI_SD_Card               (SPI_PortInfo_t* pPort);
                                    ~CSPI_SD_Card           ();

        DSTATUS                     Initialize              ();
        DSTATUS                     Status                  ();
        DRESULT                     Read                    (uint8_t* pBuffer, uint32_t Sector, uint8_t Count);
        DRESULT                     Write                   (const uint8_t* pBuffer, uint32_t Sector, uint8_t Count);
        DRESULT                     IO_Control              (uint8_t Control, void *pBuffer);
        bool                        IsOperational           ();
        void                        TickHook                ();
        uint8_t                     SendCommand             (uint8_t Command, uint32_t Argument, uint16_t *pRCA);

    private:

        void                        ChipSelect              (ChipSelect_e Select);
        SystemState_e               WaitReadyAndChipSelect  (ChipSelect_e Select);
        SystemState_e               Erase                   (uint32_t Command);
        uint16_t                    GetLastChipStatus       ()                              { return m_ChipStatus; }

        uint8_t                     WaitReady               ();

        uint8_t                     m_TickPeriod;
        volatile uint32_t           m_Timer1;
        volatile uint32_t           m_Timer2;
};

//-------------------------------------------------------------------------------------------------




