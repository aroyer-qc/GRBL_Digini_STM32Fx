//-------------------------------------------------------------------------------------------------
//
//  File : main.cpp
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

#include "bsp.h"
#include "Task_grbl.h"
#include "Task_loading.h"
#include "Task_network.h"
#include "Task_comm.h"

//-------------------------------------------------------------------------------------------------
//
// Name:           main
// Parameter(s):   void
// Return:         int
//
// Description:    main() what more can be said
//
// Note(s):        Here we create the task that will start all the other
//
//-------------------------------------------------------------------------------------------------
int main()
{
  #if (DIGINI_USE_STACKTISTIC == DEF_ENABLED)
    STACK_FillIdle();           // Use to initialize the stack for monitoring of the stack usage
  #endif

    // Prevent stepping in every IRQ
    (DBGMCU)->APB1FZ = 0x7E01BFF;
    (DBGMCU)->APB2FZ = 0x70003;

    nOS_Init();
    BSP_Initialize();     // All hardware and system initialization
    pTaskLoading->Initialize();
 //   pTaskNetwork->Initialize();
    pTaskGRBL->Initialize();
    nOS_Start();
    BSP_PostOS_Initialize();
    pTaskCOMM->Run();                     // It is the idle task..
    return 0;
}

//-------------------------------------------------------------------------------------------------
