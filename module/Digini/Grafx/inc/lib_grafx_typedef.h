//-------------------------------------------------------------------------------------------------
//
//  File : lib_grafx_typedef.h
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

#include "lib_grafx.h"

//-------------------------------------------------------------------------------------------------
// Typedef(s)
//-------------------------------------------------------------------------------------------------

struct StaticImageInfo_t
{
   uint16_t        SizeX;
   uint16_t        SizeY;
   uint16_t        BytesPerLine;
   uint8_t         BitsPerPixel;
   uint8_t*        pData;
   uint32_t        RawSize;
   PixelFormat_e   PixelFormat;
   Compression_e   Compression;
};

struct Cartesian_t
{
    int16_t        X;
    int16_t        Y;
};

struct BoxSize_t
{
    uint16_t       Width;
    uint16_t       Height;
};

struct Box_t
{
    Cartesian_t    Pos;
    BoxSize_t      Size;
};

struct Circle_t
{
   Cartesian_t     Pos;
   uint16_t        R;
};

struct Pie_t
{
   Circle_t           Circle;
   uint16_t           StartAngle;
   uint16_t           EndAngle;
};

struct Polygon_t
{
    Cartesian_t    Pos1;
    Cartesian_t    Pos2;
    Cartesian_t    Pos3;
    Cartesian_t    Pos4;
};

struct ImageInfo_t
{
    void*          pPointer;
    BoxSize_t      Size;
    PixelFormat_e  PixelFormat;
};

union EventArea_t
{
    Polygon_t      Polygon;

    struct
    {
        Circle_t   C;
        uint16_t   wNU[5];
    } Circle;

    struct
    {
        Box_t      Box;
        uint16_t   NU[4];
    } Rectangle;
};

// Font

struct FontSize_t
{
    uint8_t        Width;
    uint8_t        Height;
};

struct FontInfo_t
{
    uint8_t        Height;
    uint8_t        Interline;
    uint8_t        Width;
};

struct FontDescriptor_t
{
    int8_t         LeftBearing;
    int8_t         RightBearing;
    FontSize_t     Size;                                    // Width and Height pixel zone
    uint8_t        Width;                                   // Width increment
    int8_t         OffsetY;                                 // Offset in Y for this character
    uint16_t       TotalSize;
    uint8_t*       pAddress;
};

struct PageSlideRange_t
{
    int16_t        StartPos;
    int16_t        EndPos;
};

// --------- Widget element ---------

struct Image_t
{
    Skin_e         ID_List[SERVICE_ID_STATE_SIZE];         // IMAGE_INVALID = No icon or glyph present
};

struct Glyph_t
{
    Cartesian_t    Pos;
    Skin_e         ID_List[SERVICE_ID_STATE_SIZE];         // IMAGE_INVALID = No icon or glyph present
};

struct Text_t
{
    Box_t           Box;
    uint32_t       Color[3];
    Font_e         Font;
    BlendMode_e    Blend;
    Label_e         Label;                                   // INVALID_LABEL //0xFF = No label present
    uint8_t           Options;
};

// --------- Exchange Widget element ---------

struct ExchangeCommon_t
{
    ExchangeType_e      ExType;
    Label_e             Label;
    void*               pValue;
};

struct InputDecimal_t
{
    ExchangeCommon_t    Common;
    Label_e             FormatLabel;
    int32_t             Min;
    int32_t             Max;
};

struct InputFloat_t
{
    ExchangeCommon_t   Common;
    Label_e             FormatLabel;
    float               Min;
    float               Max;
};

struct InputString_t
{
    ExchangeCommon_t   Common;
};

// --------- Service ---------

struct Service_t
{
    uint32_t       ID;
    uint16_t       SubID;
    uint16_t       Timing;
};

struct ServiceReturn_t                                      // Basic service return
{
    ServiceType_e  ServiceType;
    uint16_t       IndexState;
};

struct ServiceInvalid_t
{
    ServiceType_e  ServiceType;
    uint16_t       IndexState;
};

struct ServiceType1_t
{
    ServiceType_e  ServiceType;
    uint16_t       IndexState;
    uint16_t       Data;
};

struct ServiceType2_t
{
    ServiceType_e  ServiceType;
    uint16_t       IndexState;
    s16_t          Data[SERVICE_TYPE2_SIZE_LIST];
};

struct ServiceType3_t
{
    ServiceType_e  ServiceType;
    uint16_t       IndexState;
    Link_e         Link;
};

struct ServiceType4_t
{
    ServiceType_e  ServiceType;
    uint16_t       IndexState;
    char*          pString[SERVICE_TYPE4_SIZE_LIST];
};

struct ServiceType5_t
{
    ServiceType_e  ServiceType;
    void*          pAddress;
    BoxSize_t      Size;
    PixelFormat_e  PixelFormat;
};

struct ServiceType6_t                   // Service to override position of a widget
{
    ServiceType_e  ServiceType;
    Cartesian_t    Position;
    PixelFormat_e  PixelFormat;
};

struct ServiceType7_t                   // Service to override position of a widget
{
    ServiceType_e  ServiceType;
    Cartesian_t    Position;
    void*          pData;               // Must be a memory block by allocation, it will be freed by widget
    uint16_t       Size;
};

// TODO (Alain#1#) make this work
struct ServiceType8_t                   // Service to override position of a widget
{
    ServiceType_e  ServiceType;
    uint32_t       Data;
};

// --------- Widget ---------

struct Background_t
{
    Service_t      Service;
    Cartesian_t       Pos;
    Image_t        Image;
    uint16_t       Options;
//    uint16_t     RefreshRate;      // background animation ??
};

struct BasicBox_t
{
    Service_t      Service;
    Box_t          Box;                // Calculate position of all image from this also is touch area
    Skin_e         ImageTL;
    Skin_e         ImageTR;
    Skin_e         ImageBL;
    Skin_e         ImageBR;
    Skin_e         ImageH;
    Skin_e         ImageV;
    uint16_t       ServiceFilter;
    uint16_t       Options;
};

struct BasicButton_t
{
    Service_t      Service;
    Box_t          Box;                // Serve also as touch area
    uint32_t       BackColor;
    uint32_t       PressedBackColor;
    uint32_t       BoxColor;
    uint32_t       PressedBoxColor;
    Text_t         Text;
    uint16_t       ServiceFilter;
    uint16_t       Options;
    uint8_t        Thickness;
};

struct BasicRect_t
{
    Service_t      Service;
    Box_t          Box;                // Serve also as touch area
    uint32_t       RectColor;
    uint16_t       ServiceFilter;
    uint16_t       Options;
};

struct Button_t
{
    Service_t      Service;
    Cartesian_t       Pos;
    Image_t        Image;
    Glyph_t        Glyph;
    Text_t         Text;
    EventArea_t    EventArea;
    uint16_t       ServiceFilter;
    //uint16_t     WaveList[SERVICE_FILE_INDEX_SIZE];
    uint16_t       Options;
};

struct Icon_t
{
    Service_t        Service;
    Box_t          Box;
    Image_t        Image;
    uint16_t       Options;
};

struct Gif_t
{
    Service_t        Service;
    Cartesian_t    Pos;
    Image_t        Image;
    uint16_t       Options;
};

struct Label_t
{
    Service_t      Service;
    Text_t         Text;
    uint16_t       Options;
};

struct LabelList_t
{
    Service_t      Service;
    Text_t         Text;
    Label_e         Label[LABEL_LIST_SIZE];
    uint16_t       Options;
};

struct Meter_t
{
    Service_t      Service;
    Box_t          Box;
    uint16_t       Radius;
    uint16_t       StartAngle;
    uint16_t       EndAngle;
    uint16_t       Range;
    Skin_e         Minimum;
    Skin_e         Maximum;
    Skin_e         Cursor;
    Text_t         Text;
    uint16_t       Options;
};

struct PageSlide_t
{
    Service_t      Service;
    uint16_t	   ServiceFilter;
    Box_t		   Box;				// It is also the Touch Area
    SlideDir_e     direction;
    uint16_t       Options;
};

struct Panel_t
{
    Service_t      Service;
    uint16_t       ServiceFilter;
    Box_t           Box;                // It is also the Touch Area
    Image_t           Image;
    Text_t         Text;
    uint16_t       Options;
};

struct Progress_t
{
    Service_t      Service;
    Box_t          Box;
    uint16_t       PixelRange;
    Skin_e         Background;
    Skin_e         Bar;
    Skin_e         Cursor;
    Text_t         Text;
    Cartesian_t    CursorOffset;
    uint16_t       StartValue;
    uint16_t       EndValue;
    uint16_t       Options;
};

struct Spectrum_t
{
    Service_t      Service;
    Box_t          Box;
    BoxSize_t      Resolution;              // Bar * Dot
    uint16_t       Spacing;
    Skin_e         Bar;
    uint16_t       Options;
};

struct Sprite_t
{
    Service_t      Service;
    BoxSize_t      Size;
    Image_t        Image;
    uint16_t       Options;
};

struct Terminal_t
{
    Service_t      Service;
    Box_t          Box;
    uint32_t       TextColor;
    Font_e         Font;
    uint16_t       ServiceFilter;
    uint16_t       Options;
};

struct VirtualHub_t
{
    Service_t      Service;
    Link_e         LinkID[8];
};

struct VirtualWindow_t
{
    Service_t      Service;
    Box_t          Box;
};

// --------- Page ---------

struct MsgRefresh_t
{
    MessageType_e  Type;            // Message type for the receiving task
    ServiceEvent_e Touch;           // State of the touch zone
    Widget_e       WidgetID;        // ID of the zone
    Cartesian_t    EventPosition;   // Position xy of the zone
};

//-------------------------------------------------------------------------------------------------
