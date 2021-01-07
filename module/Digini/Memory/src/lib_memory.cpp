//-------------------------------------------------------------------------------------------------
//
//  File : lib_memory.cpp
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
#include <string.h>
#define MEM_GLOBAL
#include "lib_digini.h"
#undef MEM_GLOBAL
#include "nOS.h"
#ifdef MEM_BLOCK_DEF

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CMEM
//
//   Parameter(s):  None
//
//   Description:   Initializes the memory allocation table and private support data
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------

CMem::CMem()
{
    uint8_t i;

    i = 0;
    // Get the address of each group in pointer array
    #define X_MEM_BLOCK(ENUM_ID, GROUP_NAME, ALLOC_NAME, BLOCK_MAX, BLOCK_SIZE) m_pBufferArray[i++] = (void*)&m_##GROUP_NAME[0][0];
      MEM_BLOCK_DEF
    #undef X_MEM_BLOCK

    //Initialize nOS Memory array structure
     i = 0;
    #define X_MEM_BLOCK(ENUM_ID, GROUP_NAME, ALLOC_NAME, BLOCK_MAX, BLOCK_SIZE)   \
     memset(&m_nOS_MemArray[i], 0, sizeof(nOS_Mem));                              \
     nOS_MemCreate(&m_nOS_MemArray[i], m_pBufferArray[i], BLOCK_SIZE, BLOCK_MAX); \
     i++;
      MEM_BLOCK_DEF
    #undef X_MEM_BLOCK
}

//-------------------------------------------------------------------------------------------------
//
//   Destructor:    ~CMEM
//
//   Description:   Free up any resources if any
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CMem::~CMem()
{
  #if (NOS_CONFIG_MEM_DELETE_ENABLE > 0)
    for(i = 0; i < APP_NB_MEM_BLOCK_GROUP; i++)
    {
        nOS_MemDelete(&m_nOS_MemArray[i]);
    }
  #endif
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: Alloc
//
//   Parameter(s):  size_t          SizeRequired,       Size of the needed data block
//                  nOS_TickCounter TimeOut             How it can wait for the data block
//
//                                  NOS_NO_WAIT :                 Don't wait if no blocks available.
//                                  0 > tout < NOS_WAIT_INIFITE : Maximum number of ticks to wait
//                                                                until a block became available.
//                                  NOS_WAIT_INIFINE :            Wait indefinitely until a block
//                                                                became available.
//
//   Return:        void*           Pointer to allocated block of memory.
//                                  nullptr = No Block available
//
//   Description:   Try to take one block from any of the memory array when the size is equal or
//                  more then the requested amount of data. If no block available, calling thread
//                  will be removed from list of SYS_READY to run threads and be placed in list of
//                  waiting threads for number of ticks specified by TimeOut. If a block of memory
//                  is freed before end of timeout, thread will be awoken and pointer to memory
//                  block will be returned.
//
//   Note(s):       This class wrap the nOS Function
//
//-------------------------------------------------------------------------------------------------
void* CMem::Alloc(size_t SizeRequired, nOS_TickCounter TimeOut)
{
   void* MemPtr;

  #if APP_RESTRICT_MEMORY_ALLOC_TO_BLOCK_SIZE == 0
    // First loop will check for any block available, so we don't wait to be freed
    for(uint8_t i = 0; i < MEM_BLOCK_GROUP_SIZE; i++)
    {
        if(m_nOS_MemArray[i].bsize >= SizeRequired)
        {
            MemPtr = nOS_MemAlloc(&m_nOS_MemArray[i], NOS_NO_WAIT);
            if(MemPtr != nullptr)
            {
                return MemPtr;
            }
        }
    }
  #endif

    // If we reach here then we did not succeed to get a block, so we will wait
    for(uint8_t i = 0; i < MEM_BLOCK_GROUP_SIZE; i++)
    {
        if(m_nOS_MemArray[i].bsize >= SizeRequired)
        {
            MemPtr = nOS_MemAlloc(&m_nOS_MemArray[i], TimeOut);
            return MemPtr;
        }
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: AllocAndClear
//
//   Parameter(s):  size_t          SizeRequired,       Size of the needed data block
//                  nOS_TickCounter TimeOut             How it can wait for the data block
//
//                                  NOS_NO_WAIT :                 Don't wait if no blocks available.
//                                  0 > tout < NOS_WAIT_INIFITE : Maximum number of ticks to wait
//                                                                until a block became available.
//                                  NOS_WAIT_INIFINE :            Wait indefinitely until a block
//                                                                became available.
//
//   Return:        void*           Pointer to allocated block of memory.
//                                  nullptr = No Block available
//
//   Description:   Try to take one block from any of the memory array when the size is equal or
//                  more then the requested amount of data. If no block available, calling thread
//                  will be removed from list of SYS_READY to run threads and be placed in list of
//                  waiting threads for number of ticks specified by TimeOut. If a block of memory
//                  is freed before end of timeout, thread will be awoken and pointer to memory
//                  block will be returned. Also it clear it's content
//
//   Note(s):       This class wrap the nOS Function
//
//-------------------------------------------------------------------------------------------------
void* CMem::AllocAndClear(size_t SizeRequired, nOS_TickCounter TimeOut)
{
    return AllocAndSet(SizeRequired, 0, TimeOut);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: AllocAndClear
//
//   Parameter(s):  size_t          SizeRequired,       Size of the needed data block
//                  nOS_TickCounter TimeOut             How it can wait for the data block
//
//                                  NOS_NO_WAIT :                 Don't wait if no blocks available.
//                                  0 > tout < NOS_WAIT_INIFITE : Maximum number of ticks to wait
//                                                                until a block became available.
//                                  NOS_WAIT_INIFINE :            Wait indefinitely until a block
//                                                                became available.
//
//   Return:        void*           Pointer to allocated block of memory.
//                                  nullptr = No Block available
//
//   Description:   Try to take one block from any of the memory array when the size is equal or
//                  more then the requested amount of data. If no block available, calling thread
//                  will be removed from list of SYS_READY to run threads and be placed in list of
//                  waiting threads for number of ticks specified by TimeOut. If a block of memory
//                  is freed before end of timeout, thread will be awoken and pointer to memory
//                  block will be returned. Also it fill the buffer with the fill value
//
//   Note(s):       This class wrap the nOS Function
//
//-------------------------------------------------------------------------------------------------
void* CMem::AllocAndSet(size_t SizeRequired, uint8_t FillValue, nOS_TickCounter TimeOut)
{
    void* MemPtr;

    MemPtr = Alloc(SizeRequired, TimeOut);

    if(MemPtr != nullptr)
    {
        for(size_t i = 0; i < SizeRequired; i++)
        {
            ((uint8_t*)MemPtr)[i] = FillValue;
        }
    }
    else
    {
        __asm("nop");
    }

    return MemPtr;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: Free
//
//   Parameter(s):  void* pBlock    Pointer to previously allocated block.
//   Return:        bool            true  : Memory block has been freed with success.
//                                  false : Some error occurred
//
//   Description:   Free a previously allocated block of memory.
//
//   Note(s):       if exact result code is necessary, you can call GetLastError();
//
//-------------------------------------------------------------------------------------------------
bool CMem::Free(void** pBlock)
{
    uint8_t GroupID;

    for(GroupID = 0; GroupID < MEM_BLOCK_GROUP_SIZE; GroupID++)
    {
        if((*pBlock >= (void*)((uint8_t*)m_nOS_MemArray[GroupID].buffer)) &&
           (*pBlock < ((void*)((uint8_t*)m_nOS_MemArray[GroupID].buffer + (m_nOS_MemArray[GroupID].bsize * m_nOS_MemArray[GroupID].bmax)))))
        {
            m_LastError = nOS_MemFree(&m_nOS_MemArray[GroupID], *pBlock);
            if(m_LastError == NOS_OK)
            {
                return true;
            }
            return false;
        }
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: IsAvailable
//
//   Parameter(s):  size_t          SizeRequired,       Size of the needed data block
//   Return::       bool                                Block availability.
//                                  false : No block of memory is currently available.
//                                  true  : At least one block of memory is available.
//
//   Description:   Check if at least one block of memory is available.
//
//-------------------------------------------------------------------------------------------------
bool CMem::IsAvailable(size_t SizeRequired)
{
    for(uint8_t i = 0; i < MEM_BLOCK_GROUP_SIZE; i++)
    {
        if(m_nOS_MemArray[i].bsize <= SizeRequired)
        {
            if(nOS_MemIsAvailable(&m_nOS_MemArray[i]) == true)
            {
                return true;
            }

          #if APP_RESTRICT_MEMORY_ALLOC_TO_BLOCK_SIZE > 0
            return false;
          #endif
        }
    }
    return false;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: GetLastError
//
//   Parameter(s):  None
//   Return::       nOS_Error
//
//   Description:   Return the last known error
//
//-------------------------------------------------------------------------------------------------
nOS_Error CMem::GetLastError(void)
{
    return m_LastError;
}

//-------------------------------------------------------------------------------------------------

#endif // MEM_BLOCK_DEF
