//-------------------------------------------------------------------------------------------------
//
//  File :  eeprom_var.h
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
//  Note: EEPROM variables for GRBL
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef EEPROM_DRIVER_GLOBAL

const EEPROM_Info_t GRBL_SettingE2 =
{
    &myI2C_External,
    GRBL_EEPROM_SIZE,
    GRBL_NB_OF_PAGE,
    GRBL_DEVICE_ADDRESS,
};

class EEPROM_Driver myE2_Setting(&GRBL_SettingE2);

#else // EEPROM_DRIVER_GLOBAL

extern const EEPROM_Info_t GRBL_SettingE2;

extern class EEPROM_Driver myE2_Setting;

#endif // EEPROM_DRIVER_GLOBAL

//-------------------------------------------------------------------------------------------------
