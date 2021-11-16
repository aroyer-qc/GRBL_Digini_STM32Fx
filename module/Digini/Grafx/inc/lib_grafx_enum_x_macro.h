//-------------------------------------------------------------------------------------------------
//
//  File : lib_grafx_enum_x_macro.h
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

#include "color_cfg.h"

//-------------------------------------------------------------------------------------------------
// Auto generated Enum Typedef(s)
//-------------------------------------------------------------------------------------------------

enum Widget_e
{
    INVALID_WIDGET    = 0,   // need to be here as it is the value to erase touchscreen detection layer

  // ---------- Background widget list --------------------

  #ifdef BACK_DEF
      APP_START_BACK_CONST,
    #define X_BACK(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, IMG_0, IMG_1, IMG_2, IMG_3, IMG_4, IMG_5, IMG_6, IMG_7, IMG_8, IMG_9, IMG_10, IMG_11, IMG_12, IMG_13, IMG_14, IMG_15, OPTIONS) ENUM_ID,
      BACK_DEF
    #undef  X_BACK
      APP_END_BACK_CONST,
  #endif

  // ---------- Basic Box widget list ---------------------

  #ifdef BASIC_BOX_DEF
      APP_START_BASIC_BOX_CONST,
    #define X_BASIC_BOX(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, IMG_TL, IMG_TR, IMG_BL, IMG_BR, IMG_H, IMG_V, SERVICE_FILTER, OPTIONS) ENUM_ID,
      BASIC_BOX_DEF
    #undef  X_BASIC_BOX
      APP_END_BASIC_BOX_CONST,
  #endif

  // ---------- Basic Button widget list ------------------

  #ifdef BASIC_BTN_DEF
      APP_START_BASIC_BTN_CONST,
    #define X_BASIC_BTN(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, THICKNESS, BACK_COLOR, PRESSED_BACK_COLOR, BOX_COLOR, PRESSED_BOX_COLOR, POS_TEXT_X, POS_TEXT_Y, SIZE_TEXT_X, SIZE_TEXT_Y, TEXT_COLOR, PRESSED_TEXT_COLOR, FONT_ID, TEXT_OPTION, TEXT_LABEL, SERVICE_FILTER) ENUM_ID,
      BASIC_BTN_DEF
    #undef  X_BASIC_BTN
      APP_END_BASIC_BTN_CONST,
  #endif

  // ---------- Basic Rectangle widget list ---------------

  #ifdef BASIC_RECT_DEF
      APP_START_BASIC_RECT_CONST,
    #define X_BASIC_RECT(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, COLOR, SERVICE_FILTER, OPTIONS) ENUM_ID,
      BASIC_RECT_DEF
    #undef  X_BASIC_RECT
      APP_END_BASIC_RECT_CONST,
  #endif

  // ---------- Button widget list ------------------------

  #ifdef BTN_DEF
      APP_START_BTN_CONST,
    #define X_BTN( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, RELEASED_BUTTON, PRESSED_BUTTON, INACTIVE_BUTTON,  POS_GLYPH_X, POS_GLYPH_Y, RELEASED_GLYPH, PRESSED_GLYPH, INACTIVE_GLYPH, POS_TOUCH_X, POS_TOUCH_Y, SIZE_TOUCH_X, SIZE_TOUCH_Y, POS_TEXT_X, POS_TEXT_Y, SIZE_TEXT_X, SIZE_TEXT_Y, TEXT_COLOR, TEXT_PR_COLOR, TEXT_GR_COLOR, FONT_ID, TEXT_OPTION, TEXT_LABEL, OPTIONS, SERVICE_FILTER)  ENUM_ID,

      BTN_DEF
    #undef  X_BTN
      APP_END_BTN_CONST,
  #endif

  // ---------- Gif widget list ---------------------------

  #ifdef GIF_DEF
      APP_START_GIF_CONST,
    #define X_GIF(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, GIF) ENUM_ID,
      GIF_DEF
    #undef  X_GIF
      APP_END_GIF_CONST,
  #endif

  // ---------- Graph widget list -------------------------

  #ifdef GRAPH_DEF
      APP_START_GRAPH_CONST,
    #define X_GRAPH(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, GRAPH, GRID_COLOR, GRID_X, GRID_Y, SCALE_X, SCALE_Y, SCROLL, OPTION) ENUM_ID,
      GRAPH_DEF
    #undef  X_GRAPH
      APP_END_GRAPH_CONST,
  #endif

  // ---------- Icon widget list --------------------------

  #ifdef ICON_DEF
      APP_START_ICON_CONST,
    #define X_ICON(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, ICON_0, ICON_1, ICON_2, ICON_3, OPTION) ENUM_ID,
      ICON_DEF
    #undef  X_ICON
      APP_END_ICON_CONST,
  #endif

  // ---------- Label widget list -------------------------

  #ifdef LABEL_DEF
      APP_START_LABEL_CONST,
    #define X_LBL(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, TEXT_COLOR, TEXT_ALT_COLOR, TEXT_ALT2_COLOR, FONT_ID, TEXT_BLEND, TEXT_LABEL, TEXT_OPTION, LABEL_OPTION) ENUM_ID,
      LABEL_DEF
    #undef  X_LBL
      APP_END_LABEL_CONST,
  #endif

  // ---------- Label List widget list --------------------

  #ifdef LABEL_LIST_DEF
      APP_START_LABEL_LIST_CONST,
    #define X_LABEL_LIST(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, TEXT_COLOR, TEXT_ALT_COLOR, TEXT_ALT2_COLOR, FONT_ID, TEXT_BLEND, TEXT_STATE_0, TEXT_STATE_1, TEXT_STATE_2, TEXT_STATE_3, TEXT_OPTION, LABEL_OPTION) ENUM_ID,
      LABEL_LIST_DEF
    #undef  X_LABEL_LIST
      APP_END_LABEL_LIST_CONST,
  #endif

  // ---------- Meter widget list -------------------------

  #ifdef METER_DEF
      APP_START_METER_CONST,
    #define X_METER(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, MINIMUM, MAXIMUM, CURSOR, CURSOR_RADIUS, START_ANGLE, END_ANGLE, RANGE, OPTIONS, POS_TEXT_X, POS_TEXT_Y, SIZE_TEXT_X, SIZE_TEXT_Y, TEXT_COLOR, TEXT_ALT_COLOR, FONT_ID, FONT_ALT_ID, TEXT_OPTION, TEXT_LABEL) ENUM_ID,
      METER_DEF
    #undef  X_METER
      APP_END_METER_CONST,
  #endif

  // ---------- PageSlide widget list ---------------------

  #ifdef PAGE_SLIDE_DEF
      APP_START_PAGE_SLIDE_CONST,
    #define X_PAGE_SLIDE(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, SLIDE_DIR, OPTIONS, SERVICE_FILTER) ENUM_ID,
      PAGE_SLIDE_DEF
    #undef  X_PAGE_SLIDE
      APP_END_PAGE_SLIDE_CONST,
  #endif

  // ---------- Panel widget list -------------------------

  #ifdef PANEL_DEF
      APP_START_PANEL_CONST,
    #define X_PANEL(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, RELEASED_PANEL, PRESSED_PANEL, POS_TEXT_X, POS_TEXT_Y, SIZE_TEXT_X, SIZE_TEXT_Y, TEXT_COLOR, TEXT_ALT_COLOR, FONT_ID, FONT_ALT_ID, TEXT_OPTION, TEXT_LABEL, SERVICE_FILTER) ENUM_ID,
      PANEL_DEF
    #undef  X_PANEL
      APP_END_PANEL_CONST,
  #endif

  // ---------- Progress widget list ----------------------

  #ifdef PROGRESS_DEF
      APP_START_PROGRESS_CONST,
    #define X_PROGRESS(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, MINIMUM, MAXIMUM, CURSOR, CURSOR_OFFSET_X, CURSOR_OFFSET_Y, START_POSITION, END_POSITION, PIXEL_RANGE, POS_TEXT_X, POS_TEXT_Y, SIZE_TEXT_X, SIZE_TEXT_Y, TEXT_COLOR, TEXT_ALT_COLOR, FONT_ID, FONT_ALT_ID, TEXT_OPTION, TEXT_LABEL, OPTIONS) ENUM_ID,
      PROGRESS_DEF
    #undef  X_PROGRESS
      APP_END_PROGRESS_CONST,
  #endif

  // ---------- Spectrum widget list ----------------------

  #ifdef SPECTRUM_DEF
      APP_START_SPECTRUM_CONST,
    #define X_SPECTRUM(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, RES_X, RES_Y, SPACING, BAR, OPTIONS) ENUM_ID,
      SPECTRUM_DEF
    #undef  X_SPECTRUM
      APP_END_SPECTRUM_CONST,
  #endif

  // ---------- Sprite widget list ------------------------

  #ifdef SPRITE_DEF
      APP_START_SPRITE_CONST,
    #define X_SPRITE(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, SIZE_X, SIZE_Y, IMAGE) ENUM_ID,
      SPRITE_DEF
    #undef  X_SPRITE
      APP_END_SPRITE_CONST,
  #endif

  // ---------- Terminal widget list ----------------------

  #ifdef TERMINAL_DEF
      APP_START_TERMINAL_CONST,
    #define X_TERMINAL(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, TEXT_COLOR, FONT_ID, SERVICE_FILTER, OPTIONS) ENUM_ID,
      TERMINAL_DEF
    #undef  X_TERMINAL
      APP_END_TERMINAL_CONST,
  #endif

  // ---------- Virtual Hub widget list -------------------

  #ifdef VIRTUAL_HUB_DEF
      APP_START_VIRTUAL_HUB_CONST,
    #define X_HUB(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, LINK_ID1, LINK_ID2, LINK_ID3, LINK_ID4, LINK_ID5, LINK_ID6, LINK_ID7, LINK_ID8) ENUM_ID,
      VIRTUAL_HUB_DEF
    #undef  X_HUB
      APP_END_VIRTUAL_HUB_CONST,
  #endif

  // ---------- Virtual Window widget list ----------------

  #ifdef VIRTUAL_WINDOW_DEF
      APP_START_VIRTUAL_WINDOW_CONST,
    #define X_WINDOW(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y)  ENUM_ID,
      VIRTUAL_WINDOW_DEF
    #undef  X_WINDOW
      APP_END_VIRTUAL_WINDOW_CONST,
  #endif

};

//-------------------------------------------------------------------------------------------------

  // Exchange data list
enum Exchange_e
{
  #ifdef XCHANGE_DEF
    #define X_XCHANGE(ENUM_ID, POINTER) ENUM_ID,
      XCHANGE_DEF
    #undef  X_XCHANGE
      XCHANGE_COUNT,
  #endif
};

//-------------------------------------------------------------------------------------------------

enum Layer_e
{
  #ifdef LAYER_DEF
   #define X_LAYER(ENUM_ID, WORK_LAYER, PIXEL_FORMAT, SIZE_X, SIZE_Y) ENUM_ID,
    LAYER_DEF
   #undef X_LAYER
  #endif
    LAYER_COUNT,
    LAYER_FIRST_ITEM = 0,
};


// 16 bits colors
enum ColorTable_e
{
  #define X_COLOR(ENUM_ID, VALUE) ENUM_ID,
    COLOR_DEF
  #undef X_COLOR
  #define X_GRAY(ENUM_ID, VALUE) ENUM_ID,
    GRAY_DEF
  #undef X_GRAY
    #ifdef CUSTOM_COLOR_DEF
    #define X_CUSTOM_COLOR(ENUM_ID, VALUE) ENUM_ID,
        CUSTOM_COLOR_DEF
    #undef  X_CUSTOM_COLOR
  #endif
    COL_NB_COLOR_CONST
} ;

//-------------------------------------------------------------------------------------------------
