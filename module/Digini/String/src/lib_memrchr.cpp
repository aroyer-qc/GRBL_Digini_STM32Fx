//-------------------------------------------------------------------------------------------------
//
//  File : Lib_memrchr.c
//
//-------------------------------------------------------------------------------------------------
#if 0

//------ Note(s) ----------------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include "lib_string.h"

// ...

//-------------------------------------------------------------------------------------------------
// Private variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

// ...

//-------------------------------------------------------------------------------------------------
// Private function(s)
//-------------------------------------------------------------------------------------------------

// ...

//-------------------------------------------------------------------------------------------------
//
//  Function name:      LIB_memrchr
//
//  Parameter(s):       const void*             Pointer to the Memory
//                      uint16_t                Character to replace
//                      uint16_t                Character replacement
//                      uint8_t                 Count max before exiting
//  Return:             void
//
//  Description:        Locate all occurrence of character in memory and replace by provided character
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void LIB_memrchr(const void* pStr, uint16_t wOldChr, uint16_t wNewChr, uint8_t byCount)
{
    if(Flag.b.UseWORD_StringSize == NO)
    {
        LIB_memrchr_B((const char*)pStr, (uint8_t)wOldChr, (uint8_t)wNewChr, byCount);
    }
    else
    {
        LIB_memrchr_W((const uint16_t*)pStr, wOldChr, wNewChr, byCount);
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Function name:      LIB_memrchr_B
//
//  Parameter(s):       const char*             Pointer to the Memory
//                      char                    Character to replace
//                      char                    Character replacement
//                      uint8_t                    Count max before exiting
//  Return:             void
//
//  Description:        Locate all occurrence of character in memory and replace by provided character
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void LIB_memrchr_B(const char* pStr, uint8_t byOldChr, uint8_t byNewChr, uint8_t byCount)
{
    uint8_t* pStrPtr = (uint8_t*)pStr;

    while(byCount)
    {
        if(*pStrPtr == byOldChr)
        {
            *pStrPtr = byNewChr;
        }

        byCount--;
        pStrPtr++;
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Function name:      LIB_memrchr_W
//
//  Parameter(s):       const uint16_t*             Pointer to the Memory
//                      uint16_t                    Character to replace
//                      uint16_t                    Character replacement
//                      uint8_t                    Count max before exiting
//  Return:             void
//
//  Description:        Locate all occurrence of character in memory and replace by provided character
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void LIB_memrchr_W(const uint16_t* pStr, uint16_t wOldChr, uint16_t wNewChr, uint8_t byCount)
{
    uint16_t* pStrPtr = (uint16_t*)pStr;

    while(byCount)
    {
        if(*pStrPtr == wOldChr)
        {
            *pStrPtr = wNewChr;
        }

        byCount--;
        pStrPtr++;
    }
}

#endif
