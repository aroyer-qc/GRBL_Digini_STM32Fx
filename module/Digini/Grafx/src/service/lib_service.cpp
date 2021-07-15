//-------------------------------------------------------------------------------------------------
//
//  File : lib_service.cpp
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
//
//
//
//
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <ctype.h>
#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "lib_grafx.h"
#include "lib_digini.h"
#include "lib_time_typedef.h"
#include "lib_class_database.h"
#include "lib_memory.h"
#include "lib_service.h"
#include "lib_utility.h"

//-------------------------------------------------------------------------------------------------
// Check(s)
//-------------------------------------------------------------------------------------------------

#if defined(DIGINI_USE_SERV_INPD) || \
    defined(DIGINI_USE_SERV_INPF) || \
    defined(DIGINI_USE_SERV_INPH) || \
    defined(DIGINI_USE_SERV_INPS)
  #ifndef DIGINI_USE_SERV_XCHG
    #define DIGINI_USE_SERV_XCHG                    // Must be define if input service are used
  #endif
#endif

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define INPx_DECIMAL_SIZE               21
#define INPx_HEXADECIMAL_SIZE           9           // TODO (Alain#2#) implement the HEXA Part
#define INPx_FLOAT_SIZE                 16
#define INPx_STRING_SIZE                64

//-------------------------------------------------------------------------------------------------
// Private global variable(s)
//-------------------------------------------------------------------------------------------------

const uint8_t ServiceSizeType[NB_SERVICE_CONST] =
{
    sizeof(ServiceReturn_t),
    sizeof(ServiceType1_t),
    sizeof(ServiceType2_t),
    sizeof(ServiceType3_t),
    sizeof(ServiceType4_t),
    sizeof(ServiceType5_t),
    sizeof(ServiceType6_t),
    sizeof(ServiceType7_t),
    sizeof(ServiceType8_t),
    sizeof(ServiceInvalid_t),
};


char Buffer[20];   // TODO fix this for needed size by using mem alloc class

//-------------------------------------------------------------------------------------------------
//
//
//  Private Function
//
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_BACK
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//  Return:         ServiceReturn_t
//
//  Description:    This function is used to display a backgrounf, in a single draw
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_BACK(ServiceEvent_e* pServiceState)
{
    ServiceReturn_t* pService = nullptr;

    if(*pServiceState == SERVICE_START)
    {
        pService = GetServiceStruct(SERVICE_RETURN);
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_BDEF
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//  Return:         ServiceReturn_t
//
//  Description:    This function is used to refresh button, and panel
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_BDEF(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    ServiceReturn_t* pService = nullptr;

  #ifdef GRAFX_USE_SLIDING_PAGE
    if(*pServiceState == SERVICE_RELEASED)
    {
        GUI_Task.SetSlidingDirection(SlideDir_e(SubService));

/*  this test show how to slide partial screen
        PageSlideRange_t Test;
        Test.StartPos = 100;
        Test.EndPos = 200;
        GUI_Task.SetSlidingRange (&Test);
*/
    }
  #endif

    if(*pServiceState != SERVICE_FINALIZE)
    {
        pService = GetServiceStruct(SERVICE_RETURN);
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_DATE
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//  Return:         ServiceReturn_t
//
//  Description:    This function return the current date in service
//
//-------------------------------------------------------------------------------------------------
#ifdef DIGINI_USE_SERV_DATE
static ServiceReturn_t* SERV_DATE(ServiceEvent_e* pServiceState)
{
    ServiceReturn_t* pService = nullptr;
    Date_t           Date;
    static Date_t    OldDate;

    if(*pServiceState == SERVICE_START)
    {
        OldDate.Day = 32;   // Force refresh
    }

    if(*pServiceState != SERVICE_FINALIZE)
    {
        DB_Central.Get(&Date, REC_DATE, 0, 0);
        if(Date.Day != OldDate.Day)
        {
            pService = GetServiceStruct(SERVICE_RETURN_TYPE2);
            if(pService != nullptr)
            {
                ((ServiceType2_t*)pService)->Data[0].u_16 = Date.Day;
                ((ServiceType2_t*)pService)->Data[1].u_16 = Date.Month;
                ((ServiceType2_t*)pService)->Data[2].u_16 = 2000 + Date.Year;
                *pServiceState = SERVICE_REFRESH;
                OldDate = Date;
            }
        }
    }

    return pService;
}
#endif // DIGINI_USE_SERV_DATE

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_INPT
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    This function is used to input float/Decimal/Hexa data value with an
//                  on screen keypad.
//
//  Note(s():       Only one instance per page with the SubService = SUB_SERVICE_CFG
//
//-------------------------------------------------------------------------------------------------
#if defined(DIGINI_USE_SERV_INPD) || defined(DIGINI_USE_SERV_INPF) || defined(DIGINI_USE_SERV_INPH)
static ServiceReturn_t* SERV_INPT(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    static char*          pStr = nullptr;
    ServiceReturn_t*      pService;

  #if defined (DIGINI_USE_SERV_INPD) || defined(DIGINI_USE_SERV_INPH)
    static InputDecimal_t Input;
  #endif
  #if defined (DIGINI_USE_SERV_INPF)
    static InputFloat_t   InputF;
  #endif
    static uint8_t        StrMaxLength;
    size_t                Length;
    ExchangeCommon_t*     pCommon = (ExchangeCommon_t*)GUI_pMailBox;

    // --------------------------------------------------------------------------------------------
    // Get Data from GUI_Mailbox
    if(*pServiceState == SERVICE_START)
    {
        if(pStr == nullptr)     // Configuration phase of the service
        {
          #if defined(DIGINI_USE_SERV_INPF)
            if(pCommon->ExType == EXCHANGE_INPUT_TYPE_FLOAT)
            {
                memcpy(&InputF, GUI_pMailBox, sizeof(InputFloat_t));    // Get the working data from mailbox
                if((pStr = (char*)pMemory->Alloc(INPx_FLOAT_SIZE)) != nullptr)
                {
                    LIB_ProcessFloatToString(pStr, (double)*((float*)InputF.Common.pValue), 5);
                    StrMaxLength = INPx_FLOAT_SIZE;
                }
            }
          #endif
          #if defined(DIGINI_USE_SERV_INPF) && (defined(DIGINI_USE_SERV_INPD) || defined(DIGINI_USE_SERV_INPH))
            else
          #endif
          #if defined(DIGINI_USE_SERV_INPD) || defined(DIGINI_USE_SERV_INPH)
            {
                memcpy(&Input, GUI_pMailBox, sizeof(InputDecimal_t));   // Get the working data from mailbox
                size_t Size = (pCommon->ExType == EXCHANGE_INPUT_TYPE_DECIMAL) ? INPx_DECIMAL_SIZE : INPx_HEXADECIMAL_SIZE;

                if((pStr = (char*)pMemory->Alloc(Size)) != nullptr)
                {
                    snprintf(pStr, Size, "%ld", *((int32_t*)Input.Common.pValue)); // TODO what about hexa?
                    StrMaxLength = Size;
                }
            }
          #endif
        }
    }

    if(pStr != nullptr)
    {
        // ----------------------------------------------------------------------------------------
        // Print the Input line
        if(SubService == 'P')
        {
            if(*pServiceState != SERVICE_FINALIZE)
            {
                if((pService = GetServiceStruct(SERVICE_RETURN_TYPE4)) != nullptr)
                {
                  #if defined(DIGINI_USE_SERV_INPF)
                    if(pCommon->ExType == EXCHANGE_INPUT_TYPE_FLOAT)
                    {
                      float Value = atof(pStr);
                      pService->IndexState = ((Value >= InputF.Min) && (Value <= InputF.Max)) ? 0 : 1;    // Change color if value is out of range
                    }
                  #endif
                  #if defined(DIGINI_USE_SERV_INPD)
                    if(pCommon->ExType == EXCHANGE_INPUT_TYPE_DECIMAL)
                    {
                        int32_t Value = atol(pStr);
                        pService->IndexState = ((Value >= Input.Min) && (Value <= Input.Max)) ? 0 : 1;      // Change color if value is out of range
                    }
                  #endif
                  #if defined(DIGINI_USE_SERV_INPH)
                    if(pCommon->ExType == EXCHANGE_INPUT_TYPE_HEXA)
                    {
                        uint32_t Value;

                        sscanf(pStr,"%lx",&Value);
                        pService->IndexState = ((Value >= uint32_t(Input.Min)) && (Value <= uint32_t(Input.Max))) ? 0 : 1;      // Change color if value is out of range
                    }
                  #endif


                    ((ServiceType4_t*)pService)->pString[0] = (strlen(pStr) == 0) ? (char*)"0" : pStr;
                    *pServiceState = SERVICE_REFRESH;
                }
            }
        }
        // ----------------------------------------------------------------------------------------
        // Print the title line
        else if(SubService == 'L')
        {
            if((pService = GetServiceStruct(SERVICE_RETURN_TYPE4)) != nullptr)
            {
                Language_e  Language;
                char*       pString;

                // TODO (alain#1#) use language
                Language = LANG_ENGLISH;//DB_Central.Get(&Language, SYSTEM_LANGUAGE, 0, 0);
                DB_Central.Get(&pString, APPLICATION_LABEL, pCommon->Label, Language);
                ((ServiceType4_t*)pService)->pString[0] = pString;
                *pServiceState = SERVICE_REFRESH;
            }
        }
        // ----------------------------------------------------------------------------------------

        else if(*pServiceState != SERVICE_FINALIZE)
        {
            pService = GetServiceStruct(SERVICE_RETURN);
            Length = strlen(pStr);

            // ------------------------------------------------------------------------------------
            // Section Editing of the value in string and also button appearance are processed
            if((SubService >= '0') && (SubService <= '9'))
            {
                if(*pServiceState == SERVICE_RELEASED)
                {
                    if((atof(pStr) == 0) && (Length == 1))
                    {
                        if(SubService != '0')   // Already zero !  don't add more!.
                        {
                            *pStr = SubService;
                        }
                    }
                    else
                    {
                        *(pStr + Length) = SubService;
                        Length++;
                    }
                }
            }
          #if defined(DIGINI_USE_SERV_INPH)
            else if((toupper(SubService) >= 'A') && (toupper(SubService) <= 'F'))
            {
                if(pCommon->ExType != EXCHANGE_INPUT_TYPE_HEXA)
                {
                    pService->IndexState = 2;           // Invalidate button
                }
                else if(*pServiceState == SERVICE_RELEASED)
                {
                    uint32_t Value;

                    sscanf(pStr,"%lx",&Value);

                    if((Value == 0) && (Length == 1))
                    {
                        *pStr = SubService;
                    }
                    else
                    {
                        *(pStr + Length) = SubService;
                        Length++;
                    }
                }
            }
          #endif
            else if(SubService == '±')
            {
              #if defined(DIGINI_USE_SERV_INPF)
                if(pCommon->ExType == EXCHANGE_INPUT_TYPE_FLOAT)
                {
                    if((InputF.Min >= 0) || ((atof(pStr) == 0) && (Length <= 1)))
                    {
                        pService->IndexState = 2;       // Invalidate the button +/-
                    }
                }
              #endif
              #if defined(DIGINI_USE_SERV_INPD)
                if(pCommon->ExType == EXCHANGE_INPUT_TYPE_DECIMAL)
                {
                    if((Input.Min >= 0) || ((atol(pStr) == 0) && (Length <= 1)))
                    {
                        pService->IndexState = 2;       // Invalidate the button +/-
                    }
                }
              #endif
              #if defined(DIGINI_USE_SERV_INPH)
                if(pCommon->ExType == EXCHANGE_INPUT_TYPE_HEXA)
                {
                    pService->IndexState = 3;           // Do not draw the button
                }
              #endif

                if((*pServiceState == SERVICE_RELEASED) && (pService->IndexState == 0))
                {
                    if(*pStr == '-')
                    {
                        // Remove negative sign by moving value upfront;
                        memcpy(pStr, pStr + 1, Length + 1);
                        Length--;
                    }
                    else
                    {
                        // Moving value back of string to add negative sign in front
                        for(int i = Length; i > 0; i--)
                        {
                            *(pStr + i) = *(pStr + (i - 1));
                        }

                        *pStr = '-';
                        Length++;
                    }
                }

                *pServiceState = SERVICE_REFRESH;
            }
            else if(SubService == '<')
            {
                if((Length <= 1) && (*pStr == '0'))
                {
                    pService->IndexState = 2;           // Invalidate button DEL
                }
                else if(*pServiceState == SERVICE_RELEASED)
                {
                    if(Length > 0)
                    {
                        Length--;
                        if(Length != 0)
                        {
                            *(pStr + Length) = '\0';
                        }
                        else
                        {
                            *pStr = '0';
                            Length++;
                        }
                    }

                }

                *pServiceState = SERVICE_REFRESH;
            }
            else if(SubService == '.')
            {
                if((strchr(pStr, '.') != nullptr) || (pCommon->ExType != EXCHANGE_INPUT_TYPE_FLOAT))
                {
                    pService->IndexState = 2;           // Invalidate button DOT
                }
                else if(*pServiceState == SERVICE_RELEASED)
                {
                    if(strchr(pStr, '.') == nullptr)
                    {
                        if(atof(pStr) == 0)
                        {
                            *pStr = '0';
                        }

                        *(pStr + Length) = '.';
                        Length++;
                    }
                }

                *pServiceState = SERVICE_REFRESH;
            }
            // ------------------------------------------------------------------------------------
            // Section button Ok handling and status
            else if(SubService == '@')  // Ok
            {
              // Invalidate button if value is out of range
              #if defined(DIGINI_USE_SERV_INPF)
                if(pCommon->ExType == EXCHANGE_INPUT_TYPE_FLOAT)
                {
                  float Value = atof(pStr);
                  pService->IndexState = ((Value >= InputF.Min) && (Value <= InputF.Max)) ? 0 : 2;
                }
              #endif
              #if defined(DIGINI_USE_SERV_INPD)
                if(pCommon->ExType == EXCHANGE_INPUT_TYPE_DECIMAL)
                {
                    int32_t Value = atol(pStr);
                    pService->IndexState = ((Value >= Input.Min) && (Value <= Input.Max)) ? 0 : 2;
                }
              #endif
              #if defined(DIGINI_USE_SERV_INPH)
                if(pCommon->ExType == EXCHANGE_INPUT_TYPE_HEXA)
                {
                    uint32_t Value;

                    sscanf(pStr,"%lx",&Value);
                    pService->IndexState = ((Value >= uint32_t(Input.Min)) && (Value <= uint32_t(Input.Max))) ? 0 : 2;
                }
              #endif

                if(pService->IndexState == 0)
                {
                    if(*pServiceState == SERVICE_RELEASED)
                    {
                      #if defined(DIGINI_USE_SERV_INPF)
                        if(pCommon->ExType == EXCHANGE_INPUT_TYPE_FLOAT)
                        {
                            *((float*)(InputF.Common.pValue)) = atof(pStr);
                            memcpy(GUI_pMailBox, &Input, sizeof(InputFloat_t));     // return value to mailbox
                        }
                      #endif
                      #if defined(DIGINI_USE_SERV_INPF) && (defined(DIGINI_USE_SERV_INPD) || defined(DIGINI_USE_SERV_INPH))
                        else
                      #endif
                      #if defined(DIGINI_USE_SERV_INPD) || defined(DIGINI_USE_SERV_INPH)
                        {
                            *((uint32_t*)Input.Common.pValue) = atoll(pStr);
                            memcpy(GUI_pMailBox, &Input, sizeof(InputDecimal_t));   // return value to mailbox
                        }
                      #endif

                        pMemory->Free((void**)&pStr);
                        pStr = nullptr;
                    }
                    else
                    {
                        *pServiceState = SERVICE_REFRESH;
                    }
                }
                else
                {
                    *pServiceState = SERVICE_REFRESH;
                }
            }

            // ------------------------------------------------------------------------------------
            // Trap negative sign on empty line
            if((Length == 1) && (*pStr == '-'))
            {
               *pStr = '0';
            }

            if(Length >= StrMaxLength)     // Protect overflow working one char less than max length
            {
                *(pStr + StrMaxLength) = '\0';
            }
            else
            {
                *(pStr + Length) = '\0';
            }
        }
    }

    return pService;
}
#endif // DIGINI_USE_SERV_INPF

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_NONE
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//  Return:         ServiceReturn_t
//
//  Description:    This service is used for static display of label, icon
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_NONE(ServiceEvent_e* pServiceState)
{
    ServiceReturn_t* pService = nullptr;

    if(*pServiceState != SERVICE_FINALIZE)
    {
        pService = GetServiceStruct(SERVICE_RETURN);
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_RFSH
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//  Return:         ServiceReturn_t
//
//  Description:    This service is used for static display of label, icon
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_RFSH(ServiceEvent_e* pServiceState)
{
    ServiceReturn_t* pService = nullptr;

    if(*pServiceState != SERVICE_FINALIZE)
    {
        pService = GetServiceStruct(SERVICE_RETURN);
        *pServiceState = SERVICE_REFRESH;
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_SKLD
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    This service is used for to get status on the dynamic skin loading
//
//-------------------------------------------------------------------------------------------------
#if defined DIGINI_USE_LOAD_SKIN
static ServiceReturn_t* SERV_SKLD(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    ServiceReturn_t* pService = nullptr;

    if(*pServiceState != SERVICE_FINALIZE)
    {
        switch(SubService)
        {
            case 0:
            {
                // if Skin is loaded then we return with valid service to jump to new link page
                pService = GetServiceStruct((SKIN_pTask->IsSkinLoaded() == true) ? SERVICE_RETURN : SERVICE_RETURN_INVALID_LINK);
                *pServiceState = SERVICE_REFRESH;
                break;
            }

            case 1:
            {
                pService = GetServiceStruct(SERVICE_RETURN);
                *pServiceState = SERVICE_REFRESH;
                break;
            }

            case 2: // Return the % of the skin loaded
            {
                pService = GetServiceStruct(SERVICE_RETURN_TYPE1);
                if(pService != nullptr)
                {
                    ((ServiceType1_t*)pService)->Data = SKIN_pTask->PercentLoader();
                }
                break;
            }
        }
    }

    return pService;
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_TCHD
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:
//
//-------------------------------------------------------------------------------------------------
/*
static ServiceReturn_t* SERV_TCHD(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    ServiceReturn_t* pService = nullptr;

    //static eTCHDState State = THCD_INIT;

    static Cartesian_t TouchPoint[2];
    static Cartesian_t DesiredPoint[2];
    static sLinearEquation CorrectionEq[2];

    static bool IsInitialized = false;
    static bool IsCalibrated = false;

    pService = GetServiceStruct(SERVICE_RETURN);
    pService->IndexState = 0;                                                   // Draw button

    if(!IsInitialized)
    {
      //  DesiredPoint[0].u_16.X = Button[CALIB_TOP].TouchArea.Rectangle.Box.Pos.u_16.X + Button[CALIB_TOP].TouchArea.Rectangle.Box.Size.Width / 2;
      //  DesiredPoint[0].u_16.Y = Button[CALIB_TOP].TouchArea.Rectangle.Box.Pos.u_16.Y + Button[CALIB_TOP].TouchArea.Rectangle.Box.Size.Height / 2;
      //  DesiredPoint[1].u_16.X = Button[CALIB_BOTTOM].TouchArea.Rectangle.Box.Pos.u_16.X + Button[CALIB_BOTTOM].TouchArea.Rectangle.Box.Size.Width / 2;
      //  DesiredPoint[1].u_16.Y = Button[CALIB_BOTTOM].TouchArea.Rectangle.Box.Pos.u_16.Y + Button[CALIB_BOTTOM].TouchArea.Rectangle.Box.Size.Height / 2;
        IsInitialized = true;
    }

    if(!IsCalibrated)
    {
        switch(SubService)
        {
        case 0:
            if(*pServiceState == SERVICE_PRESSED || *pServiceState == SERVICE_TYPEMATIC || *pServiceState == SERVICE_SUPERKEY)
            {
                BSP_PDI->GetEventPoint(&TouchPoint[0]);
                //DB_Central.Get(&TouchPoint[0], EVENT_POINT, 0, 0);

                pService = GetServiceStruct(SERVICE_TYPE3);
                ((ServiceType3_t*)pService)->Link = INVALID_LINK;//1;

                pService->IndexState = SERVICE_INDEX_STATE_NO_DRAW;
            }
        break;

        case 1:
            if(*pServiceState == SERVICE_PRESSED || *pServiceState == SERVICE_TYPEMATIC || *pServiceState == SERVICE_SUPERKEY)
            {
                BSP_PDI->GetEventPoint(&TouchPoint[1]);
               // DB_Central.Get(&TouchPoint[1], EVENT_POINT, 0 , 0);

                //pService = GetServiceStruct(SERVICE_TYPE3);
                //((ServiceType3_t*)pService)->Link = 0;
                pService->IndexState = SERVICE_INDEX_STATE_NO_DRAW;

                CorrectionEq[0].Slope = ((DesiredPoint[1].X - DesiredPoint[0].X ) * 1000) / (TouchPoint[1].X  - TouchPoint[0].X );
                CorrectionEq[0].Y_Intercept = 1000 * DesiredPoint[0].X  -  CorrectionEq[0].Slope * TouchPoint[0].X ;

                CorrectionEq[1].Slope = ((DesiredPoint[1].Y - DesiredPoint[0].Y ) * 1000) / (TouchPoint[1].Y  - TouchPoint[0].Y );
                CorrectionEq[1].Y_Intercept = 1000 * DesiredPoint[0].Y  -  CorrectionEq[1].Slope * TouchPoint[0].Y ;

                BSP_pTSC->SetTouchCorrection(CorrectionEq[0], CorrectionEq[1]);
                IsCalibrated = true;
            }

          break;
        }
    }

    return pService;
    return nullptr;
}
    */

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_TIME
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//  Return:         ServiceReturn_t
//
//  Description:    This function return the current time in service
//
//-------------------------------------------------------------------------------------------------
#ifdef DIGINI_USE_SERV_TIME
static ServiceReturn_t* SERV_TIME(ServiceEvent_e* pServiceState)
{
    ServiceReturn_t* pService = nullptr;
    Time_t           Time;
    static Time_t    OldTime;

    if(*pServiceState == SERVICE_START)
    {
        OldTime.Second = 25;   // Force refresh
    }

    if(*pServiceState != SERVICE_FINALIZE)
    {
        DB_Central.Get(&Time, REC_TIME, 0, 0);
        if(Time.Second != OldTime.Second)
        {
            pService = GetServiceStruct(SERVICE_RETURN_TYPE2);
            if(pService != nullptr)
            {
                ((ServiceType2_t*)pService)->Data[0].u_16 = Time.Hour;
                ((ServiceType2_t*)pService)->Data[1].u_16 = Time.Minute;
                ((ServiceType2_t*)pService)->Data[2].u_16 = Time.Second;
                *pServiceState = SERVICE_REFRESH;
                OldTime = Time;
            }
        }
    }

    return pService;
}
#endif // DIGINI_USE_SERV_TIME

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_XCHG
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    This function act like SERV_BDEF service but it add
//                  a configuration set GUI_pMailBox to the struct pointer specified by SubService
//
//-------------------------------------------------------------------------------------------------
#ifdef DIGINI_USE_SERV_XCHG
static ServiceReturn_t* SERV_XCHG(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    ServiceReturn_t* pService = nullptr;
    ExchangeType_e   ExchangeType;
    Language_e       Language;
    char*            pString;
    Label_e          FormatLabel;

    if(*pServiceState != SERVICE_FINALIZE)
    {
        if(SubService < uint16_t(XCHANGE_COUNT))
        {
            if(*pServiceState == SERVICE_RELEASED)
            {
                GUI_pMailBox = pExchange[SubService];
                pService = GetServiceStruct(SERVICE_RETURN);
            }
            else
            {
                ExchangeType = pExchange[SubService]->ExType;

                Language = LANG_ENGLISH;//DB_Central.Get(&Language, SYSTEM_LANGUAGE, 0, 0);

                switch(ExchangeType)
                {
                    case EXCHANGE_INPUT_TYPE_DECIMAL:
                    {
                        pService = GetServiceStruct(SERVICE_RETURN_TYPE4);
                        FormatLabel = ((InputDecimal_t*)pExchange[SubService])->FormatLabel;
                        if(FormatLabel != INVALID_LABEL)
                        {
                            DB_Central.Get(&pString, APPLICATION_LABEL, FormatLabel, Language);
                            snprintf(&Buffer[0], 20, pString, *((uint32_t*)pExchange[SubService]->pValue));
                            ((ServiceType4_t*)pService)->pString[0] = &Buffer[0];
                        }
                    }
                    break;

                    case EXCHANGE_INPUT_TYPE_FLOAT:
                    {
                        pService = GetServiceStruct(SERVICE_RETURN_TYPE4);
                        FormatLabel = ((InputFloat_t*)pExchange[SubService])->FormatLabel;
                        if(FormatLabel != INVALID_LABEL)
                        {
                            DB_Central.Get(&pString, APPLICATION_LABEL, FormatLabel, Language);
                            snprintf(&Buffer[0], 20, pString, *((float*)pExchange[SubService]->pValue));
                            ((ServiceType4_t*)pService)->pString[0] = &Buffer[0];
                        }
                    }
                    break;

                    default:
                    {
                        pService = GetServiceStruct(SERVICE_RETURN);
                    }
                    break;
                }
            }
        }
    }

    return pService;
}
#endif // DIGINI_USE_SERV_XCHG

//-------------------------------------------------------------------------------------------------
//
//
//  Public Function
//
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           ServiceCall
//  Parameter(s):   Service_t*       pService
//                  ServiceEvent_e*  pServiceState
//  Return:         ServiceReturn_t
//
//  Description:    Generic callback service provided by library for the application
//
//  Note(s):        Will Call any service from TaskGUI
//
//-------------------------------------------------------------------------------------------------
ServiceReturn_t* ServiceCall(Service_t* pService, ServiceEvent_e* pServiceState)
{
    ServiceReturn_t* pServiceReturn = nullptr;
    s32_t           ServiceRange;
    bool            ServiceWasProcessed = false;

    if(pService->ID != 0xFFFFFFFF)
    {
        ServiceRange.u_32 = pService->ID;

        switch(ServiceRange.u8_Array[0])        // To speed up process
        {
/*
            case 'A':
            {
                switch(pService->ID)
                {
                }
            }
            break;
*/
            case 'B':
            {
                switch(pService->ID)
                {
                    case SERV_ID_BACK: pServiceReturn = SERV_BACK(pServiceState);                  ServiceWasProcessed = true; break;
                    case SERV_ID_BDEF: pServiceReturn = SERV_BDEF(pServiceState, pService->SubID); ServiceWasProcessed = true; break;
                  //case SERV_ID_BOOT: pServiceReturn = SERV_BOOT(pServiceState, pService->SubID); ServiceWasProcessed = true; break;
                }
            }
            break;
/*
            case 'C':
            {
                switch(pService->ID)
                {
                }
            }
            break;
*/
            case 'D':
            {
                switch(pService->ID)
                {
                  #ifdef DIGINI_USE_SERV_DATE
                    case SERV_ID_DATE: pServiceReturn = SERV_DATE(pServiceState); ServiceWasProcessed = true; break;
                  #endif
                }
            }
            break;

/*
            case 'E':
            {
                switch(pService->ID)
                {
                }
            }
            break;

            case 'F':
            {
                switch(pService->ID)
                {
                }
            }
            break;

            case 'G':
            {
                switch(pService->ID)
                {
                }
            }
            break;

            case 'H':
            {
                switch(pService->ID)
                {
                }
            }
            break;
*/
            case 'I':
            {
                switch(pService->ID)
                {
                  #if defined(DIGINI_USE_SERV_INPD) || defined(DIGINI_USE_SERV_INPF) || defined(DIGINI_USE_SERV_INPH)
                    case SERV_ID_INPT: pServiceReturn = SERV_INPT(pServiceState, pService->SubID); ServiceWasProcessed = true; break;
                  #endif
                  #ifdef DIGINI_USE_SERV_INPS
                    case SERV_ID_INPS: pServiceReturn = SERV_INPS(pServiceState, pService->SubID); ServiceWasProcessed = true; break;
                  #endif
                }
            }
            break;
/*
            case 'J':
            {
                switch(pService->ID)
                {
                }
            }
            break;

            case 'K':
            {
                switch(pService->ID)
                {
                }
            }
            break;

            case 'L':
            {
                switch(pService->ID)
                {
                  //case SERV_ID_LIST: pServiceReturn = SERV_LIST(pServiceState, pService->SubID); ServiceWasProcessed = true; break;
                }
            }
            break;
*/
            case 'N':
            {
                switch(pService->ID)
                {
                    case SERV_ID_NONE:pServiceReturn = SERV_NONE(pServiceState);                   ServiceWasProcessed = true; break;
                }
            }
            break;

            case 'S':
            {
                switch(pService->ID)
                {
                  #if defined(DIGINI_USE_LOAD_SKIN)
                    case SERV_ID_SKLD: pServiceReturn = SERV_SKLD(pServiceState, pService->SubID); ServiceWasProcessed = true; break;
                  #endif
                }
            }
            break;

            case 'R':
            {
                switch(pService->ID)
                {
                    case SERV_ID_RFSH: pServiceReturn = SERV_RFSH(pServiceState);                  ServiceWasProcessed = true; break;
                }
            }
            break;

            case 'T':
            {
                switch(pService->ID)
                {
                  #if defined DIGINI_USE_SERV_TIME
                    case SERV_ID_TIME: pServiceReturn = SERV_TIME(pServiceState);                  ServiceWasProcessed = true; break;
                  #endif
                }
            }
            break;
/*
            case 'U':
            {
                switch(pService->ID)
                {
                }
            }
            break;

            case 'V':
            {
                switch(pService->ID)
                {
                }
            }
            break;

            case 'W':
            {
                switch(pService->ID)
                {
                }
            }
            break;
*/
            case 'X':
            {
                switch(pService->ID)
                {
                  #ifdef DIGINI_USE_SERV_XCHG
                    case SERV_ID_XCHG: pServiceReturn = SERV_XCHG(pServiceState, pService->SubID); ServiceWasProcessed = true; break;
                  #endif
                }
            }
            break;
/*
            case 'Y':
            {
                switch(pService->ID)
                {
                }
            }
            break;

            case 'Z':
            {
                switch(pService->ID)
                {
                }
            }
            break;
*/
        }

        if(ServiceWasProcessed == false)
        {
            pServiceReturn = ServiceCallApp(pService, pServiceState);
        }
    }

    return pServiceReturn;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           GetServiceStruct
//
//  Parameter(s):   ServiceType_e    ServiceType Type of struct you want to reserve and initialize
//  Return:         ServiceReturn_t*             Ptr on the newly allocates block for the struct
//
//  Description:    Allocate a block for the size of the structure
//                  Clear the structure
//                  Set the indicator in the structure for the interpretor
//
//-------------------------------------------------------------------------------------------------
ServiceReturn_t* GetServiceStruct(ServiceType_e ServiceType)
{
    ServiceReturn_t* pService = nullptr;

    pService = (ServiceReturn_t*)pMemory->AllocAndClear(ServiceSizeType[ServiceType], 0);

    if(ServiceType == SERVICE_RETURN_TYPE3)
    {
        ((ServiceType3_t*)pService)->Link = INVALID_LINK;
    }

    pService->ServiceType = ServiceType;

    return pService;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           FreeServiceStruct
//
//  Parameter(s):   ServiceReturn_t  pService        ServiceReturn_t pointer
//  Return:         none
//
//  Description:    ServiceReturn_t pointer if freed
//
//-------------------------------------------------------------------------------------------------
void FreeServiceStruct(ServiceReturn_t** pService)
{
    if(pService != nullptr)
    {
        pMemory->Free((void**)pService);
    }
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
