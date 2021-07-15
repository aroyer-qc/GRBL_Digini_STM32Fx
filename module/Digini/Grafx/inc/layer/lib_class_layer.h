//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_layer.h
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
// Include(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

#define CLAYER_STACK_LEVEL  8

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class CLayer
{
    public:

        // Constructor
                                CLayer                  (Layer_e          VirtualLayer,
                                                         uint32_t         u32LayerAddress,
                                                         LayerType_e      ActiveOnLayer,
                                                         uint16_t         SizeX,
                                                         uint16_t         SizeY,
                                                         PixelFormat_e    PixelFormat);

        // Destructor
                               ~CLayer                  ();

        // Function
        void                    Clear                   ();
        void                    SetAddress              (uint32_t Address);
        uint32_t                GetAddress              ();



        uint32_t                GetTotalSize            ();
        Cartesian_t             GetSize                 ();
        PixelFormat_e           GetPixelFormat          ();
        uint8_t                 GetPixelSize            ();
        void                    SetActive               (LayerType_e LayerType);
        LayerType_e             GetActive               ();
        Layer_e                 GetVirtual              ();
        void                    SetAlpha                (uint8_t);
        uint8_t                 GetAlpha                ();

        // Static function

        static Layer_e          GetActiveLayer          (LayerType_e LayerType);
        static void             SetActiveLayer          (LayerType_e LayerType, CLayer* pLayer);
        static void             SetActiveLayer          (LayerType_e LayerType, Layer_e Layer);

        static Layer_e          GetDrawing              ();
        static void             SetDrawing              (Layer_e Layer);
        static void             PopDrawing              ();
        static void             PushDrawing             ();

        // Static function they all work on the drawing layer
        static void             SetColor                (ColorTable_e Index);
        static void             SetColor                (uint32_t Color);
        static uint32_t         GetColor                ();
        static void             SetTextColor            (ColorTable_e Index);
        static void             SetTextColor            (uint32_t Color);
        static uint32_t         GetTextColor            ();

        static uint8_t          GetPixelSize            (PixelFormat_e PixelFormat);

    private:

        //  Variables
        Layer_e                 m_VirtualLayer;                  // This is the memory layer
        uint32_t                m_LayerAddress;
        LayerType_e             m_ActiveOnLayer;                 // This is the physical if any ( to enable the physical part )

        PixelFormat_e           m_PixelFormat;
        uint8_t                 m_PixelSize;
        uint8_t                 m_Alpha;
        Cartesian_t             m_Size;
        uint32_t                m_TotalSize;
        uint32_t                m_Color;
        uint32_t                m_TextColor;

        // Static variables
        static Layer_e          m_ActiveDrawingLayer;

      #ifdef GRAFX_USE_BACKGROUND_LAYER
        static CLayer*          m_pActiveBG_Layer;
        static CLayer*          m_pConstructBG_Layer;
      #endif
        static CLayer*          m_pActiveFG_Layer;
        static CLayer*          m_pConstructFG_Layer;
        static uint8_t          m_LayerStackCounter;
        static Layer_e          m_LayerStack[CLAYER_STACK_LEVEL];
        static const uint8_t    m_LayerPixelSize[PIXEL_FORMAT_COUNT];
};

//-------------------------------------------------------------------------------------------------
// Global variable(s)
//-------------------------------------------------------------------------------------------------

extern CLayer    LayerTable[LAYER_COUNT];

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

void                ClearLayer                  ();
void                SetConstructLayer           (LayerType_e LayerType, Layer_e Layer);
void                SetDrawingOnConstructLayer  (LayerType_e LayerType);
void                SetAlphaLayer               (uint8_t Alpha);

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
