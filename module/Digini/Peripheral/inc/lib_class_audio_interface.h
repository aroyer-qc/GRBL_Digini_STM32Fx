//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_audio_driver.h
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

#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

// Codec POWER DOWN modes
#define CODEC_PDWN_HW                         1
#define CODEC_PDWN_SW                         2

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class AudioDriverInterface
{
    public:

        virtual SystemState_e         Initialize                (void*    pArgControl,
                                                             void*    pArgData,
                                                             uint16_t InOutDevice,
                                                             uint8_t  InitialVolume)            = 0;
        //virtual SystemState_e     DeInitialize            (void)                                = 0;
        virtual uint16_t            ReadID                    (void)                                = 0;
        virtual SystemState_e        Reset                    (void)                                = 0;
        virtual SystemState_e         Play                    (uint16_t* pBuffer, uint16_t Size)  = 0;
        virtual SystemState_e         Stop                    (uint32_t CodecPdwnMode)            = 0;
        virtual SystemState_e         Pause                    (void)                                = 0;
        virtual SystemState_e         Resume                    (void)                                = 0;
        virtual SystemState_e         SetVolume                (uint8_t Volume)                    = 0;
        virtual SystemState_e         SetFrequency            (uint32_t AudioFrequency)           = 0;
        virtual SystemState_e         SetMute                    (uint32_t Command)                    = 0;
        virtual SystemState_e         SetOutputMode            (uint8_t Mode)                        = 0;
        virtual void                 DataPortAccessFunction    (int Function, void* pData)         = 0;
};

//-------------------------------------------------------------------------------------------------

