/*
  SpindleControl.c - Spindle control methods
  Part of Grbl-Advanced

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud
  Copyright (c) 2017-2020 Patrick F.

  Grbl-Advanced is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl-Advanced is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl-Advanced.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "lib_digini.h"
#include "Protocol.h"
#include "Settings.h"
#include "System.h"
#include "GCode.h"
#include "SpindleControl.h"
#include "Config.h"


static float pwm_gradient; // Precalulated value to speed up rpm to PWM conversions.
static bool SpindleEnabled = false;
static bool SpindleDirectionClockwise = true;

//extern uint32_t spindle_rpm;


void Spindle_Initialize(void)
{
    IO_PinInit(IO_SPINDLE_ENABLE);
    IO_PinInit(IO_SPINDLE_DIRECTION);
    IO_PinInit(IO_SPINDLE_PWM);

    // Configure variable Spindle PWM and enable pin, if required, PWM and enable are
    // combined unless configured otherwise.

	/* GPIO Configuration:  */
/* TODO identified what it was<- this is IO for speed encoder ...
    if(Config.LatheModeEnable == false)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
*/

    // if(Setting.UseSpindleCounter == true)        // TODO not only for LATHE, we also want it for CNC machine
    {
        mySpindleCounter.Initialize();
    }

    if(Config.VariableSpindleEnable == true)
    {
        pwm_gradient = SPINDLE_PWM_RANGE/(Settings.rpm_max-Settings.rpm_min);

        // add the timer pwm init for the spindle speed
        // TODO add option for servo type spindle with brushless motor

    }

    SpindleDirectionClockwise = true;
    Spindle_Stop();
}


// Disables the Spindle and sets PWM output to zero when PWM variable Spindle speed is enabled.
// Called by various main program and ISR routines. Keep routine small, fast, and efficient.
// Called by spindle_init(), spindle_set_speed(), spindle_set_state(), and mc_reset().
void Spindle_Stop(void)
{
    // TODO AR Remove temp TIM1->CCR1 = TIM1_INIT; // Disable PWM. Output voltage is zero.
    IO_SetPin(IO_SPINDLE_ENABLE, SET_IO_SPINDLE_DISABLE);
    SpindleEnabled = false;
}

// Enables the Spindle
void Spindle_Start(void)
{
    IO_SetPin(IO_SPINDLE_ENABLE, SET_IO_SPINDLE_ENABLE);
    SpindleEnabled = true;
}


uint8_t Spindle_GetState(void)
{
    // Check if PWM is enabled.
    if(SpindleEnabled == true)
    {
        if(SpindleDirectionClockwise == false)
        {
            return SPINDLE_STATE_CCW;
        }
        else
        {
            return SPINDLE_STATE_CW;
        }
    }

    return SPINDLE_STATE_DISABLE;
}


// Sets Spindle speed PWM output and enable pin, if configured. Called by spindle_set_state()
// and stepper ISR. Keep routine small and efficient.
void Spindle_SetSpeed(uint8_t pwm_value)
{
  // TODO AR Remove temp  TIM1->CCR1 = TIM1_INIT - pwm_value; // Set PWM output level.
#ifdef SPINDLE_ENABLE_OFF_WITH_ZERO_SPEED
    if (pwm_value == SPINDLE_PWM_OFF_VALUE)
    {
        Spindle_Stop();
    }
    else
    {
        TIM1->CR1 |= TIM_CR1_CEN;//  TODO AR validate this TIM_Cmd(TIM1, ENABLE); // Ensure PWM output is enabled.
        Spindle_Start();
    }
#else
    if(pwm_value == SPINDLE_PWM_OFF_VALUE)
    {
        // TODO AR Remove temp  TIM1->CCR1 = TIM1_INIT;    // Disable PWM. Output voltage is zero.
        TIM1->CR1 &= (uint16_t)~TIM_CR1_CEN; // Disable PWM. Output voltage is zero.
        SpindleEnabled = false;
    }
    else
    {
        TIM1->CR1 |= TIM_CR1_CEN; // Ensure PWM output is enabled.
        SpindleEnabled = true;
    }
#endif
}


uint32_t Spindle_GetRPM(void)
{
    uint32_t Count;

    Count = mySpindleCounter.GetPulseCount();


    // Calculate RPM


    return Count;
}


// Called by spindle_set_state() and step segment generator. Keep routine small and efficient.
uint16_t Spindle_ComputePwmValue(float rpm) // 328p PWM register is 8-bit.
{
    uint8_t pwm_value;

    rpm *= (0.010*System.spindle_speed_ovr); // Scale by Spindle speed override value.

    // Calculate PWM register value based on rpm max/min settings and programmed rpm.
    if((Settings.rpm_min >= Settings.rpm_max) || (rpm >= Settings.rpm_max))
    {
        // No PWM range possible. Set simple on/off Spindle control pin state.
        System.spindle_speed = Settings.rpm_max;
        pwm_value = SPINDLE_PWM_MAX_VALUE;
    }
    else if(rpm <= Settings.rpm_min)
    {
        if(rpm == 0.0)   // S0 disables Spindle
        {
            System.spindle_speed = 0.0;
            pwm_value = SPINDLE_PWM_OFF_VALUE;
        }
        else   // Set minimum PWM output
        {
            System.spindle_speed = Settings.rpm_min;
            pwm_value = SPINDLE_PWM_MIN_VALUE;
        }
    }
    else
    {
        // Compute intermediate PWM value with linear Spindle speed model.
        // NOTE: A nonlinear model could be installed here, if required, but keep it VERY light-weight.
        System.spindle_speed = rpm;
        pwm_value = floor((rpm-Settings.rpm_min)*pwm_gradient) + SPINDLE_PWM_MIN_VALUE;
    }

    return pwm_value;
}


// Immediately sets Spindle running state with direction and Spindle rpm via PWM, if enabled.
// Called by g-code parser spindle_sync(), parking retract and restore, g-code program end,
// sleep, and Spindle stop override.
void Spindle_SetState(uint8_t state, float rpm)
{
    if(System.Abort == true)
    {
        // Block during abort.
        return;
    }

    if(state == SPINDLE_DISABLE)   // Halt or set Spindle direction and rpm.
    {
        System.spindle_speed = 0.0;
        Spindle_Stop();
    }
    else
    {
        if(state == SPINDLE_ENABLE_CW)
        {
            IO_SetPinLow(IO_SPINDLE_DIRECTION);
            SpindleDirectionClockwise = true;
        }
        else
        {
            IO_SetPinHigh(IO_SPINDLE_DIRECTION);
            SpindleDirectionClockwise = false;
        }

        Spindle_Start();

        // NOTE: Assumes all calls to this function is when Grbl is not moving or must remain off.
        if(Settings.flags & BITFLAG_LASER_MODE)
        {
            if(state == SPINDLE_ENABLE_CCW)
            {
                // TODO: May need to be rpm_min*(100/MAX_SPINDLE_SPEED_OVERRIDE);
                rpm = 0.0;
            }
        }

        Spindle_SetSpeed(Spindle_ComputePwmValue(rpm));
    }

    System.report_ovr_counter = 0; // Set to report change immediately
}


// G-code parser entry-point for setting Spindle state. Forces a planner buffer sync and bails
// if an abort or check-mode is active.
void Spindle_Sync(uint8_t state, float rpm)
{
    if(System.State == STATE_CHECK_MODE)
    {
        return;
    }

    Protocol_BufferSynchronize(); // Empty planner buffer to ensure Spindle is set when programmed.
    Spindle_SetState(state, rpm);
}


void Spindle_SetSurfaceSpeed(float x_pos)
{
    if(isEqual_f(x_pos, 0.0))
    {
        x_pos = 0.5;
    }
    float u = (fabs(x_pos) * 2) * M_PI;
    float rpm = gc_state.spindle_speed / (u / 1000);

    // Limit Max RPM
    if(gc_state.spindle_limit > 0)
    {
        rpm = AbsMin(rpm, gc_state.spindle_limit);
    }
    System.spindle_speed = rpm;

    if(System.State == STATE_CHECK_MODE)
    {
        return;
    }

    Spindle_SetSpeed(Spindle_ComputePwmValue(rpm));
}
