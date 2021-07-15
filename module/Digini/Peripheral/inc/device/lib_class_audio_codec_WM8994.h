//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_audio_codec_WM8994.h
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

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <lib_class_audio_interface.h>
#include "digini_cfg.h"
#include "stm32f7xx.h"
#include "lib_class_i2c.h"
#include "lib_class_sai.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define OUTPUT_DEVICE_SPEAKER               ((uint16_t)0x0001)
#define OUTPUT_DEVICE_HEADPHONE             ((uint16_t)0x0002)
#define OUTPUT_DEVICE_BOTH                  ((uint16_t)0x0003)
#define OUTPUT_DEVICE_AUTO                  ((uint16_t)0x0004)
#define INPUT_DEVICE_DIGITAL_MICROPHONE_1   ((uint16_t)0x0100)
#define INPUT_DEVICE_DIGITAL_MICROPHONE_2   ((uint16_t)0x0200)
#define INPUT_DEVICE_INPUT_LINE_1           ((uint16_t)0x0300)
#define INPUT_DEVICE_INPUT_LINE_2           ((uint16_t)0x0400)
#define INPUT_DEVICE_DIGITAL_MIC1_MIC2      ((uint16_t)0x0800)

// Volume Levels values
#define DEFAULT_VOLMIN                        0x00
#define DEFAULT_VOLMAX                        0xFF
#define DEFAULT_VOLSTEP                       0x04

#define AUDIO_PAUSE                           0
#define AUDIO_RESUME                          1

// MUTE commands
#define AUDIO_MUTE_ON                         1
#define AUDIO_MUTE_OFF                        0

#define WM8994_ID                              0x8994

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class WM8994 : public AudioDriverInterface
{
    public:


        SystemState_e         Initialize                    (void* pArgControl, void* pArgData, uint16_t Device, uint8_t InitialVolume);
        //SystemState_e     DeInitialize                (void);
        uint16_t            ReadID                        (void);
        SystemState_e        Reset                        (void);

        SystemState_e         Play                        (uint16_t* pBuffer, uint16_t Size);
        SystemState_e         Stop                        (uint32_t CodecPdwnMode);
        SystemState_e         Pause                        (void);
        SystemState_e         Resume                        (void);

        SystemState_e         SetVolume                    (uint8_t Volume);
        SystemState_e         SetFrequency                (uint32_t AudioFrequency);
        SystemState_e         SetMute                        (uint32_t Command);
        SystemState_e         SetOutputMode                (uint8_t Mode);
        void                 DataPortAccessFunction        (int Function, void* pData);

    private:

        SystemState_e       Read                        (uint16_t Register, uint16_t* pData);
        SystemState_e        Write                        (uint16_t Register, uint16_t Data);

        uint8_t                m_DeviceAddress;
        //uint8_t             m_OutputConfig;
        uint16_t            m_OutputMode;
        uint16_t            m_InputMode;
        bool                 m_IsOutputStopped;
        bool                 m_IsInputStopped;
        SAI_Driver*            m_pSAI;
        I2C_Driver*            m_pI2C;
};

//-------------------------------------------------------------------------------------------------
