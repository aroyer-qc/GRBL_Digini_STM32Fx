//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_fatfs_usb.cpp
//
//*************************************************************************************************

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_FATFS_USE_USB_KEY
#include <stdint.h>
#define STM32F4_USB_GLOBAL
#include "lib_class_STM32F4_fatfs_usb.h"
#undef  STM32F4_USB_GLOBAL
#include "lib_class_usb.h"
#include "usb_hcd.h"

//-------------------------------------------------------------------------------------------------
//
//   Class: CFatFS_USB
//
//
//   Description:   Class to handle FatFS for device connected to USB
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CFatFS_USB
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Initializes the USB Host peripheral
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CFatFS_USB::CFatFS_USB(void)
{
}


//-------------------------------------------------------------------------------------------------
//
//   Destructor:   CFatFS_USB
//
//   Parameter(s):
//   Return Value:
//
//  Description:    Deinitializes the USB_Host
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
CFatFS_USB::~CFatFS_USB(void)
{
	// Nothing to do at this point
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
DSTATUS CFatFS_USB::Initialize(void)
{
    return this->Status();
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
DSTATUS CFatFS_USB::Status()
{


    if(USB.GetStatus() == USB_MSC_DEV_CONNECTED)
    {
        m_Status = (DSTATUS)(m_Status & ~STA_NOINIT);
    }
    else
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
DRESULT CFatFS_USB::Read(uint8_t* pBuffer, uint32_t Sector, uint8_t NumberOfBlocks)
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

    if(USB.GetStatus() == USB_MSC_DEV_CONNECTED)
    {
        do
        {
            Status = USBH_MSC_Read10(&USB.m_OTG_Core, pBuffer, Sector, 512 * NumberOfBlocks);
            USBH_MSC_HandleBOTXfer(&USB.m_OTG_Core, &USB.m_Host);

            if(HCD_IsDeviceConnected(&USB.m_OTG_Core) == 0)
            {
                return RES_ERROR;
            }
        }
        while(Status == USBH_MSC_BUSY);

        if(Status == USBH_MSC_OK)
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
DRESULT CFatFS_USB::Write(const uint8_t* pBuffer, uint32_t Sector, uint8_t NumberOfBlocks)
{
	uint8_t Status = USBH_MSC_OK;

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

    if(USB.GetStatus() == USB_MSC_DEV_CONNECTED)
	{
		do
		{
			Status = USBH_MSC_Write10(&USB.m_OTG_Core, (BYTE*)pBuffer, Sector, 512 * NumberOfBlocks);
			USBH_MSC_HandleBOTXfer(&USB.m_OTG_Core, &USB.m_Host);

			if(HCD_IsDeviceConnected(&USB.m_OTG_Core) == 0)
			{
				return RES_ERROR;
			}
		}
		while(Status == USBH_MSC_BUSY);

        if(Status == USBH_MSC_OK)
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
//   Parameter(s):  uint8_t    Control		Control code
//	                void*      pBuffer		Buffer to send/receive control data
//   Return value:  DRESULT
//
//   Description:   Control
//
//   Note(s):
//
//   ----------------------------------------------------------------------------------------------
//   date           author              description
//   -------------  ------------------  -----------------------------------------------------------
//   Nov  13, 2015  Alain Royer         New code from MCD
//
//-------------------------------------------------------------------------------------------------
#if _USE_IOCTL == 1
DRESULT CFatFS_USB::IO_Control(uint8_t Control, void *pBuffer)
{
	DRESULT res = RES_ERROR;

	if(m_Status & STA_NOINIT)
	{
		return RES_NOTRDY;
	}

	switch(Control)
	{
		case CTRL_SYNC:											// Make sure that no pending write process
		{
			res = RES_OK;
			break;
		}

		case GET_SECTOR_COUNT:									// Get number of sectors on the disk (unit32_t)
		{
			*(uint32_t*)pBuffer = (uint32_t)USBH_MSC_Param.MSCapacity;
			res = RES_OK;
			break;
		}

		case GET_SECTOR_SIZE:									// Get R/W sector size (unit16_t)
		{
			*(uint16_t*)pBuffer = 512;
			res = RES_OK;
			break;
		}

		case GET_BLOCK_SIZE:									// Get erase block size in unit of sector (unit32_t)
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


