//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_rtc.h
//
//*************************************************************************************************


#ifndef __STM32F4_RTC_H__
#define __STM32F4_RTC_H__

#ifdef STM32F4_RTC_GLOBAL
    #define STM32F4_RTC_EXTERN
#else
    #define STM32F4_RTC_EXTERN extern
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
                                ~CRTC                    ();

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
        Clock_t                 m_Clock;
        static const uint8_t    m_MonthSize[12];
        static const uint8_t    m_WeekDayTable[12];
        static const uint16_t   m_DaysSoFar[12];

};

#endif
