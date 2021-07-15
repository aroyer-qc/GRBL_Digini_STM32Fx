//-------------------------------------------------------------------------------------------------
//
//  File : lib_database_wrapper.cpp
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
#include "lib_macro.h"
#include "lib_database_wrapper.h"
#include "lib_time_typedef.h"
#include "bsp.h" // temporary see bellow TODO
#include "driver_cfg.h"


//-------------------------------------------------------------------------------------------------
//
//   Function:      AccessTime
//
//   Description:   Request time
//
//   Note(s):       AccessTime() does not use Number and SubNumber
//
//-------------------------------------------------------------------------------------------------
void AccessTime(AccessRequest_e AccessRequest, void* pData, uint16_t Number, uint16_t SubNumber)
{
    VAR_UNUSED(Number);
    VAR_UNUSED(SubNumber);

  #if (USE_RTC_DRIVER == DEF_ENABLED)
    if(AccessRequest == ACCESS_READ) BSP_pRTC->GetTime((Time_t*)pData);
    else                             BSP_pRTC->SetTime((Time_t*)pData);
  #else // User function
    if(AccessRequest == ACCESS_READ) BSP_GetTime((Time_t*)pData);
    else                             BSP_SetTime((Time_t*)pData);
  #endif
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      AccessDate
//
//   Description:   Request date
//
//-------------------------------------------------------------------------------------------------
void AccessDate(AccessRequest_e AccessRequest, void* pData, uint16_t Number, uint16_t SubNumber)
{
    VAR_UNUSED(Number);
    VAR_UNUSED(SubNumber);

  #if (USE_RTC_DRIVER == DEF_ENABLED)
    if(AccessRequest == ACCESS_READ) BSP_pRTC->GetDate((Date_t*)pData);
    else                             BSP_pRTC->SetDate((Date_t*)pData);
  #else // User function
    if(AccessRequest == ACCESS_READ) BSP_GetDate((Date_t*)pData);
    else                             BSP_SetDate((Date_t*)pData);
  #endif
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      AccessTimeFormat
//
//   Description:   Request date
//
//-------------------------------------------------------------------------------------------------
void AccessTimeFormat(AccessRequest_e AccessRequest, void* pData, uint16_t Number, uint16_t SubNumber)
{
    VAR_UNUSED(Number);
    VAR_UNUSED(SubNumber);

  #if (USE_RTC_DRIVER == DEF_ENABLED)
    if(AccessRequest == ACCESS_READ) BSP_pRTC->GetTimeFormat((TimeFormat_e*)pData);
    else                             BSP_pRTC->SetTimeFormat((TimeFormat_e*)pData);
  #else // User function
  #endif
}

//-------------------------------------------------------------------------------------------------
