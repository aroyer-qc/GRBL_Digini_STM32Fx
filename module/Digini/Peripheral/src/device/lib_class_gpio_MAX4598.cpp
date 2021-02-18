//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_gpio_MAX4598.cpp
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

#define LIB_MAX4598_GLOBAL
#include "lib_class_gpio_MAX4598.h"
#undef  LIB_MAX4598_GLOBAL

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   void* pArg
//
//  Return:         SystemState_e
//
//  Description:    Initialize the MAX4598 Analog Switch
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e MAX4598::Initialize(void* pArg)
{
    m_pPinStruct = (MAX4598_PinStruct_t*)pArg;
    MAX4598::Reset();
    IO_Output(m_pPinStruct->GPIO_Enable, IO_SET);
    return SYS_READY;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Reset
//
//  Parameter(s):   None
//
//  Return:         None
//
//  Description:    Software Reset the MAX4598.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void MAX4598::Reset(void)
{
    SetChannel(MAX4598_SELECT_HIZ_HIZ);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           SetChannel
//
//  Parameter(s):   MAX4598_e     Channel
//
//  Return:         None
//
//  Description:    Select the analog switch channel 
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void MAX4598::SetChannel(MAX4598_e Channel)
{
    IO_Output(m_pPinStruct->GPIO_A0, ((Channel & 0x01) != 0x00) ? IO_SET : IO_RESET);
    IO_Output(m_pPinStruct->GPIO_A1, ((Channel & 0x02) != 0x00) ? IO_SET : IO_RESET);
    IO_Output(m_pPinStruct->GPIO_A2, ((Channel & 0x04) != 0x00) ? IO_SET : IO_RESET);
    IO_Output(m_pPinStruct->GPIO_A3, ((Channel & 0x08) != 0x00) ? IO_SET : IO_RESET);
    IO_Output(m_pPinStruct->GPIO_Latch, IO_SET);
    IO_Output(m_pPinStruct->GPIO_Latch, IO_RESET);
}

//-------------------------------------------------------------------------------------------------
