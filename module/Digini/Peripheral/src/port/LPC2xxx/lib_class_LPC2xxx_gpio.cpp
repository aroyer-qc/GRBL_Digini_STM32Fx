//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_gpio.cpp
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
#define STM32F4_GPIO_GLOBAL
#include "lib_class_STM32F4_gpio.h"
#undef  STM32F4_GPIO_GLOBAL

//-------------------------------------------------------------------------------------------------
//
//  Name:           IO_OutputInit
//
//  Parameter(s):   IO_Input_e Input      Specified the input to configured
//  Return:         none
//
//  Description:    Initialize selected GPIO to output
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void IO_InputInit(IO_Input_e Input)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

   // GPIO Periph clock enable
    RCC_AHB1PeriphClockCmd(IO_In[Input].Clock, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = IO_In[Input].Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd  = IO_In[Input].Pull;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IO_In[Input].pPort, &GPIO_InitStructure);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           IO_OutputInit
//
//  Parameter(s):   IO_Output Output      Specified the output to configured and control
//  Return:         none
//
//  Description:    Initialize selected IO to output
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void IO_OutputInit(IO_Output_e Output)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    // Enable the GPIO Clock
    RCC_AHB1PeriphClockCmd(IO_Out[Output].Clock, ENABLE);

    // Configure the GPIO pin
    GPIO_InitStructure.GPIO_Pin   = IO_Out[Output].Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = IO_Out[Output].Type;
    GPIO_InitStructure.GPIO_PuPd  = IO_Out[Output].Pull;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IO_Out[Output].pPort, &GPIO_InitStructure);

    IO_Output(Output, IO_Out[Output].InitState);
}


//-------------------------------------------------------------------------------------------------
//
//  Function:       IO_IRQ_Init
//
//  Parameter(s):   IO_IRQ_e Irq
//  Return:         none
//
//  Description:    Initialize this pin as IRQ and set NVIC accordingly
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void IO_IRQ_Init(IO_Input_e Input)
{
    uint32_t           Priority;
    EXTI_InitTypeDef   EXTI_InitStructure;
    IO_IRQ_t*           pIO_IRQ;


    IO_InputInit(Input);

    if((IO_IRQ_e)Input < NB_IRQ_CONST)                                           // it need to be a define IRQ line to be init that way
    {                                                                           // If not, it is only a normal input line!
        pIO_IRQ = &IO_IRQ[(IO_IRQ_e)Input];

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);                  // Enable SYSCFG clock
        SYSCFG_EXTILineConfig(pIO_IRQ->PortSource, pIO_IRQ->PinSource);         // Connect EXTI to pin

        EXTI_InitStructure.EXTI_Line    = pIO_IRQ->Line;                        // Configure EXTI
        EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = pIO_IRQ->Trigger;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        Priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), pIO_IRQ->PreempPrio, 0x00);
        NVIC_SetPriority(pIO_IRQ->IRQn, Priority);
        NVIC_EnableIRQ(pIO_IRQ->IRQn);
    }
}


//-------------------------------------------------------------------------------------------------
//
//  Function:       IO_Input
//
//  Parameter(s):   IO_Input_e       Input
//  Return:         PinState_e       State
//
//  Description:    Read pin and return satate according to config
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
PinState_e IO_Input(IO_Input_e Input)
{
    PinState_e State;

    if((IO_In[Input].pPort->IDR & IO_In[Input].Pin))
    {
        State = (IO_In[Input].ActiveState == IO_SET_HIGH) ? IO_SET : IO_RESET;
    }
    else
    {
        State = (IO_In[Input].ActiveState == IO_SET_HIGH) ? IO_RESET : IO_SET;
    }

    return State;
}


//-------------------------------------------------------------------------------------------------
//
//  Function:       IO_Output
//
//  Parameter(s):   IO_Output_e    Output
//                  PinState_e     State
//  Return:         none
//
//  Description:    set pin according to config
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void IO_Output(IO_Output_e Output, PinState_e State)
{
    if(IO_Out[Output].ActiveState == IO_SET_LOW)
    {
        State = (State == IO_SET) ? IO_RESET : IO_SET;
    }

    if(State == IO_SET) IO_Out[Output].pPort->BSRRL = IO_Out[Output].Pin;
    else                IO_Out[Output].pPort->BSRRH = IO_Out[Output].Pin;
}


//-------------------------------------------------------------------------------------------------
//
//  Function:       IO_OutputToggle
//
//  Parameter(s):   IO_Output_e    Output
//  Return:         none
//
//  Description:    Toggle pin
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void IO_OutputToggle(IO_Output_e Output)
{
    IO_Out[Output].pPort->ODR  ^= IO_Out[Output].Pin;
}


//-------------------------------------------------------------------------------------------------
//
//  Function:       IO_ChangeInputActiveState
//
//  Parameter(s):   IO_Input_e       Input
//                  ActiveState_e    ActiveState
//  Return:         none
//
//  Description:    Change the active state for the selected pin
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void IO_ChangeInputActiveState(IO_Input_e Input, ActiveState_e ActiveState)
{
    IO_In[Input].ActiveState = ActiveState;
}


//-------------------------------------------------------------------------------------------------
//
//  Function:       IO_ChangeOutputActiveState
//
//  Parameter(s):   IO_Output_e      Output
//                  ActiveState_e    ActiveState
//  Return:         none
//
//  Description:    Change the active state for the selected pin
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void IO_ChangeOutputActiveState(IO_Output_e Output, ActiveState_e ActiveState)
{
    IO_Out[Output].ActiveState = ActiveState;
}

//-------------------------------------------------------------------------------------------------

