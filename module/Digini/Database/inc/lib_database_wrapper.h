//-------------------------------------------------------------------------------------------------
//
//  File : lib_database_wrapper.h
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

#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------
// Type definition(s), enum(s) and structure(s)
//-------------------------------------------------------------------------------------------------

typedef void(*Func_DatabaseCallBack)(AccessRequest_e, void*, uint16_t, uint16_t);

//-------------------------------------------------------------------------------------------------

void AccessTime             (AccessRequest_e AccessRequest, void* pData, uint16_t Number, uint16_t SubNumber);
void AccessDate             (AccessRequest_e AccessRequest, void* pData, uint16_t Number, uint16_t SubNumber);
void AccessTimeFormat       (AccessRequest_e AccessRequest, void* pData, uint16_t Number, uint16_t SubNumber);

//-------------------------------------------------------------------------------------------------