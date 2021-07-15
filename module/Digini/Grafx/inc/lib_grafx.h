//-------------------------------------------------------------------------------------------------
//
//  File : lib_grafx.h
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

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "digini_cfg.h"

//#ifdef DIGINI_USE_GRAFX
#include "lib_compression.h"
#include "widget_cfg.h"
#include "grafx_cfg.h"
#include "lib_typedef.h"
#include "lib_label.h"
#include "lib_grafx_define.h"
#include "lib_grafx_font.h"
#include "static_skin_image.h"
#include "lib_grafx_skin.h"
#include "lib_grafx_enum_x_macro.h"
#include "menu_link_cfg.h"
#include "lib_grafx_link_enum.h"
#include "lib_grafx_link_list.h"
#include "lib_grafx_page_widget.h"
#include "menu_cfg.h"
#include "lib_grafx_link_const.h"
#include "lib_grafx_enum.h"
#include "lib_grafx_typedef.h"
#include "lib_class_widget.h"
#include "lib_widget_variable.h"
#include "lib_grafx_basic.h"
#include "lib_grafx_color.h"
#include "lib_grafx_const.h"
#include "lib_class_layer.h"
#include "lib_class_font.h"
#include "lib_gui_task.h"
#include "widget_var.h"

#ifdef DIGINI_USE_A_SKIN
  #include "lib_skin_task.h"
#endif

#include "driver_cfg.h"
#include GRAFX_DRIVER_INCLUDE

#ifdef DIGINI_USE_POINTING_DEVICE
  #include "lib_class_pointing_device_interface.h"
  #include "lib_pdi_task.h"
#endif


//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#ifdef GRAFX_USE_DISPLAY_RAM
  #define GFX_BASE_ADDRESS       (uint32_t)&__gfx_display_base__
#endif

#ifdef GRAFX_USE_RAM_DATA
  #define GFX_RAM_ADDRESS       (uint32_t)&__gfx_ram_data_base__
#endif

#ifdef GRAFX_USE_ROM_DATA
  #define GFX_ROM_ADDRESS        (uint32_t)&__gfx_rom_base__
#endif

#ifdef DIGINI_USE_QUAD_SPI_FOR_GRAFX_DATABASE

 #define GFX_QSPI_DBASE_ADDRESS (uint32_t)&__gfx_qspi_data_base__

 #define GFX_RAM_DBASE_DEF  X_GFX_RAM_DBASE(  GFX_FREE_RAM_POINTER,    1,                          1,                           sizeof(void*)               )   \
                            X_GFX_RAM_DBASE(  GFX_FREE_RELOAD_POINTER, 1,                          1,                           sizeof(void*)               )   \

 #define GFX_QSPI_DBASE_DEF X_GFX_QSPI_DBASE( GFX_FONT_INFO,           DBASE_MAX_SKIN_FONT_QTY,    1,                           sizeof(FontInfo_t)          )   \
                            X_GFX_QSPI_DBASE( GFX_FONT_DESC_INFO,      DBASE_MAX_SKIN_FONT_QTY,    FONT_CHARACTER_PER_FONT_MAX, sizeof(FontDescriptor_t)    )   \
                            X_GFX_QSPI_DBASE( GFX_IMAGE_INFO,          DBASE_MAX_SKIN_IMAGE_QTY,   1,                           sizeof(ImageInfo_t)         )   \

#else

 #define GFX_RAM_DBASE_DEF  X_GFX_RAM_DBASE(  GFX_FREE_RAM_POINTER,    1,                          1,                           sizeof(void*)               )   \
                            X_GFX_RAM_DBASE(  GFX_FREE_RELOAD_POINTER, 1,                          1,                           sizeof(void*)               )   \
                            X_GFX_RAM_DBASE(  GFX_FONT_INFO,           DBASE_MAX_SKIN_FONT_QTY,    1,                           sizeof(FontInfo_t)          )   \
                            X_GFX_RAM_DBASE(  GFX_FONT_DESC_INFO,      DBASE_MAX_SKIN_FONT_QTY,    FONT_CHARACTER_PER_FONT_MAX, sizeof(FontDescriptor_t)    )   \
                            X_GFX_RAM_DBASE(  GFX_IMAGE_INFO,          DBASE_MAX_SKIN_IMAGE_QTY,   1,                           sizeof(ImageInfo_t)         )   \

#endif

//-------------------------------------------------------------------------------------------------
// Typedef(s)
//-------------------------------------------------------------------------------------------------

//#if defined(DIGINI_USE_POINTING_DEVICE) /* || defined(DIGINI_USE_XXX) */
struct GRAFX_PostInitSubDriverPtr_t
{
/*
    #if DIGINI_USE_XXX

    #endif // DIGINI_USE_XXX
*/

  #ifdef DIGINI_USE_POINTING_DEVICE
    void*  PDI_pHardInterface;                  // Communication interface for "Pointing Device Interface" ( I2C, SPI, etc...)
  #endif // DIGINI_USE_POINTING_DEVICE

};
//#endif

//-------------------------------------------------------------------------------------------------
// Const(s)
//-------------------------------------------------------------------------------------------------

extern const uint32_t __gfx_display_base__;             // Pointer to beginning RAM for the display

// TODO Change this as we don't always have ram to load data... sometime it is in ROM
#ifdef GRAFX_USE_ROM_DATA
extern const uint32_t __gfx_rom_base__;                 // Pointer to graphic data
#endif

#ifdef GRAFX_USE_RAM_DATA
extern const uint32_t __gfx_ram_data_base__;            // Pointer to ram graphic data
#endif

extern const uint32_t __gfx_ram_layer_base__;           // Pointer on display and/or virtual layer

#ifdef DIGINI_USE_QUAD_SPI_FOR_GRAFX_DATABASE
extern const uint32_t __gfx_qspi_data_base__;           // Pointer to database for skin info
#endif

//-------------------------------------------------------------------------------------------------
// Variable(s)
//--------------------------------------------------------------------------------------------------

#ifdef GRAFX_USE_SOFT_ALPHA
//uint16_t           g_AlphaTableRed[32][32];
//uint16_t           g_AlphaTableGreen[64][64];
//uint16_t           g_AlphaTableBlue[32][32];
#endif


#if defined(DIGINI_USE_POINTING_DEVICE) /* || defined(DIGINI_USE_XXX) */
    extern GRAFX_PostInitSubDriverPtr_t GRAFX_PostInitSubDriverPtr;

 #ifdef GFX_GLOBAL
    GRAFX_PostInitSubDriverPtr_t GRAFX_PostInitSubDriverPtr =
    {
        /*
      #if DIGINI_USE_XXX

      #endif // DIGINI_USE_XXX
        */

      #ifdef DIGINI_USE_POINTING_DEVICE
        GRAFX_PDI_HARD_INTERFACE,
      #endif // DIGINI_USE_POINTING_DEVICE
    };
 #endif
#endif

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

SystemState_e       GRAFX_Initialize            (void);
SystemState_e       GRAFX_PostInitialize        (void);

// Font function
void                PrintFont                   (FontDescriptor_t* pDescriptor, Cartesian_t* pPos);

void                DrawCursorOnCircle          (Skin_e Image, Cartesian_t* pPos, uint16_t Radius, uint16_t Angle);

size_t              WidgetPrint                 (Text_t* pText, ServiceReturn_t* pService);

// Alpha function
void                AlphaBlend                  (void);

// Tools function

Cartesian_t         GetMaxSize                  (void);
bool                BoxValid                    (Box_t* pBox);
void                SetXY_Justification         (uint8_t Justification);
uint8_t             GetXY_Justification         (void);

ServiceReturn_t*    ServiceCall                 (Service_t* pService, ServiceEvent_e* pServiceState);
ServiceReturn_t*    ServiceCallApp              (Service_t* pService, ServiceEvent_e* pServiceState);
ServiceReturn_t*    GetServiceStruct            (ServiceType_e ServiceType);
void                FreeServiceStruct           (ServiceReturn_t** pService);


class GPrintf
{
    public:

        size_t              Draw                      (Box_t* pBox, const char* pFormat, ...);

    private:

        size_t              Draw                      (Box_t* pBox, const char* pFormat, va_list arg);
        size_t              PutString                 (void);
        void                ParseFeature              (void);
        void                ParseString               (void);
        void                IncrementFeaturePointer   (void);

        char                m_String[DIGINI_MAX_PRINT_SIZE];
        Font_e*             m_pMovingUsedFontPtr;                                     // Moving pointer in "font use" string
        Font_e*             m_pFontUsedInString;                                      // "Font use" by the character in string
        uint32_t*           m_pMovingUsedColorPtr;                                    // Moving pointer in "font use" string
        uint32_t*           m_pColorUsedInString;                                     // "Font use" by the character in string
        size_t              m_Size;                                                   // Size of the entire string
        Cartesian_t         m_Position;
        Cartesian_t         m_CorrectedPos;
        FontDescriptor_t    m_FontDescriptor;
        Box_t*              m_pLocalBox;
        uint16_t            m_BoxSizeX;                                               // Final width of the print box
        uint16_t            m_OffsetJustX;
        uint16_t            m_BoxSizeY;                                               // Final height of the print box
        uint16_t            m_OffsetJustY;

        // Multi line support
     // #ifdef DIGINI_USE_MULTI_LINE
        uint8_t             m_Line;                                                   // Number of line in the print
        char*               m_pSubLineString  [DIGINI_MAX_PRINT_NUMBER_OF_LINE];      // Pointer on each start of a sub line
        size_t              m_SubLineSizeChar [DIGINI_MAX_PRINT_NUMBER_OF_LINE];      // Size of each line    in caracter
        size_t              m_SubLineSizePixX [DIGINI_MAX_PRINT_NUMBER_OF_LINE];      // Width of each line   in pixel
        uint8_t             m_MinY            [DIGINI_MAX_PRINT_NUMBER_OF_LINE];      // Minimum Y pixel position of the line
     // #endif
};

//-------------------------------------------------------------------------------------------------

//#endif //DIGINI_USE_GRAFX

//-------------------------------------------------------------------------------------------------
