//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_dac.cpp
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

//-------------------------------------------------------------------------------------------------

#if USE_DAC_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
/*

static void                 SOUND_DAC_AndSoundInit(void);
static void                 SOUND_DAC_ISR_Handler(void);

// Sound variable
static int16_t*               SOUND_pHighSoundAddress;                                              // original copy if loopback
static int16_t*               SOUND_pLowSoundAddress;
static volatile uint32_t       SOUND_dwHighSoundSample                       = 0;
static volatile uint32_t       SOUND_dwLowSoundSample                        = 0;
static uint32_t                SOUND_dwHighStatus;
static uint32_t                SOUND_dwLowStatus;
static int16_t*               SOUND_pHighInISR_Address;
static int16_t*               SOUND_pLowInISR_Address;
static uint32_t                SOUND_dwHighInISR_Sample                      = 0;
static uint32_t                SOUND_dwLowInISR_Sample                       = 0;
static int32_t                 SOUND_lVolumeHighPrio;
static int32_t                 SOUND_lVolumeLowPrio;
static uint8_t                 SOUND_byRaiseVolumeHighPrio;
static uint8_t                 SOUND_byRaiseVolumeLowPrio;
static volatile uint8_t        SOUND_byInPlayHigh                            = SOUND_PRIO_LOW;
static volatile uint8_t        SOUND_byInPlayLow                             = SOUND_PRIO_LOW;

//-------------------------------------------------------------------------------------------------
//
//  Name:           SOUND_Init
//
//  Parameter(s):   none.
//
//  Return:         none.
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CSound::Init()
{
    uint32_t  pclk_freq;
    uint32_t  rld_cnts;

  #ifdef NXP_LPC1788_TM50

    IOCON_P0_30_bit.FUNC    = 0;                                            // shutdown
    FIO0CLR_bit.P0_30       = 1;
    FIO0DIR_bit.P0_30       = 1;

    IOCON_P0_26_bit.FUNC    = 2;                                            // AOUT function on pin
    IOCON_P0_26_bit.DACEN   = 1;                                            // DAC Enable
    IOCON_P0_26_bit.MODE    = 0;                                            // No Pull-Up or Pull-Down on DAC pin

    pclk_freq               = BSP_PC_OSC_FREQ;                              // Get the peripheral clock frequency.
    rld_cnts                = pclk_freq / 22050;
    DACCNTVAL               = rld_cnts;

    DMACINTTCCLEAR_bit.INTTCCLEAR2 = 1;                                     // Clear interrupt flag
    DMACINTERRCLR_bit.INTERRCLR2   = 1;
    DMACC2CONFIGURATION_bit.E      = 1;

    DACCTRL_bit.CNT_ENA     = 1;
    DACCTRL_bit.DBLBUF_ENA  = 1;

    PCONP_bit.PCTIM1        = 1;                                            // Enable power on module
                                                                            // VIC timer #1 init.
    T1TCR_bit.CR            = 1;                                            // Disable and reset ctr 0 and the prescale ctr 0.
    T1TCR_bit.CR            = 0;                                            // Clear the reset bit.
    T1PR                    = 0;                                            // Prescaler is set to no division.
    pclk_freq               = BSP_PC_OSC_FREQ;                              // Get the peripheral clock frequency.
    rld_cnts                = pclk_freq / 22050;                            // Calculate the nbr of cnts necessary
    T1MR0                   = rld_cnts;

    T1MCR_bit.MR0I          = 1;                                            // Interrupt on MR0 (reset TC), stop TC.
    T1MCR_bit.MR0R          = 1;
    T1CCR                   = 0;                                            // Capture is disabled.
    T1EMR                   = 0;                                            // No external match output.
    T1TCR_bit.CE            = 1;                                            // Enable timer 1.

    BSP_RegisterISR(NVIC_TIMER1, BSP_ISR_Priority_DAC, SOUND_DAC_ISR_Handler);
    BSP_EnableISR(NVIC_TIMER1);
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           SOUND_DAC_ISR_Handler
//
//  Parameter(s):   none.
//
//  Return:         none.
//
//  Description:    Handle DAC streaming.
//
//  Note(s):        1. This is a ISR.
//
//-------------------------------------------------------------------------------------------------

void SOUND_DAC_ISR_Handler(void)
{
    uint32_t        AdjustedValueLOW;
    uint32_t        AdjustedValueHIGH;
    uint32_t        NextAdjustedValue;
    int16_t         Point1;
    int16_t         Point2;
    static int16_t  Pente              = 0;     // put as member
    static uint8_t  Ratio             = 1;      // put as member
    uint8_t         Shutdown          = 0;

    T1IR  = 0xFF;                                                                   // Clear timer #1 interrupt.

    AdjustedValueLOW.l = 0;

    if(HighStatus > BSP_PLAY_STOP_STANDBY)
    {
        if((HighSoundSample != 0) && (HighInISR_Sample == 0))           // Update pointer and counter if HIGH sound priority exist
        {
            HighInISR_Sample   = HighSoundSample;
            pHighInISR_Address = pHighSoundAddress;
        }
    }
    else
    {
        if(HighStatus > BSP_PLAY_STOP)
        {
            Point1 = *pHighInISR_Address;
            Point2 = *(pHighInISR_Address + 1);

            if(((Point1 <= 0) && (Point2 >= 0)) ||
               ((Point1 >= 0) && (Point2 <= 0)))
            {
                HighStatus = SOUND_PLAY_STOP;
            }
        }
        else   // Full stop
        {
            HighInISR_Sample = 0;
            HighSoundSample  = 0;
            Shutdown        |= 0x01;
        }
    }

    if(LowStatus > SOUND_PLAY_STOP_STANDBY)
    {
        if((LowSoundSample != 0) && (LowInISR_Sample == 0))             // Update pointer and counter if LOW sound priority exist
        {
            LowInISR_Sample   = LowSoundSample;
            pLowInISR_Address = pLowSoundAddress;
        }
    }
    else
    {
        if(LowStatus > SOUND_PLAY_STOP)
        {
            Point1 = *pLowInISR_Address;
            Point2 = *(pLowInISR_Address + 1);

            if(((Point1 <= 0) && (Point2 >= 0)) ||
               ((Point1 >= 0) && (Point2 <= 0)))
            {
                LowStatus = SOUND_PLAY_STOP;
            }
        }
        else   // Full stop
        {
            LowInISR_Sample = 0;
            LowSoundSample  = 0;
            Shutdown       |= 0x02;
        }
    }

    if((Shutdown & 0x03) == 0x03)
    {
        FIO0CLR_bit.P0_30   = 1;
    }

    if(LowInISR_Sample != 0)
    {
        AdjustedValueLOW.l = (VolumeLowPrio * (int32_t)(*pLowInISR_Address)) / 31;

        if(HighInISR_Sample == 0)
        {
            if     (AdjustedValueLOW.l >  32767) AdjustedValueLOW.dw = 65535L;
            else if(AdjustedValueLOW.l < -32768) AdjustedValueLOW.dw = 0;
            else                                 AdjustedValueLOW.dw = (uint32_t)(AdjustedValueLOW.l + 32768);

            FIO0SET_bit.P0_30 = 1;
            DACR                = AdjustedValueLOW.dw | 0x00010000;                                                // Put low prio sound in DAC
        }
        else
        {
            if     (AdjustedValueLOW.l >  32767) AdjustedValueLOW.l =  32767;
            else if(AdjustedValueLOW.l < -32768) AdjustedValueLOW.l = -32768;
        }
    }

    if(HighInISR_Sample != 0)                                                                             // Do we have high priority sound to play
    {
        AdjustedValueHIGH.l = (VolumeHighPrio * (int32_t)(*pHighInISR_Address)) / 31;

        if(LowInISR_Sample != 0)
        {
            if     (AdjustedValueHIGH.l >  32767) AdjustedValueHIGH.l =  32767;
            else if(AdjustedValueHIGH.l < -32768) AdjustedValueHIGH.l = -32768;

            AdjustedValueHIGH.l += AdjustedValueLOW.l;                                                            // Sum of the Low and High of the actual sample

            if(Pente == 0)
            {
                if(LowInISR_Sample >= 2)                                                                  // Do we have high priority sound to play
                {
                    AdjustedValueLOW.l = (VolumeLowPrio * (int32_t)(*(LowInISR_Address + 1))) / 31;    // Next sample of adjust value LOW priority
                    if     (AdjustedValueLOW.l >  32767) AdjustedValueLOW.l =  32767;
                    else if(AdjustedValueLOW.l < -32768) AdjustedValueLOW.l = -32768;
                }
                else
                {
                    AdjustedValueLOW.l = 0;
                }

                if(HighInISR_Sample >= 2)                                                                 // Do we have high priority sound to play
                {
                    NextAdjustedValue.l = (VolumeHighPrio * (int32_t)(*(HighInISR_Address + 1))) / 31; // Next sample of adjust value HIGH priority
                    if     (NextAdjustedValue.l >  32767) NextAdjustedValue.l =  32767;
                    else if(NextAdjustedValue.l < -32768) NextAdjustedValue.l = -32768;
                }
                else
                {
                    NextAdjustedValue.l = 0;
                }

                NextAdjustedValue.l += AdjustedValueLOW.l;                                                        // Sum of the Low and High of the next sample

                if(AdjustedValueHIGH.l >= 28086)                                                                  // 950/1024 on high part off waveform
                {
                    Pente = NextAdjustedValue.l - AdjustedValueHIGH.l;
                }
                else if(AdjustedValueHIGH.l <= -28032)                                                            // 74/1024 on low part off waveform
                {
                    Pente = AdjustedValueHIGH.l - NextAdjustedValue.l;
                }

                if(Pente < 0)
                {
                    Pente = 0;
                }
                Ratio = (uint8_t)((((int32_t)Pente << 3) / 5120) + 1);

                if(Ratio > 8)
                {
                    Ratio = 8;
                }
            }
            else if((AdjustedValueHIGH.l < 28086) && (AdjustedValueHIGH.l > -28032))
            {
                Pente  = 0;
                Ratio = 1;
            }

            if(AdjustedValueHIGH.l >= 28086)
            {
                AdjustedValueHIGH.l = ((AdjustedValueHIGH.l - 28086) / Ratio) + 28086;
            }
            else if(AdjustedValueHIGH.l <= -28032)
            {
                AdjustedValueHIGH.l = ((AdjustedValueHIGH.l + 28032) / Ratio) - 28032;
            }
        }

        if     (AdjustedValueHIGH.l >  32767) AdjustedValueHIGH.dw = 65535L;
        else if(AdjustedValueHIGH.l < -32768) AdjustedValueHIGH.dw = 0;
        else                                  AdjustedValueHIGH.dw = (uint32_t)(AdjustedValueHIGH.l + 32768);

        FIO0SET_bit.P0_30 = 1;
        DACR                = AdjustedValueHIGH.dw | 0x00010000;                                                  // Put high prio sound in DAC
    }

    if(LowInISR_Sample != 0)
    {
        pLowInISR_Address++;
        LowInISR_Sample--;

        if(LowInISR_Sample == 0)                                                                          // If loop is completed, check for loop play
        {
            if(LowStatus == SOUND_PLAY_SINGLE)                                                              // Reset original variable to stop playback
            {
                LowSoundSample = 0;
                Stop(SOUND_PRIORITY_LOW);
            }
        }
    }

    if(HighInISR_Sample != 0)                                                                             // Do we have high priority sound to play
    {
        pHighInISR_Address++;
        dwHighInISR_Sample--;

        if(HighInISR_Sample == 0)                                                                         // If loop is completed, check for loop play
        {
            if(HighStatus == BSP_PLAY_SINGLE)                                                             // Reset original variable to stop playback
            {
                HighSoundSample = 0;
                Stop(SOUND_PRIORITY_HIGH);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//   function name: Start
//
//   parameter(s):  uint8_t    Number    Number of the sound to play
//                  uint8_t    Mode      Playback mode               BSP_PLAY_SINGLE
//                                                                   BSP_PLAY_LOOP
//                  uint8_t    Priority  Channel priority            SOUND_PRIORITY_HIGH
//                                                                   SOUND_PRIORITY_LOW
//                  uint8_t    Level     Sound priority level for application level
//   return value:  none
//
//   description:
//
//   note:
//
//-------------------------------------------------------------------------------------------------
// ARGO TEMPO
uint8_t VolHighPrio;
uint8_t VolLowPrio;

uint8_t CSound::Start(uint8_t Number, uint8_t Mode, uint8_t Priority, uint8_t Level)
{
    sWaveDescriptor     Wave;
    OS_CPU_SR           cpu_sr;
    uint8_t             Result = FALSE;

SOUND_lVolumeHighPrio = VolHighPrio;
SOUND_lVolumeLowPrio  = VolLowPrio;

    if(Flag.b.SoundAreLoaded == YES)
    {
        if(GLCD_GetSoundInfo(&Wave, byNumber) == ERR_NONE)
        {
            if(Wave.dwNbSamples > 0)
            {
                if(Priority == SOUND_PRIORITY_HIGH)
                {
                    if(Level <= SOUND_InPlayHigh)
                    {
                        OS_ENTER_CRITICAL();
                        HighStatus          = Mode;
                        pHighSoundAddress   = (int16_t*)Wave.pAddress;
                        HighSoundSample     = Wave.NbSamples;
                        OS_EXIT_CRITICAL();
                        InPlayHigh          = Level;
                        RaiseVolumeHighPrio = VolumeHighPrio;
                        Result              = TRUE;
                    }
                }
                else
                {
                    if(Flag.b.MuteStatus == FALSE)
                    {
                        if(Level <= InPlayLow)
                        {
                            OS_ENTER_CRITICAL();
                            LowStatus          = Mode;
                            pLowSoundAddress   = (int16_t*)Wave.pAddress;
                            LowSoundSample     = Wave.NbSamples;
                            OS_EXIT_CRITICAL();
                            InPlayLow          = Level;
                            RaiseVolumeLowPrio = VolumeLowPrio;
                            Result             = TRUE;
                        }
                    }
                }
            }
        }
    }
    return Result;
}

//-------------------------------------------------------------------------------------------------
//
//   function name: Stop
//
//   parameter(s):  uint8_t  Priority
//
//   return value:  none
//
//   description:   will stop the sound in progress if it is running
//
//   note:          If a low priority sound was running prior to the start of a high priority,
//                  it will be reenable unless it has ended.
//
//-------------------------------------------------------------------------------------------------
void CSound::Stop(uint8_t Priority)
{
    if(Priority == SOUND_PRIORITY_HIGH)
    {
        RaiseVolumeHighPrio     = 0;
        InPlayHigh              = SOUND_PRIO_LOW;
        HighStatus              = BSP_PLAY_STOP;
    }
    else
    {
        RaiseVolumeLowPrio      = 0;
        InPlayLow               = SOUND_PRIO_LOW;
        LowStatus               = BSP_PLAY_STOP;
    }
}

//-------------------------------------------------------------------------------------------------
//
//   function name: RaiseVolumeTemporary
//
//   parameter(s):
//
//   return value:
//
//   description:   By calling this function you can raise volume of actual sound, this is useful
//                  for example: Alarm clock -> they can increment if user is not waking up
//
//   note:
//
//-------------------------------------------------------------------------------------------------
void CSound::RaiseVolumeTemporary(uint8_t Priority)
{
    if(Priority == SOUND_PRIORITY_HIGH)
    {
        if(RaiseVolumeHighPrio < SOUND_VOLUME_MASK)
        {
            RaiseVolumeHighPrio++;
        }
        VolumeHighPrio = SoundVolume[RaiseVolumeHighPrio & SOUND_VOLUME_MASK];
    }
    else
    {
        if(RaiseVolumeLowPrio < SOUND_VOLUME_MASK)
        {
            RaiseVolumeLowPrio++;
        }
        VolumeLowPrio = SoundVolume[RaiseVolumeLowPrio & SOUND_VOLUME_MASK];
    }
}

//-------------------------------------------------------------------------------------------------
//
//   function name: Volume
//
//   parameter(s):
//
//   return value:
//
//   description:
//
//   note:
//
//-------------------------------------------------------------------------------------------------
void CSound::Volume(uint8_t Volume, uint8_t Priority)
{
    if(Volume < 11)
    {
        Volume = SoundVolume[Volume];

        if(Priority == SOUND_PRIORITY_HIGH)
        {
            VolHighPrio = Volume;
        }
        else
        {
            VolLowPrio = Volume;
        }
    }
}


*/


#endif // USE_DAC_DRIVER == DEF_ENABLED
