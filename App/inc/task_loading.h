//-------------------------------------------------------------------------------------------------
//
//  File : task_loading.h
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

#include "nOS.h"

//-------------------------------------------------------------------------------------------------
// Global Macro
//-------------------------------------------------------------------------------------------------

#ifdef  TASK_LOADING_GLOBAL
    #define TASK_LOADING_EXTERN
#else
    #define TASK_LOADING_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define TASK_LOADING_STACK_SIZE              1024
#define TASK_LOADING_PRIO                    4

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class ClassTaskLoading
{
  public:

    void            Run                (void);
    nOS_Error       Initialize         (void);

  private:

    nOS_Thread      m_Handle;
    nOS_Stack       m_Stack[TASK_LOADING_STACK_SIZE];
    //nOS_Flag        m_Flag;
   // const char*     m_pDrive;
   // FATFS           m_FS;
   // char            m_Path[15];

};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

TASK_LOADING_EXTERN class   ClassTaskLoading   TaskLoading;

#ifdef TASK_LOADING_GLOBAL
                    class   ClassTaskLoading*  pTaskLoading = &TaskLoading;
#else
    extern          class   ClassTaskLoading*  pTaskLoading;
#endif

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

extern "C" void TaskLoading_Wrapper(void* pvParameters);

//-------------------------------------------------------------------------------------------------

