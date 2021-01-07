//-------------------------------------------------------------------------------------------------
//
//  File : lib_template_lcd_driver.h
//
//-------------------------------------------------------------------------------------------------

#ifndef __LIB_TEMPLATE_LCD_DRIVER__
#define __LIB_TEMPLATE_LCD_DRIVER__

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// -
// - This LCD generic driver configuration
// -

#define GRAFX_NUMBER_OF_ACTIVE_LAYER              1 - x                 // How many layer the LCD or virtual screen support
#define GRAFX_PIXEL_FORMAT_COUNT                  11                    // how many colour format are supported

#define GRAFX_USE_V_SYNC                          // We use this driver hardware support V Sync

//#define GRAFX_USE_SOFT_PIXEL                    // if commented, we use this driver DMA for this function
//#define GRAFX_USE_SOFT_BOX                      // if commented, we use this driver function to draw a box (rectangle)
//#define GRAFX_USE_SOFT_VLINE                    // if commented, we use this driver function to draw a vertical line
//#define GRAFX_USE_SOFT_HLINE                    // if commented, we use this driver function to draw a horizontal line
#define GRAFX_USE_SOFT_DLINE                      // if commented, we use this driver function to draw a diagonal line
#define GRAFX_USE_SOFT_CIRCLE                     // if commented, we use this driver function to draw a circle line

//#define GRAFX_USE_SOFT_PRINT_FONT               // we use this driver to print FONT using DMA2D
//#define GRAFX_USE_SOFT_ALPHA                    // We use this uP + LCD controller has alpha acceleration
//#define GRAFX_USE_SOFT_FILL                     // We use this driver DMA for this function

//-------------------------------------------------------------------------------------------------
// -
// - This LCD generic configuration
// -

// Display size
#define GRAFX_SIZE_X                                480 // example
#define GRAFX_SIZE_Y                                272 // example

//-------------------------------------------------------------------------------------------------
// -
// - This LCD versus processor configuration (very specific to this combination, not digini )
// -

// Put here driver specific to the LCD and processor combination

//-------------------------------------------------------------------------------------------------
// const(s)
//-------------------------------------------------------------------------------------------------


static bool bDriverInitialize = false;

// -------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

#endif // __LIB_TEMPLATE_LCD_DRIVER__
