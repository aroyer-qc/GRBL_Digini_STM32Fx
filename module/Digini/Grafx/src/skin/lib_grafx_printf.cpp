//-------------------------------------------------------------------------------------------------
//
//  File : lib_grafx_printf.cpp
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

//------ Note(s) ----------------------------------------------------------------------------------
//
//  only support 'wchar_t' for future compatibility
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>
#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "lib_grafx.h"
#include "bsp.h"

//-------------------------------------------------------------------------------------------------
//
//   Function Name: WidgetPrint
//
//   Parameter(s):  Text_t*              pText,
//                  ServiceReturn_t*     pService
//   Return Value:  size_t              Size
//
//   Description:   Print a formatted text on the display.
//
//   note:          It received information from the Text_t structure
//                  It received te data from ServiceReturn_t structure
//
//-------------------------------------------------------------------------------------------------
size_t WidgetPrint(Text_t* pText, ServiceReturn_t* pService)
{
    GPrintf     Printf;
    char*       pString;
    Language_e  Language;
    size_t      Size = 0;

    if(pText->Label != INVALID_LABEL)
    {
        CLayer::SetTextColor(pText->Color[pService->IndexState]);
        FontDefault.Set(pText->Font);
        SetXY_Justification(pText->Options);
    //    DB_Central.Get(&Language, SYSTEM_LANGUAGE, 0, 0);         // hide this from BSP... it should be available in Digini
    Language = LANG_ENGLISH;

        DB_Central.Get(&pString, APPLICATION_LABEL, pText->Label, Language);

        if(pText->Blend == CLEAR_BLEND)
        {
            CLayer::SetColor(TRANSPARENT);
            DrawRectangle(&pText->Box);
        }

      #ifdef GRAFX_PAINT_BOX_DEBUG
        if((pText->Box.Size.Width != 0) && (pText->Box.Size.Height != 0))
        {
            CLayer::SetColor(GRAFX_PAINT_BOX_DEBUG_COLOR);
            DrawDebugBox(&pText->Box);
        }
      #endif

        if(pService->ServiceType == SERVICE_RETURN_TYPE1)
        {
            Size = Printf.Draw(&pText->Box, (const char*)pString, ((ServiceType1_t*)pService)->Data);
        }
        else if(pService->ServiceType == SERVICE_RETURN_TYPE2)         // it might go up to maximum
        {
            Size = Printf.Draw(&pText->Box, (const char*)pString, ((ServiceType2_t*)pService)->Data[0],
                                                                  ((ServiceType2_t*)pService)->Data[1],
                                                                  ((ServiceType2_t*)pService)->Data[2],
                                                                  ((ServiceType2_t*)pService)->Data[3],
                                                                  ((ServiceType2_t*)pService)->Data[4],
                                                                  ((ServiceType2_t*)pService)->Data[5],
                                                                  ((ServiceType2_t*)pService)->Data[6],
                                                                  ((ServiceType2_t*)pService)->Data[7],
                                                                  ((ServiceType2_t*)pService)->Data[8],
                                                                  ((ServiceType2_t*)pService)->Data[9],
                                                                  ((ServiceType2_t*)pService)->Data[10],
                                                                  ((ServiceType2_t*)pService)->Data[11],
                                                                  ((ServiceType2_t*)pService)->Data[12],
                                                                  ((ServiceType2_t*)pService)->Data[13],
                                                                  ((ServiceType2_t*)pService)->Data[14],
                                                                  ((ServiceType2_t*)pService)->Data[15]);
        }
        else if(pService->ServiceType == SERVICE_RETURN_TYPE4)         // it might go up to maximum
        {


// TODO (Alain#1#):           redo and test this

            // Service 4 is use to print a series of value using %d and order position...
            // if(((ServiceType4_t*)pService)->Mode == 1)
            // {
            //     DB_Central.Get(&pString, APPLICATION_LABEL, ((ServiceType4_t*)pService)->AlternatLabel_e, Language);
            // }
            Size = Printf.Draw(&pText->Box, (const char*)pString, ((ServiceType4_t*)pService)->pString[0],
                                                                  ((ServiceType4_t*)pService)->pString[1],
                                                                  ((ServiceType4_t*)pService)->pString[2],
                                                                  ((ServiceType4_t*)pService)->pString[3],
                                                                  ((ServiceType4_t*)pService)->pString[4],
                                                                  ((ServiceType4_t*)pService)->pString[5],
                                                                  ((ServiceType4_t*)pService)->pString[6],
                                                                  ((ServiceType4_t*)pService)->pString[7],
                                                                  ((ServiceType4_t*)pService)->pString[8],
                                                                  ((ServiceType4_t*)pService)->pString[9],
                                                                  ((ServiceType4_t*)pService)->pString[10],
                                                                  ((ServiceType4_t*)pService)->pString[11],
                                                                  ((ServiceType4_t*)pService)->pString[12],
                                                                  ((ServiceType4_t*)pService)->pString[13],
                                                                  ((ServiceType4_t*)pService)->pString[14],
                                                                  ((ServiceType4_t*)pService)->pString[15]);
        }
        else
        {
            if(pService->ServiceType == SERVICE_RETURN_TYPE8)
            {
                __asm("nop");
            }


            Size = Printf.Draw(&pText->Box, (const char*)pString);
        }
    }
    return Size;
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

