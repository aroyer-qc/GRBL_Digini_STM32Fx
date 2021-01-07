//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_i2c_FT5336.cpp
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
//#include "grafx_cfg.h"
#define LIB_FT5336_GLOBAL
#include "lib_grafx.h"
#undef  LIB_FT5336_GLOBAL

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

// I2C Slave address of touchscreen FocalTech FT5336
#define FT5336_I2C_SLAVE_ADDRESS                    ((uint8_t)0x38)

// Possible values of global variable 'TS_I2C_Initialized'
#define FT5336_I2C_NOT_INITIALIZED                  ((uint8_t)0x00)
#define FT5336_I2C_INITIALIZED                      ((uint8_t)0x01)

#define FT5336_MAX_DETECTABLE_TOUCH                 ((uint8_t)0x05)         // Max detectable simultaneous touch

// Definitions for FT5336 I2C register addresses on 8 bit
#define FT5336_DEV_MODE_REG                         ((uint8_t)0x00)         // Current mode register of the FT5336 (R/W)

    // Possible values of FT5336_DEV_MODE_REG
    #define FT5336_DEV_MODE_WORKING                 ((uint8_t)0x00)
    #define FT5336_DEV_MODE_FACTORY                 ((uint8_t)0x04)

    #define FT5336_DEV_MODE_MASK                    ((uint8_t)0x07)
    #define FT5336_DEV_MODE_SHIFT                   ((uint8_t)0x04)

// Gesture ID register
#define FT5336_GEST_ID_REG                          ((uint8_t)0x01)

    // Possible values of FT5336_GEST_ID_REG
    #define FT5336_GEST_ID_NO_GESTURE               ((uint8_t)0x00)
    #define FT5336_GEST_ID_MOVE_UP                  ((uint8_t)0x10)
    #define FT5336_GEST_ID_MOVE_RIGHT               ((uint8_t)0x14)
    #define FT5336_GEST_ID_MOVE_DOWN                ((uint8_t)0x18)
    #define FT5336_GEST_ID_MOVE_LEFT                ((uint8_t)0x1C)
    #define FT5336_GEST_ID_SINGLE_CLICK             ((uint8_t)0x20)
    #define FT5336_GEST_ID_DOUBLE_CLICK             ((uint8_t)0x22)
    #define FT5336_GEST_ID_ROTATE_CLOCKWISE         ((uint8_t)0x28)
    #define FT5336_GEST_ID_ROTATE_C_CLOCKWISE       ((uint8_t)0x29)
    #define FT5336_GEST_ID_ZOOM_IN                  ((uint8_t)0x40)
    #define FT5336_GEST_ID_ZOOM_OUT                 ((uint8_t)0x49)

// Touch Data Status register : gives number of active touch points (0..5)
#define FT5336_TD_STAT_REG                          ((uint8_t)0x02)

    // Values related to FT5336_TD_STAT_REG
    #define FT5336_TD_STAT_MASK                     ((uint8_t)0x0F)
    #define FT5336_TD_STAT_SHIFT                    ((uint8_t)0x00)

// Values Pn_XH and Pn_YH related
#define FT5336_TOUCH_EVT_FLAG_PRESS_DOWN            ((uint8_t)0x00)
#define FT5336_TOUCH_EVT_FLAG_LIFT_UP               ((uint8_t)0x01)
#define FT5336_TOUCH_EVT_FLAG_CONTACT               ((uint8_t)0x02)
#define FT5336_TOUCH_EVT_FLAG_NO_EVENT              ((uint8_t)0x03)

#define FT5336_TOUCH_EVT_FLAG_SHIFT                 ((uint8_t)0x06)
#define FT5336_TOUCH_EVT_FLAG_MASK                  ((uint8_t)(3 << FT5336_TOUCH_EVT_FLAG_SHIFT))

#define FT5336_TOUCH_POS_MSB_MASK                   ((uint8_t)0x0F)
//#define FT5336_TOUCH_POS_MSB_SHIFT                  ((uint8_t)0x00)

// Values Pn_XL and Pn_YL related
//#define FT5336_TOUCH_POS_LSB_MASK                   ((uint8_t)0xFF)
//#define FT5336_TOUCH_POS_LSB_SHIFT                  ((uint8_t)0x00)

#define FT5336_P1_XH_REG                            ((uint8_t)0x03)
#define FT5336_P1_XL_REG                            ((uint8_t)0x04)
#define FT5336_P1_YH_REG                            ((uint8_t)0x05)
#define FT5336_P1_YL_REG                            ((uint8_t)0x06)

// Touch Pressure register value (R)
#define FT5336_P1_WEIGHT_REG                        ((uint8_t)0x07)

// Values Pn_WEIGHT related
#define FT5336_TOUCH_WEIGHT_MASK                    ((uint8_t)0xFF)
#define FT5336_TOUCH_WEIGHT_SHIFT                   ((uint8_t)0x00)

// Touch area register
#define FT5336_P1_MISC_REG                          ((uint8_t)0x08)

// Values related to FT5336_Pn_MISC_REG
#define FT5336_TOUCH_AREA_MASK                      ((uint8_t)(0x04 << 4))
#define FT5336_TOUCH_AREA_SHIFT                     ((uint8_t)0x04)

#define FT5336_P2_XH_REG                            ((uint8_t)0x09)
#define FT5336_P2_XL_REG                            ((uint8_t)0x0A)
#define FT5336_P2_YH_REG                            ((uint8_t)0x0B)
#define FT5336_P2_YL_REG                            ((uint8_t)0x0C)
#define FT5336_P2_WEIGHT_REG                        ((uint8_t)0x0D)
#define FT5336_P2_MISC_REG                          ((uint8_t)0x0E)

#define FT5336_P3_XH_REG                            ((uint8_t)0x0F)
#define FT5336_P3_XL_REG                            ((uint8_t)0x10)
#define FT5336_P3_YH_REG                            ((uint8_t)0x11)
#define FT5336_P3_YL_REG                            ((uint8_t)0x12)
#define FT5336_P3_WEIGHT_REG                        ((uint8_t)0x13)
#define FT5336_P3_MISC_REG                          ((uint8_t)0x14)

#define FT5336_P4_XH_REG                            ((uint8_t)0x15)
#define FT5336_P4_XL_REG                            ((uint8_t)0x16)
#define FT5336_P4_YH_REG                            ((uint8_t)0x17)
#define FT5336_P4_YL_REG                            ((uint8_t)0x18)
#define FT5336_P4_WEIGHT_REG                        ((uint8_t)0x19)
#define FT5336_P4_MISC_REG                          ((uint8_t)0x1A)

#define FT5336_P5_XH_REG                            ((uint8_t)0x1B)
#define FT5336_P5_XL_REG                            ((uint8_t)0x1C)
#define FT5336_P5_YH_REG                            ((uint8_t)0x1D)
#define FT5336_P5_YL_REG                            ((uint8_t)0x1E)
#define FT5336_P5_WEIGHT_REG                        ((uint8_t)0x1F)
#define FT5336_P5_MISC_REG                          ((uint8_t)0x20)

#define FT5336_P6_XH_REG                            ((uint8_t)0x21)
#define FT5336_P6_XL_REG                            ((uint8_t)0x22)
#define FT5336_P6_YH_REG                            ((uint8_t)0x23)
#define FT5336_P6_YL_REG                            ((uint8_t)0x24)
#define FT5336_P6_WEIGHT_REG                        ((uint8_t)0x25)
#define FT5336_P6_MISC_REG                          ((uint8_t)0x26)

#define FT5336_P7_XH_REG                            ((uint8_t)0x27)
#define FT5336_P7_XL_REG                            ((uint8_t)0x28)
#define FT5336_P7_YH_REG                            ((uint8_t)0x29)
#define FT5336_P7_YL_REG                            ((uint8_t)0x2A)
#define FT5336_P7_WEIGHT_REG                        ((uint8_t)0x2B)
#define FT5336_P7_MISC_REG                          ((uint8_t)0x2C)

#define FT5336_P8_XH_REG                            ((uint8_t)0x2D)
#define FT5336_P8_XL_REG                            ((uint8_t)0x2E)
#define FT5336_P8_YH_REG                            ((uint8_t)0x2F)
#define FT5336_P8_YL_REG                            ((uint8_t)0x30)
#define FT5336_P8_WEIGHT_REG                        ((uint8_t)0x31)
#define FT5336_P8_MISC_REG                          ((uint8_t)0x32)

#define FT5336_P9_XH_REG                            ((uint8_t)0x33)
#define FT5336_P9_XL_REG                            ((uint8_t)0x34)
#define FT5336_P9_YH_REG                            ((uint8_t)0x35)
#define FT5336_P9_YL_REG                            ((uint8_t)0x36)
#define FT5336_P9_WEIGHT_REG                        ((uint8_t)0x37)
#define FT5336_P9_MISC_REG                          ((uint8_t)0x38)

#define FT5336_P10_XH_REG                           ((uint8_t)0x39)
#define FT5336_P10_XL_REG                           ((uint8_t)0x3A)
#define FT5336_P10_YH_REG                           ((uint8_t)0x3B)
#define FT5336_P10_YL_REG                           ((uint8_t)0x3C)
#define FT5336_P10_WEIGHT_REG                       ((uint8_t)0x3D)
#define FT5336_P10_MISC_REG                         ((uint8_t)0x3E)

#define FT5336_TH_GROUP_REG                         ((uint8_t)0x80) // Threshold for touch detection
#define FT5336_THRESHOLD_MASK                       ((uint8_t)0xFF) // Values FT5336_TH_GROUP_REG : threshold related
#define FT5336_THRESHOLD_SHIFT                      ((uint8_t)0x00)
#define FT5336_TH_DIFF_REG                          ((uint8_t)0x85) // Filter function coefficients
#define FT5336_CTRL_REG                             ((uint8_t)0x86) // Control register

// Values related to FT5336_CTRL_REG
#define FT5336_CTRL_KEEP_ACTIVE_MODE                ((uint8_t)0x00) // Will keep the Active mode when there is no touching
#define FT5336_CTRL_KEEP_AUTO_SWITCH_MONITOR_MODE   ((uint8_t)0x01) // Switching from Active mode to Monitor mode automatically when there is no touching

#define FT5336_TIMEENTERMONITOR_REG                 ((uint8_t)0x87) // The time period of switching from Active mode to Monitor mode when there is no touching
#define FT5336_PERIODACTIVE_REG                     ((uint8_t)0x88) // Report rate in Active mode
#define FT5336_PERIODMONITOR_REG                    ((uint8_t)0x89) // Report rate in Monitor mode
#define FT5336_RADIAN_VALUE_REG                     ((uint8_t)0x91) // The value of the minimum allowed angle while Rotating gesture mode
#define FT5336_OFFSET_LEFT_RIGHT_REG                ((uint8_t)0x92) // Maximum offset while Moving Left and Moving Right gesture
#define FT5336_OFFSET_UP_DOWN_REG                   ((uint8_t)0x93) // Maximum offset while Moving Up and Moving Down gesture
#define FT5336_DISTANCE_LEFT_RIGHT_REG              ((uint8_t)0x94) // Minimum distance while Moving Left and Moving Right gesture
#define FT5336_DISTANCE_UP_DOWN_REG                 ((uint8_t)0x95) // Minimum distance while Moving Up and Moving Down gesture
#define FT5336_DISTANCE_ZOOM_REG                    ((uint8_t)0x96) // Maximum distance while Zoom In and Zoom Out gesture
#define FT5336_LIB_VER_H_REG                        ((uint8_t)0xA1) // High 8-bit of LIB Version info
#define FT5336_LIB_VER_L_REG                        ((uint8_t)0xA2) // Low 8-bit of LIB Version info
#define FT5336_CIPHER_REG                           ((uint8_t)0xA3) // Chip Selecting
#define FT5336_GMODE_REG                            ((uint8_t)0xA4) // Interrupt mode register (used when in interrupt mode)

#define FT5336_G_MODE_INTERRUPT_MASK                ((uint8_t)0x03)
#define FT5336_G_MODE_INTERRUPT_SHIFT               ((uint8_t)0x00)

#define FT5336_G_MODE_INTERRUPT_POLLING             ((uint8_t)0x00) // Possible values of FT5336_GMODE_REG
#define FT5336_G_MODE_INTERRUPT_TRIGGER             ((uint8_t)0x01)

#define FT5336_PWR_MODE_REG                         ((uint8_t)0xA5) // Current power mode the FT5336 system is in (R)
#define FT5336_FIRMID_REG                           ((uint8_t)0xA6) // FT5336 firmware version
#define FT5336_CHIP_ID_REG                          ((uint8_t)0xA8) // FT5336 Chip identification register
    #define FT5336_ID_VALUE                         ((uint8_t)0x51) // Possible values of FT5336_CHIP_ID_REG

#define FT5336_STATE_REG                            ((uint8_t)0xBC) // Current operating mode the FT5336 system is in (R)

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   void* pArg          Pointer on the type of driver use by this class
//
//  Return:         SystemState_e
//
//  Description:    Initialize the FT5336 communication bus
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e FT5336::Initialize(void* pArg)
{
    m_pI2C = (I2C_Driver*)pArg;

    nOS_Sleep(2000);        // Wait at least 200ms after power up before accessing registers TRSI timing
    return (this->ReadID() == FT5336_ID_VALUE) ? SYS_READY : SYS_FAIL;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Reset
//
//  Parameter(s):   None
//
//  Return:         None
//
//  Description:    Software Reset the FT5336.
//
//  Note(s):        No software reset sequence available in FT5336 IC
//
//-------------------------------------------------------------------------------------------------
void FT5336::Reset(void)
{
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ReadID
//
//  Parameter(s):   None
//
//  Return:         uint16_t    The Device ID (two bytes).
//
//  Description:    Read the FT5336 device ID, pre initialize I2C in case of need to be able to
//                  read the FT5336 device ID, and verify this is a FT5336.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
uint16_t FT5336::ReadID(void)
{
    uint8_t ID;
    uint8_t ReadAttempts;
    bool    FoundDevice;
    uint8_t Register;

    ID          = 0;                    // In case we cannot read the ID
    FoundDevice = false;                // Devive not found by default

    // At maximum 4 attempts to read ID : exit at first finding of the searched device ID
    for(ReadAttempts = 0; ((ReadAttempts < 3) && (FoundDevice == false)); ReadAttempts++)
    {
        // Read register FT5336_CHIP_ID_REG as DeviceID detection
        Register = FT5336_CHIP_ID_REG;
        m_pI2C->ReadRegister(Register, &ID, sizeof(uint8_t), FT5336_I2C_SLAVE_ADDRESS);
        if(ID == FT5336_ID_VALUE)       // Found the searched device ID?
        {
            FoundDevice = true;         // Set device as found
        }
    }

    return ID;                          // Return the device ID value
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           TS_DetectEvent
//
//  Parameter(s):   None
//
//  Return:         uint8_t     Number of active events detected (can be 0, 1 or 2).
//
//  Description:    Return if there is Events detected or not. Try to detect new events and
//                  forget the old ones (reset internal global variables).
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
uint8_t FT5336::DetectEvent(void)
{
    uint8_t Event;

    // Read register FT5336_TD_STAT_REG to check number of touches detection
    m_pI2C->ReadRegister(FT5336_TD_STAT_REG, &Event, sizeof(uint8_t), FT5336_I2C_SLAVE_ADDRESS);
    Event &= FT5336_TD_STAT_MASK;

    if(Event > FT5336_MAX_DETECTABLE_TOUCH)
    {
        // If invalid number of event detected, set it to zero
        Event = 0;
    }

    // Update FT5336 driver internal global : current number of active touches
    m_CurrentActiveEventNumber = Event;

    // Reset current active event index on which to work on
    m_CurrentActiveEventIndex = 0;

    return Event;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           GetXY
//
//  Parameter(s):   Cartesian_t* pCartesian     Pointer to Cartesian struct containing X & Y
//
//  Return:         None
//
//  Description:    Get the event X and Y positions values. Manage multi event thanks to
//                  event index variable 'm_CurrentActiveEventIndex'.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void FT5336::GetXY(Cartesian_t* pCartesian)
{
    uint8_t  ReadData = 0;
    uint8_t  regAddressXLow  = 0;
    uint8_t  regAddressXHigh = 0;
    uint8_t  regAddressYLow  = 0;
    uint8_t  regAddressYHigh = 0;

    if(m_CurrentActiveEventIndex < m_CurrentActiveEventNumber)
    {
        switch(m_CurrentActiveEventIndex)
        {
            case 0:
            {
                regAddressXLow  = FT5336_P1_XL_REG;
                regAddressXHigh = FT5336_P1_XH_REG;
                regAddressYLow  = FT5336_P1_YL_REG;
                regAddressYHigh = FT5336_P1_YH_REG;
                break;
            }

            case 1:
            {
                regAddressXLow  = FT5336_P2_XL_REG;
                regAddressXHigh = FT5336_P2_XH_REG;
                regAddressYLow  = FT5336_P2_YL_REG;
                regAddressYHigh = FT5336_P2_YH_REG;
                break;
            }

            case 2:
            {
                regAddressXLow  = FT5336_P3_XL_REG;
                regAddressXHigh = FT5336_P3_XH_REG;
                regAddressYLow  = FT5336_P3_YL_REG;
                regAddressYHigh = FT5336_P3_YH_REG;
                break;
            }

            case 3:
            {
                regAddressXLow  = FT5336_P4_XL_REG;
                regAddressXHigh = FT5336_P4_XH_REG;
                regAddressYLow  = FT5336_P4_YL_REG;
                regAddressYHigh = FT5336_P4_YH_REG;
                break;
            }

            case 4:
            {
                regAddressXLow  = FT5336_P5_XL_REG;
                regAddressXHigh = FT5336_P5_XH_REG;
                regAddressYLow  = FT5336_P5_YL_REG;
                regAddressYHigh = FT5336_P5_YH_REG;
                break;
            }

            case 5:
            {
                regAddressXLow  = FT5336_P6_XL_REG;
                regAddressXHigh = FT5336_P6_XH_REG;
                regAddressYLow  = FT5336_P6_YL_REG;
                regAddressYHigh = FT5336_P6_YH_REG;
                break;
            }

            case 6:
            {
                regAddressXLow  = FT5336_P7_XL_REG;
                regAddressXHigh = FT5336_P7_XH_REG;
                regAddressYLow  = FT5336_P7_YL_REG;
                regAddressYHigh = FT5336_P7_YH_REG;
                break;
            }

            case 7:
            {
                regAddressXLow  = FT5336_P8_XL_REG;
                regAddressXHigh = FT5336_P8_XH_REG;
                regAddressYLow  = FT5336_P8_YL_REG;
                regAddressYHigh = FT5336_P8_YH_REG;
                break;
            }

            case 8:
            {
                regAddressXLow  = FT5336_P9_XL_REG;
                regAddressXHigh = FT5336_P9_XH_REG;
                regAddressYLow  = FT5336_P9_YL_REG;
                regAddressYHigh = FT5336_P9_YH_REG;
                break;
            }

            case 9:
            {
                regAddressXLow  = FT5336_P10_XL_REG;
                regAddressXHigh = FT5336_P10_XH_REG;
                regAddressYLow  = FT5336_P10_YL_REG;
                regAddressYHigh = FT5336_P10_YH_REG;
                break;
            }

            default:
                break;

        }

        m_pI2C->LockToDevice(FT5336_I2C_SLAVE_ADDRESS);

        // Read low part of X position
        m_pI2C->ReadRegister(regAddressXLow, &ReadData, sizeof(uint8_t));
      #ifndef GRAFX_PDI_SWAP_XY
        pCartesian->X = ReadData;
      #else
        pCartesian->Y = ReadData;
      #endif
        // Read high part of X position
        m_pI2C->ReadRegister(regAddressXHigh, &ReadData, sizeof(uint8_t));
      #ifndef GRAFX_PDI_SWAP_XY
        pCartesian->X |= (ReadData & FT5336_TOUCH_POS_MSB_MASK) << 8;
      #else
        pCartesian->Y |= (ReadData & FT5336_TOUCH_POS_MSB_MASK) << 8;
      #endif

        // Read low part of Y position
        m_pI2C->ReadRegister(regAddressYLow, &ReadData, sizeof(uint8_t));
      #ifndef GRAFX_PDI_SWAP_XY
        pCartesian->Y = ReadData;
      #else
        pCartesian->X = ReadData;
      #endif

        // Read high part of Y position
        m_pI2C->ReadRegister(regAddressYHigh, &ReadData, sizeof(uint8_t));
      #ifndef GRAFX_PDI_SWAP_XY
        pCartesian->Y |= (ReadData & FT5336_TOUCH_POS_MSB_MASK) << 8;
      #else
        pCartesian->X |= (ReadData & FT5336_TOUCH_POS_MSB_MASK) << 8;
      #endif

        m_pI2C->UnlockFromDevice(FT5336_I2C_SLAVE_ADDRESS);

        m_CurrentActiveEventIndex++; // next call will work on next touch

    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           EnableIT
//
//  Parameter(s):   None
//
//  Return:         SystemState_e
//
//  Description:    Configure the FT5336 device to generate IT on given INT pin connected to MCU as
//                  EXTI.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e FT5336::EnableIT(void)
{
    uint8_t WriteBuffer[2];

    // Set interrupt trigger mode in FT5336_GMODE_REG
    WriteBuffer[0] = (FT5336_G_MODE_INTERRUPT_TRIGGER & (FT5336_G_MODE_INTERRUPT_MASK >> FT5336_G_MODE_INTERRUPT_SHIFT)) << FT5336_G_MODE_INTERRUPT_SHIFT;
    return m_pI2C->Transfer(FT5336_GMODE_REG, 1, &WriteBuffer[0], 1, 0, 0, FT5336_I2C_SLAVE_ADDRESS);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DisableIT
//
//  Parameter(s):   None
//
//  Return:         SystemState_e
//
//  Description:    Configure the FT5336 device to stop generating IT on the given INT pin
//                  connected to MCU as EXTI.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e FT5336::DisableIT(void)
{
    uint8_t WriteBuffer[2];

    // Reset interrupt trigger mode in FT5336_GMODE_REG
    WriteBuffer[1] = (FT5336_G_MODE_INTERRUPT_POLLING & (FT5336_G_MODE_INTERRUPT_MASK >> FT5336_G_MODE_INTERRUPT_SHIFT)) << FT5336_G_MODE_INTERRUPT_SHIFT;
    return m_pI2C->Transfer(FT5336_GMODE_REG, 1, &WriteBuffer[0], 1, 0, 0, FT5336_I2C_SLAVE_ADDRESS);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ITStatus
//
//  Parameter(s):   None
//
//  Return:         uint8_t     TS interrupts status : always return 0 here
//
//  Description:    Get IT status from FT5336 interrupt status registers Should be called Following
//                  an EXTI coming to the MCU to know the detailed reason of the interrupt.
//
//  Note(s):        Always return 0 as feature not applicable to FT5336
//
//-------------------------------------------------------------------------------------------------
SystemState_e FT5336::ITStatus(void)
{
    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ClearIT
//
//  Parameter(s):   None
//
//  Return:         None
//
//  Description:    Clear IT status in FT5336 interrupt status clear registers Should be called
//                  following an EXTI coming to the MCU.
//
//  Note(s):        This feature is not applicable to FT5336.
//
//-------------------------------------------------------------------------------------------------
SystemState_e FT5336::ClearIT(void)
{
    return SYS_READY;
}

/**** NEW FEATURES enabled when Multi-touch support is enabled ****/

#ifdef DIGINI_USE_PDI_MULTI_EVENT

//-------------------------------------------------------------------------------------------------
//
//  Name:           GetGestureID
//
//  Parameter(s):   None
//
//  Return:         ServiceEvent_e
//
//  Description:    Get the last event gesture identification (zoom, move up/down...).
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
ServiceEvent_e FT5336::GetGestureID(void)
{
    uint8_t        ReadData;
    ServiceEvent_e GestureID;

    // TODO handle return for error

    // Read gesture ID register
    m_pI2C->ReadRegister(FT5336_GEST_ID_REG, &ReadData, sizeof(uint8_t), FT5336_I2C_SLAVE_ADDRESS);

    // Translate FT5336 ID to Digini ID
    switch(ReadData)
    {
        case FT5336_GEST_ID_NO_GESTURE: GestureID = SERVICE_NO_GESTURE;  break;
        case FT5336_GEST_ID_MOVE_UP:    GestureID = SERVICE_MOVE_UP;     break;
        case FT5336_GEST_ID_MOVE_RIGHT: GestureID = SERVICE_MOVE_RIGHT;  break;
        case FT5336_GEST_ID_MOVE_DOWN:  GestureID = SERVICE_MOVE_DOWN;   break;
        case FT5336_GEST_ID_MOVE_LEFT:  GestureID = SERVICE_MOVE_LEFT;   break;
        case FT5336_GEST_ID_ZOOM_IN:    GestureID = SERVICE_ZOOM_IN;     break;
        case FT5336_GEST_ID_ZOOM_OUT:   GestureID = SERVICE_ZOOM_OUT;    break;
        default:                        GestureID = SERVICE_INVALID;     break;
    }

    return GestureID;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:
//
//  Parameter(s):   uint32_t  EventIdx  Passed index of the event (0..1) on which we want to
//                                      get the detailed information.
//                  uint32_t* pWeight   Pointer to to get the weight information of 'EventIdx'.
//                  uint32_t* pArea     Pointer to to get the sub-area information of 'EventIdx'.
//                  uint32_t* pEvent    Pointer to to get the event information of 'EventIdx'.
//
//  Return:         None
//
//  Description:    Get the event detailed informations on touch number 'EventIdx' (0..1)
//                      This event detailed information contains:
//                          - weight that was applied to this event
//                          - sub-area of the event in the pointing device zone
//                          - type of linked to the event (press down, lift up, ...)
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void FT5336::GetEventInfo(uint32_t EventIdx, uint32_t * pWeight, uint32_t * pArea, uint32_t * pEvent)
{
    uint8_t             ReadData = 0;
    uint8_t             regAddressXHigh;
    uint8_t             regAddressPWeight;
    uint8_t             regAddressPMisc;

    if(EventIdx < m_CurrentActiveEventNumber)
    {
        switch(EventIdx)
        {
            case 0:
            {
                regAddressXHigh   = FT5336_P1_XH_REG;
                regAddressPWeight = FT5336_P1_WEIGHT_REG;
                regAddressPMisc   = FT5336_P1_MISC_REG;
                break;
            }

            case 1:
            {
                regAddressXHigh   = FT5336_P2_XH_REG;
                regAddressPWeight = FT5336_P2_WEIGHT_REG;
                regAddressPMisc   = FT5336_P2_MISC_REG;
                break;
            }

            case 2:
            {
                regAddressXHigh   = FT5336_P3_XH_REG;
                regAddressPWeight = FT5336_P3_WEIGHT_REG;
                regAddressPMisc   = FT5336_P3_MISC_REG;
                break;
            }

            case 3:
            {
                regAddressXHigh   = FT5336_P4_XH_REG;
                regAddressPWeight = FT5336_P4_WEIGHT_REG;
                regAddressPMisc   = FT5336_P4_MISC_REG;
                break;
            }

            case 4:
            {
                regAddressXHigh   = FT5336_P5_XH_REG;
                regAddressPWeight = FT5336_P5_WEIGHT_REG;
                regAddressPMisc   = FT5336_P5_MISC_REG;
                break;
            }

            case 5:
            {
                regAddressXHigh   = FT5336_P6_XH_REG;
                regAddressPWeight = FT5336_P6_WEIGHT_REG;
                regAddressPMisc   = FT5336_P6_MISC_REG;
                break;
            }

            case 6:
            {
                regAddressXHigh   = FT5336_P7_XH_REG;
                regAddressPWeight = FT5336_P7_WEIGHT_REG;
                regAddressPMisc   = FT5336_P7_MISC_REG;
                break;
            }

            case 7:
            {
                regAddressXHigh   = FT5336_P8_XH_REG;
                regAddressPWeight = FT5336_P8_WEIGHT_REG;
                regAddressPMisc   = FT5336_P8_MISC_REG;
                break;
            }

            case 8:
            {
                regAddressXHigh   = FT5336_P9_XH_REG;
                regAddressPWeight = FT5336_P9_WEIGHT_REG;
                regAddressPMisc   = FT5336_P9_MISC_REG;
                break;
            }

            case 9:
            {
                regAddressXHigh   = FT5336_P10_XH_REG;
                regAddressPWeight = FT5336_P10_WEIGHT_REG;
                regAddressPMisc   = FT5336_P10_MISC_REG;
                break;
            }

            default:
            {
                break;
            }
        }

        // TODO handle error return
        m_pI2C->LockToDevice(FT5336_I2C_SLAVE_ADDRESS);

        // Read Event Id of event index
        m_pI2C->Transfer(regAddressXHigh, sizeof(uint8_t), 0, 0, &ReadData, sizeof(uint8_t));
        *pEvent = (ReadData & FT5336_TOUCH_EVT_FLAG_MASK) >> FT5336_TOUCH_EVT_FLAG_SHIFT;

        // Read weight of event index
        //m_pI2C->Transfer(&regAddressPWeight, sizeof(uint8_t), &ReadData, sizeof(uint8_t));
        m_pI2C->Transfer(regAddressPWeight, sizeof(uint8_t), 0, 0, &ReadData, sizeof(uint8_t));
        *pWeight = (ReadData & FT5336_TOUCH_WEIGHT_MASK) >> FT5336_TOUCH_WEIGHT_SHIFT;

        // Read area of event index
        m_pI2C->Transfer(regAddressPMisc, sizeof(uint8_t), 0, 0, &ReadData, sizeof(uint8_t));
        *pArea = (ReadData & FT5336_TOUCH_AREA_MASK) >> FT5336_TOUCH_AREA_SHIFT;

        m_pI2C->UnlockFromDevice(FT5336_I2C_SLAVE_ADDRESS);

    }
}

#endif // DIGINI_USE_PDI_MULTI_EVENT

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_POINTING_DEVICE
#endif // DIGINI_USE_GRAFX
