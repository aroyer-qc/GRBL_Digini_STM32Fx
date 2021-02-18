//-------------------------------------------------------------------------------------------------
//
//  File : lib_CTimer.h
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
#include "lib_RTOS.h"

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class CTimerCallBackInterface
{
    public:

        virtual void TimerCallBack() = 0;
};

class CTimer
{
    public:

                    CTimer                      ();

        void        Create                      (const char*     TimerName,
                                                 uint32_t        TimerPeriodInTicks,
                                                 bool            bAutoReload,
                                                 void*           pInstance
                                                );
        void        Create                      (const char*       TimerName,
                                                 uint32_t          TimerPeriodInTicks,
                                                 bool              bAutoReload,
                                                 tmrTIMER_CALLBACK CallBack
                                                );

        bool        IsActive                    ();
        xTaskHandle GetDaemonTaskHandle         ();
        bool        Start                       (portTickType xBlockTime);
        bool        Stop                        (portTickType xBlockTime);
        bool        ChangePeriod                (portTickType xNewPeriod, portTickType xBlockTime);
        bool        Delete                      (portTickType xBlockTime);
        bool        Reset                       (portTickType xBlockTime);
        bool        StartFromISR                (portBASE_TYPE *pxHigherPriorityTaskWoken);
        bool        StopFromISR                 (portBASE_TYPE *pxHigherPriorityTaskWoken);
        bool        ChangePeriodFromISR         (portTickType xNewPeriod, portBASE_TYPE *pxHigherPriorityTaskWoken);
        bool        ChangePeriodFromISR         (portBASE_TYPE *pxHigherPriorityTaskWoken);

    private:

        xTimerHandle        TimerHandle;
};

//-------------------------------------------------------------------------------------------------

extern "C" void OsTimerCallback_Wrapper(xTimerHandle TimerHandle);
