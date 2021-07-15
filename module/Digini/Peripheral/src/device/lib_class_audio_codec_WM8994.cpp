//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_audio_codec_WM8994.cpp
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
//    Note(s) This file provides a set of functions needed to drive the WM8994 audio codec.
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#include "lib_class_audio_codec_WM8994.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

// I2C Slave address of of the audio codec Cirrus Logic WM8994
#define WM8994_I2C_SLAVE_ADDRESS          ((uint8_t)0x1A)

#define WM8994_CHIPID_ADDR                  0x0000

// todo was in common file... do we need to put in interface
//#define CODEC_STANDARD                0x04
//#define I2S_STANDARD                  I2S_STANDARD_PHILIPS

#define OUTPUT_DEVICE_MASK                   0x00FF
#define INPUT_DEVICE_MASK                   0xFF00


/* Delay for the Codec to be correctly reset */
//#define CODEC_RESET_DELAY             0x4FFF

#define VOLUME_CONVERT(Volume)            (((Volume) > 100)? 100:((uint8_t)(((Volume) * 63) / 100)))
#define VOLUME_IN_CONVERT(Volume)         (((Volume) >= 100)? 239:((uint8_t)(((Volume) * 240) / 100)))

#define DMA_MAX_SIZE                    ((uint16_t)0xFFFF)
#define DMA_MAX(x)                       (((x) <= DMA_MAX_SIZE)? (x):DMA_MAX_SIZE)
#define AUDIODATA_SIZE                  ((uint16_t)2)                               // 16-bits audio data size

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   void*    pArgControl    Pointer on the type of driver use for control.
//                  void*    pArgData       Pointer on the type of driver use for data transfert.
//                  uint16_t InOutDevice    Input and output device
//                  uint8_t  InitialVolume  Volume set default
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:    Initializes the audio codec and the control interface.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::Initialize(void* pArgControl, void* pArgData, uint16_t InOutDevice, uint8_t InitialVolume)
{
    SystemState_e State                   = SYS_READY;
    uint16_t      PowerManagementRegister = 0;

    m_pI2C = (I2C_Driver*)pArgControl;
    m_pSAI = (SAI_Driver*)pArgData;

    // Initialize the Control interface of the Audio Codec

    if(ReadID() == WM8994_ID)
    {
        // Reset the codec Registers
        Reset();

        m_OutputMode = InOutDevice & OUTPUT_DEVICE_MASK;
        m_InputMode  = InOutDevice & INPUT_DEVICE_MASK;

        // WM8994 errata work around
        State = SystemState_e(int(State) | int(Write(0x102, 0x0003)));
        State = SystemState_e(int(State) | int(Write(0x817, 0x0000)));
        State = SystemState_e(int(State) | int(Write(0x102, 0x0000)));

        // Enable VMID soft start (fast), Start-up Bias Current Enabled
        State = SystemState_e(int(State) | int(Write(0x39, 0x006C)));

        // Enable bias generator, Enable VMID
        if(m_InputMode > 0) State = SystemState_e(int(State) | int(Write(0x01, 0x0013)));
        else                 State = SystemState_e(int(State) | int(Write(0x01, 0x0003)));

        nOS_Sleep(50);        // Add Delay

        // Path Configurations for output
        if(m_OutputMode > 0)
        {
            switch(m_OutputMode)
            {
                case OUTPUT_DEVICE_SPEAKER:
                {
                    State = SystemState_e(int(State) | int(Write(0x05,  0x0C0C))); // Enable DAC1 (Left), Enable DAC1 (Right), Disable DAC2 (Left), Disable DAC2 (Right)
                    State = SystemState_e(int(State) | int(Write(0x601, 0x0000))); // Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x602, 0x0000))); // Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path
                    State = SystemState_e(int(State) | int(Write(0x604, 0x0002))); // Disable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x605, 0x0002))); // Disable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path
                }
                break;

                case OUTPUT_DEVICE_HEADPHONE:
                {
                    State = SystemState_e(int(State) | int(Write(0x05,  0x0303))); // Disable DAC1 (Left), Disable DAC1 (Right), Enable DAC2 (Left), Enable DAC2 (Right)
                    State = SystemState_e(int(State) | int(Write(0x601, 0x0001))); // Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x602, 0x0001))); // Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path
                    State = SystemState_e(int(State) | int(Write(0x604, 0x0000))); // Disable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x605, 0x0000))); // Disable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path
                }
                break;

                case OUTPUT_DEVICE_BOTH:
                {
                    if(m_InputMode == INPUT_DEVICE_DIGITAL_MIC1_MIC2)
                    {
                        State = SystemState_e(int(State) | int(Write(0x05,  0x0303 | 0x0C0C))); // Enable DAC1 (Left), Enable DAC1 (Right), also Enable DAC2 (Left), Enable DAC2 (Right)
                        State = SystemState_e(int(State) | int(Write(0x601, 0x0003)));          // Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path Enable the AIF1 Timeslot 1 (Left) to DAC 1 (Left) mixer path
                        State = SystemState_e(int(State) | int(Write(0x602, 0x0003)));          // Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path Enable the AIF1 Timeslot 1 (Right) to DAC 1 (Right) mixer path
                        State = SystemState_e(int(State) | int(Write(0x604, 0x0003)));          // Enable the AIF1 Timeslot 0 (Left) to DAC 2 (Left) mixer path enable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path
                        State = SystemState_e(int(State) | int(Write(0x605, 0x0003)));            // Enable the AIF1 Timeslot 0 (Right) to DAC 2 (Right) mixer path Enable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path
                    }
                    else
                    {
                        State = SystemState_e(int(State) | int(Write(0x05,  0x0303 | 0x0C0C))); // Enable DAC1 (Left), Enable DAC1 (Right), also Enable DAC2 (Left), Enable DAC2 (Right)
                        State = SystemState_e(int(State) | int(Write(0x601, 0x0001)));          // Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path
                        State = SystemState_e(int(State) | int(Write(0x602, 0x0001)));          // Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path
                        State = SystemState_e(int(State) | int(Write(0x604, 0x0002)));          // Enable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path
                        State = SystemState_e(int(State) | int(Write(0x605, 0x0002)));          // Enable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path
                    }
                }
                break;

                // case OUTPUT_DEVICE_AUTO:
                default:
                {
                    State = SystemState_e(int(State) | int(Write(0x05,  0x0303))); // Disable DAC1 (Left), Disable DAC1 (Right), Enable DAC2 (Left), Enable DAC2 (Right)
                    State = SystemState_e(int(State) | int(Write(0x601, 0x0001))); // Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x602, 0x0001))); // Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path
                    State = SystemState_e(int(State) | int(Write(0x604, 0x0000))); // Disable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x605, 0x0000))); // Disable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path
                }
                break;
            }

            m_IsOutputStopped = (State == SYS_READY) ? false : true;
        }

        // Path Configurations for input
        if(m_InputMode != 0)
        {
            switch(m_InputMode)
            {
                case INPUT_DEVICE_DIGITAL_MICROPHONE_2:
                {
                    State = SystemState_e(int(State) | int(Write(0x04,  0x0C30))); // Enable AIF1ADC2 (Left), Enable AIF1ADC2 (Right) Enable DMICDAT2 (Left), Enable DMICDAT2 (Right) Enable Left ADC, Enable Right ADC
                    State = SystemState_e(int(State) | int(Write(0x450, 0x00DB))); // Enable AIF1 DRC2 Signal Detect & DRC in AIF1ADC2 Left/Right Timeslot 1
                    State = SystemState_e(int(State) | int(Write(0x02,  0x6000))); // Disable IN1L, IN1R, IN2L, IN2R, Enable Thermal sensor & shutdown
                    State = SystemState_e(int(State) | int(Write(0x608, 0x0002))); // Enable the DMIC2(Left) to AIF1 Timeslot 1 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x609, 0x0002))); // Enable the DMIC2(Right) to AIF1 Timeslot 1 (Right) mixer path
                    State = SystemState_e(int(State) | int(Write(0x700, 0x000E))); // GPIO1 pin configuration GP1_DIR = output, GP1_FN = AIF1 DRC2 signal detect
                }
                break;

                case INPUT_DEVICE_INPUT_LINE_1:
                {
                    State = SystemState_e(int(State) | int(Write(0x28,  0x0011))); // IN1LN_TO_IN1L, IN1LP_TO_VMID, IN1RN_TO_IN1R, IN1RP_TO_VMID
                    State = SystemState_e(int(State) | int(Write(0x29,  0x0035))); // Disable mute on IN1L_TO_MIXINL and +30dB on IN1L PGA output
                    State = SystemState_e(int(State) | int(Write(0x2A,  0x0035))); // Disable mute on IN1R_TO_MIXINL, Gain = +30dB
                    State = SystemState_e(int(State) | int(Write(0x04,  0x0303))); // Enable AIF1ADC1 (Left), Enable AIF1ADC1 (Right) Enable Left ADC, Enable Right ADC
                    State = SystemState_e(int(State) | int(Write(0x440, 0x00DB))); // Enable AIF1 DRC1 Signal Detect & DRC in AIF1ADC1 Left/Right Timeslot 0
                    State = SystemState_e(int(State) | int(Write(0x02,  0x6350))); // Enable IN1L and IN1R, Disable IN2L and IN2R, Enable Thermal sensor & shutdown
                    State = SystemState_e(int(State) | int(Write(0x606, 0x0002))); // Enable the ADCL(Left) to AIF1 Timeslot 0 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x607, 0x0002))); // Enable the ADCR(Right) to AIF1 Timeslot 0 (Right) mixer path
                    State = SystemState_e(int(State) | int(Write(0x700, 0x000D))); // GPIO1 pin configuration GP1_DIR = output, GP1_FN = AIF1 DRC1 signal detect
                }
                break;

                case INPUT_DEVICE_DIGITAL_MICROPHONE_1:
                {
                    State = SystemState_e(int(State) | int(Write(0x04,  0x030C))); // Enable AIF1ADC1 (Left), Enable AIF1ADC1 (Right) Enable DMICDAT1 (Left), Enable DMICDAT1 (Right) Enable Left ADC, Enable Right ADC
                    State = SystemState_e(int(State) | int(Write(0x440, 0x00DB))); // Enable AIF1 DRC2 Signal Detect & DRC in AIF1ADC1 Left/Right Timeslot 0
                    State = SystemState_e(int(State) | int(Write(0x02,  0x6350))); // Disable IN1L, IN1R, IN2L, IN2R, Enable Thermal sensor & shutdown
                    State = SystemState_e(int(State) | int(Write(0x606, 0x0002))); // Enable the DMIC2(Left) to AIF1 Timeslot 0 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x607, 0x0002))); // Enable the DMIC2(Right) to AIF1 Timeslot 0 (Right) mixer path
                    State = SystemState_e(int(State) | int(Write(0x700, 0x000D))); // GPIO1 pin configuration GP1_DIR = output, GP1_FN = AIF1 DRC1 signal detect
                }
                break;

                case INPUT_DEVICE_DIGITAL_MIC1_MIC2:
                {
                    State = SystemState_e(int(State) | int(Write(0x04,  0x0F3C))); // Enable AIF1ADC1 (Left), Enable AIF1ADC1 (Right) Enable DMICDAT1 (Left), Enable DMICDAT1 (Right) Enable Left ADC, Enable Right ADC
                    State = SystemState_e(int(State) | int(Write(0x450, 0x00DB))); // Enable AIF1 DRC2 Signal Detect & DRC in AIF1ADC2 Left/Right Timeslot 1
                    State = SystemState_e(int(State) | int(Write(0x440, 0x00DB))); // Enable AIF1 DRC2 Signal Detect & DRC in AIF1ADC1 Left/Right Timeslot 0
                    State = SystemState_e(int(State) | int(Write(0x02,  0x63A0))); // Disable IN1L, IN1R, Enable IN2L, IN2R, Thermal sensor & shutdown
                    State = SystemState_e(int(State) | int(Write(0x606, 0x0002))); // Enable the DMIC2(Left) to AIF1 Timeslot 0 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x607, 0x0002))); // Enable the DMIC2(Right) to AIF1 Timeslot 0 (Right) mixer path
                    State = SystemState_e(int(State) | int(Write(0x608, 0x0002))); // Enable the DMIC2(Left) to AIF1 Timeslot 1 (Left) mixer path
                    State = SystemState_e(int(State) | int(Write(0x609, 0x0002))); // Enable the DMIC2(Right) to AIF1 Timeslot 1 (Right) mixer path
                    State = SystemState_e(int(State) | int(Write(0x700, 0x000D))); // GPIO1 pin configuration GP1_DIR = output, GP1_FN = AIF1 DRC1 signal detect
                }
                break;

                // case INPUT_DEVICE_INPUT_LINE_2:
                default:
                {
                    State = SYS_DEVICE_ERROR;    // Actually, no other input devices supported
                }
                break;
            }
            m_IsOutputStopped = (State == SYS_READY) ? false : true;
        }

        //  Clock Configurations
        State = SystemState_e(int(State) | int(SetFrequency(m_pSAI->GetFrequency())));

        if(m_InputMode == INPUT_DEVICE_DIGITAL_MIC1_MIC2) State = SystemState_e(int(State) | int(Write(0x300, 0x4018))); // AIF1 Word Length = 16-bits, AIF1 Format = DSP mode
        else                                               State = SystemState_e(int(State) | int(Write(0x300, 0x4010))); // AIF1 Word Length = 16-bits, AIF1 Format = I2S (Default Register Value)

        State = SystemState_e(int(State) | int(Write(0x302, 0x0000))); // Slave mode
        State = SystemState_e(int(State) | int(Write(0x208, 0x000A))); // Enable the DSP processing clock for AIF1, Enable the core clock
        State = SystemState_e(int(State) | int(Write(0x200, 0x0001))); // Enable AIF1 Clock, AIF1 Clock Source = MCLK1 pin

        if(m_OutputMode != 0)  // Audio output selected
        {
            // Analog Output Configuration
            State = SystemState_e(int(State) | int(Write(0x03, 0x0300))); // Enable SPKRVOL PGA, Enable SPKMIXR, Enable SPKLVOL PGA, Enable SPKMIXL
            State = SystemState_e(int(State) | int(Write(0x22, 0x0000))); // Left Speaker Mixer Volume = 0dB
            State = SystemState_e(int(State) | int(Write(0x23, 0x0000))); // Speaker output mode = Class D, Right Speaker Mixer Volume = 0dB ((0x23, 0x0100) = class AB)
            State = SystemState_e(int(State) | int(Write(0x36, 0x0300))); // Unmute DAC2 (Left) to Left Speaker Mixer (SPKMIXL) path, Unmute DAC2 (Right) to Right Speaker Mixer (SPKMIXR) path
            State = SystemState_e(int(State) | int(Write(0x01, 0x3003))); // Enable bias generator, Enable VMID, Enable SPKOUTL, Enable SPKOUTR

            // Headphone/Speaker Enable
            if(m_InputMode == INPUT_DEVICE_DIGITAL_MIC1_MIC2) State = SystemState_e(int(State) | int(Write(0x51, 0x0205))); // Enable Class W, Class W Envelope Tracking = AIF1 Timeslots 0 and 1
            else                                              State = SystemState_e(int(State) | int(Write(0x51, 0x0005))); // Enable Class W, Class W Envelope Tracking = AIF1 Timeslot 0

            PowerManagementRegister |= 0x0303 | 0x3003;        // Enable bias generator, Enable VMID, Enable HPOUT1 (Left) and Enable HPOUT1 (Right) input stages idem for Speaker
            State = SystemState_e(int(State) | int(Write(0x01, PowerManagementRegister)));
            State = SystemState_e(int(State) | int(Write(0x60, 0x0022)));                // Enable HPOUT1 (Left) and HPOUT1 (Right) intermediate stages
            State = SystemState_e(int(State) | int(Write(0x4C, 0x9F25)));                // Enable Charge Pump
            nOS_Sleep(15);  // Add Delay
            State = SystemState_e(int(State) | int(Write(0x2D, 0x0001)));                // Select DAC1 (Left) to Left Headphone Output PGA (HPOUT1LVOL) path
            State = SystemState_e(int(State) | int(Write(0x2E, 0x0001)));                // Select DAC1 (Right) to Right Headphone Output PGA (HPOUT1RVOL) path
            State = SystemState_e(int(State) | int(Write(0x03, 0x0030 | 0x0300)));        // Enable Left Output Mixer (MIXOUTL), Enable Right Output Mixer (MIXOUTR) idem for SPKOUTL and SPKOUTR
            State = SystemState_e(int(State) | int(Write(0x54, 0x0033)));                // Enable DC Servo and trigger start-up mode on left and right channels
            nOS_Sleep(250); // Add Delay
            State = SystemState_e(int(State) | int(Write(0x60, 0x00EE)));                // Enable HPOUT1 (Left) and HPOUT1 (Right) intermediate and output stages. Remove clamps

            // Unmutes
            State = SystemState_e(int(State) | int(Write(0x610, 0x00C0))); // Unmute DAC 1 (Left)
            State = SystemState_e(int(State) | int(Write(0x611, 0x00C0))); // Unmute DAC 1 (Right)
            State = SystemState_e(int(State) | int(Write(0x420, 0x0000))); // Unmute the AIF1 Timeslot 0 DAC path
            State = SystemState_e(int(State) | int(Write(0x612, 0x00C0))); // Unmute DAC 2 (Left)
            State = SystemState_e(int(State) | int(Write(0x613, 0x00C0))); // Unmute DAC 2 (Right)
            State = SystemState_e(int(State) | int(Write(0x422, 0x0000))); // Unmute the AIF1 Timeslot 1 DAC2 path
            SetVolume(InitialVolume);                // Volume Control
        }

        if(m_InputMode > 0) // Audio input selected
        {
            if((m_InputMode == INPUT_DEVICE_DIGITAL_MICROPHONE_1) || (m_InputMode == INPUT_DEVICE_DIGITAL_MICROPHONE_2))
            {
                PowerManagementRegister |= 0x0013;                // Enable Microphone bias 1 generator, Enable VMID
                State = SystemState_e(int(State) | int(Write(0x01, PowerManagementRegister)));
                State = SystemState_e(int(State) | int(Write(0x620, 0x0002)));            // ADC oversample enable
                State = SystemState_e(int(State) | int(Write(0x411, 0x3800)));          // AIF ADC2 HPF enable, HPF cut = voice mode 1 fc=127Hz at fs=8kHz
            }
            else if(m_InputMode == INPUT_DEVICE_DIGITAL_MIC1_MIC2)
            {
                PowerManagementRegister |= 0x0013;                // Enable Microphone bias 1 generator, Enable VMID
                State = SystemState_e(int(State) | int(Write(0x01, PowerManagementRegister)));
                State = SystemState_e(int(State) | int(Write(0x620, 0x0002)));          // ADC oversample enable
                State = SystemState_e(int(State) | int(Write(0x410, 0x1800)));          // AIF ADC1 HPF enable, HPF cut = voice mode 1 fc=127Hz at fs=8kHz
                State = SystemState_e(int(State) | int(Write(0x411, 0x1800)));          // AIF ADC2 HPF enable, HPF cut = voice mode 1 fc=127Hz at fs=8kHz
            }
            else if((m_InputMode == INPUT_DEVICE_INPUT_LINE_1) || (m_InputMode == INPUT_DEVICE_INPUT_LINE_2))
            {
                State = SystemState_e(int(State) | int(Write(0x18,  0x000B)));            // Disable mute on IN1L, IN1L Volume = +0dB
                State = SystemState_e(int(State) | int(Write(0x1A,  0x000B)));          // Disable mute on IN1R, IN1R Volume = +0dB
                State = SystemState_e(int(State) | int(Write(0x410, 0x1800)));          // AIF ADC1 HPF enable, HPF cut = hifi mode fc=4Hz at fs=48kHz
            }

            SetVolume(InitialVolume); // Volume Control
        }
        return State;
    }
    return SYS_DEVICE_ERROR;
}
#if 0
//-------------------------------------------------------------------------------------------------
//
//  Name:           Reset
//
//  Parameter(s):   DeviceAddr: Device address on communication Bus.
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:    Resets WM8994 registers.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void WM8994::DeInitialize(void)
{
  // Deinitialize Audio Codec interface
  AUDIO_IO_DeInit();
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           DataPortAccessFunction
//
//  Parameter(s):    pInitStruct            Pointer on a structure or variable to configure device
//                                        driver
//
//  Return:         None
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void WM8994::DataPortAccessFunction(int Function, void* pData)
{
    if(Function == SAI_FUNCTION_SET_AUDIO_FRAME_SLOT)
    {
        m_pSAI->SetAudioFrameSlot(*((uint32_t*)pData));
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ReadID
//
//  Parameter(s):    None
//
//  Return:         ID
//
//  Description:    Get the CS43L22 ID.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
uint16_t WM8994::ReadID(void)
{
    uint16_t Value = 0;

    Read(WM8994_CHIPID_ADDR, &Value);

    return Value;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Play
//
//  Parameter(s):    not used parameter
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:    Start the audio codec.
//
//  Note(s):        For this codec no play options are required.
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::Play(uint16_t* pBuffer, uint16_t Size)
{
    SystemState_e State;

    if(m_IsOutputStopped == true)
    {
        State = SetMute(AUDIO_MUTE_OFF);
        m_IsOutputStopped = false;
    }

    if(State == SYS_READY)
    {
        // Update the Media layer and enable it for play
        m_pSAI->Transmit_DMA((uint8_t*)pBuffer, DMA_MAX(Size / AUDIODATA_SIZE));
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Pause
//
//  Parameter(s):   DeviceAddr: Device address on communication Bus.
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:       Pauses playing on the audio codec.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::Pause(void)
{
    SystemState_e State = SYS_READY;

    State = SystemState_e(int(State) | int(SetMute(AUDIO_MUTE_ON)));    // Mute the output first
    State = SystemState_e(int(State) | int(Write(0X02, 0x01)));            // Put the codec in power save mode
    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Resume
//
//  Parameter(s):   None
//
//  Return:         true or false
//
//  Description:    Resumes playing on the audio codec.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::Resume(void)
{
    return SetMute(AUDIO_MUTE_OFF);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Stop
//
//  Parameter(s):   uint32_t CodecPdwnMode        Selects the  power down mode.
//  *                                              - CODEC_PDWN_HW: Physically power down the codec.
//
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:    Stops audio Codec playing. It powers down the codec.
//
//  Note(s):        When resuming from this mode, the codec is set to default configuration
//                    User should re-Initialize the codec in order to play again the audio stream.
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::Stop(uint32_t CodecPdwnMode)
{
    SystemState_e State = SYS_READY;

    if(m_IsOutputStopped == false)
    {
        m_pSAI->StopDMA();

        State = SystemState_e(int(State) | int(SetMute(AUDIO_MUTE_ON))); // Mute the output first

        if(CodecPdwnMode == CODEC_PDWN_SW)
        {
            // Only output mute required
        }
        else // CODEC_PDWN_HW
        {
            State = SystemState_e(int(State) | int(Write(0x420,  0x0200))); // Mute the AIF1 Timeslot 0 DAC1 path
            State = SystemState_e(int(State) | int(Write(0x422,  0x0200)));    // Mute the AIF1 Timeslot 1 DAC2 path
            State = SystemState_e(int(State) | int(Write(0x2D,   0x0000)));    // Disable DAC1L_TO_HPOUT1L
            State = SystemState_e(int(State) | int(Write(0x2E,   0x0000)));    // Disable DAC1R_TO_HPOUT1R
            State = SystemState_e(int(State) | int(Write(0x05,   0x0000)));    // Disable DAC1 and DAC2
            State = SystemState_e(int(State) | int(Write(0x0000, 0x0000)));    // Reset codec by writing in 0x0000 address register
            m_IsOutputStopped = true;
        }
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SetVolume
//
//  Parameter(s):   uint8_t     Volume:     Byte value from 0 to 255.
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:    Sets higher or lower the codec volume level.
//
//  Note(s):        Value are adapted to the codec in use
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::SetVolume(uint8_t Volume)
{
    SystemState_e State           = SYS_READY;
    uint16_t       ConvertedVolume = VOLUME_CONVERT(Volume);

    if(m_OutputMode != 0)            // Output volume
    {
        if(ConvertedVolume > 0x3E)
        {
            State = SystemState_e(int(State) | int(SetMute(AUDIO_MUTE_OFF)));    // Unmute audio codec
            State = SystemState_e(int(State) | int(Write(0x1C, 0x17F)));        // Left Headphone Volume
            State = SystemState_e(int(State) | int(Write(0x1D, 0x17F)));        // Right Headphone Volume
            State = SystemState_e(int(State) | int(Write(0x26, 0x17F)));        // Left Speaker Volume
            State = SystemState_e(int(State) | int(Write(0x27, 0x17F)));        // Right Speaker Volume
        }
        else if (Volume == 0)
        {
            State = SystemState_e(int(State) | int(SetMute(AUDIO_MUTE_ON)));    // Mute audio codec
        }
        else
        {
            State = SystemState_e(int(State) | int(SetMute(AUDIO_MUTE_OFF)));    // Unmute audio codec
            ConvertedVolume |= 0x140;
            State = SystemState_e(int(State) | int(Write(0x1C, ConvertedVolume))); // Left Headphone Volume
            State = SystemState_e(int(State) | int(Write(0x1D, ConvertedVolume))); // Right Headphone Volume
            State = SystemState_e(int(State) | int(Write(0x26, ConvertedVolume))); // Left Speaker Volume
            State = SystemState_e(int(State) | int(Write(0x27, ConvertedVolume))); // Right Speaker Volume
        }
    }

    if(m_InputMode != 0)            // Input volume
    {
        ConvertedVolume = VOLUME_IN_CONVERT(Volume) | 0x100;
        State = SystemState_e(int(State) | int(Write(0x400, ConvertedVolume)));    // Left AIF1 ADC1 volume
        State = SystemState_e(int(State) | int(Write(0x401, ConvertedVolume)));    // Right AIF1 ADC1 volume
        State = SystemState_e(int(State) | int(Write(0x404, ConvertedVolume)));    // Left AIF1 ADC2 volume
        State = SystemState_e(int(State) | int(Write(0x405, ConvertedVolume)));    // Right AIF1 ADC2 volume
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SetMute
//
//  Parameter(s):   uint32_t Command    AUDIO_MUTE_ON to enable the mute
//                                         AUDIO_MUTE_OFF to disable the
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:    Enables or disables the mute feature on the audio codec.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::SetMute(uint32_t Command)
{
    SystemState_e State = SYS_READY;

    if(Command == AUDIO_MUTE_ON)    // Set the Mute mode
    {
        State = SystemState_e(int(State) | int(Write(0x420, 0x0200))); // Soft Mute the AIF1 Time slot 0 DAC1 path L&R
        State = SystemState_e(int(State) | int(Write(0x422, 0x0200))); // Soft Mute the AIF1 Time slot 1 DAC2 path L&R
    }
    else                             // AUDIO_MUTE_OFF Disable the Mute
    {
        State = SystemState_e(int(State) | int(Write(0x420, 0x0000))); // Unmute the AIF1 Time slot 0 DAC1 path L&R
        State = SystemState_e(int(State) | int(Write(0x422, 0x0000))); // Unmute the AIF1 Time slot 1 DAC2 path L&R
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SetOutputMode
//
//  Parameter(s):      uint8_t Mode    Specifies the audio output target:     OUTPUT_DEVICE_SPEAKER
//                                                                        OUTPUT_DEVICE_HEADPHONE
//                                                                        OUTPUT_DEVICE_BOTH
//                                                                      OUTPUT_DEVICE_AUTO
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:    Switch dynamically the output target (speaker or headphone).
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::SetOutputMode(uint8_t Mode)
{
    SystemState_e State = SYS_READY;

    switch(Mode)
    {
        case OUTPUT_DEVICE_SPEAKER:
        {
            State = SystemState_e(int(State) | int(Write(0x05,  0x0C0C))); // Enable DAC1 (Left), enable DAC1 (Right), disable DAC2 (Left), disable DAC2 (Right)
            State = SystemState_e(int(State) | int(Write(0x601, 0x0000))); // Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path
            State = SystemState_e(int(State) | int(Write(0x602, 0x0000))); // Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path
            State = SystemState_e(int(State) | int(Write(0x604, 0x0002))); // Disable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path
            State = SystemState_e(int(State) | int(Write(0x605, 0x0002))); // Disable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path
        }
        break;

        case OUTPUT_DEVICE_HEADPHONE:
        {
            State = SystemState_e(int(State) | int(Write(0x05,  0x0303))); // Disable DAC1 (Left), disable DAC1 (Right), enable DAC2 (Left), enable DAC2 (Right)
            State = SystemState_e(int(State) | int(Write(0x601, 0x0001))); // Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path
            State = SystemState_e(int(State) | int(Write(0x602, 0x0001))); // Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path
            State = SystemState_e(int(State) | int(Write(0x604, 0x0000))); // Disable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path
            State = SystemState_e(int(State) | int(Write(0x605, 0x0000))); // Disable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path
        }
        break;

        case OUTPUT_DEVICE_BOTH:
        {
            State = SystemState_e(int(State) | int(Write(0x05,  0x0303 | 0x0C0C))); // Enable DAC1 (Left), enable DAC1 (Right), enable DAC2 (Left), enable DAC2 (Right)
            State = SystemState_e(int(State) | int(Write(0x601, 0x0001)));            // Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path
            State = SystemState_e(int(State) | int(Write(0x602, 0x0001)));            // Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path
            State = SystemState_e(int(State) | int(Write(0x604, 0x0002)));            // Enable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path
            State = SystemState_e(int(State) | int(Write(0x605, 0x0002)));            // Enable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path
        }
        break;

        default:
        {
            State = SystemState_e(int(State) | int(Write(0x05,  0x0303)));            // Disable DAC1 (Left), disable DAC1 (Right), enable DAC2 (Left), enable DAC2 (Right)
            State = SystemState_e(int(State) | int(Write(0x601, 0x0001)));            // Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path
            State = SystemState_e(int(State) | int(Write(0x602, 0x0001)));            // Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path
            State = SystemState_e(int(State) | int(Write(0x604, 0x0000)));            // Disable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path
            State = SystemState_e(int(State) | int(Write(0x605, 0x0000)));            // Disable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path
        }
        break;
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SetFrequency
//
//  Parameter(s):   uint32_t AudioFrequency     Audio frequency used to play the audio stream.
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:    Sets new frequency.
//
//  Note(s):        Feature not supported
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::SetFrequency(uint32_t AudioFrequency)
{
    uint16_t ConfigFrequency;

    switch(AudioFrequency)     //  Clock Configurations
    {
        case SAI_AUDIO_FREQUENCY_8K:  ConfigFrequency = 0x0003; break; // AIF1 Sample Rate = 8 (KHz),      ratio=256
        case SAI_AUDIO_FREQUENCY_16K: ConfigFrequency = 0x0033; break; // AIF1 Sample Rate = 16 (KHz),     ratio=256
        case SAI_AUDIO_FREQUENCY_48K: ConfigFrequency = 0x0083; break; // AIF1 Sample Rate = 48 (KHz),     ratio=256
        case SAI_AUDIO_FREQUENCY_96K: ConfigFrequency = 0x00A3; break; // AIF1 Sample Rate = 96 (KHz),     ratio=256
        case SAI_AUDIO_FREQUENCY_11K: ConfigFrequency = 0x0013; break; // AIF1 Sample Rate = 11.025 (KHz), ratio=256
        case SAI_AUDIO_FREQUENCY_22K: ConfigFrequency = 0x0043; break; // AIF1 Sample Rate = 22.050 (KHz), ratio=256
        case SAI_AUDIO_FREQUENCY_44K: ConfigFrequency = 0x0073; break; // AIF1 Sample Rate = 44.1 (KHz),   ratio=256
        default:                      ConfigFrequency = 0x0083; break; // AIF1 Sample Rate = 48 (KHz),     ratio=256
    }

    return Write(0x210, ConfigFrequency);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Read
//
//  Parameter(s):    Register             Register to write
//                    pData                Data for register
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:    Read data.
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::Read(uint16_t Register, uint16_t* pData)
{
    SystemState_e    State;
    uint8_t         TX_Buffer[2];
    uint8_t         RX_Buffer[2];

    TX_Buffer[0] = uint8_t(Register >> 8);
    TX_Buffer[1] = uint8_t(Register);
    State = m_pI2C->Transfer(&TX_Buffer[0], sizeof(uint16_t), &RX_Buffer[0], sizeof(uint16_t), WM8994_I2C_SLAVE_ADDRESS);
    *pData  = uint16_t(RX_Buffer[0]) << 8;
    *pData |= uint16_t(RX_Buffer[1]);

    return State;
}
//-------------------------------------------------------------------------------------------------
//
//  Name:           Write
//
//  Parameter(s):    Register             Register to write
//                    Data                Data for register
//
//  Return:         SYS_READY or SYS_DEVICE_ERROR
//
//  Description:    Write data.
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::Write(uint16_t Register, uint16_t Data)
{
    SystemState_e    State;
    uint8_t         Buffer[4];

    Buffer[0] = uint8_t(Register >> 8);
    Buffer[1] = uint8_t(Register);
    Buffer[2] = uint8_t(Data >> 8);
    Buffer[3] = uint8_t(Data);

    State = m_pI2C->Transfer(&Buffer[0], 4, nullptr, 0, WM8994_I2C_SLAVE_ADDRESS);
  #ifdef VERIFY_WRITTENDATA
    // Verify that the data has been correctly written
    State = SystemState_e(int(State) | int((AUDIO_IO_Read(m_Address, Register) == Data) ? true : false;
  #endif // VERIFY_WRITTENDATA

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Reset
//
//  Parameter(s):     None
//
//  Return:         SYS_READY
//
//  Description:    Resets WM8994 registers.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e WM8994::Reset(void)
{
    SystemState_e State;

    State = Write(0x0000, 0x0000);    // Reset codec by writing in 0x0000 address register

    if(State == SYS_READY)
    {
        m_IsOutputStopped = true;
        m_IsInputStopped  = true;
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
