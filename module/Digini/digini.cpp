//-------------------------------------------------------------------------------------------------
//
//  File : digini.cpp
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

#define DIGINI_GLOBAL
#include "lib_digini.h"
#undef  DIGINI_GLOBAL
#include "bsp.h"
#include "lib_grafx.h"
#include "lib_class_spi.h"
//#include "lib_class_usb.h"
#include "lib_class_sdio.h"

//-------------------------------------------------------------------------------------------------
// local Class
//-------------------------------------------------------------------------------------------------

#if defined(EEPROM_DBASE_DEF)
  class CDataBaseInterface*        pDB_EEPROM;
#endif

#if defined(ROM_DBASE_DEF)
  class CDataBaseInterface*        pDB_ROM;
#endif

#if defined(RAM_DBASE_DEF) || defined(GFX_RAM_DBASE_DEF) || defined(NV_RAM_DBASE_DEF)
  class CDataBaseInterface*        pDB_RAM;
#endif

#if defined(BACKUP_DBASE_DEF)
  class CDataBaseInterface*        pDB_BackupRegister;
#endif

#if defined(HARD_DBASE_DEF)
   class CDataBaseInterface*       pDB_Hardware;               // Time, ADC, etc...
#endif

//-------------------------------------------------------------------------------------------------
//
// Name:           DIGINI_Initialize
// Parameter(s):   void
// Return:         SystemState_e
//
// Description:    Initialize DIGINI Lib
//
// Note(s):        Here we create the task that will start the GRAFX if used
//                 Also initialize other lib
//
//-------------------------------------------------------------------------------------------------
SystemState_e DIGINI_Initialize(void)
{
  #if (USE_RTC_DRIVER == DEF_ENABLED)
    BSP_pRTC = new CRTC(&Mutex.Handle.MutexRTC, RTC_CLOCK_MODE_LSI);            // RTC module object
  #endif // USE_RTC_DRIVER == DEF_ENABLED

    // Register all database driver

  #if defined(EEPROM_DBASE_DEF)
    pDB_EEPROM = new CEEPROM_DataBase();
     DB_Central.RegisterDriver(pDB_EEPROM);
  #endif

  #if defined(ROM_DBASE_DEF)
    pDB_ROM = new CROM_DataBase();                                              // ROM Database
    DB_Central.RegisterDriver(pDB_ROM);
  #endif

  #if defined(RAM_DBASE_DEF) || defined(GFX_RAM_DBASE_DEF) || defined(NV_RAM_DBASE_DEF)
    RAM_DBaseRegionPointer_t DBaseRegion;

   #if defined(RAM_DBASE_DEF)
    DBaseRegion.pRam = reinterpret_cast<uint8_t*>(RAM_DBASE_ADDRESS);
   #endif

   #if defined(GFX_RAM_DBASE_DEF)
    DBaseRegion.pGFX_Ram = reinterpret_cast<uint8_t*>(GFX_RAM_ADDRESS);
   #endif

   #if defined(NV_RAM_DBASE_DEF)
    DBaseRegion.pNV_Ram = NV_RAM_DBASE_ADDRESS;
   #endif

    pDB_RAM = new CRAM_DataBase();                                              // RAM Database
    pDB_RAM->Initialize((void *)&DBaseRegion, sizeof(void*));
    DB_Central.RegisterDriver(pDB_RAM);
  #endif

  #if defined(BACKUP_DBASE_DEF)
    pDB_BackupRegister = new CBKPREG_DataBase(BSP_pRTC);                        // Backup register Database
    if(pDB_BackupRegister->Init(nullptr, 0) != SYS_READY)
    {
        assert_failed((const char*)__FILE__, __LINE__);
    }
    DB_Central.RegisterDriver(pDB_BackupRegister);
  #endif

   #if defined(HARD_DBASE_DEF)
    pDB_Hardware = new CHARD_DataBase();                                        // Hardware Database
    DB_Central.RegisterDriver(pDB_Hardware);
   #endif

    GRAFX_Initialize();

    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
// Name:           DIGINI_PostInitialize
// Parameter(s):   void
// Return:         SystemState_e
//
// Description:    Initialize DIGINI Lib
//
// Note(s):        Here we create the task that will start the GRAFX if used
//                 Also initialize other lib
//
//-------------------------------------------------------------------------------------------------
SystemState_e DIGINI_PostInitialize(void)
{
    //nOS_Error Error;
    //SystemState_e State;

  #if (USE_USB_DRIVER == DEF_ENABLED)
    //USB.Initialize();
  #endif // USE_USB_DRIVER == DEF_ENABLED

  #ifdef DIGINI_USE_FATFS
    FatFS_Initialize();
  #endif

  #ifdef DIGINI_USE_GRAFX
    GRAFX_PostInitialize();
  #endif

    return SYS_READY;
}
