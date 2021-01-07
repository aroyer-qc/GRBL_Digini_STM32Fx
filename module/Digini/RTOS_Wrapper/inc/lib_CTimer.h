//-------------------------------------------------------------------------------------------------
//
//  File : lib_CTimer.h
//
//*************************************************************************************************

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
