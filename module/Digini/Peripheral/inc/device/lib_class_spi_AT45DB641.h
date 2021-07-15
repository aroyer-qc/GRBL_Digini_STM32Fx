//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_spiAT45DB641.h
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

#ifdef SPI_FLASH_GLOBAL
    #define SPI_FLASH_EXTERN
#else
    #define SPI_FLASH_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_class_STM32F4_gpio.h"
#include "lib_class_STM32F4_spi.h"
#include "diskio.h"
#include "lib_define.h"
#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define AT45DB641_ID                                        0x1F280001

#define SPI_FLASH_TICK_PERIOD                               1
#define SPI_FLASH_USE_SOFTWARE_CHIP_SELECT_CONTROL          1
#define SPI_FLASH_USE_RESET                                 1
#define SPI_FLASH_USE_WRITE_PROTECT                         1

#define SPI_FLASH_AT45DB641_ID                              (uint8_t) 0x9F
#define SPI_FLASH_AT45DB641_SOFTWARE_RESET                  (uint32_t)0xF0000000
#define SPI_FLASH_AT45DB641_CHANGE_PAGE_SIZE_TO_256         (uint32_t)0x3D2A80A6
#define SPI_FLASH_AT45DB641_DISABLE_SECTOR_PROTECTION       (uint32_t)0x3D2A7F9A
#define SPI_FLASH_AT45DB641_DISABLE_SECTOR_LOCKDOWN         (uint32_t)0x3455AA40


#define SPI_FLASH_AT45DB641_BLOCK_ERASE                     (uint32_t)0x50000000
#define SPI_FLASH_AT45DB641_PAGE_ERASE                      (uint32_t)0x81000000
#define SPI_FLASH_AT45DB641_SECTOR_ERASE                    (uint32_t)0x7C000000
#define SPI_FLASH_AT45DB641_CHIP_ERASE                      (uint32_t)0xC794809A
#define SPI_FLASH_AT45DB641_STREAM_READ_HS                  (uint8_t) 0x1B
#define SPI_FLASH_AT45DB641_WRITE_1_TO_MEMORY               (uint32_t)0x82000000
#define SPI_FLASH_AT45DB641_WRITE_2_TO_MEMORY               (uint32_t)0x85000000
#define SPI_FLASH_AT45DB641_STATUS                          (uint8_t) 0xD7

#define SPI_FLASH_AT45DB641_BUSY_MASK                       (uint16_t)0x8000
#define SPI_FLASH_AT45DB641_DENSITY_MASK                    (uint16_t(0x3C00
#define SPI_FLASH_AT45DB641_SECTOR_PROTECTION_MASK          (uint16_t)0x0200
#define SPI_FLASH_AT45DB641_PAGE_SIZE_MASK                  (uint16_t)0x0100
#define SPI_FLASH_AT45DB641_ERROR_ERASE_PROGRAM_MASK        (uint16_t)0x0020
#define SPI_FLASH_AT45DB641_SECTOR_LOCKDOWN_MASK            (uint16_t)0x0008
#define SPI_FLASH_AT45DB641_INIT_MASK                       (uint16_t)(SPI_FLASH_AT45DB641_SECTOR_LOCKDOWN_MASK    | \
                                                                       SPI_FLASH_AT45DB641_SECTOR_PROTECTION_MASK  | \
                                                                       SPI_FLASH_AT45DB641_DENSITY_MASK            | \
                                                                       SPI_FLASH_AT45DB641_PAGE_SIZE_MASK)
#define SPI_FLASH_AT45DB641_INIT_READY_MASK                 (uint16_t)(SPI_FLASH_AT45DB641_DENSITY_MASK            | \
                                                                       SPI_FLASH_AT45DB641_PAGE_SIZE_MASK)

#define SPI_FLASH_RETRY_TIMER                               (20 / SPI_FLASH_TICK_PERIOD)

#define SPI_FLASH_PAGE_SIZE                                    256
#define SPI_FLASH_PAGE_COUNT                                32768

#define SPI_FLASH_PAGE_ERASE_SIZE                            (256)
#define SPI_FLASH_BLOCK_ERASE_SIZE                            (2048)
#define SPI_FLASH_PAGE_IN_BLOCK                             SPI_FLASH_BLOCK_ERASE_SIZE / SPI_FLASH_PAGE_ERASE_SIZE
#define SPI_FLASH_CHIP_ERASE_SIZE                            ((64/8)*1024*1024)

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------
class CSPI_FLash
{
    public:

                                    CSPI_FLash                   (CSPI* pSPI, eIO_Output ChipSelect, eIO_Output ChipReset, eIO_Output ChipWriteProtect);
                                    ~CSPI_FLash                ();

        DSTATUS                     Initialize              ();
        DSTATUS                     Status                  ();
        DRESULT                     Read                    (uint8_t* pBuffer, uint32_t Sector, uint8_t Count);
        DRESULT                     Write                   (const uint8_t* pBuffer, uint32_t Sector, uint8_t Count);
        DRESULT                     IO_Control              (uint8_t Control, void *pBuffer);
        bool                        IsOperational           ();
        void                        TickHook                ();

    private:

        void                        ChipSelect              (eChipSelect Select);
        eSystemState                WaitReadyAndChipSelect  (eChipSelect Select);
        eSystemState                Erase                   (uint32_t Command);
        uint16_t                    GetLastChipStatus       ()                              { return m_ChipStatus; }

       #if SPI_FLASH_USE_RESET == 1
        void                        ChipReset               (eChipReset Reset)              { IO_Output(m_ChipReset, (Reset == CHIP_RESET) ? IO_SET : IO_RESET); }
       #else
        void                        ChipReset               (eChipReset Reset)              { VAR_UNUSED(Reset); }
       #endif

       #if SPI_FLASH_USE_WRITE_PROTECT == 1
        void                        WriteProtect            (eWritePermission Permission)  { IO_Output(m_ChipWriteProtect, (Permission == WRITE_ALLOWED) ? IO_RESET : IO_SET); }
       #else
        void                        WriteProtect            (eWritePermission Permission)  { VAR_UNUSED(Permission); }
       #endif


        CSPI*                       m_pSPI;
        SPI_sDeviceInfo*            m_pDevice;
        eIO_Output                  m_ChipSelect;
        eIO_Output                  m_ChipReset;
        eIO_Output                  m_ChipWriteProtect;
        uint8_t                     m_TickPeriod;
        eSystemState                m_Status;
        uint16_t                    m_ChipStatus;
        volatile uint32_t           m_TimeOut;
};

//-------------------------------------------------------------------------------------------------




