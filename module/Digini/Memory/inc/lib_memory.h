//-------------------------------------------------------------------------------------------------
//
//  File : lib_memory.h
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

#include "lib_typedef.h"
#include "app_cfg.h"

#ifdef MEM_BLOCK_DEF

//-------------------------------------------------------------------------------------------------
// MEM_BLOCK list declaration section
//-------------------------------------------------------------------------------------------------

// To found how many block type there is
    enum MEM_BlockList_e
    {
      #define X_MEM_BLOCK(ENUM_ID, GROUP_NAME, ALLOC_NAME, BLOCK_MAX, BLOCK_SIZE) ENUM_ID,
        MEM_BLOCK_DEF
      #undef X_MEM_BLOCK
        MEM_BLOCK_GROUP_SIZE
    };

//-------------------------------------------------------------------------------------------------

class CMem
{
    public:

                    CMem                        ();
                    ~CMem                       ();

        void*       Alloc                       (size_t SizeRequired, nOS_TickCounter TimeOut = NOS_WAIT_INFINITE);
        void*       AllocAndClear               (size_t SizeRequired, nOS_TickCounter TimeOut = NOS_WAIT_INFINITE);
        void*       AllocAndSet                 (size_t SizeRequired, uint8_t FillValue, nOS_TickCounter TimeOut = NOS_WAIT_INFINITE);
        bool        Free                        (void** pBlock);
        bool        IsAvailable                 (size_t SizeRequired);
        nOS_Error   GetLastError                (void);

    private:

        nOS_Mem                     m_nOS_MemArray      [MEM_BLOCK_GROUP_SIZE];                  // handler to give to nOS_Mem... function
        void*                       m_pBufferArray      [MEM_BLOCK_GROUP_SIZE];                  // pointer array of the memory block
        nOS_Error                   m_LastError;

      #define X_MEM_BLOCK(ENUM_ID, GROUP_NAME, ALLOC_NAME, BLOCK_MAX, BLOCK_SIZE) uint8_t m_##GROUP_NAME[BLOCK_MAX][BLOCK_SIZE] __attribute__ ((aligned (4)));
        MEM_BLOCK_DEF
      #undef X_MEM_BLOCK
};


// ----- Memory allocation(s) ------
#ifdef MEM_GLOBAL
class CMem*                      pMemory;
#else
extern class CMem*               pMemory;
#endif

#endif  // MEM_BLOCK_DEF
