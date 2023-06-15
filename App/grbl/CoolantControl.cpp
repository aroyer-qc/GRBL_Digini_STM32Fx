/*
  CoolantControl.c - coolant control methods
  Part of Grbl-Advanced

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2017 Patrick F.

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
#include "System.h"
#include "Protocol.h"
#include "GCode.h"
#include "CoolantControl.h"
#include "Config.h"

void Coolant_Initialize(void)
{
  	IO_PinInit(IO_COOLANT);
  	IO_PinInit(IO_COOLANT_FLOOD);
  	IO_PinInit(IO_COOLANT_MIST);
}

// Directly called by coolant_init(), coolant_set_state(), and mc_reset(), which can be at
// an interrupt-level. No report flag set, but only called by routines that don't need it.
void Coolant_Stop(void)
{
  #ifdef INVERT_COOLANT_FLOOD_PIN
    IO_SetPinHigh(IO_COOLANT_FLOOD);
  #else
    IO_SetPinLow(IO_COOLANT_FLOOD);
  #endif

  #ifdef ENABLE_M7
   #ifdef INVERT_COOLANT_MIST_PIN
    IO_SetPinHigh(IO_COOLANT_MIST);
   #else
    IO_SetPinLow(IO_COOLANT_MIST);
   #endif
  #endif
}

// Returns current coolant output state. Overrides may alter it from programmed state.
uint8_t Coolant_GetState(void)
{
    uint8_t cl_state = COOLANT_STATE_DISABLE;

  #ifdef INVERT_COOLANT_FLOOD_PIN
    if(IO_GetInputPin(IO_COOLANT_FLOOD) == 0)
    {
  #else
    if(IO_GetInputPin(IO_COOLANT_FLOOD) == 1)
    {
  #endif
        cl_state |= COOLANT_STATE_FLOOD;
    }

  #ifdef ENABLE_M7
   #ifdef INVERT_COOLANT_MIST_PIN
    if(IO_GetInputPin(IO_COOLANT_MIST) == 0)
    {
   #else
    if(IO_GetInputPin(IO_COOLANT_MIST) == 1)
    {
   #endif
        cl_state |= COOLANT_STATE_MIST;
    }
  #endif

    return cl_state;
}


// Main program only. Immediately sets flood coolant running state and also mist coolant,
// if enabled. Also sets a flag to report an update to a coolant state.
// Called by coolant toggle override, parking restore, parking retract, sleep mode, g-code
// parser program end, and g-code parser coolant_sync().
void Coolant_SetState(uint8_t mode)
{
    if(System.abort)
    {
        // Block during abort.
        return;
    }

    if(mode & COOLANT_FLOOD_ENABLE)
    {
      #ifdef INVERT_COOLANT_FLOOD_PIN
        IO_SetPinLow(IO_COOLANT_FLOOD);
      #else
        IO_SetPinHigh(IO_COOLANT_FLOOD);
      #endif
    }
    else
    {
      #ifdef INVERT_COOLANT_FLOOD_PIN
        IO_SetPinHigh(IO_COOLANT_FLOOD);
      #else
        IO_SetPinLow(IO_COOLANT_FLOOD);
      #endif
    }

  #ifdef ENABLE_M7
    if(mode & COOLANT_MIST_ENABLE)
    {
      #ifdef INVERT_COOLANT_MIST_PIN
        IO_SetPinLow(IO_COOLANT_MIST);
      #else
        IO_SetPinHigh(IO_COOLANT_MIST);
      #endif
    }
    else
    {
      #ifdef INVERT_COOLANT_MIST_PIN
        IO_SetPinHigh(IO_COOLANT_MIST);
      #else
        IO_SetPinLow(IO_COOLANT_MIST);
      #endif
    }
  #endif

    System.report_ovr_counter = 0; // Set to report change immediately
}


// G-code parser entry-point for setting coolant state. Forces a planner buffer sync and bails
// if an abort or check-mode is active.
void Coolant_Sync(uint8_t mode)
{
    if(System.state == STATE_CHECK_MODE)
    {
        return;
    }

    Protocol_BufferSynchronize(); // Ensure coolant turns on when specified in program.
    Coolant_SetState(mode);
}
