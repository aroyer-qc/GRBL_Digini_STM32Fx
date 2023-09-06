//-------------------------------------------------------------------------------------------------
//
//  File : vt100_user_callback.cpp
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2023 Alain Royer.
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
// For all callback function here:
//
// Parameter Type
//
//
//         VT100_CALLBACK_INIT     -> Initialize part of the display in the menu, or variables in use
//                                  while in menu.
//
//                                  It is called only once when then menu is being draw item by
//                                  item.
//
//         VT100_CALLBACK_ON_INPUT -> This call is made when a key that select an item in the menu is
//                                  pressed.
//
//                                  If it is used for a display page with static or dynamic
//                                  informations, you need to return with a VT100_INPUT_ESCAPE.
//
//                                  If it is used for a modification in the actual menu, you only
//                                  perform the action and then return with the VT100_INPUT_MENU_CHOICE
//                                  to wait for a new input.
//
//         VT100_CALLBACK_REFRESH  -> Called while waiting for any key to be pressed. Use this to
//                                  refresh displayed information.
//                                  NOTE: There is no VT100_CALLBACK_REFRESH called if
//                                        VT100_INPUT_MENU_CHOICE is returned. UNLESS: you put a
//                                        callback in Option 0 of the menu list
//
//         VT100_CALLBACK_FLUSH    -> If variables need to be reset or function need to be called to
//                                  reset some app state. each existing callback will be called
//                                  leaving a menu.
//                                  Do not use any malloc function in the process of
//                                  VT100_CALLBACK_INIT/VT100_CALLBACK_FLUSH unless you manage that it
//                                  might already exist.
//
// return VT100_InputType_e     Kind of input management for this page
//
//
//         VT100_INPUT_ESCAPE      -> It is not a menu is this a page with information updated on
//                                  screen, this kind of is not waiting any key other than
//                                  <ESCAPE> to exit
//
//         VT100_INPUT_MENU_CHOICE -> It tell the console it is a menu and waiting for an input.
//
//         VT100_INPUT_DECIMAL     -> If the callback make call for an VT100_SetDecimalInput(), then
//                                  it must return with this enum value to tell the console what
//                                  to do. It will comeback to the callback only when the input
//                                  decimal mode will be escaped by <ESCAPE> or value validated by
//                                  <ENTER>.
//
//         VT100_INPUT_STRING      -> If the callback make call for an VT100_SetStringInput(), then it
//                                  must return with this enum value to tell the console what to
//                                  do. We will comeback to the callback only when the input
//                                  decimal mode will be escaped by <ESCAPE> or value validated
//                                  by <ENTER>.
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_digini.h"
#include "grbl_advance.h"
#include "ethernetif.h"
#include "lwip/netif.h"

//-------------------------------------------------------------------------------------------------

#if (DIGINI_USE_VT100_MENU == DEF_ENABLED)

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

// this is not the place for those Settings only what is generic should stay here


#define VT100_CFG_NO_REFRESH                                  0
#define VT100_CFG_REFRESH_INFO                                32768
#define VT100_CFG_REFRESH_ALL                                 65535
#define VT100_CFG_REFRESH_COMMON_MASK                         (32768 + 16384)

#define VT100_MISC_CFG_REFRESH_SET_POINT_LOW                  1
#define VT100_MISC_CFG_REFRESH_SET_POINT_HIGH                 2
#define VT100_MISC_CFG_REFRESH_SET_POINT_BACKUP_LOW_VOLTAGE   4
#define VT100_MISC_CFG_REFRESH_SET_POINT_POWER_LOW_VOLTAGE    8
#define VT100_MISC_CFG_REFRESH_SERIAL                         16
#define VT100_MISC_CFG_REFRESH_LOCATION                       32
#define VT100_MISC_CFG_REFRESH_SITE_ID                        64

//-------------------------------------------------------------------------------------------------
// Typedef(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Prototype(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Variable(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           PrintMenuStaticInfo
//
//  Description:    Print user header/footer or static display information
//
//  Note(s);        Be aware of your cursor location
//
//-------------------------------------------------------------------------------------------------
#if (VT100_USE_USER_MENU_STATIC_INFO == DEF_ENABLED)
void VT100_Terminal::PrintMenuStaticInfo(void)
{
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           VT100_CallbackInitialize
//
//  Description:    Callback Initialize
//
//  Note(s):        Put in this function what you want to be initialize only once at boot up
//
//-------------------------------------------------------------------------------------------------
#if (VT100_USER_CALLBACK_INITIALIZE == DEF_ENABLED)
void VT100_Terminal::CallbackInitialize(void)
{
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           CALLBACK_MenuRedirection
//
//  Description:    This redirect to proper menu according to configuration
//
//  Note(s):        In the present case, it redirect if we are at specific time in boot-up
//
//-------------------------------------------------------------------------------------------------
VT100_InputType_e VT100_Terminal::CALLBACK_MenuRedirection(uint8_t Input, VT100_CallBackType_e Type)
{
    VAR_UNUSED(Input);

    if(Type == VT100_CALLBACK_INIT)
    {
        if(myVT100.m_IsItInStartup == true)
        {
            myVT100.GoToMenu(MenuBoot_ID);
        }
        else
        {
            myVT100.GoToMenu(MenuMain_ID);
        }
    }

    return VT100_INPUT_MENU_CHOICE;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           CALLBACK_LedControl
//
//  Description:    Allowed the LED to be controlled in override mode
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
/*
VT100_InputType_e VT100_Terminal::CALLBACK_LedControl(uint8_t Input, VT100_CallBackType_e Type)
{
    uint8_t Led;

    Led = Input - 1;

    if(Type == VT100_CALLBACK_INIT)
    {
        if(Input == DOOR_CTRL_MENU_TITLE)
        {
            VT100_Generic_uint64 = 0;
        }
        else
        {
            IO_Ctrl(Led, STATE_CLEAR);
        }
    }

    myVT100.SetForeColor(VT100_COLOR_GREEN);

    if((((uint64_t)1 << Input) & VT100_Generic_uint64) != 0)
    {
        if(Type == VT100_CALLBACK_ON_INPUT)
        {
            VT100_Generic_uint64 &= ~((uint64_t)1 << Input);
            IO_Ctrl(Led, STATE_CLEAR);
        }
        else
        {
            IO_Ctrl(Led, STATE_SET);
        }
    }
    else
    {
        if(Type == VT100_CALLBACK_ON_INPUT)
        {
            VT100_Generic_uint64 |= ((uint64_t)1 << Input);
            IO_Ctrl(Led, STATE_SET);
        }
        else
        {
            IO_Ctrl(Led, STATE_CLEAR);
        }
    }

    if(Type == VT100_CALLBACK_REFRESH)
    {
        myVT100.SetForeColor(VT100_COLOR_GREEN);

        for(uint8_t i = 0; i <= 3; i++)
        {
            myVT100.SetCursorPosition(37, 10 + i);
            if((VT100_Generic_uint64 & ((uint64_t)1 << (i + 1))) == 0)
            {
                myVT100.InMenuPrintf(" ");
            }
            else
            {
                myVT100.InMenuPrintf("*");
            }
        }
    }
    return VT100_INPUT_MENU_CHOICE;
}
*/

//-------------------------------------------------------------------------------------------------
//
//  Name:           CALLBACK_InputReading
//
//  Description:    Display the input of the Module
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
VT100_InputType_e VT100_Terminal::CALLBACK_InputReading(uint8_t Input, VT100_CallBackType_e Type)
{
    bool    State;
    //int32_t Temperature;
    uint8_t VerticalOffset;

    VAR_UNUSED(Input);

    switch(Type)
    {
        case VT100_CALLBACK_INIT:
        {
            myVT100.SetForeColor(VT100_COLOR_WHITE);
            myVT100.InMenuPrintf(2,  10, LBL_LIMIT_X);
            myVT100.InMenuPrintf(2,  11, LBL_LIMIT_Y);
            myVT100.InMenuPrintf(2,  12, LBL_LIMIT_Z);
            myVT100.InMenuPrintf(20, 10, LBL_LIMIT_A);
            myVT100.InMenuPrintf(20, 11, LBL_LIMIT_B);
            myVT100.InMenuPrintf(20, 12, LBL_LIMIT_C);
            myVT100.InMenuPrintf(2,  13, LBL_INPUT_ABORT);
            myVT100.InMenuPrintf(2,  14, LBL_INPUT_RESUME);
            myVT100.InMenuPrintf(20, 13, LBL_INPUT_HOLD);
            myVT100.InMenuPrintf(20, 14, LBL_INPUT_PROBE);

            myVT100.SetForeColor(VT100_COLOR_YELLOW);
            myVT100.InMenuPrintf(17,  10, VT100_LBL_STATUS);
            myVT100.InMenuPrintf(17,  11, VT100_LBL_STATUS);
            myVT100.InMenuPrintf(17,  12, VT100_LBL_STATUS);
            myVT100.InMenuPrintf(35,  10, VT100_LBL_STATUS);
            myVT100.InMenuPrintf(35,  11, VT100_LBL_STATUS);
            myVT100.InMenuPrintf(35,  12, VT100_LBL_STATUS);
            myVT100.InMenuPrintf(17,  13, VT100_LBL_STATUS);
            myVT100.InMenuPrintf(35,  14, VT100_LBL_STATUS);
            myVT100.InMenuPrintf(17,  13, VT100_LBL_STATUS);
            myVT100.InMenuPrintf(35,  14, VT100_LBL_STATUS);
        }
        break;

        case VT100_CALLBACK_ON_INPUT:
        {
           // VT100_Generic_uint64 = 10;    // Use for refresh ADC

            myVT100.SetForeColor(VT100_COLOR_CYAN);
//            myVT100.InMenuPrintf(LBL_TEMP_SENSOR);
//            myVT100.InMenuPrintf(VT100_LBL_LINE_SEPARATOR);

            myVT100.SetForeColor(VT100_COLOR_YELLOW);
//            myVT100.InMenuPrintf(LBL_CPU_TEMP_SENSOR);

            myVT100.SetForeColor(VT100_COLOR_CYAN);
//            myVT100.InMenuPrintf(LBL_INPUT_ANALOG);
//            myVT100.InMenuPrintf(VT100_LBL_LINE_SEPARATOR);

            myVT100.SetForeColor(VT100_COLOR_YELLOW);
            //myVT100.InMenuPrintf(LBL_12_VOLT);
            //myVT100.InMenuPrintf(LBL_CPU_VDD);
            //myVT100.InMenuPrintf(LBL_BATTERY_LEVEL);
            //myVT100.InMenuPrintf(LBL_BACKUP_BATTERY_LEVEL);

            myVT100.SetForeColor(VT100_COLOR_CYAN);
//            myVT100.InMenuPrintf(LABEL_pStr[LBL_INPUT_DIGITAL]);
//            myVT100.InMenuPrintf(VT100_LBL_LINE_SEPARATOR);

            myVT100.SetForeColor(VT100_COLOR_YELLOW);
            //myVT100.InMenuPrintf(LBL_EXTERNAL_SWITCH);
            myVT100.InMenuPrintf(VT100_LBL_ESCAPE);
        }
        break;

        case VT100_CALLBACK_REFRESH:
        {
            // Temporary
            myVT100.SetForeColor(IO_IsItValid(IO_LIMIT_X) ? VT100_COLOR_GREEN : VT100_COLOR_GREEN);
            State = IO_GetInputPin(IO_LIMIT_X);
            myVT100.InMenuPrintf(18, 11, LBL_CHAR, State ? '\254' : ' ');   // \254 is a full square font character
            State = IO_GetInputPin(IO_LIMIT_Y);
            myVT100.InMenuPrintf(18, 12, LBL_CHAR, State ? '\254' : ' ');
            State = IO_GetInputPin(IO_LIMIT_Z);
            myVT100.InMenuPrintf(18, 13, LBL_CHAR, State ? '\254' : ' ');
            State = IO_GetInputPin(IO_LIMIT_A);
            myVT100.InMenuPrintf(36, 11, LBL_CHAR, State ? '\254' : ' ');
            State = IO_GetInputPin(IO_LIMIT_B);
            myVT100.InMenuPrintf(36, 12, LBL_CHAR, State ? '\254' : ' ');
            State = IO_GetInputPin(IO_LIMIT_C);
            myVT100.InMenuPrintf(36, 13, LBL_CHAR, State ? '\254' : ' ');
            State = IO_GetInputPin(IO_CONTROL_RESET);       // ABORT
            myVT100.InMenuPrintf(18, 14, LBL_CHAR, State ? '\254' : ' ');
            State = IO_GetInputPin(IO_CONTROL_START);       // RESUME
            myVT100.InMenuPrintf(36, 14, LBL_CHAR, State ? '\254' : ' ');
            State = IO_GetInputPin(IO_CONTROL_FEED);        // HOLD
            myVT100.InMenuPrintf(18, 15, LBL_CHAR, State ? '\254' : ' ');
            State = IO_GetInputPin(IO_PROBE);
            myVT100.InMenuPrintf(36, 15, LBL_CHAR, State ? '\254' : ' ');

            //VT100_Generic_uint64--;
            //if(VT100_Generic_uint64 == 0)
            {
           //     VT100_Generic_uint64 = 10;
                //for(ADC_Channel_e i = ADC_FIRST_ITEM; i < NUMBER_OF_ADC_CHANNEL; i++)
                {
//                    ADC_StartConversion(i);
                }
            }

/*
            // Temperature Sensor
          #if (VT100_USE_COLOR == DEF_ENABLED)
            myVT100.SetForeColor(VT100_COLOR_WHITE);
          #endif
*/

/*
            // Print in Celsius For CPU Sensor
           // Temperature = ADC_GetTemperature();
            myVT100.SetCursorPosition(28, 6);
            myVT100.InMenuPrintf("%3d.%02d ", Temperature / 1000, (abs(Temperature % 1000) / 10));

            // Print in Fahrenheit CPU
            Temperature = 0;//TEMP_SENSOR_ConvertTemperatureToFahrenheit(Temperature);
            myVT100.SetCursorPosition(41, 6);
            myVT100.InMenuPrintf("%3d.%02d ", Temperature / 1000, (abs(Temperature % 1000) / 10));

            VerticalOffset = 10;

            VT100_PrintVoltage(28, VerticalOffset++, 1234);
            VT100_PrintVoltage(28, VerticalOffset++, 501);
            VT100_PrintVoltage(28, VerticalOffset++, 4200);
            VT100_PrintVoltage(28, VerticalOffset++, 301);
*/
            VerticalOffset += 3;
            myVT100.SetCursorPosition(28, VerticalOffset++);
            if(1)//IO_Ctrl(IO_EXT_SWITCH, STATE_READ) == STATE_SET)
            {
                myVT100.SetColor(VT100_COLOR_BLACK, VT100_COLOR_GREEN);
              //  myVT100.InMenuPrintf(" High ");
            }
            else
            {
                myVT100.SetColor(VT100_COLOR_BLACK, VT100_COLOR_RED);
            //    myVT100.InMenuPrintf(" Low  ");
            }
        }
        break;

        default: break;
    }

    return VT100_INPUT_ESCAPE;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           CALLBACK_MiscCfg
//
//  Description:    Menu setting configuration for orphan item
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
VT100_InputType_e VT100_Terminal::CALLBACK_NetworkInfo(uint8_t Input, VT100_CallBackType_e Type)
{
    VAR_UNUSED(Input);

    switch(Type)
    {
        case VT100_CALLBACK_INIT:
        {
            myVT100.SetForeColor(VT100_COLOR_WHITE);
            myVT100.InMenuPrintf(1, 5, LBL_NETWORK_INFO);

            myVT100.SetForeColor(VT100_COLOR_WHITE);
            myVT100.InMenuPrintf(2, 8,  LBL_IP_ADDR);
            myVT100.InMenuPrintf(2, 9,  LBL_IP_MASK);
            myVT100.InMenuPrintf(2, 10, LBL_IP_GATEWAY);
            myVT100.InMenuPrintf(2, 11, LBL_IP_DNS);
            myVT100.InMenuPrintf(2, 13, LBL_IP_DHCP_STATE);
            myVT100.InMenuPrintf(2, 14, LBL_IP_LINK_STATE);
            myVT100.InMenuPrintf(2, 15, LBL_IP_LINK_SPEED);
            myVT100.InMenuPrintf(2, 16, LBL_MAC_ADDRESS);
            // Add Lease obtain and expire???
        }
        break;

        case VT100_CALLBACK_REFRESH:
        {
            struct netif*    pNetif = netif_find(IF_NAME);
            ETH_LinkInfo_t   LinkInfo;
            ETH_LinkState_e  LinkState;
            const char*      pSpeed;

            myVT100.SetForeColor(VT100_COLOR_WHITE);
            myVT100.InMenuPrintf(24, 8,  LBL_STRING, ip_ntoa(&pNetif->ip_addr));
            myVT100.InMenuPrintf(24, 9,  LBL_STRING, ip_ntoa(&pNetif->netmask));
            myVT100.InMenuPrintf(24, 10, LBL_STRING, ip_ntoa(&pNetif->gw));
			//auto t = dns_getserver(0);
            ip_addr_t ipEmpty{};
            myVT100.InMenuPrintf(24, 11, LBL_STRING, ip_ntoa(&ipEmpty));
			//auto t = dns_getserver(1);
            myVT100.InMenuPrintf(24, 12, LBL_STRING, ip_ntoa(&ipEmpty));
            myVT100.InMenuPrintf(24, 13, LBL_STRING, "DHCP N/A");
            LinkState = myETH_PHY->GetLinkState();
            myVT100.InMenuPrintf(24, 14, LBL_STRING, (LinkState == ETH_LINK_UP) != 0 ? "Up  " : "Down");

            LinkInfo = myETH_PHY->GetLinkInfo();

            switch(LinkInfo.Speed)
            {
                case ETH_PHY_SPEED_10M:     pSpeed = "10  Mb/Sec";  break;
                case ETH_PHY_SPEED_100M:    pSpeed = "100 Mb/Sec";  break;
                case ETH_PHY_SPEED_1G:      pSpeed = "1   Gb/Sec";  break;

            }

            myVT100.InMenuPrintf(24, 15, LBL_STRING, pSpeed);
            myVT100.InMenuPrintf(24, 16, LBL_MAC_ADDRESS_VALUE, pNetif->hwaddr[0], pNetif->hwaddr[1], pNetif->hwaddr[2],
                                                                pNetif->hwaddr[3], pNetif->hwaddr[4], pNetif->hwaddr[5]);
        }
        break;

        default:
            break;
    }

    return VT100_INPUT_ESCAPE;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           CALLBACK_MiscCfg
//
//  Description:    Menu setting configuration for orphan item
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
VT100_InputType_e VT100_Terminal::CALLBACK_MiscCfg(uint8_t Input, VT100_CallBackType_e Type)
{
    /*
    static int16_t  TemperatureAlarmLow;
    static int16_t  TemperatureAlarmHigh;
    static uint16_t BackupBatteryLowVoltage;
    static uint16_t PowerBatteryLowVoltage;
    uint32_t        Refresh;
    uint32_t        EditedValue;        // if we come back from decimal input
    uint8_t         InputID;            // contain the value from this input ID
    uint8_t         PosY;
    uint16_t        Size;
    char*           pStr;

    Refresh = VT100_CFG_NO_REFRESH;

    if(Type == VT100_CALLBACK_INIT) // Menu Redraw
    {
        /// Print the box
        DrawBox(8, 23, 77, 9, VT100_COLOR_GREEN);

        /// Print the static info in the
        PosY = 24;
      #if (VT100_USE_COLOR == DEF_ENABLED)
        SetForeColor(VT100_COLOR_YELLOW);
      #endif
        SetCursorPosition(13, PosY++);
        InMenuPrintf(LBL_MISC_TEMPERATURE_LOW);
        SetCursorPosition(13, PosY++);
        InMenuPrintf(LBL_MISC_TEMPERATURE_HIGH);
        SetCursorPosition(13, PosY++);
        InMenuPrintf(LBL_MISC_BACKUP_LOW_BATT);
        SetCursorPosition(13, PosY++);
        InMenuPrintf(LBL_MISC_POWER_LOW_BATT);
        SetCursorPosition(13, PosY++);
        InMenuPrintf(LBL_MISC_SERIAL_NUMBER);
        SetCursorPosition(13, PosY++);
        InMenuPrintf(LBL_MISC_LOCATION);
        SetCursorPosition(13, PosY++);
        InMenuPrintf(LBL_MISC_SITE_ID);
        Refresh = VT100_CFG_REFRESH_ALL;
    }

    if(Type == VT100_CALLBACK_ON_INPUT)
    {
        switch(Input)
        {
            case MISC_SETTING_TEMP_LOW_SET_POINT: // Edit Temperature Alarm set LOW point
            {
                int16_t Min;
                int16_t Max;

                // // SYS_ReadMinMax(SYS_TEMP_ALARM_LEVEL_LOW, 0, &Min, &Max);
                SetDecimalInput(32, 16, Min, Max, TemperatureAlarmLow, 10, Input, LBL_MISC_TEMPERATURE_LOW);
                return VT100_INPUT_DECIMAL;
            }

            case MISC_SETTING_TEMP_HIGH_SET_POINT: // Edit Temperature Alarm set HIGH point
            {
                int16_t Min;
                int16_t Max;

                // // SYS_ReadMinMax(SYS_TEMP_ALARM_LEVEL_HIGH, 0, &Min, &Max);
                SetDecimalInput(32, 16, Min, Max, TemperatureAlarmHigh, 10, Input, LBL_MISC_TEMPERATURE_HIGH);
                return VT100_INPUT_DECIMAL;
            }

            case MISC_SETTING_BATTERY_BACKUP_RTC_LOW_VOLTAGE:
            {
                uint16_t Min;
                uint16_t Max;

                // // SYS_ReadMinMax(SYS_BACKUP_BATT_LOW_VOLT_THOLD, 0, &Min, &Max);
                SetDecimalInput(32, 16, Min, Max, BackupBatteryLowVoltage, 1000, Input, LBL_MISC_BACKUP_LOW_BATT);
                return VT100_INPUT_DECIMAL;
            }

            case MISC_SETTING_BATTERY_POWER_LOW_VOLTAGE:
            {
                uint16_t Min;
                uint16_t Max;

                // // SYS_ReadMinMax(SYS_BATT_LOW_VOLTAGE_THRESHOLD, 0, &Min, &Max);
                SetDecimalInput(32, 16, Min, Max, PowerBatteryLowVoltage, 1000, Input, LBL_MISC_POWER_LOW_BATT);
                return VT100_INPUT_DECIMAL;
            }

            case MISC_SETTING_SERIAL_NUMBER:
            {
                //Size = SYS_GetSingleEntryTypeSize(SYS_SERIAL_NUMBER);
                SetStringInput(32, 16, Size, Input, LBL_MISC_SERIAL_NUMBER, &m_GenericString[1][0][0]);
                return VT100_INPUT_STRING;
            }

            case MISC_SETTING_LOCATION:
            {
               // Size = SYS_GetSingleEntryTypeSize(SYS_LOCATION);
                SetStringInput(32, 16, Size, Input, LBL_MISC_LOCATION, &m_GenericString[2][0][0]);
                return VT100_INPUT_STRING;
            }

            case MISC_SETTING_SITE_ID:
            {
              //  Size = SYS_GetSingleEntryTypeSize(SYS_SITE_ID);
                SetStringInput(32, 16, Size, Input, LBL_MISC_SITE_ID, &m_GenericString[3][0][0]);
                return VT100_INPUT_STRING;
            }

            case MISC_SETTING_SAVE_CONFIGURATION:
            {
                if(myVT100.GetConfigFlag(0) != 0)
                {
                    if((GetConfigFlag(0) & VT100_MISC_CFG_REFRESH_SET_POINT_LOW) != 0)
                    {
                        // DB_Central.Set(&TemperatureAlarmLow, SYS_TEMP_ALARM_LEVEL_LOW);
                    }

                    if((myVT100.GetConfigFlag(0) & VT100_MISC_CFG_REFRESH_SET_POINT_HIGH) != 0)
                    {
                        // DB_Central.Set(&TemperatureAlarmHigh, SYS_TEMP_ALARM_LEVEL_HIGH);
                    }

                    if((myVT100.GetConfigFlag(0) & VT100_MISC_CFG_REFRESH_SET_POINT_BACKUP_LOW_VOLTAGE) != 0)
                    {
                        // DB_Central.Set(&BackupBatteryLowVoltage, SYS_BACKUP_BATT_LOW_VOLT_THOLD);
                    }

                    if((myVT100.GetConfigFlag(0) & VT100_MISC_CFG_REFRESH_SET_POINT_POWER_LOW_VOLTAGE) != 0)
                    {
                        // DB_Central.Set(&PowerBatteryLowVoltage, SYS_BATT_LOW_VOLTAGE_THRESHOLD);
                    }

                    if((myVT100.GetConfigFlag(0) & VT100_MISC_CFG_REFRESH_SERIAL) != 0)
                    {
                        // DB_Central.Set(&m_GenericString[1][0][0], SYS_SERIAL_NUMBER);
                    }

                    if((myVT100.GetConfigFlag(0) & VT100_MISC_CFG_REFRESH_LOCATION) != 0)
                    {
                        // DB_Central.Set(&m_GenericString[2][0][0], SYS_LOCATION);
                    }

                    if((myVT100.GetConfigFlag(0) & VT100_MISC_CFG_REFRESH_SITE_ID) != 0)
                    {
                        // DB_Central.Set(&m_GenericString[3][0][0], SYS_SITE_ID);
                    }
                }

                myVT100.SetConfigFlag(0) = 0;
                Refresh   = VT100_CFG_REFRESH_INFO;
                break;
            }
        }
    }

    //--------------------------------------------------------------------------------------------
    // Refresh all information on the page according to flag
    //--------------------------------------------------------------------------------------------

// use memory allocation

    if(Type == VT100_CALLBACK_INIT)
    {
        /// Get string
        InputID = 0;
        pStr = &m_GenericString[0][0][0];
        GetStringInput(pStr, &InputID);             // Get the newly edit string

        STR_strnstrip(pStr, strlen(pStr));          // Strip all trailing space

        if(InputID == MISC_SETTING_SERIAL_NUMBER)
        {
            memcpy(&m_GenericString[1][0][0], pStr, VT100_STRING_SZ);
            Refresh         |= VT100_MISC_CFG_REFRESH_SERIAL;
            m_ConfigFlag[0] |= VT100_MISC_CFG_REFRESH_SERIAL;
        }
        else if(InputID == MISC_SETTING_LOCATION)
        {
            memcpy(&m_GenericString[2][0][0], pStr, VT100_STRING_SZ);
            Refresh         |= VT100_MISC_CFG_REFRESH_LOCATION;
            m_ConfigFlag[0] |= VT100_MISC_CFG_REFRESH_LOCATION;
        }
        else if(InputID == MISC_SETTING_SITE_ID)
        {
            memcpy(&m_GenericString[3][0][0], pStr, VT100_STRING_SZ);
            Refresh         |= VT100_MISC_CFG_REFRESH_SITE_ID;
            m_ConfigFlag[0] |= VT100_MISC_CFG_REFRESH_SITE_ID;
        }
        else if((InputID == 0) && (Input == DOOR_CTRL_MENU_TITLE) && (m_ConfigFlag[0] == 0))
        {
            // DB_Central.Get(&m_GenericString[1][0][0], SYS_SERIAL_NUMBER);
            m_GenericString[1][0][Size] = '\0';
            // DB_Central.Get(&m_GenericString[2][0][0], SYS_LOCATION);
            m_GenericString[2][0][Size] = '\0';
            // DB_Central.Get(&m_GenericString[3][0][0], SYS_SITE_ID);
            m_GenericString[3][0][Size] = '\0';
        }

        /// Get decimal
        InputID = 0;
        GetDecimalInputValue(&EditedValue, &InputID);

        if(InputID == MISC_SETTING_TEMP_LOW_SET_POINT)
        {
            TemperatureAlarmLow  = (int16_t)EditedValue;
            Refresh          |= VT100_MISC_CFG_REFRESH_SET_POINT_LOW;
            m_ConfigFlag[0]  |= VT100_MISC_CFG_REFRESH_SET_POINT_LOW;
        }
        else if(InputID == MISC_SETTING_TEMP_HIGH_SET_POINT)
        {
            TemperatureAlarmHigh  = (int16_t)EditedValue;
            Refresh           |= VT100_MISC_CFG_REFRESH_SET_POINT_HIGH;
            m_ConfigFlag[0]   |= VT100_MISC_CFG_REFRESH_SET_POINT_HIGH;
        }
        else if(InputID == MISC_SETTING_BATTERY_BACKUP_RTC_LOW_VOLTAGE)
        {
            BackupBatteryLowVoltage = (uint16_t)EditedValue;
            Refresh             |= VT100_MISC_CFG_REFRESH_SET_POINT_BACKUP_LOW_VOLTAGE;
            m_ConfigFlag[0]     |= VT100_MISC_CFG_REFRESH_SET_POINT_BACKUP_LOW_VOLTAGE;
        }
        else if(InputID == MISC_SETTING_BATTERY_POWER_LOW_VOLTAGE)
        {
            PowerBatteryLowVoltage  = (uint16_t)EditedValue;
            Refresh             |= VT100_MISC_CFG_REFRESH_SET_POINT_POWER_LOW_VOLTAGE;
            m_ConfigFlag[0]     |= VT100_MISC_CFG_REFRESH_SET_POINT_POWER_LOW_VOLTAGE;
        }
        else if((InputID == 0) && (Input == MISC_SETTING_MENU_TITLE) && (m_ConfigFlag[0] == 0))
        {
            // DB_Central.Get( &TemperatureAlarmLow, SYS_TEMP_ALARM_LEVEL_LOW);
            // DB_Central.Get(&TemperatureAlarmHigh, SYS_TEMP_ALARM_LEVEL_HIGH);
            // DB_Central.Get(&BackupBatteryLowVoltage, SYS_BACKUP_BATT_LOW_VOLT_THOLD);
            // DB_Central.Get(&PowerBatteryLowVoltage, SYS_BATT_LOW_VOLTAGE_THRESHOLD);
        }
        VT100_BackFromEdition = false;
    }

    //--------------------------------------------------------------------------------------------
    // Refresh part of display


    if(Refresh != 0)
    {
        PosY = 17;

      #if (VT100_USE_COLOR == DEF_ENABLED)
        PrintSaveLabel(9, PosY, (m_ConfigFlag[0] != 0) ? VT100_COLOR_YELLOW : VT100_COLOR_BLUE);
      #else
        PrintSaveLabel(9, PosY);
      #endif
    }

    if(((Refresh & VT100_MISC_CFG_REFRESH_SET_POINT_LOW) != 0) || ((Refresh & VT100_MISC_CFG_REFRESH_SET_POINT_HIGH) != 0))
    {
      #if (VT100_USE_COLOR == DEF_ENABLED)
        SetForeColor(VT100_COLOR_CYAN);
      #endif

        if((Refresh & VT100_MISC_CFG_REFRESH_SET_POINT_LOW) != 0)
        {
            SetCursorPosition(46, 24);
            InMenuPrintf("%ld.%d", TemperatureAlarmLow / 10,  abs(TemperatureAlarmLow % 10));
        }

        if((Refresh & VT100_MISC_CFG_REFRESH_SET_POINT_HIGH) != 0)
        {
            SetCursorPosition(46, 25);
            InMenuPrintf("%ld.%d", TemperatureAlarmHigh / 10, abs(TemperatureAlarmHigh % 10));
        }

        if((Refresh & VT100_MISC_CFG_REFRESH_SET_POINT_BACKUP_LOW_VOLTAGE) != 0)
        {
            SetCursorPosition(46, 26);
            InMenuPrintf("%ld.%03d", BackupBatteryLowVoltage / 1000,  abs(BackupBatteryLowVoltage % 1000));
        }

        if((Refresh & VT100_MISC_CFG_REFRESH_SET_POINT_POWER_LOW_VOLTAGE) != 0)
        {
            SetCursorPosition(46, 27);
            InMenuPrintf("%ld.%03d", PowerBatteryLowVoltage / 1000, abs(PowerBatteryLowVoltage % 1000));
        }

        PosY = 28;

        if((Refresh & VT100_MISC_CFG_REFRESH_SERIAL) != 0)
        {
            SetCursorPosition(46, PosY);
            InMenuPrintf(VT100_STRING_SZ, &m_GenericString[1][0][0]);
        }

        if((Refresh & VT100_MISC_CFG_REFRESH_LOCATION) != 0)
        {
            SetCursorPosition(46, PosY + 1);
            InMenuPrintf(VT100_STRING_SZ, &m_GenericString[2][0][0]);
        }

        if((Refresh & VT100_MISC_CFG_REFRESH_SITE_ID) != 0)
        {
            SetCursorPosition(46, PosY + 2);
            InMenuPrintf(VT100_STRING_SZ, &m_GenericString[3][0][0]);
        }
    }
*/
    return VT100_INPUT_MENU_CHOICE;
}

//-------------------------------------------------------------------------------------------------

#endif // (DIGINI_USE_VT100_MENU == DEF_ENABLED)


