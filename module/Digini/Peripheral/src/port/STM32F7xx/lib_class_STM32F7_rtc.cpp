//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_rtc.cpp
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
// TO DO Add support for IRQ
//
//  Create a pool of event, with callback
//  use Alarm A for recurring alarm
//  use Alarm B for one shot alarm
//  need function to register an (event, callback, single or recurring)
//  need function to unregister an event
//  need function to sort the pool
//  need to write IRQ to handle event callback. and update for next event
//       Need to update recurring event with new stamp ans remap it...
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#define STM32F7_RTC_GLOBAL
#include "lib_class_STM32F4_rtc.h"
#undef STM32F7_RTC_GLOBAL
#include "string.h"
#include "STM32F7xx.h"
#include "lib_utility.h"

//-------------------------------------------------------------------------------------------------

#if USE_RTC_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
//
//   Class: CRTC
//
//   Description:   Class CRTC
//
//-------------------------------------------------------------------------------------------------

const uint8_t  CRTC::m_MonthSize[12]     = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint8_t  CRTC::m_WeekDayTable[12]  = {4, 7, 7, 3, 5, 8, 3, 6, 9, 4, 7, 9};
const uint16_t CRTC::m_DaysSoFar[12]     = {0, 31, 59, 90, 120, 151, 181, 212, 243, 274, 303, 334};

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CRTC
//
//   Parameter(s):  nOS_Mutex*          pMutex
//                  uint32_t            Mode
//
//
//   Description:   Initializes the RTC peripheral according to the specified Parameters
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CRTC::CRTC(nOS_Mutex* pMutex, uint32_t Mode)
{
    m_pMutex = pMutex;
    // Init power and clock for RTC
    RCC->APB1ENR |=  ( RCC_APB1ENR_PWREN);     // Backup interface & Power interface clock enable
    PWR->CR   |= PWR_CR_DBP;                                        // RTC register access allowed
    RCC->BDCR &= uint32_t(~RCC_BDCR_RTCSEL);                        // Clear clock selection
    RCC->BDCR |= Mode;                                              // Set it with argument
    RCC->BDCR |= RCC_BDCR_RTCEN;                                    // RTC clock enable
    if(Mode == RTC_CLOCK_MODE_LSE) RCC->BDCR |= RCC_BDCR_LSEON;     // External 32.768 kHz oscillator ON
    if(Mode == RTC_CLOCK_MODE_LSI) RCC->CSR  |= RCC_CSR_LSION;      // Internal 32 kHz oscillator ON
    PWR->CR   &= uint32_t(~PWR_CR_DBP);                             // RTC register access blocked
    UnlockRegister();
    EnterInitMode();
    switch(Mode)
    {
        case RTC_CLOCK_MODE_LSE:
        {
            RTC->PRER = 0x000000FF;                                 // As per data sheet to separate write are necessary (value from data sheet)
            RTC->PRER = 0x007F00FF;                                 // First is the synchronous prescaler then the asynchronous prescaler factor
            break;
        }

        case RTC_CLOCK_MODE_LSI:
        {
            RTC->PRER = 0x000000FA;
            RTC->PRER = 0x007F00FA;
            break;
        }
       #ifdef USE_RTC_HSE_CLOCK
        case RTC_CLOCK_MODE_HSE:
        {
            RTC->PRER = RTC_CLOCK_SYNC_PRESCALER;
            RTC->PRER = RTC_CLOCK_SYNC_PRESCALER | RTC_CLOCK_ASYNC_PRESCALER;
        }
       #endif
    }

    RTC->CR &= uint32_t(~RTC_CR_FMT);                           // 12/24 Time format mode set default to 24 hours
    ExitInitMode();
    LockRegister();
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: GetBackupRegister
//
//   Parameter(s):  uint8_t     Register
//   Return value:  uint32_t    Value
//
//   Description:   Return value at requested backup register
//
//-------------------------------------------------------------------------------------------------
uint32_t CRTC::GetBackupRegister(uint8_t Register)
{
    return *(&RTC->BKP0R + Register);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: SetBackupRegister
//
//   Parameter(s):  uint8_t     Register
//                  uint32_t    Value
//   Return value:  None
//
//   Description:   Set value at a requested backup register
//
//-------------------------------------------------------------------------------------------------
void CRTC::SetBackupRegister(uint8_t Register, uint32_t Value)
{
    PWR->CR  |= PWR_CR_DBP;
    *(&RTC->BKP0R + Register) = Value;
    PWR->CR  &= uint32_t(~PWR_CR_DBP);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: GetDate
//
//   Parameter(s):  Date_t*      pDate
//   Return value:  None
//
//   Description:   Return real date
//
//-------------------------------------------------------------------------------------------------
void CRTC::GetDate(Date_t* pDate)
{
    Lock();
    UpdateTimeFeature();
    memcpy(pDate, &m_Clock.Date, sizeof(Date_t));
    Unlock();
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: GetTime
//
//   Parameter(s):  Time_t*      pTime
//   Return value:  None
//
//   Description:   Return real time
//
//-------------------------------------------------------------------------------------------------
void CRTC::GetTime(Time_t* pTime)
{
    Lock();
    UpdateTimeFeature();
    memcpy(pTime, &m_Clock.Time, sizeof(Time_t));
    Unlock();
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: SetDate
//
//   Parameter(s):
//   Return value:
//
//   Description:
//
//-------------------------------------------------------------------------------------------------
void CRTC::SetDate(Date_t* pDate)
{
    uint32_t Date;
    uint8_t  DayOfWeek;

    DayOfWeek = GetDayOfWeek(pDate);
    if(DayOfWeek == 0) DayOfWeek = 7;
    Date  = (uint32_t(DayOfWeek) + 1) << 13;
    Date |= uint32_t(LIB_2DecBcd(pDate->Year)) << 16;
    Date |= uint32_t(LIB_2DecBcd(pDate->Month)) << 8;
    Date |= uint32_t(LIB_2DecBcd(pDate->Day));

    Enable();
    RTC->DR = Date;
    memcpy(&m_Clock.Date, pDate, sizeof(Date_t));
    m_Clock.DayOfWeek = DayOfWeek;
    UpdateTimeFeature();
    Disable();
}

void CRTC::SetDate(uint8_t Day, uint8_t Month, uint16_t Year)
{
    Date_t Date;

    Date.Day   = Day;
    Date.Month = Month;
    Date.Year  = Year;
    SetDate(&Date);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: SetTime
//
//   Parameter(s):
//   Return value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CRTC::SetTime(Time_t* pTime)
{
    uint32_t Time;

    Time  = uint32_t(LIB_2DecBcd(pTime->Hour)) << 16;
    Time |= uint32_t(LIB_2DecBcd(pTime->Minute)) << 8;                      // Minute
    Time |= uint32_t(LIB_2DecBcd(pTime->Second));                           // Second

    Enable();
    RTC->TR = Time;
    memcpy(&m_Clock.Time, pTime, sizeof(Time_t));
    UpdateTimeFeature();
    Disable();
}


void CRTC::SetTime(uint8_t Hour, uint8_t Minute, uint8_t Second)
{
    Time_t Time;

    m_Clock.Time.Hour   = Hour;
    m_Clock.Time.Minute = Minute;
    m_Clock.Time.Second = Second;
    SetTime(&Time);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: TickHook
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Call by the high level SD Card class
//
//   Note(s):       Do not put in OS tick hook
//
//-------------------------------------------------------------------------------------------------
void CRTC::TickHook(void)
{
    if(m_TimeOut > 0)
    {
        m_TimeOut--;
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: Enable
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   Allow time, date, etc.. to be set
//
//-------------------------------------------------------------------------------------------------
void CRTC::Enable(void)
{
    Lock();
    UnlockRegister();
    EnterInitMode();
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: Disable
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   Disable all access to RTC
//
//-------------------------------------------------------------------------------------------------
void CRTC::Disable(void)
{
    ExitInitMode();
    LockRegister();
    Unlock();
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: EnterInitMode
//
//   Parameter(s):  None
//   Return value:  SystemState_e    State
//
//   Description:   Stop calendar and enter initialization mode
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e CRTC::EnterInitMode()
{
    SystemState_e    State;

    // Enter initialization mode, and stop calendar counter (INIT)
    RTC->ISR |= RTC_ISR_INIT;

    // Wait until RTC is ready or time out has expired
    m_TimeOut = 10;
    do
    {
        OS_TaskYield();
    }
    while(((RTC->ISR & RTC_ISR_INITF) == 0) && (m_TimeOut != 0));

    if(m_TimeOut == 0) State = TIME_OUT;
    else               State = SYS_READY;

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: ExitInitMode
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   Stop calendar and enter initialization mode
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CRTC::ExitInitMode(void)
{
    // Exit initialization mode, and restart calendar counter
    RTC->ISR &= uint32_t(~RTC_ISR_INIT);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: GetDayOfWeek
//
//   Parameter(s):  Date_t*  pDate
//   Return value:  uint8_t DayOfWeek
//
//   Description:   Get the day of the week from the date
//
//   Note(s):       (0-6, Sunday-Saturday) to stay generic, even if ST RTC are 1-Monday 7-Sunday
//
//-------------------------------------------------------------------------------------------------
uint8_t CRTC::GetDayOfWeek(Date_t* pDate)
{
    uint16_t Day;

    // Pre calculate the day of the week (0-6, SUNDAY-SATURDAY)
    Day  = (pDate->Year >> 2) + 2;
    Day += (pDate->Year + pDate->Day + m_WeekDayTable[pDate->Month - 1]);

    if((pDate->Year % 4) == 0)
    {
        if(pDate->Month < 3)
        {
            if(Day == 0) Day = 6;
            else         Day--;
        }
    }

    return uint8_t(Day % 7);
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Lock
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Lock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CRTC::Lock(void)
{
    while(nOS_MutexLock(m_pMutex, NOS_WAIT_INFINITE) != NOS_OK);
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Unlock
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Unlock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CRTC::Unlock(void)
{
    while(nOS_MutexUnlock(m_pMutex) != NOS_OK);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: UnlockRegister
//                  lockRegister
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   Lock or unlock access to the RTC register
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CRTC::LockRegister(void)
{
    // Write wrong key to relock "Write Protection Register"
    RTC->WPR  = 0x00;
    PWR->CR  &= uint32_t(~PWR_CR_DBP);
}


void CRTC::UnlockRegister(void)
{
    // Write unlock key to "Write Protection Register"
    PWR->CR  |= PWR_CR_DBP;
    RTC->WPR  = 0xCA;
    RTC->WPR  = 0x53;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: UpdateTimeFeature
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   Update all feature unhandle by RTC
//
//-------------------------------------------------------------------------------------------------
void CRTC::UpdateTimeFeature(void)
{
    uint32_t Time;
    uint32_t Date;

    Time = RTC->TR;
    Date = RTC->DR;

    m_Clock.Time.Hour   = LIB_2BcdDec(uint8_t(Time >> 16) & 0x3F);
    if((Time & RTC_TR_PM) != 0) m_Clock.Time.Hour += 12;
    m_Clock.Time.Minute = LIB_2BcdDec(uint8_t(Time >> 8) & 0x7F);
    m_Clock.Time.Second = LIB_2BcdDec(uint8_t(Time) & 0x7F);
    m_Clock.Date.Day    = LIB_2BcdDec(uint8_t(Date) & 0x3F);
    m_Clock.Date.Month  = LIB_2BcdDec(uint8_t(Date >> 8) & 0x1F);
    m_Clock.Date.Year   = LIB_2BcdDec(uint8_t(Date >> 16));
    m_Clock.DayOfWeek   = uint8_t(Date >> 13);
    if(m_Clock.DayOfWeek == 7)  m_Clock.DayOfWeek = 0;

    // ---------- Minute of the day ------------------------------------------

    m_Clock.MinuteOfDay = (uint16_t(m_Clock.Time.Hour) * 60) + uint16_t(m_Clock.Time.Minute);

    // ---------- Day of the year --------------------------------------------

    // Set the day number in the year(1 - 365/366)
    m_Clock.DayOfYear = m_DaysSoFar[m_Clock.Date.Month - 1] + m_Clock.Date.Day;

    // ---------- Leap year --------------------------------------------------

    // Add one day for any month after february on a leap year
    if(((m_Clock.Date.Year & 0x03) == 0) && (m_Clock.Date.Month > 2))
    {
        // Add a day
        m_Clock.DayOfYear++;
    }
    // ---------- Week of the year -------------------------------------------

    // Set the week in the year (1 - 52) using day of the week
    m_Clock.WeekOfYear = 7 - m_Clock.DayOfWeek;
    m_Clock.WeekOfYear = (m_Clock.DayOfYear + m_Clock.WeekOfYear) / 7;
    m_Clock.WeekOfYear++;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: WaitForSynchro
//
//   Parameter(s):  None
//   Return value:  SystemState_e    State
//
//   Description:   Waits until the RTC Time and Date registers are synchronized
//
//-------------------------------------------------------------------------------------------------
SystemState_e CRTC::WaitForSynchro(void)
{
    SystemState_e State;

    RTC->ISR &= uint32_t(~RTC_ISR_RSF);                         // Clear RSF flag

    // Wait the registers to be synchronised
    m_TimeOut = 10;
    do
    {
        OS_TaskYield();
    }
    while(((RTC->ISR & RTC_ISR_RSF) == 0) && (m_TimeOut != 0));

    if(m_TimeOut == 0) State = TIME_OUT;
    else               State = SYS_READY;

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   IRQ Handler:   WakeUp_IRQ_Handler
//                  Stamp_IRQ_Handler
//                  Alarm_IRQ_Handler
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   IRQ Handler of the RTC module
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CRTC::WakeUp_IRQ_Handler(void)
{

}

void CRTC::Stamp_IRQ_Handler(void)
{

}

void CRTC::Alarm_IRQ_Handler(void)
{

}

//-------------------------------------------------------------------------------------------------

#endif // USE_RTC_DRIVER == DEF_ENABLED
