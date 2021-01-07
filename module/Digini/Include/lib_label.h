//-------------------------------------------------------------------------------------------------
//
//  File : lib_label.h
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

#include "label_cfg.h"

//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
// Enum(s)
//-------------------------------------------------------------------------------------------------

enum Language_e
{
    LANG_ENGLISH = 0,
    LANG_FRENCH,
    NB_LANGUAGE_CONST,
};


enum Label_e
{
    #define X_LABEL(ID, LBL1, LBL2) ID,
        LABEL_LANGUAGE_DEF
    #undef X_LABEL
        NB_LABEL_CONST,
        INVALID_LABEL,
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef DIGINI_GLOBAL

  const char* LBL_Application[NB_LABEL_CONST][NB_LANGUAGE_CONST] =
  {
    #define X_LABEL(ID, LBL1, LBL2) {LBL1, LBL2},
      LABEL_LANGUAGE_DEF
    #undef X_LABEL
  };

#else

 extern const char* LBL_Application[NB_LABEL_CONST][NB_LANGUAGE_CONST];

#endif // DIGINI_GLOBAL

//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------------------------------
