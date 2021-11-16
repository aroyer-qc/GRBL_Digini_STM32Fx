//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_variable.h
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

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "lib_grafx.h"

//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

 #ifdef BACK_DEF
  #define APP_NB_BACK_CONST                 (APP_END_BACK_CONST             - APP_START_BACK_CONST)             - 1
  extern Background_t       Background          [APP_NB_BACK_CONST];
 #endif

 #ifdef BASIC_BOX_DEF
  #define APP_NB_BASIC_BOX_CONST            (APP_END_BASIC_BOX_CONST        - APP_START_BASIC_BOX_CONST)        - 1
  extern BasicBox_t         BasicBox            [APP_NB_BASIC_BOX_CONST];
 #endif

 #ifdef BASIC_BTN_DEF
  #define APP_NB_BASIC_BTN_CONST            (APP_END_BASIC_BTN_CONST        - APP_START_BASIC_BTN_CONST)        - 1
  extern BasicButton_t      BasicButton         [APP_NB_BASIC_BTN_CONST];
 #endif

 #ifdef BASIC_RECT_DEF
  #define APP_NB_BASIC_RECT_CONST           (APP_END_BASIC_RECT_CONST       - APP_START_BASIC_RECT_CONST)       - 1
  extern BasicRect_t        BasicRect           [APP_NB_BASIC_RECT_CONST];
 #endif

 #ifdef BTN_DEF
  #define APP_NB_BTN_CONST                  (APP_END_BTN_CONST              - APP_START_BTN_CONST)              - 1
  extern Button_t           Button              [APP_NB_BTN_CONST];
 #endif

 #ifdef GIF_DEF
  #define APP_NB_GIF_CONST                  (APP_END_GIF_CONST              - APP_START_GIF_CONST)              - 1
  extern Gif_t              Gif                 [APP_NB_GIF_CONST];
 #endif

 #ifdef GRAPH_DEF
  #define APP_NB_GRAPH_CONST                (APP_END_GRAPH_CONST            - APP_START_GRAPH_CONST)            - 1
  extern Graph_t            Graph               [APP_NB_GRAPH_CONST];
 #endif

 #ifdef ICON_DEF
  #define APP_NB_ICON_CONST                 (APP_END_ICON_CONST             - APP_START_ICON_CONST)             - 1
  extern Icon_t             Icon                [APP_NB_ICON_CONST];
 #endif

 #ifdef LABEL_DEF
  #define APP_NB_LABEL_CONST                (APP_END_LABEL_CONST            - APP_START_LABEL_CONST)            - 1
  extern Label_t            Label               [APP_NB_LABEL_CONST];
 #endif

 #ifdef LABEL_LIST_DEF
  #define APP_NB_LABEL_LIST_CONST           (APP_END_LABEL_LIST_CONST       - APP_START_LABEL_LIST_CONST)       - 1
  extern LabelList_t        LabelList           [APP_NB_LABEL_LIST_CONST];
 #endif

 #ifdef METER_DEF
  #define APP_NB_METER_CONST                (APP_END_METER_CONST            - APP_START_METER_CONST)            - 1
  extern Meter_t            Meter               [APP_NB_METER_CONST];
 #endif

 #ifdef PAGE_SLIDE_DEF
  #define APP_NB_PAGE_SLIDE_CONST           (APP_END_PAGE_SLIDE_CONST       - APP_START_PAGE_SLIDE_CONST)       - 1
  extern PageSlide_t        PageSlide           [APP_NB_PAGE_SLIDE_CONST];
 #endif

 #ifdef PANEL_DEF
  #define APP_NB_PANEL_CONST                (APP_END_PANEL_CONST            - APP_START_PANEL_CONST)            - 1
  extern Panel_t            Panel               [APP_NB_PANEL_CONST];
 #endif

 #ifdef PROGRESS_DEF
  #define APP_NB_PROGRESS_CONST             (APP_END_PROGRESS_CONST         - APP_START_PROGRESS_CONST)         - 1
  extern Progress_t         Progress            [APP_NB_PROGRESS_CONST];
 #endif

 #ifdef SPECTRUM_DEF
  #define APP_NB_SPECTRUM_CONST             (APP_END_SPECTRUM_CONST         - APP_START_SPECTRUM_CONST)         - 1
  extern Spectrum_t         Spectrum            [APP_NB_SPECTRUM_CONST];
 #endif

 #ifdef SPRITE_DEF
  #define APP_NB_SPRITE_CONST               (APP_END_SPRITE_CONST           - APP_START_SPRITE_CONST)           - 1
  extern Sprite_t           Sprite              [APP_NB_SPRITE_CONST];
 #endif

 #ifdef TERMINAL_DEF
  #define APP_NB_TERMINAL_CONST             (APP_END_TERMINAL_CONST         - APP_START_TERMINAL_CONST)         - 1
  extern Terminal_t         Terminal            [APP_NB_TERMINAL_CONST];
 #endif

 #ifdef VIRTUAL_HUB_DEF
  #define APP_NB_VIRTUAL_HUB_CONST          (APP_END_VIRTUAL_HUB_CONST      - APP_START_VIRTUAL_HUB_CONST)      - 1
  extern VirtualHub_t       VirtualHub          [APP_NB_VIRTUAL_HUB_CONST];
 #endif

 #ifdef VIRTUAL_WINDOW_DEF
  #define APP_NB_VIRTUAL_WINDOW_CONST       (APP_END_VIRTUAL_WINDOW_CONST   - APP_START_VIRTUAL_WINDOW_CONST)   - 1
  extern VirtualWindow_t    VirtualWindow       [APP_NB_VIRTUAL_WINDOW_CONST];
 #endif

//-------------------------------------------------------------------------------------------------

#ifdef XCHANGE_DEF
extern ExchangeCommon_t* pExchange[XCHANGE_COUNT];
#endif   // XCHANGE_DEF

//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
