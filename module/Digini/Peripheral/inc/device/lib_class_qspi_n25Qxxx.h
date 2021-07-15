//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_qspi_N25Qxxx.h
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

#ifdef QSPI_FLASH_GLOBAL
    #define QSPI_FLASH_EXTERN
#else
    #define QSPI_FLASH_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

//#include "lib_class_STM32F7_qspi.h"
//#include "lib_RTOS.h"
//#include "diskio.h"
//#include "lib_define.h"
//#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class N25Qxx
{
    public:

                                    N25Qxx                       ();
                                    ~N25Qxx                    ();

        SystemState_e               Initialize              (void* pArg)
        SystemState_e               Read                    (uint8_t* pData, uint32_t ReadAddress, uint32_t Size);
        SystemState_e               Write                   (uint8_t* pData, uint32_t WriteAddress, uint32_t Size);

    private:

        CQSPI*                      m_pQSPI;
        SPI_sDeviceInfo*            m_pDevice;
        eSystemState                m_Status;
        uint16_t                    m_ChipStatus;
};

//-------------------------------------------------------------------------------------------------




