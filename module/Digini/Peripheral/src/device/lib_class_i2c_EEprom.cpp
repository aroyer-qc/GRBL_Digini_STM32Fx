//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_I2C_EEprom.cpp
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

#include <stdint.h>
#include "lib_macro.h"
#include "lib_class_i2c.h"
#define EEPROM_DRIVER_GLOBAL
#include "lib_class_i2c_EEprom.h"
#undef  EEPROM_DRIVER_GLOBAL

//-------------------------------------------------------------------------------------------------

#if USE_I2C_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define EEPROM_SIZE_LIMIT_64K          0xFFFF

//-------------------------------------------------------------------------------------------------
//
//   Class: EEPROM_Driver
//
//
//   Description:    Class to handle eeprom 24xxx memory
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   EEPROM_Driver
//
//   Parameter(s):
//
//   Description:   Initializes the EEprom object
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
EEPROM_Driver::EEPROM_Driver(const EEPROM_Info_t* pInfo)
{
    m_pInfo                 = pInfo;
    m_PageSize              = (pInfo->DeviceSize / pInfo->DeviceNbOfPage);
    m_PageMask              = m_PageSize - 1;
    m_DeviceAddressingSize  = (pInfo->DeviceSize > EEPROM_SIZE_LIMIT_64K) ? 3 : 2;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Read
//
//  Parameter(s):   uint32_t   Address
//                  void*      pDest
//                  size_t     Size
//  Return:         uint8_t    Status
//
//  Description:    Read data block from internal E2 device.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e EEPROM_Driver::Read(uint32_t Address, void* pDest, size_t Size)
{
    return m_pInfo->pI2C->Transfer(Address,
                                   m_DeviceAddressingSize,
                                   0,
                                   0,
                                   pDest,
                                   Size,
                                   m_pInfo->DeviceAddress);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Write
//
//  Parameter(s):   uint32_t     Address    Address in E2 where to read data
//                  const void*  pSrc       Source data buffer containing data to write
//                  size_t       Size       How much data we want to write
//  Return:         uint8_t      Status
//
//  Description:    Writes a data block to internal E2
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e EEPROM_Driver::Write(uint32_t Address, const void* pSrc, size_t Size)
{
    SystemState_e State;
    uint8_t*      pBuffer;
    uint16_t      BytesToWrite;

    pBuffer = (uint8_t*)pSrc;

    if((State = m_pInfo->pI2C->LockToDevice(m_pInfo->DeviceAddress)) == SYS_READY)
    {
        while((Size != 0) && (State == SYS_READY))                                  // Loop for the number of page we need to write
        {
            BytesToWrite = m_PageSize - (Address & m_PageMask);                     // Get the number of byte to write in this pass

            if(Size < BytesToWrite)
            {
                BytesToWrite = Size;
            }

            State =  m_pInfo->pI2C->Transfer(Address,
                                             m_DeviceAddressingSize,
                                             pSrc,
                                             Size,
                                             0,
                                             0);                                    // Write

            //BSP_Delay(10);

           // if(OSRunning == OS_TRUE)
           // {
           //     OSTimeDly(1);                                                     // If OS is started we sleep to give other task runtime
           // }
           // else
           // {
           //     BSP_Delay1mS(5);
           // }

            Size    -= BytesToWrite;                                                // Remove the count from the total
            Address += BytesToWrite;                                                // Update address for the next pass
            pBuffer += BytesToWrite;                                                // Update source buffer pointer for the next pass
        }

        m_pInfo->pI2C->UnlockFromDevice(m_pInfo->DeviceAddress);                             // No State check, we know we have the lock
    }

    return State;
}

//-------------------------------------------------------------------------------------------------

#endif // USE_I2C_DRIVER == DEF_ENABLED
