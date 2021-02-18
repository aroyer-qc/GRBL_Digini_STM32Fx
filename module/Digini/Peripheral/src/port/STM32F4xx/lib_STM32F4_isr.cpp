//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F4_isr.cpp
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include "lib_STM32F4_isr.h"

//-------------------------------------------------------------------------------------------------
// public function
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Function:       ISR_Init
//
//  Parameter(s):   Channel     ISR Channel to init
//                  pPrio       Pointer on the structure containing ISR priority configuration
//  Return:         none
//
//  Description:    Initialization of ISR vector
//
//-------------------------------------------------------------------------------------------------
void ISR_Init(IRQn_Type Channel, const ISR_Prio_t* pPrio)
{
    // Enable interrupt
    if(p_Prio != NULL)
    {
        NVIC_SetPriority(Channel, NVIC_EncodePriority(p_Prio->IsrPriorityGroup,
                                  p_Prio->IsrPremptionPriority,
                                  p_Prio->IsrSubPriority));
        NVIC_EnableIRQ(Channel);
    }
}

//-------------------------------------------------------------------------------------------------