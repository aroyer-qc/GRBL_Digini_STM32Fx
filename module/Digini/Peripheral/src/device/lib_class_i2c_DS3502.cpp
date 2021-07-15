//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_i2c_DS3502.cpp
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

#define LIB_DS3502_GLOBAL
#include "lib_class_i2c_DS3502.h"
#undef  LIB_DS3502_GLOBAL

//-------------------------------------------------------------------------------------------------

#if USE_I2C_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define DS3502_WR_IVR_REGISTER                        ((uint8_t)0x00)
#define DS3502_CR_REGISTER                            ((uint8_t)0x02)

#define DS3502_CR_MODE_0__WRITE_BOTH_WR_IVR         ((uint8_t)0x00)
#define DS3502_CR_MODE_1__WRITE_WR_ONLY             ((uint8_t)0x80)

#define DS3502_MAX_VALUE                            ((uint8_t)0x7F)

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   void* pArg          Pointer on the type of driver use by this class
//
//  Return:         SystemState_e
//
//  Description:    Initialize the DS3502 Digital pot
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e DS3502::Initialize(void* pArg)
{
    uint8_t WriteBuffer[2];

    m_pI2C = (I2C*)pArg;

   // Initialize I2C link
   m_pI2C->Initialize();

    // Read initial wiper position
    m_pI2C->ReadRegister(DS3502_WR_IVR_REGISTER, m_WiperPos, sizeof(uint8_t), DS3502_I2C_SLAVE_ADDRESS);

    // Always leave the DS3502 in mode 1
    WriteBuffer[0] = DS3502_CR_REGISTER;
    WriteBuffer[1] = DS3502_CR_MODE_1__WRITE_WR_ONLY;
    m_pI2C->Transfer(&WriteBuffer[0], 2, nullptr, 0, DS3502_I2C_SLAVE_ADDRESS);

     m_WiperIV = m_WiperPos;            // At power-up both value are equal
     m_MaxValue = DS3502_MAX_VALUE;     // Set max value to chip max

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
//  Description:    Software Reset the DS3502.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DS3502::Reset(void)
{
    uint8_t WriteBuffer[2];

    // Reset to initial wiper position

    WriteBuffer[0] = DS3502_CR_REGISTER;
    WriteBuffer[1] = DS3502_CR_MODE_0__WRITE_BOTH_WR_IVR;
    m_pI2C->Transfer(&WriteBuffer[0], 2, nullptr, 0, DS3502_I2C_SLAVE_ADDRESS);

    WriteBuffer[0] = DS3502_WR_IVR_REGISTER;
    WriteBuffer[1] = m_WiperIV;
    m_pI2C->Transfer(&WriteBuffer[0], 2, nullptr, 0, DS3502_I2C_SLAVE_ADDRESS);

    WriteBuffer[0] = DS3502_CR_REGISTER;
    WriteBuffer[1] = DS3502_CR_MODE_1__WRITE_WR_ONLY;
    m_pI2C->Transfer(&WriteBuffer[0], 2, nullptr, 0, DS3502_I2C_SLAVE_ADDRESS);

    // Both value are equal
    m_WiperPos = m_WiperIV;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SetWiperIV
//
//  Parameter(s):   uint8_t     InitialValue
//
//  Return:         None
//
//  Description:    Set the Initial Value of the Pot
//
//  Note(s):        It also set a new wiper pot position
//
//-------------------------------------------------------------------------------------------------
void DS3502::SetWiperIV(uint8_t InitialValue)
{
    m_WiperIV  = InitialValue;
    this->Reset();
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SetWiper
//
//  Parameter(s):   uint8_t     WiperValue
//
//  Return:         None
//
//  Description:    Set a new wiper pot position
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DS3502::SetWiper(uint8_t WiperValue)
{
    uint8_t WriteBuffer[2];

    m_WiperPos = WiperValue;

    // Set the wiper position
    WriteBuffer[0] = DS3502_WR_IVR_REGISTER;
    WriteBuffer[1] = WiperValue;
    m_pI2C->Transfer(&WriteBuffer[0], 2, nullptr, 0, DS3502_I2C_SLAVE_ADDRESS);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Up
//
//  Parameter(s):   uint8_t     WiperValue
//
//  Return:         None
//
//  Description:    Raise pot value 1 step
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DS3502::Up(void)
{
    if(m_WiperPos < m_MaxValue)
    {
        m_WiperPos++;
    }

    this->SetWiper(m_WiperPos);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Down
//
//  Parameter(s):   uint8_t     WiperValue
//
//  Return:         None
//
//  Description:    Lower pot value 1 step
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DS3502::Down(void)
{
    if(m_WiperPos != 0)
    {
        m_WiperPos--;
    }

    this->SetWiper(m_WiperPos);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SetMaxValue
//
//  Parameter(s):   uint8_t     MaxValue
//
//  Return:         None
//
//  Description:    Set a max value for the wiper pot
//
//  Note(s):        Also if new max is lower than actual wiper position, it will lower it
//
//-------------------------------------------------------------------------------------------------
void DS3502::SetMaxValue(uint8_t MaxValue)
{
    if(MaxValue <= DS3502_MAX_VALUE)
    {
        m_MaxValue = MaxValue;

        if(m_WiperPos > MaxValue)
        {
            this->SetWiper(MaxValue);
        }
    }
}

//-------------------------------------------------------------------------------------------------

#endif // USE_I2C_DRIVER == DEF_ENABLED
