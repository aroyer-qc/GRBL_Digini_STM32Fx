//-------------------------------------------------------------------------------------------------
//
//  File : lib_sprintf.cpp
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
#include "grafx_cfg.h"
#include "lib_memory.h"
#include "lib_string.h"
#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

/*
#define LIB_PAD_ZERO                        (uint8_t)1
#define LIB_PAD_LEFT                        (uint8_t)16
#define LIB_MAX_PRINT_VALUE                 (uint8_t)12

#define LIB_VAR_INT                         (uint8_t)1
#define LIB_VAR_LONG                        (uint8_t)2
*/

#define STR_OPT_PAD_ZERO            128        // value in bit Position for padding with zero
#define STR_OPT_PAD_LEFT            64         // value in bit Position for Padding flag
#define STR_OPT_LOWERCASE           32       // value in bit Position for base
#define STR_OPT_SIGN_NEGATIVE       16         // value in bit Position for sign
#define STR_OPT_BASE_HEXA           8       // value in bit Position for base


#define STR_putchar(s,c)                      {*((char*)s) = (char)(c);}


//-------------------------------------------------------------------------------------------------
// enum(s)
//-------------------------------------------------------------------------------------------------

enum VarLength_e
{
    STR_VAR_16,
    STR_VAR_32,
    STR_VAR_64,
};

//-------------------------------------------------------------------------------------------------
// Forward declaration
//-------------------------------------------------------------------------------------------------
static size_t STR_printi     (char* pOut, int64_t Value, size_t Width, uint8_t Option);
static size_t STR_prints     (char* pOut, char* pString, size_t Width, uint8_t Option);
static size_t STR_str2str    (char* pOut, const char* pString);

//-------------------------------------------------------------------------------------------------
// Private function(s)
//-------------------------------------------------------------------------------------------------

static const char StrNULL[7] = "(null)";

//-------------------------------------------------------------------------------------------------
// Private function(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Function Name: STR_prints
//
//   Parameter(s):  char*   pOut        Output string pointer
//                  char*   pString     String to print
//                  size_t  Width       Width in the print string
//                  uint8_t Option      Padding, Space or 0
//   Return Value:  size_t
//
//   Description:   Service function to print string for printf and sprintf
//
//-------------------------------------------------------------------------------------------------
static size_t STR_prints(char* pOut, char* pString, size_t Width, uint8_t Option)
{
    size_t  Counter;
    size_t  Len;
    char    PadChar;

    Counter = 0;
    PadChar = ' ';

    if(Width > 0)                                           // If different than '0' -> there is padding
    {
        Len = strnlen(pString, 255);

        // Check if length of the string is bigger or equal than the width of padding
        if(Len >= Width)               Width   = 0;         // No padding necessary, the number is bigger than the padding space
        else                           Width  -= Len;       // Remove unnecessary padding because of the length of the number
        if(Option & STR_OPT_PAD_ZERO)  PadChar = '0';       // If padding is zero, than set the variable

        if(Option & STR_OPT_PAD_LEFT)
        {
            for(; Width > 0; Width--)
            {
                STR_putchar(&pOut[Counter], PadChar);
                Counter++;
            }
        }
    }

    Counter += STR_str2str(&pOut[Counter], pString);        // Print character until EOL

    for(; Width > 0; Width--)                               // Print padding character if any
    {
        STR_putchar(&pOut[Counter], PadChar);
        Counter++;
    }

    return Counter;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: STR_printi
//
//   Parameter(s):  char*       pOut            Output string pointer
//                  int32_t     Value           Value to print
//                  size_t      Width           Width in the print string
//                  uint8_t     Option          Padding, Space or 0, Upper or lowercase
//   Return Value:  size_t                      Number of character printed
//
//   Description:   Service function to print integer for printf and sprintf
//
//-------------------------------------------------------------------------------------------------
static size_t STR_printi(char* pOut, int64_t Value, size_t Width, uint8_t Option)
{
    size_t      Counter;
    uint64_t    _Value;
    char        PrintBuffer[STR_MAX_PRINT_VALUE];
    char*       pString;
    bool        Negative;
    char        Temp;
  #ifdef STR_USE_HEX_SUPPORT
    uint8_t     LetterBase;
    uint8_t     Base;
  #endif

    Counter    = 0;
    Negative   = false;
    _Value     = Value;

  #ifdef STR_USE_HEX_SUPPORT
    LetterBase = 'A' + (Option & STR_OPT_LOWERCASE);
    Base       = (Option & STR_OPT_BASE_HEXA) ? 16 : 10;
  #endif

    if(Value == 0)
    {
        PrintBuffer[0] = '0';
        PrintBuffer[1] = '\0';
        return STR_prints(pOut, PrintBuffer, Width, Option);
    }

  #ifdef STR_USE_HEX_SUPPORT
    if((Option & STR_OPT_SIGN_NEGATIVE) && (Base == 10) && (Value < 0))
  #else
    if((Option & STR_OPT_SIGN_NEGATIVE) && (Value < 0))
  #endif
    {
        Negative = true;
        _Value   = -Value;
    }

    pString = PrintBuffer + (STR_MAX_PRINT_VALUE - 1);
    *pString = '\0';

    while(_Value)
    {
      #ifdef STR_USE_HEX_SUPPORT

        Temp = _Value % Base;

        if(Temp >= 10)
        {
            Temp += (LetterBase -'0' - 10);
        }

        *--pString = Temp + '0';
        _Value = _Value / Base;

      #else

        Temp = _Value % 10;
        *--pString = Temp + '0';
        _Value = _Value / 10;

      #endif
    }

    if(Negative == true)
    {
        if((Width != 0) && (Option & STR_OPT_PAD_ZERO))
        {
            STR_putchar(pOut, '-');
            Counter++;
            Width--;
        }
        else
        {
            *--pString = '-';
        }
    }

    return Counter + STR_prints(pOut, pString, Width, Option);
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: STR_str2str
//
//   Parameter(s):  char*       pOut        Output string pointer
//                  const char* pFormat     Formatted string
//   Return Value:  size_t      Size
//
//   Description:   Transfer the string to the other string until EOL
//
//-------------------------------------------------------------------------------------------------

static size_t STR_str2str(char* pOut, const char* pString)
{
    size_t Counter = 0;

    for(; *pString; pString++)                                  // Print caracter until EOL
    {
        STR_putchar(&pOut[Counter], (wchar_t)*pString);
        Counter++;
    }
    STR_putchar(&pOut[Counter], '\0');

    return Counter;
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: STR_vsnprintf
//
//   Parameter(s):  char*           pOut        Output string pointer
//                  size_t          Size        Maximum character to print
//                  const char*     pFormat     Formatted string
//                  va_list         va          list of argument
//   Return Value:  size_t                      Number of character printed
//
//   Description:   Print a formatted text in a buffer
//
//                  will support the following Parameter
//
//                  %%               Print symbol %
//                  %s               Print a string
//                  %0               pad with zero
//                  %1 to %255       number of padding space
//                  %01 to %0255     number of padding 0
//                  %c               special character
//                  %l               for long support               ( valid only for d,u,x,X )
//                  %ll              for longlong (64 Bits) support ( valid only for d,u,x,X ) Ex. %llu
//                  %u               unsigned integer
//                  %d               integer
//                  %X               hexadecimal in upper case
//
//
//                   note: this is the order in which they must be use
//
//                   ex.  %05lX   pad with ZERO +
//                                5 padding character +
//                                it will be a long +
//                                it's a HEX printed in uppercase
//
//                   ex. %6ld     normal padding ( no Parameter ) +
//                                padding with space ( no Parameter ) +
//                                6  padding space on the left +
//                                it will be a long +
//                                it is a decimal value
//
//-------------------------------------------------------------------------------------------------
size_t STR_vsnprintf(char* pOut, size_t Size, const char* pFormat, va_list va)
{

    uint32_t    Counter;
    size_t      Width;
    VarLength_e SizeVar;
    int64_t     n;
    uint8_t     Option;
    char*       s;
    char        scr[2];

    Counter = 0;

    for(; (*pFormat != '\0') && (Counter < Size); pFormat++)
    {
        if(*pFormat == '%')
        {
            pFormat++;
            Width   = 0;
            Option  = STR_OPT_PAD_LEFT;
            SizeVar = STR_VAR_16;

            if(*pFormat == '%')
            {
                STR_putchar(&pOut[Counter], *(char*)pFormat);
                Counter++;
            }

            if(*pFormat == 's')
            {
                s = va_arg(va, char *);
                Counter += STR_prints(&pOut[Counter], s ? s : (char*)"(null)", Width, Option);
                continue;
            }

            if(*pFormat == 'c')
            {
                scr[0] = (uint8_t)va_arg(va, int);
                scr[1] = '\0';
                Counter += STR_prints(&pOut[Counter], scr, Width, Option);
                continue;
            }

            while(*pFormat == '0')
            {
                pFormat++;
                Option |= STR_OPT_PAD_ZERO;
            }

            for(; (*pFormat >= '0') && (*pFormat <= '9'); pFormat++)
            {
                Width *= 10;
                Width += *pFormat - '0';
            }

            if(*pFormat == 'l')
            {
                SizeVar = STR_VAR_32;
                pFormat++;
                if(*pFormat == 'l')
                {
                    SizeVar = STR_VAR_64;
                    pFormat++;
                }
            }

            if(*pFormat == 'd')
            {
                switch(SizeVar)
                {
                    case STR_VAR_16: n = (int16_t)va_arg(va, int32_t);    break;
                    case STR_VAR_32: n = (int32_t)va_arg(va, int32_t);    break;
                    case STR_VAR_64: n = (int64_t)va_arg(va, int64_t);    break;
                }
                Counter += STR_printi(&pOut[Counter], n, Width, Option | STR_OPT_SIGN_NEGATIVE | STR_OPT_LOWERCASE);
                continue;
            }

            switch(SizeVar)
            {
                case STR_VAR_16: n = (uint16_t)va_arg(va, uint32_t);    break;
                case STR_VAR_32: n = (uint32_t)va_arg(va, uint32_t);    break;
                case STR_VAR_64: n = (uint64_t)va_arg(va, uint64_t);    break;
            }

          #ifdef STR_USE_HEX_SUPPORT
            if(*pFormat == 'x')
            {
                Counter += STR_printi(&pOut[Counter], n, Width, Option | STR_OPT_BASE_HEXA | STR_OPT_LOWERCASE);
                continue;
            }

            if(*pFormat == 'X')
            {
                Counter += STR_printi(&pOut[Counter], n, Width, Option | STR_OPT_BASE_HEXA);
                continue;
            }
          #endif

            if(*pFormat == 'u')
            {
                Counter += STR_printi(&pOut[Counter], n, Width, Option | STR_OPT_LOWERCASE);
                continue;
            }
        }
        else
        {
            STR_putchar(&pOut[Counter], *pFormat);
            Counter++;
        }
    }

    STR_putchar(&pOut[Counter], '\0');

    return Counter;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: STR_vsnformat
//
//   Parameter(s):  char*       pOut        Pointer on output string
//                  size_t      Size        Maximum character to print
//                  const char* pFormat     Formatted string
//                  va_list     va          list of argument
//   Return Value:  size_t      Size
//
//   Description:   Special printf that allow argument to be located anywhere in string according
//                  to the position override
//
//-------------------------------------------------------------------------------------------------
size_t STR_vsnformat(char* pOut, size_t Size, const char* pFormat, va_list va)
{
    STR_Format_t* pFmt;
    size_t        PointerCounter = 0;

    pFmt = (STR_Format_t*)pMemory->Alloc(sizeof(STR_Format_t));
    pFmt->pFormat = (char*)pMemory->Alloc(DIGINI_MAX_PRINT_SIZE);                                  // Get memory to work this printf

    strncpy(pFmt->pFormat, pFormat, DIGINI_MAX_PRINT_SIZE);                                        // Copy from possible const location to RAM
    pFmt->pFmtPtr = pFmt->pFormat;
    pFmt->Counter = 0;

    memset(&pFmt->Position[0], 0xFF, STR_NUMBER_OF_VA_LIST_ITEMS);

    // This first loop is to capture each position
    for(; *pFmt->pFmtPtr != 0; pFmt->pFmtPtr++)
    {
        if(*pFmt->pFmtPtr == '$')                                                                   // Process each position override
        {
            pFmt->pFmtPtr++;
            pFmt->pDestination = pFmt->pFmtPtr;

            if(*pFmt->pFmtPtr != '$')
            {
                *(pFmt->pFmtPtr - 1) = (uint8_t)ASCII_SUBSTITUTION;

                if((*pFmt->pFmtPtr >= '0') && (*pFmt->pFmtPtr <= '9'))                              // Get position from 0 to 9
                {
                    pFmt->Position[pFmt->Counter++] = (uint8_t)(*pFmt->pFmtPtr - '0');
                    pFmt->pFmtPtr++;
                }
                else if((*pFmt->pFmtPtr >= 'A') && (*pFmt->pFmtPtr <= 'F'))                         // Get position from A to F
                {
                    pFmt->Position[pFmt->Counter++] = (uint8_t)(*pFmt->pFmtPtr - ('A' - 10));
                    pFmt->pFmtPtr++;
                }
                else
                {
                    pFmt->pDestination--;
                }
                strncpy(pFmt->pDestination, pFmt->pFmtPtr, DIGINI_MAX_PRINT_SIZE);                 // Strip '$' and position override from string
                pFmt->pFmtPtr = pFmt->pDestination - 1;
            }
            else
            {
                pFmt->pFmtPtr++;
                strncpy(pFmt->pDestination, pFmt->pFmtPtr, DIGINI_MAX_PRINT_SIZE);                 // Strip '$' and position override from string
                pFmt->pFmtPtr = pFmt->pDestination;
            }
        }
    }

    pFmt->pFmtPtr    = pFmt->pFormat;
    pFmt->pFormatPtr = (char*)pFormat;
    pFmt->Counter  = 0;

    memset(&pFmt->pSwitchArg[0], 0x00, STR_NUMBER_OF_VA_LIST_ITEMS);


    // This second loop will strip any formatting info from the copy string and get pointer on each formatting info in original string
    for(; *pFmt->pFmtPtr != '\0'; pFmt->pFmtPtr++, pFmt->pFormatPtr++)
    {
        if(*pFmt->pFmtPtr == '%')
        {
            pFmt->pDestination = pFmt->pFmtPtr;
            pFmt->pFmtPtr++;
            pFmt->pFormatPtr++;

            if(*pFmt->pFmtPtr == '%')
            {
                pFmt->pDestination++;
                pFmt->pFmtPtr++;
                pFmt->pFormatPtr++;
            }
            else
            {
                for(pFmt->i = 0; pFmt->i < STR_NUMBER_OF_VA_LIST_ITEMS; pFmt->i++)
                {
                    if(pFmt->Position[pFmt->i] == pFmt->Counter)
                    {
                        pFmt->pSwitchArg[pFmt->i] = pFmt->pFormatPtr;
                        pFmt->pListArg[pFmt->i]   = va;
                        break;
                    }
                }
                pFmt->Counter++;

                if(*pFmt->pFmtPtr == 's')
                {
                    va_arg(va, char*);
                    pFmt->pFmtPtr++;
                    pFmt->pFormatPtr++;
                }
                else if(*pFmt->pFmtPtr == 'c')
                {
                    va_arg(va, int);//va_arg(va, uint8_t);
                    pFmt->pFmtPtr++;
                    pFmt->pFormatPtr++;
                }
                else
                {
                    while(*pFmt->pFmtPtr == '0')
                    {
                        pFmt->pFmtPtr++;
                        pFmt->pFormatPtr++;
                    }

                    if((*pFmt->pFmtPtr >= '0') && (*pFmt->pFmtPtr <= '9'))
                    {
                        pFmt->pFmtPtr++;
                        pFmt->pFormatPtr++;
                    }

                    if(*pFmt->pFmtPtr == 'l')
                    {
                        pFmt->SizeVar = STR_VAR_32;
                        pFmt->pFmtPtr++;
                        pFmt->pFormatPtr++;
                    }
                    else
                    {
                        pFmt->SizeVar  = STR_VAR_16;
                    }

                    if(*pFmt->pFmtPtr == 'd')
                    {
                        (pFmt->SizeVar == STR_VAR_16) ? va_arg(va, int/*int16_t*/) : va_arg(va, long);
                        pFmt->pFmtPtr++;
                        pFmt->pFormatPtr++;
                    }
                    else if((*pFmt->pFmtPtr == 'X') || (*pFmt->pFmtPtr == 'u'))
                    {
                        (pFmt->SizeVar == STR_VAR_16) ? va_arg(va, int/*uint16_t*/) : va_arg(va, uint32_t);
                        pFmt->pFmtPtr++;
                        pFmt->pFormatPtr++;
                    }
                }
            }

            strcpy(pFmt->pDestination, pFmt->pFmtPtr);                                    // Strip any formatting information
            pFmt->pFmtPtr = pFmt->pDestination - 1;
            pFmt->pFormatPtr--;
        }
    }

    pFmt->pFormatPtr = (char*)pFormat;
    pFmt->Counter  = 0;

    // Do the actual formatting according to correct argument placement
    for(; (*pFmt->pFmtPtr != '\0') && (PointerCounter < Size); pFmt->pFmtPtr++)
    {
        if(*pFmt->pFmtPtr == (uint8_t)ASCII_SUBSTITUTION)
        {
            pFmt->Width    = 0;
            pFmt->Option   = STR_OPT_PAD_LEFT;
            pFmt->SizeVar  = STR_VAR_16;
            pFmt->pFormatPtr = pFmt->pSwitchArg[pFmt->Counter];

            if(pFmt->pFormatPtr != nullptr)
            {
                if(*pFmt->pFormatPtr == 's')
                {
                    pFmt->s = va_arg(pFmt->pListArg[pFmt->Counter], char*);
                    pFmt->Counter++;
                    PointerCounter += STR_prints(&pOut[PointerCounter], pFmt->s ? pFmt->s : (char*)StrNULL, pFmt->Width, pFmt->Option);
                    pFmt->pFormatPtr++;
                    continue;
                }

                if(*pFmt->pFormatPtr == 'c')
                {
                    pFmt->scr[0] = va_arg(pFmt->pListArg[pFmt->Counter], int/*char*/);
                    pFmt->scr[1] = '\0';
                    pFmt->Counter++;
                    PointerCounter += STR_prints(&pOut[PointerCounter], pFmt->scr, pFmt->Width, pFmt->Option);
                    pFmt->pFormatPtr++;
                    continue;
                }

                while(*pFmt->pFormatPtr == '0')
                {
                    pFmt->Option |= STR_OPT_PAD_ZERO;
                    pFmt->pFormatPtr++;
                }

                for(; *pFmt->pFormatPtr >= '0' && *pFmt->pFormatPtr <= '9'; pFmt->pFormatPtr++)
                {
                    pFmt->Width *= 10;
                    pFmt->Width += *pFmt->pFormatPtr - '0';
                }

                if(*pFmt->pFormatPtr == 'l')
                {
                    pFmt->SizeVar = STR_VAR_32;
                    pFmt->pFormatPtr++;
                }

                if(*pFmt->pFormatPtr == 'd')
                {
                    if(pFmt->SizeVar == STR_VAR_16)   pFmt->n = va_arg(pFmt->pListArg[pFmt->Counter], int/*int16_t*/);
                    else                              pFmt->n = va_arg(pFmt->pListArg[pFmt->Counter], long);

                    pFmt->Counter++;
                    PointerCounter += STR_printi(&pOut[PointerCounter], pFmt->n, pFmt->Width, pFmt->Option | STR_OPT_SIGN_NEGATIVE | STR_OPT_LOWERCASE);
                    pFmt->pFormatPtr++;
                    continue;
                }

                if(*pFmt->pFormatPtr == 'X')
                {
                    if(pFmt->SizeVar == STR_VAR_16)  pFmt->n = va_arg(pFmt->pListArg[pFmt->Counter], int/*uint16_t*/);
                    else                             pFmt->n = va_arg(pFmt->pListArg[pFmt->Counter], uint32_t);

                    pFmt->Counter++;
                    PointerCounter += STR_printi(&pOut[PointerCounter], pFmt->n, pFmt->Width, pFmt->Option | STR_OPT_BASE_HEXA);
                    pFmt->pFormatPtr++;
                    continue;
                }

                if(*pFmt->pFormatPtr == 'u')
                {
                    if(pFmt->SizeVar == STR_VAR_16)   pFmt->n = va_arg(pFmt->pListArg[pFmt->Counter], int/*uint16_t*/);
                    else                              pFmt->n = va_arg(pFmt->pListArg[pFmt->Counter], uint32_t);

                    pFmt->Counter++;
                    PointerCounter += STR_printi(&pOut[PointerCounter], pFmt->n, pFmt->Width, pFmt->Option | STR_OPT_LOWERCASE);
                    pFmt->pFormatPtr++;
                    continue;
                }
            }
        }
        else
        {
            STR_putchar(&pOut[PointerCounter++], *pFmt->pFmtPtr);
        }
    }

    if(pOut)
    {
        pOut[PointerCounter] = '\0';
    }

    pMemory->Free((void**)&pFmt->pFormat);
    pMemory->Free((void**)&pFmt);

    return PointerCounter;
}

//-------------------------------------------------------------------------------------------------

#endif
