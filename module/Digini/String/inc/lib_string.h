//-------------------------------------------------------------------------------------------------
//
//  File : lib_string.h
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

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "digini_cfg.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define STR_MAX_PRINT_VALUE             80
#define STR_USE_HEX_SUPPORT
#define STR_NUMBER_OF_VA_LIST_ITEMS     16

//-------------------------------------------------------------------------------------------------
// Type definition(s) and structure(s)
//-------------------------------------------------------------------------------------------------

// Struct use exclusively in STR_Format
struct STR_Format_t
{
    char*    pDestination;
    char*    pFormat;
    char*    pFmtPtr;
    char*    pFormatPtr;

    uint8_t  Option;
    uint8_t  Width;
    uint8_t  SizeVar;
    uint8_t  Counter;

    uint8_t  i;
    uint8_t  p;
    int32_t  n;
    char*    s;
    char     scr[2];
    uint8_t  Position[STR_NUMBER_OF_VA_LIST_ITEMS];
    char*    pSwitchArg[STR_NUMBER_OF_VA_LIST_ITEMS];
    va_list  pListArg[STR_NUMBER_OF_VA_LIST_ITEMS];
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifndef  LIB_STR_GLOBAL
//    extern const char        pNULL_Str[7];
#endif

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

size_t      STR_vsnprintf   (char* pOut, size_t Size, const char* pFormat, va_list vaArg);
size_t      STR_vsnformat   (char* pOut, size_t Size, const char* pFormat, va_list vaArg);
char*       STR_strnchr     (char* pStr, size_t Size, char Chr);

//-------------------------------------------------------------------------------------------------
