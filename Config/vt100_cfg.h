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

//#pragma once

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define DIGINI_VT100_USE_COLOR                  DEF_ENABLED
#define DIGINI_VT100_IS_A_TASK                  DEF_ENABLED

#define VT100_USER_LABEL_LANGUAGE_DEF(X_VT100_USER_LBL_CFG) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MAIN_MENU,                        "Main Menu",                              "Menu Principal"                              ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_SYSTEM_INFO,                      "Display System Information",             "Information Sysême"                          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG,                            "Debug",                                  "Débogage"                                    ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_SYSTEM_SETTING,                   "System Settings",                        "Configuration Systême"                       ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_TEST,                             "Test",                                   "Test"                                        ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_STACK_USAGE,                      "Stack Usage",                            "Usage du Stack"                              ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_MENU,                       "Debug Menu",                             "Menu de Débogage"                            ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_1,                    "Dbg level 1  - System Status   ( )",     "Dbg level 1  - Status Système  ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_2,                    "Dbg level 2  - System Action   ( )",     "Dbg level 2  - Action Système  ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_3,                    "Dbg level 3  - System Health   ( )",     "Dbg level 3  - Santé Système   ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_4,                    "Dbg level 4  - Monitor/Logs    ( )",     "Dbg level 4  - Moniteur/Logs   ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_5,                    "Dbg level 5  - Free            ( )",     "Dbg level 5  - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_6,                    "Dbg level 6  - Free            ( )",     "Dbg level 6  - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_7,                    "Dbg level 7  - Free            ( )",     "Dbg level 7  - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_8,                    "Dbg level 8  - Free            ( )",     "Dbg level 8  - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_9,                    "Dbg level 9  - Free            ( )",     "Dbg level 9  - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_SYSTEM_SETTING_MENU,              "System Settings Menu",                   "Menu Configuration Système"                  ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MISCELLEANEOUS,                   "Miscelleaneous",                         "Divers"                                      ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_TIME_AND_DATE,                    "Time and Date",                          "Heure et Date"                               ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_TEST_1,                           "Test 1",                                 "Test 1"                                      ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_POUTINE_1,                        "Poutine ski                    ( )",     "Poutine ski                    ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_POUTINE_2,                        "Poutine ski                    ( )",     "Poutine ski                    ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_TIME_AND_DATE_CONFIGURATION,      "Time and Date Configuration",            "Configuration Heure et Date"                 ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_HOUR,                             "Hour\r\n",                               "Heure\r\n"                                   ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MINUTE,                           "Minute\r\n",                             "Minute\r\n"                                  ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_SECOND,                           "Second\r\n",                             "Secondw\r\n"                                 ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DAY,                              "Day\r\n",                                "Jour\r\n"                                    ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MONTH,                            "Month\r\n",                              "Mois\r\n"                                    ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_YEAR,                             "Year\r\n",                               "Année\r\n"                                   ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_BLANK,                            " ",                                      " "                                           ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MISC_CONFIG_MENU,                 "Miscellaneous Configuration Menu",       "Menu de Configuration Divers"                ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_SERIAL_NUMBER_SETTING,            "Setting Serial Number",                  "Configurer Numéro de Série"                  ) \


//#define COMPARE_MAIN(x)    x
//#define COMPARE_FLASHE2(x) x
//#define COMPARE_DEBG(x)    x
//#define COMPARE_TEST(x)    x
//#define COMPARE_LED(x)     x
//#define COMPARE_SETTING(x) x
//#define COMPARE_MISC(x)    x
//#define COMPARE_DATETIM(x) x

// Define here all the menu and sub menu you will use in the VT100 terminal
#define VT100_MENU_DEF(ENTRY)        \
    ENTRY(VT100_MenuRedirection    ) \
    ENTRY(VT100_MenuMain           ) \
    ENTRY(VT100_MenuBoot           ) \
    ENTRY(VT100_MenuDebug          ) \
    ENTRY(VT100_MenuTest           ) \
    ENTRY(VT100_MenuSetting        ) \
    ENTRY(VT100_MenuSettingTime    ) \
    ENTRY(VT100_MenuSetMisc        ) \

// Define here all the callback you will use in the VT100 terminal
#define VT100_CALLBACK(ENTRY)           \
    ENTRY(CALLBACK_MenuRedirection    ) \
    ENTRY(CALLBACK_ProductInformation ) \
    ENTRY(CALLBACK_StackUsage         ) \
    ENTRY(CALLBACK_DebugLevelSetting  ) \
    ENTRY(CALLBACK_TimeDateCfg        ) \
    ENTRY(CALLBACK_MiscCfg            ) \

// Here you define how you will navigate in the menu tree and action associated with them                               1st item of a menu
//              Menu   Member Of                Item ID                        CALLBACK for refresh                     Navigate to this menu on <ESC>          Label ID
//                                                                             on first if NULL there is no refresh     all other. Navigate to this menu
//                                                                             All other items are items processing
//                                                                             services
#define VT100_MENU_TREE_DEF(ENTRY, MENU) \
\
        ENTRY  (MENU,  VT100_MenuRedirection,   VT100_ID_REDIRECTION,          CALLBACK_MenuRedirection,                VT100_MENU_NONE,                        INVALID_LABEL                                   ) \
\
        ENTRY  (MENU,  VT100_MenuMain,          VT100_ID_MAIN_TITLE,           nullptr,                                 VT100_MenuRedirection,                  LBL_VT100_MAIN_MENU                             ) \
        ENTRY  (MENU,  VT100_MenuMain,          VT100_ID_INFO_DISPLAY,         CALLBACK_ProductInformation,             VT100_MenuMain,                         LBL_VT100_SYSTEM_INFO                           ) \
        ENTRY  (MENU,  VT100_MenuMain,          VT100_ID_DEBUG_MENU,           nullptr,                                 VT100_MenuDebug,                        LBL_VT100_DEBUG                                 ) \
        ENTRY  (MENU,  VT100_MenuMain,          VT100_ID_SETTING_MENU,         nullptr,                                 VT100_MenuSetting,                      LBL_VT100_SYSTEM_SETTING                        ) \
        ENTRY  (MENU,  VT100_MenuMain,          VT100_ID_TEST_MENU,            nullptr,                                 VT100_MenuTest1,                        LBL_VT100_TEST                                  ) \
        ENTRY  (MENU,  VT100_MenuMain,          VT100_ID_STACK_DISPLAY,        CALLBACK_StackUsage,                     VT100_MenuRedirection,                  LBL_VT100_STACK_USAGE                           ) \
\
		ENTRY  (MENU,  VT100_MenuBoot,          VT100_ID_MAIN_TITLE,           nullptr,                                 VT100_MenuRedirection,                  LBL_VT100_MAIN_MENU                             ) \
        ENTRY  (MENU,  VT100_MenuBoot,          VT100_ID_INFO_DISPLAY,         VT100_CALLBACK_ProductInformation,       VT100_MenuMain,                         LBL_VT100_SYSTEM_INFO                           ) \
        ENTRY  (MENU,  VT100_MenuBoot,          VT100_ID_DEBUG_MENU,           nullptr,                                 VT100_MenuDebug,                        LBL_VT100_DEBUG                                 ) \
        ENTRY  (MENU,  VT100_MenuBoot,          VT100_ID_SETTING_MENU,         nullptr,                                 VT100_MenuSetting,                      LBL_VT100_SYSTEM_SETTING                        ) \
        ENTRY  (MENU,  VT100_MenuBoot,          VT100_ID_TEST_MENU,            nullptr,                                 VT100_MenuTest1,                        LBL_VT100_TEST                                  ) \
        ENTRY  (MENU,  VT100_MenuBoot,          VT100_ID_STACK_DISPLAY,        CALLBACK_StackUsage,                     VT100_MenuRedirection,                  LBL_VT100_STACK_USAGE                           ) \
\
        ENTRY  (MENU,  VT100_MenuDebug,         VT100_ID_DEBUG_TITLE,          nullptr,                                 VT100_MenuRedirection,                  LBL_VT100_DEBUG_MENU                            ) \
        ENTRY  (MENU,  VT100_MenuDebug,         VT100_ID_DBG_LVL_1,            CALLBACK_DebugLevelSetting,              VT100_MenuDebug,                        LBL_VT100_DEBUG_LEVEL_1                         ) \
        ENTRY  (MENU,  VT100_MenuDebug,         VT100_ID_DBG_LVL_2,            CALLBACK_DebugLevelSetting,              VT100_MenuDebug,                        LBL_VT100_DEBUG_LEVEL_2                         ) \
        ENTRY  (MENU,  VT100_MenuDebug,         VT100_ID_DBG_LVL_3,            CALLBACK_DebugLevelSetting,              VT100_MenuDebug,                        LBL_VT100_DEBUG_LEVEL_3                         ) \
        ENTRY  (MENU,  VT100_MenuDebug,         VT100_ID_DBG_LVL_4,            CALLBACK_DebugLevelSetting,              VT100_MenuDebug,                        LBL_VT100_DEBUG_LEVEL_4                         ) \
        ENTRY  (MENU,  VT100_MenuDebug,         VT100_ID_DBG_LVL_5,            CALLBACK_DebugLevelSetting,              VT100_MenuDebug,                        LBL_VT100_DEBUG_LEVEL_5                         ) \
        ENTRY  (MENU,  VT100_MenuDebug,         VT100_ID_DBG_LVL_6,            CALLBACK_DebugLevelSetting,              VT100_MenuDebug,                        LBL_VT100_DEBUG_LEVEL_6                         ) \
        ENTRY  (MENU,  VT100_MenuDebug,         VT100_ID_DBG_LVL_7,            CALLBACK_DebugLevelSetting,              VT100_MenuDebug,                        LBL_VT100_DEBUG_LEVEL_7                         ) \
        ENTRY  (MENU,  VT100_MenuDebug,         VT100_ID_DBG_LVL_8,            CALLBACK_DebugLevelSetting,              VT100_MenuDebug,                        LBL_VT100_DEBUG_LEVEL_8                         ) \
        ENTRY  (MENU,  VT100_MenuDebug,         VT100_ID_DBG_LVL_9,            CALLBACK_DebugLevelSetting,              VT100_MenuDebug,                        LBL_VT100_DEBUG_LEVEL_9                         ) \
\
        ENTRY  (MENU,  VT100_MenuSetting,       VT100_ID_SETTING_TITLE,        nullptr,                                 VT100_MenuRedirection,                  LBL_VT100_SYSTEM_SETTING_MENU                   ) \
        ENTRY  (MENU,  VT100_MenuSetting,       VT100_ID_MISC_MENU,            nullptr,                                 VT100_MenuSetMisc,                      LBL_VT100_MISCELLEANEOUS                        ) \
        ENTRY  (MENU,  VT100_MenuSetting,       VT100_ID_TIME_MENU,            nullptr,                                 VT100_MenuSettingTime,                  LBL_VT100_TIME_AND_DATE                         ) \
\
        ENTRY  (MENU,  VT100_MenuTest,          VT100_ID_DBG_STACK,            CALLBACK_DebugStack,                     VT100_MenuTest,                         LBL_VT100_STACKTISTIC                           ) \
        ENTRY  (MENU,  VT100_MenuTest,          VT100_ID_DBG_LVL_10,           nullptr,                                 VT100_MenuRedirection,                  LBL_VT100_TEST_1                                ) \
        ENTRY  (MENU,  VT100_MenuTest,          VT100_ID_DBG_LVL_10,           nullptr,                                 VT100_MenuTest,                         LBL_VT100_POUTINE_1                             ) \
        ENTRY  (MENU,  VT100_MenuTest,          VT100_ID_DBG_LVL_11,           nullptr,                                 VT100_MenuTest,                         LBL_VT100_POUTINE_2                             ) \
\
		ENTRY  (MENU,  VT100_MenuSettingTime,   VT100_ID_TIME_DATE_TITLE,      nullptr,                                 VT100_MenuSetting,                      LBL_VT100_TIME_AND_DATE_CONFIGURATION           ) \
        ENTRY  (MENU,  VT100_MenuSettingTime,   VT100_ID_HOUR,                 CALLBACK_TimeDateCfg,                    VT100_MenuSettingTime,                  LBL_VT100_HOUR                                  ) \
        ENTRY  (MENU,  VT100_MenuSettingTime,   VT100_ID_MINUTE,               CALLBACK_TimeDateCfg,                    VT100_MenuSettingTime,                  LBL_VT100_MINUTE                                ) \
        ENTRY  (MENU,  VT100_MenuSettingTime,   VT100_ID_SECOND,               CALLBACK_TimeDateCfg,                    VT100_MenuSettingTime,                  LBL_VT100_SECOND                                ) \
        ENTRY  (MENU,  VT100_MenuSettingTime,   VT100_ID_DAY,                  CALLBACK_TimeDateCfg,                    VT100_MenuSettingTime,                  LBL_VT100_DAY                                   ) \
        ENTRY  (MENU,  VT100_MenuSettingTime,   VT100_ID_MONTH,                CALLBACK_TimeDateCfg,                    VT100_MenuSettingTime,                  LBL_VT100_MONTH                                 ) \
        ENTRY  (MENU,  VT100_MenuSettingTime,   VT100_ID_YEAR,                 CALLBACK_TimeDateCfg,                    VT100_MenuSettingTime,                  LBL_VT100_YEAR                                  ) \
        ENTRY  (MENU,  VT100_MenuSettingTime,   VT100_ID_TIME_DATE_SAVE,       CALLBACK_TimeDateCfg,                    VT100_MenuSettingTime,                  LBL_VT100_BLANK                                 ) \
\
        ENTRY  (MENU,  VT100_MenuSetMisc,       VT100_ID_MISC_TITLE,           nullptr,                                 VT100_MenuSetting,                      LBL_VT100_MISC_CONFIG_MENU                      ) \
        ENTRY  (MENU,  VT100_MenuSetMisc,       VT100_ID_MISC_OPT1,            CALLBACK_MiscCfg,                        VT100_MenuSetMisc,                      LBL_VT100_BLANK                                 ) \
        ENTRY  (MENU,  VT100_MenuSetMisc,       VT100_ID_MISC_OPT2,            CALLBACK_MiscCfg,                        VT100_MenuSetMisc,                      LBL_VT100_BLANK                                 ) \
        ENTRY  (MENU,  VT100_MenuSetMisc,       VT100_ID_MISC_SERIAL_NUMBER,   CALLBACK_MiscCfg,                        VT100_MenuSetMisc,                      LBL_VT100_SERIAL_NUMBER_SETTING                 ) \
        ENTRY  (MENU,  VT100_MenuSetMisc,       VT100_ID_MISC_SAVE,            CALLBACK_MiscCfg,                        VT100_MenuSetMisc,                      LBL_VT100_BLANK                                 ) \

// enums ------------------------------------------------------------------------------------------------------------

//TODO move to console stuff
// Item ID and Flag for menu
//XMENU(EXPAND_AS_MENU_ENUMS);

// ------------------------------------------------------------------------------------------------------------------
























#if 0

// For each line here you need a const declaration in console_var.h
//                     Menu ID                  Ptr menu declaration
#define VT100_MENU_DEF(X_VT100_MENU)  \
    X_VT100_MENU    ( VT100_MenuMain,           VT100_Terminal::m_MENU_Main                 )  \
    X_VT100_MENU    ( VT100_MenuBoot,           VT100_Terminal::m_MENU_Boot                 )  \
    X_VT100_MENU    ( VT100_MenuDebug,          VT100_Terminal::m_MENU_Debug                )  \
    X_VT100_MENU    ( VT100_MenuTest1,          VT100_Terminal::m_MENU_Test                 )  \
    X_VT100_MENU    ( VT100_MenuSetting,        VT100_Terminal::m_MENU_Setting              )  \
    X_VT100_MENU    ( VT100_MenuSettingTime,    VT100_Terminal::m_MENU_SettingTimeDate      )  \
    X_VT100_MENU    ( VT100_MenuSetMisc,        VT100_Terminal::m_MENU_SettingMisc          )  \

//    X_VT100_MENU    ( VT100_MENU_SELECT_MENU,   VT100_Terminal::m_MENU_Select               )


// Callback list for menu items
#define VT100_CALLBACK_DEF(X_VT100_CALLBACK)                          \
/* --- Redirection callback menu                                --- */\
    X_VT100_CALLBACK (  VT100_Terminal:CALL_MenuSelection            )\
/*------------------------------------------------------------------*/\
/* --- Main menu callback                                       --- */\
    X_VT100_CALLBACK (  VT100_Terminal:CALL_ProductInformation       )\
    X_VT100_CALLBACK (  VT100_Terminal:CALL_DebugLevelSetting        )\
/*------------------------------------------------------------------*/\
/* --- Test menu callback                                       --- */\
    X_VT100_CALLBACK (  VT100_Terminal:CALL_InputReading             )\
    X_VT100_CALLBACK (  VT100_Terminal:CALL_LedControl               )\
    X_VT100_CALLBACK (  VT100_Terminal:CALL_Stacktistic              )\
/*------------------------------------------------------------------*/\
/* --- Main setting menu                                        --- */\
    X_VT100_CALLBACK (  VT100_Terminal:CALL_SettingCfg               )\
    X_VT100_CALLBACK (  VT100_Terminal:CALL_MiscCfg                  )\
    X_VT100_CALLBACK (  VT100_Terminal:CALL_TimeDateCfg              )\
/*------------------------------------------------------------------*/\


#endif

//-------------------------------------------------------------------------------------------------




