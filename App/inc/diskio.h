//-------------------------------------------------------------------------------------------------
//
//  File : diskio.h
//
//-------------------------------------------------------------------------------------------------

#pragma once

#ifdef DISKIO_GLOBAL
    #define DISKIO_EXTERN
#else
    #define DISKIO_EXTERN      extern
#endif

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "integer.h"
#include "stdint.h"
#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define _USE_WRITE                      1       // 1: Enable disk_write function
#define _USE_IOCTL                      1

// Command code for disk_ioctrl()

// Generic command
#define CTRL_SYNC                       0       // Mandatory for write functions
#define GET_SECTOR_COUNT                1       // Mandatory for only f_mkfs()
#define GET_SECTOR_SIZE                 2
#define GET_BLOCK_SIZE                  3       // Mandatory for only f_mkfs()
#define CTRL_ERASE_SECTOR               4        // Force erased a block of sectors (for only _USE_ERASE)

//-------------------------------------------------------------------------------------------------
// Type definition(s) and structure(s)
//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

typedef enum __DiskMedia_e
{
    DISK_SDIO_SD_CARD,
    DISK_USB_KEY,
    NUMBER_OF_DISK
} DiskMedia_e;

// Status of Disk Functions
typedef enum SystemState_e    SystemState_e;
typedef SystemState_e         DSTATUS;

// Results of Disk Functions
typedef enum
{
    RES_OK = 0,             // 0: Successful
    RES_ERROR,              // 1: R/W Error
    RES_WRPRT,              // 2: Write Protected
    RES_NOTRDY,             // 3: Not Ready
    RES_PARERR              // 4: Invalid Parameter
} DRESULT;

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

void                FatFS_Initialize        (void);

#ifdef __cplusplus
extern "C" {
#endif

// Prototypes for disk control functions
DSTATUS             disk_initialize         (uint8_t);
DSTATUS             disk_status             (uint8_t);
DRESULT             disk_read               (uint8_t, uint8_t*, uint32_t, uint16_t);
#if _USE_WRITE == 1
DRESULT             disk_write              (uint8_t, const uint8_t*, uint32_t, uint16_t);
#endif
DRESULT             disk_ioctl              (uint8_t, uint8_t, void*);
uint32_t            get_fattime             ();

WCHAR               ff_convert              (WCHAR wch, UINT dir);
WCHAR               ff_wtoupper             (WCHAR wch);

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------------------------------
