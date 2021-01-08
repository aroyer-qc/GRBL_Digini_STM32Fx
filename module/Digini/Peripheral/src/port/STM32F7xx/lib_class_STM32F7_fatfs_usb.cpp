//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_fatfs_usb.cpp
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
#ifdef DIGINI_FATFS_USE_USB_KEY
#include <stdint.h>
#define STM32F7_USB_GLOBAL
#include "lib_class_STM32F7_fatfs_usb.h"
#undef  STM32F7_USB_GLOBAL
//#include "lib_class_usb_interface.h"
//#include "usb_hcd.h"

//-------------------------------------------------------------------------------------------------
//
//   Class: USB_FatFS
//
//
//   Description:   Class to handle FatFS for device connected to USB
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   USB_FatFS
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Initializes the variables
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
USB_FatFS::USB_FatFS(USB_HostInterface* pUSB)
{
    m_pUSB       = pUSB;
    m_Initialize = false;
    m_Status     = STA_NOINIT;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: Initialize
//
//   Parameter(s):  None
//   Return value:  DSTATUS
//
//   Description:   Initialize USB Host
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DSTATUS USB_FatFS::Initialize(void)
{
    return Status();
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: Status
//
//   Parameter(s):  None
//   Return value:  DSTATUS
//
//   Description:   Get Status from USB Device
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DSTATUS USB_FatFS::Status()
{
   // if(USB.GetStatus() == USB_MSC_DEV_CONNECTED)
    {
        m_Status = (DSTATUS)(m_Status & ~STA_NOINIT);
    }
   // else
    {
        m_Status = (DSTATUS)(m_Status |  STA_NOINIT);
    }

    return m_Status;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: Read
//
//   Parameter(s):  uint8_t*  pBuffer
//                  uint32_t  Sector
//                  uint8_t   Count
//   Return value:  DRESULT
//
//   Description:   Read From USB Device
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DRESULT USB_FatFS::Read(uint8_t* pBuffer, uint32_t Sector, uint8_t NumberOfBlocks)
{
    uint8_t Status;

    if(NumberOfBlocks == 0)
    {
        return RES_PARERR;
    }

    if(m_Status & STA_NOINIT)
    {
        return RES_NOTRDY;
    }

   // if(USB.GetStatus() == USB_MSC_DEV_CONNECTED)
    {
//        do
        {
       //     Status = USBH_MSC_Read10(&USB.m_OTG_Core, pBuffer, Sector, 512 * NumberOfBlocks);
       //     USBH_MSC_HandleBOTXfer(&USB.m_OTG_Core, &USB.m_Host);

        //    if(HCD_IsDeviceConnected(&USB.m_OTG_Core) == 0)
            {
                return RES_ERROR;
            }
        }
     //   while(Status == USBH_MSC_BUSY);

      //  if(Status == USBH_MSC_OK)
        {
            return  RES_OK;
        }
    }

    return RES_ERROR;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: Write
//
//   Parameter(s):  const uint8_t*  pBuffer
//                  uint32_t        Sector
//                  uint8_t         NumberOfBlocks
//   Return value:  DRESULT
//
//   Description:   Write to the USB Device
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DRESULT USB_FatFS::Write(const uint8_t* pBuffer, uint32_t Sector, uint8_t NumberOfBlocks)
{
    //uint8_t Status = USBH_MSC_OK;

    if(NumberOfBlocks == 0)
    {
        return RES_PARERR;
    }

    if(m_Status & STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    if(m_Status & STA_PROTECT)
    {
        return RES_WRPRT;
    }

  //  if(USB.GetStatus() == USB_MSC_DEV_CONNECTED)
    {
    //    do
        {
            //Status = USBH_MSC_Write10(&USB.m_OTG_Core, (BYTE*)pBuffer, Sector, 512 * NumberOfBlocks);
        //    USBH_MSC_HandleBOTXfer(&USB.m_OTG_Core, &USB.m_Host);

        //    if(HCD_IsDeviceConnected(&USB.m_OTG_Core) == 0)
            {
                return RES_ERROR;
            }
        }
    //    while(Status == USBH_MSC_BUSY);

     //   if(Status == USBH_MSC_OK)
        {
            return RES_OK;
        }
    }

    return RES_ERROR;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: IO_Control
//
//   Parameter(s):  uint8_t    Control        Control code
//                    void*      pBuffer        Buffer to send/receive control data
//   Return value:  DRESULT
//
//   Description:   Control
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
#if _USE_IOCTL == 1
DRESULT USB_FatFS::IO_Control(uint8_t Control, void *pBuffer)
{
    DRESULT res = RES_ERROR;

    if(m_Status & STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    switch(Control)
    {
        case CTRL_SYNC:                                            // Make sure that no pending write process
        {
            res = RES_OK;
            break;
        }

        case GET_SECTOR_COUNT:                                    // Get number of sectors on the disk (unit32_t)
        {
        //    *(uint32_t*)pBuffer = (uint32_t)USBH_MSC_Param.MSCapacity;
            res = RES_OK;
            break;
        }

        case GET_SECTOR_SIZE:                                    // Get R/W sector size (unit16_t)
        {
            *(uint16_t*)pBuffer = 512;
            res = RES_OK;
            break;
        }

        case GET_BLOCK_SIZE:                                    // Get erase block size in unit of sector (unit32_t)
        {
            *(uint32_t*)pBuffer = 512;
            break;
        }


        default:
        {
            res = RES_PARERR;
        }
    }

    return res;
}
#endif

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_FATFS_USE_USB_KEY


