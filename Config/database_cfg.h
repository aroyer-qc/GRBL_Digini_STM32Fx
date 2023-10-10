//-------------------------------------------------------------------------------------------------
//
//  File : database_cfg.h
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

#include "lib_label.h"
#include "project_def.h"
#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

// TODO move this to the database lib
#define     DBASE_INTERFACE_POOL                7                                   // Size of the interface pool (must fit next define)

#define     DBASE_INDEX_BKPREG_RANGE            0
#define     DBASE_INDEX_EEPROM_RANGE            1000
#define     DBASE_INDEX_HARD_RANGE              2000
#define     DBASE_INDEX_RAM_RANGE               3000
#define     DBASE_INDEX_ROM_RANGE               4000
#define     DBASE_INDEX_SYSTEM_RANGE            5000
#define     DBASE_INDEX_QSPI_RANGE              6000

// specific to Grafx support

#define     DBASE_MAX_SKIN_IMAGE_QTY            256                                 // this is max the number of image in database
#define     DBASE_MAX_SKIN_FONT_QTY             32                                  // this is max the number of font in database

//-------------------------------------------------------------------------------------------------

//....Items SubQTY              is TYPE Variable [][THIS]
//....Items QTY                 is TYPE Variable [THIS][]
//....Item Size                 Memory space reserve for the record

// There is 20 uint32_t backup register in the STM32F7
// Make sure the count are 20 register or less
// Size is fix at sizeof(uint32_t)
//                  Enum ID                 Items QTY,  Items SubQTY
#define BKPREG_DBASE_DEF(X_BKPREG_DBASE) \
    X_BKPREG_DBASE( SYS_DEBUG_LEVEL,        1,          1             )   \
    X_BKPREG_DBASE( TEST2,                  1,          1             )   \
    X_BKPREG_DBASE( TEST3,                  1,          1             )   \
    X_BKPREG_DBASE( TEST4,                  1,          1             )   \
    X_BKPREG_DBASE( TEST5,                  4,          4             )   \

// Do not change position of SYSTEM_LANGUAGE and GRBL_SETTINGS_VERSION.
//                  Enum ID                     Driver,              Items QTY,        Items SubQTY     Item Size
#define EEPROM_DBASE_DEF(X_EEPROM_DBASE) \
    X_EEPROM_DBASE( SYSTEM_LANGUAGE,            myE2_Setting,        1,                1,               sizeof(Language_e)         ) \
    X_EEPROM_DBASE( SYSTEM_SETTINGS_VERSION,    myE2_Setting,        1,                1,               sizeof(uint8_t)            ) \
    X_EEPROM_DBASE( SYSTEM_OEM_MODEL_NAME,      myE2_Setting,        1,                1,               sizeof(OEM_MODEL_NAME)     ) \
    X_EEPROM_DBASE( SYSTEM_SERIAL_NUMBER,       myE2_Setting,        1,                1,               sizeof(OEM_SERIAL_NUMBER)  ) \
    X_EEPROM_DBASE( GRBL_CONFIGURATION,         myE2_Setting,        1,                1,               sizeof(Config_t)           ) \
    X_EEPROM_DBASE( GRBL_BUILD_INFO,            myE2_Setting,        1,                1,               STARTUP_LINE_LEN           ) \
    X_EEPROM_DBASE( GRBL_BUILD_INFO_CHKSUM,     myE2_Setting,        1,                1,               sizeof(uint8_t)            ) \
    X_EEPROM_DBASE( GRBL_GLOBAL_SETTINGS,       myE2_Setting,        1,                1,               sizeof(Settings_t)         ) \
    X_EEPROM_DBASE( GRBL_TOOL_TABLE,            myE2_Setting,        MAX_TOOL_NR,      1,               sizeof(ToolParams_t)       ) \
    X_EEPROM_DBASE( GRBL_STARTUP_BLOCK,         myE2_Setting,        N_STARTUP_LINE,   1,               STARTUP_LINE_LEN           ) \
    X_EEPROM_DBASE( GRBL_STARTUP_BLOCK_CHKSUM,  myE2_Setting,        N_STARTUP_LINE,   1,               sizeof(uint8_t)            ) \


// Configurable value
extern char OEM_CONFIG_VERSION[24];




    // don't seem to be used     X_EEPROM_DBASE( GRBL_PARAMETERS,        myE2_Setting,        1,                1,               sizeof(ToolTable_t)        )

//                Enum ID                 Items QTY,       Items SubQTY       Item Size               Get/Set Callback
#define HARD_DBASE_DEF(X_HARD_DBASE) \
    X_HARD_DBASE( REC_TIME,               1,               1,                 sizeof(Time_t),          AccessTime)         \
    X_HARD_DBASE( REC_DATE,               1,               1,                 sizeof(Date_t),          AccessDate)         \
    X_HARD_DBASE( REC_TIME_FORMAT,        1,               1,                 sizeof(TimeFormat_e),    AccessTimeFormat)   \
    X_HARD_DBASE( APPLICATION_LABEL,      NB_LABEL_CONST,  NB_LANGUAGE_CONST, sizeof(char*),           GetLabel)           \

/*
#define NV_RAM_DBASE_ADDRESS   You must give address of your database of the NV_RAM Region here (Ex. &ByteArrayNV_Ram[x]

//                  Enum ID                             Items QTY,                  Items SubQTY    Item Size
#define NV_RAM_DBASE_DEF(X_NV_RAM_DBASE) \
    X_NV_RAM_DBASE( NV_RAM_TEST1,                       1,                          1,              sizeof(uint8_t)             )   \
    X_NV_RAM_DBASE( NV_RAM_TEST2,                       1,                          1,              sizeof(uint8_t)             )   \
*/


// FOR static location RAM DBASE You must define RAM_DBASE_ADDRESS and give address of your database of the RAM Region here (Ex. &ByteArray_Ram[x]
// FOR dynamic location RAM_DBASE_ADDRESS is declare NULL, you must then call set address to setup your dynamic RAM DBASE location
extern const uint32_t __user_ram_data_base__;
#define RAM_DBASE_ADDRESS   (uint32_t)&__user_ram_data_base__

//               Enum ID                             Items QTY,  Items SubQTY,   Item Size
#define RAM_DBASE_DEF(X_RAM_DBASE) \
    X_RAM_DBASE( ABSOLUTE_RELATIVE,                  1,          1,              1                )   \
    X_RAM_DBASE( RAW_DATA,                           1,          1,              256 * 1024       )   \


/*
//                Enum ID                             Items QTY,                  Items SubQTY    Item Size
#define QSPI_DBASE_DEF(X_QSPI_DBASE) \
    X_QSPI_DBASE( EXAMPLE_QSPI_ENTRY_ID,              8,                          1,              sizeof(QSPI_Example_t)      )   \

*/


//               Enum ID                Item Address                   Items QTY,        Items SubQTY       Item Size
#define ROM_DBASE_DEF(X_ROM_DBASE) \
    X_ROM_DBASE( FIRMWARE_NAME_TEXT,    &OUR_FIRMWARE_NAME[0],         1,                1,                 32/*sizeof(OUR_FIRMWARE_NAME)       */ )   \
    X_ROM_DBASE( FIRMWARE_VERSION_TEXT, &OUR_FIRMWARE_VERSION[0],      1,                1,                 32/*sizeof(OUR_FIRMWARE_VERSION)    */ )   \
    X_ROM_DBASE( FW_GUI_NAME_TEXT,      &OUR_FIRMWARE_GUI_NAME[0],     1,                1,                 32/*sizeof(OUR_FIRMWARE_GUI_NAME)   */ )   \
    X_ROM_DBASE( FW_GUI_VERSION_TEXT,   &OUR_FIRMWARE_GUI_VERSION[0],  1,                1,                 32/*sizeof(OUR_FIRMWARE_GUI_VERSION)*/ )   \
    X_ROM_DBASE( FW_BUILD_DATE_TEXT,    &OUR_BUILD_DATE[0],            1,                1,                 32/*sizeof(OUR_BUILD_DATE)          */ )   \

// System Database are record that are both in RAM ( cpu ram, board ram, nvram, backup register) and in rom (E2, etc..)
// Also there is an interval and a start time to save them in E2 ( save can also be forced )
// Create a volatile record in RAM, NVRAM or Backup RAM
// Create his counter part in EEprom or any other external memory
// Make sure all record match in size and array configuration
// Interval at 0 are never saved, use 'ForceUpdate' to update the value in ROM part
//
//                            Enum ID         RAM             ROM         Interval                First Start
#if 0

#define DBASE_DEF(X_DBASE) \
    X_DBASE( SYSTEM_TEST1,   NV_RAM_TEST1,   E2_TEST1,   CVT_HOUR_TO_SECOND(24), CVT_HOUR_TO_SECOND(1)   )   \
    X_DBASE( SYSTEM_TEST2,   NV_RAM_TEST2,   E2_TEST2,   CVT_HOUR_TO_SECOND(0),  0                       )   \

#endif
