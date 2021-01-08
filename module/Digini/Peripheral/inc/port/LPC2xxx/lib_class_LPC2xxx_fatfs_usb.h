// lib_class_STM32F4_fatfs_usb.h

#ifndef __STM32F4_FATFS_USB_H__
#define __STM32F4_FATFS_USB_H__

#include "diskio.h"


class CFatFS_USB
{
    public:

                    CFatFS_USB          (void);
                   ~CFatFS_USB          (void);

        DSTATUS     Initialize          (void);
        DSTATUS     Status              (void);
        DRESULT     Read                (uint8_t* pBuffer, uint32_t Sector, uint8_t NumberOfBlocks);
        DRESULT     Write               (const uint8_t* pBuffer, uint32_t Sector, uint8_t NumberOfBlocks);
        #if _USE_IOCTL == 1
        DRESULT     IO_Control          (uint8_t Control, void *pBuffer);
        #endif

    private:

        DSTATUS      m_Status;
};


#endif
