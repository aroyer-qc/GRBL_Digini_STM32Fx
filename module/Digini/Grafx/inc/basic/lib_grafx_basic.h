//-------------------------------------------------------------------------------------------------
//
//  File : lib_grafx_basic.h
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
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

void                BlockCopy                   (void* pSrc, Box_t* pBox, Cartesian_t DstPos, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);
void                BlockCopy                   (void* pSrc, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t DstX, uint16_t DstY, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);
void                CopyLinear                  (void* pSrc, Box_t* pBox, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);
void                CopyLinear                  (void* pSrc, uint16_t PosX, uint16_t PosY, uint16_t Width, uint16_t Height, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);
void                CopyLinear                  (Skin_e Image, Cartesian_t Position, BlendMode_e BlendMode);
void                CopyLayerToLayer            (Layer_e SrcLayer, Layer_e DstLayer, Box_t* pBox);
void                CopyLayerToLayer            (Layer_e SrcLayer, Layer_e DstLayer, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height);
void                DrawHLine                   (uint16_t PosY, uint16_t PosX1, uint16_t PosX2, uint16_t Thickness = 1);
void                DrawVLine                   (uint16_t PosX, uint16_t PosY1, uint16_t PosY2, uint16_t Thickness = 1);
void                DrawDLine                   (float X1, float Y1, float X2, float Y2);
void                DrawBox                     (Box_t* pBox, uint16_t Thickness = 1);
void                DrawBox                     (uint16_t PosX, uint16_t PosY, uint16_t Width, uint16_t Height, uint16_t Thickness = 1);
void                DrawCircle                  (Circle_t* pCircle, PolygonMode_e PolygonMode);
void                DrawCircle                  (uint16_t PosX, uint16_t PosY, uint16_t Radius, PolygonMode_e PolygonMode);
void                DrawPie                     (Skin_e Image, Pie_t* pPie, BlendMode_e BlendMode);
void                DrawPie                     (Skin_e Image, Cartesian_t Position, uint16_t Radius, uint16_t StartAngle, uint16_t EndAngle, BlendMode_e BlendMode);
void                DrawPixel                   (uint16_t X, uint16_t Y);
void                DrawPatternPolygon          (void* pSrc, uint16_t SrcLineWidth, Cartesian_t SrcImageOffset, Cartesian_t Pos1, Cartesian_t Pos2, Cartesian_t Pos3, Cartesian_t Pos4, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);
void                DrawPolygon                 (Cartesian_t Pos1, Cartesian_t Pos2, Cartesian_t Pos3, Cartesian_t Pos4, PolygonMode_e PolygonMode);
void                DrawPolygon                 (Polygon_t* pPolygon, PolygonMode_e PolygonMode);
void                DrawRectangle               (Box_t* pBox);
void                DrawRectangle               (uint16_t PosX, uint16_t PosY, uint16_t Width, uint16_t Height);
void                DrawDebugBox                (Box_t* pBox);

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
