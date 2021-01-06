//-------------------------------------------------------------------------------------------------
//
//  File :  widget_var.h
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
//
//  Notes:
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
// include(s)
//-------------------------------------------------------------------------------------------------

#include "system.h"
#include "bsp.h"

//-------------------------------------------------------------------------------------------------
// Constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef WIDGET_VARIABLE_GLOBAL

InputFloat_t Input5_5_F_Cfg
{
    EXCHANGE_INPUT_TYPE_FLOAT,
    LBL_TEST_1,
    (void*)&gc_state.coord_system[0],       // Variables to take data from, and deposit after edit
    INVALID_LABEL,
    -99.9999,                               // Minimum value for edit
    99.9999,                                // Maximum value for edit
};

InputDecimal_t Input10_D_Cfg
{
    EXCHANGE_INPUT_TYPE_DECIMAL,
    LBL_TEST_2,
    &TestInput,                             // Variables to take data from, and deposit after edit
    INVALID_LABEL,
    -100,                                   // Minimum value for edit
    100000000,                              // Maximum value for edit
};

InputDecimal_t Input6_D_Cfg
{
    EXCHANGE_INPUT_TYPE_HEXA,
    LBL_TEST_2,
    &TestInput,                             // Variables to take data from, and deposit after edit
    INVALID_LABEL,
    0,                                      // Minimum value for edit
    880000,                                 // Maximum value for edit
};

InputDecimal_t Input5_D_Cfg
{
    EXCHANGE_INPUT_TYPE_DECIMAL,
    LBL_TEST_3,
    &TestInput,                             // Variables to take data from, and deposit after edit
    INVALID_LABEL,
    0,                                      // Minimum value for edit
    99999,                                  // Maximum value for edit
};


InputFloat_t InputSpindleMin_Cfg
{
    EXCHANGE_INPUT_TYPE_FLOAT,
    LBL_MIN_SPINDLE,
    &settings.rpm_min,                      // Variables to take data from, and deposit after edit
    LBL_SPEED_FLOAT,
    0,                                      // Minimum value for edit
    500,                                    // Maximum value for edit to evaluate
};

InputFloat_t InputSpindleMax_Cfg
{
    EXCHANGE_INPUT_TYPE_FLOAT,
    LBL_MAX_SPINDLE,
    &settings.rpm_max,                      // Variables to take data from, and deposit after edit
    LBL_SPEED_FLOAT,
    510,                                    // Minimum value for edit
    99999,                                  // Maximum value for edit to evaluate
};

#else

extern InputFloat_t     Input5_5_F_Cfg;
extern InputDecimal_t   Input10_D_Cfg;
extern InputDecimal_t   Input6_D_Cfg;
extern InputDecimal_t   Input5_D_Cfg;
extern InputDecimal_t   InputSpindleMin_Cfg;
extern InputDecimal_t   InputSpindleMax_Cfg;

#endif

//-------------------------------------------------------------------------------------------------
