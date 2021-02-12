//-------------------------------------------------------------------------------------------------
//
//  File : lib_pdi_task.cpp
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

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#ifdef DIGINI_USE_POINTING_DEVICE
#define LIB_PDI_TASK_GLOBAL
#include "lib_pdi_task.h"
#undef  LIB_PDI_TASK_GLOBAL

//-------------------------------------------------------------------------------------------------
// Local Define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Private variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local function
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           PDI_TaskWrapper
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//  Description:    main() for the taskTouch
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
extern "C" void PDI_TaskWrapper(void* pvParameters)
{
    (static_cast<PDI_myClassTask*>(pvParameters))->Run();
}

//-------------------------------------------------------------------------------------------------
//
//   Class: PDI_myClassTask
//
//
//   Description:   Class to handle Touch panel with I2C chip
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   uint16_t    SizeX
//                  uint16_t    SizeY
//                  uint8_t     Orientation
//  Return:         nOS_Error
//
//  Description:    Initializes and configures the pointing device functionalities and configures
//                  all necessary hardware resources (GPIOs, I2C, USB, clocks..).
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e PDI_myClassTask::Initialize(PointingDeviceInterface* pDriver, uint16_t SizeX, uint16_t SizeY, uint8_t Orientation)
{
    nOS_Error       Error;
    SystemState_e   State;

    State     = SYS_FAIL;
    m_pDriver = nullptr;

    if((Error = nOS_ThreadCreate(&this->m_Handle,
                                 PDI_TaskWrapper,
                                 this,
                                 &this->m_Stack[0],
                                 PDI_TASK_STACK_SIZE,
                                 PDI_TASK_PRIO)) == NOS_OK)
    {
      #ifdef GRAFX_PDI_INTERRUPT_IO
        if((Error = nOS_SemCreate(&this->m_FlagTouchDetected, 0, 1)) == NOS_OK)
      #endif
        {
            m_pDriver = pDriver;
          #ifdef GRAFX_PDI_INTERRUPT_IO
            this->EnableIT();
         #endif
            m_ConfigSizeX = SizeX;
            m_ConfigSizeY = SizeY;
            m_Orientation = Orientation;
            State         = SYS_READY;
        }
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           run
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//  Description:    main() for the taskTouch
//
//  Note(s):        Process the pointing device, handle all the state the event state and debounce it
//                  also get the event zone ID and send it to the TaskGUI.
//
//-------------------------------------------------------------------------------------------------
void PDI_myClassTask::Run()
{
    ServiceEvent_e      CurrentState;
    ServiceEvent_e      Event;
    Widget_e            EventWidgetID;
    Widget_e            LastEventWidgetID;
    Widget_e            HoldEventWidgetID;
    uint16_t            ReleasedDebounceCount;
    uint32_t            PressedCount;
    bool                SuperKeyTrigged;
    MsgRefresh_t        Msg;

    CurrentState          = SERVICE_IDLE;
    Event                 = SERVICE_NO_EVENT;
    EventWidgetID         = INVALID_WIDGET;
    LastEventWidgetID     = INVALID_WIDGET;
    HoldEventWidgetID     = INVALID_WIDGET;
    ReleasedDebounceCount = 0;
    PressedCount          = 0;
    SuperKeyTrigged       = false;

    // Wait for the initialization to be completed
    while(m_pDriver == nullptr)
    {
        nOS_Sleep(10);
    };

  #ifdef GRAFX_PDI_INTERRUPT_IO
    // Flush touch
    while(nOS_SemTake(&m_FlagTouchDetected, TOUCH_LOOP_DELAY) != NOS_OK)
    {
        nOS_Sleep(10);
    };
  #endif

    for(;;)
    {
      #ifdef GRAFX_PDI_INTERRUPT_IO
        if(nOS_SemTake(&m_FlagTouchDetected, TOUCH_LOOP_DELAY) == NOS_OK)
      #else
        nOS_Sleep(TOUCH_LOOP_DELAY);
        if(m_pDriver->DetectEvent() != 0)
      #endif
        {
            m_pDriver->GetXY(&m_EventPosition);
            //this->GetGestureID();
            EventWidgetID   = this->GetZoneID();                                    // NOTE: It can return a no touch

            //DB_Central.Set(&m_EventPosition, EVENT_POINT, 0, 0);            // TODO use variable in the pointing device
           // this->SetEventPoint(&m_EventPosition); // ???

            ReleasedDebounceCount = 0;

            if(HoldEventWidgetID == INVALID_WIDGET)   HoldEventWidgetID = EventWidgetID;

            if((EventWidgetID == HoldEventWidgetID) && (EventWidgetID != INVALID_WIDGET))
            {
                PressedCount++;
                LastEventWidgetID = EventWidgetID;

                if     (PressedCount == TOUCH_PRESS_COUNT)              Event = SERVICE_PRESSED;
                else if(PressedCount == TOUCH_SUPERKEY_COUNT)           Event = SERVICE_SUPERKEY;
                else if((PressedCount > TOUCH_TYPEMATIC_PRECOUNT) &&
                        ((PressedCount % TOUCH_TYPEMATIC_COUNT) == 0))  Event = SERVICE_TYPEMATIC;
                else                                                    Event = SERVICE_SAME_EVENT;
            }
            else
            {
                Event             = SERVICE_EXIT_EVENT;
                HoldEventWidgetID = INVALID_WIDGET;
            }
        }
        else
        {
            if(++ReleasedDebounceCount == 2)
            {
                ReleasedDebounceCount = 0;
                PressedCount          = 0;
                Event                 = SERVICE_NO_EVENT;
                EventWidgetID         = INVALID_WIDGET;
                HoldEventWidgetID     = INVALID_WIDGET;
                SuperKeyTrigged       = false;
            }
        }

        //Transition state machine
        switch(CurrentState)
        {
            case SERVICE_RELEASED:
            case SERVICE_CANCEL:
            case SERVICE_IDLE:
            {
                if(Event == SERVICE_PRESSED)                CurrentState = SERVICE_PRESSED;
                else                                        CurrentState = SERVICE_IDLE;
            }
            break;

            case SERVICE_PRESSED:
            {
                if     (Event == SERVICE_NO_EVENT)          CurrentState = SERVICE_RELEASED;
                else if(Event == SERVICE_EXIT_EVENT)        CurrentState = SERVICE_CANCEL;
                else if(Event == SERVICE_SAME_EVENT)        CurrentState = SERVICE_TRANSIT;
                else if(Event == SERVICE_TYPEMATIC)         CurrentState = SERVICE_TYPEMATIC;
            }
            break;

            case SERVICE_TYPEMATIC:
            {
                if     (Event == SERVICE_NO_EVENT)          CurrentState = SERVICE_RELEASED;
                else if(Event == SERVICE_EXIT_EVENT)        CurrentState = SERVICE_CANCEL;
                else if(Event == SERVICE_SAME_EVENT)        CurrentState = SERVICE_TRANSIT;
            }
            break;

            case SERVICE_SUPERKEY:
            {
                if     (Event == SERVICE_NO_EVENT)          CurrentState = SERVICE_RELEASED;
                else if(Event == SERVICE_EXIT_EVENT)        CurrentState = SERVICE_CANCEL;
                else if(Event == SERVICE_SAME_EVENT)        CurrentState = SERVICE_TRANSIT;

                SuperKeyTrigged = true;
            }
            break;

            case SERVICE_TRANSIT:
            {
                if     (Event == SERVICE_NO_EVENT)          CurrentState = SERVICE_RELEASED;
                else if(Event == SERVICE_EXIT_EVENT)        CurrentState = SERVICE_CANCEL;
                else if(Event == SERVICE_TYPEMATIC)         CurrentState = SERVICE_TYPEMATIC;
                else if(Event == SERVICE_SUPERKEY &&
                        SuperKeyTrigged == false)           CurrentState = SERVICE_SUPERKEY;
            }
            break;

            default: break; // shut-up warning
        }


        // Send zone ID
        if((CurrentState != SERVICE_IDLE) && (CurrentState != SERVICE_TRANSIT))
        {
            Msg.Type     = MESSAGE_TYPE_PDI_EVENT_INFO;
            Msg.Touch    = CurrentState;
            Msg.WidgetID = LastEventWidgetID;
            if(nOS_QueueWrite(&GUI_pTask->m_Q_Msg, &Msg, 0) != NOS_OK)
            {
                // nothing to do for now
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ClearAllZone
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    clear the touch zone layer
//
//-------------------------------------------------------------------------------------------------
void PDI_myClassTask::ClearAllZone(void)
{
    Box_t Box = {{0, 0},{m_ConfigSizeX, m_ConfigSizeY}};
    CLayer::PushDrawing();
    CLayer::SetDrawing(TOUCH_SENSE_LAYER);
    CLayer::SetColor(BLACK);
    DrawRectangle(&Box);
    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           CreateZone
//
//  Parameter(s):   EventArea_t*    pEventArea
//                  uint16_t        Options
//                  Widget_e        Widget
//  Return:         none
//
//  Description:    create the touch zone on the TOUCH_SENSE_LAYER
//
//-------------------------------------------------------------------------------------------------
void PDI_myClassTask::CreateZone(EventArea_t* pEventArea, uint16_t Options, Widget_e Widget)
{

    CLayer::PushDrawing();
    CLayer::SetDrawing(TOUCH_SENSE_LAYER);
    CLayer::SetColor(Widget);

    switch(Options & GRAFX_OPTION_TOUCH_MASK)
    {
        case GRAFX_OPTION_TOUCH_RECTANGLE:    DrawRectangle(&pEventArea->Rectangle.Box);          break;
        case GRAFX_OPTION_TOUCH_CIRCLE:       DrawCircle   (&pEventArea->Circle.C, POLY_FILL);    break;
        case GRAFX_OPTION_TOUCH_POLYGON:      DrawPolygon  (&pEventArea->Polygon, POLY_FILL);     break;
    }

    CLayer::PopDrawing();
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ResetTouchData
//
//  Parameter(s):   None
//
//  Return:         None
//
//  Description:    Function used to reset all event data before a new acquisition of information.
//
//-------------------------------------------------------------------------------------------------
void PDI_myClassTask::ResetEventData(void)
{
  #ifdef DIGINI_USE_PDI_MULTI_EVENT
    m_GestureID = SERVICE_NO_STATE;

    for(int Index = 0; Index < PDI_NUMBER_OF_EVENT; Index++)
    {
        m_EventX[Index]      = SERVICE_NO_EVENT;
        m_EventY[Index]      = SERVICE_NO_EVENT;
        m_EventArea[Index]   = 0;
        m_EventWeight[Index] = 0;
        m_EventID[Index]     = SERVICE_NO_EVENT;
    }
  #else
    m_EventX = SERVICE_NO_EVENT;
    m_EventY = SERVICE_NO_EVENT;
    m_EventDetected = 0;
  #endif
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           GetGestureID
//
//  Parameter(s):   TS_StateTypeDef *TS_State
//
//  Return:         ServiceEvent_e
//
//  Description:    Get the gesture ID from the device
//
//-------------------------------------------------------------------------------------------------
#ifdef DIGINI_USE_PDI_MULTI_EVENT
ServiceEvent_e PDI_myClassTask::GetGestureID(void)
{
    return m_pDriver->GetGestureID();
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           GetZoneID
//
//  Parameter(s):   None
//  Return:         Widget_e     WidgetID
//
//  Description:    Return the widget ID of the XY position
//
//-------------------------------------------------------------------------------------------------
Widget_e PDI_myClassTask::GetZoneID(void)
{
    uint32_t     PixelAddress;
    uint8_t      PixelSize;
    Cartesian_t  LayerSize;
    Widget_e     WidgetZoneID;

    LayerSize     = LayerTable[TOUCH_SENSE_LAYER].GetSize();
    PixelSize     = LayerTable[TOUCH_SENSE_LAYER].GetPixelSize();

    PixelAddress  = (m_EventPosition.Y * LayerSize.X) + m_EventPosition.X;
    PixelAddress *= PixelSize;
    PixelAddress += LayerTable[TOUCH_SENSE_LAYER].GetAddress();

    switch(PixelSize)
    {
        case 1: // 1 Bytes touch zone
        {
            WidgetZoneID = Widget_e(*((uint8_t*)PixelAddress));
        }
        break;

        case 2: // 2 Bytes touch zone
        {
            WidgetZoneID = Widget_e(*((uint16_t*)PixelAddress));
        }
        break;

        // More than 65536 touch zone ID is useless
    }

    return WidgetZoneID;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           EnableIT
//
//  Parameter(s):   None
//
//  Return:         SystemState_e
//
//  Description:    Configures and enables the touch screen interrupts.
//
//-------------------------------------------------------------------------------------------------
#ifdef GRAFX_PDI_INTERRUPT_IO
SystemState_e PDI_myClassTask::EnableIT(void)
{
    IO_IRQ_Init(GRAFX_PDI_INTERRUPT_IO);
    return m_pDriver->EnableIT();
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           DisableIT
//
//  Parameter(s):   None
//
//  Return:         SystemState_e
//
//  Description:    Disable the pointing event interrupts.
//
//-------------------------------------------------------------------------------------------------
#ifdef GRAFX_PDI_INTERRUPT_IO
SystemState_e PDI_myClassTask::DisableIT(void)
{
    return m_pDriver->DisableIT();
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           GetIT_Status
//
//  Parameter(s):   None
//
//  Return:         SystemState_e
//
//  Description:    Gets the pointing device interrupt status.
//
//-------------------------------------------------------------------------------------------------
#ifdef GRAFX_PDI_INTERRUPT_IO
SystemState_e PDI_myClassTask::GetIT_Status(void)
{
    return SYS_READY;
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           ClearIT
//
//  Parameter(s):   None
//
//  Return:         SystemState_e
//
//  Description:    Clears all pointing device interrupts.
//
//-------------------------------------------------------------------------------------------------
#ifdef GRAFX_PDI_INTERRUPT_IO
SystemState_e PDI_myClassTask::ClearIT(void)
{
    return SYS_READY;
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           GetState
//
//  Parameter(s):   None
//
//  Return:         SystemState_e
//
//  Description:    Returns status and positions of the pointing device.
//
//-------------------------------------------------------------------------------------------------
SystemState_e PDI_myClassTask::GetState(void)
{
  #ifdef DIGINI_USE_PDI_MULTI_EVENT
    uint32_t weight = 0;
    uint32_t area = 0;
    uint32_t event = 0;
    static uint32_t _x[5] = {0, 0};
    static uint32_t _y[5] = {0, 0};
    uint16_t x[5];
    uint16_t y[5];
    Cartesian_t brute[5];
  #else
    static uint32_t _x = 0;
    static uint32_t _y = 0;
    uint16_t x;
    uint16_t y;
    Cartesian_t brute;
  #endif
    SystemState_e State;
    uint16_t x_diff;
    uint16_t y_diff;
    uint32_t index;

    State = SYS_READY;

    // Check and update the number of touches active detected
    m_EventDetected = m_pDriver->DetectEvent();             // TODO Change detect touch for detect event for pointing device

    if(m_EventDetected)
    {
      #ifdef DIGINI_USE_PDI_MULTI_EVENT
        for(index = 0; index < m_EventDetected; index++)
        {
            // Get each touch coordinates
            m_pDriver->GetXY(&brute[index]);

            if(m_Orientation == PDI_SWAP_NONE)  { x[index] = brute[index].X; y[index] = brute[index].Y; }
            if(m_Orientation & PDI_SWAP_X)      { x[index] = 4096 - brute[index].X;                     }
            if(m_Orientation & PDI_SWAP_Y)      { y[index] = 4096 - brute[index].Y;                     }
            if(m_Orientation & PDI_SWAP_XY)     { y[index] = brute[index].X; x[index] = brute[index].Y; }
            x_diff = x[index] > _x[index]? (x[index] - _x[index]): (_x[index] - x[index]);
            y_diff = y[index] > _y[index]? (y[index] - _y[index]): (_y[index] - y[index]);
            if((x_diff + y_diff) > 5)           { _x[index] = x[index]; _y[index] = y[index]; }

            m_EventX[index] = (ServiceEvent_e)x[index];
            m_EventY[index] = (ServiceEvent_e)y[index];

            // Get touch info related to the current touch
            m_pDriver->GetEventInfo(index, &weight, &area, &event);

            // Update
            m_EventWeight[index] = weight;
            m_EventArea[index]   = area;

            // Remap touch event
            //switch(event)
            //{
            //    case FT5336_TOUCH_EVT_FLAG_PRESS_DOWN:  m_EventID[index] = SERVICE_PRESS_DOWN; break;
            //   case FT5336_TOUCH_EVT_FLAG_LIFT_UP:     m_EventID[index] = SERVICE_LIFT_UP;    break;
            //  case FT5336_TOUCH_EVT_FLAG_CONTACT:     m_EventID[index] = SERVICE_CONTACT;    break;
            //    case FT5336_TOUCH_EVT_FLAG_NO_EVENT:    m_EventID[index] = SERVICE_NO_EVENT;   break;
            //    default:                                State = SYS_FAIL;                      break;
            //}

            // Get gesture Id
            m_GestureID = this->GetGestureID();
        }

      #else
        // Get each touch coordinates
        m_pDriver->GetXY(&brute);

        if(m_Orientation == PDI_SWAP_NONE) { x = brute.X; y = brute.Y; }
        if(m_Orientation & PDI_SWAP_X)     { x = 4096 - brute.X;       }
        if(m_Orientation & PDI_SWAP_Y)     { y = 4096 - brute.Y;       }
        if(m_Orientation & PDI_SWAP_XY)    { y = brute.X; x = brute.Y; }
        x_diff = x > _x ? (x - _x) : (_x - x);
        y_diff = y > _y ? (y - _y) : (_y - y);
        if((x_diff + y_diff) > 5)          { _x = x; _y = y;}
        m_EventX = (ServiceEvent_e)x;
        m_EventY = (ServiceEvent_e)y;
      #endif
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           IRQ_Handler
//
//  Description:    This function handles USARTx interrupt.
//
//-------------------------------------------------------------------------------------------------
#ifdef GRAFX_PDI_INTERRUPT_IO
void PDI_myClassTask::IRQ_Handler(void)
{
    // Signal main task to process the event
    nOS_SemGive(&m_FlagTouchDetected);
}
#endif

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_POINTING_DEVICE
#endif // DIGINI_USE_GRAFX
