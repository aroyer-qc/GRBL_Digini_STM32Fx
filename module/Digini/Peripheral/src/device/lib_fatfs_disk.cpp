//-------------------------------------------------------------------------------------------------
//
//  File : lib_fatfs_disk.cpp
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

#define LIB_FATFS_DISK_GLOBAL
#include "lib_fatfs_disk.h"
#undef LIB_FATFS_DISK_GLOBAL
#include "string.h"
#include "ff.h"


static FATFS fso;

//-------------------------------------------------------------------------------------------------
// Private Function(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Function name: FATFS_DISK_GetVolume
//
//   Parameter(s):  DiskMedia_e  Device
//
//   Return Value:  char *          Volume name
//
//   Description:   Returns the volume name for media type as a string
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
static char* FATFS_DISK_GetVolume(DiskMedia_e Device)
{

  // snprintf();

  // precreate the name list with x-Macro or something else
//   if(Device == DISK_SD_CARD) return "0:":
//   if(Device == DISK_USB_KEY) return "1:";
   return nullptr;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: FATFS_DISK_CheckMedia
//
//   Parameter(s):  DiskMedia_e Device       Media device
//
//   Return Value:  true on success, otherwise false
//
//   Description:   Check if media exists
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
bool FATFS_DISK_CheckMedia(DiskMedia_e Device)
{
  #ifdef DIGINI_FATFS_USE_SD_CARD
    if(Device == DISK_SDCARD)
    {

        // TODO use class with interface for this
        return (SDCard_CheckMedia() == SD_PRESENT) ? true : false;
    }
  #endif

  #ifdef DIGINI_FATFS_USE_USB_KEY
    if(Device == DISK_USB_KEY)
    {
    //    return (USB_KEY_CheckMedia() == USB_PRESENT) ? true : false;
    }
  #endif

    return false;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: FATFS_DISK_Mount
//
//   Parameter(s):  DiskMedia_e Device       Media device
//
//   Return Value:  true on success, otherwise false
//
//   Description:   Mount media device
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
bool FATFS_DISK_Mount(DiskMedia_e Device)
{
    uint32_t FreeClusters;
    FATFS*   fs;
    char*    Volume;

    Volume = FATFS_DISK_GetVolume(Device);

    if((f_mount(&fso, Volume, 1) == FR_OK) && (f_getfree(Volume, &FreeClusters, &fs) == FR_OK))
    {
        return true;
    }

    return false;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: FATFS_DISK_Unmount
//
//   Parameter(s):  DiskMedia_e Device       Media device
//
//   Return Value:  true on success, otherwise false
//
//   Description:   Unmount media device
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
bool FATFS_DISK_Unmount(DiskMedia_e Device)
{
    if(f_mount(NULL, FATFS_DISK_GetVolume(Device), 1) == FR_OK)
    {
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
