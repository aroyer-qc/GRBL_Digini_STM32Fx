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

#define VT100_STARTUP_MENU_CFG                  MenuMain

#define VT100_USE_COLOR                         DEF_ENABLED
#define VT100_USER_CALLBACK_INITIALIZE          DEF_DISABLED

// Only one of those define can be used or none of them
#define VT100_USE_STANDARD_MENU_STATIC_INFO     DEF_ENABLED
#define VT100_USE_USER_MENU_STATIC_INFO         DEF_DISABLED

#define VT100_USER_LABEL_LANGUAGE_DEF(X_VT100_USER_LBL_CFG) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MENU_TITLE,                       "GRBL Configuration Menu",                "Menu de Configuration GRBL"                  ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MAIN_MENU,                        "Main Menu",                              "Menu Principal"                              ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_SYSTEM_INFO,                      "Display System Information",             "Information Système"                         ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG,                            "Debug",                                  "Débogage"                                    ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_SYSTEM_SETTING,                   "System Settings",                        "Configuration Système"                       ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_TEST,                             "Test",                                   "Test"                                        ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_STACKTISTIC,                      "Stack Usage",                            "Usage des Stacks"                            ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_MENU,                       "Debug Menu",                             "Menu de Débogage"                            ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_1,                    "Dbg level 1  - System Status   ( )",     "Dbg level 1  - Status Système  ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_2,                    "Dbg level 2  - System Action   ( )",     "Dbg level 2  - Action Système  ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_3,                    "Dbg level 3  - System Health   ( )",     "Dbg level 3  - Santé Système   ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_4,                    "Dbg level 4  - Monitor/Logs    ( )",     "Dbg level 4  - Moniteur/Logs   ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_5,                    "Dbg level 5  - Stack Warning   ( )",     "Dbg level 5  - Alerte Stack    ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_6,                    "Dbg level 6  - Free            ( )",     "Dbg level 6  - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_7,                    "Dbg level 7  - Free            ( )",     "Dbg level 7  - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_8,                    "Dbg level 8  - Free            ( )",     "Dbg level 8  - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_9,                    "Dbg level 9  - Free            ( )",     "Dbg level 9  - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_10,                   "Dbg level 10 - Free            ( )",     "Dbg level 10 - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_11,                   "Dbg level 11 - Free            ( )",     "Dbg level 11 - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_12,                   "Dbg level 12 - Free            ( )",     "Dbg level 12 - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_13,                   "Dbg level 13 - Free            ( )",     "Dbg level 13 - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_14,                   "Dbg level 14 - Free            ( )",     "Dbg level 14 - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_15,                   "Dbg level 15 - Free            ( )",     "Dbg level 15 - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DEBUG_LEVEL_16,                   "Dbg level 16 - Free            ( )",     "Dbg level 16 - Libre           ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_SYSTEM_SETTING_MENU,              "System Settings Menu",                   "Menu Configuration Système"                  ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MISCELLEANEOUS,                   "Miscelleaneous",                         "Divers"                                      ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_TIME_AND_DATE,                    "Time and Date",                          "Heure et Date"                               ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_TEST_1,                           "Test 1",                                 "Test 1"                                      ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_POUTINE_1,                        "Poutine ski                    ( )",     "Poutine ski                    ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_POUTINE_2,                        "Poutine ski                    ( )",     "Poutine ski                    ( )"          ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_TIME_AND_DATE_CONFIGURATION,      "Time and Date Configuration",            "Configuration Heure et Date"                 ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_HOUR,                             "Hour\r",                                 "Heure\r"                                     ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MINUTE,                           "Minute\r",                               "Minute\r"                                    ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_SECOND,                           "Second\r",                               "Seconde\r"                                   ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_DAY,                              "Day\r",                                  "Jour\r"                                      ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MONTH,                            "Month\r",                                "Mois\r"                                      ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_YEAR,                             "Year\r",                                 "Année\r"                                     ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_BLANK,                            " ",                                      " "                                           ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_MISC_CONFIG_MENU,                 "Miscellaneous Configuration Menu",       "Menu de Configuration Divers"                ) \
    X_VT100_USER_LBL_CFG( LBL_VT100_SERIAL_NUMBER_SETTING,            "Setting Serial Number",                  "Configurer Numéro de Série"                  ) \


// Define the compare name (I did not find a way to get rid of those definition by the use of advanced macro
#define COMPARE_MenuRedirection(x)  x
#define COMPARE_MenuMain(x) 		x
#define COMPARE_MenuBoot(x)    		x
#define COMPARE_MenuInfo(x)    		x
#define COMPARE_MenuStackUsage(x)   x
#define COMPARE_MenuDebug(x)    	x
#define COMPARE_MenuTest(x)     	x
#define COMPARE_MenuSetting(x)		x
#define COMPARE_MenuSettingTime(x)  x
#define COMPARE_MenuSetMisc(x) 		x

// Define here all the menu and sub menu you will use in the VT100 terminal
#define VT100_MENU_DEF(ENTRY)  \
    ENTRY(MenuRedirection    ) \
    ENTRY(MenuMain           ) \
    ENTRY(MenuBoot           ) \
    ENTRY(MenuInfo           ) \
    ENTRY(MenuStackUsage     ) \
    ENTRY(MenuDebug          ) \
    ENTRY(MenuTest           ) \
    ENTRY(MenuSetting        ) \
    ENTRY(MenuSettingTime    ) \
    ENTRY(MenuSetMisc        ) \

// Define here all the callback you will use in the VT100 terminal
#define VT100_CALLBACK(ENTRY)           \
    ENTRY(CALLBACK_MenuRedirection    ) \
    ENTRY(CALLBACK_ProductInformation ) \
    ENTRY(CALLBACK_StackUsage         ) \
    ENTRY(CALLBACK_DebugLevelSetting  ) \
    ENTRY(CALLBACK_TimeDateCfg        ) \
    ENTRY(CALLBACK_MiscCfg            ) \

// Here you define how you will navigate in the menu tree and action associated with them                   1st item of a menu
//              Menu   Member Of          Item ID                  CALLBACK for refresh                     Navigate to this menu on <ESC>    Label ID
//                                                                 on first if CALLBACK_None there is       all other. Navigate to this menu
//                                                                 no refresh. All other items are
//                                                                  items processing services
#define VT100_MENU_TREE_DEF(ENTRY, MENU) \
\
        ENTRY  (MENU,  MenuRedirection,   ID_REDIRECTION,          CALLBACK_MenuRedirection,                VT100_MENU_NONE,                  INVALID_LABEL                                   ) \
\
        ENTRY  (MENU,  MenuMain,          ID_MAIN_TITLE,           CALLBACK_None,                           VT100_MENU_NONE,                  LBL_VT100_MAIN_MENU                             ) \
        ENTRY  (MENU,  MenuMain,          ID_INFO_DISPLAY,         CALLBACK_None,                           MenuInfo,                         LBL_VT100_SYSTEM_INFO                           ) \
        ENTRY  (MENU,  MenuMain,          ID_DEBUG_MENU,           CALLBACK_None,                           MenuDebug,                        LBL_VT100_DEBUG                                 ) \
        ENTRY  (MENU,  MenuMain,          ID_SETTING_MENU,         CALLBACK_None,                           MenuSetting,                      LBL_VT100_SYSTEM_SETTING                        ) \
        ENTRY  (MENU,  MenuMain,          ID_TEST_MENU,            CALLBACK_None,                           MenuTest,                         LBL_VT100_TEST                                  ) \
        ENTRY  (MENU,  MenuMain,          ID_STACK_DISPLAY,        CALLBACK_None,                           MenuStackUsage,                   LBL_VT100_STACKTISTIC                           ) \
\
		ENTRY  (MENU,  MenuBoot,          ID_MAIN_TITLE,           CALLBACK_None,                           VT100_MENU_NONE,                  LBL_VT100_MAIN_MENU                             ) \
        ENTRY  (MENU,  MenuBoot,          ID_INFO_DISPLAY,         CALLBACK_None,                           MenuMain,                         LBL_VT100_SYSTEM_INFO                           ) \
        ENTRY  (MENU,  MenuBoot,          ID_DEBUG_MENU,           CALLBACK_None,                           MenuDebug,                        LBL_VT100_DEBUG                                 ) \
        ENTRY  (MENU,  MenuBoot,          ID_SETTING_MENU,         CALLBACK_None,                           MenuSetting,                      LBL_VT100_SYSTEM_SETTING                        ) \
        ENTRY  (MENU,  MenuBoot,          ID_TEST_MENU,            CALLBACK_None,                           MenuTest,                         LBL_VT100_TEST                                  ) \
        ENTRY  (MENU,  MenuBoot,          ID_STACK_DISPLAY,        CALLBACK_None,                           MenuStackUsage,                   LBL_VT100_STACKTISTIC                           ) \
\
        ENTRY  (MENU,  MenuInfo,          ID_INFO_DISPLAY,         CALLBACK_ProductInformation,             MenuRedirection,                  LBL_VT100_SYSTEM_INFO                           ) \
\
        ENTRY  (MENU,  MenuStackUsage,    ID_INFO_DISPLAY,         CALLBACK_StackUsage,                     MenuRedirection,                  LBL_VT100_STACKTISTIC                           ) \
\
        ENTRY  (MENU,  MenuDebug,         ID_DEBUG_TITLE,          CALLBACK_None,                           MenuRedirection,                  LBL_VT100_DEBUG_MENU                            ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_0,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_1                         ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_1,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_2                         ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_2,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_3                         ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_3,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_4                         ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_4,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_5                         ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_5,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_6                         ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_6,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_7                         ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_7,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_8                         ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_8,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_9                         ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_9,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_10                        ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_A,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_11                        ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_B,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_12                        ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_C,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_13                        ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_D,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_14                        ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_E,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_15                        ) \
        ENTRY  (MENU,  MenuDebug,         ID_DBG_LVL_F,            CALLBACK_DebugLevelSetting,              MenuDebug,                        LBL_VT100_DEBUG_LEVEL_16                        ) \
\
        ENTRY  (MENU,  MenuSetting,       ID_SETTING_TITLE,        CALLBACK_None,                           MenuRedirection,                  LBL_VT100_SYSTEM_SETTING_MENU                   ) \
        ENTRY  (MENU,  MenuSetting,       ID_MISC_MENU,            CALLBACK_None,                           MenuSetMisc,                      LBL_VT100_MISCELLEANEOUS                        ) \
        ENTRY  (MENU,  MenuSetting,       ID_TIME_MENU,            CALLBACK_None,                           MenuSettingTime,                  LBL_VT100_TIME_AND_DATE                         ) \
\
        ENTRY  (MENU,  MenuTest,          ID_DBG_STACK,            CALLBACK_StackUsage,                     MenuTest,                         LBL_VT100_STACKTISTIC                           ) \
        ENTRY  (MENU,  MenuTest,          ID_DBG_LVL_10,           CALLBACK_None,                           MenuRedirection,                  LBL_VT100_TEST_1                                ) \
        ENTRY  (MENU,  MenuTest,          ID_DBG_LVL_11,           CALLBACK_None,                           MenuTest,                         LBL_VT100_POUTINE_1                             ) \
        ENTRY  (MENU,  MenuTest,          ID_DBG_LVL_12,           CALLBACK_None,                           MenuTest,                         LBL_VT100_POUTINE_2                             ) \
\
		ENTRY  (MENU,  MenuSettingTime,   ID_TIME_DATE_TITLE,      CALLBACK_None,                           MenuSetting,                      LBL_VT100_TIME_AND_DATE_CONFIGURATION           ) \
        ENTRY  (MENU,  MenuSettingTime,   ID_HOUR,                 CALLBACK_TimeDateCfg,                    MenuSettingTime,                  LBL_VT100_HOUR                                  ) \
        ENTRY  (MENU,  MenuSettingTime,   ID_MINUTE,               CALLBACK_TimeDateCfg,                    MenuSettingTime,                  LBL_VT100_MINUTE                                ) \
        ENTRY  (MENU,  MenuSettingTime,   ID_SECOND,               CALLBACK_TimeDateCfg,                    MenuSettingTime,                  LBL_VT100_SECOND                                ) \
        ENTRY  (MENU,  MenuSettingTime,   ID_DAY,                  CALLBACK_TimeDateCfg,                    MenuSettingTime,                  LBL_VT100_DAY                                   ) \
        ENTRY  (MENU,  MenuSettingTime,   ID_MONTH,                CALLBACK_TimeDateCfg,                    MenuSettingTime,                  LBL_VT100_MONTH                                 ) \
        ENTRY  (MENU,  MenuSettingTime,   ID_YEAR,                 CALLBACK_TimeDateCfg,                    MenuSettingTime,                  LBL_VT100_YEAR                                  ) \
        ENTRY  (MENU,  MenuSettingTime,   ID_TIME_DATE_SAVE,       CALLBACK_TimeDateCfg,                    MenuSettingTime,                  LBL_VT100_BLANK                                 ) \
\
        ENTRY  (MENU,  MenuSetMisc,       ID_MISC_TITLE,           CALLBACK_None,                           MenuSetting,                      LBL_VT100_MISC_CONFIG_MENU                      ) \
        ENTRY  (MENU,  MenuSetMisc,       ID_MISC_OPT1,            CALLBACK_MiscCfg,                        MenuSetMisc,                      LBL_VT100_BLANK                                 ) \
        ENTRY  (MENU,  MenuSetMisc,       ID_MISC_OPT2,            CALLBACK_MiscCfg,                        MenuSetMisc,                      LBL_VT100_BLANK                                 ) \
        ENTRY  (MENU,  MenuSetMisc,       ID_MISC_SERIAL_NUMBER,   CALLBACK_MiscCfg,                        MenuSetMisc,                      LBL_VT100_SERIAL_NUMBER_SETTING                 ) \
        ENTRY  (MENU,  MenuSetMisc,       ID_MISC_SAVE,            CALLBACK_MiscCfg,                        MenuSetMisc,                      LBL_VT100_BLANK                                 ) \

// ------------------------------------------------------------------------------------------------------------------
