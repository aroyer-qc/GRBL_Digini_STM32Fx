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

struct testChain
{
    uint8_t Poutine;
    uint32_t Poutine2;
    uint16_t Poutine3;
    uint8_t Poutine4;
};

ChainList myTestChainList(sizeof(testChain));

int main()
{
    Language_e Language;

    void* pDummy;

    myTestChainList.AddNode(1, &pDummy);
    myTestChainList.AddNode(2, &pDummy);
    myTestChainList.AddNode(3, &pDummy);
    myTestChainList.AddNode(4, &pDummy);
    myTestChainList.AddNode(5, &pDummy);
    myTestChainList.AddNode(6, &pDummy);
    myTestChainList.AddNode(7, &pDummy);
    myTestChainList.AddNode(8, &pDummy);
    myTestChainList.AddNode(9, &pDummy);

    static uint16_t number = myTestChainList.GetNumberOfNode();

    myTestChainList.GetNodeDataPointer(5, &pDummy);
    myTestChainList.GetNodeDataPointer(1, &pDummy);
    myTestChainList.GetNodeDataPointer(4, &pDummy);
    myTestChainList.GetNodeDataPointer(2, &pDummy);
    myTestChainList.GetNodeDataPointer(3, &pDummy);
    myTestChainList.GetNodeDataPointer(6, &pDummy);
    myTestChainList.GetNodeDataPointer(7, &pDummy);
    myTestChainList.GetNodeDataPointer(9, &pDummy);
    myTestChainList.GetNodeDataPointer(8, &pDummy);



    myTestChainList.RemoveNode(5);
    myTestChainList.RemoveNode(7);
    myTestChainList.RemoveNode(6);
    myTestChainList.RemoveNode(3);
    myTestChainList.RemoveNode(4);
    myTestChainList.RemoveNode(8);
    myTestChainList.RemoveNode(9);
    myTestChainList.RemoveNode(1);
    myTestChainList.RemoveNode(2);

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

    DB_Central.Get(&Language, SYSTEM_LANGUAGE, 0, 0);
    // = LANG_FRENCH;
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
