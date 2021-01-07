//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F7_isr.h
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "stm32f7xx.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define NVIC_PRIORITYGROUP_0    ((uint32_t)0x7U) // 0 bits pre-emption priority 4 bits subpriority
#define NVIC_PRIORITYGROUP_1    ((uint32_t)0x6U) // 1 bits pre-emption priority 3 bits subpriority
#define NVIC_PRIORITYGROUP_2    ((uint32_t)0x5U) // 2 bits pre-emption priority 2 bits subpriority
#define NVIC_PRIORITYGROUP_3    ((uint32_t)0x4U) // 3 bits pre-emption priority 1 bits subpriority
#define NVIC_PRIORITYGROUP_4    ((uint32_t)0x3U) // 4 bits pre-emption priority 0 bits subpriority

#define ISR_Enable()                            __asm volatile("cpsie i")
#define ISR_Disable()                           __asm volatile("cpsid i")

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

struct ISR_Prio_t
{
    uint32_t PriorityGroup;
    uint8_t  PremptionPriority;
    uint8_t  SubPriority;
};

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

void ISR_Initialize     (void);
void ISR_Init           (IRQn_Type Channel, const ISR_Prio_t* pPrio);

//-------------------------------------------------------------------------------------------------
