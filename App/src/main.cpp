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

#include "./Digini/lib_digini.h"
#include "bsp.h"

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
    ISR_Disable();

    MPU_Initialize(MPU_PRIVILEGED_DEFAULT);

  #if (DIGINI_USE_STACKTISTIC == DEF_ENABLED)
    myStacktistic.Initialize();                 // Initialize the Stack Check process before any task
  #endif

    nOS_Init();
    BSP_Initialize();                           // All hardware and system initialization
    nOS_Start();
    BSP_PostOS_Initialize();                    // All initialization that must be done after the OS is started

    for(;;)                                     // It is the idle task..
    {
      #if (DIGINI_USE_COMM_MODULE == DEF_ENABLED) && (DIGINI_USE_COMM_AS_A_TASK == DEF_ENABLED)
        pTaskCOMM->Process();
      #endif
      #if (DIGINI_USE_ETHERNET == DEF_ENABLED)
        pTaskNetwork->Process();
      #endif
    }

    return 0;                                   // will never return
}

//-------------------------------------------------------------------------------------------------
