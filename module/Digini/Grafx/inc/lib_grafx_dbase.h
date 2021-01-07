//-------------------------------------------------------------------------------------------------
//
//  File : lib_grafx_dbase.h
//
//*************************************************************************************************

#pragma once

#ifdef DIGINI_USE_QUAD_SPI_FOR_GRAFX_DATABASE

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
