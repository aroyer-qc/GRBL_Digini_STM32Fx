//-------------------------------------------------------------------------------------------------
//
//  File : lib_CTimer.cpp
//
//*************************************************************************************************

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------
#include "lib_CTimer.h"




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

