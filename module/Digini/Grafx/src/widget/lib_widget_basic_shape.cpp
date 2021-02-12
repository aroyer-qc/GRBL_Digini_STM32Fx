//-------------------------------------------------------------------------------------------------
//
//  File : lib_widget_basic_shape.cpp
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

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "widget_cfg.h"
#ifdef BASIC_SHAPE_DEF
#include <stdint.h>
#include "lib_grafx.h"
#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
//
//  Constructor:    CBasicShape
//
//  Parameter(s):   BasicShape_t         pBasicShape         Pointer to BasicShape_t structure
//
//  Description:    Initialize widget's service.
//
//  Note(s):        Class
//
//-------------------------------------------------------------------------------------------------
CBasicShape::CBasicShape(BasicShape_t* pBasicShape)
{
    m_pBasicShape = pBasicShape;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Create
//
//  Parameter(s):   PageWidget_t* pPageWidget
//  Return:         INVALID_LINK
//
//  Description:    This function call and pass exchange data to service
//
//  Note(s)
//
//-------------------------------------------------------------------------------------------------
Link_e BasicShape::Create(PageWidget_t* pPageWidget)
{
    VAR_UNUSED(pPageWidget);
    // Nothing to do
    return INVALID_LINK;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Refresh
//
//  Parameter(s):   MsgRefresh_t*        pMsg
//  Return:         INVALID_LINK
//
//  Description:    This function does nothing
//
//-------------------------------------------------------------------------------------------------
Link_e BasicShape::Refresh(MsgRefresh_t* pMsg)
{
    VAR_UNUSED(pMsg);
    // Nothing to do
    return INVALID_LINK;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Finalize
//
//  Parameter(s):   none
//  Return:         void
//
//  Description:    This function call service associated with widget to finalize it properly.
//
//-------------------------------------------------------------------------------------------------
void BasicShape::Finalize()
{
    // Nothing to do
}

//-------------------------------------------------------------------------------------------------

#endif // BASIC_SHAPE_DEF
#endif // DIGINI_USE_GRAFX
