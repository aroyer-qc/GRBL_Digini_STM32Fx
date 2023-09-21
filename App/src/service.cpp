//-------------------------------------------------------------------------------------------------
//
//  File : service.cpp
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

/*

Service type 2 - Return data array compose of 16 bits value. Array size define by SERVICE_TYPE2_SIZE_LIST
Service type 3 - Return link to new page
                        INVALID_LINK  - that instruct GUI to do nothing
                        REMOVE_WIDGET - remove the widget from the list of widget to refresh
Service type 4 - Return string array pointer. Array size define by SERVICE_TYPE4_SIZE_LIST
Service type 5 - Return data for an image -> Address, Size, Pixel format
Service type 6 - Return data to override position of a widget
Service type 7 - Return data to override position of a widget (Must be a memory block by allocation, it will be freed by widget)
*/

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------`

#include "lib_digini.h"
#if (DIGINI_USE_GRAFX == DEF_ENABLED)
#include "grbl_advance.h"
#include "project_def.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------`

#define AXIS_COORD_SYSTEM_ABS       0       // Absolute position
#define AXIS_COORD_SYSTEM_REL       1       // Relative position
#define AXIS_COORD_SYSTEM_MAC       2       // Machine position
#define AXIS_COORD_SYSTEM_TOOL      3       // Tool length offset position
#define SERVICE_PRINT_BUFFER_SIZE   80

//-------------------------------------------------------------------------------------------------
// Variable(s)
//-------------------------------------------------------------------------------------------------`

uint32_t    CoordinateSystem[5]       = {AXIS_COORD_SYSTEM_ABS, AXIS_COORD_SYSTEM_REL, AXIS_COORD_SYSTEM_MAC, AXIS_COORD_SYSTEM_ABS, AXIS_COORD_SYSTEM_MAC};
bool        IsDisplayMachineEnable[5] = {true, true, false, true, false};

uint32_t    Machine                   = (MACHINE_XYZ | MAC_AXIS_C);

bool        IsTerminalEnabled         = false;

//-------------------------------------------------------------------------------------------------
// Support Function(s)
//-------------------------------------------------------------------------------------------------`

uint16_t GetIO_State(IO_ID_e LimitID)
{
    uint16_t State;

    if(IO_IsItValid(LimitID) == true)
    {
        State = uint16_t(IO_GetInputPin(LimitID));
    }
    else
    {
        State = 2;  // Grayout
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_AXIS
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    This function return position data to display for any valid axis
//
//  Note(s)         SubService 10 and up  will display only machine position
//                  This service used color override for axis display value
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_AXIS(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    static char*     pBuffer               = nullptr;
    static bool      IsItFirstServiceStart = true;
    ServiceReturn_t* pService              = nullptr;
    float            Coordinate            = 0;
    uint16_t         SubOffset;
    struct32_t       Color;
    bool             IsItBlank;

    // Handling of memory allocation while service is active (Shared for all active SERV_AXIS SubService)
    if(*pServiceState == SERVICE_START)
    {
        if(IsItFirstServiceStart == true)
        {
            IsItFirstServiceStart = false;
            pBuffer = (char*)pMemoryPool->Alloc(SERVICE_PRINT_BUFFER_SIZE);
        }
    }

    if(*pServiceState == SERVICE_FINALIZE)
    {
        if(IsItFirstServiceStart == false)
        {
            IsItFirstServiceStart = true;
            pMemoryPool->Free((void**)&pBuffer);
        }
    }
    else
    {
        SubOffset = SubService % 10;
        SubService %= 20;
        IsItBlank = true;

        if(SubService < 10) // 23 and 24 are for sign color on axis so we modulo to 20 to get color check from 3 to 4 (rotary axis)
        {
            if(CoordinateSystem[SubOffset] == AXIS_COORD_SYSTEM_MAC)
            {
                Coordinate = gc_state.CoordSystem[SubOffset];
                Color = GFX_ColorTable[LIGHT_GREEN];
            }
            else if(CoordinateSystem[SubOffset] == AXIS_COORD_SYSTEM_ABS)
            {
                Coordinate = gc_state.CoordSystem[SubOffset] + gc_state.coord_offset[SubOffset];
                Color = GFX_ColorTable[LIGHT_RED];
            }
            else // if(CoordinateSystem[SubOffset] == AXIS_COORD_SYSTEM_REL)
            {
                Coordinate = gc_state.CoordSystem[SubOffset] + gc_state.coord_offset[SubOffset];
                Color = GFX_ColorTable[LIGHT_YELLOW];
            }

            IsItBlank = false;
        }
        else
        {
            if(CoordinateSystem[SubOffset] != AXIS_COORD_SYSTEM_MAC) // Only display little axis info if not set as machine on big axis information
            {
                Coordinate = gc_state.CoordSystem[SubOffset];
                Color = GFX_ColorTable[LIGHT_GREEN];
                IsItBlank = false;
            }
        }

    //     if(SubService == TOOL_LENGTH_OFFSET_AXIS)
    //    {
    //         Coordinate += gc_state.ToolLengthOffset;
    //     }

        if(IsItBlank == false)
        {
            if(SubOffset < 3)  // linear axis
            {
                if(BIT_IS_TRUE(Settings.flags, BITFLAG_REPORT_INCHES))
                {
                    snprintf(pBuffer, 20, "%c%6lX%8.4f", ASCII_COLOR_OVERRIDE, Color.u_32, Coordinate * INCH_PER_MM);
                }
                else
                {
                    snprintf(pBuffer, 20, "%c%6lX%8.3f", ASCII_COLOR_OVERRIDE, Color.u_32, Coordinate);
                }
            }
            else if(SubOffset <= 5)  // rotational axis
            {
                snprintf(pBuffer, 20, "%c%6lX%7.2f%c%2X°", ASCII_COLOR_OVERRIDE, Color.u_32, Coordinate, ASCII_SINGLE_FONT_OVERRIDE, FT_ARIAL_16);
            }
        }
        else
        {
            *pBuffer = '\0';
        }

        pService = GetServiceStruct(SERVICE_RETURN_TYPE4);
        if(pService != nullptr)
        {
            ((ServiceType4_t*)pService)->pString[0] = pBuffer;
            *pServiceState = SERVICE_REFRESH;
        }
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_COOR
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    This function return Spindle speed
//
//  Note(s)         SubService 10 and up will display label for machine position if not set for it
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_COOR(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    static char*     pBuffer               = nullptr;
    static bool      IsItFirstServiceStart = true;
    ServiceReturn_t* pService              = nullptr;
    uint16_t         Sub;

    // Handling of memory allocation while service is active (Shared for all active SERV_COOR SubService)
    if(*pServiceState == SERVICE_START)
    {
        if(IsItFirstServiceStart == true)
        {
            IsItFirstServiceStart = false;
            pBuffer = (char*)pMemoryPool->Alloc(SERVICE_PRINT_BUFFER_SIZE);
        }
    }

    if(*pServiceState == SERVICE_FINALIZE)
    {
        if(IsItFirstServiceStart == false)
        {
            IsItFirstServiceStart = true;
            pMemoryPool->Free((void**)&pBuffer);
        }
    }
    else
    {
        Sub = SubService;
        SubService %= 10;

        if(Sub >= 10)
        {
            if(CoordinateSystem[SubService] != AXIS_COORD_SYSTEM_MAC)
            {
                snprintf(pBuffer, 5, "mac");
            }
            else
            {
                *pBuffer = '\0';
            }
        }
        else
        {
            switch(CoordinateSystem[SubService])
            {
                case AXIS_COORD_SYSTEM_ABS: snprintf(pBuffer, 5, "abs"); break;
                case AXIS_COORD_SYSTEM_REL: snprintf(pBuffer, 5, "rel"); break;
                case AXIS_COORD_SYSTEM_MAC: snprintf(pBuffer, 5, "mac"); break;
            }
        }

        pService = GetServiceStruct(SERVICE_RETURN_TYPE4);
        if(pService != nullptr)
        {
            ((ServiceType4_t*)pService)->pString[0] = pBuffer;
            *pServiceState = SERVICE_REFRESH;
        }
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_ENBL
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    This function return ENABLE state for different internal config
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_ENBL(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    static uint32_t count = 0;

    count++;

    if (count > 30) count =0;


    ServiceReturn_t* pService = nullptr;

    if(*pServiceState != SERVICE_FINALIZE)
    {
        pService = GetServiceStruct(SERVICE_RETURN);
        if(pService != nullptr)
        {
            *pServiceState = SERVICE_REFRESH;
            if(SubService == 9) // Check for Spindle
            {
//                pService->IndexState = (spindle_get_state() == SPINDLE_STATE_DISABLE) ? 0 : 1;


            pService->IndexState = (count >15) ? 0 : 1;

            }
        }
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_GCOD
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    This function simply return the buffer screen to print on terminal windows
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_GCOD(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    static char*     pBuffer               = nullptr;
    static bool      IsItFirstServiceStart = true;
    ServiceReturn_t* pService              = nullptr;
    static uint32_t  Count = 0;
    static uint32_t  LineCount = 0;

    // Handling of memory allocation while service is active (Shared for all active SERV_GCOD SubService)
    if(*pServiceState == SERVICE_START)
    {
        if(IsItFirstServiceStart == true)
        {
            IsItFirstServiceStart = false;
            pBuffer = (char*)pMemoryPool->Alloc(SERVICE_PRINT_BUFFER_SIZE);
        }
    }

    if(*pServiceState == SERVICE_FINALIZE)
    {
        if(IsItFirstServiceStart == false)
        {
            IsItFirstServiceStart = true;
            pMemoryPool->Free((void**)&pBuffer);
        }
    }
    else
    {
        // TODO (Alain#1#) SERV_GCOD
        VAR_UNUSED(SubService);

        Count++;

        if(*pServiceState == SERVICE_RELEASED)
        {
            pService = GetServiceStruct(SERVICE_RETURN);
        }
        else
        {
            if(Count >= 1)
            {
                Count = 0;

                LineCount++;
                if((pService = GetServiceStruct(SERVICE_RETURN_TYPE4)) != nullptr)
                {
                    sprintf(pBuffer, "Test:line number %lu", LineCount);
                    ((ServiceType4_t*)pService)->pString[0] = pBuffer;
                }
            }
        }
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_INCH
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    This function return measuring system string to GUI
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_INCH(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    static char*     pBuffer               = nullptr;
    static bool      IsItFirstServiceStart = true;
    ServiceReturn_t* pService              = nullptr;

    // Handling of memory allocation while service is active (Shared for all active SERV_INCH SubService)
    if(*pServiceState == SERVICE_START)
    {
        if(IsItFirstServiceStart == true)
        {
            IsItFirstServiceStart = false;
            pBuffer = (char*)pMemoryPool->Alloc(SERVICE_PRINT_BUFFER_SIZE);
        }
    }

    if(*pServiceState == SERVICE_FINALIZE)
    {
        if(IsItFirstServiceStart == false)
        {
            IsItFirstServiceStart = true;
            pMemoryPool->Free((void**)&pBuffer);
        }
    }
    else
    {
        if(SubService == 0) // Subservice 0 is for word Inch or MM
        {
            if(BIT_IS_TRUE(Settings.flags, BITFLAG_REPORT_INCHES))
            {
                snprintf(pBuffer, 5, "Inch");
            }
            else
            {
                snprintf(pBuffer, 5,  "mm");
            }
        }

        if(SubService == 0) // Subservice 0 is for word Inch or MM
        {
            if((pService = GetServiceStruct(SERVICE_RETURN_TYPE4)) != nullptr)
            {
                ((ServiceType4_t*)pService)->pString[0] = pBuffer;
                *pServiceState = SERVICE_REFRESH;
            }
        }
        else if(SubService == 1) // Subservice 1 printing back off segment for mm or inch
        {
            if((pService = GetServiceStruct(SERVICE_RETURN_TYPE1)) != nullptr)
            {
                ((ServiceType1_t*)pService)->Data = BIT_IS_TRUE(Settings.flags, BITFLAG_REPORT_INCHES) ? 1 : 0;
                *pServiceState = SERVICE_REFRESH;
            }
        }
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_INFO
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    This function return information on software
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_INFO(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    static char*     pBuffer               = nullptr;
    static bool      IsItFirstServiceStart = true;
    ServiceReturn_t* pService              = nullptr;

    // Handling of memory allocation while service is active (Shared for all active SERV_INFO SubService)
    if(*pServiceState == SERVICE_START)
    {
        if(IsItFirstServiceStart == true)
        {
            IsItFirstServiceStart = false;
            pBuffer = (char*)pMemoryPool->Alloc(SERVICE_PRINT_BUFFER_SIZE);
        }
    }

    if(*pServiceState == SERVICE_FINALIZE)
    {
        if(IsItFirstServiceStart == false)
        {
            IsItFirstServiceStart = true;
            pMemoryPool->Free((void**)&pBuffer);
        }
    }
    else
    {
        if((pService = GetServiceStruct(SERVICE_RETURN_TYPE4)) != nullptr)
        {
            switch(SubService)
            {
                case 0: snprintf(pBuffer, 24, "%s", OUR_FIRMWARE_NAME);        break;
                case 1: snprintf(pBuffer, 24, "%s", OUR_FIRMWARE_VERSION);     break;
                case 2: snprintf(pBuffer, 24, "%s", OUR_FIRMWARE_GUI_NAME);    break;
                case 3: snprintf(pBuffer, 24, "%s", OUR_FIRMWARE_GUI_VERSION); break;
                case 4: snprintf(pBuffer, 24, "%s", OEM_MODEL_NAME);           break;
                case 5: snprintf(pBuffer, 24, "%s", OEM_SERIAL_NUMBER);        break;
                case 6: snprintf(pBuffer, 24, "%s", OUR_BUILD_DATE);           break;
                // Add Config version
            }

            ((ServiceType4_t*)pService)->pString[0] = pBuffer;
            *pServiceState = SERVICE_REFRESH;
        }
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_INPU
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    This function return status on input
//
//  Note(s):        If IO is not defined, it will be grayout
//                  TODO Might add further state control for enabled feature into setting menu!!
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_INPU(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    uint16_t         State;
    ServiceReturn_t* pService = nullptr;

    switch(SubService)
    {
        case 0:  State = GetIO_State(IO_LIMIT_X1);       break;
        case 1:  State = GetIO_State(IO_LIMIT_Y1);       break;
        case 2:  State = GetIO_State(IO_LIMIT_Z1);       break;
        case 3:  State = GetIO_State(IO_LIMIT_A1);       break;
        case 4:  State = GetIO_State(IO_LIMIT_B1);       break;
        case 5:  State = GetIO_State(IO_LIMIT_C1);       break;
        case 6:  State = GetIO_State(IO_LIMIT_X1);       break;
        case 7:  State = GetIO_State(IO_LIMIT_Y1);       break;
        case 8:  State = GetIO_State(IO_LIMIT_Z1);       break;
        case 9:  State = GetIO_State(IO_LIMIT_A1);       break;
        case 10: State = GetIO_State(IO_LIMIT_B1);       break;
        case 11: State = GetIO_State(IO_LIMIT_C1);       break;
        case 12: State = GetIO_State(IO_CONTROL_RESET);  break;
        case 13: State = GetIO_State(IO_CONTROL_START);  break;
        case 14: State = GetIO_State(IO_CONTROL_FEED);   break;
        case 15: State = GetIO_State(IO_PROBE);          break;
    }

    if(*pServiceState != SERVICE_FINALIZE)
    {
        pService = GetServiceStruct(SERVICE_RETURN);
        if(pService != nullptr)
        {
            pService->IndexState = State;
            *pServiceState = SERVICE_REFRESH;
        }
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_MACH
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_MACH(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    static uint32_t RefreshABC = 0;

    ServiceReturn_t* pService = nullptr;

    // Get the original settings
    if(SubService == 0)
    {
        if(*pServiceState == SERVICE_START)
        {
            //Machine = Settings.Machine;
        }
        else if(*pServiceState == SERVICE_FINALIZE)
        // Save new settings if different
        {
            Settings.Machine = Machine;
           // write_global_settings();
        }
    }

    if(*pServiceState != SERVICE_FINALIZE)
    {
        if((pService = GetServiceStruct(SERVICE_RETURN)) != nullptr)
        {
            pService->IndexState = 0;

            switch(SubService)
            {
                case 0: // Touch XY
                {
                    if(*pServiceState == SERVICE_RELEASED)
                    {
                        Machine = ((Machine & ~(MACHINE_XYZ)) | MACHINE_XY);    // Set Machine as XY machine    (Plasma)
                        Machine &= ~MAC_AXIS_C;                                 // C axis cannot exist with this configuration
                        RefreshABC = OPTION_MAC_MASK;
                    }
                }
                break;

                case 1: // Touch ZX
                {
                    if(*pServiceState == SERVICE_RELEASED)
                    {
                        Machine =  ((Machine & ~(MACHINE_XYZ)) | MACHINE_ZX);   // Set Machine as ZX machine    (Lathe)
                        Machine &= ~MAC_AXIS_B;                                 // B axis cannot exist with this configuration
                        RefreshABC = OPTION_MAC_MASK;
                    }
                }
                break;

                case 2: // Touch XYZ
                {
                    if(*pServiceState == SERVICE_RELEASED)
                    {
                        Machine |= MACHINE_XYZ;                                 // Set Machine as XYZ machine   (Milling)
                        RefreshABC = OPTION_MAC_MASK;
                    }
                }
                break;

                case 3: // Touch A
                {
                    if(*pServiceState == SERVICE_RELEASED)
                    {
                        if((Machine & MAC_AXIS_X) == MAC_AXIS_X)
                        {
                            Machine ^= MAC_AXIS_A;
                        }
                    }
                    else
                    {
                        if((Machine & MAC_AXIS_X) != MAC_AXIS_X)
                        {
                            Machine &= ~(MAC_AXIS_A);
                            pService->IndexState = 2; // Inactive
                            *pServiceState = SERVICE_REFRESH;
                        }

                        if((RefreshABC & MAC_AXIS_A) != 0)                      // Refresh button release or inactive
                        {
                            RefreshABC &= ~MAC_AXIS_A;
                            *pServiceState = SERVICE_REFRESH;
                        }
                    }
                }
                break;

                case 4: // Touch B
                {
                    if(*pServiceState == SERVICE_RELEASED)
                    {
                        if((Machine & MAC_AXIS_Y) == MAC_AXIS_Y)
                        {
                            Machine ^= MAC_AXIS_B;
                        }
                    }
                    else
                    {
                        if((Machine & MAC_AXIS_Y) != MAC_AXIS_Y)
                        {
                            Machine &= ~(MAC_AXIS_B);
                            pService->IndexState = 2; // Inactive
                            *pServiceState = SERVICE_REFRESH;
                        }

                        if((RefreshABC & MAC_AXIS_B) != 0)              // Refresh button release or inactive
                        {
                            RefreshABC &= ~MAC_AXIS_B;
                            *pServiceState = SERVICE_REFRESH;
                        }
                    }
                }
                break;

                case 5: // Touch C
                {
                    if(*pServiceState == SERVICE_RELEASED)
                    {
                        if((Machine & MAC_AXIS_Z) == MAC_AXIS_Z)
                        {
                            Machine ^= MAC_AXIS_C;
                        }
                    }
                    else
                    {
                        if((Machine & MAC_AXIS_Z) != MAC_AXIS_Z)
                        {
                            Machine &= ~(MAC_AXIS_C);
                            pService->IndexState = 2; // Inactive
                            *pServiceState = SERVICE_REFRESH;
                        }

                        if((RefreshABC & MAC_AXIS_C) != 0)              // Refresh button release or inactive
                        {
                            RefreshABC &= ~MAC_AXIS_C;
                            *pServiceState = SERVICE_REFRESH;
                        }
                    }
                }
                break;

                case 10: // Check Mark XY
                {
                    if((Machine & MACHINE_XYZ) == MACHINE_XY)
                    {
                        pService->IndexState = 1;
                    }
                }
                break;

                case 20: // Icon XY
                {
                    if((Machine & MACHINE_XY) == MACHINE_XY) pService->IndexState = 1;
                }
                break;

                case 11: // Check Mark ZX
                case 21: // Icon ZX
                {
                    if((Machine & MACHINE_XYZ) == MACHINE_ZX) pService->IndexState = 1;
                }
                break;

                case 12: // Check Mark XYZ
                case 22: // Icon Z
                {
                    if((Machine & MACHINE_XYZ) == MACHINE_XYZ) pService->IndexState = 1;
                }
                break;

                case 13: // Check Mark A
                {
                    if((Machine & MAC_AXIS_A) == MAC_AXIS_A) pService->IndexState = 1;
                }
                break;

                case 14: // Check Mark B
                case 24: // Icon B
                {
                    if((Machine & MAC_AXIS_Y) == MAC_AXIS_Y)
                    {
                        if((Machine & MAC_AXIS_B) == MAC_AXIS_B) pService->IndexState = 1;
                    }
                }
                break;

                case 15: // Check Mark C
                {
                    if((Machine & MAC_AXIS_Z) == MAC_AXIS_Z)
                    {
                        if((Machine & MAC_AXIS_C) == MAC_AXIS_C) pService->IndexState = 1;
                    }
                }
                break;

                case 23: // Icon A
                {
                    if(((Machine & MACHINE_XYZ) == MACHINE_XYZ) &
                       ((Machine & MAC_AXIS_X) == MAC_AXIS_X))
                    {
                        if((Machine & MAC_AXIS_A) == MAC_AXIS_A) pService->IndexState = 1;
                    }
                }
                break;

                case 25: // Icon C
                {
                    if(((Machine & MACHINE_XYZ) == MACHINE_XYZ) &
                       ((Machine & MAC_AXIS_Z) == MAC_AXIS_Z))
                    {
                        if((Machine & MAC_AXIS_C) == MAC_AXIS_C) pService->IndexState = 1;
                    }
                }
                break;

                case 26: // Icon A2
                {
                    if(((Machine & MACHINE_XYZ) == MACHINE_ZX) &
                       ((Machine & MAC_AXIS_X) == MAC_AXIS_X))
                    {
                        if((Machine & MAC_AXIS_A) == MAC_AXIS_A) pService->IndexState = 1;
                    }
                }
                break;

                case 27: // Icon C2
                {
                    if(((Machine & MACHINE_XYZ) == MACHINE_ZX) &
                       ((Machine & MAC_AXIS_Z) == MAC_AXIS_Z))
                    {
                        if((Machine & MAC_AXIS_C) == MAC_AXIS_C) pService->IndexState = 1;
                    }
                }
                break;
            }
        }
        //*pServiceState = SERVICE_REFRESH;
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_MHUB
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//  Return:         ServiceReturn_t
//
//  Description:    This is a HUB to redirect the GUI to the right page to display
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_MHUB(ServiceEvent_e* pServiceState)
{
    ServiceReturn_t* pService = nullptr;

    pService = GetServiceStruct(SERVICE_RETURN);
    *pServiceState = SERVICE_REFRESH;

// TODO (Alain#1#) add check for page type Terminal or not
// TODO (Alain#1#) instead of GRBL_MACHINE_USE_AXIS_XZ use machine select config

    if((Machine & MACHINE_XYZ) == MACHINE_XY)
    {
        ((ServiceReturn_t*)pService)->IndexState = 0;
    }
    else if((Machine & MACHINE_XYZ) == MACHINE_ZX)
    {
        ((ServiceReturn_t*)pService)->IndexState = 1;
    }
    else if((Machine & MACHINE_XYZ) == MACHINE_XYZ)
    {
        ((ServiceReturn_t*)pService)->IndexState = 2;
    }

    if(IsTerminalEnabled == true)
    {
        ((ServiceReturn_t*)pService)->IndexState += 3;
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SERV_SPIN
//
//  Parameter(s):   ServiceEvent_e*  pServiceState
//                  uint16_t         SubService
//  Return:         ServiceReturn_t
//
//  Description:    SubSservice         0 -     Spindle speed
//                                      1 -     1% PWM   = this Minimum RPM
//                                      2 -     100% PWM = this Maximum RPM
//                                      3 -     Decrease Minimum RPM value by 10
//                                      4 -     Increase Minimum RPM value by 10
//                                      5 -     Decrease Maximum RPM value by 10
//                                      6 -     Increase Maximum RPM value by 10
//
//-------------------------------------------------------------------------------------------------
static ServiceReturn_t* SERV_SPIN(ServiceEvent_e* pServiceState, uint16_t SubService)
{
    ServiceReturn_t* pService = nullptr;

    if(*pServiceState != SERVICE_FINALIZE)
    {
        if(SubService <= 2) // Label sub service
        {
            if((pService = GetServiceStruct(SERVICE_RETURN_TYPE1)) != nullptr)
            {
                switch(SubService)
                {
                    case 0: ((ServiceType1_t*)pService)->Data = uint32_t(gc_state.spindle_speed); break;
                    case 1: ((ServiceType1_t*)pService)->Data = uint32_t(Settings.rpm_min);       break;
                    case 2: ((ServiceType1_t*)pService)->Data = uint32_t(Settings.rpm_max);       break;
                }

                *pServiceState = SERVICE_REFRESH;
            }
        }
        else    // Up/Down button handling sub service
        {
            if((pService = GetServiceStruct(SERVICE_RETURN)) != nullptr)
            {
                if((*pServiceState == SERVICE_PRESSED) || (*pServiceState == SERVICE_TYPEMATIC))
                {
                    switch(SubService)
                    {
                        case 3: Settings.rpm_min -= 10; break;
                        case 4: Settings.rpm_min += 10; break;
                        case 5: Settings.rpm_max -= 10; break;
                        case 6: Settings.rpm_max += 10; break;
                    }
                }
            }
        }
    }

    return pService;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ServiceCallApp
//  Parameter(s):   Service_t*       pService
//                  ServiceEvent_e*  pServiceState
//  Return:         ServiceReturn_t
//
//  Description:    Callback service provided for application side
//
//  Note(s):        Will Call any service from TaskGUI that are not generic
//
//-------------------------------------------------------------------------------------------------
ServiceReturn_t* ServiceCallApp(Service_t* pService, ServiceEvent_e* pServiceState)
{
    ServiceReturn_t* pServiceReturn = nullptr;
    struct32_t       ServiceRange;

    ServiceRange.u_32 = pService->ID;

    switch(ServiceRange.u8_Array[0])        // To speed up process
    {
        case 'A':
        {
            switch(pService->ID)
            {
                case SERV_ID_AXIS:  pServiceReturn = SERV_AXIS(pServiceState, pService->SubID);  break;
            }
            break;
        }

        case 'C':
        {
            switch(pService->ID)
            {
                case SERV_ID_COOR:  pServiceReturn = SERV_COOR(pServiceState, pService->SubID);  break;
            }
            break;
        }

        case 'E':
        {
            switch(pService->ID)
            {
                case SERV_ID_ENBL:  pServiceReturn = SERV_ENBL(pServiceState, pService->SubID);  break;
            }
            break;
        }

        case 'G':
        {
            switch(pService->ID)
            {
                case SERV_ID_GCOD:  pServiceReturn = SERV_GCOD(pServiceState, pService->SubID);  break;
            }
            break;

        }

        case 'I':
        {
            switch(pService->ID)
            {
                case SERV_ID_INCH:  pServiceReturn = SERV_INCH(pServiceState, pService->SubID);  break;
                case SERV_ID_INFO:  pServiceReturn = SERV_INFO(pServiceState, pService->SubID);  break;
                case SERV_ID_INPU:  pServiceReturn = SERV_INPU(pServiceState, pService->SubID);  break;
            }
            break;

        }

        case 'M':
        {
            switch(pService->ID)
            {
                case SERV_ID_MHUB:  pServiceReturn = SERV_MHUB(pServiceState);                   break;
                case SERV_ID_MACH:  pServiceReturn = SERV_MACH(pServiceState, pService->SubID);  break;
            }
            break;
        }

        case 'S':
        {
            switch(pService->ID)
            {
                case SERV_ID_SPIN:  pServiceReturn = SERV_SPIN(pServiceState, pService->SubID);  break;
            }
            break;
        }
    }

    return pServiceReturn;
}


//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
