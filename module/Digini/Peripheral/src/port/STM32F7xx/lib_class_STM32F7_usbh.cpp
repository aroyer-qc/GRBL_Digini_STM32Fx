//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_usbh.cpp
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

//#include "digini_cfg.h"
#include "lib_class_STM32F7_usbh.h"
//#include "usbh_core.h"
//#include "usb_hcd_int.h"
//#include "stm32f7xx_ll_usb.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define USBH_RUN_PROCESS                         0x01


//-------------------------------------------------------------------------------------------------
//
//  Name:           USB_HostWrapper
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//  Description:    main() for the task1
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
extern "C" void USB_HostWrapper(void* pvParameters)
{
    (static_cast<USB_Host*>(pvParameters))->Run();
}

//-------------------------------------------------------------------------------------------------
//
//   Class: USB_Host
//
//
//   Description:   Class to handle USB Host
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   USB_Host
//
//   Parameter(s):
//
//   Description:   Initializes the USB peripheral
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
//USB_Host::USB_Host()
//{
//}

//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void USB_Host::Initialize(class USB_Application* pUSB)
{
    m_Host.pUSB_App = pUSB;

    // Initialize IO, clocks, etc...
    pUSB->Initialize();

    m_Host.pBase = this;

    // Init Host Library
    // USBH_Init(&pUSB->hUSBHost, USBH_UserProcess, 0);


    //m_USB_Host.id           = 0;            // Set Driver ID at 0
    m_Host.pActiveClass = nullptr;        // Unlink class
    m_Host.ClassNumber  = 0;

    DeInitStateMachine();

    // Assign User process
    m_Host.pUser = nullptr; //TODO fix thispUserProcessCallBack; // USB User Process

    // Create USB Host Queue
    nOS_FlagCreate(&m_Host.HostEvent, USBH_RUN_PROCESS);

    // Create USB Host Task
    nOS_ThreadCreate(&m_Handle,
                     USB_HostWrapper,
                     this,
                     &m_Stack[0],
                    USBH_STACK_SIZE,
                    USBH_PRIO);

    // Initialize low level driver
    USB_OTG_CfgTypeDef OTG_Config;
    OTG_Config.Host_channels       = 11;
    OTG_Config.dma_enable          = 0;
    OTG_Config.low_power_enable    = 0;
    OTG_Config.phy_itface          = HCD_PHY_EMBEDDED;
    OTG_Config.Sof_enable          = 0;
    OTG_Config.speed               = HCD_SPEED_FULL;
    OTG_Config.vbus_sensing_enable = 0;


    // Link the driver to the stack
    //hhcd.pData = &host;
    //host.pData = &hhcd;

    // Initialize the LL driver
    USB_OTG_FS->GAHBCFG &= ~USB_OTG_GAHBCFG_GINT;            // Disable the Interrupts
    USB_CoreInit(USB_OTG_FS, OTG_Config);                     // Init the Core (common init.)
    USB_SetCurrentMode(USB_OTG_FS, USB_OTG_HOST_MODE);         // Force Host Mode
    USB_HostInit(USB_OTG_FS, OTG_Config);                   // Init Host

    //m_USB_Host.Timer = (USB_OTG_FS->HFNUM & USB_OTG_HFNUM_FRNUM);
}

//-------------------------------------------------------------------------------------------------
//
//   Function:        RegisterClass
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e USB_Host::RegisterClass(class USB_HostClassInterface* pClass)
{
    SystemState_e State = SYS_READY;

    if(m_Host.ClassNumber < USBH_MAX_NUM_SUPPORTED_CLASS)
    {
        // link the class to the USB Host handle
        m_Host.pClass[m_Host.ClassNumber++] = pClass;
    }
    else
    {
        //USBH_ErrLog("Max Class Number reached");
        State = SYS_ERROR;
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:        Start
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void USB_Host::Start(void)
{
    m_Host.pUSB_App->Start();                            // Start the low level driver
    m_Host.pUSB_App->DriverVBUS(true);                    // Activate VBUS on the port
}

//-------------------------------------------------------------------------------------------------
//
//   Function:        Start
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void USB_Host::Stop(void)
{
    m_Host.pUSB_App->Stop();                                // Stop and cleanup the low level driver
    m_Host.pUSB_App->DriverVBUS(false);                    // DeActivate VBUS on the port

    // Free control pipes
    FreePipe(m_Host.Control.PipeIn);
    FreePipe(m_Host.Control.PipeOut);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           run
//
//  Parameter(s):   void
//  Return:         void
//
//  Description:    main() for the task1
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void USB_Host::Run(void)
{
    nOS_FlagBits Flag;
    SystemState_e Status = SYS_FAIL;

    for(;;)
    {
        nOS_FlagWait(&m_Host.HostEvent, USBH_RUN_PROCESS, &Flag, nOS_FlagOption(NOS_FLAG_WAIT_ALL | NOS_FLAG_CLEAR_ON_EXIT), NOS_WAIT_INFINITE);

        switch(m_Host.State)
        {
            case HOST_IDLE:
            {
                if(m_Host.Device.is_connected)
                {
                    // Wait for 200 ms after connection
                    m_Host.State = HOST_DEV_WAIT_FOR_ATTACHMENT;
                    nOS_Sleep(200);
                    m_Host.pUSB_App->ResetPort();
                    nOS_FlagSend(&m_Host.HostEvent, USBH_PORT_EVENT, USBH_PORT_EVENT);
                }
            }
            break;

            case HOST_DEV_WAIT_FOR_ATTACHMENT:
            {
            }
            break;

            case HOST_DEV_ATTACHED:
            {
                //USBH_UsrLog("USB Device Attached");

                // Wait for 100 ms after Reset
                nOS_Sleep(100);

                m_Host.Device.speed = m_Host.pUSB_App->GetSpeed();
                m_Host.State = HOST_ENUMERATION;

                m_Host.Control.PipeOut = AllocPipe(0x00);
                m_Host.Control.PipeIn  = AllocPipe(0x80);

                // Open Control pipes
                m_Host.pUSB_App->OpenPipe(m_Host.Control.PipeIn,
                                   0x80,
                                 m_Host.Device.address,
                                 m_Host.Device.speed,
                                 USBH_EP_CONTROL,
                                 m_Host.Control.PipeSize);

                // Open Control pipes
                m_Host.pUSB_App->OpenPipe(m_Host.Control.PipeOut,
                                   0x00,
                                 m_Host.Device.address,
                                 m_Host.Device.speed,
                                 USBH_EP_CONTROL,
                                 m_Host.Control.PipeSize);

                nOS_FlagSend(&m_Host.HostEvent, USBH_PORT_EVENT, USBH_PORT_EVENT);
            }
            break;

            case HOST_ENUMERATION:
            {
                // Check for enumeration status
                if(HandleEnum() == SYS_READY)
                {
                    // The function shall return USBH_OK when full enumeration is complete
                    //USBH_UsrLog("Enumeration done.");
                    m_Host.Device.current_interface = 0;

                    if(m_Host.Device.DevDesc.bNumConfigurations == 1)
                    {
                        //USBH_UsrLog("This device has only 1 configuration.");
                        m_Host.State = HOST_SET_CONFIGURATION;

                    }
                    else
                    {
                        m_Host.State = HOST_INPUT;
                    }
                }
            }
            break;

            case HOST_INPUT:
            {
                // user callback for end of device basic enumeration
                if(m_Host.pUser != nullptr)
                {
                    m_Host.pUser(HOST_USER_SELECT_CONFIGURATION);
                    m_Host.State = HOST_SET_CONFIGURATION;
                    nOS_FlagSend(&m_Host.HostEvent, USBH_STATE_CHANGED_EVENT, USBH_STATE_CHANGED_EVENT);
                }
            }
            break;

            case HOST_SET_CONFIGURATION:
            {
                // set configuration
                if(SetConfig(m_Host.Device.CfgDesc.bConfigurationValue) == SYS_READY)
                {
                    m_Host.State = HOST_CHECK_CLASS;
                    //USBH_UsrLog("Default configuration set.");
                }
            }
            break;

            case HOST_CHECK_CLASS:
            {
                if(m_Host.ClassNumber == 0)
                {
                    //USBH_UsrLog("No Class has been registered.");
                }
                else
                {
                    m_Host.pActiveClass = nullptr;

                    for(int Index = 0; Index < USBH_MAX_NUM_SUPPORTED_CLASS; Index++)
                    {
                        //if(m_Host.pClass[Index]->ClassCode == m_Host.Device.CfgDesc.Itf_Desc[0].bInterfaceClass)
                        {
                            m_Host.pActiveClass = m_Host.pClass[Index];
                        }
                    }

                    if(m_Host.pActiveClass != nullptr)
                    {
                        if(m_Host.pActiveClass->InterfaceInitialize(&m_Host) == SYS_READY)
                        {
                            m_Host.State  = HOST_CLASS_REQUEST;
                            //USBH_UsrLog("%s class started.", m_Host.pActiveClass->Name);

                            // Inform user that a class has been activated
                            m_Host.pUser(HOST_USER_CLASS_SELECTED);
                        }
                        else
                        {
                            m_Host.State  = HOST_ABORT_STATE;
                            //USBH_UsrLog("Device not supporting %s class.", m_Host.pActiveClass->Name);
                        }
                    }
                    else
                    {
                        m_Host.State  = HOST_ABORT_STATE;
                        //USBH_UsrLog("No registered class for this device.");
                    }
                }
                nOS_FlagSend(&m_Host.HostEvent, USBH_STATE_CHANGED_EVENT, USBH_STATE_CHANGED_EVENT);
            }

            break;

            case HOST_CLASS_REQUEST:
            {
                // process class standard control requests state machine
                if(m_Host.pActiveClass != nullptr)
                {
//                    status = m_Host.pActiveClass->Requests(&m_Host);

                    if(Status == SYS_READY)
                    {
                        m_Host.State  = HOST_CLASS;
                    }
                }
                else
                {
                    m_Host.State  = HOST_ABORT_STATE;
                    //USBH_ErrLog ("Invalid Class Driver.");
                    nOS_FlagSend(&m_Host.HostEvent, USBH_STATE_CHANGED_EVENT, USBH_STATE_CHANGED_EVENT);
                }
            }
            break;

            case HOST_CLASS:
            {
                /* process class state machine */
                if(m_Host.pActiveClass != nullptr)
                {
                    m_Host.pActiveClass->Process();
                }
            }
            break;

            case HOST_DEV_DISCONNECTED:
            {
                DeInitStateMachine();

                // Re-Initialize Host for new enumeration
                if(m_Host.pActiveClass != nullptr)
                {
                    m_Host.pActiveClass->InterfaceDeInitialize();
                    m_Host.pActiveClass = nullptr;
                }
            }
            break;

            case HOST_ABORT_STATE:
            default:
            {
            }
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           HandleEnum
//
//  Parameter(s):   void
//  Return:         void
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e USB_Host::HandleEnum(void)
{
    SystemState_e Status = SYS_BUSY;

    switch(m_Host.EnumState)
    {
        case ENUM_IDLE:
        {
            // Get Device Desc for only 1st 8 bytes : To get EP0 MaxPacketSize
            if(GetDeviceDescriptor(8) == SYS_READY)
            {
                m_Host.Control.PipeSize = m_Host.Device.DevDesc.bMaxPacketSize;
                m_Host.EnumState = ENUM_GET_FULL_DEV_DESC;

                // Modify control channels configuration for MaxPacket size
                m_Host.pUSB_App->OpenPipe(m_Host.Control.PipeIn,
                                 0x80,
                                     m_Host.Device.address,
                                 m_Host.Device.speed,
                                 USBH_EP_CONTROL,
                                 m_Host.Control.PipeSize);

                // Open Control pipes
                m_Host.pUSB_App->OpenPipe(m_Host.Control.PipeOut,
                                 0x00,
                                 m_Host.Device.address,
                                 m_Host.Device.speed,
                                 USBH_EP_CONTROL,
                                 m_Host.Control.PipeSize);
            }
        }
        break;

        case ENUM_GET_FULL_DEV_DESC:
        {
            // Get FULL Device Desc
            if(GetDeviceDescriptor(USB_DEVICE_DESC_SIZE) == SYS_READY)
            {
                //USBH_UsrLog("PID: %xh", m_Host.Device.DevDesc.idProduct);
                //USBH_UsrLog("VID: %xh", m_Host.Device.DevDesc.idVendor);
                m_Host.EnumState = ENUM_SET_ADDR;
            }
        }
        break;

        case ENUM_SET_ADDR:
        {
            // Set address
            if(SetAddress(USBH_DEVICE_ADDRESS) == SYS_READY)
            {
                nOS_Sleep(2);
                m_Host.Device.address = USBH_DEVICE_ADDRESS;

                // user callback for device address assigned
                //USBH_UsrLog("Address (#%d) assigned.", m_Host.Device.address);
                m_Host.EnumState = ENUM_GET_CFG_DESC;

                // modify control channels to update device address
                m_Host.pUSB_App->OpenPipe(m_Host.Control.PipeIn,
                                 0x80,
                                 m_Host.Device.address,
                                 m_Host.Device.speed,
                                 USBH_EP_CONTROL,
                                 m_Host.Control.PipeSize);

                // Open Control pipes
                m_Host.pUSB_App->OpenPipe(m_Host.Control.PipeOut,
                                 0x00,
                                 m_Host.Device.address,
                                 m_Host.Device.speed,
                                 USBH_EP_CONTROL,
                                 m_Host.Control.PipeSize);
            }
        }
        break;

        case ENUM_GET_CFG_DESC:
        {
            // get standard configuration descriptor
            if(GetConfigDescriptor(USB_CONFIGURATION_DESC_SIZE) == SYS_READY)
            {
                m_Host.EnumState = ENUM_GET_FULL_CFG_DESC;
            }
        }
        break;

        case ENUM_GET_FULL_CFG_DESC:
        {
            // get FULL config descriptor (config, interface, endpoints)
            if(GetConfigDescriptor(m_Host.Device.CfgDesc.wTotalLength) == SYS_READY)
            {
                m_Host.EnumState = ENUM_GET_MFC_STRING_DESC;
            }
        }
        break;

        case ENUM_GET_MFC_STRING_DESC:
        {
            if(m_Host.Device.DevDesc.iManufacturer != 0)
            {
                /* Check that Manufacturer String is available */

                if(GetStringDescriptor(m_Host.Device.DevDesc.iManufacturer,
                                       m_Host.Device.Data,
                                       0xff) == SYS_READY)
                {
                    // User callback for Manufacturing string
                    //USBH_UsrLog("Manufacturer : %s",  (char *)m_Host.Device.Data);
                    m_Host.EnumState = ENUM_GET_PRODUCT_STRING_DESC;
                    nOS_FlagSend(&m_Host.HostEvent, USBH_STATE_CHANGED_EVENT, USBH_STATE_CHANGED_EVENT);
                }
            }
            else
            {
                //USBH_UsrLog("Manufacturer : N/A");
                m_Host.EnumState = ENUM_GET_PRODUCT_STRING_DESC;
                nOS_FlagSend(&m_Host.HostEvent, USBH_STATE_CHANGED_EVENT, USBH_STATE_CHANGED_EVENT);
            }
        }
        break;

        case ENUM_GET_PRODUCT_STRING_DESC:
        {
            if(m_Host.Device.DevDesc.iProduct != 0)
            {
                /* Check that Product string is available */
                if(GetStringDescriptor(m_Host.Device.DevDesc.iProduct,
                                       m_Host.Device.Data,
                                       0xff) == SYS_READY)
                {
                    /* User callback for Product string */
                    //USBH_UsrLog("Product : %s",  (char *)m_Host.Device.Data);
                    m_Host.EnumState = ENUM_GET_SERIALNUM_STRING_DESC;
                }
            }
            else
            {
                //USBH_UsrLog("Product : N/A");
                m_Host.EnumState = ENUM_GET_SERIALNUM_STRING_DESC;
                nOS_FlagSend(&m_Host.HostEvent, USBH_STATE_CHANGED_EVENT, USBH_STATE_CHANGED_EVENT);
            }
        }
        break;

        case ENUM_GET_SERIALNUM_STRING_DESC:
        {
            if(m_Host.Device.DevDesc.iSerialNumber != 0)
            {
                // Check that Serial number string is available
                if(GetStringDescriptor(m_Host.Device.DevDesc.iSerialNumber,
                                       m_Host.Device.Data,
                                       0xff) == SYS_READY)
                {
                    // User callback for Serial number string
                    //USBH_UsrLog("Serial Number : %s",  (char *)m_Host.Device.Data);
                    Status = SYS_READY;
                }
            }
            else
            {
                //USBH_UsrLog("Serial Number : N/A");
                Status = SYS_READY;
                nOS_FlagSend(&m_Host.HostEvent, USBH_STATE_CHANGED_EVENT, USBH_STATE_CHANGED_EVENT);
            }
        }
        break;

        default:
        {
        }
        break;
    }

    return Status;
}

/**
  * @brief  DeInitStateMachine
  *         De-Initialize the Host state machine.
  * @param  phost: Host Handle
  * @retval USBH Status
  */
void  USB_Host::DeInitStateMachine(void)
{
    // Clear Pipes flags
    for(int i = 0; i < USBH_MAX_PIPES_NBR; i++)
    {
        m_Host.Pipes[i] = 0;
    }

    for(int i = 0; i< USBH_MAX_DATA_BUFFER; i++)
    {
        m_Host.Device.Data[i] = 0;
    }

    m_Host.State        = HOST_IDLE;
    m_Host.EnumState    = ENUM_IDLE;
    m_Host.RequestState = CMD_SEND;
    m_Host.Timer        = 0;

    m_Host.Control.State      = CTRL_SETUP;
    m_Host.Control.PipeSize   = USBH_MPS_DEFAULT;
    m_Host.Control.ErrorCount = 0;

    m_Host.Device.address = USBH_ADDRESS_DEFAULT;
    m_Host.Device.speed   = USBH_SPEED_FULL;
}

#if 0
//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
USB_MSC_HostStatus_e USB_Host::GetStatus(void)
{

    /*
    if(HCD_IsDeviceConnected(&m_OTG_Core) && (m_MSC_HostStatus == USB_MSC_DEV_CONNECTED))
    {
        return m_MSC_HostStatus;
    }
*/
    return USB_MSC_DEV_DETACHED;
}


/*
void CUSB::IRQHandler(void)
{
    USBH_OTG_ISR_Handler(&m_OTG_Core);
}
*/

//-------------------------------------------------------------------------------------------------
#endif

/**
  * @brief  USBH_Alloc_Pipe
  *         Allocate a new Pipe
  * @param  ep_addr: End point for which the Pipe to be allocated
  * @retval Pipe number
  */
uint8_t USB_Host::AllocPipe(uint8_t ep_addr)
{
    uint16_t pipe;

        pipe = GetFreePipe();

        if(pipe != 0xFFFF)
        {
            m_Host.Pipes[pipe] = 0x8000 | ep_addr;
        }
        return pipe;
}

/**
  * @brief  USBH_Free_Pipe
  *         Free the USB Pipe
  * @param  idx: Pipe number to be freed
  * @retval USBH Status
  */
void USB_Host::FreePipe(uint8_t Index)
{
    if(Index < 11)
    {
        m_Host.Pipes[Index] &= 0x7FFF;
    }
}

/**
  * @brief  USBH_GetFreePipe
  *         Get a free Pipe number for allocation to a device endpoint
  * @retval idx: Free Pipe number
  */
uint16_t USB_Host::GetFreePipe(void)
{
    for(int Index = 0; Index < 11; Index++)
    {
        if((m_Host.Pipes[Index] & 0x8000) == 0)
        {
            return Index;
        }
    }

    return 0xFFFF;
}

SystemState_e USB_Host::SetConfig(uint16_t cfg_idx)
{
    if(m_Host.RequestState == CMD_SEND)
    {
        m_Host.Control.Setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_DEVICE |    USB_REQ_TYPE_STANDARD;
        m_Host.Control.Setup.b.bRequest      = USB_REQ_SET_CONFIGURATION;
        m_Host.Control.Setup.b.wValue.w      = cfg_idx;
        m_Host.Control.Setup.b.wIndex.w      = 0;
        m_Host.Control.Setup.b.wLength.w     = 0;
    }
    return ControlRequest(0, 0);
}

//  USBH_CtlReq  USBH_CtlReq sends a control request and provide the status after completion of the request
/* @param  phost: Host Handle
  * @param  req: Setup Request Structure
  * @param  buff: data buffer address to store the response
  * @param  length: length of the response
  * @retval USBH Status
  */
SystemState_e USB_Host::ControlRequest(uint8_t* pBuffer, uint16_t length)
{
    SystemState_e Status = SYS_BUSY;

    switch(m_Host.RequestState)
    {
        case CMD_SEND:
        {
            // Start a SETUP transfer
            m_Host.Control.pBuffer = pBuffer;
            m_Host.Control.Length  = length;
            m_Host.Control.State   = CTRL_SETUP;
            m_Host.RequestState    = CMD_WAIT;
            Status            = SYS_BUSY;
            nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
        }
        break;

        case CMD_WAIT:
        {
            Status = HandleControl();

            if(Status == SYS_READY)
            {
                // Commands successfully sent and Response Received
                m_Host.RequestState  = CMD_SEND;
                m_Host.Control.State = CTRL_IDLE;
                Status = SYS_READY;
            }
            else if (Status == SYS_FAIL)
            {
                // Failure Mode
                m_Host.RequestState = CMD_SEND;
                Status = SYS_FAIL;
            }
        }
        break;

        default:
        {
        }
        break;
    }

    return Status;
}


/**
  * @brief  USBH_HandleControl
  *         Handles the USB control transfer state machine
  * @param  phost: Host Handle
  * @retval USBH Status
  */
SystemState_e USB_Host::HandleControl(void)
{
    uint8_t direction;
    SystemState_e Status = SYS_BUSY;
    USBH_URB_State_e URB_Status = USBH_URB_IDLE;

    switch(m_Host.Control.State)
    {
        case CTRL_SETUP:
        {
            // send a SETUP packet
            m_Host.pUSB_App->SubmitURB(m_Host.Control.PipeOut, 0, USBH_EP_CONTROL, USBH_PID_SETUP, (uint8_t *)m_Host.Control.Setup.d8, USBH_SETUP_PKT_SIZE, 0);
            m_Host.Control.State = CTRL_SETUP_WAIT;
        }
        break;

        case CTRL_SETUP_WAIT:
        {
            URB_Status = m_Host.pUSB_App->GetURB_State(m_Host.Control.PipeOut);

            // case SETUP packet sent successfully
            if(URB_Status == USBH_URB_DONE)
            {
                direction = (m_Host.Control.Setup.b.bmRequestType & USB_REQ_DIR_MASK);

                // check if there is a data stage
                if(m_Host.Control.Setup.b.wLength.w != 0)
                {
                    if(direction == USB_D2H)
                    {
                        // Data Direction is IN
                        m_Host.Control.State = CTRL_DATA_IN;
                    }
                    else
                    {
                        // Data Direction is OUT
                        m_Host.Control.State = CTRL_DATA_OUT;
                    }
                }
                else // No DATA stage
                {
                    // If there is No Data Transfer Stage
                    if(direction == USB_D2H)
                    {
                        // Data Direction is IN
                        m_Host.Control.State = CTRL_STATUS_OUT;
                    }
                    else
                    {
                        // Data Direction is OUT
                        m_Host.Control.State = CTRL_STATUS_IN;
                    }
                }
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
            else if(URB_Status == USBH_URB_ERROR)
            {
                m_Host.Control.State = CTRL_ERROR;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
        }
        break;

        case CTRL_DATA_IN:
        {
            // Issue an IN token
            m_Host.Control.Timer = m_Host.Timer;
            m_Host.pUSB_App->SubmitURB(m_Host.Control.PipeIn, 1, USBH_EP_CONTROL, USBH_PID_DATA, m_Host.Control.pBuffer, m_Host.Control.Length, 0);
            m_Host.Control.State = CTRL_DATA_IN_WAIT;
        }
        break;

        case CTRL_DATA_IN_WAIT:
        {
            URB_Status = m_Host.pUSB_App->GetURB_State(m_Host.Control.PipeIn);

            // check is DATA packet transferred successfully
            if(URB_Status == USBH_URB_DONE)
            {
                m_Host.Control.State = CTRL_STATUS_OUT;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }

            // manage error cases
            if(URB_Status == USBH_URB_STALL)
            {
                // In stall case, return to previous machine state
                Status = SYS_NOT_SUPPORTED;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
            else if(URB_Status == USBH_URB_ERROR)
            {
                // Device error
                m_Host.Control.State = CTRL_ERROR;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
        }
        break;

        case CTRL_DATA_OUT:
        {
            // Control Send data
            m_Host.pUSB_App->SubmitURB(m_Host.Control.PipeOut, 0, USBH_EP_CONTROL, USBH_PID_DATA, m_Host.Control.pBuffer, m_Host.Control.Length, 1);
            m_Host.Control.Timer = m_Host.Timer;
            m_Host.Control.State = CTRL_DATA_OUT_WAIT;
        }
        break;

        case CTRL_DATA_OUT_WAIT:
        {
            URB_Status = m_Host.pUSB_App->GetURB_State(m_Host.Control.PipeOut);

            if(URB_Status == USBH_URB_DONE)
            {
                // If the Setup Pkt is sent successful, then change the state
                m_Host.Control.State = CTRL_STATUS_IN;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
            else if(URB_Status == USBH_URB_STALL)                // handle error cases
            {
                // In stall case, return to previous machine state
                m_Host.Control.State = CTRL_STALLED;
                Status = SYS_NOT_SUPPORTED;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
            else if(URB_Status == USBH_URB_NOTREADY)
            {
                // Nack received from device
                m_Host.Control.State = CTRL_DATA_OUT;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
            else if(URB_Status == USBH_URB_ERROR)
            {
                // device error
                m_Host.Control.State = CTRL_ERROR;
                Status = SYS_FAIL;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
        }
        break;

        case CTRL_STATUS_IN:
        {
            // Send 0 bytes out packet
            m_Host.pUSB_App->SubmitURB(m_Host.Control.PipeIn, 1, USBH_EP_CONTROL, USBH_PID_DATA, nullptr, 0, 0);
            m_Host.Control.Timer = m_Host.Timer;
            m_Host.Control.State = CTRL_STATUS_IN_WAIT;
        }
        break;

        case CTRL_STATUS_IN_WAIT:
        {
            URB_Status = m_Host.pUSB_App->GetURB_State(m_Host.Control.PipeIn);

            if(URB_Status == USBH_URB_DONE)
            {
                // Control transfers completed, Exit the State Machine
                m_Host.Control.State = CTRL_COMPLETE;
                Status = SYS_READY;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
            else if(URB_Status == USBH_URB_ERROR)
            {
                m_Host.Control.State = CTRL_ERROR;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
            else if(URB_Status == USBH_URB_STALL)
            {
                // Control transfers completed, Exit the State Machine
                Status = SYS_NOT_SUPPORTED;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
        }
        break;

        case CTRL_STATUS_OUT:
        {
             // Control Send data
            m_Host.pUSB_App->SubmitURB(m_Host.Control.PipeOut, 0, USBH_EP_CONTROL, USBH_PID_DATA, nullptr, 0, 1);
            m_Host.Control.Timer = m_Host.Timer;
            m_Host.Control.State = CTRL_STATUS_OUT_WAIT;
        }
        break;

        case CTRL_STATUS_OUT_WAIT:
        {
            URB_Status = m_Host.pUSB_App->GetURB_State(m_Host.Control.PipeOut);

            if(URB_Status == USBH_URB_DONE)
            {
                Status = SYS_READY;
                m_Host.Control.State = CTRL_COMPLETE;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
            else if(URB_Status == USBH_URB_NOTREADY)
            {
                m_Host.Control.State = CTRL_STATUS_OUT;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
            else if(URB_Status == USBH_URB_ERROR)
            {
                m_Host.Control.State = CTRL_ERROR;
                nOS_FlagSend(&m_Host.HostEvent, USBH_CONTROL_EVENT, USBH_CONTROL_EVENT);
            }
        }
        break;

        case CTRL_ERROR:
        {
            /* After a halt condition is encountered or an error is detected by the
               host, a control endpoint is allowed to recover by accepting the next Setup
               PID; i.e., recovery actions via some other pipe are not required for control
               endpoints. For the Default Control Pipe, a device reset will ultimately be
               required to clear the halt or error condition if the next Setup PID is not
               accepted. */
            if(m_Host.Control.ErrorCount++ < USBH_MAX_ERROR_COUNT)
            {
                // try to recover control
                m_Host.pUSB_App->Stop();

                // Do the transmission again, starting from SETUP Packet
                m_Host.Control.State = CTRL_SETUP;
                m_Host.RequestState  = CMD_SEND;
            }
            else
            {
              m_Host.pUser(HOST_USER_UNRECOVERED_ERROR);
              m_Host.Control.ErrorCount = 0;
              //USBH_ErrLog("Control error");
              Status = SYS_FAIL;
            }
        }
        break;

        default:
        {

        }
        break;
    }

    return Status;
}

// USBH_Get_StringDesc  Issues string Descriptor command to the device. Once the response received, it parses the string descriptor and updates the status.
  /* @param  phost: Host Handle
  * @param  string_index: String index for the descriptor
  * @param  buff: Buffer address for the descriptor
  * @param  length: Length of the descriptor
  * @retval USBH Status
  */
SystemState_e USB_Host::GetStringDescriptor(uint8_t string_index, uint8_t *buff, uint16_t Length)
{
    SystemState_e Status;

    if((Status = GetDescriptor(USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD, USB_DESC_STRING | string_index, m_Host.Device.Data, Length)) == SYS_READY)
    {
        // Commands successfully sent and Response Received
        ParseStringDescriptor(m_Host.Device.Data, buff, Length);
    }

    return Status;
}

/**
  * @brief  USBH_GetDescriptor
  *         Issues Descriptor command to the device. Once the response received,
  *         it parses the descriptor and updates the status.
  * @param  phost: Host Handle
  * @param  req_type: Descriptor type
  * @param  value_idx: Value for the GetDescriptr request
  * @param  buff: Buffer to store the descriptor
  * @param  length: Length of the descriptor
  * @retval USBH Status
  */
SystemState_e USB_Host::GetDescriptor(uint8_t req_type, uint16_t value_idx, uint8_t* buff, uint16_t length)
{
    if(m_Host.RequestState == CMD_SEND)
    {
        m_Host.Control.Setup.b.bmRequestType = USB_D2H | req_type;
        m_Host.Control.Setup.b.bRequest = USB_REQ_GET_DESCRIPTOR;
        m_Host.Control.Setup.b.wValue.w = value_idx;

        if((value_idx & 0xff00) == USB_DESC_STRING)
        {
            m_Host.Control.Setup.b.wIndex.w = 0x0409;
        }
        else
        {
            m_Host.Control.Setup.b.wIndex.w = 0;
        }

        m_Host.Control.Setup.b.wLength.w = length;
    }

    return ControlRequest(buff, length);
}


/**
  * @brief  USBH_ParseStringDesc
  *         This function Parses the string descriptor
  * @param  psrc: Source pointer containing the descriptor data
  * @param  pdest: Destination address pointer
  * @param  length: Length of the descriptor
  * @retval None
  */
void USB_Host::ParseStringDescriptor(uint8_t* psrc, uint8_t* pdest, uint16_t length)
{
    uint16_t strlength;
    uint16_t idx;

    /* The UNICODE string descriptor is not NULL-terminated. The string length is
         computed by substracting two from the value of the first byte of the descriptor. */

    /* Check which is lower size, the Size of string or the length of bytes read from the device */

    if(psrc[1] == USB_DESC_TYPE_STRING)
    {
        // Make sure the Descriptor is String Type

        // psrc[0] contains Size of Descriptor, subtract 2 to get the length of string
        strlength = (((psrc[0] - 2) <= length) ? (psrc[0] - 2) : length);
        psrc += 2; // Adjust the offset ignoring the String Len and Descriptor type

        for(idx = 0; idx < strlength; idx += 2)
        {
            // Copy Only the string and ignore the UNICODE ID, hence add the src
            *pdest =  psrc[idx];
            pdest++;
        }
        *pdest = 0; // mark end of string
    }
}


/**
  * @brief  USBH_Get_DevDesc
  *         Issue Get Device Descriptor command to the device. Once the response
  *         received, it parses the device descriptor and updates the status.
  * @param  phost: Host Handle
  * @param  length: Length of the descriptor
  * @retval USBH Status
  */
SystemState_e USB_Host::GetDeviceDescriptor(uint8_t length)
{
    SystemState_e Status;

    if((Status = GetDescriptor(USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD, USB_DESC_DEVICE, m_Host.Device.Data, length)) == SYS_READY)
    {
        // Commands successfully sent and Response Received
        ParseDeviceDescriptor(&m_Host.Device.DevDesc, m_Host.Device.Data, length);
    }

    return Status;
}

/**
  * @brief  USBH_ParseDevDesc
  *         This function Parses the device descriptor
  * @param  dev_desc: device_descriptor destination address
  * @param  buf: Buffer where the source descriptor is available
  * @param  length: Length of the descriptor
  * @retval None
  */
void USB_Host::ParseDeviceDescriptor(USBH_DeviceDescriptor_t* pDescriptor, uint8_t *buf, uint16_t length)
{
    pDescriptor->bLength            = *(uint8_t*)(buf + 0);
    pDescriptor->bDescriptorType    = *(uint8_t*)(buf + 1);
    pDescriptor->bcdUSB             = LE16(buf + 2);
    pDescriptor->bDeviceClass       = *(uint8_t*)(buf + 4);
    pDescriptor->bDeviceSubClass    = *(uint8_t*)(buf + 5);
    pDescriptor->bDeviceProtocol    = *(uint8_t*)(buf + 6);
    pDescriptor->bMaxPacketSize     = *(uint8_t*)(buf + 7);

    if(length > 8)
    {
        // For 1st time after device connection, Host may issue only 8 bytes for Device Descriptor Length
        pDescriptor->idVendor           = LE16(buf + 8);
        pDescriptor->idProduct          = LE16(buf + 10);
        pDescriptor->bcdDevice          = LE16(buf + 12);
        pDescriptor->iManufacturer      = *(uint8_t*)(buf + 14);
        pDescriptor->iProduct           = *(uint8_t*)(buf + 15);
        pDescriptor->iSerialNumber      = *(uint8_t*)(buf + 16);
        pDescriptor->bNumConfigurations = *(uint8_t*)(buf + 17);
    }
}

/**
  * @brief  USBH_SetAddress
  *         This command sets the address to the connected device
  * @param  phost: Host Handle
  * @param  DeviceAddress: Device address to assign
  * @retval USBH Status
  */
SystemState_e USB_Host::SetAddress(uint8_t DeviceAddress)
{
    if(m_Host.RequestState == CMD_SEND)
    {
        m_Host.Control.Setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD;
        m_Host.Control.Setup.b.bRequest      = USB_REQ_SET_ADDRESS;
        m_Host.Control.Setup.b.wValue.w      = (uint16_t)DeviceAddress;
        m_Host.Control.Setup.b.wIndex.w      = 0;
        m_Host.Control.Setup.b.wLength.w     = 0;
    }

    return ControlRequest(0, 0);
}

/**
  * @brief  ClrFeature
  *         This request is used to clear or disable a specific feature.
  * @param  phost: Host Handle
  * @param  ep_num: endpoint number
  * @param  hc_num: Host channel number
  * @retval USBH Status
  */
SystemState_e USB_Host::ClrFeature(uint8_t ep_num)
{

    if(m_Host.RequestState == CMD_SEND)
    {
        m_Host.Control.Setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_ENDPOINT | USB_REQ_TYPE_STANDARD;
         m_Host.Control.Setup.b.bRequest      = USB_REQ_CLEAR_FEATURE;
        m_Host.Control.Setup.b.wValue.w      = FEATURE_SELECTOR_ENDPOINT;
        m_Host.Control.Setup.b.wIndex.w      = ep_num;
        m_Host.Control.Setup.b.wLength.w     = 0;
    }

    return ControlRequest(0, 0);
}


/**
  * @brief  USBH_Get_CfgDesc
  *         Issues Configuration Descriptor to the device. Once the response
  *         received, it parses the configuration descriptor and updates the
  *         status.
  * @param  phost: Host Handle
  * @param  length: Length of the descriptor
  * @retval USBH Status
  */
SystemState_e USB_Host::GetConfigDescriptor(uint16_t length)
{
    SystemState_e Status;
    uint8_t *pData;

   #if(USBH_KEEP_CFG_DESCRIPTOR == 1)
    pData = phost->device.CfgDesc_Raw;
   #else
    pData = m_Host.Device.Data;
   #endif

    if((Status = GetDescriptor(USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD, USB_DESC_CONFIGURATION, pData, length)) == SYS_READY)
    {
        // Commands successfully sent and Response Received
        ParseConfigDescriptor(&m_Host.Device.CfgDesc, pData, length);
    }

    return Status;
}

/**
  * @brief  USBH_ParseCfgDesc
  *         This function Parses the configuration descriptor
  * @param  cfg_desc: Configuration Descriptor address
  * @param  buf: Buffer where the source descriptor is available
  * @param  length: Length of the descriptor
  * @retval None
  */
void USB_Host::ParseConfigDescriptor(USBH_ConfigDescriptor_t* cfg_desc, uint8_t* buf, uint16_t length)
{
    USBH_InterfaceDescriptor_t   *pif;
    USBH_EndPointDescriptor_t    *pep;
    USBH_DescHeader_t             *pdesc = (USBH_DescHeader_t *)buf;
    uint16_t                      ptr;
    int8_t                        if_ix = 0;
    int8_t                        ep_ix = 0;

    pdesc   = (USBH_DescHeader_t *)buf;

    /* Parse configuration descriptor */
    cfg_desc->bLength             = *(uint8_t  *) (buf + 0);
    cfg_desc->bDescriptorType     = *(uint8_t  *) (buf + 1);
    cfg_desc->wTotalLength        = LE16 (buf + 2);
    cfg_desc->bNumInterfaces      = *(uint8_t  *) (buf + 4);
    cfg_desc->bConfigurationValue = *(uint8_t  *) (buf + 5);
    cfg_desc->iConfiguration      = *(uint8_t  *) (buf + 6);
    cfg_desc->bmAttributes        = *(uint8_t  *) (buf + 7);
    cfg_desc->bMaxPower           = *(uint8_t  *) (buf + 8);

    if(length > USB_CONFIGURATION_DESC_SIZE)
    {
        ptr = USB_LEN_CFG_DESC;
        pif = (USBH_InterfaceDescriptor_t *)0;

           while((if_ix < USBH_MAX_NUM_INTERFACES ) && (ptr < cfg_desc->wTotalLength))
           {
               pdesc = GetNextDescriptor((uint8_t *)pdesc, &ptr);

               if(pdesc->bDescriptorType   == USB_DESC_TYPE_INTERFACE)
               {
                   pif = &cfg_desc->Itf_Desc[if_ix];
                   ParseInterfaceDescriptor(pif, (uint8_t *)pdesc);

                   ep_ix = 0;
                   pep = (USBH_EndPointDescriptor_t *)0;

                   while((ep_ix < pif->bNumEndpoints) && (ptr < cfg_desc->wTotalLength))
                   {
                       pdesc = GetNextDescriptor((uint8_t*) pdesc, &ptr);

                       if(pdesc->bDescriptorType == USB_DESC_TYPE_ENDPOINT)
                       {
                           pep = &cfg_desc->Itf_Desc[if_ix].Ep_Desc[ep_ix];
                           ParseEndPointDescriptor(pep, (uint8_t *)pdesc);
                           ep_ix++;
                       }
                   }

                   if_ix++;
               }
           }
    }
}

// This function return the next descriptor header
  /* @param  buf: Buffer where the cfg descriptor is available
  * @param  ptr: data pointer inside the cfg descriptor
  * @retval next header
  */
USBH_DescHeader_t* USB_Host::GetNextDescriptor(uint8_t* pbuf, uint16_t* ptr)
{
    USBH_DescHeader_t* pnext;

    *ptr += ((USBH_DescHeader_t *)pbuf)->bLength;
    pnext = (USBH_DescHeader_t *)((uint8_t *)pbuf + ((USBH_DescHeader_t *)pbuf)->bLength);
    return pnext;
}

/**
* @brief  USBH_ParseInterfaceDesc
*         This function Parses the interface descriptor
* @param  if_descriptor : Interface descriptor destination
* @param  buf: Buffer where the descriptor data is available
* @retval None
*/
void USB_Host::ParseInterfaceDescriptor(USBH_InterfaceDescriptor_t* if_descriptor, uint8_t* buf)
{
    if_descriptor->bLength            = *(uint8_t*)(buf + 0);
    if_descriptor->bDescriptorType    = *(uint8_t*)(buf + 1);
    if_descriptor->bInterfaceNumber   = *(uint8_t*)(buf + 2);
    if_descriptor->bAlternateSetting  = *(uint8_t*)(buf + 3);
    if_descriptor->bNumEndpoints      = *(uint8_t*)(buf + 4);
    if_descriptor->bInterfaceClass    = *(uint8_t*)(buf + 5);
    if_descriptor->bInterfaceSubClass = *(uint8_t*)(buf + 6);
    if_descriptor->bInterfaceProtocol = *(uint8_t*)(buf + 7);
    if_descriptor->iInterface         = *(uint8_t*)(buf + 8);
}

/**
  * @brief  USBH_ParseEPDesc
  *         This function Parses the endpoint descriptor
  * @param  ep_descriptor: Endpoint descriptor destination address
  * @param  buf: Buffer where the parsed descriptor stored
  * @retval None
  */
void USB_Host::ParseEndPointDescriptor(USBH_EndPointDescriptor_t* ep_descriptor, uint8_t *buf)
{
    ep_descriptor->bLength          = *(uint8_t*)(buf + 0);
    ep_descriptor->bDescriptorType  = *(uint8_t*)(buf + 1);
    ep_descriptor->bEndpointAddress = *(uint8_t*)(buf + 2);
    ep_descriptor->bmAttributes     = *(uint8_t*)(buf + 3);
    ep_descriptor->wMaxPacketSize   = LE16 (buf + 4);
    ep_descriptor->bInterval        = *(uint8_t*)(buf + 6);
}

/**
  * @brief  USBH_SelectInterface
  *         Select current interface.
  * @param  phost: Host Handle
  * @param  interface: Interface number
  * @retval USBH Status
  */
SystemState_e USB_Host::SelectInterface(uint8_t interface)
{
    SystemState_e Status = SYS_READY;

    if(interface < m_Host.Device.CfgDesc.bNumInterfaces)
    {
        m_Host.Device.current_interface = interface;
        //USBH_UsrLog("Switching to Interface (#%d)", interface);
        //USBH_UsrLog("Class    : %xh", m_Host.Device.CfgDesc.Itf_Desc[interface].bInterfaceClass);
        //USBH_UsrLog("SubClass : %xh", m_Host.Device.CfgDesc.Itf_Desc[interface].bInterfaceSubClass);
        //USBH_UsrLog("Protocol : %xh", m_Host.Device.CfgDesc.Itf_Desc[interface].bInterfaceProtocol);
    }
    else
    {
        //USBH_ErrLog ("Cannot Select This Interface.");
        Status = SYS_FAIL;
    }

    return Status;
}



/**
  * @brief  USBH_FindInterfaceIndex
  *         Find the interface index for a specific class interface and alternate setting number.
  * @param  phost: Host Handle
  * @param  interface_number: interface number
  * @param  alt_settings    : alternate setting number
  * @retval interface index in the configuration structure
  * @note : (1)interface index 0xFF means interface index not found
  */
/*
uint8_t  USB_Host::FindInterfaceIndex(uint8_t interface_number, uint8_t alt_settings)
{
    USBH_InterfaceDescriptor_t*   pif;
    USBH_ConfigDescriptor_t*      pcfg;
    int8_t                        if_ix = 0;

    pif = (USBH_InterfaceDescriptor_t *)0;
    pcfg = &m_Host.Device.CfgDesc;

    while(if_ix < USBH_MAX_NUM_INTERFACES)
    {
        pif = &pcfg->Itf_Desc[if_ix];

        if((pif->bInterfaceNumber == interface_number) && (pif->bAlternateSetting == alt_settings))
        {
            return  if_ix;
        }

        if_ix++;
    }
    return USB_NOT_VALID_INTERFACE;
}
*/

/**
  * @brief  USBH_FindInterface
  *         Find the interface index for a specific class.
  * @param  phost: Host Handle
  * @param  Class: Class code
  * @param  SubClass: SubClass code
  * @param  Protocol: Protocol code
  * @retval interface index in the configuration structure
  * @note : (1)interface index 0xFF means interface index not found
  */
uint8_t USB_Host::FindInterface(uint8_t Class, uint8_t SubClass, uint8_t Protocol)
{
    USBH_InterfaceDescriptor_t* pif;
    USBH_ConfigDescriptor_t*    pcfg;
    int8_t                      if_ix = 0;

    pif  = (USBH_InterfaceDescriptor_t *)0;
    pcfg = &m_Host.Device.CfgDesc;

    while(if_ix < USBH_MAX_NUM_INTERFACES)
    {
        pif = &pcfg->Itf_Desc[if_ix];

        if(((pif->bInterfaceClass    == Class)    || (Class    == USB_NOT_VALID_INTERFACE))  &&
           ((pif->bInterfaceSubClass == SubClass) || (SubClass == USB_NOT_VALID_INTERFACE))  &&
           ((pif->bInterfaceProtocol == Protocol) || (Protocol == USB_NOT_VALID_INTERFACE)))
        {
            return  if_ix;
        }

        if_ix++;
    }

    return USB_NOT_VALID_INTERFACE;
}


