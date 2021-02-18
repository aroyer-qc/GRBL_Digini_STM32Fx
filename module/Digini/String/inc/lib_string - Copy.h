//-------------------------------------------------------------------------------------------------
//
//  File : lib_str.h
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

#define LIB_PAD_ZERO                        (uint8_t)1
#define LIB_PAD_LEFT                        (uint8_t)16
#define LIB_MAX_PRINT_VALUE                 (uint8_t)12

#define LIB_VAR_INT                         (uint8_t)1
#define LIB_VAR_LONG                        (uint8_t)2

#define STR_USE_HEX_SUPPORT

//-------------------------------------------------------------------------------------------------
// Macro(s)
//-------------------------------------------------------------------------------------------------

#define _W(x) L ##x


//#define LIB_FILTER_MEDIAN(x)                ((x + 1) / 2)

//-------------------------------------------------------------------------------------------------
// Type definition(s) and structure(s)
//-------------------------------------------------------------------------------------------------

// Struct use exclusively in LIB_Format
typedef struct LIB_Format_s
{
    wchar_t*    pDestination;
    wchar_t*    pFormat;
    wchar_t*    pFmtPtr;
    wchar_t*    pFormatPtr;
    uint8_t     Width;
    uint8_t     Pad;
    uint8_t     SizeVar;
    uint8_t     Counter;
    uint8_t     i;
    uint8_t     p;
    int32_t     n;
    wchar_t*    s;
    wchar_t     scr[2];
    uint8_t     byPosition[16];
    wchar_t*    pSwitchArg[16];
    va_list     pListArg[16];
} LIB_sFormat;

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

//LIB_STR_EXTERN char                g_LIB_AsciiSwapTable[256];

/*
    #ifdef  LIB_STR_GLOBAL
        const wchar_t               pNULL_Str[7]               = _W("(null)");
        const wchar_t               pNULL                      = _W('\0');
    #else
        extern const wchar_t        pNULL_Str[7];
        extern const wchar_t        pNULL;
    #endif
*/

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------
size_t      LIB_strnlen     (wchar_t* pOut, size_t Size);
size_t      LIB_strnlen     (char*    pOut, size_t Size);

template<class T> size_t LIB_snprintf(T* pOut, size_t Size, const T* pFormat, ...)
//size_t      LIB_snprintf    (wchar_t* pOut, size_t Size, const wchar_t* pFormat, ...);
//size_t      LIB_snprintf    (char*    pOut, size_t Size, const char*    pFormat, ...);
size_t      LIB_vsnprintf   (wchar_t* pOut, size_t Size, const wchar_t* pFormat, va_list vaArg);
size_t      LIB_vsnprintf   (char*    pOut, size_t Size, const char*    pFormat, va_list vaArg);
size_t      LIB_vsnformat   (wchar_t* pOut, size_t Size, const wchar_t* pFormat, va_list vaArg);
//size_t      LIB_vsnformat   (char*    pOut, size_t Size, const char*    pFormat, va_list va);
char*       LIB_strncpy     (char*    pDest, const char*    pSrc, size_t Size);
wchar_t*    LIB_strncpy     (wchar_t* pDest, const wchar_t* pSrc, size_t Size);
char*       LIB_strnchr     (char*    pStr, size_t Size, char Chr);
wchar_t*    LIB_strnchr     (wchar_t* pStr, size_t Size, wchar_t Chr);

//-------------------------------------------------------------------------------------------------


