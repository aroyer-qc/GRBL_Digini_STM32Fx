//-------------------------------------------------------------------------------------------------
//
//  File : lib_LPC2xxx_lcd_480x272.h
//
//*************************************************************************************************

#ifndef __LIB_LPC2XXX_LCD_480X272__
#define __LIB_LPC2XXX_LCD_480X272__

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "stm32f4xx.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------
//#define C_GLCD_REFRESH_FREQ	50
//#define C_GLCD_H_PULSE	2
//#define C_GLCD_V_PULSE 2

//-------------------------------------------------------------------------------------------------
// -
// - This LCD generic driver configuration
// -

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

//-------------------------------------------------------------------------------------------------
// -
// - This LCD generic configuration
// -

// Display size
#define GRAFX_SIZE_X                                480
#define GRAFX_SIZE_Y                                272

//-------------------------------------------------------------------------------------------------
// -
// - This LCD versus processor configuration (very specific to this combination, not digini )
// -

// Horizontal configuration
#define GRAFX_HSYNC                                 40
#define GRAFX_HBP                                   40	// from internet
#define GRAFX_HFP                                   10  // from internet

// Vertical configuration
#define GRAFX_VSYNC                                 9
#define GRAFX_VBP                                   4  // from internet
#define GRAFX_VFP                                   2  // from internet


// TODO (Alain#1#): yep do something about this.... maybe a hook
// TO DO put this in XMACRO or BSP file!!!!   God dawn it !!!
#define LCD_BL_CTRL_PORT                            GPIOA
#define LCD_BL_CTRL_PIN                             GPIO_Pin_8

//-------------------------------------------------------------------------------------------------
// enum(s)
//-------------------------------------------------------------------------------------------------

enum PixelFormatTable_e
{
    PIXEL_FORMAT_ARGB8888,
    PIXEL_FORMAT_RGB888,
    PIXEL_FORMAT_RGB565,
    PIXEL_FORMAT_ARGB1555,
    PIXEL_FORMAT_ARGB4444,
    PIXEL_FORMAT_L8,
    PIXEL_FORMAT_AL44,
    PIXEL_FORMAT_AL88,
    PIXEL_FORMAT_L4,
    PIXEL_FORMAT_A8,
    PIXEL_FORMAT_A4,
    GRAFX_PIXEL_FORMAT_COUNT,
};

//-------------------------------------------------------------------------------------------------
// const(s)
//-------------------------------------------------------------------------------------------------

const LTDC_Layer_TypeDef* LTDC_baLayer[GRAFX_NUMBER_OF_ACTIVE_LAYER] =
{
    LTDC_Layer1,
    LTDC_Layer2
};

static bool bDriverInitialize = false;

// -------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

#endif // __LIB_LPC2XXX_LCD_480X272__
