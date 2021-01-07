//-------------------------------------------------------------------------------------------------
//
//  File : lib_tools.cpp
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
#include "lib_typedef.h"
#include "lib_grafx.h"
//#include "lib_define.h"

//-------------------------------------------------------------------------------------------------
// Private variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

static uint8_t Justification;

//-------------------------------------------------------------------------------------------------
// External low level function from driver
//-------------------------------------------------------------------------------------------------

//void DRV_LayerConfig       (CLayer* pLayer);

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetMaxSize
//
//   Parameter(s):  none
//   Return Value:  Cartesian_t      Size
//
//   Description:   set
//
//-------------------------------------------------------------------------------------------------
//Cartesian_t GetMaxSize()
//{
     //return pActiveDrawingLayer->GetSize();
//}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: BoxValid
//
//   Parameter(s):  Box_t*      pBox
//   Return Value:  BOOL        true/false
//
//   Description:   Validate if BOX is in screen range
//
//-------------------------------------------------------------------------------------------------
bool BoxValid(Box_t* pBox)
{
    bool Valid = false;

    if((pBox->Pos.X < GRAFX_SIZE_X) && (pBox->Pos.Y < GRAFX_SIZE_Y))
    {
        if((pBox->Size.Width > 0) && (pBox->Size.Height > 0))
        {
            if(((pBox->Pos.X + pBox->Size.Width - 1) < GRAFX_SIZE_X) && ((pBox->Pos.Y + pBox->Size.Height - 1) < GRAFX_SIZE_Y))
            {
                Valid = true;
            }
        }
    }

    return Valid;
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: SetXY_Justification
//
//   Parameter(s):  uint8_t        _Justification
//   Return Value:  none
//
//   Description:   Set the global justification
//
//-------------------------------------------------------------------------------------------------
void SetXY_Justification(uint8_t _Justification)
{
    Justification = _Justification;
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetXY_Justification
//
//   Parameter(s):  void
//   Return Value:  uint8_t            byJustification
//
//   Description:   Get the global justification
//
//-------------------------------------------------------------------------------------------------
uint8_t GetXY_Justification(void)
{
    return Justification;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GFX_WaitFor_V_Sync
//
//   Parameter(s):  none
//   Return Value:  void
//
//   Description:   set
//
//-------------------------------------------------------------------------------------------------
#ifdef GRAFX_USE_V_SYNC
void WaitFor_V_Sync(void)
{
    myGrafx->WaitFor_V_Sync();
}
#endif

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

