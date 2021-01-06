//-------------------------------------------------------------------------------------------------
//
//  File :  i2c_var.h
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
//  Notes: I2C variables for STM32F746NG
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef I2C_DRIVER_GLOBAL

const I2C_PortInfo_t I2C_PortInfo[I2C_DRIVER_NB_PORT_CONST] =
{
  #if (I2C_DRIVER_SUPPORT_I2C1 == DEF_ENABLED)
	{
		I2C1,                       // I2Cx
        IO_I2C1_SCL,                // SCL
        IO_I2C1_SDA,                // SDA
		RCC_APB1ENR_I2C1EN,         // RCC_APB1ENR
		0x00A01B5B,                 // Timing for this module
		7,                          // PreempPrio
		I2C1_EV_IRQn,               // IRQn_Channel
		I2C1_ER_IRQn,               // IRQn_Channel
	},
  #endif

  #if (I2C_DRIVER_SUPPORT_I2C2 == DEF_ENABLED)
	{
		I2C2,                       // I2Cx
        IO_I2C2_SCL,                // SCL
        IO_I2C2_SDA,                // SDA
		RCC_APB1ENR_I2C2EN,         // RCC_APB1ENR
		0x00A01B5B,                 // Timing for this module
		7,                          // PreempPrio
		I2C2_EV_IRQn,               // IRQn_Channel
		I2C2_ER_IRQn,               // IRQn_Channel
	},
  #endif

  #if (I2C_DRIVER_SUPPORT_I2C3 == DEF_ENABLED)
	{
		I2C3,                       // I2Cx
        IO_I2C3_SCL,                // SCL
        IO_I2C3_SDA,                // SDA
		RCC_APB1ENR_I2C3EN,         // RCC_APB1ENR
		0x00A01B5B,                 // Timing for this module
		7,                          // PreempPrio
		I2C3_EV_IRQn,               // IRQn_Channel
		I2C3_ER_IRQn,               // IRQn_Channel
	},
  #endif

  #if (I2C_DRIVER_SUPPORT_I2C4 == DEF_ENABLED)
	{
		I2C4,                       // I2Cx
        IO_I2C4_SCL,                // SCL
        IO_I2C4_SDA,                // SDA
		RCC_APB1ENR_I2C4EN,         // RCC_APB1ENR
		0x00A01B5B,                 // Timing for this module
		7,                          // PreempPrio
		I2C4_EV_IRQn,               // IRQn_Channel
		I2C4_ER_IRQn,               // IRQn_Channel
	},
  #endif
};

class I2C_Driver myI2C_Control(&I2C_PortInfo[DRIVER_I2C_ID_3]);
class I2C_Driver myI2C_External(&I2C_PortInfo[DRIVER_I2C_ID_1]);

#else // I2C_DRIVER_GLOBAL

extern const I2C_PortInfo_t I2C_PortInfo[I2C_DRIVER_NB_PORT_CONST];

extern class I2C_Driver myI2C_Control;
extern class I2C_Driver myI2C_External;

#endif // I2C_DRIVER_GLOBAL

//-------------------------------------------------------------------------------------------------
