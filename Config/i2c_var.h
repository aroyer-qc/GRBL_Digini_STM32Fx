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

I2C_Info_t I2C_Info[NB_OF_I2C_DRIVER] =
{
  #if (I2C_DRIVER_SUPPORT_I2C1_CFG == DEF_ENABLED)
    {
        DRIVER_I2C1_ID,             // DRIVER_I2Cx_ID
        I2C1,                       // pI2Cx
        IO_I2C1_SCL,                // SCL
        IO_I2C1_SDA,                // SDA
      #if (I2C_DRIVER_SUPPORT_CLK_SELECTION_CFG == DEF_ENABLED)
        I2C1_SELECT_PLCK1,
      #endif
        RCC_APB1ENR_I2C1EN,         // RCC_APB1ENR
        0x00A01B5B,                 // Timing for this module
      #if (I2C_DRIVER_SUPPORT_ADVANCED_MODE_CFG == DEF_ENABLED)
        I2C_ADDRESSING_MODE_7_BIT,  // Addressing Mode
        I2C_DUAL_ADDRESS_DISABLE,   // Dual Address Mode
        0,                          // Own Address_1
        0,                          // Own Address_2
        0,                          // Own Address_2 Masks
        I2C_GENERAL_CALL_DISABLE,   // General Call Mode
        I2C_NO_STRETCH_DISABLE,     // No Stretch Mode
      #endif
        7,                          // PreempPrio
        I2C1_EV_IRQn,               // IRQn_Channel
        I2C1_ER_IRQn,               // IRQn_Channel
    },
  #endif

  #if (I2C_DRIVER_SUPPORT_I2C2_CFG == DEF_ENABLED)
    {
        DRIVER_I2C2_ID,             // DRIVER_I2Cx_ID
        I2C2,                       // pI2Cx
        IO_I2C2_SCL,                // SCL
        IO_I2C2_SDA,                // SDA
      #if (I2C_DRIVER_SUPPORT_CLK_SELECTION_CFG == DEF_ENABLED)
        I2C2_SELECT_PLCK1,
      #endif
        RCC_APB1ENR_I2C2EN,         // RCC_APB1ENR
        0x00A01B5B,                 // Timing for this module
      #if (I2C_DRIVER_SUPPORT_ADVANCED_MODE_CFG == DEF_ENABLED)
        I2C_ADDRESSING_MODE_7_BIT,  // Addressing Mode
        I2C_DUAL_ADDRESS_DISABLE,   // Dual Address Mode
        0,                          // Own Address_1
        0,                          // Own Address_2
        0,                          // Own Address_2 Masks
        I2C_GENERAL_CALL_DISABLE,   // General Call Mode
        I2C_NO_STRETCH_DISABLE,     // No Stretch Mode
      #endif
        7,                          // PreempPrio
        I2C2_EV_IRQn,               // IRQn_Channel
        I2C2_ER_IRQn,               // IRQn_Channel
    },
  #endif

  #if (I2C_DRIVER_SUPPORT_I2C3_CFG == DEF_ENABLED)
    {
        DRIVER_I2C3_ID,             // DRIVER_I2Cx_ID
        I2C3,                       // pI2Cx
        IO_I2C3_SCL,                // SCL
        IO_I2C3_SDA,                // SDA
      #if (I2C_DRIVER_SUPPORT_CLK_SELECTION_CFG == DEF_ENABLED)
        I2C3_SELECT_PLCK1,
      #endif
        RCC_APB1ENR_I2C3EN,         // RCC_APB1ENR
        0x00A01B5B,                 // Timing for this module
      #if (I2C_DRIVER_SUPPORT_ADVANCED_MODE_CFG == DEF_ENABLED)
        I2C_ADDRESSING_MODE_7_BIT,  // Addressing Mode
        I2C_DUAL_ADDRESS_DISABLE,   // Dual Address Mode
        0,                          // Own Address_1
        0,                          // Own Address_2
        0,                          // Own Address_2 Masks
        I2C_GENERAL_CALL_DISABLE,   // General Call Mode
        I2C_NO_STRETCH_DISABLE,     // No Stretch Mode
      #endif
        7,                          // PreempPrio
        I2C3_EV_IRQn,               // IRQn_Channel
        I2C3_ER_IRQn,               // IRQn_Channel
    },
  #endif

  #if (I2C_DRIVER_SUPPORT_I2C4_CFG == DEF_ENABLED)
    {
        DRIVER_I2C4_ID,             // DRIVER_I2Cx_ID
        I2C4,                       // pI2Cx
        IO_I2C4_SCL,                // SCL
        IO_I2C4_SDA,                // SDA
      #if (I2C_DRIVER_SUPPORT_CLK_SELECTION_CFG == DEF_ENABLED)
        I2C3_SELECT_PLCK1,
      #endif
        RCC_APB1ENR_I2C4EN,         // RCC_APB1ENR
        0x00A01B5B,                 // Timing for this module
      #if (I2C_DRIVER_SUPPORT_ADVANCED_MODE_CFG == DEF_ENABLED)
        I2C_ADDRESSING_MODE_7_BIT,  // Addressing Mode
        I2C_DUAL_ADDRESS_DISABLE,   // Dual Address Mode
        0,                          // Own Address_1
        0,                          // Own Address_2
        0,                          // Own Address_2 Masks
        I2C_GENERAL_CALL_DISABLE,   // General Call Mode
        I2C_NO_STRETCH_DISABLE,     // No Stretch Mode
      #endif
        7,                          // PreempPrio
        I2C4_EV_IRQn,               // IRQn_Channel
        I2C4_ER_IRQn,               // IRQn_Channel
    },
  #endif
};

class I2C_Driver myI2C_Control(DRIVER_I2C3_ID);
class I2C_Driver myI2C_External(DRIVER_I2C1_ID);

#else // I2C_DRIVER_GLOBAL

extern const I2C_Info_t I2C_Info[NB_OF_I2C_DRIVER];

extern class I2C_Driver myI2C_Control;
extern class I2C_Driver myI2C_External;

#endif // I2C_DRIVER_GLOBAL

//-------------------------------------------------------------------------------------------------
