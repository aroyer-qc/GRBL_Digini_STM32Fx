//-------------------------------------------------------------------------------------------------
//
//  File :  lib_grafx_define.h
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
// Define(s)
//-------------------------------------------------------------------------------------------------


#define SUB_SERVICE_CFG                             (uint16_t)0xFFFF

// Justification of the GRAFX_Printf
#define _X_CENTER_Y_CENTER                          (uint8_t)0x11
#define _X_CENTER_Y_TOP                             (uint8_t)0x12
#define _X_CENTER_Y_BOTTOM                          (uint8_t)0x13
#define _X_LEFT_Y_CENTER                            (uint8_t)0x21
#define _X_LEFT_Y_TOP                               (uint8_t)0x22
#define _X_LEFT_Y_BOTTOM                            (uint8_t)0x23
#define _X_RIGHT_Y_CENTER                           (uint8_t)0x31
#define _X_RIGHT_Y_TOP                              (uint8_t)0x32
#define _X_RIGHT_Y_BOTTOM                           (uint8_t)0x33

//  Mask for justification of GRAFX_Printf
#define _X_JUSTIFICATION                            (uint8_t)0x30
#define _X_CENTER                                   (uint8_t)0x10
#define _X_LEFT                                     (uint8_t)0x20
#define _X_RIGHT                                    (uint8_t)0x30
#define _X_LINE_JUSTIFICATION                       (uint8_t)0xC0
#define _X_LINE_LEFT                                (uint8_t)0x40
#define _X_LINE_CENTER                              (uint8_t)0x80
#define _X_LINE_RIGHT                               (uint8_t)0xC0
#define _Y_JUSTIFICATION                            (uint8_t)0x03
#define _Y_CENTER                                   (uint8_t)0x01
#define _Y_TOP                                      (uint8_t)0x02
#define _Y_BOTTOM                                   (uint8_t)0x03

#define SERVICE_TYPE2_SIZE_LIST                     16
#define SERVICE_TYPE4_SIZE_LIST                     16

#define SERVICE_ID_STATE_SIZE                       16
#define LABEL_LIST_SIZE                             4
#define LINK_LIST_ID_SIZE                           8


#define COLOR_DEF      X_COLOR( BLACK,            0x00000000 )\
                       X_COLOR( BLUE,             0x000000FF )\
                       X_COLOR( GREEN,            0x0000FF00 )\
                       X_COLOR( CYAN,             0x0000FFFF )\
                       X_COLOR( RED,              0x00FF0000 )\
                       X_COLOR( MAGENTA,          0x00FF00FF )\
                       X_COLOR( BROWN,            0x004F2700 )\
                       X_COLOR( LIGHT_GRAY,       0x00A7A7A7 )\
                       X_COLOR( GRAY,             0x007F7F7F )\
                       X_COLOR( LIGHT_BLUE,       0x003F7FFF )\
                       X_COLOR( LIGHT_GREEN,      0x007FFF7F )\
                       X_COLOR( LIGHT_CYAN,       0x007FFFFF )\
                       X_COLOR( LIGHT_RED,        0x00FF7F7F )\
                       X_COLOR( LIGHT_MAGENTA,    0x00FF7FFF )\
                       X_COLOR( YELLOW,           0x00FFF700 )\
                       X_COLOR( WHITE,            0x00FFFFFF )\
                       X_COLOR( ORANGE,           0x00FF4F2F )\
                       X_COLOR( LIGHT_YELLOW,     0x00FFFF21 )\
                       X_COLOR( TRANSPARENT,      0x00000000 )\

#define GRAY_DEF       X_GRAY( GRAY_1,            0x00070707 )\
                       X_GRAY( GRAY_2,            0x000F0F0F )\
                       X_GRAY( GRAY_3,            0x00171717 )\
                       X_GRAY( GRAY_4,            0x001F1F1F )\
                       X_GRAY( GRAY_5,            0x00272727 )\
                       X_GRAY( GRAY_6,            0x002F2F2F )\
                       X_GRAY( GRAY_7,            0x00373737 )\
                       X_GRAY( GRAY_8,            0x003F3F3F )\
                       X_GRAY( GRAY_9,            0x00474747 )\
                       X_GRAY( GRAY_10,           0x004F4F4F )\
                       X_GRAY( GRAY_11,           0x00575757 )\
                       X_GRAY( GRAY_12,           0x005F5F5F )\
                       X_GRAY( GRAY_13,           0x00676767 )\
                       X_GRAY( GRAY_14,           0x006F6F6F )\
                       X_GRAY( GRAY_15,           0x00777777 )\
                       X_GRAY( GRAY_16,           0x007F7F7F )\
                       X_GRAY( GRAY_17,           0x00878787 )\
                       X_GRAY( GRAY_18,           0x008F8F8F )\
                       X_GRAY( GRAY_19,           0x00979797 )\
                       X_GRAY( GRAY_20,           0x009F9F9F )\
                       X_GRAY( GRAY_21,           0x00A7A7A7 )\
                       X_GRAY( GRAY_22,           0x00AFAFAF )\
                       X_GRAY( GRAY_23,           0x00B7B7B7 )\
                       X_GRAY( GRAY_24,           0x00BFBFBF )\
                       X_GRAY( GRAY_25,           0x00C7C7C7 )\
                       X_GRAY( GRAY_26,           0x00CFCFCF )\
                       X_GRAY( GRAY_27,           0x00D7D7D7 )\
                       X_GRAY( GRAY_28,           0x00DFDFDF )\
                       X_GRAY( GRAY_29,           0x00E7E7E7 )\
                       X_GRAY( GRAY_30,           0x00EFEFEF )\
                       X_GRAY( GRAY_31,           0x00F7F7F7 )

// ---------- Service definition ----------

// TO DO move this in config ... so it is user that decide

#define SERVICE_LINK_STATE_SIZE                     16
#define SERVICE_INDEX_STATE_NO_DRAW                 SERVICE_ID_STATE_SIZE
#define SERVICE_INVALID_STATE                       0xFF

// Filter for 'GUI_FilterServiceStateFilterServiceState' in 'lib_gui_task.cpp'
#define SERVICE_FILTER_RELEASED                     0x0001
#define SERVICE_FILTER_IDLE                         0x0002
#define SERVICE_FILTER_PRESSED                      0x0004
#define SERVICE_FILTER_TRANSIT                      0x0008
#define SERVICE_FILTER_TYPEMATIC                    0x0010
#define SERVICE_FILTER_SUPERKEY                     0x0020
#define SERVICE_FILTER_CANCEL                       0x0040
#define SERVICE_FILTER_START                        0x0080
#define SERVICE_FILTER_REFRESH                      0x0100
#define SERVICE_FILTER_FORCED                       0x0200
#define SERVICE_FILTER_FINALIZE                     0x0400

#define SERVICE_FILTER_NORMAL                       0x0045              // if service need the PRESSED and RELEASED only State
#define SERVICE_FILTER_NORMAL_START                 0x00C5              // if service need the START, PRESSED and RELEASED only State
#define SERVICE_FILTER_NORMAL_AND_TYPEMATIC         0x0055              // if service need the PRESSED and RELEASED and TYPEMATIC State
#define SERVICE_FILTER_NORMAL_AND_SUPERKEY          0x0065              // if service need the PRESSED and RELEASED and SUPERKEY State
#define SERVICE_FILTER_NORMAL_EXTENDED              0x0075              // if service need the PRESSED and RELEASED and TYPEMATIC and SUPERKEY State

// ---------- Widget option definition ----------

#define GRAFX_OPTION_NONE                           (uint16_t)0x0000

// Widget Option flag
#define GRAFX_OPTION_DRAW_AT_REFRESH                (uint16_t)0x0001    // Use this option to draw only at refresh
#define GRAFX_OPTION_DRAW_ONCE                      (uint16_t)0x0002    // Use this option to draw only once at create. but can be use as button zone with link
#define GRAFX_OPTION_DRAW_ONCE_AND_REMOVE           (uint16_t)0x0004    // Use this option to draw only once at create. it is remove after
#define GRAFX_OPTION_DRAW_ON_BACK                   (uint16_t)0x0008
#define GRAFX_OPTION_MOVABLE_OBJECT                 (uint16_t)0x0010    // TODO widget can be drag on another location

// Widget Touch option
#define GRAFX_OPTION_TOUCH_RECTANGLE                (uint16_t)0x0020
#define GRAFX_OPTION_TOUCH_POLYGON                  (uint16_t)0x0040
#define GRAFX_OPTION_TOUCH_CIRCLE                   (uint16_t)0x0060
#define GRAFX_OPTION_TOUCH_MASK                     (uint16_t)0x0060

// Widget Orientation
#define GRAFX_OPTION_ORIENTATION_VERTICAL           (uint16_t)0x0080
#define GRAFX_OPTION_ORIENTATION_HORIZONTAL         (uint16_t)0x0100
#define GRAFX_OPTION_ORIENTATION_MASK               (uint16_t)0x0180

// Cursor Priority (Meter)
#define GRAFX_OPTION_CURSOR_PRIORITY                (uint16_t)0x0200

// Blending mode
#define GRAFX_OPTION_BLEND_CLEAR                    (uint16_t)0x0400
#define GRAFX_OPTION_BLEND_ALPHA                    (uint16_t)0x0800

#define GRAFX_OPTION_TEXT                           (uint16_t)0x1000

#define GRAFX_OPTION_CLEAR                          (uint16_t)0x2000

#define GRAFX_INIT_DONE                             (uint16_t)0x8000

//-------------------------------------------------------------------------------------------------



