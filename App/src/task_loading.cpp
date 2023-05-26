//-------------------------------------------------------------------------------------------------
//
//  File : task_loading.cpp
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
//  TaskLoading will take care of loading the CNC file  ....maybe
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_digini.h"
#define TASK_LOADING_GLOBAL
#include "Task_loading.h"
#undef TASK_LOADING_GLOBAL

//-------------------------------------------------------------------------------------------------
//
//   Static Variables
//
//-------------------------------------------------------------------------------------------------

nOS_Thread ClassTaskLoading::m_Handle;
nOS_Stack  ClassTaskLoading::m_Stack[TASK_LOADING_STACK_SIZE];

//-------------------------------------------------------------------------------------------------
//
//  Name:           TaskLoading_Wrapper
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//  Description:    main() for the taskLoading
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
extern "C" void TaskLoading_Wrapper(void* pvParameters)
{
    (static_cast<ClassTaskLoading*>(pvParameters))->Run();
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   void
//  Return:         void
//
//  Description:    Initialize
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
nOS_Error ClassTaskLoading::Initialize(void)
{
    nOS_Error Error;

//    snprintf(m_Path, 15, "%s%s", MY_DRIVE, MY_PLAYLIST);
    //m_pDrive = MY_DRIVE;

    Error = nOS_ThreadCreate(&m_Handle,
                             TaskLoading_Wrapper,
                             this,
                             &m_Stack[0],
                             TASK_LOADING_STACK_SIZE,
                             TASK_LOADING_PRIO);

    //Error = nOS_FlagCreate(&this->m_Flag, 0);

    return Error;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           run
//
//  Parameter(s):   void
//  Return:         void
//
//  Description:    main() for the task1
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void ClassTaskLoading::Run(void)
{
    //FRESULT  Result;
    //char*    pSavePointer;
    //char*    pToken;
    //uint8_t* pMemory;

    //char Buffer[160];

    while(SKIN_pTask->IsSkinLoaded() == false)
    {
        nOS_Sleep(100);
    };

    // We have wait for skin loading to complete... now we can use what is left from the free extended memory
    // DB_Central.Get(&pMemory, GFX_FREE_RAM_POINTER, 0,0);
    // DB_Central.SetDB_Address((void**)&pMemory, DBASE_INDEX_RAM_RANGE);
    // DB_Central.Set(&pMemory, GFX_FREE_RAM_POINTER, 0,0);
    nOS_Sleep(100);

    //do
    //{
        //Result = f_mount(&m_FS, m_pDrive, 1);
        //nOS_Sleep(10);
    //}
    //while(Result != FR_OK);

    for(;;)
    {
        nOS_Sleep(10);
    }
}

//-------------------------------------------------------------------------------------------------
