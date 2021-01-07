//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F4_lcd_240x320.h
//
//*************************************************************************************************

#ifndef __LIB_STM32F4_LCD_240X320__
#define __LIB_STM32F4_LCD_240X320__

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "stm32f4xx.h"
#include "lib_class_STM32F4_spi.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define GRAFX_NUMBER_OF_ACTIVE_LAYER              2

#define GRAFX_USE_V_SYNC                          // We use this driver hardware support V Sync

//#define GRAFX_USE_SOFT_PIXEL                    // We use this driver DMA for this function
//#define GRAFX_USE_SOFT_BOX                      // We use this driver function
//#define GRAFX_USE_SOFT_VLINE
//#define GRAFX_USE_SOFT_HLINE
#define GRAFX_USE_SOFT_DLINE
#define GRAFX_USE_SOFT_CIRCLE

//#define GRAFX_USE_SOFT_PRINT_FONT               // we use this driver to print FONT using DMA2D
//#define GRAFX_USE_SOFT_ALPHA                    // We use this uP + LCD controller has alpha acceleration
//#define GRAFX_USE_SOFT_FILL                     // We use this driver DMA for this function


// Display size
#define GRAFX_SIZE_X                                240
#define GRAFX_SIZE_Y                                320

//-------------------------------------------------------------------------------------------------
// struct(s)
//-------------------------------------------------------------------------------------------------

struct DRV_Info_t
{
    CSPI*               pSPI;
    SPI_DeviceInfo_t*   pDeviceInfo;
};

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

#endif // __LIB_STM32F4_LCD_240X320__
