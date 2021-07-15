//-------------------------------------------------------------------------------------------------
//
//  File : lib_grafx.cpp
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

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#define GFX_GLOBAL
#include "lib_grafx.h"
#undef  GFX_GLOBAL
#include <stdint.h>
#include "bsp.h"
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------
// const(s)
//-------------------------------------------------------------------------------------------------

#if 0
// Calculate the offset for the Free memory after the layers used by GRAFX
#ifdef LAYER_DEF
const uint32_t GFX_LoadingAddress =
  #define X_LAYER(ENUM_ID, WORK_LAYER, PIXEL_FORMAT, SIZE_X, SIZE_Y) ((SIZE_X * SIZE_Y) * GFX_PixelSize[PIXEL_FORMAT]) +
    LAYER_DEF
  #undef X_LAYER
    GFX_BASE_ADDRESS;
#endif
#endif

const uint32_t GFX_LoadingAddress = GFX_BASE_ADDRESS;

//-------------------------------------------------------------------------------------------------
// External low level function from driver
//-------------------------------------------------------------------------------------------------

extern void    DRV_Config                     (void);
extern void    DRV_DisplayOn                  (void);
extern void    DRV_DisplayOff                 (void);

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GRAFX_Initialize
//
//   Parameter(s):  None
//   Return Value:  SystemState_e
//
//   Description:   Perform pre-OS initialization
//
//-------------------------------------------------------------------------------------------------
SystemState_e GRAFX_Initialize(void)
{
   // SystemState_e State;
   // nOS_Error     Error;
    uint32_t      Address;

    // Init display device
    myGrafx->Initialize(GRAFX_DRIVER_ARGUMENT);                                                         // Call the right driver according to configuration

  #ifdef LAYER_DEF
    // Precalculate address for each layer and also clear the layer
    Address = GFX_BASE_ADDRESS;
    for(int Layer = LAYER_FIRST_ITEM; Layer < LAYER_COUNT; Layer++)
    {
        LayerTable[Layer].SetAddress(Address);
        Address += LayerTable[Layer].GetTotalSize();

        LayerTable[Layer].Clear();
    }
  #endif

  #ifdef GFX_USE_SOFT_ALPHA
    GFX_PrecomputeAlphaTable();
  #endif

    CLayer::SetActiveLayer(LAYER_FOREGROUND, FOREGROUND_DISPLAY_LAYER_0);

  #if GRAFX_USE_BACKGROUND_LAYER
    CLayer::SetActiveLayer(LAYER_BACKGROUND, BACKGROUND_DISPLAY_LAYER_0);
    CLayer::SetDrawing(BACKGROUND_DISPLAY_LAYER_0);
  #else
    CLayer::SetDrawing(FOREGROUND_DISPLAY_LAYER_0);
  #endif

    return SYS_READY;
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: GRAFX_PostInitialize
//
//   Parameter(s):  None
//   Return Value:  SystemState_e
//
//   Description:   Perform Post-OS initialization
//-------------------------------------------------------------------------------------------------
SystemState_e GRAFX_PostInitialize(void)
{
  #ifdef DIGINI_USE_POINTING_DEVICE
    SystemState_e State;
  #endif
    nOS_Error     Error;
    uint32_t      FreePointer;

    DB_Central.Set(&GFX_LoadingAddress, GFX_FREE_RAM_POINTER, 0, 0);   // Record the free SDRAM pointer in database at reload ID

    FONT_Initialize();

    if((Error = GUI_pTask->Initialize()) != NOS_OK)
    {
        return SYS_FAIL;
    }

  #ifdef DIGINI_USE_A_SKIN
    //  SKIN_pTask need then at the same pointer
    DB_Central.Get(&FreePointer, GFX_FREE_RAM_POINTER,  0, 0);
    LIB_AlignPointer(FreePointer);
    DB_Central.Set(&FreePointer, GFX_FREE_RAM_POINTER,  0, 0);
    DB_Central.Set(&FreePointer, GFX_FREE_RELOAD_POINTER, 0, 0);

    if((Error = SKIN_pTask->Initialize()) != NOS_OK)
    {
        return SYS_FAIL;
    }
  #endif

  #ifdef DIGINI_USE_POINTING_DEVICE
    if((State = PDI_pDriver->Initialize(GRAFX_PostInitSubDriverPtr.PDI_pHardInterface)) != SYS_READY)
    {
        return State;
    }
    if((State = PDI_pTask->Initialize(PDI_pDriver, GRAFX_SIZE_X, GRAFX_SIZE_Y, PDI_SWAP_NONE)) != SYS_READY)
    {
        return State;
    }
  #endif

    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_GRAFX
