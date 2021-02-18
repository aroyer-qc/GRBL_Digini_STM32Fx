//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_i2c_STMPE811.cpp
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
#ifdef DIGINI_USE_POINTING_DEVICE
#include "lib_class_i2c_STMPE811.h"
#include "lib_STM32F4_lcd_quarter_vga.h"
#include "lib_macro.h"
#include "lib_define.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define TOUCH_DELAY                 20

// ---- The IO Expanders chip IDs ----
#define STMPE811_ID                 0x0811

// ---- STMPE811 device register definition ----
// Identification registers
#define CHIP_ID                     0x00
#define ID_VER                      0x02

// General Control Registers
#define SYS_CTRL1                   0x03
#define SYS_CTRL2                   0x04
#define SPI_CFG                     0x08

// Interrupt Control register
#define INT_CTRL                    0x09
#define INT_EN                      0x0A
#define INT_STA                     0x0B
#define GPIO_INT_EN                 0x0C
#define GPIO_INT_STA                0x0D

// GPIO Registers
#define GPIO_SET_PIN                0x10
#define GPIO_CLR_PIN                0x11
#define GPIO_MP_STA                 0x12
#define GPIO_DIR                    0x13
#define GPIO_ED                     0x14
#define GPIO_RE                     0x15
#define GPIO_FE                     0x16
#define GPIO_AF                     0x17

// ADC Registers
#define ADC_INT_EN                  0x0E
#define ADC_INT_STA                 0x0F
#define ADC_CTRL1                   0x20
#define ADC_CTRL2                   0x21
#define ADC_CAPT                    0x22
#define ADC_DATA_CH0                0x30
#define ADC_DATA_CH1                0x32
#define ADC_DATA_CH2                0x34
#define ADC_DATA_CH3                0x36
#define ADC_DATA_CH4                0x38
#define ADC_DATA_CH5                0x3A
#define ADC_DATA_CH6                0x3B
#define ADC_DATA_CH7                0x3C

// TouchPanel Registers
#define TSC_CTRL                    0x40
#define TSC_CFG                     0x41
#define WDM_TR_X                    0x42
#define WDM_TR_Y                    0x44
#define WDM_BL_X                    0x46
#define WDM_BL_Y                    0x48
#define FIFO_TH                     0x4A
#define FIFO_STA                    0x4B
#define FIFO_SIZE                   0x4C
#define TSC_DATA_X                  0x4D
#define TSC_DATA_Y                  0x4F
#define TSC_DATA_Z                  0x51
#define TSC_DATA_XYZ                0x52
#define TSC_FRACTION_Z              0x56
#define TSC_DATA                    0x57
#define TSC_I_DRIVE                 0x58
#define TSC_SHIELD                  0x59
#define TSC_DATA_XYZ_NON_INC        0xD7

// ---- Global interrupt Enable bit ----
#define GIT_EN                      0x01

// ---- Global Interrupts definitions ----
#define GIT_GPIO                    0x80
#define GIT_ADC                     0x40
#define GIT_TEMP                    0x20
#define GIT_FE                      0x10
#define GIT_FF                      0x08
#define GIT_FOV                     0x04
#define GIT_FTH                     0x02
#define GIT_TOUCH                   0x01

// ---- Touch detection flag ----
#define TOUCH_DETECTED              0x80


//-------------------------------------------------------------------------------------------------
//
//   Class: CTouch
//
//
//   Description:   Class to handle Touch panel with I2C chip
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CTouch
//
//   Description:   Initializes the Touch panel
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CTouch::CTouch(CI2C* pI2C)
{
    m_pI2C               = pI2C;
    m_pDevice            = &I2C_DeviceInfo[I2C_DEVICE_STMPE811];
    m_IsCalibrated       = false;
    m_IsCorrectionEnable = false;
    this->Initialize();
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   none
//  Return:         none
//
//  Description:    Perform chip initialization
//
//  Note(s):        I2C and interrupt pin on processor are initialize in BSP
//
//-------------------------------------------------------------------------------------------------
bool CTouch::Initialize()
{
    if(this->IsOperational() == false)                                          // Read IO Expander ID
    {
      return false;
    }

    m_pI2C->LockToDevice(m_pDevice);

    // ---- Resets the IO Expander by Software (SYS_CTRL1, RESET bit). ----
    m_pI2C->Write(SYS_CTRL1,            (uint8_t)0x02);                         // Power Down the Touch controller
    OS_TaskDelay(TOUCH_DELAY);                                                  // wait for a delay to insure registers erasing
    m_pI2C->Write(SYS_CTRL1,            (uint8_t)0x00);                         // Power On after the power off => all registers are reinitialized

    m_pI2C->Write(GPIO_INT_EN,          (uint8_t)0x00);                         // No GPIO interrupt
    m_pI2C->Write(ADC_INT_EN,           (uint8_t)0x00);                         // No ADC interrupt

    // ---- Configures the touch Panel Controller (Single point detection) ----
    m_pI2C->Write(SYS_CTRL2,            (uint8_t)0x0C);                         // Enable ADC and TSC module
    m_pI2C->Write(ADC_CTRL1,            (uint8_t)0x48);                         // Select sample Time, bit number and ADC Reference
    m_pI2C->Write(ADC_CTRL2,            (uint8_t)0x01);
    m_pI2C->Write(GPIO_AF,              (uint8_t)0x00);                         // All pin are alternate function
    m_pI2C->Write(TSC_CFG,              (uint8_t)0x9A);
    m_pI2C->Write(FIFO_TH,              (uint8_t)0x01);
    m_pI2C->Write(FIFO_STA,             (uint8_t)0x01);
    m_pI2C->Write(FIFO_STA,             (uint8_t)0x00);
    m_pI2C->Write(TSC_FRACTION_Z,       (uint8_t)0x07);
    m_pI2C->Write(TSC_I_DRIVE,          (uint8_t)0x01);
    m_pI2C->Write(TSC_CTRL,             (uint8_t)0x01);

    // ---- Configures Interrupt ----
    m_pI2C->Write(INT_CTRL,             (uint8_t)0x01);                         // Active high, Level, Global Enable
    m_pI2C->Write(INT_EN,               (uint8_t)0x02);                         // IRQ on FIFO threshold and Touch detection
    m_pI2C->Write(INT_STA,              (uint8_t)0xFF);                         // Clear pending bit

    m_pI2C->UnlockFromDevice(m_pDevice);

    m_X             = 0;
    m_Y             = 0;
    m_IntX          = 0;
    m_IntY          = 0;

    return true;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:            GetXY
//
//  Parameter(s):   uint16_t* X
//                  uint16_t* Y
//
//  Return:         None
//
//  Description:    Return the X and Y point
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CTouch::TS_GetXY(uint16_t *X, uint16_t *Y)
{
    *X = m_X;
    *Y = m_Y;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SetTouchCorrection
//
//  Parameter(s):   sLinearEquation X_Correction
//                  sLinearEquation Y_Correction
//  Return:         none
//
//  Description:    Set the touch correction
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CTouch::SetTouchCorrection(sLinearEquation X_Correction, sLinearEquation Y_Correction)
{
    m_X_Correction = X_Correction;
    m_Y_Correction = Y_Correction;
    m_IsCalibrated = true;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           EnableTouchCorrection
//
//  Parameter(s):   Enable
//  Return:         none
//
//  Description:    Set the touch correction
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CTouch::EnableTouchCorrection(bool Enable)
{
    m_IsCorrectionEnable = m_IsCalibrated && Enable;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:            ProcessTouch
//
//  Parameter(s):   none
//  Return:         none
//
//  Description:    Process the touch point
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CTouch::ProcessTouch()

{
    uint8_t     FifoSize;
    uint32_t    X_Diff;
    uint32_t    Y_Diff;
    uint32_t    X;
    uint32_t    Y;
    uint8_t     Data[4];
    uint32_t    DataXYZ;

    m_pI2C->LockToDevice(m_pDevice);

    m_pI2C->Read(FIFO_SIZE, &FifoSize);

    while(FifoSize--)
    {
        m_pI2C->Read(TSC_DATA_XYZ_NON_INC, &Data, 4);
    }
    m_pI2C->UnlockFromDevice(m_pDevice);

    DataXYZ = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | (Data[3] << 0);
    X = (DataXYZ >> 20) & 0x00000FFF;
    Y = (DataXYZ >>  8) & 0x00000FFF;

    // First correction
    X =  (X * GRAFX_SIZE_X) >> 12;
    Y = (Y * GRAFX_SIZE_Y) >> 12;

    // Second correction
    if(m_IsCorrectionEnable)
    {
        X = (X *  m_X_Correction.Slope +  m_X_Correction.Y_Intercept)/1000;
        Y = (Y *  m_Y_Correction.Slope +  m_Y_Correction.Y_Intercept)/1000;
    }

    if(Y >= GRAFX_SIZE_Y)  Y = GRAFX_SIZE_Y - 1;
    if(X >= GRAFX_SIZE_X)  X = GRAFX_SIZE_X - 1;

    X_Diff = X > m_IntX ? (X - m_IntX): (m_IntX - X);
    Y_Diff = Y > m_IntY ? (Y - m_IntY): (m_IntY - Y);

    if(X_Diff + Y_Diff > 5)
    {
      m_IntX = X;
      m_IntY = Y;
    }

    m_X = m_IntX;                                                   // Update the X position
    m_Y = GRAFX_SIZE_Y - m_IntY;                                    // Update the Y position

    this->FifoReset();
    this->ClearIRQ_Status(IRQ_FLAG_FTH | IRQ_FLAG_TOUCH);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           FifoReset
//
//  Parameter(s):   none
//  Return:         bool            State
//
//  Description:    Checks if the IOE device is correctly configured and communicates correctly.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CTouch::FifoReset()
{
    m_pI2C->LockToDevice(m_pDevice);
    m_pI2C->Write(FIFO_STA, (uint8_t)0x01);                         // Clear the interrupt pending bit and enable the FIFO again
    m_pI2C->Write(FIFO_STA, (uint8_t)0x00);
    m_pI2C->UnlockFromDevice(m_pDevice);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           IsOperational
//
//  Parameter(s):   none
//  Return:         bool            State
//
//  Description:    Checks if the IOE device is correctly configured and communicates correctly.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
bool CTouch::IsOperational()
{
    uint16_t ID;

    this->m_pI2C->Read(CHIP_ID, &ID, m_pDevice);                        // Read device ID
    ID = swap16(ID);

    if(ID != (uint16_t)STMPE811_ID)                                     // Return Error if the ID is not correct
    {
        return false;
    }
    return true;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           SetIRQ_Enable
//
//  Parameter(s):   bool        State      State of the interupt
//
//                                              - Could be DEF_ENABLE or DEF_DISABLE.
//
//  Return:         none
//
//  Description:    Enables or disables the interrupt.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CTouch::SetIRQ_Enable(bool State)
{
    uint8_t Register;

    m_pI2C->LockToDevice(m_pDevice);
    m_pI2C->Read(INT_CTRL, &Register);                                  // Read the Interrupt Control register

    if(State != DEF_DISABLE)  Register |=  GIT_EN;                      // Set the global interrupts to be Enabled
    else                      Register &= ~GIT_EN;                      // Set the global interrupts to be Disabled

    m_pI2C->Write(INT_CTRL, Register);                                  // Write Back the Interrupt Control register
    m_pI2C->UnlockFromDevice(m_pDevice);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           GetIRQ_Status
//
//  Parameter(s):   uint8_t  IRQ_Flag       The interrupt source to be checked, could be:
//
//                             - IRQ_FLAG_ADC    : ADC interrupt
//                             - IRQ_FLAG_T_TEMP : Temperature Sensor interrupts
//                             - IRQ_FLAG__FE    : Touch Panel Controller FIFO Error interrupt
//                             - IRQ_FLAG__FF    : Touch Panel Controller FIFO Full interrupt
//                             - IRQ_FLAG__FOV   : Touch Panel Controller FIFO Overrun interrupt
//                             - IRQ_FLAG__FTH   : Touch Panel Controller FIFO Threshold interrupt
//                             - IRQ_FLAG__TOUCH : Touch Panel Controller Touch Detected interrupt
//
//  Return:         bool    Status          Status of the checked flag. Could be DEF_SET/DEF_RESET.
//
//  Description:    Checks the selected Global interrupt source pending bit
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
bool CTouch::GetIRQ_Status(uint8_t IRQ_Flag)
{
    uint8_t Register;

    m_pI2C->Read(INT_STA, &Register, m_pDevice);                    // Get the Interrupt status
    if((Register & (uint8_t)IRQ_Flag) != 0)
    {
        return true;
    }

  return false;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           ClearIRQ_Status
//
//  Parameter(s):   uint8_t  IRQ_Flag       The Global interrupt to be cleared, could be any
//                                          combination of the following values:
//
//                             - IRQ_FLAG_ADC    : ADC interrupt
//                             - IRQ_FLAG_T_TEMP : Temperature Sensor interrupts
//                             - IRQ_FLAG__FE    : Touch Panel Controller FIFO Error interrupt
//                             - IRQ_FLAG__FF    : Touch Panel Controller FIFO Full interrupt
//                             - IRQ_FLAG__FOV   : Touch Panel Controller FIFO Overrun interrupt
//                             - IRQ_FLAG__FTH   : Touch Panel Controller FIFO Threshold interrupt
//                             - IRQ_FLAG__TOUCH : Touch Panel Controller Touch Detected interrupt
//
//  Return:         none
//
//  Description:    Clears the selected Global interrupt pending bit(s)
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CTouch::ClearIRQ_Status(uint8_t IRQ_Flag)
{
    m_pI2C->Write(INT_STA, IRQ_Flag, m_pDevice);                    // Write 1 to the bits that have to be cleared
}

//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_POINTING_DEVICE
#endif // DIGINI_USE_GRAFX
