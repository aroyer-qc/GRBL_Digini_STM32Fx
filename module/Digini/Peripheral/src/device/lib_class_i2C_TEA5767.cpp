//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_i2c_TEA5767.cpp
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

#ifdef DIGINI_USE_GRAFX
#ifdef DIGINI_USE_I2C

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#define LIB_TEA5767_GLOBAL
#include "lib_class_i2c_TEA5767.h"
#undef  LIB_TEA5767_GLOBAL

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

// ----- Definitions for the Wire communication

// might be define BSP
//#define TEA5767_I2C_ADR        0x60

// ----- Radio chip specific definitions including the registers

#define QUARTZ              32768
#define FILTER              225000

// Define the registers

#define REG_1               0x00
#define REG_1_MUTE          0x80
#define REG_1_SM            0x40
#define REG_1_PLL           0x3F

#define REG_2               0x01
#define REG_2_PLL           0xFF

#define REG_3               0x02
#define REG_3_MS            0x08
#define REG_3_SSL_LO        0x20
#define REG_3_SSL_MID       0x40
#define REG_3_SSL_HI        0x60
#define REG_3_SUD           0x80

#define REG_4               0x03
#define REG_4_SMUTE         0x08
#define REG_4_XTAL          0x10
#define REG_4_BL            0x20
#define REG_4_STBY          0x40

#define REG_5               0x04
#define REG_5_PLLREF        0x80
#define REG_5_DTC           0x40

#define STAT_3              0x02
#define STAT_3_STEREO       0x80

#define STAT_4              0x03
#define STAT_4_ADC          0xF0

// // Use this define to setup European FM specific settings in the chip.
#define IN_EUROPE

//-------------------------------------------------------------------------------------------------
//
//   Class: TEA5767
//
//
//   Description:   Class to handle radio TEA5767
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   TEA5767
//
//   Description:   Initializes the Radio chip
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
TEA5767::TEA5767(CI2C* pI2C)
{
    m_pI2C               = pI2C;
    m_pDevice            = &I2C_DeviceInfo[I2C_DEVICE_TEA5767];
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
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
bool TEA5767::Initialize()
{
    if(this->IsOperational() == false)                                          // Read IO TEA5767 ID
    {
      return false;
    }

    m_Registers[REG_1] = 0x00;
    m_Registers[REG_2] = 0x00;
    m_Registers[REG_3] = REG_3_SSL_MID;
    m_Registers[REG_4] = REG_4_XTAL | REG_4_SMUTE;
    m_Registers[REG_5] = REG_5_DTC; // 75 ms Europe setup

    this-SaveRegisters(REG_0, 5);

    return true;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           SetMono
//
//  Parameter(s):   bool SwitchOn
//
//  Return:         None
//
//  Description:    Force mono receiving mode.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void TEA5767::SetMono(bool SwitchOn)
{
    m_Mono = switchOn;

    if(switchOn) m_Registers[REG_3] |=  REG_3_MS;
    else         m_Registers[REG_3] &= ~REG_3_MS;

    this->SaveRegisters(REG_3, 1);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           SetMute
//
//  Parameter(s):   bool SwitchOn
//
//  Return:         none
//
//  Description:    Force mute mode.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void TEA5767::SetMute(bool SwitchOn)
{
    m_Mute = switchOn;

    if(switchOn) m_Registers[REG_1] |=  REG_1_MUTE;
    else         m_Registers[REG_1] &= ~REG_1_MUTE;

    this->SaveRegisters(REG_1, 1);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           SetBand
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Tune to new a band.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void TEA5767::SetBand(void)
{
    //Freq(MHz) = 0.200(in USA) * Channel + 87.5MHz
    m_Registers[REG_4] |= REG_4_BL;

    this->SaveRegisters(REG_4, 1);

  // FM mixer for conversion to IF of the US/Europe (87.5 MHz to 108 MHz)
  //                                  and Japanese  (76 MHz to 91 MHz)
}


//-------------------------------------------------------------------------------------------------
//
//  Name:             GetFrequency
//
//  Parameter(s):   None
//
//  Return:         TEA5767_Freq     The current frequency.
//
//  Description:    Retrieve the real frequency from the chip after automatic tuning.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
TEA5767_Freq_t TEA5767::GetFrequency(void)
{
    uint32_t frequencyW;

    this->ReadRegisters();
    frequencyW = ((m_Status[REG_1] & REG_1_PLL) << 8) | m_Status[REG_2];
    frequencyW = ((frequencyW * QUARTZ / 4) - FILTER) / 10000;
    return (TEA5767_Freq_t)frequencyW;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:             SetFrequency
//
//  Parameter(s):   TEA5767_Freq    newF
//
//  Return:         None
//
//  Description:    Change the frequency in the chip.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void TEA5767::SetFrequency(TEA5767_Freq_t newF)
{
    uint16_t frequencyB;

    m_Freq = newF;

    frequencyB = 4 * (newF * 10000L + FILTER) / QUARTZ;

    m_Registers[REG_0] = frequencyB >> 8;
    m_Registers[REG_1] = frequencyB & 0XFF;
    this->SaveRegisters(REG_0, 2);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:             SetFrequency
//
//  Parameter(s):   TEA5767_Freq    newF
//
//  Return:         None
//
//  Description:    Get the radio information.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void TEA5767::GetRadioInfo(TEA5767_Info_t* Info)
{
    this->ReadRegisters(STAT_3, 2);
    Info->Stereo = (m_Status[STAT_3] & STAT_3_STEREO) ? true : false;
    Info->RSSI   = (m_Status[STAT_4] & STAT_4_ADC) >> 4;
    Info->Mute   = m_Mute;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:             ReadRegisters
//
//  Parameter(s):   None
//
//  Return:         None
//
//  Description:    Load all status registers from to the chip.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void TEA5767::ReadRegisters(int Register, size_t Size)
{
    m_pI2C->Read(Register, &m_Registers[Register], Size, m_pDevice);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:             SaveRegisters
//
//  Parameter(s):   None
//
//  Return:         None
//
//  Description:    Save writable registers back to the chip.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void TEA5767::SaveRegisters(int Register, size_t Size)
{
    m_pI2C->WriteRead(Register, &m_Registers[Register], Size, m_pDevice);
}



//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_I2C
#endif // DIGINI_USE_GRAFX

//void TEA5767_Seek(bool seekUp);
//void TEA5767_WaitEnd(void);
/// Start seek mode upwards.
//void TEA5767_SeekUp(bool toNextSender)
//{
    //DEBUG_FUNC0("seekUp");
//}
/// Start seek mode downwards.
//void TEA5767_SeekDown(bool toNextSender)
//{
//}
//void TEA5767_CheckRDS(void)
//{
  // DEBUG_FUNC0("checkRDS");
//}
/// Seeks out the next available station
//void TEA5767_Seek(bool seekUp)
//{
    //DEBUG_FUNC0("_seek");
//}
/// wait until the current seek and tune operation is over.
//void TEA5767_WaitEnd(void)
//{
   // DEBUG_FUNC0("_waitEnd");
//}


