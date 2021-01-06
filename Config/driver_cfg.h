//-------------------------------------------------------------------------------------------------
//
//  File : driver_cfg.h
//
//-------------------------------------------------------------------------------------------------

#include "bsp.h"

// Driver to use with this board

// For the LCD
#include "lib_STM32F7_lcd_480x272 - RK043FN48H.h"
#define GRAFX_DRIVER_ARGUMENT                   nullptr             	// this driver does not have argument (ex. pointer on structure and/or peripheral)


#ifdef DIGINI_USE_POINTING_DEVICE
    #define GRAFX_PDI_HARD_INTERFACE            (void*)&myI2C_Control   	// For the pointing device interface
    //#define GRAFX_PDI_INTERRUPT_IO                                	// not available on DISCO746
    #define DIGINI_USE_PDI_MULTI_EVENT                              	// Set Multi-touch as supported
    #include "lib_class_i2c_FT5336.h"
#endif

// #ifdef do same thing with IRQ line for SD Detect
    #define DIGINI_SD_CARD_DETECT_IO            DETECT_SD_CARD
//#endif

// For the Audio
//#include "lib_class_audio_codec_WM8994.h"
//#define DIGINI_AUDIO_DRIVER_ARG_CTRL            (void*)&myI2C_Control    // For the audio device interface control
//#define DIGINI_AUDIO_DRIVER_ARG_DATA            (void*)&mySAI    		// For the audio device interface data

