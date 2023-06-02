//-------------------------------------------------------------------------------------------------
//
//  File :  vt100_var.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2022 Alain Royer.
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
// variable(s)
//-------------------------------------------------------------------------------------------------

#if 0


#ifdef VT100_HEADER_CLASS_CONSTANT_MEMBER

static const VT100_MenuDef_t VT100_Terminal::m_MENU_Select          [];
static const VT100_MenuDef_t VT100_Terminal::m_MENU_Main            [];
static const VT100_MenuDef_t VT100_Terminal::m_MENU_Boot            [];
static const VT100_MenuDef_t VT100_Terminal::m_MENU_Debug           [];
static const VT100_MenuDef_t VT100_Terminal::m_MENU_Test            [];
static const VT100_MenuDef_t VT100_Terminal::m_MENU_Setting         [];
static const VT100_MenuDef_t VT100_Terminal::m_MENU_SettingMisc     [];
static const VT100_MenuDef_t VT100_Terminal::m_MENU_SettingTimeDate [];

#endif // VT100_HEADER_CLASS_CONSTANT_MEMBER

//-------------------------------------------------------------------------------------------------

#ifdef VT100_CLASS_CONSTANT_MEMBER

// Definition for the 'Main Menu' ( this menu only decide which menu to chose between 'Boot Time' or 'Running App'
const VT100_MenuDef_t VT100_Terminal::m_MENU_Select[] =
{
    { LBL_MAIN_MENU, CALL_MenuSelection,   VT100_MENU_NONE },
};

// Definition for the 'Main Menu'
const VT100_MenuDef_t VT100_Terminal::m_MENU_Main[] =
{
    { LBL_MAIN_MENU, nullptr,                       VT100_MENU_NONE   },
    { LBL_MAIN_OPT1, CALL_ProductInformation,       VT100_MenuMain    },
    { LBL_MAIN_OPT2, nullptr,                       VT100_MenuDebug   },
    { LBL_MAIN_OPT3, nullptr,                       VT100_MenuSetting },
};

// Definition for the 'Main Menu at boot time'
const VT100_MenuDef_t VT100_Terminal::m_MENU_Boot[] =
{
    { LBL_MAIN_MENU, nullptr,                       VT100_MENU_NONE   },
    { LBL_MAIN_OPT1, CALL_ProductInformation,       VT100_MenuBoot    },
    { LBL_MAIN_OPT2, nullptr,                       VT100_MenuDebug   },
    { LBL_MAIN_OPT3, nullptr,                       VT100_MenuSetting },
    { LBL_MAIN_OPT4, nullptr,                       VT100_MenuTest1   },
};

// Definition for the 'test 1 Menu'
const VT100_MenuDef_t VT100_Terminal::m_MENU_Debug[] =
{
    { LBL_DEBUG_MENU, nullptr,                      VT100_MenuMain  },
    { LBL_DEBUG_OPT1, CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT2, CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT3, CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT4, CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT5, CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT6, CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT7, CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT8, CALL_DebugLevelSetting,       VT100_MenuDebug },
};

// Definition for the 'test 1 Menu'
const VT100_MenuDef_t VT100_Terminal::m_MENU_Test[] =
{
    { LBL_DEBUG_MENU,      nullptr,                      VT100_MenuMain  },
    { LBL_DEBUG_OPT1,      CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT2,      CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT3,      CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT4,      CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT5,      CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT6,      CALL_DebugLevelSetting,       VT100_MenuDebug },
    { LBL_DEBUG_OPT7,      CALL_DebugLevelSetting,       VT100_MenuDebug },
  //  { LBL_VT100_STATISTIC, CALL_DebugStack,              VT100_MenuTest  },
    { LBL_TEST_OPT2,       CALL_InputReading,            VT100_MenuTest  },
};

// Definition for the 'Setting Menu'
const VT100_MenuDef_t VT100_Terminal::m_MENU_Setting[] =
{
    { LBL_SETTING_MENU,       nullptr, VT100_MenuMain        },
    { LBL_SETTING_OPT1,       nullptr, VT100_MenuSettingTime },
    { LBL_SETTING_OPT2,       nullptr, VT100_MenuSetMisc     },
};

const VT100_MenuDef_t VT100_Terminal::m_MENU_SettingMisc[] =
{
    { LBL_MISC_CFG_MENU, CALL_MiscCfg, VT100_MenuSetting      },
/*    { LBL_MISC_CFG_OPT1, CALL_MiscCfg, VT100_MenuSetMisc },
    { LBL_MISC_CFG_OPT2, CALL_MiscCfg, VT100_MenuSetMisc },
    { LBL_MISC_CFG_OPT3, CALL_MiscCfg, VT100_MenuSetMisc },
    { LBL_MISC_CFG_OPT4, CALL_MiscCfg, VT100_MenuSetMisc },
    { LBL_MISC_CFG_OPT5, CALL_MiscCfg, VT100_MenuSetMisc },
    { LBL_MISC_CFG_OPT6, CALL_MiscCfg, VT100_MenuSetMisc },
    { LBL_MISC_CFG_OPT7, CALL_MiscCfg, VT100_MenuSetMisc },
    { LBL_CFG_OPT_BLANK, CALL_MiscCfg, VT100_MenuSetMisc },*/
};

const VT100_MenuDef_t VT100_Terminal::m_MENU_SettingTimeDate[] =
{
    { LBL_TIME_CFG_MENU, CALL_TimeDateCfg, VT100_MenuSetting     },
    { LBL_TIME_CFG_OPT1, CALL_TimeDateCfg, VT100_MenuSettingTime },
    { LBL_TIME_CFG_OPT2, CALL_TimeDateCfg, VT100_MenuSettingTime },
    { LBL_TIME_CFG_OPT3, CALL_TimeDateCfg, VT100_MenuSettingTime },
    { LBL_TIME_CFG_OPT4, CALL_TimeDateCfg, VT100_MenuSettingTime },
    { LBL_TIME_CFG_OPT5, CALL_TimeDateCfg, VT100_MenuSettingTime },
    { LBL_TIME_CFG_OPT6, CALL_TimeDateCfg, VT100_MenuSettingTime },
    { LBL_CFG_OPT_BLANK, CALL_TimeDateCfg, VT100_MenuSettingTime },
};

#endif // VT100_CLASS_CONSTANT_MEMBER

#endif // #if 0


#ifdef VT100_GLOBAL

class VT100_Terminal myVT100_Terminal;

#else

extern class VT100_Terminal myVT100_Terminal;

#endif // VT100_GLOBAL

//-------------------------------------------------------------------------------------------------
