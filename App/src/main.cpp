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

#include "lib_digini.h"
#include "bsp.h"
#include "Task_grbl.h"
#include "Task_loading.h"
#include "Task_network.h"
#include "Task_comm.h"

IP_Manager myIP_Manager;

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
    Language_e Language;

    MPU_Initialize(MPU_PRIVILEGED_DEFAULT);

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

// MAC address configuration using GUID of the CPU.
//#define MAC_ADDR0	                            (((char *)0x1FF0F420)[0])
//#define MAC_ADDR1	                            (((char *)0x1FF0F420)[2])
//#define MAC_ADDR2	                            (((char *)0x1FF0F420)[4])
//#define MAC_ADDR3	                            (((char *)0x1FF0F420)[6])
//#define MAC_ADDR4	                            (((char *)0x1FF0F420)[8])
//#define MAC_ADDR5	                            (((char *)0x1FF0F420)[10])
    // to debug
    IP_MAC_Address_t MAC = {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5};
    myIP_Manager.Initialize(ETH_IF_GRBL, &MAC);
    
    pTaskNetwork->Initialize();
  #endif
   // pTaskGRBL->Initialize();


// move this to labael ????? myLabel should read language by itself
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
        pTaskCOMM->Process(); // should move this to own task! so option to run as a task or this as a process
      #if (DIGINI_USE_ETHERNET == DEF_ENABLED)
        //pTaskNetwork->Process();
      #endif

        //nOS_Yield();
    }

    return 0;                               // will never return
}

//-------------------------------------------------------------------------------------------------
