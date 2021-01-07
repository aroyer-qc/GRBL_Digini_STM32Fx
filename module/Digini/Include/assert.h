//-------------------------------------------------------------------------------------------------
//
//  File : lib_assert.h
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
// C++ linkage
//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef DIGINI_USE_ASSERT

//-------------------------------------------------------------------------------------------------
// Macro(s)
//-------------------------------------------------------------------------------------------------

#if defined(DEBUG) && defined(USE_FULL_ASSERT)
//  The assert_param macro is used for function's Parameters check.
//  If expr is false, it calls assert_failed function which reports the name of the source file and
//  the source line number of the call that failed.
//  If expr is true, it returns no value.
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((const char*)__FILE__, __LINE__))
#else
  #define assert_param(expr)           (void)0
#endif

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

#if defined(DEBUG) && defined(USE_FULL_ASSERT)
    void assert_failed(const char* file, int line);
#else

#endif

#endif

//-------------------------------------------------------------------------------------------------
// C++ linkage
//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------------------------------
