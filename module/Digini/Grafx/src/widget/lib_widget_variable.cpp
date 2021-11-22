//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_variable.cpp
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

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#define WIDGET_VARIABLE_GLOBAL
#include "lib_grafx.h"
#undef  WIDGET_VARIABLE_GLOBAL
#include "lib_service.h"
#include "service.h"

//-------------------------------------------------------------------------------------------------
//  BACKGROUND Widget
//-------------------------------------------------------------------------------------------------

#ifdef BACK_DEF
Background_t Background[APP_NB_BACK_CONST] =
{
    #define X_BACK(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, IMG_0, IMG_1, IMG_2, IMG_3, IMG_4, IMG_5, IMG_6, IMG_7, IMG_8, IMG_9, IMG_10, IMG_11, IMG_12, IMG_13, IMG_14, IMG_15, OPTIONS) \
   {                                                                      \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},               \
        {(uint16_t)POS_X, (uint16_t)POS_Y},                               \
        {{IMG_0, IMG_1, IMG_2,  IMG_3,  IMG_4,  IMG_5,  IMG_6,  IMG_7,    \
          IMG_8, IMG_9, IMG_10, IMG_11, IMG_12, IMG_13, IMG_14, IMG_15}}, \
        OPTIONS,                                                          \
    },
    BACK_DEF
    #undef X_BACK
};
#endif   // BACK_DEF

//-------------------------------------------------------------------------------------------------
//  BASIC_BOX Widget
//-------------------------------------------------------------------------------------------------

#ifdef BASIC_BOX_DEF
BasicBox_t BasicBox[APP_NB_BASIC_BOX_CONST] =
{
    #define X_BASIC_BOX(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, IMG_TL, IMG_TR, IMG_BL, IMG_BR, IMG_H, IMG_V, SERVICE_FILTER, OPTIONS)       \
    {                                                                                \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},                          \
        {{(int16_t)POS_X, (int16_t)POS_Y}, {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},    \
        IMG_TL, IMG_TR, IMG_BL, IMG_BR, IMG_H, IMG_V,                                \
        SERVICE_FILTER, OPTIONS,                                                     \
    },
    BASIC_BOX_DEF
    #undef X_BASIC_BOX
};
#endif   // BASIC_BOX_DEF

//-------------------------------------------------------------------------------------------------
//  BASIC_BUTTON Widget
//-------------------------------------------------------------------------------------------------

#ifdef BASIC_BTN_DEF
BasicButton_t BasicButton[APP_NB_BASIC_BTN_CONST] =
{
    #define X_BASIC_BTN( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, THICKNESS, BACK_COLOR, PRESSED_BACK_COLOR, BOX_COLOR, PRESSED_BOX_COLOR, POS_TEXT_X, POS_TEXT_Y, SIZE_TEXT_X, SIZE_TEXT_Y, TEXT_COLOR, PRESSED_TEXT_COLOR, FONT_ID, TEXT_OPTION, TEXT_LABEL, SERVICE_FILTER)       \
    {                                                                                \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},                          \
        {{(int16_t)POS_X, (int16_t)POS_Y}, {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},    \
        BACK_COLOR, PRESSED_BACK_COLOR, BOX_COLOR, PRESSED_BOX_COLOR,                \
        {{{(int16_t)(POS_TEXT_X + POS_X), (int16_t)(POS_TEXT_Y + POS_Y)},            \
         {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},                                      \
         {GFX_ColorTable[TEXT_COLOR].u_32, GFX_ColorTable[PRESSED_TEXT_COLOR].u_32}, \
         {FONT_ID, FONT_ID}, ALPHA_BLEND, TEXT_LABEL,                                \
         (uint8_t)TEXT_OPTION},                                                      \
         SERVICE_FILTER, OPTION_NONE, THICKNESS,                                     \
    },
    BASIC_BTN_DEF
    #undef X_BASIC_BTN
};
#endif   // BASIC_BTN_DEF

//-------------------------------------------------------------------------------------------------
//  BASIC_RECT Widget
//-------------------------------------------------------------------------------------------------

#ifdef BASIC_RECT_DEF
BasicRect_t BasicRect[APP_NB_BASIC_RECT_CONST] =
{
    #define X_BASIC_RECT(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, COLOR, SERVICE_FILTER, OPTIONS)       \
    {                                                                                \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},                          \
        {{(int16_t)POS_X, (int16_t)POS_Y}, {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},    \
        GFX_ColorTable[COLOR].u_32,                                                  \
        SERVICE_FILTER, OPTIONS,                                                     \
    },
    BASIC_RECT_DEF
  };
#endif   // BASIC_RECT_DEF

//-------------------------------------------------------------------------------------------------
//  BUTTON Widget
      #undef X_BASIC_RECT
//-------------------------------------------------------------------------------------------------

#ifdef BTN_DEF
Button_t Button[APP_NB_BTN_CONST] =
{
    #define X_BTN( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, RELEASED_BUTTON, PRESSED_BUTTON, INACTIVE_BUTTON,  POS_GLYPH_X, POS_GLYPH_Y, RELEASED_GLYPH, PRESSED_GLYPH, INACTIVE_GLYPH, POS_TOUCH_X, POS_TOUCH_Y, SIZE_TOUCH_X, SIZE_TOUCH_Y, POS_TEXT_X, POS_TEXT_Y, SIZE_TEXT_X, SIZE_TEXT_Y, TEXT_COLOR, TEXT_PR_COLOR, TEXT_GR_COLOR, FONT_ID, TEXT_OPTION, TEXT_LABEL, SERVICE_FILTER, OPTIONS)       \
    {                                                                              \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},                        \
        {(int16_t)POS_X, (int16_t)POS_Y},                                          \
        {{RELEASED_BUTTON, PRESSED_BUTTON, INACTIVE_BUTTON}},                      \
        {{(int16_t)(POS_GLYPH_X + POS_X), (int16_t)(POS_GLYPH_Y + POS_Y)},         \
         {RELEASED_GLYPH, PRESSED_GLYPH, INACTIVE_GLYPH}},                         \
        {{{(int16_t)(POS_TEXT_X + POS_X),  (int16_t)(POS_TEXT_Y + POS_Y)},         \
          {(uint16_t)SIZE_TEXT_X, (uint16_t)SIZE_TEXT_Y}},                         \
         {GFX_ColorTable[TEXT_COLOR].u_32,                                         \
          GFX_ColorTable[TEXT_PR_COLOR].u_32,                                      \
          GFX_ColorTable[TEXT_GR_COLOR].u_32},                                     \
         FONT_ID, ALPHA_BLEND, TEXT_LABEL,                                         \
         (uint8_t)TEXT_OPTION},                                                    \
        {{{(int16_t)POS_TOUCH_X, (int16_t)POS_TOUCH_Y},                            \
         {(uint16_t)SIZE_TOUCH_X, (uint16_t)SIZE_TOUCH_Y}}},                       \
        (uint16_t)SERVICE_FILTER,                                                  \
        (uint16_t)OPTIONS,                                                         \
    },
    BTN_DEF
    #undef X_BTN
};
#endif   // BTN_DEF

//-------------------------------------------------------------------------------------------------
//  GIF Widget
//-------------------------------------------------------------------------------------------------

#ifdef GIF_DEF
Gif_t Gif[APP_NB_GIF_CONST] =
{
    #define X_GIF( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, GIF)\
    {                                                       \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING}, \
        {(int16_t)POS_X, (int16_t)POS_Y},                   \
        {{GIF}},                                            \
        OPTION_NONE,                                        \
    },
    GIF_DEF
    #undef X_GIF
};
#endif   // GIF_DEF

//-------------------------------------------------------------------------------------------------
//  GRAPH Widget
//-------------------------------------------------------------------------------------------------

#ifdef GRAPH_DEF
Graph_t Graph[APP_NB_GRAPH_CONST] =
{
    #define X_GRAPH(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, GRAPH, GRID_COLOR, GRID_X, GRID_Y, SCALE_X, SCALE_Y, SCROLL, OPTION)\
    {                                                       \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING}, \
        {{(int16_t)POS_X, (int16_t)POS_Y},                  \
         {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},             \
        GRAPH,                                              \
        GRID_COLOR,                                         \
        GRID_X,                                             \
        GRID_Y,                                             \
        SCALE_X,                                            \
        SCALE_Y,                                            \
        SCROLL,                                             \
        OPTION_NONE,                                        \
    },
    GRAPH_DEF
    #undef XGRAPH
};
#endif   // GRAPH_DEF

//-------------------------------------------------------------------------------------------------
//  ICON Widget
//-------------------------------------------------------------------------------------------------

#ifdef ICON_DEF
Icon_t Icon[APP_NB_ICON_CONST] =
{
    #define X_ICON( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, ICON_0, ICON_1, ICON_2, ICON_3, OPTIONS)\
    {                                                       \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING}, \
        {{(int16_t)POS_X, (int16_t)POS_Y},                  \
         {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},             \
        {{ICON_0, ICON_1, ICON_2, ICON_3}},                 \
        (uint16_t)OPTIONS                                   \
    },
    ICON_DEF
    #undef X_ICON
};
#endif   // ICON_DEF

//-------------------------------------------------------------------------------------------------
//  LABEL Widget
//-------------------------------------------------------------------------------------------------

#ifdef LABEL_DEF
Label_t Label[APP_NB_LABEL_CONST] =
{
    #define X_LBL(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, TEXT_COLOR, TEXT_ALT_COLOR, TEXT_ALT2_COLOR, FONT_ID, TEXT_BLEND, TEXT_LABEL, TEXT_OPTION, LABEL_OPTION)\
    {                                                                                \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},                          \
        {{{(int16_t)POS_X, (int16_t)POS_Y},                                          \
         {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},                                      \
         {GFX_ColorTable[TEXT_COLOR].u_32,                                           \
          GFX_ColorTable[TEXT_ALT_COLOR].u_32,                                       \
          GFX_ColorTable[TEXT_ALT2_COLOR].u_32},                                     \
         FONT_ID,                                                                    \
         TEXT_BLEND, TEXT_LABEL,                                                     \
         (uint8_t)TEXT_OPTION},                                                      \
        LABEL_OPTION,                                                                \
    },
    LABEL_DEF
    #undef X_LBL
};
#endif   // LABEL_DEF

//-------------------------------------------------------------------------------------------------
//  LABEL_LIST Widget
//-------------------------------------------------------------------------------------------------

#ifdef LABEL_LIST_DEF
LabelList_t LabelList[APP_NB_LABEL_LIST_CONST] =
{
    #define X_LABEL_LIST(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, TEXT_COLOR, TEXT_ALT_COLOR, TEXT_ALT2_COLOR, FONT_ID, TEXT_BLEND, TEXT_STATE_0, TEXT_STATE_1, TEXT_STATE_2, TEXT_STATE_3, TEXT_OPTION, LABEL_OPTION) \
    {                                                                            \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},                      \
        {{{(int16_t)POS_X, (int16_t)POS_Y},                                      \
          {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},                                 \
         {GFX_ColorTable[TEXT_COLOR].u_32,                                       \
          GFX_ColorTable[TEXT_ALT_COLOR].u_32,                                   \
          GFX_ColorTable[TEXT_ALT2_COLOR].u_32},                                 \
         FONT_ID,                                                                \
         TEXT_BLEND,                                                             \
         INVALID_LABEL,                                                          \
         (uint8_t)TEXT_OPTION},                                                  \
         {TEXT_STATE_0,  TEXT_STATE_1,  TEXT_STATE_2,  TEXT_STATE_3},            \
         LABEL_OPTION,                                                           \
    },
    LABEL_LIST_DEF
    #undef X_LABEL_LIST
};
#endif   // LABEL_LIST_DEF

//-------------------------------------------------------------------------------------------------
//  METER Widget
//-------------------------------------------------------------------------------------------------

#ifdef METER_DEF
Meter_t Meter[APP_NB_METER_CONST] =
{
    #define X_METER( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, MINIMUM, MAXIMUM, CURSOR, CURSOR_RADIUS, START_ANGLE, END_ANGLE, RANGE, OPTIONS, POS_TEXT_X, POS_TEXT_Y, SIZE_TEXT_X, SIZE_TEXT_Y, TEXT_COLOR, TEXT_ALT_COLOR, FONT_ID, FONT_ALT_ID, TEXT_OPTION, TEXT_LABEL)\
    {                                                                                                          \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},                                                    \
        {{(int16_t)POS_X, (int16_t)POS_Y}, {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},                              \
        CURSOR_RADIUS, START_ANGLE, END_ANGLE, RANGE, MINIMUM, MAXIMUM, CURSOR,                                \
        {{{(int16_t)(POS_TEXT_X + POS_X), (int16_t)(POS_TEXT_Y + POS_Y)},                                      \
        {(uint16_t)SIZE_TEXT_X, (uint16_t)SIZE_TEXT_Y}},                                                       \
        {GFX_ColorTable[TEXT_COLOR].u_32, GFX_ColorTable[TEXT_ALT_COLOR].u_32},                                \
         {FONT_ID, FONT_ALT_ID},                                                                               \
         ALPHA_BLEND,                                                                                          \
         TEXT_LABEL,                                                                                           \
         (uint8_t)TEXT_OPTION},                                                                                \
        (uint16_t)OPTIONS,                                                                                     \
    },
    METER_DEF
  #undef X_METER
};
#endif   // METER_DEF

//-------------------------------------------------------------------------------------------------
//  PAGE_SLIDE Widget
//-------------------------------------------------------------------------------------------------

#ifdef PAGE_SLIDE_DEF
PageSlide_t PageSlide[APP_NB_PAGE_SLIDE_CONST] =
{
    #define X_PAGE_SLIDE( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, SLIDE_DIR, OPTIONS, SERVICE_FILTER)\
    {                                                                                                    \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING}, SERVICE_FILTER,                              \
        {{(int16_t)POS_X, (int16_t)POS_Y}, {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},                        \
		 (SlideDir_e)SLIDE_DIR,                                                                          \
         (uint16_t)OPTIONS,                                                                              \
    },
    PAGE_SLIDE_DEF
  #undef X_PAGE_SLIDE
};
#endif   // PAGE_SLIDE_DEF

//-------------------------------------------------------------------------------------------------
//  PANEL Widget
//-------------------------------------------------------------------------------------------------

#ifdef PANEL_DEF
Panel_t Panel[APP_NB_PANEL_CONST] =
{
    #define X_PANEL( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, RELEASED_PANEL, PRESSED_PANEL, POS_TEXT_X, POS_TEXT_Y, SIZE_TEXT_X, SIZE_TEXT_Y, TEXT_COLOR, TEXT_ALT_COLOR, FONT_ID, FONT_ALT_ID, TEXT_OPTION, TEXT_LABEL, SERVICE_FILTER)\
    {                                                                                                    \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING}, SERVICE_FILTER,                              \
        {{(int16_t)POS_X, (int16_t)POS_Y}, {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},                        \
        {{RELEASED_PANEL, PRESSED_PANEL}},                                                               \
        {{{(int16_t)(POS_TEXT_X + POS_X), (int16_t)(POS_TEXT_Y + POS_Y)},                                \
         {(uint16_t)SIZE_TEXT_X, (uint16_t)SIZE_TEXT_Y}},                                                \
         {GFX_ColorTable[TEXT_COLOR].u_32, GFX_ColorTable[TEXT_ALT_COLOR].u_32},                         \
         {FONT_ID, FONT_ALT_ID}, ALPHA_BLEND, TEXT_LABEL},                                               \
         (uint8_t)TEXT_OPTION,                                                                           \
    },
    PANEL_DEF
  #undef X_PANEL
};
#endif   // PANEL_DEF

//-------------------------------------------------------------------------------------------------
//  PROGRESS Widget
//-------------------------------------------------------------------------------------------------

#ifdef PROGRESS_DEF
Progress_t Progress[APP_NB_PROGRESS_CONST] =
{
    #define X_PROGRESS( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, BACKGROUND, BAR, CURSOR, CURSOR_OFFSET_X, CURSOR_OFFSET_Y, START_VALUE, END_VALUE, PIXEL_RANGE, POS_TEXT_X, POS_TEXT_Y, SIZE_TEXT_X, SIZE_TEXT_Y, TEXT_COLOR, TEXT_ALT_COLOR, TEXT_ALT2_COLOR, FONT_ID, TEXT_OPTION, TEXT_LABEL, OPTIONS)\
    {                                                                                   \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},                             \
        {{(int16_t)POS_X, (int16_t)POS_Y},                                              \
         {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},                                         \
        PIXEL_RANGE, BACKGROUND, BAR, CURSOR,                                           \
        {{{(int16_t)(POS_TEXT_X + POS_X), (int16_t)(POS_TEXT_Y + POS_Y)},               \
          {(uint16_t)SIZE_TEXT_X, (uint16_t)SIZE_TEXT_Y}},                              \
         {GFX_ColorTable[TEXT_COLOR].u_32,                                              \
          GFX_ColorTable[TEXT_ALT_COLOR].u_32,                                          \
          GFX_ColorTable[TEXT_ALT2_COLOR].u_32},                                        \
         FONT_ID, ALPHA_BLEND, TEXT_LABEL,                                              \
         (uint8_t)TEXT_OPTION},                                                         \
        {(int16_t)CURSOR_OFFSET_X, (int16_t)CURSOR_OFFSET_Y},                           \
        START_VALUE, END_VALUE,                                                         \
        (uint16_t)OPTIONS,                                                              \
    },
    PROGRESS_DEF
    #undef X_PROGRESS
};
#endif   // PROGRESS_DEF

//-------------------------------------------------------------------------------------------------
//  SPECTRUM Widget
//-------------------------------------------------------------------------------------------------

#ifdef SPECTRUM_DEF
Spectrum_t Spectrum[APP_NB_SPECTRUM_CONST] =
{
    #define X_SPECTRUM( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, RES_X, RES_Y, SPACING, BAR, OPTIONS)\
    {                                                                                   \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},                             \
        {{(int16_t)POS_X, (int16_t)POS_Y}, {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},       \
        {(int16_t)RES_X, (int16_t)RES_Y},                                               \
        SPACING,                                                                        \
        BAR,                                                                            \
        (uint16_t)OPTIONS,                                                              \
    },
    SPECTRUM_DEF
    #undef X_SPECTRUM
};
#endif   // SPECTRUM_DEF

//-------------------------------------------------------------------------------------------------
//  SPRITE Widget
//-------------------------------------------------------------------------------------------------

#ifdef SPRITE_DEF
Sprite_t Sprite[APP_NB_SPRITE_CONST] =
{
    #define X_SPRITE( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, SIZE_X, SIZE_Y, SPRITE)\
    {                                                       \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING}, \
        {(uint16_t)SIZE_X, (uint16_t)SIZE_Y},               \
        {{SPRITE}},                                         \
        OPTION_NONE,                                        \
    },
    SPRITE_DEF
    #undef X_SPRITE
};
#endif   // SPRITE_DEF

//-------------------------------------------------------------------------------------------------
//  TERMINAL Widget
//-------------------------------------------------------------------------------------------------

#ifdef TERMINAL_DEF
Terminal_t Terminal[APP_NB_TERMINAL_CONST] =
{
    #define X_TERMINAL(ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y, TEXT_COLOR, FONT_ID, SERVICE_FILTER, OPTIONS)\
    {                                                                     \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING},               \
        {{(int16_t)POS_X, (int16_t)POS_Y},                                \
         {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},                           \
         GFX_ColorTable[TEXT_COLOR].u_32,                                 \
         FONT_ID,                                                         \
         SERVICE_FILTER, OPTIONS,                                         \
    },
    TERMINAL_DEF
    #undef X_TERMINAL
};
#endif   // TERMINAL_DEF

//-------------------------------------------------------------------------------------------------
//  VIRTUAL HUB Widget
//-------------------------------------------------------------------------------------------------

#ifdef VIRTUAL_HUB_DEF
VirtualHub_t VirtualHub[APP_NB_VIRTUAL_HUB_CONST] =
{
    #define X_HUB( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, LINK_ID1, LINK_ID2, LINK_ID3, LINK_ID4, LINK_ID5, LINK_ID6, LINK_ID7, LINK_ID8)\
    {                                                       \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING}, \
        {LINK_ID1, LINK_ID2, LINK_ID3, LINK_ID4,            \
         LINK_ID5, LINK_ID6, LINK_ID7, LINK_ID8},           \
    },
    VIRTUAL_HUB_DEF
  #undef X_HUB
};
#endif   // VIRTUAL_HUB_DEF

//-------------------------------------------------------------------------------------------------
//  VIRTUAL WINDOW Widget
//-------------------------------------------------------------------------------------------------

#ifdef VIRTUAL_WINDOW_DEF
VirtualWindow_t VirtualWindow[APP_NB_VIRTUAL_WINDOW_CONST] =
{
    #define X_WINDOW( ENUM_ID, SERVICE, SUB_SERVICE, TIMING, POS_X, POS_Y, SIZE_X, SIZE_Y)\
    {                                                       \
        {(uint32_t)SERVICE, (uint16_t)SUB_SERVICE, TIMING}, \
        {{(int16_t)POS_X, (int16_t)POS_Y},                  \
         {(uint16_t)SIZE_X, (uint16_t)SIZE_Y}},             \
    },
    VIRTUAL_WINDOW_DEF
  #undef X_WINDOW
};
#endif   // VIRTUAL_WINDOW_DEF

//-------------------------------------------------------------------------------------------------
//  XCHANGE data
//-------------------------------------------------------------------------------------------------

#ifdef XCHANGE_DEF
ExchangeCommon_t* pExchange[XCHANGE_COUNT] =
{
    #define X_XCHANGE( ENUM_ID, PTR) (ExchangeCommon_t*)PTR,
    XCHANGE_DEF
    #undef X_XCHANGE
};
#endif   // XCHANGE_DEF

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
