//-------------------------------------------------------------------------------------------------
//
//  File : lib_LPC2xxx_240x320 - SF-TC240-9370.h
//
//*************************************************************************************************

#ifndef __LIB_LPC2XXX_LCD_240X320__
#define __LIB_LPC2XXX_LCD_240X320__

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "lpc2xxx.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// -
// - This LCD generic driver configuration
// -

#define GRAFX_NUMBER_OF_ACTIVE_LAYER              2
#define GRAFX_PIXEL_FORMAT_COUNT                  11

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

//-------------------------------------------------------------------------------------------------
// -
// - This LCD generic configuration
// -

// Display size
#define GRAFX_SIZE_X                                240
#define GRAFX_SIZE_Y                                320

//-------------------------------------------------------------------------------------------------
// -
// - This LCD versus processor configuration (very specific to this combination, not digini )
// -

// Horizontal configuration
#define GRAFX_HSYNC                                 9
#define GRAFX_HBP                                   20
#define GRAFX_HFP                                   10

// Vertical configuration
#define GRAFX_VSYNC                                 1
#define GRAFX_VBP                                   2
#define GRAFX_VFP                                   4

// LTDC define missing in ST Lib
//#define LTDC_Pixelformat_L4     8
//#define LTDC_Pixelformat_A8     9
//#define LTDC_Pixelformat_A4     10

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

#endif // __LIB_LPC2xxx_LCD_240X320__
