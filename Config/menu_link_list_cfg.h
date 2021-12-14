//-------------------------------------------------------------------------------------------------
//
//  File : Menu_link_list_cfg.h
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

//-------------------------------------------------------------------------------------------------
// Global Define(s)
//-------------------------------------------------------------------------------------------------

#pragma once


//                                    Enum ID,                  ID1,                ID2,                ID3,                ID4,                ID5,                ID6,                 ID7,               ID8
#ifndef LINK_LIST_DEF
#define LINK_LIST_DEF(X_LINK_LIST) \
                        X_LINK_LIST ( LINK_LIST_MAIN_HUB,       LINK_MAIN_PAGE_XY,  LINK_MAIN_PAGE_ZX,  LINK_MAIN_PAGE_XYZ, LINK_MAIN_PAGE_XYT, LINK_MAIN_PAGE_ZXT, LINK_MAIN_PAGE_XYZT, INVALID_LINK,      INVALID_LINK ) \
                /*      X_LINK_LIST ( LINK_LIST_JOG_HUB,        LINK_JOG_PAGE_XY,   LINK_JOG_PAGE_ZX,   LINK_JOG_PAGE_XYZ,  INVALID_LINK,       INVALID_LINK,       INVALID_LINK,        INVALID_LINK,      INVALID_LINK ) \ */
#endif

//-------------------------------------------------------------------------------------------------

