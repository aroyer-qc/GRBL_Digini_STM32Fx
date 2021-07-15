//-------------------------------------------------------------------------------------------------
//
//  File : lib_grafx_dbase.h
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
