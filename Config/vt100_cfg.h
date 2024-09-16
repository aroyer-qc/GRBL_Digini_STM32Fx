//-------------------------------------------------------------------------------------------------
//
//  File :  vt100_cfg.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2021 Alain Royer.
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
// Define(s)
//-------------------------------------------------------------------------------------------------

#define VT100_STARTUP_MENU_CFG                  MenuMain                // User can choose another menu startup entry for VT100.
#define VT100_MENU_MAIN_FALLBACK_CFG            VT100_MENU_NONE         // if you change the VT100_STARTUP_MENU_CFG to other menu, put the name of the menu here. Otherwiswe it will exit the VT100 menu system.
#define VT100_USE_COLOR                         DEF_ENABLED
#define VT100_USER_CALLBACK_INITIALIZE          DEF_DISABLED

// Only one of those define can be used or none of them
#define VT100_USE_STANDARD_MENU_STATIC_INFO     DEF_ENABLED             // VT100 Class provide its own header. then enable this if it is desire.
#define VT100_USE_USER_MENU_STATIC_INFO         DEF_DISABLED            // If user wish to create his own header or special formatting.

// Configuration option in menu system, choose the one you want
#define VT100_USE_LANGUAGE_SELECTION            DEF_ENABLED
#define VT100_USE_TEMPERATURE_SELECTION         DEF_ENABLED
#define VT100_USE_SERIAL_NUMBER_SETTING         DEF_ENABLED
#define VT100_USE_TIME_AND_DATE_SETTING         DEF_ENABLED


#define VT100_USER_LABEL_LANGUAGE_DEF(X_VT100_USER_LBL_CFG) \
    X_VT100_USER_LBL_CFG( VT100_LBL_MENU_TITLE,                       "GRBL Configuration Menu",                                  "Menu de Configuration GRBL"                                 ) \
    X_VT100_USER_LBL_CFG( VT100_LBL_TEST,                             "Test",                                                     "Test"                                                       ) \
    X_VT100_USER_LBL_CFG( VT100_LBL_MISC_STAT,                        "Misc. Statistic",                                          "Statistiques Divers"                                        ) \
    X_VT100_USER_LBL_CFG( VT100_LBL_MISCELLEANEOUS,                   "Miscelleaneous",                                           "Divers"                                                     ) \
    X_VT100_USER_LBL_CFG( VT100_LBL_TEST_1,                           "Test 1",                                                   "Test 1"                                                     ) \
\
    X_VT100_USER_LBL_CFG( VT100_LBL_SYSTEM_SETTING,                   "System Settings",                                          "Configuration Syst\x8ame"                                   ) \
    X_VT100_USER_LBL_CFG( VT100_LBL_MISC_CONFIG_MENU,                 "Miscellaneous Configuration Menu",                         "Menu de Configuration Divers"                               ) \
    X_VT100_USER_LBL_CFG( VT100_LBL_SYSTEM_SETTING_MENU,              "System Settings Menu",                                     "Menu Configuration Syst\x8ame"                              ) \

// Note(s) ther is standard definition in the VT100
// Define the compare name (I did not find a way to get rid of those definition by the use of advanced macro)
#define COMPARE_MenuRedirection(x)      x
#define COMPARE_MenuBoot(x)    		    x


// Define here all the menu and sub menu you will use in the VT100 terminal
#define VT100_USER_MENU_DEF(ENTRY)  \
    ENTRY(MenuRedirection    )  \
    ENTRY(MenuBoot           )  \

// Define here all the callback you will use in the VT100 terminal
#define VT100_USER_CALLBACK(ENTRY)       \
    ENTRY(CALLBACK_MenuRedirection    )  \
    ENTRY(CALLBACK_InputReading       )
//   ENTRY(CALLBACK_TimeDateCfg        )

// Here you define how you will navigate in the menu tree and action associated with them                             1st item of a menu
//                                                     Menu   Member Of          Item ID                              CALLBACK for refresh                     Navigate to this menu on <ESC>    Label ID
//                                                                               on first if CALLBACK_None there is   all other. Navigate to this menu
//                                                                               no refresh. All other items are
//                                                                               items processing services.

#define VT100_USER_MENU_TREE_DEF(ENTRY, MENU) \
\
                                                ENTRY  (MENU,  MenuRedirection,   ID_REDIRECTION,                      CALLBACK_MenuRedirection,                VT100_MENU_NONE,                  INVALID_LABEL                                   )  \
\
                                                ENTRY  (MENU,  MenuBoot,          ID_MAIN_TITLE,                       CALLBACK_None,                           VT100_MENU_NONE,                  VT100_LBL_MAIN_MENU                             )  \
    IF_USE( LABEL_USE_PRODUCT_INFO,             ENTRY  (MENU,  MenuBoot,          ID_INFO_DISPLAY,                     CALLBACK_None,                           MenuInfo,                         VT100_LBL_SYSTEM_INFO                           ) )\
    IF_USE( DIGINI_USE_STACKTISTIC,             ENTRY  (MENU,  MenuBoot,          ID_STACK_DISPLAY,                    CALLBACK_None,                           MenuStackUsage,                   VT100_LBL_STACKTISTIC                           ) )\
    IF_USE( DIGINI_USE_DEBUG_IN_CONSOLE,        ENTRY  (MENU,  MenuBoot,          ID_DEBUG_MENU,                       CALLBACK_None,                           MenuDebug,                        VT100_LBL_DEBUG                                 ) )\
    IF_USE( DIGINI_USE_STATIC_MEMORY_ALLOC,     ENTRY  (MENU,  MenuBoot,          ID_MEMORY_POOL_MENU,                 CALLBACK_None,                           MenuMemoryPool,                   VT100_LBL_MEMORY_POOL_STAT                      ) )\
    IF_USE( DIGINI_USE_ETHERNET,                ENTRY  (MENU,  MenuBoot,          ID_NETWORK_INFO,                     CALLBACK_None,                           MenuNetwork,                      LBL_NETWORK_INFO                                ) )\
    IF_USE( DIGINI_DEBUG_SDCARD_INFO_ON_VT100,  ENTRY  (MENU,  MenuBoot,          ID_SD_CARD_MENU,                     CALLBACK_None,                           MenuSD_Card,                      LBL_SD_CARD_INFORMATION                         ) )\
                                                ENTRY  (MENU,  MenuBoot,          ID_MISC_SETTING,                     CALLBACK_None,                           MenuSystemSetting,                VT100_LBL_SYSTEM_SETTING                        )  \

// ------------------------------------------------------------------------------------------------------------------
