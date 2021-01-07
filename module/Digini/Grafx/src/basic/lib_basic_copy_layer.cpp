#if 0
//-------------------------------------------------------------------------------------------------
//
//  File : lib_basic_copy_layer.cpp
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
#if defined(LAYER_DEF) && (LAYER_COUNT > 1)
#include "lib_grafx.h"

//-------------------------------------------------------------------------------------------------
// External low level function from driver
//-------------------------------------------------------------------------------------------------

//extern void DRV_Copy(void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);

//-------------------------------------------------------------------------------------------------
//
//   Function Name: CopyLayerToLayer
//
//   Parameter(s):  Layer_e              SrcLayer
//                  Layer_e              DstLayer
//                  Box_t*               pBox
//   Return Value:  none
//
//   Description:   Copy a rectangle region from a layer to another layer
//
//-------------------------------------------------------------------------------------------------
void CopyLayerToLayer(Layer_e SrcLayer, Layer_e DstLayer, Box_t* pBox)
{
    CLayer* pLayer;

    CLayer::PushDrawing();
    CLayer::SetDrawing(DstLayer);
    pLayer = &LayerTable[SrcLayer];
    DRV_Copy((void*)pLayer->GetAddress(), pBox, &pBox->Pos, pLayer->GetPixelFormat(), CLEAR_BLEND);
    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: CopyLayerToLayer
//
//   Parameter(s):  Layer_e             SrcLayer
//                  Layer_e             DstLayer
//                  uint16_t            X
//                  uint16_t            Y
//                  uint16_t            Width
//                  uint16_t            Height
//   Return Value:  none
//
//   Description:   Copy a rectangle region from a layer to another layer
//
//-------------------------------------------------------------------------------------------------
void CopyLayerToLayer(Layer_e SrcLayer, Layer_e DstLayer, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height)
{
    Box_t        Box;

    Box.Pos.X       = X;
    Box.Size.Width  = Width;
    Box.Pos.Y       = Y;
    Box.Size.Height = Height;

    CopyLayerToLayer(SrcLayer, DstLayer, &Box);
}

//-------------------------------------------------------------------------------------------------
#endif // LAYER_DEF
#endif // DIGINI_USE_GRAFX

#endif
