//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_rtc.h
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

#ifdef STM32F7_RTC_GLOBAL
    #define STM32F7_RTC_EXTERN
#else
    #define STM32F7_RTC_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "nOS.h"
#include "lib_typedef.h"
#include "lib_time_typedef.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define RTC_CLOCK_MODE_LSE          uint32_t(0x00000100)
#define RTC_CLOCK_MODE_LSI          uint32_t(0x00000200)
#define RTC_CLOCK_MODE_HSE          uint32_t(0x00000300)


// This is for a 4 MHz HSE Clock
#define USE_RTC_HSE_CLOCK
#define RTC_CLOCK_SYNC_PRESCALER   uint32_t(0x00007A12)
#define RTC_CLOCK_ASYNC_PRESCALER  uint32_t(0x007F7A12)

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

// TO DO : Add alarm if needed

class CRTC
{
    public:

                                CRTC                    (nOS_Mutex* pMutex, uint32_t Mode);
                                ~CRTC                   ();

        void                    GetDate                 (Date_t* pDate);
        void                    GetTime                 (Time_t* pTime);
        void                    SetDate                 (Date_t* pDate);
        void                    SetTime                 (Time_t* pTime);
        void                    SetDate                 (uint8_t Day, uint8_t Month, uint16_t Year);
        void                    SetTime                 (uint8_t Hour, uint8_t Minute, uint8_t Second);
        void                    GetTimeFormat           (TimeFormat_e* pTimeFormat)                            {*pTimeFormat = m_TimeFormat;};
        void                    SetTimeFormat           (TimeFormat_e* pTimeFormat)                            {m_TimeFormat = *pTimeFormat;};
        uint32_t                GetBackupRegister       (uint8_t Register);
        void                    SetBackupRegister       (uint8_t Register, uint32_t);
        void                    TickHook                ();

        void                    WakeUp_IRQ_Handler      ();
        void                    Stamp_IRQ_Handler       ();
        void                    Alarm_IRQ_Handler       ();

    private:

        void                    Disable                 ();
        void                    Enable                  ();
        SystemState_e           EnterInitMode           ();
        void                    ExitInitMode            ();
        uint8_t                 GetDayOfWeek            (Date_t* pDate);
        void                    Lock                    ();
        void                    Unlock                  ();
        void                    LockRegister            ();
        void                    UnlockRegister          ();
        void                    SetRTC                  ();
        void                    UpdateTimeFeature       ();
        SystemState_e           WaitForSynchro          ();

        nOS_Mutex*              m_pMutex;
        uint32_t                m_TimeOut;
        TimeFormat_e            m_TimeFormat;
        //  Clock_t                 m_Clock;    use nOS clock def
        static const uint8_t    m_MonthSize[12];
        static const uint8_t    m_WeekDayTable[12];
        static const uint16_t   m_DaysSoFar[12];

};

//-------------------------------------------------------------------------------------------------
