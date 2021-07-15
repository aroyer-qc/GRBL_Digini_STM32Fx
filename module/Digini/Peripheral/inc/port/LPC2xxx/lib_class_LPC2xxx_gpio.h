//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_gpio.h
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


#ifndef __STM32F4_GPIO_H__
#define __STM32F4_GPIO_H__

#ifdef STM32F4_GPIO_GLOBAL
    #define STM32F4_GPIO_EXTERN
#else
    #define STM32F4_GPIO_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "bsp_cfg.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"

//-------------------------------------------------------------------------------------------------
// define(s) and macro(s)
//-------------------------------------------------------------------------------------------------

// TODO change this has no busuiness here
#define LED_Init(l)             IO_OutputInit(l)
#define LED_ON(o)               IO_Output(o, IO_SET)
#define LED_OFF(o)              IO_Output(o, IO_RESET)
#define LED_Toggle(o)           IO_OutputToggle(o)

//#define Add_IRQ(d)              IRQ_##NAME
//-------------------------------------------------------------------------------------------------
// struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

enum ActiveState_e
{
    IO_SET_HIGH,
    IO_SET_LOW,
};

enum PinState_e
{
    IO_SET,
    IO_RESET,
};

// To found how many Input
enum IO_Input_e
{
  #define X_IO_IRQ(ENUM_ID, PREEMP_PRIO, PORT, PIN, CLOCK, SOURCE_PORT, SOURCE_PIN, LINE, VECTOR, TRIGGER, PULL, ACTIVE_STATE) ENUM_ID,
    IO_IRQ_DEF
  #undef X_IO_IRQ

  #define X_IO_INPUT(ENUM_ID, PORT, PIN, CLOCK, PULL, ACTIVE_STATE) ENUM_ID,
    IO_INPUT_DEF
  #undef X_IO_INPUT

    NB_INPUT_CONST
};

// To found how many Output
enum IO_Output_e
{
  #define X_IO_OUTPUT(ENUM_ID, PORT, PIN, CLOCK, OUTPUT_TYPE, PULL, ACTIVE_STATE, INIT_STATE) ENUM_ID,
    IO_OUTPUT_DEF
  #undef X_IO_OUTPUT
    NB_OUTPUT_CONST
};

// To found how many GPIO_IRQ
enum IO_IRQ_e
{
  #define X_IO_IRQ(ENUM_ID, PREEMP_PRIO, PORT, PIN, CLOCK, SOURCE_PORT, SOURCE_PIN, LINE, VECTOR, TRIGGER, PULL, ACTIVE_STATE) IRQ_##ENUM_ID,
    IO_IRQ_DEF
  #undef X_IO_IRQ
    NB_IRQ_CONST
};

struct IO_Input_t
{
    GPIO_TypeDef*       pPort;
    uint16_t            Pin;
    uint32_t            Clock;
    GPIOPuPd_TypeDef    Pull;
    ActiveState_e       ActiveState;
};

struct IO_Output_t
{
    GPIO_TypeDef*       pPort;
    uint16_t            Pin;
    uint32_t            Clock;
    GPIOOType_TypeDef   Type;
    GPIOPuPd_TypeDef    Pull;
    ActiveState_e       ActiveState;
    PinState_e          InitState;
};

struct IO_IRQ_t
{
    uint8_t             PreempPrio;
    uint8_t             PortSource;
    uint8_t             PinSource;
    uint32_t            Line;
    IRQn_Type           IRQn;
    EXTITrigger_TypeDef Trigger;
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef STM32F4_GPIO_GLOBAL

  IO_Input_t IO_In[NB_INPUT_CONST] =
  {
    #define X_IO_IRQ( ENUM_ID, PREEMP_PRIO, PORT, PIN, CLOCK, SOURCE_PORT, SOURCE_PIN, LINE, VECTOR, TRIGGER, PULL, ACTIVE_STATE ) \
                                          { PORT, PIN, CLOCK,                                                 PULL, ACTIVE_STATE },
      IO_IRQ_DEF
    #undef X_IO_IRQ

    #define X_IO_INPUT( ENUM_ID, PORT, PIN, CLOCK, PULL, ACTIVE_STATE )  \
                               { PORT, PIN, CLOCK, PULL, ACTIVE_STATE },
      IO_INPUT_DEF
    #undef X_GPIO_INPUT
  };

  IO_Output_t IO_Out[NB_OUTPUT_CONST] =
  {
    #define X_IO_OUTPUT( ENUM_ID, PORT, PIN, AHB_CLOCK, OUTPUT_TYPE, PULL_TYPE, ACTIVE_STATE, INIT_STATE )  \
                                { PORT, PIN, AHB_CLOCK, OUTPUT_TYPE, PULL_TYPE, ACTIVE_STATE, INIT_STATE },
      IO_OUTPUT_DEF
    #undef X_IO_OUTPUT
  };

  IO_IRQ_t IO_IRQ[NB_IRQ_CONST] =
  {
    #define X_IO_IRQ( ENUM_ID, PREEMP_PRIO, PORT, PIN, CLOCK, SOURCE_PORT, SOURCE_PIN, LINE,            VECTOR, TRIGGER, PULL, ACTIVE_STATE) \
                             { PREEMP_PRIO,                   SOURCE_PORT, SOURCE_PIN, LINE, (IRQn_Type)VECTOR, TRIGGER },
      IO_IRQ_DEF
    #undef X_IO_IRQ
  };

#else

extern IO_Input_t   IO_In  [NB_INPUT_CONST];
extern IO_Output_t  IO_Out [NB_OUTPUT_CONST];
extern IO_IRQ_t     IO_IRQ [NB_IRQ_CONST];

#endif

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

void        IO_InputInit                (IO_Input_e Input);
void        IO_OutputInit               (IO_Output_e Output);
void        IO_IRQ_Init                 (IO_Input_e Input);
PinState_e  IO_Input                    (IO_Input_e Input);
void        IO_Output                   (IO_Output_e Output, PinState_e State);
void        IO_OutputToggle             (IO_Output_e Output);
void        IO_ChangeInputActiveState   (IO_Input_e Input, ActiveState_e ActiveState);
void        IO_ChangeOutputActiveState  (IO_Output_e Output, ActiveState_e ActiveState);

#endif
