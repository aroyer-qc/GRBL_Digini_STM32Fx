//-------------------------------------------------------------------------------------------------
//
//  File :  i2c_cfg.h
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
//  Notes: I2C configuration for STM32F746NG
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
// Global Define(s)
//-------------------------------------------------------------------------------------------------

#define I2C_DRIVER_NUM_SUPPORTED        2

#define I2C_DRIVER_SUPPORT_I2C1			DEF_ENABLED
#define I2C_DRIVER_SUPPORT_I2C2			DEF_DISABLED
#define I2C_DRIVER_SUPPORT_I2C3			DEF_ENABLED
#define I2C_DRIVER_SUPPORT_I2C4			DEF_DISABLED


//-------------------------------------------------------------------------------------------------
// typedef(s)
//-------------------------------------------------------------------------------------------------

enum I2C_Port_e
{
    #if (I2C_DRIVER_SUPPORT_I2C1 == DEF_ENABLED)
		DRIVER_I2C_ID_1,
	#endif

    #if (I2C_DRIVER_SUPPORT_I2C2 == DEF_ENABLED)
		DRIVER_I2C_ID_2,
	#endif

    #if (I2C_DRIVER_SUPPORT_I2C3 == DEF_ENABLED)
		DRIVER_I2C_ID_3,
	#endif

    #if (I2C_DRIVER_SUPPORT_I2C4 == DEF_ENABLED)
		DRIVER_I2C_ID_4,
	#endif

    I2C_DRIVER_NB_PORT_CONST,
};

//-------------------------------------------------------------------------------------------------

