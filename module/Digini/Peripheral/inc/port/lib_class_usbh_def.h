//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_usbh_def.h
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


#pragma once

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define USBH_MAX_NUM_ENDPOINTS                            3
#define USBH_MAX_NUM_INTERFACES                           1
#define USBH_MAX_NUM_CONFIGURATION                        1
#define USBH_MAX_NUM_SUPPORTED_CLASS                      1
#define USBH_KEEP_CFG_DESCRIPTOR                          0
#define USBH_MAX_SIZE_CONFIGURATION                       0x200
#define USBH_MAX_DATA_BUFFER                              0x200
#define USBH_DEBUG_LEVEL                                  2

#define USBH_PID_SETUP                                    0
#define USBH_PID_DATA                                     1

#define USBH_EP_CONTROL                                   0
#define USBH_EP_ISO                                       1
#define USBH_EP_BULK                                      2
#define USBH_EP_INTERRUPT                                 3

#define USB_EP_TYPE_CTRL                                0x00
#define USB_EP_TYPE_ISOC                                0x01
#define USB_EP_TYPE_BULK                                0x02
#define USB_EP_TYPE_INTR                                0x03

#define USB_EP_DIR_OUT                                  0x00
#define USB_EP_DIR_IN                                   0x80
#define USB_EP_DIR_MSK                                  0x80

#define USBH_SETUP_PKT_SIZE                             8

#define USB_LEN_DESC_HDR                                0x02
#define USB_LEN_DEV_DESC                                0x12
#define USB_LEN_CFG_DESC                                0x09
#define USB_LEN_IF_DESC                                 0x09
#define USB_LEN_EP_DESC                                 0x07
#define USB_LEN_OTG_DESC                                0x03
#define USB_LEN_SETUP_PKT                               0x08

// bmRequestType :D7 Data Phase Transfer Direction
#define USB_REQ_DIR_MASK                                0x80
#define USB_H2D                                         0x00
#define USB_D2H                                         0x80

// bmRequestType D6..5 Type
#define USB_REQ_TYPE_STANDARD                           0x00
#define USB_REQ_TYPE_CLASS                              0x20
#define USB_REQ_TYPE_VENDOR                             0x40
#define USB_REQ_TYPE_RESERVED                           0x60

// bmRequestType D4..0 Recipient
#define USB_REQ_RECIPIENT_DEVICE                        0x00
#define USB_REQ_RECIPIENT_INTERFACE                     0x01
#define USB_REQ_RECIPIENT_ENDPOINT                      0x02
#define USB_REQ_RECIPIENT_OTHER                         0x03

// Table 9-4. Standard Request Codes
// bRequest , Value
#define USB_REQ_GET_STATUS                              0x00
#define USB_REQ_CLEAR_FEATURE                           0x01
#define USB_REQ_SET_FEATURE                             0x03
#define USB_REQ_SET_ADDRESS                             0x05
#define USB_REQ_GET_DESCRIPTOR                          0x06
#define USB_REQ_SET_DESCRIPTOR                          0x07
#define USB_REQ_GET_CONFIGURATION                       0x08
#define USB_REQ_SET_CONFIGURATION                       0x09
#define USB_REQ_GET_INTERFACE                           0x0A
#define USB_REQ_SET_INTERFACE                           0x0B
#define USB_REQ_SYNCH_FRAME                             0x0C

#define HOST_USER_SELECT_CONFIGURATION                  1
#define HOST_USER_CLASS_ACTIVE                          2
#define HOST_USER_CLASS_SELECTED                        3
#define HOST_USER_CONNECTION                            4
#define HOST_USER_DISCONNECTION                         5
#define HOST_USER_UNRECOVERED_ERROR                     6

// Table 9-5. Descriptor Types of USB Specifications
#define USB_DESC_TYPE_DEVICE                               1
#define USB_DESC_TYPE_CONFIGURATION                        2
#define USB_DESC_TYPE_STRING                               3
#define USB_DESC_TYPE_INTERFACE                            4
#define USB_DESC_TYPE_ENDPOINT                             5
#define USB_DESC_TYPE_DEVICE_QUALIFIER                     6
#define USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION            7
#define USB_DESC_TYPE_INTERFACE_POWER                      8
#define USB_DESC_TYPE_HID                                  0x21
#define USB_DESC_TYPE_HID_REPORT                           0x22

#define USB_DEVICE_DESC_SIZE                            18
#define USB_CONFIGURATION_DESC_SIZE                         9
#define USB_HID_DESC_SIZE                               9
#define USB_INTERFACE_DESC_SIZE                         9
#define USB_ENDPOINT_DESC_SIZE                          7

// Descriptor Type and Descriptor Index
// Use the following values when calling the function USBH_GetDescriptor
#define USB_DESC_DEVICE                                    ((USB_DESC_TYPE_DEVICE << 8) & 0xFF00)
#define USB_DESC_CONFIGURATION                             ((USB_DESC_TYPE_CONFIGURATION << 8) & 0xFF00)
#define USB_DESC_STRING                                    ((USB_DESC_TYPE_STRING << 8) & 0xFF00)
#define USB_DESC_INTERFACE                                 ((USB_DESC_TYPE_INTERFACE << 8) & 0xFF00)
#define USB_DESC_ENDPOINT                                  ((USB_DESC_TYPE_INTERFACE << 8) & 0xFF00)
#define USB_DESC_DEVICE_QUALIFIER                          ((USB_DESC_TYPE_DEVICE_QUALIFIER << 8) & 0xFF00)
#define USB_DESC_OTHER_SPEED_CONFIGURATION                 ((USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION << 8) & 0xFF00)
#define USB_DESC_INTERFACE_POWER                           ((USB_DESC_TYPE_INTERFACE_POWER << 8) & 0xFF00)
#define USB_DESC_HID_REPORT                                ((USB_DESC_TYPE_HID_REPORT << 8) & 0xFF00)
#define USB_DESC_HID                                       ((USB_DESC_TYPE_HID << 8) & 0xFF00)

#define SCSI_SENSE_KEY_NO_SENSE                         0x00
#define SCSI_SENSE_KEY_RECOVERED_ERROR                  0x01
#define SCSI_SENSE_KEY_NOT_READY                        0x02
#define SCSI_SENSE_KEY_MEDIUM_ERROR                     0x03
#define SCSI_SENSE_KEY_HARDWARE_ERROR                   0x04
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST                  0x05
#define SCSI_SENSE_KEY_UNIT_ATTENTION                   0x06
#define SCSI_SENSE_KEY_DATA_PROTECT                     0x07
#define SCSI_SENSE_KEY_BLANK_CHECK                      0x08
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC                  0x09
#define SCSI_SENSE_KEY_COPY_ABORTED                     0x0A
#define SCSI_SENSE_KEY_ABORTED_COMMAND                  0x0B
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW                  0x0D
#define SCSI_SENSE_KEY_MISCOMPARE                       0x0E

#define USBH_DEVICE_ADDRESS_DEFAULT                     0
#define USBH_MAX_ERROR_COUNT                            2
#define USBH_DEVICE_ADDRESS                             1

#ifndef USBH_MAX_PIPES_NBR
 #define USBH_MAX_PIPES_NBR                             15
#endif // USBH_MAX_PIPES_NBR


#define LE16(addr)                                         (((uint16_t)(*((uint8_t *)(addr)))) + (((uint16_t)(*(((uint8_t *)(addr)) + 1))) << 8))
#define LE16S(addr)                                      (uint16_t)(LE16((addr)))


#ifndef MAX_SUPPORTED_LUN
    #define MAX_SUPPORTED_LUN       2
#endif

// MSC Class Config
#define MSC_MEDIA_PACKET                                8192

#define SENSE_LIST_DEEPTH                               4

#define OPCODE_TEST_UNIT_READY                            0x00
#define OPCODE_READ_CAPACITY10                            0x25
#define OPCODE_READ10                                     0x28
#define OPCODE_WRITE10                                    0x2A
#define OPCODE_REQUEST_SENSE                              0x03
#define OPCODE_INQUIRY                                    0x12

#define BOT_DIR_IN                                       0
#define BOT_DIR_OUT                                      1
#define BOT_DIR_BOTH                                     2

#define BOT_CBW_SIGNATURE                               0x43425355
#define BOT_CBW_TAG                                     0x20304050
#define BOT_CSW_SIGNATURE                               0x53425355
#define BOT_CBW_LENGTH                                  31
#define BOT_CSW_LENGTH                                  13
#define BOT_CBW_CB_LENGTH                               16

#define DATA_LEN_MODE_TEST_UNIT_READY                    0
#define DATA_LEN_READ_CAPACITY10                         8
#define DATA_LEN_INQUIRY                                36
#define DATA_LEN_REQUEST_SENSE                          14


#define CBW_CB_LENGTH                                   16
#define CBW_LENGTH                                      10

#define FEATURE_SELECTOR_ENDPOINT                         0X00
#define FEATURE_SELECTOR_DEVICE                           0X01

#define INTERFACE_DESC_TYPE                               0x04
#define ENDPOINT_DESC_TYPE                                0x05
#define INTERFACE_DESC_SIZE                               0x09

#define USB_AC_CLASS                                    0x01            // in ST AC_CLASS
#define USB_CDC_CLASS                                   0x02
#define USB_HID_CLASS                                   0x03
#define USB_MTP_CLASS                                   0x06     // Still Image Class
#define USB_MSC_CLASS                                   0x08

#define USB_NOT_VALID_INTERFACE                            0xFF

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

enum USBH_State_e
{
    USBH_PRESENT,
    USBH_NOT_PRESENT,
};

enum USBH_Speed_e
{
    USBH_SPEED_HIGH  = 0,
    USBH_SPEED_FULL  = 1,
    USBH_SPEED_LOW   = 2,
};

// Following states are used for State
enum HOST_State_e
{
    HOST_IDLE = 0,
    HOST_DEV_WAIT_FOR_ATTACHMENT,
    HOST_DEV_ATTACHED,
    HOST_DEV_DISCONNECTED,
    HOST_DETECT_DEVICE_SPEED,
    HOST_ENUMERATION,
    HOST_CLASS_REQUEST,
    HOST_INPUT,
    HOST_SET_CONFIGURATION,
    HOST_CHECK_CLASS,
    HOST_CLASS,
    HOST_SUSPENDED,
    HOST_ABORT_STATE,
};

// Following states are used for CtrlXferStateMachine
enum CTRL_State_e
{
    CTRL_IDLE =0,
    CTRL_SETUP,
    CTRL_SETUP_WAIT,
    CTRL_DATA_IN,
    CTRL_DATA_IN_WAIT,
    CTRL_DATA_OUT,
    CTRL_DATA_OUT_WAIT,
    CTRL_STATUS_IN,
    CTRL_STATUS_IN_WAIT,
    CTRL_STATUS_OUT,
    CTRL_STATUS_OUT_WAIT,
    CTRL_ERROR,
    CTRL_STALLED,
    CTRL_COMPLETE
};

// Following states are used for EnumerationState
enum ENUM_State_e
{
    ENUM_IDLE = 0,
    ENUM_GET_FULL_DEV_DESC,
    ENUM_SET_ADDR,
    ENUM_GET_CFG_DESC,
    ENUM_GET_FULL_CFG_DESC,
    ENUM_GET_MFC_STRING_DESC,
    ENUM_GET_PRODUCT_STRING_DESC,
    ENUM_GET_SERIALNUM_STRING_DESC,
};

// Following states are used for RequestState
enum CMD_State_e
{
    CMD_IDLE =0,
    CMD_SEND,
    CMD_WAIT
};

enum USBH_URB_State_e
{
    USBH_URB_IDLE = 0,
    USBH_URB_DONE,
    USBH_URB_NOTREADY,
    USBH_URB_NYET,
    USBH_URB_ERROR,
    USBH_URB_STALL
};

enum MSC_State_e
{
    MSC_INIT = 0,
    MSC_IDLE,
    MSC_TEST_UNIT_READY,
    MSC_READ_CAPACITY10,
    MSC_READ_INQUIRY,
    MSC_REQUEST_SENSE,
    MSC_READ,
    MSC_WRITE,
    MSC_UNRECOVERED_ERROR,
    MSC_PERIODIC_CHECK,
};

enum MSC_Error_e
{
    MSC_OK,
    MSC_NOT_READY,
    MSC_ERROR,
};

enum MSC_RequestState_e
{
    MSC_REQ_IDLE = 0,
    MSC_REQ_RESET,
    MSC_REQ_GET_MAX_LUN,
    MSC_REQ_ERROR,
};

enum BOT_CMD_State_e
{
    BOT_CMD_IDLE  = 0,
    BOT_CMD_SEND,
    BOT_CMD_WAIT,
};

// CSW Status Definitions
enum BOT_CSW_Status_e
{

    BOT_CSW_CMD_PASSED   =        0x00,
    BOT_CSW_CMD_FAILED   =        0x01,
    BOT_CSW_PHASE_ERROR  =        0x02,
};

enum BOT_State_e
{
  BOT_SEND_CBW  = 1,
  BOT_SEND_CBW_WAIT,
  BOT_DATA_IN,
  BOT_DATA_IN_WAIT,
  BOT_DATA_OUT,
  BOT_DATA_OUT_WAIT,
  BOT_RECEIVE_CSW,
  BOT_RECEIVE_CSW_WAIT,
  BOT_ERROR_IN,
  BOT_ERROR_OUT,
  BOT_UNRECOVERED_ERROR
};

struct USBH_DescHeader_t
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
};

struct USBH_EndPointDescriptor_t
{
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint8_t   bEndpointAddress;       // indicates what endpoint this descriptor is describing
    uint8_t   bmAttributes;           // specifies the transfer type.
    uint16_t  wMaxPacketSize;        // Maximum Packet Size this endpoint is capable of sending or receiving
    uint8_t   bInterval;              // is used to specify the polling interval of certain transfers.
};

struct USBH_InterfaceDescriptor_t
{
    uint8_t                     bLength;
    uint8_t                     bDescriptorType;
    uint8_t                     bInterfaceNumber;
    uint8_t                     bAlternateSetting;                    // Value used to select alternative setting
    uint8_t                     bNumEndpoints;                        // Number of Endpoints used for this interface
    uint8_t                     bInterfaceClass;                      // Class Code (Assigned by USB Org)
    uint8_t                     bInterfaceSubClass;                   // Subclass Code (Assigned by USB Org)
    uint8_t                     bInterfaceProtocol;                   // Protocol Code
    uint8_t                     iInterface;                           // Index of String Descriptor Describing this interface
    USBH_EndPointDescriptor_t   Ep_Desc[USBH_MAX_NUM_ENDPOINTS];
};

struct USBH_ConfigDescriptor_t
{
    uint8_t                       bLength;
    uint8_t                       bDescriptorType;
    uint16_t                      wTotalLength;                        // Total Length of Data Returned
    uint8_t                       bNumInterfaces;                       // Number of Interfaces
    uint8_t                       bConfigurationValue;                  // Value to use as an argument to select this configuration
    uint8_t                       iConfiguration;                       // Index of String Descriptor Describing this configuration
    uint8_t                       bmAttributes;                         // D7 Bus Powered , D6 Self Powered, D5 Remote Wakeup , D4..0 Reserved (0)
    uint8_t                       bMaxPower;                            // Maximum Power Consumption
    USBH_InterfaceDescriptor_t  Itf_Desc[USBH_MAX_NUM_INTERFACES];
};

struct USBH_DeviceDescriptor_t
{
  uint8_t   bLength;
  uint8_t   bDescriptorType;
  uint16_t  bcdUSB;                    /* USB Specification Number which device complies too */
  uint8_t   bDeviceClass;
  uint8_t   bDeviceSubClass;
  uint8_t   bDeviceProtocol;
  /* If equal to Zero, each interface specifies its own class
  code if equal to 0xFF, the class code is vendor specified.
  Otherwise field is valid Class Code.*/
  uint8_t   bMaxPacketSize;
  uint16_t  idVendor;      /* Vendor ID (Assigned by USB Org) */
  uint16_t  idProduct;     /* Product ID (Assigned by Manufacturer) */
  uint16_t  bcdDevice;     /* Device Release Number */
  uint8_t   iManufacturer;  /* Index of Manufacturer String Descriptor */
  uint8_t   iProduct;       /* Index of Product String Descriptor */
  uint8_t   iSerialNumber;  /* Index of Serial Number String Descriptor */
  uint8_t   bNumConfigurations; /* Number of Possible Configurations */
};


// Attached device structure
struct USBH_Device_t
{
  #if (USBH_KEEP_CFG_DESCRIPTOR == 1)
    uint8_t                         CfgDesc_Raw[USBH_MAX_SIZE_CONFIGURATION];
  #endif
    uint8_t                         Data[USBH_MAX_DATA_BUFFER];
    uint8_t                         address;
    uint8_t                         speed;
    volatile uint8_t                is_connected;
    uint8_t                         current_interface;
    USBH_DeviceDescriptor_t         DevDesc;
    USBH_ConfigDescriptor_t         CfgDesc;
};

struct HCD_Handle_t
{
    HCD_TypeDef*                       Instance;  /*!< Register base address    */
    HCD_InitTypeDef                   Init;       /*!< HCD required parameters  */
    HCD_HCTypeDef                     hc[15];     /*!< Host channels parameters */
    HAL_LockTypeDef                   Lock;       /*!< HCD peripheral status    */
    volatile HCD_StateTypeDef         State;      /*!< HCD communication state  */
    void*                              pData;     /*!< Pointer Stack Handler    */
};


union uint16_t_uint8_t
{
    uint16_t w;
    struct BW
    {
        uint8_t msb;
        uint8_t lsb;
    }
    bw;
};

union USB_Setup_t
{
    uint32_t d8[2];

    struct _SetupPkt_Struc
    {
        uint8_t           bmRequestType;
        uint8_t           bRequest;
        uint16_t_uint8_t  wValue;
        uint16_t_uint8_t  wIndex;
        uint16_t_uint8_t  wLength;
    } b;
};

// Control request structure
struct USBH_Control_t
{
    uint8_t               PipeIn;
    uint8_t               PipeOut;
    uint8_t               PipeSize;
    uint8_t*              pBuffer;
    uint16_t              Length;
    uint16_t              Timer;
    USB_Setup_t           Setup;
    CTRL_State_e          State;
    uint8_t               ErrorCount;

};

// Capacity data.
struct SCSI_Capacity_t
{
  uint32_t block_nbr;
  uint16_t block_size;
};


// Sense data.
struct SCSI_Sense_t
{
  uint8_t key;
  uint8_t asc;
  uint8_t ascq;
};

// INQUIRY data.
struct SCSI_StdInquiryData_t
{
  uint8_t PeripheralQualifier;
  uint8_t DeviceType;
  uint8_t RemovableMedia;
  uint8_t vendor_id[9];
  uint8_t product_id[17];
  uint8_t revision_id[5];
};

// Structure for LUN
struct MSC_LUN_t
{
    MSC_State_e            state;
    MSC_Error_e            error;
    SystemState_e          prev_ready_state;
    SCSI_Capacity_t        capacity;
    SCSI_Sense_t           sense;
    SCSI_StdInquiryData_t  inquiry;
    uint8_t                state_changed;
};

union BOT_CBW_t
{
  struct __CBW
  {
    uint32_t Signature;
    uint32_t Tag;
    uint32_t DataTransferLength;
    uint8_t  Flags;
    uint8_t  LUN;
    uint8_t  CBLength;
    uint8_t  CB[16];
  }field;
  uint8_t data[31];
};

union BOT_CSW_t
{
  struct __CSW
  {
    uint32_t Signature;
    uint32_t Tag;
    uint32_t DataResidue;
    uint8_t  Status;
  }field;
  uint8_t data[13];
};

struct BOT_Handle_t
{
    uint32_t            data[16];
    BOT_State_e         state;
    BOT_State_e            prev_state;
    BOT_CMD_State_e     cmd_state;
    BOT_CBW_t           cbw;
    uint8_t             Reserved1;
    BOT_CSW_t           csw;
    uint8_t             Reserved2[3];
    uint8_t*             pbuf;
};

// USB Host handle structure
struct USBH_Handle_t
{
    volatile HOST_State_e           State;           //  Host State Machine Value
    ENUM_State_e                     EnumState;    // Enumeration state Machine
    CMD_State_e                     RequestState;
    USBH_Control_t                    Control;
    USBH_Device_t                   Device;
    class USB_HostClassInterface*   pClass[USBH_MAX_NUM_SUPPORTED_CLASS];
    class USB_HostClassInterface*   pActiveClass;
    uint32_t                          ClassNumber;
    uint32_t                          Pipes[15];
    volatile uint32_t                 Timer;
    uint8_t                           ID;
    void*                             pData;
    void                             (*pUser)(uint8_t id);
    nOS_Flag                          HostEvent;
    class USB_Application*          pUSB_App;
    class USB_Host*                    pBase;
};

struct MSC_Handle_t
{
    uint32_t            max_lun;
    uint8_t             InPipe;
    uint8_t             OutPipe;
    uint8_t             OutEp;
    uint8_t             InEp;
    uint16_t            OutEpSize;
    uint16_t               InEpSize;
    MSC_State_e               state;
    MSC_Error_e         error;
    MSC_RequestState_e  req_state;
    MSC_RequestState_e  prev_req_state;
    BOT_Handle_t        hbot;
    MSC_LUN_t              unit[MAX_SUPPORTED_LUN];
    uint16_t            current_lun;
      uint16_t            rw_lun;
      uint32_t            timer;
};

//enum USBH_MSC_HostStatus_e
//{
//    USBH_MSC_HOST_NO_INIT = 0,  // USB interface not initialized
//    USBH_MSC_DEV_DETACHED,      // no device connected
//    USBH_MSC_SPEED_ERROR,       // unsupported USB speed
//    USBH_MSC_DEV_NOT_SUPPORTED, // unsupported device
//    USBH_MSC_DEV_WRITE_PROTECT, // device is write protected
//    USBH_MSC_OVER_CURRENT,      // over current detected
//    USBH_MSC_DEV_CONNECTED      // device connected and ready
//};

//-------------------------------------------------------------------------------------------------
