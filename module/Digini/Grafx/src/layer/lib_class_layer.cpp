//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_layer.cpp
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
#include "lib_grafx.h"

//-------------------------------------------------------------------------------------------------
//
//   global Clayer array
//
//-------------------------------------------------------------------------------------------------

#ifdef LAYER_DEF
CLayer LayerTable[LAYER_COUNT] =
{
  #define X_LAYER(ENUM_ID, WORK_LAYER, PIXEL_FORMAT, SIZE_X, SIZE_Y) CLayer(ENUM_ID, 0, WORK_LAYER, SIZE_X, SIZE_Y, PIXEL_FORMAT),
    LAYER_DEF
  #undef X_LAYER
};
#endif

Layer_e      CLayer::m_ActiveDrawingLayer;
      //static CLayer*      m_pActiveDrawingLayer;

CLayer*      CLayer::m_pActiveBG_Layer;
CLayer*      CLayer::m_pActiveFG_Layer;
CLayer*      CLayer::m_pConstructBG_Layer;
CLayer*      CLayer::m_pConstructFG_Layer;
uint8_t      CLayer::m_LayerStackCounter;
Layer_e      CLayer::m_LayerStack[CLAYER_STACK_LEVEL];

// This table must match the enum PixelFormat_e (lib_grafx_enum.h)
const uint8_t CLayer::m_LayerPixelSize[PIXEL_FORMAT_COUNT] =
{
  #ifdef GRAFX_COLOR_ARGB8888
    4,
  #endif
  #ifdef GRAFX_COLOR_RGB888
    4,
  #endif
  #ifdef GRAFX_COLOR_RGB565
    2,
  #endif
  #ifdef GRAFX_COLOR_ARGB1555
    2,
  #endif
  #ifdef GRAFX_COLOR_ARGB4444
    4,
  #endif
  #ifdef GRAFX_COLOR_L8
    1,
  #endif
  #ifdef GRAFX_COLOR_AL44
    1,
  #endif
  #ifdef GRAFX_COLOR_AL88
    2,
  #endif
  #ifdef GRAFX_COLOR_L4
    1,
  #endif
  #ifdef GRAFX_COLOR_A8
    1,
  #endif
  #ifdef GRAFX_COLOR_A4
    1,
  #endif
  #ifdef GRAFX_COLOR_RGB332
    1,
  #endif
  #ifdef GRAFX_COLOR_RGB444
    2,
  #endif
    0,    // Dummy
};

//-------------------------------------------------------------------------------------------------
//
//   class: CLayer
//
//
//   Description:   Class to handle layer properties and function
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CLayer
//
//   Parameter(s):  Layer_e      VirtualLayer
//                  uint32_t     LayerAddress
//                  LayerType_e  ActiveOnLayer
//                  uint16_t     SizeX
//                  uint16_t     SizeY
//                  PixelFormat_e PixelFormat
//   Return Value:  none
//
//-------------------------------------------------------------------------------------------------
CLayer::CLayer(Layer_e          VirtualLayer,
               uint32_t         LayerAddress,
               LayerType_e      ActiveOnLayer,
               uint16_t         SizeX,
               uint16_t         SizeY,
               PixelFormat_e    PixelFormat)
{
    m_VirtualLayer   = VirtualLayer;
    m_LayerAddress   = LayerAddress;
    m_ActiveOnLayer  = ActiveOnLayer;
    m_Size.X         = SizeX;
    m_Size.Y         = SizeY;
    m_PixelSize      = CLayer::GetPixelSize(PixelFormat);
    m_TotalSize      = (uint32_t)SizeX * (uint32_t)SizeY * (uint32_t)m_PixelSize;
    m_PixelFormat    = PixelFormat;
    m_Alpha          = 255;
    m_Color          = GetFormatColor(PixelFormat, BLACK);
    m_TextColor      = GetFormatColor(PixelFormat, WHITE);

    CLayer::m_LayerStackCounter = CLAYER_STACK_LEVEL;
}

//-------------------------------------------------------------------------------------------------
//
//   Destructor
//
//   Parameter(s):  none
//   Return Value:  none
//
//-------------------------------------------------------------------------------------------------
CLayer::~CLayer()
{
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: Clear
//
//   Parameter(s):  none
//   Return Value:  none
//
//   Description:   Clear the layer
//
//-------------------------------------------------------------------------------------------------
void CLayer::Clear()
{
    uint32_t DrawingColor = m_Color;
    CLayer::PushDrawing();
    SetDrawing(m_VirtualLayer);
    m_Color = 0;
    DrawRectangle(0, 0, m_Size.X, m_Size.Y);
    m_Color = DrawingColor;
    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetAddress
//
//   Parameter(s):  uint32_t           Address
//   Return Value:  none
//
//   Description:   Set layer base address
//
//-------------------------------------------------------------------------------------------------
void CLayer::SetAddress(uint32_t Address)
{
    m_LayerAddress = Address;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetAddress
//
//   Parameter(s):  none
//   Return Value:  uint32_t           Address
//
//   Description:   Get layer base address
//
//-------------------------------------------------------------------------------------------------
uint32_t CLayer::GetAddress()
{
    return m_LayerAddress;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetTotalSize
//
//   Parameter(s):  none
//   Return Value:  uint32_t           Address
//
//   Description:   Get the total size of the layer
//
//-------------------------------------------------------------------------------------------------
uint32_t CLayer::GetTotalSize()
{
    return m_TotalSize;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetSize
//
//   Parameter(s):  none
//   Return Value:  Cartesian_t  Size
//
//   Description:   Get the size in X and Y of the layer
//
//-------------------------------------------------------------------------------------------------
Cartesian_t CLayer::GetSize()
{
    return m_Size;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetPixelFormat
//
//   Parameter(s):  none
//   Return Value:  PixelFormat_e    PixelFormat
//
//   Description:   Return the pixel format value
//
//-------------------------------------------------------------------------------------------------
PixelFormat_e CLayer::GetPixelFormat()
{
    return m_PixelFormat;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetPixelSize
//
//   Parameter(s):  none
//   Return Value:  uint8_t    PixelSize
//
//   Description:   Return the pixel size
//
//-------------------------------------------------------------------------------------------------
uint8_t CLayer::GetPixelSize()
{
    return m_PixelSize;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetPixelSize
//
//   Parameter(s):  PixelFormat_e Pixel format to get the pixel size
//   Return Value:  uint8_t        PixelSize
//
//   Description:   Return the pixel size
//
//-------------------------------------------------------------------------------------------------
uint8_t CLayer::GetPixelSize(PixelFormat_e PixelFormat)
{
    return m_LayerPixelSize[PixelFormat];
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetActive
//
//   Parameter(s):  Layer_e          ActiveLayer
//   Return Value:  none
//
//   Description:   Set the the virtual layer to be use as active layer
//
//-------------------------------------------------------------------------------------------------
void CLayer::SetActive(LayerType_e ActiveLayer)
{
    m_ActiveOnLayer = ActiveLayer;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetActive
//
//   Parameter(s):  none
//   Return Value:  LayerType_e      ActiveLayer
//
//   Description:   Get setting for active layer
//
//-------------------------------------------------------------------------------------------------
LayerType_e CLayer::GetActive()
{
    return m_ActiveOnLayer;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetVitual
//
//   Parameter(s):  none
//   Return Value:  Layer_e          ActiveLayer
//
//   Description:   Get setting for virtual layer
//
//-------------------------------------------------------------------------------------------------
Layer_e CLayer::GetVirtual()
{
    return m_VirtualLayer;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetAlpha
//
//   Parameter(s):  uint8_t       Alpha
//   Return Value:  none
//
//   Description:   Set the Alpha constant for the layer.
//
//   note(s):       Update the physical layer if active
//
//-------------------------------------------------------------------------------------------------
void CLayer::SetAlpha(uint8_t Alpha)
{
    m_Alpha = Alpha;

    if(m_ActiveOnLayer != LAYER_VIRTUAL)
    {
        // Force new value on global alpha blending
        SetActiveLayer(m_ActiveOnLayer, m_VirtualLayer);
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetAlpha
//
//   Parameter(s):  none
//   Return Value:  uint8_t       Alpha
//
//   Description:   Get the Alpha constant for the layer
//
//-------------------------------------------------------------------------------------------------
uint8_t CLayer::GetAlpha()
{
    return m_Alpha;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetColor
//
//   2 Overloaded functions
//
//   Parameter(s):  ColorTable_e    Index
//   Return Value:  none
//
//   Parameter(s):  uint32_t        Color
//   Return Value:  none
//
//   Description:   Set the drawing color
//
//-------------------------------------------------------------------------------------------------
void CLayer::SetColor(ColorTable_e Index)
{
     SetColor(GetFormatColor(LayerTable[CLayer::m_ActiveDrawingLayer].m_PixelFormat, Index));
}

void CLayer::SetColor(uint32_t Color)
{
     LayerTable[CLayer::m_ActiveDrawingLayer].m_Color = Color;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetTextColor
//
//   2 Overloaded functions
//
//   Parameter(s):  ColorTable_e    Index
//   Return Value:  none
//
//   Parameter(s):  uint32_t        Color
//   Return Value:  none
//
//   Description:   Set the text drawing color
//
//-------------------------------------------------------------------------------------------------
void CLayer::SetTextColor(ColorTable_e Index)
{
     SetTextColor(GetFormatColor(LayerTable[CLayer::m_ActiveDrawingLayer].m_PixelFormat, Index));
}

void CLayer::SetTextColor(uint32_t Color)
{
     LayerTable[CLayer::m_ActiveDrawingLayer].m_TextColor = Color;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetColor
//
//   Parameter(s):  none
//   Return Value:  uint32_t   Value
//
//   Description:   Return the color value according to pixel format
//
//-------------------------------------------------------------------------------------------------
uint32_t CLayer::GetColor()
{
    return LayerTable[CLayer::m_ActiveDrawingLayer].m_Color;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetTextColor
//
//   Parameter(s):  none
//   Return Value:  uint32_t   Value
//
//   Description:   Return the text color value according to pixel format
//
//-------------------------------------------------------------------------------------------------
uint32_t CLayer::GetTextColor()
{
    return LayerTable[CLayer::m_ActiveDrawingLayer].m_TextColor;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetActiveLayer
//
//   Parameter(s):  LayerType_e LayerType               Physical layer affected
//
//   Return Value:  Layer_e     Layer                   Virtual layer used
//

//   Description:   Get the active layer according to Parameter
//
//-------------------------------------------------------------------------------------------------

Layer_e CLayer::GetActiveLayer(LayerType_e LayerType)
{
    return GetActiveLayer(LayerType);
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetActiveLayer
//
//   Parameter(s):  LayerType_e LayerType               Physical layer affected
//                  CLayer*     pLayer                  Virtual layer to used
//   Return Value:  none
//
//   Description:   Set the active layer according to Parameter
//
//   Note(s)        For practical purpose, the previous layer will be assign to construct duty
//
//-------------------------------------------------------------------------------------------------
void CLayer::SetActiveLayer(LayerType_e LayerType, CLayer* pLayer)
{
    switch(LayerType)
    {
        case LAYER_BACKGROUND:
        {
            // Deactivate physical access from previous owner of the active layer
            if(CLayer::m_pActiveBG_Layer != nullptr)
            {
                CLayer::m_pActiveBG_Layer->SetActive(LAYER_VIRTUAL);                    // Set the old layer to virtual status
                CLayer::m_pConstructBG_Layer = CLayer::m_pActiveBG_Layer;               // And also assign this layer to construct duty
            }

            // Activate new owner of the active layer
            CLayer::m_pActiveBG_Layer = pLayer;
            CLayer::m_pActiveBG_Layer->SetActive(LayerType);
            myGrafx->LayerConfig(CLayer::m_pActiveBG_Layer);
            break;
        }

        case LAYER_FOREGROUND:
        {
            // Deactivate physical access from previous owner of the active layer
            if(CLayer::m_pActiveFG_Layer != nullptr)
            {
                CLayer::m_pActiveFG_Layer->SetActive(LAYER_VIRTUAL);                    // Set the old layer to virtual status
                CLayer::m_pConstructFG_Layer = CLayer::m_pActiveBG_Layer;               // And also assign this layer to construct duty
            }

            // Activate new owner of the active layer
            CLayer::m_pActiveFG_Layer = pLayer;
            CLayer::m_pActiveFG_Layer->SetActive(LayerType);
            myGrafx->LayerConfig(CLayer::m_pActiveFG_Layer);
            break;
        }

        case LAYER_VIRTUAL:
        {
            // TODO (Alain#1#): Handle the virtual layer if any

            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetActiveLayer
//
//   Parameter(s):  LayerType_e LayerType               Physical layer affected
//                  Layer_e     Layer                   Virtual layer to used
//   Return Value:  none
//
//   Description:   Set the active layer according to Parameter
//
//   Note(s)        For practical purpose, the previous layer will be assign to construct duty
//
//-------------------------------------------------------------------------------------------------

void CLayer::SetActiveLayer(LayerType_e LayerType, Layer_e Layer)
{
    SetActiveLayer(LayerType, &LayerTable[Layer]);
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetDrawingLayer
//
//   Parameter(s):  none
//   Return Value:  Layer_e     Layer
//
//   Description:   set
//
//-------------------------------------------------------------------------------------------------

Layer_e CLayer::GetDrawing()
{
     return CLayer::m_ActiveDrawingLayer;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetDrawingLayer
//
//   Parameter(s):  Layer_e     Layer
//   Return Value:  none
//
//   Description:   Set the default layer for drawing (it can be virtual or physical)
//
//-------------------------------------------------------------------------------------------------
void CLayer::SetDrawing(Layer_e Layer)
{
    CLayer::m_ActiveDrawingLayer = Layer;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: PushDrawingLayer
//
//   Parameter(s):  none
//   Return Value:  none
//
//   Description:   Push the actual drawing layer on a stack
//
//-------------------------------------------------------------------------------------------------
void CLayer::PushDrawing()
{
    if(m_LayerStackCounter != 0)
    {
        m_LayerStackCounter--;
        m_LayerStack[m_LayerStackCounter] = m_ActiveDrawingLayer;
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: PopDrawingLayer
//
//   Parameter(s):  none
//   Return Value:  none
//
//   Description:   Get a drawing layer from stack
//
//-------------------------------------------------------------------------------------------------
void CLayer::PopDrawing()
{
    if(CLayer::m_LayerStackCounter < CLAYER_STACK_LEVEL)
    {
        CLayer::m_ActiveDrawingLayer = CLayer::m_LayerStack[m_LayerStackCounter];
        CLayer::m_LayerStackCounter++;
    }
}

//-------------------------------------------------------------------------------------------------
// Private variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Function Name: ClearLayer
//
//   Parameter(s):  none
//   Return Value:  none
//
//   Description:   Clear the layer
//
//-------------------------------------------------------------------------------------------------
//void ClearLayer()
//{
//    ActiveDrawingLayer->Clear();
//}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: ToggleLayer
//
//   Parameter(s):  none
//   Return Value:  none
//
//   Description:   Toggle between active and construct layer
//
//-------------------------------------------------------------------------------------------------
//void ToggleLayer()
//{
//    SetActiveLayer(LAYER_BACKGROUND, pConstructBG_Layer);
//    SetActiveLayer(LAYER_FOREGROUND, pConstructFG_Layer);
//}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetConstructLayer
//
//   Parameter(s):  LayerType_e  LayerType              Physical layer affected
//                  Layer_e     Layer                   Virtual layer to used
//   Return Value:  none
//
//   Description:   Set the construct layer
//
//-------------------------------------------------------------------------------------------------
/*
void SetConstructLayer(LayerType_e LayerType, Layer_e Layer)
{
    switch(LayerType)
    {
        case LAYER_BACKGROUND:
        {
            pConstructBG_Layer = &LayerTable[Layer];            // And also assign this layer to construct duty
            break;
        }

        case LAYER_FOREGROUND:
        {
            pConstructFG_Layer = &LayerTable[Layer];            // And also assign this layer to construct duty
            break;
        }
    }
    pConstructFG_Layer->SetActiveLayer(LAYER_VIRTUAL);              // Make sure it is to virtual status
}
*/

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetDrawingOnConstructLayer
//
//   Parameter(s):  LayerType_e  LayerType
//   Return Value:  none
//
//   Description:   Set drawing layer on one of hte construct layer
//
//-------------------------------------------------------------------------------------------------
/*
void SetDrawingOnConstructLayer(LayerType_e LayerType)
{
    if(LayerType == LAYER_BACKGROUND) pActiveDrawingLayer = pConstructBG_Layer;
    else                              pActiveDrawingLayer = pConstructFG_Layer;
}
*/

//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetAlphaLayer
//
//   Parameter(s):  uint8_t    Alpha
//   Return Value:  none
//
//   Description:   Set the alpha value globally for the drawing layer
//
//-------------------------------------------------------------------------------------------------
/*
void SetAlphaLayer(uint8_t Alpha)
{
    pActiveDrawingLayer->SetAlpha(Alpha);
}
*/
//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_GRAFX




