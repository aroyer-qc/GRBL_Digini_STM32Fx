//-------------------------------------------------------------------------------------------------
//
//  File :  grafx_cfg.h
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
//
// Notes: if GRAFX_USE_A_SKIN is define
//
//      lib_compression make use of the display memory for:
//          - Loading of the compressed image
//          - to hold temporary data use during decompression
//          - to hold decompressed data before storage in to the database
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
//
// Memory mapping for STM32F746G-DISCO
//
//
//          0xC000 0000              GRAFX base address
//
//          0xC000 0000              Layer 1                RGB565   = 261120 Bytes
//          0xC003 FC00              Layer 2                ARGB8888 = 522240 Bytes
//          0xC00B F400              Layer 3                RGB565   = 261120 Bytes
//          0xC00F F000              Layer 4                ARGB8888 = 522240 Bytes
//          0xC017 E800              Layer 5                ARGB8888 = 522240 Bytes             Use for skin sliding page
//          0xC01F E000              Touch Sense layer      RGB565   = 261120 Bytes
//          0xC023 DC00              Free memory pointer for raw skin data 5776384 Bytes free for the skin raw data
//              ...
//          0xC07C 0000              256K database for skin info
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// GRAFX Configuration for decompressing data
//
// Note(s) you must provide memory address for decompressing input and output data
//
//-------------------------------------------------------------------------------------------------

// Du to memory limitation (8M), we use unused layer for temporary decoding
#define GRAFX_RAW_INPUT_DATA_ADDRESS                    0xC00BF400      // 128K
#define GRAFX_DECODE_ARRAY_ADDRESS                      0xC00DF400      // 224K
#define GRAFX_APPEND_ARRAY_ADDRESS                      0xC0117400      // 224K
#define GRAFX_PREFIX_ARRAY_ADDRESS                      0xC014F400      // 224K
#define GRAFX_DECOMPRESS_METHOD_ADDRESS                 0xC0187400      // 8K
#define GRAFX_DATA_SIZE_ADDRESS                         0xC0189400      // 8K

//-------------------------------------------------------------------------------------------------

#define GRAFX_SKIN_FILENAME                             "grbl.skn"
#define GRAFX_USE_POINTING_DEVICE                       DEF_ENABLED

//-------------------------------------------------------------------------------------------------

// Special section use in this project
#define GRAFX_USE_DISPLAY_RAM                           DEF_ENABLED
#define GRAFX_USE_RAM_DATA                              DEF_ENABLED     // this setting is not very clear at this point... probably for LCD with ram access
#define GRAFX_USE_ROM_DATA                              DEF_DISABLED

#define GRAFX_USE_MULTI_LINE                            DEF_ENABLED

#define GRAFX_TICK_WAIT_BETWEEN_REFRESH_LOOP            8

#define GRAFX_USE_SLIDING_PAGE                          DEF_ENABLED
#define GRAFX_SLIDING_PAGE_GRANULARITY                  16              // Each step is 10 Pixel wide
#define GRAFX_TICK_WAIT_BETWEEN_SLIDE_IN_LOOP           16

#define GRAFX_USE_BACKGROUND_LAYER                      DEF_ENABLED     // if you're LCD support more than 1 layer
#define GRAFX_USE_CONSTRUCTION_BACKGROUND_LAYER         DEF_ENABLED     // if there is enough memory to construct on a hidden layer. (prevent glitch)
#define GRAFX_USE_CONSTRUCTION_FOREGROUND_LAYER         DEF_ENABLED

#define GRAFX_DEBUG_GUI                                 DEF_DISABLED
#define GRAFX_PAINT_BOX_DEBUG                           DEF_DISABLED
#define GRAFX_PAINT_BOX_DEBUG_COLOR                     RED

#ifdef DIGINI_USE_QSPI
  #ifndef DIGINI_USE_QUAD_SPI_FOR_GRAFX_DATABASE
    #define DIGINI_USE_QUAD_SPI_FOR_GRAFX_DATABASE      DEF_DISABLED
  #endif
#endif

#if (GRAFX_USE_POINTING_DEVICE == DEF_ENABLED)
  #define GRAFX_PDI_SWAP_XY                             DEF_ENABLED
  #define GRAFX_PDI_INVERT_X                            DEF_DISABLED    // Not existent on this setup
  #define GRAFX_PDI_INVERT_Y                            DEF_DISABLED    // Not existent on this setup
#endif

#define GRAFX_USE_LOAD_SKIN                             DEF_ENABLED
#define GRAFX_USE_PDI_MULTI_EVENT                       DEF_ENABLED     // Enable support for Multi-touch
#define GRAFX_PDI_INTERRUPT_IO                          DEF_DISABLED    // Pointing device (touch) IRQ is not available on DISCO746


//#define GRAFX_USE_TIMED_WIDGET
#define GRAFX_USE_FONT_SIZE_8                           DEF_DISABLED    // Not used on this setup
#define GRAFX_USE_FONT_SIZE_12                          DEF_DISABLED    // Not used on this setup
#define GRAFX_USE_FONT_SIZE_16                          DEF_ENABLED

///------------------------------------------------------------------------------------------------
/// Color supported mode in this project
#define GRAFX_COLOR_ARGB8888                            DEF_ENABLED
#define GRAFX_COLOR_RGB888                              DEF_DISABLED
#define GRAFX_COLOR_RGB565                              DEF_ENABLED
#define GRAFX_COLOR_ARGB1555                            DEF_DISABLED
#define GRAFX_COLOR_ARGB4444                            DEF_DISABLED
#define GRAFX_COLOR_L8                                  DEF_DISABLED
#define GRAFX_COLOR_AL44                                DEF_DISABLED
#define GRAFX_COLOR_AL88                                DEF_DISABLED
#define GRAFX_COLOR_L4                                  DEF_DISABLED
#define GRAFX_COLOR_A8                                  DEF_DISABLED
#define GRAFX_COLOR_A4                                  DEF_DISABLED
#define GRAFX_COLOR_RGB332                              DEF_DISABLED
#define GRAFX_COLOR_RGB444                              DEF_DISABLED

#define DIGINI_MAX_PRINT_NUMBER_OF_LINE                 8               // This is the maximum number of line in the same string

// Default service available in Grafx
#define USE_SERV_DATE                                   DEF_ENABLED     // Service to provide the date
#define USE_SERV_INPD                                   DEF_ENABLED     // Service to input a decimal value
#define USE_SERV_INPF                                   DEF_ENABLED     // Service to input a float value
#define USE_SERV_INPH                                   DEF_ENABLED     // Service to input a Hexa value
#define USE_SERV_INPS                                   DEF_DISABLED    // Service to input a string
#define USE_SERV_TIME                                   DEF_ENABLED     // Service to provide time
#define USE_SERV_XCHG                                   DEF_ENABLED     // Special service to exchange data.

//-------------------------------------------------------------------------------------------------

