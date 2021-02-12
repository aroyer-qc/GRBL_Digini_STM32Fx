//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_gpio_MAX4598.h
//
//*************************************************************************************************

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_class_gpio.h"

//-------------------------------------------------------------------------------------------------
// typedef(s)
//-------------------------------------------------------------------------------------------------

enum MAX4598_e
{
    MAX4598_SELECT_NO1_GND = 0x00,
    MAX4598_SELECT_NO2_GND = 0x01,
    MAX4598_SELECT_NO3_GND = 0x02,
    MAX4598_SELECT_NO4_GND = 0x03,
    MAX4598_SELECT_NO5_GND = 0x04,
    MAX4598_SELECT_NO6_GND = 0x05,
    MAX4598_SELECT_NO7_GND = 0x06,
    MAX4598_SELECT_NO8_GND = 0x07,
    MAX4598_SELECT_NO1_NO2 = 0x08,
    MAX4598_SELECT_NO3_NO4 = 0x09,
    MAX4598_SELECT_NO5_NO6 = 0x0A,
    MAX4598_SELECT_NO7_NO8 = 0x0B,
    MAX4598_SELECT_GND_GND = 0x0C,
    MAX4598_SELECT_VCC_GND = 0x0D,
    MAX4598_SELECT_NO8_N08 = 0x0E,
    MAX4598_SELECT_HIZ_HIZ = 0x0F,
};

struct MAX4598_PinStruct_t
{
    IO_Output_e            GPIO_A0;
    IO_Output_e            GPIO_A1;
    IO_Output_e            GPIO_A2;
    IO_Output_e            GPIO_A3;
    IO_Output_e            GPIO_Enable;
    IO_Output_e            GPIO_Latch;
};

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class MAX4598
{
    public:

        SystemState_e   Initialize                      (void* pArg);
        void            Reset                           (void);
        void            SetChannel                      (MAX4598_e Channel);

    private:

        MAX4598_PinStruct_t*        m_pPinStruct;
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

extern class   MAX4598                     SWITCH_MAX4598;

#ifdef LIB_DS3502_GLOBAL
 class   MAX4598                        SWITCH_MAX4598;
#endif

//-------------------------------------------------------------------------------------------------

