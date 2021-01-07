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

#include <stdint.h>
#include "lib_CTimer.h"

//-------------------------------------------------------------------------------------------------
// Class CTimer
//-------------------------------------------------------------------------------------------------

CTimer::CTimer(void)
{
    m_pTimerHandle = nullptr;
}

void CTimer::Create(uint32_t        TimerPeriodInTicks,
                    bool            bAutoReload,
                    void*           pInstance)
{
    nOS_TimerCreate(m_pTimerHandle,
                    nullptr,//OTime_trCallback_Wrapper,
                    nullptr,                                // to do add support for argument
                    TimerPeriodInTicks,
                    (bAutoReload == true) ? NOS_TIMER_FREE_RUNNING : NOS_TIMER_ONE_SHOT);
}

void CTimer::Create(uint32_t          TimerPeriodInTicks,
                    bool              bAutoReload,
                    nOS_TimerCallback CallBack,
                    void*             pCallBackArg)
{
    nOS_TimerCreate(m_pTimerHandle,
                    CallBack,
                    pCallBackArg,                                // to do add support for argument
                    TimerPeriodInTicks,
                    (bAutoReload == true) ? NOS_TIMER_FREE_RUNNING : NOS_TIMER_ONE_SHOT);
}

bool CTimer::Start(void)
{
    nOS_Error Error;

    Error = nOS_TimerStart(m_pTimerHandle);
    return (Error == NOS_OK) ? true : false;
}

bool CTimer::Stop(void)
{
    nOS_Error Error;

    Error = nOS_TimerStop(m_pTimerHandle, 0);   // TODO find out what the instant is in the head of JIM
    return (Error == NOS_OK) ? true : false;
}

bool CTimer::Reset(void)
{
    nOS_Error Error;

    Error = nOS_TimerStart(m_pTimerHandle);
    return (Error == NOS_OK) ? true : false;
}

bool CTimer::Pause(void)
{
    nOS_Error Error;

    Error = nOS_TimerPause(m_pTimerHandle);
    return (Error == NOS_OK) ? true : false;
}

bool CTimer::Continue(void)
{
    nOS_Error Error;

    Error = nOS_TimerContinue(m_pTimerHandle);
    return (Error == NOS_OK) ? true : false;
}

#if NOS_CONFIG_SIGNAL_DELETE_ENABLE > 0
bool CTimer::Delete()
{
    nOS_Error Error;

    Error = nOS_TimerDelete(m_pTimerHandle);
    return (Error == NOS_OK) ? true : false;
}
#endif

bool CTimer::ChangePeriod(uint32_t TimerPeriodInTicks)
{
    nOS_Error Error;

    Error = nOS_TimerSetReload(m_pTimerHandle, TimerPeriodInTicks);
    return (Error == NOS_OK) ? true : false;
}

bool CTimer::IsActive(void)
{
    return nOS_TimerIsRunning(m_pTimerHandle);
}

bool CTimer::SetCallBack(void* CallBack, void* arg)
{
    nOS_Error Error;

    Error = nOS_TimerSetCallback(m_pTimerHandle, (nOS_TimerCallback)CallBack, arg);
    return (Error == NOS_OK) ? true : false;
}

//-------------------------------------------------------------------------------------------------
/*
extern "C" void OTime_trCallback_Wrapper(nOS_Timer* pTimer)
{
    class CTimerCallBackInterface* pInterface;

    pInterface = (CTimerCallBackInterface*)(pvTimerGetTimerID(TimerHandle));
    if(pInterface != nullptr)
    {
        pInterface->TimerCallBack();
    }
}
*/
