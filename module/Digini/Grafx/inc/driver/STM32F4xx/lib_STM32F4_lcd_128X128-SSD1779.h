//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F4_lcd_128x128-SSD1779.h
//
//*************************************************************************************************

#ifndef __LIB_STM32F4_LCD_128X128_SSD1779__
#define __LIB_STM32F4_LCD_128X128_SSD1779__

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include <lib_lcd_driver.h>
#include "stm32f4xx.h"
#include "lib_class_io.h"
#include "lib_class_io_bus.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define GRAFX_NUMBER_OF_ACTIVE_LAYER              2

//#define GRAFX_USE_V_SYNC
//#define GRAFX_USE_SOFT_PIXEL
//#define GRAFX_USE_SOFT_BOX
//#define GRAFX_USE_SOFT_VLINE
//#define GRAFX_USE_SOFT_HLINE
//#define GRAFX_USE_SOFT_DLINE
//#define GRAFX_USE_SOFT_CIRCLE

//#define GRAFX_USE_SOFT_PRINT_FONT
#define GRAFX_USE_SOFT_ALPHA
//#define GRAFX_USE_SOFT_FILL

// Display size
#define GRAFX_SIZE_X            128
#define GRAFX_SIZE_Y            128

#define SSD1779_NO_FILL         0
#define SSD1779_FILL            1

//-------------------------------------------------------------------------------------------------
// Class
//-------------------------------------------------------------------------------------------------

class LCD_Driver : public GRAFX_Interface
{
    public:

        void            Initialize              (void* pArg);
        void            DisplayOn               (void);
        void            DisplayOff              (void);
        void            LayerConfig             (CLayer* pLayer);
        void            CopyLinear              (void* pSrc, Box_t* pBox, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);
        void            BlockCopy               (void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);
        void            DrawRectangle           (Box_t* pBox);
        void            PrintFont               (FontDescriptor_t* pDescriptor, Cartesian_t* pPos);
        void            DrawBox                 (uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Height, uint16_t Thickness);
        void            DrawPixel               (uint16_t PosX, uint16_t PosY);
        void            DrawHLine               (uint16_t PosY, uint16_t PosX1, uint16_t PosX2, uint16_t ThickNess);
        void            DrawVLine               (uint16_t PosX, uint16_t PosY1, uint16_t PosY2, uint16_t ThickNess);
        void            DrawLine                (uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Thickness, DrawMode_e Direction);
        void            DrawCircle              (uint8_t X, uint8_t Y, uint8_t Radius, uint8_t Mode);
        void            Copy                    (void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat_e, BlendMode_e BlendMode);

        #ifdef GRAFX_USE_V_SYNC
        void            WaitFor_V_Sync          (void);
        #endif

    private:

        void            ControllerInitialize    (void);     // LCD is equipped with a IL9341 controller
        void            WriteCommand            (uint8_t Register);
        void            WriteData               (uint8_t Data);
        void            WriteData               (uint16_t Data);
        void            Line                    (uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t ThickNess, DrawMode_e Direction);
        void            DrawRectangle           (Box_t* pBox, uint8_t Mode = SSD1779_NO_FILL);
        void             Clear                    (void);

        IO_PinDriver    m_RegSelect;
        IO_PinDriver    m_Reset;
        IO_BusDriver    m_Bus;
        CLayer*         m_pLayer;
};

//-------------------------------------------------------------------------------------------------

#ifdef LCD_DRIVER_GLOBAL
class LCD_Driver              LCD_Driver;
class GRAFX_Interface*        myGrafx = &LCD_Driver;
#else
extern class LCD_Driver       myLCD_Driver;
extern class GRAFX_Interface* myGrafx;
#endif

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

#endif // __LIB_STM32F4_LCD_128X128_SSD1779__
