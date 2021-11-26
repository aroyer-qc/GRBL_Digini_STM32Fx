//-------------------------------------------------------------------------------------------------
//
//  File :  widget_cfg.h
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
//  Notes:
//
//  Px      -> Position X
//  Py      -> Position Y
//  Px1-Px4 -> Position Point X
//  Py1-Py4 -> Position Point Y
//  Sx      -> Size X
//  Sy      -> Size Y
//  TPx     -> Text Position X
//  TPy     -> Text Position Y
//  TSx     -> Text Size X
//  TSy     -> Text Size Y
//  GPx     -> Glyph Position X
//  GPy     -> Glyph Position Y
//  BC      -> Back Color
//  TC      -> Text Color
//  TAC     -> Text Alternate Color
//  TAC2    -> Text Alternate Color 2
//  TC_PR   -> Text Color for pressed button
//  TC_GR   -> Text Color for inactive button
//  PTC     -> Pressed Text Color
//  TOPx    -> Touch Position X
//  TOPy    -> Touch Position Y
//  TOSx    -> Touch Size X
//  TOSy    -> Touch Size Y
//  COx     -> Cursor Offset X
//  COy     -> Cursor Offset Y
//  CSV     -> Cursor Start Value      ex. 0%
//  CEV     -> Cursor End Value        ex. 100%
//  CPR     -> Cursor Pixel Range      ex. 0% to 100% = 384 Pixel
//  ResX    -> Resolution Widget X     Not necessarily in pixel ( Ex. SPECTRUM -> 16 line x 12 dot)
//  ResY    -> Resolution Widget Y
//
//
//-------------------------------------------------------------------------------------------------
//
//  'Timing' is for future development, so we can decide to refresh widget at different interval
//                  GRAFX_OPTION_MOVABLE_OBJECT future development
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
// Individual Exchange Data Definition
//-------------------------------------------------------------------------------------------------

//           Enum ID,           Struct Ptr
// This X-Macro build a list of struct pointer to exchange data between page and/or application service.
// Note(s) First data of every exchange data structure is always ExchangeType_e. So Button can use this exchange system and able to print label into button
#define XCHANGE_DEF \
 X_XCHANGE ( CFG_INPUT_5_5_F,   &Input5_5_F_Cfg)        \
 X_XCHANGE ( CFG_INPUT_10_D,    &Input10_D_Cfg)         \
 X_XCHANGE ( CFG_INPUT_6_D,     &Input6_D_Cfg)          \
 X_XCHANGE ( CFG_INPUT_5_D,     &Input5_D_Cfg)          \
 X_XCHANGE ( CFG_IN_SPIN_MIN,   &InputSpindleMin_Cfg)   \
 X_XCHANGE ( CFG_IN_SPIN_MAX,   &InputSpindleMax_Cfg)   \

//-------------------------------------------------------------------------------------------------
// Individual widget Definition
//-------------------------------------------------------------------------------------------------
//       Enum ID,         Service,      Sub, Timing, Px, Py, IMG0,             IMG_1,         IMG_2,         IMG_3,         IMG_4,         IMG_5,         IMG_6,         IMG_7,         IMG_8,         IMG_9,         IMG_10,        IMG_11,        IMG_12,        IMG_13,        IMG_14,        IMG_15,        Options
#define BACK_DEF(X_BACK)\
 X_BACK( BACK_LOADING,    SERV_ID_RFSH, 0,   0,      0,  0,  LOADING_BACK,     INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, GRAFX_OPTION_DRAW_ONCE_AND_REMOVE )\
 X_BACK( BACK_DIAMOND,    SERV_ID_RFSH, 0,   0,      0,  0,  BG_IMG,           INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, INVALID_IMAGE, GRAFX_OPTION_DRAW_ONCE_AND_REMOVE )\

//      Enum ID,           Service,      Sub,             Timing, Px,   Py,   Released btn,   Pressed button,         Inactive button,   GPx,  GPy,  Released glyph,     Pressed glyph,      Inactive glyph  TOPx, TOPy, TOSx, TOSy, TPx,  TPy,    TSx,  TSy,  TC,         TC_PR,        TC_GR,        Font ID,          Text Option,             Text Label,     Filter,                 Option,
#define BTN_DEF(X_BTN) \
 X_BTN( BTN_SETTING,       SERV_ID_BDEF, SLIDING_RIGHT_OVLAP,   0,      426,  10,   BT_SPHERE,      BT_SPHERE_PR,           INVALID_IMAGE,     8,    8,    GL_TOOLS,           GL_TOOLS,           INVALID_IMAGE,  426,  10,   45,   45,   0,     0,     0,    0,    0,          0,            0,            INVALID_FONT,     0,                       INVALID_LABEL,  SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 /*X_BTN( BTN_JOG,           SERV_ID_BDEF, 0,             0,      426,  40,   BT_SPHERE,      BT_SPHERE_PR,           INVALID_IMAGE,     8,    8,    GL_JOG,             GL_JOG,             INVALID_IMAGE,  426,  10,   45,   45,   0,     0,     0,    0,    0,          0,            0,            INVALID_FONT,     0,                       INVALID_LABEL,  SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)*/\
 X_BTN( BTN_RETURN,        SERV_ID_BDEF, SLIDING_LEFT_DE_OVLAP,    0,      426,  10,   BT_SPHERE,      BT_SPHERE_PR,           INVALID_IMAGE,     8,    8,    GL_RETURN,          GL_RETURN,          INVALID_IMAGE,  426,  10,   45,   45,   0,     0,     0,    0,    0,          0,            0,            INVALID_FONT,     0,                       INVALID_LABEL,  SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_RETURN_DOWN,   SERV_ID_BDEF, SLIDING_DOWN,    0,      426,  10,   BT_SPHERE,      BT_SPHERE_PR,           INVALID_IMAGE,     8,    8,    GL_RETURN,          GL_RETURN,          INVALID_IMAGE,  426,  10,   45,   45,   0,     0,     0,    0,    0,          0,            0,            INVALID_FONT,     0,                       INVALID_LABEL,  SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 /* Keypad input value */\
 X_BTN( BTN_1,             SERV_ID_INPT, '1',             0,      10,   107,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  10,   107,  75,   38,   27,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_CENTER_Y_CENTER,      LBL_1,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_2,             SERV_ID_INPT, '2',             0,      90,   107,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  90,   107,  75,   38,   27,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_CENTER_Y_CENTER,      LBL_2,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_3,             SERV_ID_INPT, '3',             0,      170,  107,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  170,  107,  75,   38,   27,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_CENTER_Y_CENTER,      LBL_3,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_4,             SERV_ID_INPT, '4',             0,      10,   149,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  10,   149,  75,   38,   27,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_CENTER_Y_CENTER,      LBL_4,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_5,             SERV_ID_INPT, '5',             0,      90,   149,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  90,   149,  75,   38,   27,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_CENTER_Y_CENTER,      LBL_5,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_6,             SERV_ID_INPT, '6',             0,      170,  149,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  170,  149,  75,   38,   27,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_CENTER_Y_CENTER,      LBL_6,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_7,             SERV_ID_INPT, '7',             0,      10,   191,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  10,   191,  75,   38,   27,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_CENTER_Y_CENTER,      LBL_7,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_8,             SERV_ID_INPT, '8',             0,      90,   191,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  90,   191,  75,   38,   27,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_CENTER_Y_CENTER,      LBL_8,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_9,             SERV_ID_INPT, '9',             0,      170,  191,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  170,  191,  75,   38,   27,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_CENTER_Y_CENTER,      LBL_9,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_TOGGLE_SIGN,   SERV_ID_INPT, '±',             0,      10,   233,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      BT_YELLOW_LONG_GR, 0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  10,   233,  75,   38,   18,    6,     40,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_CENTER_Y_CENTER,      LBL_TOGGLE_SIGN,SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_0,             SERV_ID_INPT, '0',             0,      90,   233,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  90,   233,  75,   38,   27,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_CENTER_Y_CENTER,      LBL_0,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_DOT,           SERV_ID_INPT, '.',             0,      170,  233,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      BT_YELLOW_LONG_GR, 0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  170,  233,  75,   38,   32,    10,    8,    8,    GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_24,      _X_CENTER_Y_CENTER,      LBL_DOT,        SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_A,             SERV_ID_INPT, 'A',             0,      10,   233,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  10,   233,  75,   38,   28,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_CENTER_Y_CENTER,      LBL_A,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_B,             SERV_ID_INPT, 'b',             0,      170,  233,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  170,  233,  75,   38,   28,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_CENTER_Y_CENTER,      LBL_B,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_C,             SERV_ID_INPT, 'C',             0,      250,  233,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  250,  233,  75,   38,   28,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_CENTER_Y_CENTER,      LBL_C,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_D,             SERV_ID_INPT, 'd',             0,      250,  191,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  250,  191,  75,   38,   28,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_CENTER_Y_CENTER,      LBL_D,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_E,             SERV_ID_INPT, 'E',             0,      250,  149,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  250,  149,  75,   38,   28,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_CENTER_Y_CENTER,      LBL_E,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_F,             SERV_ID_INPT, 'F',             0,      250,  107,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  250,  107,  75,   38,   28,    7,     20,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_CENTER_Y_CENTER,      LBL_F,          SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_DEL,           SERV_ID_INPT, '<',             0,      400,  149,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      BT_YELLOW_LONG_GR, 0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  400,  149,  75,   38,   10,    7,     60,   26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_CENTER_Y_CENTER,      LBL_DEL,        SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_OK,            SERV_ID_INPT, '@',             0,      400,  191,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      BT_YELLOW_LONG_GR, 23,   4,    GL_OK,              GL_OK,              GL_OK_GR,       400,  191,  75,   38,   0,     0,     0,    0,    0,          0,            0,            INVALID_FONT,     0,                       INVALID_LABEL,  SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_CANCEL,        SERV_ID_INPT, '!',             0,      400,  233,  BT_YELLOW_LONG, BT_YELLOW_LONG_PR,      INVALID_IMAGE,     21,   4,    GL_CANCEL,          GL_CANCEL,          INVALID_IMAGE,  400,  233,  75,   38,   0,     0,     0,    0,    0,          0,            0,            INVALID_FONT,     0,                       INVALID_LABEL,  SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_RANGE,         SERV_ID_BDEF, SLIDING_UP,      0,      40,   70,   BT_MENU,        BT_MENU_PR,             BT_MENU_GR,        122,  12,   GL_RANGE,           GL_RANGE,           INVALID_IMAGE,  40,   70,   158,  45,   12,    13,    100,  12,   BROWN,      BLACK,        0,            FT_ARIAL_10,      0,                       LBL_RANGE,      SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_LIMITS,        SERV_ID_BDEF, SLIDING_UP,      0,      40,   120,  BT_MENU,        BT_MENU_PR,             BT_MENU_GR,        122,  12,   GL_LIMIT,           GL_LIMIT,           INVALID_IMAGE,  40,   120,  158,  45,   12,    13,    100,  12,   BROWN,      BLACK,        0,            FT_ARIAL_10,      0,                       LBL_LIMITS,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_MEASURE,       SERV_ID_BDEF, SLIDING_UP,      0,      40,   170,  BT_MENU,        BT_MENU_PR,             BT_MENU_GR,        122,  12,   GL_MEASURE,         GL_MEASURE,         INVALID_IMAGE,  40,   170,  158,  45,   12,    13,    100,  12,   BROWN,      BLACK,        0,            FT_ARIAL_10,      0,                       LBL_MEASURE,    SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_WARNINGS,      SERV_ID_BDEF, SLIDING_UP,      0,      40,   220,  BT_MENU,        BT_MENU_PR,             BT_MENU_GR,        122,  12,   GL_WARNING,         GL_WARNING,         INVALID_IMAGE,  40,   220,  158,  45,   12,    13,    100,  12,   BROWN,      BLACK,        0,            FT_ARIAL_10,      0,                       LBL_WARNINGS,   SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_REF,           SERV_ID_BDEF, SLIDING_UP,      0,      281,  70,   BT_MENU,        BT_MENU_PR,             BT_MENU_GR,        122,  12,   GL_REFERENCE,       GL_REFERENCE,       INVALID_IMAGE,  281,  70,   158,  45,   12,    13,    100,  12,   BROWN,      BLACK,        0,            FT_ARIAL_10,      0,                       LBL_REFERENCE,  SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_MODULES,       SERV_ID_BDEF, SLIDING_UP,      0,      281,  120,  BT_MENU,        BT_MENU_PR,             BT_MENU_GR,        122,  12,   GL_MODULE,          GL_MODULE,          INVALID_IMAGE,  281,  120,  158,  45,   12,    13,    100,  12,   BROWN,      BLACK,        0,            FT_ARIAL_10,      0,                       LBL_MODULES,    SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_DIAG,          SERV_ID_BDEF, SLIDING_UP,      0,      281,  170,  BT_MENU,        BT_MENU_PR,             BT_MENU_GR,        126,  12,   GL_DIAG,            GL_DIAG,            INVALID_IMAGE,  281,  170,  158,  45,   12,    13,    100,  12,   BROWN,      BLACK,        0,            FT_ARIAL_10,      0,                       LBL_DIAG,       SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_INFO,          SERV_ID_BDEF, SLIDING_UP,      0,      281,  220,  BT_MENU,        BT_MENU_PR,             BT_MENU_GR,        126,  12,   GL_INFO,            GL_INFO,            INVALID_IMAGE,  281,  220,  158,  45,   12,    13,    100,  12,   BROWN,      BLACK,        0,            FT_ARIAL_10,      0,                       LBL_INFO,       SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_XY_MAC,        SERV_ID_MACH, 0,               0,      180,  70,   BT_YELLOW,      BT_YELLOW_PR,           BT_YELLOW_GR,      0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  140,  60,   170,  40,   16,    12,    110,  16,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_LEFT_Y_CENTER,        LBL_XY_MAC,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_ZX_MAC,        SERV_ID_MACH, 1,               0,      180,  120,  BT_YELLOW,      BT_YELLOW_PR,           BT_YELLOW_GR,      0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  140,  110,  170,  40,   16,    12,    110,  16,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_LEFT_Y_CENTER,        LBL_ZX_MAC,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_XYZ_MAC,       SERV_ID_MACH, 2,               0,      180,  170,  BT_YELLOW,      BT_YELLOW_PR,           BT_YELLOW_GR,      0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  140,  160,  170,  40,   16,    12,    110,  16,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_LEFT_Y_CENTER,        LBL_XYZ_MAC,    SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_A_MAC,         SERV_ID_MACH, 3,               0,      350,  70,   BT_YELLOW,      BT_YELLOW_PR,           BT_YELLOW_GR,      0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  310,  60,   170,  40,   16,    12,    110,  16,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_LEFT_Y_CENTER,        LBL_AXIS_A,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_B_MAC,         SERV_ID_MACH, 4,               0,      350,  120,  BT_YELLOW,      BT_YELLOW_PR,           BT_YELLOW_GR,      0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  310,  110,  170,  40,   16,    12,    110,  16,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_LEFT_Y_CENTER,        LBL_AXIS_B,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_C_MAC,         SERV_ID_MACH, 5,               0,      350,  170,  BT_YELLOW,      BT_YELLOW_PR,           BT_YELLOW_GR,      0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  310,  160,  170,  40,   16,    12,    110,  16,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_16,      _X_LEFT_Y_CENTER,        LBL_AXIS_C,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
/* Test Input methods */ \
 X_BTN( BTN_INPUT_1,       SERV_ID_XCHG, CFG_INPUT_5_5_F, 0,      50,   20,   BT_MENU,        BT_MENU_PR,             INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  50,   20,   170,  40,   16,    12,    110,  16,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_10,      _X_LEFT_Y_CENTER,        LBL_TEST_1,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_INPUT_2,       SERV_ID_XCHG, CFG_INPUT_10_D,  0,      50,   60,   BT_MENU,        BT_MENU_PR,             INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  50,   60,   170,  40,   16,    12,    110,  16,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_10,      _X_LEFT_Y_CENTER,        LBL_TEST_2,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_INPUT_3,       SERV_ID_XCHG, CFG_INPUT_6_D,   0,      50,   100,  BT_MENU,        BT_MENU_PR,             INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  50,   100,  170,  40,   16,    12,    110,  16,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_10,      _X_LEFT_Y_CENTER,        LBL_TEST_3,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_INPUT_4,       SERV_ID_XCHG, CFG_INPUT_5_D,   0,      50,   140,  BT_MENU,        BT_MENU_PR,             INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  50,   140,  170,  40,   16,    12,    110,  16,   GRAY_28,    GRAY_20,      GRAY_16,      FT_ARIAL_10,      _X_LEFT_Y_CENTER,        LBL_TEST_4,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
/* Spindle page */ \
 X_BTN( BTN_DOWN_MIN,      SERV_ID_SPIN, 3,               0,      22,   220,  GL_CHECK_BOX,   GL_CHECK_BOX_PR,        GL_CHECK_BOX_GR,   5,    6,    GL_DOWN,            GL_DOWN,            INVALID_IMAGE,  22,    220, 40,   40,   0,     0,     0,    0,    0,          0,            0,            INVALID_FONT,     0,                       INVALID_LABEL,  SERVICE_FILTER_NORMAL_AND_TYPEMATIC,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_UP_MIN,        SERV_ID_SPIN, 4,               0,      192,  220,  GL_CHECK_BOX,   GL_CHECK_BOX_PR,        GL_CHECK_BOX_GR,   5,    5,    GL_UP,              GL_UP,              INVALID_IMAGE,  192,   220, 40,   40,   0,     0,     0,    0,    0,          0,            0,            INVALID_FONT,     0,                       INVALID_LABEL,  SERVICE_FILTER_NORMAL_AND_TYPEMATIC,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_DOWN_MAX,      SERV_ID_SPIN, 5,               0,      242,  220,  GL_CHECK_BOX,   GL_CHECK_BOX_PR,        GL_CHECK_BOX_GR,   5,    6,    GL_DOWN,            GL_DOWN,            INVALID_IMAGE,  242,   220, 40,   40,   0,     0,     0,    0,    0,          0,            0,            INVALID_FONT,     0,                       INVALID_LABEL,  SERVICE_FILTER_NORMAL_AND_TYPEMATIC,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_UP_MAX,        SERV_ID_SPIN, 6,               0,      412,  220,  GL_CHECK_BOX,   GL_CHECK_BOX_PR,        GL_CHECK_BOX_GR,   5,    5,    GL_UP,              GL_UP,              INVALID_IMAGE,  412,   220, 40,   40,   0,     0,     0,    0,    0,          0,            0,            INVALID_FONT,     0,                       INVALID_LABEL,  SERVICE_FILTER_NORMAL_AND_TYPEMATIC,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_BOX_MIN,       SERV_ID_XCHG, CFG_IN_SPIN_MIN, 0,      67,   220,  BT_YELLOW,      BT_YELLOW_PR,           INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  67,    220, 170,  40,   5,     7,     100,  26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_LEFT_Y_CENTER,        LBL_STRING,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_BTN( BTN_BOX_MAX,       SERV_ID_XCHG, CFG_IN_SPIN_MAX, 0,      292,  220,  BT_YELLOW,      BT_YELLOW_PR,           INVALID_IMAGE,     0,    0,    INVALID_IMAGE,      INVALID_IMAGE,      INVALID_IMAGE,  292,   220, 170,  40,   5,     7,     100,  26,   GRAY_28,    GRAY_20,      GRAY_16,      FT_DIGITAL_24,    _X_LEFT_Y_CENTER,        LBL_STRING,     SERVICE_FILTER_NORMAL,  GRAFX_OPTION_TOUCH_RECTANGLE)\

//            Enum ID,      Service,       Sub, Timing, Px,   Py,   Sx,   Sy,   Top Left,    Top Right,    Bottom Left,    Bottom Right,    Horizontal,    Vertical     Filter                 Option
#define BASIC_BOX_DEF(X_BASIC_BOX) \
 X_BASIC_BOX( BOX_T,        SERV_ID_RFSH,  0,   0,      176,  0,    240,  272,  GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_AXIS,     SERV_ID_RFSH,  0,   0,      0,    0,    480,  272,  GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_BASIC_BOX( BOX_EDIT,     SERV_ID_RFSH,  0,   0,      10,   36,   460,  60,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
/* XY or ZX Display Page */ \
 X_BASIC_BOX( BOX_1_3,      SERV_ID_BDEF,  0,   0,      0,    0,    400,  88,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_2_3,      SERV_ID_BDEF,  0,   0,      0,    91,   400,  88,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_3_3,      SERV_ID_BDEF,  0,   0,      0,    183,  400,  89,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
/* XYZ Display Page */ \
 X_BASIC_BOX( BOX_1_4,      SERV_ID_BDEF,  0,   0,      0,    0,    400,  65,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_2_4,      SERV_ID_BDEF,  0,   0,      0,    69,   400,  65,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_3_4,      SERV_ID_BDEF,  0,   0,      0,    138,  400,  65,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_4_4,      SERV_ID_BDEF,  0,   0,      0,    207,  400,  65,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
/* XYT Display Page */ \
 X_BASIC_BOX( BOX_1_3T,     SERV_ID_BDEF,  0,   0,      0,    0,    174,  88,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_2_3T,     SERV_ID_BDEF,  0,   0,      0,    91,   174,  88,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_3_3T,     SERV_ID_BDEF,  0,   0,      0,    183,  174,  89,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
/* XYZT Display Page */ \
 X_BASIC_BOX( BOX_1_4T,     SERV_ID_BDEF,  0,   0,      0,    0,    174,  65,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_2_4T,     SERV_ID_BDEF,  0,   0,      0,    69,   174,  65,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_3_4T,     SERV_ID_BDEF,  0,   0,      0,    138,  174,  65,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\
 X_BASIC_BOX( BOX_4_4T,     SERV_ID_BDEF,  0,   0,      0,    207,  174,  65,   GL_TOP_LEFT, GL_TOP_RIGHT, GL_BOTTOM_LEFT, GL_BOTTOM_RIGHT, GL_HORIZONTAL, GL_VERTICAL, SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE | GRAFX_OPTION_DRAW_ONCE)\

#define GRAPH_DEF(X_BASIC_SHAPE) \
 X_GRAPH( GRAPH_TEST_T,     SERV_ID_RFSH,  0,   0,      100,  10,   240,  110, LIGHT_GREEN, 20,     20,     200,     100,     2,      199,    GRAFX_OPTION_TOUCH_RECTANGLE)\

//            Enum ID,      Type,           Px1,   Py1,   Px2,   Py2,   Px3,   Py3,   Px4,   Py4,  Color,       Thickness,  Options
#define BASIC_SHAPE_DEF() \
 X_BASIC_SHAPE( LINE_TITLE, BASIC_LINE,     10,    34,    470,   34,    0,     0,     0,     0,    GREEN,       2,          GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\

//      Enum ID,                 Service,      Sub, Timing, Px,   Py,   Sx,   Sy,   TC,          TAC,         TAC2,    Font ID,        Text Blend,   Text Label             Text Option         Option
#define LABEL_DEF(X_LBL) \
 X_LBL( LABEL_TIME,              SERV_ID_TIME, 0,   0,      20,   7,    200,  16,   WHITE,       GREEN,       0,       SYS_FT_16,      CLEAR_BLEND,  LBL_TIME,              _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_DATE,              SERV_ID_DATE, 0,   0,      20,   67,   200,  16,   WHITE,       GREEN,       0,       SYS_FT_16,      CLEAR_BLEND,  LBL_DATE,              _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SKIN_LOADING,      SERV_ID_SKLD, 0,   0,      134,  222,  300,  16,   GRAY_16,     GRAY_16,     GRAY_16, SYS_FT_16,      CLEAR_BLEND,  LBL_SKIN_LOADING,      _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE)\
 X_LBL( LABEL_PRINT_NUMBER,      SERV_ID_INPT, 'P', 0,      16,   48,   448,  36,   GRAY_28,     RED,         0,       FT_DIGITAL_36,  CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_INPT_TITLE,        SERV_ID_INPT, 'L', 0,      16,   2,    448,  16,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_STRING,            _X_CENTER_Y_CENTER, GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_MACHINE_SELECT,    SERV_ID_RFSH, 0,   0,      20,   20,   200,  24,   GRAY_28,     0,           0,       FT_ARIAL_24,    ALPHA_BLEND,  LBL_MAC_SELECT,        _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
/* XY XZ XYZ XYZA Display Page */ \
 X_LBL( LABEL_PAGE_MAIN_MENU,    SERV_ID_RFSH, 0,   0,      20,   10,   200,  24,   GRAY_28,     0,           0,       FT_ARIAL_24,    ALPHA_BLEND,  LBL_MAIN_MENU,         _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_PAGE_INPUT_CHECK,  SERV_ID_RFSH, 0,   0,      20,   10,   300,  24,   GRAY_28,     0,           0,       FT_ARIAL_24,    ALPHA_BLEND,  LBL_INPUT_CHECK,       _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_CHECK_LIMIT_X,     SERV_ID_RFSH, 0,   0,      30,   60,   200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_LIMIT_X,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_CHECK_LIMIT_Y,     SERV_ID_RFSH, 0,   0,      30,   100,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_LIMIT_Y,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_CHECK_LIMIT_Z,     SERV_ID_RFSH, 0,   0,      30,   140,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_LIMIT_Z,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_CHECK_LIMIT_A,     SERV_ID_RFSH, 0,   0,      270,  60,   200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_LIMIT_A,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_CHECK_LIMIT_B,     SERV_ID_RFSH, 0,   0,      270,  100,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_LIMIT_B,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_CHECK_LIMIT_C,     SERV_ID_RFSH, 0,   0,      270,  140,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_LIMIT_C,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_CHECK_ABORT,       SERV_ID_RFSH, 0,   0,      30,   180,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_INPUT_ABORT,       _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_CHECK_RESUME,      SERV_ID_RFSH, 0,   0,      270,  180,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_INPUT_RESUME,      _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_CHECK_HOLD,        SERV_ID_RFSH, 0,   0,      30,   220,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_INPUT_HOLD,        _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_CHECK_PROBE,       SERV_ID_RFSH, 0,   0,      270,  220,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_INPUT_PROBE,       _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_ENABLE,            SERV_ID_RFSH, 0,   0,      420,  80,   58,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_ENABLE,            _X_CENTER_Y_CENTER, GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_COOLANT,           SERV_ID_RFSH, 0,   0,      420,  140,  58,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_COOLANT,           _X_CENTER_Y_CENTER, GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_MIST,              SERV_ID_RFSH, 0,   0,      420,  200,  58,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_MIST,              _X_CENTER_Y_CENTER, GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
/* XY or ZX Display Page */ \
 X_LBL( LABEL_AXIS_X_1_3,        SERV_ID_RFSH, 0,   0,      10,   10,   50,   20,   GRAY_28,     0,           0,       FT_ARIAL_16,    CLEAR_BLEND,  LBL_AXIS_X,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_Y_2_3,        SERV_ID_RFSH, 0,   0,      10,   100,  50,   20,   GRAY_28,     0,           0,       FT_ARIAL_16,    CLEAR_BLEND,  LBL_AXIS_Y,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_Z_2_3,        SERV_ID_RFSH, 0,   0,      10,   100,  50,   20,   GRAY_28,     0,           0,       FT_ARIAL_16,    CLEAR_BLEND,  LBL_AXIS_Z,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SPEED_3_3_BACK,    SERV_ID_RFSH, 0,   0,      176,  190,  270,  66,   GRAY_8,      0,           0,       FT_DIGITAL_64,  ALPHA_BLEND,  LBL_SPEED_BACK,        _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SPINDLE_3_3,       SERV_ID_RFSH, 0,   0,      10,   190,  100,  20,   GRAY_28,     0,           0,       FT_ARIAL_16,    CLEAR_BLEND,  LBL_SPINDLE,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_RPM_3_3,           SERV_ID_RFSH, 0,   0,      360,  249,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_RPM,               _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_X_1_3,             SERV_ID_AXIS, 0,   0,      88,   10,   272,  66,   0,           0,           0,       FT_DIGITAL_64,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_Y_2_3,             SERV_ID_AXIS, 1,   0,      88,   100,  272,  66,   0,           0,           0,       FT_DIGITAL_64,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_Z_2_3,             SERV_ID_AXIS, 2,   0,      88,   100,  272,  66,   0,           0,           0,       FT_DIGITAL_64,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SPEED_3_3,         SERV_ID_SPIN, 0,   0,      176,  190,  186,  66,   LIGHT_GREEN, 0,           0,       FT_DIGITAL_64,  CLEAR_BLEND,  LBL_SPEED,             _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_x_1_3,             SERV_ID_AXIS, 10,  0,      10,   66,   78,   14,   0,           0,           0,       FT_DIGITAL_16,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_y_2_3,             SERV_ID_AXIS, 11,  0,      10,   156,  78,   14,   0,           0,           0,       FT_DIGITAL_16,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_z_2_3,             SERV_ID_AXIS, 12,  0,      10,   156,  78,   14,   0,           0,           0,       FT_DIGITAL_16,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_X_1_3,     SERV_ID_INCH, 0,   0,      360,  69,   34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_Y_2_3,     SERV_ID_INCH, 0,   0,      360,  159,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_Z_2_3,     SERV_ID_INCH, 0,   0,      360,  159,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_X_1_3,   SERV_ID_COOR, 0,   0,      360,  10,   34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_Y_2_3,   SERV_ID_COOR, 1,   0,      360,  100,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_Z_2_3,   SERV_ID_COOR, 2,   0,      360,  100,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_x_1_3,   SERV_ID_COOR, 10,  0,      86,   69,   34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_y_2_3,   SERV_ID_COOR, 11,  0,      86,   159,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_z_2_3,   SERV_ID_COOR, 12,  0,      86,   159,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
/* XYZ Display Page */ \
 X_LBL( LABEL_AXIS_X_1_4,        SERV_ID_RFSH, 0,   0,      10,   14,   50,   20,   GRAY_28,     0,           0,       FT_ARIAL_16,    CLEAR_BLEND,  LBL_AXIS_X,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_Y_2_4,        SERV_ID_RFSH, 0,   0,      10,   82,   50,   20,   GRAY_28,     0,           0,       FT_ARIAL_16,    CLEAR_BLEND,  LBL_AXIS_Y,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_Z_3_4,        SERV_ID_RFSH, 0,   0,      10,   150,  50,   20,   GRAY_28,     0,           0,       FT_ARIAL_16,    CLEAR_BLEND,  LBL_AXIS_Z,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SPEED_4_4_BACK,    SERV_ID_RFSH, 0,   0,      200,  212,  158,  50,   GRAY_8,      0,           0,       FT_DIGITAL_48,  ALPHA_BLEND,  LBL_SPEED_BACK,        _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SPINDLE_4_4,       SERV_ID_RFSH, 0,   0,      10,   218,  100,  20,   GRAY_28,     0,           0,       FT_ARIAL_16,    CLEAR_BLEND,  LBL_SPINDLE,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_RPM_4_4,           SERV_ID_RFSH, 0,   0,      360,  244,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_RPM,               _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_X_1_4,             SERV_ID_AXIS, 0,   0,      128,  8,    228,  50,   0,           0,           0,       FT_DIGITAL_48,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_Y_2_4,             SERV_ID_AXIS, 1,   0,      128,  76,   228,  50,   0,           0,           0,       FT_DIGITAL_48,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_Z_3_4,             SERV_ID_AXIS, 2,   0,      128,  144,  228,  50,   0,           0,           0,       FT_DIGITAL_48,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SPEED_4_4,         SERV_ID_SPIN, 0,   0,      200,  212,  156,  50,   LIGHT_GREEN, 0,           0,       FT_DIGITAL_48,  CLEAR_BLEND,  LBL_SPEED,             _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_x_1_4,             SERV_ID_AXIS, 10,  0,      10,   37,   78,   14,   0,           0,           0,       FT_DIGITAL_16,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_y_2_4,             SERV_ID_AXIS, 11,  0,      10,   105,  78,   14,   0,           0,           0,       FT_DIGITAL_16,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_z_3_4,             SERV_ID_AXIS, 12,  0,      10,   173,  78,   14,   0,           0,           0,       FT_DIGITAL_16,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_X_1_4,     SERV_ID_INCH, 0,   0,      360,  40,   34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_Y_2_4,     SERV_ID_INCH, 0,   0,      360,  108,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_Z_3_4,     SERV_ID_INCH, 0,   0,      360,  176,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_X_1_4,   SERV_ID_COOR, 0,   0,      360,  10,   34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_Y_2_4,   SERV_ID_COOR, 1,   0,      360,  78,   34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_Z_3_4,   SERV_ID_COOR, 2,   0,      360,  146,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_x_1_4,   SERV_ID_COOR, 10,  0,      86,   40,   34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_y_2_4,   SERV_ID_COOR, 11,  0,      86,   108,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_z_3_4,   SERV_ID_COOR, 12,  0,      86,   176,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
/* XYT or ZXT Display Page */ \
 X_LBL( LABEL_AXIS_X_1_3T,       SERV_ID_RFSH, 0,   0,      4,    10,   30,   20,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_AXIS_X,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_Y_2_3T,       SERV_ID_RFSH, 0,   0,      4,    101,  30,   20,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_AXIS_Y,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_Z_2_3T,       SERV_ID_RFSH, 0,   0,      4,    101,  30,   20,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_AXIS_Z,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SPEED_3_3T_BACK,   SERV_ID_RFSH, 0,   0,      56,   212,  112,  38,   GRAY_8,      0,           0,       FT_DIGITAL_36,  ALPHA_BLEND,  LBL_SPEED_BACK,        _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SPINDLE_3_3T,      SERV_ID_RFSH, 0,   0,      10,   190,  30,   20,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_SPINDLE,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_RPM_3_3T,          SERV_ID_RFSH, 0,   0,      136,  251,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_RPM,               _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_X_1_3T,            SERV_ID_AXIS, 0,   0,      4,    30,   164,  38,   0,           0,           0,       FT_DIGITAL_36,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_Y_2_3T,            SERV_ID_AXIS, 1,   0,      4,    121,  164,  38,   0,           0,           0,       FT_DIGITAL_36,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_Z_2_3T,            SERV_ID_AXIS, 2,   0,      4,    121,  164,  38,   0,           0,           0,       FT_DIGITAL_36,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SPEED_3_3T,        SERV_ID_SPIN, 0,   0,      56,   212,  112,  38,   LIGHT_GREEN, 0,           0,       FT_DIGITAL_36,  CLEAR_BLEND,  LBL_SPEED,             _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_X_1_3T,    SERV_ID_INCH, 0,   0,      120,  69,   50,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_Y_2_3T,    SERV_ID_INCH, 0,   0,      120,  160,  50,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_Z_2_3T,    SERV_ID_INCH, 0,   0,      120,  160,  50,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_X_1_3T,  SERV_ID_COOR, 0,   0,      120,  12,   50,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_Y_2_3T,  SERV_ID_COOR, 1,   0,      120,  103,  50,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_Z_2_3T,  SERV_ID_COOR, 2,   0,      120,  103,  50,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
/* XYZT Display Page */ \
 X_LBL( LABEL_AXIS_X_1_4T,       SERV_ID_RFSH, 0,   0,      4,    8,    30,   20,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_AXIS_X,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_Y_2_4T,       SERV_ID_RFSH, 0,   0,      4,    77,   30,   20,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_AXIS_Y,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_Z_3_4T,       SERV_ID_RFSH, 0,   0,      4,    146,  30,   20,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_AXIS_Z,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SPEED_4_4T_BACK,   SERV_ID_RFSH, 0,   0,      92,   217,  78,   26,   GRAY_8,      0,           0,       FT_DIGITAL_24,  ALPHA_BLEND,  LBL_SPEED_BACK,        _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SPINDLE_4_4T,      SERV_ID_RFSH, 0,   0,      4,    215,  30,   20,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_SPINDLE,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_RPM_4_4T,          SERV_ID_RFSH, 0,   0,      136,  247,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_RPM,               _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_X_1_4T,            SERV_ID_AXIS, 0,   0,      56,   10,   114,  26,   0,           0,           0,       FT_DIGITAL_24,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_Y_2_4T,            SERV_ID_AXIS, 1,   0,      56,   79,   114,  26,   0,           0,           0,       FT_DIGITAL_24,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_Z_3_4T,            SERV_ID_AXIS, 2,   0,      56,   148,  114,  26,   0,           0,           0,       FT_DIGITAL_24,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SPEED_4_4T,        SERV_ID_SPIN, 0,   0,      92,   217,  78,   26,   LIGHT_GREEN, 0,           0,       FT_DIGITAL_24,  CLEAR_BLEND,  LBL_SPEED,             _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_X_1_4T,    SERV_ID_INCH, 0,   0,      120,  40,   50,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_Y_2_4T,    SERV_ID_INCH, 0,   0,      120,  109,  50,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_Z_3_4T,    SERV_ID_INCH, 0,   0,      120,  178,  50,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_RIGHT_Y_CENTER,  GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_X_1_4T,  SERV_ID_COOR, 0,   0,      82,   40,   30,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_Y_2_4T,  SERV_ID_COOR, 1,   0,      82,   108,  30,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_Z_3_4T,  SERV_ID_COOR, 2,   0,      82,   178,  30,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
/* Information Page */ \
 X_LBL( LABEL_INFO,              SERV_ID_RFSH, 0,   0,      10,   20,   200,  10,   GRAY_28,     0,           0,       FT_ARIAL_24,    ALPHA_BLEND,  LBL_PAGE_INFO,         _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_FW_NAME_INFO,      SERV_ID_RFSH, 0,   0,      10,   60,   200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_FW_NAME_INFO,      _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_FW_VERSION_INFO,   SERV_ID_RFSH, 0,   0,      10,   90,   200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_FW_VERSION_INFO,   _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_GUI_NAME_INFO,     SERV_ID_RFSH, 0,   0,      10,   120,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_GUI_NAME_INFO,     _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_GUI_VERSION_INFO,  SERV_ID_RFSH, 0,   0,      10,   150,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_GUI_VERSION_INFO,  _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_HARDWARE_INFO,     SERV_ID_RFSH, 0,   0,      10,   180,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_HARDWARE_INFO,     _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SERIAL_INFO,       SERV_ID_RFSH, 0,   0,      10,   210,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_SERIAL_INFO,       _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_COMPILE_DATE_INFO, SERV_ID_RFSH, 0,   0,      10,   240,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_COMPILE_DATE_INFO, _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_INFO_FIRMWARE,     SERV_ID_INFO, 0,   0,      200,  60,   200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_INFO_VERSION,      SERV_ID_INFO, 1,   0,      200,  90,   200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_INFO_GUI_FIRMWARE, SERV_ID_INFO, 2,   0,      200,  120,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_INFO_GUI_VERSION,  SERV_ID_INFO, 3,   0,      200,  150,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_INFO_HARDWARE,     SERV_ID_INFO, 4,   0,      200,  180,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_INFO_SERIAL,       SERV_ID_INFO, 5,   0,      200,  210,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_INFO_DATE,         SERV_ID_INFO, 6,   0,      200,  240,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_16,    ALPHA_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
/* Axis X Page */ \
 X_LBL( LABEL_PAGE_AXIS_X,       SERV_ID_RFSH, 0,   0,      10,   20,   100,  26,   GRAY_28,     0,           0,       FT_ARIAL_24,    CLEAR_BLEND,  LBL_AXIS_X,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_X,            SERV_ID_AXIS, 0,   0,      4,    80,   300,  66,   0,           0,           0,       FT_DIGITAL_64,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_X,         SERV_ID_INCH, 0,   0,      360,  40,   34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_X,       SERV_ID_COOR, 0,   0,      82,   40,   30,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_x,       SERV_ID_COOR, 10,  0,      86,   40,   34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
/* Axis Y Page */ \
 X_LBL( LABEL_PAGE_AXIS_Y,       SERV_ID_RFSH, 0,   0,      10,   20,   100,  26,   GRAY_28,     0,           0,       FT_ARIAL_24,    CLEAR_BLEND,  LBL_AXIS_Y,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_Y,            SERV_ID_AXIS, 1,   0,      4,    80,   300,  66,   0,           0,           0,       FT_DIGITAL_64,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_Y,         SERV_ID_INCH, 0,   0,      360,  108,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_Y,       SERV_ID_COOR, 1,   0,      82,   108,  30,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_y,       SERV_ID_COOR, 11,  0,      86,   108,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
/* Axis Z Page */ \
 X_LBL( LABEL_PAGE_AXIS_Z,       SERV_ID_RFSH, 0,   0,      10,   20,   100,  26,   GRAY_28,     0,           0,       FT_ARIAL_24,    CLEAR_BLEND,  LBL_AXIS_Z,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_AXIS_Z,            SERV_ID_AXIS, 2,   0,      4,    80,   300,  66,   0,           0,           0,       FT_DIGITAL_64,  CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_MM_INCH_Z,         SERV_ID_INCH, 0,   0,      360,  176,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_Z,       SERV_ID_COOR, 2,   0,      82,   178,  30,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_SYS_COORD_z,       SERV_ID_COOR, 12,  0,      86,   176,  34,   10,   GRAY_28,     0,           0,       FT_ARIAL_10,    CLEAR_BLEND,  LBL_STRING,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
/* Spindle Page */ \
 X_LBL( LABEL_PAGE_SPINDLE,      SERV_ID_RFSH, 0,   0,      10,   20,   100,  26,   GRAY_28,     0,           0,       FT_ARIAL_24,    CLEAR_BLEND,  LBL_SPINDLE,           _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SPIN_SPEED_BACK,   SERV_ID_RFSH, 0,   0,      140,  60,   230,  66,   GRAY_8,      0,           0,       FT_DIGITAL_64,  ALPHA_BLEND,  LBL_SPEED_BACK,        _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
 X_LBL( LABEL_SPIN_SPEED,        SERV_ID_SPIN, 0,   0,      140,  60,   230,  66,   LIGHT_GREEN, 0,           0,       FT_DIGITAL_64,  CLEAR_BLEND,  LBL_SPEED,             _X_LEFT_Y_CENTER,   GRAFX_OPTION_NONE)\
 X_LBL( LABEL_RPM,               SERV_ID_RFSH, 0,   0,      356,  102,  50,   18,   GRAY_28,     0,           0,       FT_ARIAL_16,    CLEAR_BLEND,  LBL_RPM,               _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
/* X, Y, Z single Axis Pages */ \
 X_LBL( LABEL_AXIS_LIMIT_BACK,   SERV_ID_RFSH, 0,   0,      100,  30,   100,  26,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_LIMITS,            _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE)\
/* Test Input methods */ \


//X_LBL(   LABEL_SAFETY_DOOR,     SERV_ID_RFSH, 0,   0,      270,  190,  200,  10,   GRAY_28,     0,           0,       FT_ARIAL_10,    ALPHA_BLEND,  LBL_INPUT_DOOR,   _X_LEFT_Y_CENTER,   GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)

//             Enum ID,                 Service,      Sub, Timing, Px,   Py,   Sx,   Sy,   TC,         TAC,       TAC2,        Font ID,        Text Blend,  State 0,          State 1,            State 2,       State 3,        Text Option,       Option
#define LABEL_LIST_DEF(X_LABEL_LIST) \
 X_LABEL_LIST( LABEL_AXIS_X_1_3_BACK,   SERV_ID_INCH, 1,   0,      88,   10,   272,  66,   GRAY_8,     0,         0,           FT_DIGITAL_64,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_Y_2_3_BACK,   SERV_ID_INCH, 1,   0,      88,   100,  272,  66,   GRAY_8,     0,         0,           FT_DIGITAL_64,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_Z_2_3_BACK,   SERV_ID_INCH, 1,   0,      88,   100,  272,  66,   GRAY_8,     0,         0,           FT_DIGITAL_64,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_X_1_4_BACK,   SERV_ID_INCH, 1,   0,      128,  8,    230,  50,   GRAY_8,     0,         0,           FT_DIGITAL_48,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_Y_2_4_BACK,   SERV_ID_INCH, 1,   0,      128,  76,   230,  50,   GRAY_8,     0,         0,           FT_DIGITAL_48,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_Z_3_4_BACK,   SERV_ID_INCH, 1,   0,      128,  144,  230,  50,   GRAY_8,     0,         0,           FT_DIGITAL_48,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_X_1_5_BACK,   SERV_ID_INCH, 1,   0,      158,  8,    170,  38,   GRAY_8,     0,         0,           FT_DIGITAL_36,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_Y_2_5_BACK,   SERV_ID_INCH, 1,   0,      158,  62,   170,  38,   GRAY_8,     0,         0,           FT_DIGITAL_36,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_Z_3_5_BACK,   SERV_ID_INCH, 1,   0,      158,  116,  170,  38,   GRAY_8,     0,         0,           FT_DIGITAL_36,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
/* XYT or ZXT Display Pages */ \
 X_LABEL_LIST( LABEL_AXIS_X_1_3T_BACK,  SERV_ID_INCH, 1,   0,      4,    30,   164,  38,   GRAY_8,     0,         0,           FT_DIGITAL_36,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_Y_2_3T_BACK,  SERV_ID_INCH, 1,   0,      4,    121,  164,  38,   GRAY_8,     0,         0,           FT_DIGITAL_36,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_Z_2_3T_BACK,  SERV_ID_INCH, 1,   0,      4,    121,  164,  38,   GRAY_8,     0,         0,           FT_DIGITAL_36,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
/* XYZT Display Page */ \
 X_LABEL_LIST( LABEL_AXIS_X_1_4T_BACK,  SERV_ID_INCH, 1,   0,      56,   10,   50,   26,   GRAY_8,     0,         0,           FT_DIGITAL_24,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_Y_2_4T_BACK,  SERV_ID_INCH, 1,   0,      56,   79,   50,   26,   GRAY_8,     0,         0,           FT_DIGITAL_24,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
 X_LABEL_LIST( LABEL_AXIS_Z_3_4T_BACK,  SERV_ID_INCH, 1,   0,      56,   148,  50,   26,   GRAY_8,     0,         0,           FT_DIGITAL_24,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\
/* X, Y, Z single Axis Pages */ \
 X_LABEL_LIST( LABEL_AXIS_BACK,         SERV_ID_INCH, 1,   0,      4,    80,   300,  66,   GRAY_8,     0,         0,           FT_DIGITAL_64,  ALPHA_BLEND, LBL_AXIS_MM_BACK, LBL_AXIS_INCH_BACK, INVALID_LABEL, INVALID_LABEL,  _X_LEFT_Y_CENTER,  GRAFX_OPTION_DRAW_ONCE_AND_REMOVE | GRAFX_OPTION_DRAW_ON_BACK)\

//           Enum ID,            Service,      Sub, Timing, Px,  Py,  Sx,  Sy,  Background IMG, Bar IMG        Cursor Image,     COx,  COy,  CSV,  CEV,  CPR,  TPx,  TPy,  TSx,  TSy,  TC,      TAC,     TAC2,     Font ID,   Text Option,         Text Label,        Option
#define PROGRESS_DEF(X_PROGRESS) \
 X_PROGRESS( PROGRESS_LOADING,   SERV_ID_SKLD, 2,   0,      25,  159, 430, 43,  INVALID_IMAGE,  LOADING_BAR,   LOADING_CURSOR,   20,   13,   0,    100,  384,  260,  63,   48,   16,   GRAY_16, GRAY_16, GRAY_16,  SYS_FT_16, _X_CENTER_Y_CENTER,  LBL_PERCENT,       GRAFX_OPTION_ORIENTATION_HORIZONTAL | GRAFX_OPTION_BLEND_CLEAR)\

// TODO change... and dont use hub for single link ( all widget can return a new link...
//      Enum ID,                Service,        Sub, Timing, LinkList
#define VIRTUAL_HUB_DEF(X_HUB) \
 X_HUB( VIRTUAL_MAIN_HUB,       SERV_ID_MHUB,   0,   0,      LINK_MAIN_PAGE_XY,  LINK_MAIN_PAGE_ZX,  LINK_MAIN_PAGE_XYZ,  LINK_MAIN_PAGE_XYT,  LINK_MAIN_PAGE_ZXT,  LINK_MAIN_PAGE_XYZT,  INVALID_LINK,  INVALID_LINK )\
/* X_HUB( VIRTUAL_JOG_HUB,       SERV_ID_JHUB,   0,   0,      LINK_JOG_PAGE_XY,  LINK_JOG_PAGE_ZX,  LINK_JOG_PAGE_XYZ,  INVALID_LINK,  INVALID_LINK,  INVALID_LINK,  INVALID_LINK,  INVALID_LINK )*/\

//       Enum ID,               Service,      Sub, Timing, Px,  Py,       Sx,   Sy,  Icon 0,           Icon 1,          Icon 2,        Icon 3,            Option,
#define ICON_DEF(X_ICON) \
 X_ICON( ICON_LED_SPINDLE_3_3,  SERV_ID_ENBL, 9,   0,      140, 212,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_BLEND_CLEAR )\
 X_ICON( ICON_LED_SPINDLE_4_4,  SERV_ID_ENBL, 9,   0,      180, 225,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_BLEND_CLEAR )\
 X_ICON( ICON_LED_LIMIT_X_3,    SERV_ID_INPU, 0,   0,      90,  11,       0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_X_4,    SERV_ID_INPU, 0,   0,      90,  15,       0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_X_3T,   SERV_ID_INPU, 0,   0,      6,   60,       0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_X_4T,   SERV_ID_INPU, 0,   0,      10,  38,       0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Y_3,    SERV_ID_INPU, 1,   0,      90,  101,      0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Y_4,    SERV_ID_INPU, 1,   0,      90,  83,       0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Y_3T,   SERV_ID_INPU, 1,   0,      6,   151,      0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Y_4T,   SERV_ID_INPU, 1,   0,      10,  107,      0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Z_3,    SERV_ID_INPU, 2,   0,      90,  101,      0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Z_4,    SERV_ID_INPU, 2,   0,      90,  151,      0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Z_3T,   SERV_ID_INPU, 2,   0,      6,   151,      0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Z_4T,   SERV_ID_INPU, 2,   0,      10,  176,      0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
/* XYT or ZXT Display Pages */ \
 X_ICON( ICON_LED_SPINDLE_3_3T, SERV_ID_ENBL, 9,   0,      20, 212,       0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_BLEND_CLEAR )\
/* XYZT Display Pages */ \
 X_ICON( ICON_LED_SPINDLE_4_4T, SERV_ID_ENBL, 9,   0,      20,  240,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_BLEND_CLEAR )\
/* All icon machine are draw on top of each other, so first one will erase area, all other are merge, single draw until ask to be redraw with service SERVICE_REDRAW */\
 X_ICON( ICON_MACHINE_XY,       SERV_ID_MACH, 20,  0,      18,  80 + 57,  0,    0,   INVALID_IMAGE,    GL_XY,           INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_MACHINE_ZX,       SERV_ID_MACH, 21,  0,      18,  80 + 50,  0,    0,   INVALID_IMAGE,    GL_ZX,           INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_MACHINE_Z,        SERV_ID_MACH, 22,  0,      18,  80,       0,    0,   INVALID_IMAGE,    GL_Z,            INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_MACHINE_A,        SERV_ID_MACH, 23,  0,      42,  80 + 101, 0,    0,   INVALID_IMAGE,    GL_A,            INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_MACHINE_B,        SERV_ID_MACH, 24,  0,      60,  80 + 67,  0,    0,   INVALID_IMAGE,    GL_B,            INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_MACHINE_C,        SERV_ID_MACH, 25,  0,      10,  80 + 35,  0,    0,   INVALID_IMAGE,    GL_C,            INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_MACHINE_A2,       SERV_ID_MACH, 26,  0,      60,  80 + 67,  0,    0,   INVALID_IMAGE,    GL_A2,           INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_MACHINE_C2,       SERV_ID_MACH, 27,  0,      41,  80 + 101, 0,    0,   INVALID_IMAGE,    GL_C2,           INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
/* Icon leds for diagnostic page */\
 X_ICON( ICON_LED_LIMIT_X,      SERV_ID_INPU, 0,   0,      10,  60,       0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Y,      SERV_ID_INPU, 1,   0,      10,  100,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Z,      SERV_ID_INPU, 2,   0,      10,  140,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_A,      SERV_ID_INPU, 3,   0,      250, 60,       0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_B,      SERV_ID_INPU, 4,   0,      250, 100,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_C,      SERV_ID_INPU, 5,   0,      250, 140,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_ABORT,        SERV_ID_INPU, 6,   0,      10,  180,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_RESUME,       SERV_ID_INPU, 7,   0,      250, 180,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_HOLD,         SERV_ID_INPU, 8,   0,      10,  220,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_PROBE,        SERV_ID_INPU, 9,   0,      250, 220,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
/* Check box and hook for machine selection page */\
 X_ICON( ICON_XY_MAC,           SERV_ID_RFSH, 0,   0,      140,  70,      40,   40,  GL_CHECK_BOX,     INVALID_IMAGE,   INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_CLEAR )\
 X_ICON( ICON_ZX_MAC,           SERV_ID_RFSH, 0,   0,      140,  120,     40,   40,  GL_CHECK_BOX,     INVALID_IMAGE,   INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_CLEAR )\
 X_ICON( ICON_XYZ_MAC,          SERV_ID_RFSH, 0,   0,      140,  170,     40,   40,  GL_CHECK_BOX,     INVALID_IMAGE,   INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_CLEAR )\
 X_ICON( ICON_A_MAC,            SERV_ID_RFSH, 0,   0,      310,  70,      40,   40,  GL_CHECK_BOX,     INVALID_IMAGE,   INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_CLEAR )\
 X_ICON( ICON_B_MAC,            SERV_ID_RFSH, 0,   0,      310,  120,     40,   40,  GL_CHECK_BOX,     INVALID_IMAGE,   INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_CLEAR )\
 X_ICON( ICON_C_MAC,            SERV_ID_RFSH, 0,   0,      310,  170,     40,   40,  GL_CHECK_BOX,     INVALID_IMAGE,   INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_CLEAR )\
 X_ICON( ICON_CHECK_XY,         SERV_ID_MACH, 10,  0,      146,  70,      0,    0,   INVALID_IMAGE,    GL_OK,           INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_CHECK_ZX,         SERV_ID_MACH, 11,  0,      146,  120,     0,    0,   INVALID_IMAGE,    GL_OK,           INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_CHECK_XYZ,        SERV_ID_MACH, 12,  0,      146,  170,     0,    0,   INVALID_IMAGE,    GL_OK,           INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_CHECK_A,          SERV_ID_MACH, 13,  0,      316,  70,      0,    0,   INVALID_IMAGE,    GL_OK,           INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_CHECK_B,          SERV_ID_MACH, 14,  0,      316,  120,     0,    0,   INVALID_IMAGE,    GL_OK,           INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_CHECK_C,          SERV_ID_MACH, 15,  0,      316,  170,     0,    0,   INVALID_IMAGE,    GL_OK,           INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_ENABLE,       SERV_ID_RFSH, 0,   0,      440,  100,     0,    0,   GL_LED_RED_OFF,   GL_LED_RED_ON,   GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_COOLANT,      SERV_ID_RFSH, 0,   0,      440,  160,     0,    0,   GL_LED_RED_OFF,   GL_LED_RED_ON,   GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_MIST,         SERV_ID_RFSH, 0,   0,      440,  220,     0,    0,   GL_LED_RED_OFF,   GL_LED_RED_ON,   GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
/* LED limit for Pages AXIS */\
 X_ICON( ICON_LED_LIMIT_X_AXIS, SERV_ID_INPU, 0,   0,      200,  30,      0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Y_AXIS, SERV_ID_INPU, 1,   0,      200,  30,      0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
 X_ICON( ICON_LED_LIMIT_Z_AXIS, SERV_ID_INPU, 2,   0,      200,  30,      0,    0,   GL_LED_RED_ON,    GL_LED_RED_OFF,  GL_LED_GRAY_OFF,  INVALID_IMAGE,  GRAFX_OPTION_NONE )\
/* Spindle Page */\
 X_ICON( ICON_LED_SPINDLE,      SERV_ID_ENBL, 9,   0,      90,   86,      0,    0,   GL_LED_GREEN_OFF, GL_LED_GREEN_ON, INVALID_IMAGE,    INVALID_IMAGE,  GRAFX_OPTION_BLEND_CLEAR )\

//             Enum ID,               Service,      Sub, Timing, Px,  Py,  Sx,  Sy,  Color,    Filter,                 Options
#define BASIC_RECT_DEF(X_BASIC_RECT) \
 X_BASIC_RECT( RECT_BLANK_MACHINE,    SERV_ID_RFSH, 0,   0,      10,  80,  120, 150, BLACK,    SERVICE_FILTER_NORMAL,  GRAFX_OPTION_NONE   ) \

//            Enum ID,          Service,       Sub, Timing, Px,  Py,  Sx,  Sy,  TC,          Font ID,      Filter,                Options
#define TERMINAL_DEF(X_TERMINAL) \
 X_TERMINAL ( TERM_GCODE,       SERV_ID_GCOD,  0,   0,      182, 5,   228, 262, LIGHT_GREEN, FT_TERMINAL,  SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE)\
 X_TERMINAL ( TERM_GCODE_FULL,  SERV_ID_GCOD,  1,   0,      0,   0,   430, 272, LIGHT_GREEN, FT_ARIAL_10,  SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE)\

//              Enum ID,         Service,       Sub, Timing, Px,  Py,  Sx,  Sy,  Slide Direction,         Filter,                Options

#if 0

// Kept for reference

#define PAGE_SLIDE_DEF \
 X_PAGE_SLIDE ( PS_MAIN_AXIS_X,  SERV_ID_NONE,  0,   0,      182, 5,   228, 262, SLIDING_LEFT_AND_RIGHT,  SERVICE_FILTER_NORMAL, GRAFX_OPTION_TOUCH_RECTANGLE)\

//                                    Enum ID,                Service,         SubService,  Timing  Pos X,  Pos Y,  Size X,  Size Y,  Thickness,  Back Color,  Press Back Color,  Box Color,   Press Box Color,  Text PosX,  Text PosY,  Text Size X,  Text Size Y,  Text Color,  pressed Text Color,  Font ID,         Text Option,              Text Label,  Filter
#define BASIC_BTN_DEF   X_BASIC_BTN(  BASIC_BUTTON_OK,        SERV_ID_BDEF,    0,           0,      400,    100,    78,      32,      3,          BLACK,       WHITE,             RED,          GREEN,           10,         7,          60,           20,           WHITE,       GRAY_16,             SYS_FT_16,  _X_CENTER_Y_CENTER,  LBL_OK,      SERVICE_FILTER_NORMAL  )\


//                                    Enum ID,                Service,         SubService,  Pos X,  Pos Y,  Gif
#define GIF_DEF         X_GIF(        ICON_GIF,               SERV_ID_RFSH,    0,           30,     2,      INVALID_IMAGE )\

//                                    Enum ID,                Service,         SubService,  Pos X,  Pos Y,  Gif
#define GRAPH_DEF       X_GGRAPH(     GRAPH_TEST,             SERV_ID_RFSH,    0,           30,     2,      INVALID_IMAGE )\

//                                    Enum ID,                Service,         SubService,  Pos X,  Pos Y,  Size X,  Size Y,  Minimum image,    Maximum image     Cursor Image,   Cursor radius,  Start Angle,  End Angle,  Range,  Option,                                       Text PosX,  Text PosY,  Text Size X,  Text Size Y,  Text Color,  Text Alternate Color,  Font ID,             Font Alternate ID,   Text Option,              Text Label,
#define METER_DEF       X_METER(      METER_TEST,             SERV_ID_NONE,    2,           322,    76,     115,     115,     INVALID_IMAGE,    INVALID_IMAGE,    INVALID_IMAGE,  57,             0,            360,        100,    OPTION_CURSOR_PRIORITY,                       15,         44,         75,           28,           0x00FFFFFF,  0x0000FF00,            SYS_FT_16,  SYS_FT_16,  _X_CENTER_Y_CENTER,  INVALID_LABEL        )\

//                                    Enum ID,                Service,         SubService,  Pos X,  Pos Y,  Size X,  Size Y,  Released panel,  Pressed panel   Text PosX,  Text PosY,  Text SizeX,  Text Size Y,  Text Color,  Text Color Pressed,  Font ID,             Font Alternate ID,    Text Option,             Text Label,    Filter
#define PANEL_DEF       X_PANEL(      PANEL_TEST,             SERV_ID_NONE,    0,           38,     41,     125,     193,     INVALID_IMAGE,   INVALID_IMAGE,  10,         5,          105,         20,           0x00FFFFFF,  0x000000FF,          SYS_FT_16,      SYS_FT_16,       _X_CENTER_Y_CENTER, INVALID_LABEL,  SERVICE_FILTER_NORMAL    )\

//                                    Enum ID,                Service,         SubService,  Size X, Size Y, Image 1
#define SPRITE_DEF      X_SPRITE(     SPRITE_TEST,            SERV_ID_RFSH,    0,           32,     32,     INVALID_IMAGE  )\


//                                    Enum ID,                Service,         SubService     Pos X,  Pos Y,  Size X,  Size Y
#define VIRTUAL_WINDOW_DEF X_WINDOW(  VIRTUAL_WINDOW_1,       SERV_ID_NONE,    0,             10,     56,     36,      160   )\

//        Enum ID,               Service,      Sub, Timing, Px,   Py,   Sx,   Sy,   ResX  ResY  Bar Spacing   Bar Image      Option
#define SPECTRUM_DEF \
 X_SPECTRUM( SPECTRUM,           SERV_ID_SPEK, 0,   0,      12,   10,   168,  72,   16,   12,   3,            SPECTRUM_BAR,  GRAFX_OPTION_NONE)\


#endif
//-------------------------------------------------------------------------------------------------










