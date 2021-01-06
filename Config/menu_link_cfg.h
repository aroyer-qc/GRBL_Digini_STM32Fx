//-------------------------------------------------------------------------------------------------
//
//  File :  menu_link_cfg.h
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
// Global Define(s)
//-------------------------------------------------------------------------------------------------

//          Enum ID,              Struct object name
#define LINK_DEF                                         \
    X_LINK( LINK_MAIN_LOADING,    PageLoading          ) \
    X_LINK( LINK_MAIN_HUB,        PageMainHub          ) \
    X_LINK( LINK_JOG_HUB,         PageJogHub           ) \
    X_LINK( LINK_MAIN_PAGE_XY,    PageMainDisplayXY    ) \
    X_LINK( LINK_MAIN_PAGE_ZX,    PageMainDisplayXZ    ) \
    X_LINK( LINK_MAIN_PAGE_XYZ,   PageMainDisplayXYZ   ) \
    X_LINK( LINK_MAIN_PAGE_XYT,   PageMainDisplayXYT   ) \
    X_LINK( LINK_MAIN_PAGE_ZXT,   PageMainDisplayXZT   ) \
    X_LINK( LINK_MAIN_PAGE_XYZT,  PageMainDisplayXYZT  ) \
/*    X_LINK( LINK_MAIN_PAGE_XYZAT, PageMainDisplayXYZAT ) */\
    X_LINK( LINK_MAIN_MENU,       PageMenu             ) \
    X_LINK( LINK_TEST_INPUT,      InputTestMenu        ) \
    X_LINK( LINK_INPUT_DATA,      InputData            ) \
    X_LINK( LINK_INPUT_DATA_HEX,  InputDataHexa        ) \
    X_LINK( LINK_AXIS_X,          PageAxisX            ) \
    X_LINK( LINK_AXIS_Y,          PageAxisY            ) \
    X_LINK( LINK_AXIS_Z,          PageAxisZ            ) \
    X_LINK( LINK_SPINDLE,         PageSpindle          ) \
    X_LINK( LINK_MACHINE_SELECT,  PageMachineSelect    ) \
    X_LINK( LINK_DIAGNOSTICS,     PageDiagnostic       ) \
    X_LINK( LINK_PAGE_INFO,       PageInfo             ) \
    X_LINK( LINK_PAGE_TERMINAL,   PageTerminal         ) \


//-------------------------------------------------------------------------------------------------

//    X_LINK( LINK_SET_REF,         PageMachineSelect    )






