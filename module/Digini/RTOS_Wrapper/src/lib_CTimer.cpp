//-------------------------------------------------------------------------------------------------
//
//  File : lib_CTimer.cpp
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

#include "lib_CTimer.h"

//-------------------------------------------------------------------------------------------------

CTimer::CTimer()
{
    TimerHandle = 0;
}

void CTimer::Create(const char*     TimerName,
                    uint32_t        TimerPeriodInTicks,
                    bool            bAutoReload,
                    void*           pInstance)
{
    TimerHandle = xTimerCreate((const char*)TimerName, (portTickType)TimerPeriodInTicks, (unsigned portBASE_TYPE)bAutoReload, pInstance, OsTimerCallback_Wrapper);
}

void CTimer::Create(const char*       TimerName,
                    uint32_t          TimerPeriodInTicks,
                    bool              bAutoReload,
                    tmrTIMER_CALLBACK CallBack)
{
    TimerHandle = xTimerCreate((const char*)TimerName, (portTickType)TimerPeriodInTicks, (unsigned portBASE_TYPE)bAutoReload, 0, CallBack);
}

bool CTimer::IsActive()
{
    return bool(xTimerIsTimerActive(TimerHandle));
}

xTaskHandle CTimer::GetDaemonTaskHandle()
{
    return xTimerGetTimerDaemonTaskHandle();
}

bool CTimer::Start(portTickType xBlockTime)
{
    return bool(xTimerStart(TimerHandle, xBlockTime));
}

bool CTimer::Stop(portTickType xBlockTime)
{
    return bool(xTimerStop(TimerHandle, xBlockTime));
}

bool CTimer::ChangePeriod(portTickType xNewPeriod, portTickType xBlockTime)
{
    return bool(xTimerChangePeriod(TimerHandle, xNewPeriod, xBlockTime));
}

bool CTimer::Delete(portTickType xBlockTime)
{
    return bool(xTimerDelete(TimerHandle, xBlockTime));
}

bool CTimer::Reset(portTickType xBlockTime)
{
    return bool(xTimerReset(TimerHandle, xBlockTime));
}

bool CTimer::StartFromISR(portBASE_TYPE *pxHigherPriorityTaskWoken)
{
    return bool(xTimerStartFromISR(TimerHandle, pxHigherPriorityTaskWoken));
}

bool CTimer::StopFromISR(portBASE_TYPE *pxHigherPriorityTaskWoken)
{
    return bool(xTimerStopFromISR(TimerHandle, pxHigherPriorityTaskWoken));
}

bool CTimer::ChangePeriodFromISR(portTickType xNewPeriod, portBASE_TYPE *pxHigherPriorityTaskWoken)
{
    return bool(xTimerChangePeriodFromISR(TimerHandle, xNewPeriod, pxHigherPriorityTaskWoken));
}

bool CTimer::ChangePeriodFromISR(portBASE_TYPE *pxHigherPriorityTaskWoken)
{
    return bool(xTimerResetFromISR(TimerHandle, pxHigherPriorityTaskWoken));
}

//-------------------------------------------------------------------------------------------------

extern "C" void OsTimerCallback_Wrapper(xTimerHandle TimerHandle)
{
    class CTimerCallBackInterface* pInterface;

    pInterface = (CTimerCallBackInterface*)(pvTimerGetTimerID(TimerHandle));
    if(pInterface != nullptr)
    {
        pInterface->TimerCallBack();
    }
}

