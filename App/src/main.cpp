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
    myStacktistic.Initialize();                             // Initialize the Stack Check process before any task
  #endif

    nOS_Init();
    BSP_Initialize();                                       // All hardware and system initialization
    nOS_Start();
    BSP_PostOS_Initialize();
    pTaskCOMM->Initialize();

    //pTaskLoading->Initialize();

  #if (DIGINI_USE_ETHERNET == DEF_ENABLED)
    pTaskNetwork->Initialize();
  #endif
   // pTaskGRBL->Initialize();

//    DB_Central.Get(&Language, SYSTEM_LANGUAGE, 0, 0);
    Language_e Language = LANG_FRENCH;
    myLabel.SetLanguage(Language);


  #ifdef DEBUG
    DateAndTime_t DateTime;

    DateTime.Date.Day    = 31;
    DateTime.Date.Month  = 7;
    DateTime.Date.Year   = 2023;
    DateTime.Time.Hour   = 15;
    DateTime.Time.Minute = 30;
    DateTime.Time.Second = 1;
    LIB_SetDateAndTime(&DateTime);
  #endif

    for(;;)                                 // It is the idle task..
    {
        pTaskCOMM->Process();
        nOS_Yield();
    }

    return 0;                               // will never return
}

//-------------------------------------------------------------------------------------------------
