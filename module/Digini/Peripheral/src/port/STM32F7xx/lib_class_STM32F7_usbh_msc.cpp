#include "lib_class_STM32F7_usbh_msc.h"
#include "nOS.h"
//#include "usbh_msc.h"
//#include "usbh_msc_bot.h"
//#include "usbh_msc_scsi.h"

/**
  * @brief  USBH_MSC_InterfaceInit 
  *         The function init the MSC class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::InterfaceInitialize(USBH_Handle_t* pHost)
{     
    uint8_t         interface;
    SystemState_e     Status;
  
    m_pHost = pHost;
    Status  = SYS_FAIL;

    interface = m_pHost->pBase->FindInterface(m_pHost->pActiveClass->GetClassCode(), MSC_TRANSPARENT, MSC_BOT);

    if(interface == USB_NOT_VALID_INTERFACE)
    {
        //USBH_DbgLog ("Cannot Find the interface for %s class.", m_pHost->pActiveClass->Name);
        Status = SYS_FAIL;      
    }
    else
    {
        m_pHost->pBase->SelectInterface(interface);
    
        //m_pHost->pActiveClass->GetDataPointer() &m_MSC_Handle;

        if(m_pHost->Device.CfgDesc.Itf_Desc[m_pHost->Device.current_interface].Ep_Desc[0].bEndpointAddress & 0x80)
        {
            m_MSC_Handle.InEp      = m_pHost->Device.CfgDesc.Itf_Desc[m_pHost->Device.current_interface].Ep_Desc[0].bEndpointAddress;
            m_MSC_Handle.InEpSize  = m_pHost->Device.CfgDesc.Itf_Desc[m_pHost->Device.current_interface].Ep_Desc[0].wMaxPacketSize;
        }
        else
        {
            m_MSC_Handle.OutEp     = m_pHost->Device.CfgDesc.Itf_Desc[m_pHost->Device.current_interface].Ep_Desc[0].bEndpointAddress;
            m_MSC_Handle.OutEpSize = m_pHost->Device.CfgDesc.Itf_Desc[m_pHost->Device.current_interface].Ep_Desc[0].wMaxPacketSize;
        }
    
        if(m_pHost->Device.CfgDesc.Itf_Desc[m_pHost->Device.current_interface].Ep_Desc[1].bEndpointAddress & 0x80)
        {
            m_MSC_Handle.InEp     = m_pHost->Device.CfgDesc.Itf_Desc[m_pHost->Device.current_interface].Ep_Desc[1].bEndpointAddress;
            m_MSC_Handle.InEpSize = m_pHost->Device.CfgDesc.Itf_Desc[m_pHost->Device.current_interface].Ep_Desc[1].wMaxPacketSize;
        }
        else
        {
            m_MSC_Handle.OutEp     = m_pHost->Device.CfgDesc.Itf_Desc[m_pHost->Device.current_interface].Ep_Desc[1].bEndpointAddress;
            m_MSC_Handle.OutEpSize = m_pHost->Device.CfgDesc.Itf_Desc[m_pHost->Device.current_interface].Ep_Desc[1].wMaxPacketSize;
        }
    
        m_MSC_Handle.current_lun = 0;
        m_MSC_Handle.rw_lun      = 0;
        m_MSC_Handle.state       = MSC_INIT;
        m_MSC_Handle.error       = MSC_OK;
        m_MSC_Handle.req_state   = MSC_REQ_IDLE;
        m_MSC_Handle.OutPipe     = m_pHost->pBase->AllocPipe(m_MSC_Handle.OutEp);
        m_MSC_Handle.InPipe      = m_pHost->pBase->AllocPipe(m_MSC_Handle.InEp);

        BOT_Initialize();
    
        // De-Initialize LUNs information
        memset(m_MSC_Handle.unit, 0, sizeof(m_MSC_Handle.unit));
    
        // Open the new channels
        m_pHost->pUSB_App->OpenPipe(m_MSC_Handle.OutPipe, m_MSC_Handle.OutEp, m_pHost->Device.address, m_pHost->Device.speed, USB_EP_TYPE_BULK, m_MSC_Handle.OutEpSize);
        m_pHost->pUSB_App->OpenPipe(m_MSC_Handle.InPipe,  m_MSC_Handle.InEp,  m_pHost->Device.address, m_pHost->Device.speed, USB_EP_TYPE_BULK, m_MSC_Handle.InEpSize);
    
        m_pHost->pUSB_App->SetToggle(m_MSC_Handle.InPipe,0);
        m_pHost->pUSB_App->SetToggle(m_MSC_Handle.OutPipe,0);
        Status = SYS_READY; 
    }

    return Status;
}


/**
  * @brief  The function DeInit the Pipes used for the MSC class.
  * @retval SystemState_e
  */
SystemState_e USB_HostMassStorage::InterfaceDeInitialize(void)
{
    if(m_MSC_Handle.OutPipe)
    {
        m_pHost->pUSB_App->ClosePipe(m_MSC_Handle.OutPipe);
        m_pHost->pBase->FreePipe (m_MSC_Handle.OutPipe);
        m_MSC_Handle.OutPipe = 0;     // Reset the channel as free
    }
  
    if(m_MSC_Handle.InPipe)
    {
        m_pHost->pUSB_App->ClosePipe(m_MSC_Handle.InPipe);
        m_pHost->pBase->FreePipe (m_MSC_Handle.InPipe);
        m_MSC_Handle.InPipe = 0;     // Reset the channel as free
    }

    return SYS_READY;
}

/**
  * @brief  USBH_MSC_ClassRequest 
  *         The function is responsible for handling Standard requests
  *         for MSC class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::ClassRequest(void)
{   
    SystemState_e Status = SYS_BUSY;
    uint8_t i;
  
    // Switch MSC REQ state machine
    switch(m_MSC_Handle.req_state)
    {
        case MSC_REQ_IDLE:
        case MSC_REQ_GET_MAX_LUN:
        {
            // Issue GetMaxLUN request
            BOT_REQ_GetMaxLUN((uint8_t *)&m_MSC_Handle.max_lun);
    
            // When devices do not support the GetMaxLun request, this should be considered as only one logical unit is supported
            if(Status == SYS_NOT_SUPPORTED)
            {
                m_MSC_Handle.max_lun = 0;
                Status = SYS_READY;
            }
    
            if(Status == SYS_READY)
            {
                m_MSC_Handle.max_lun = (uint8_t )(m_MSC_Handle.max_lun) + 1;
                //USBH_UsrLog ("Number of supported LUN: %lu", (int32_t)(m_MSC_Handle.max_lun));
      
                for(i = 0; i < m_MSC_Handle.max_lun; i++)
                {
                    m_MSC_Handle.unit[i].prev_ready_state = SYS_FAIL;
                    m_MSC_Handle.unit[i].state_changed = 0;
                }
            }
        }
        break;
    
        case MSC_REQ_ERROR:
        {
            // a Clear Feature should be issued here
            if(m_pHost->pBase->ClrFeature(0x00) == SYS_READY)
            {
                m_MSC_Handle.req_state = m_MSC_Handle.prev_req_state;
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
  * @brief  USBH_MSC_Process 
  *         The function is for managing state machine for MSC data transfers 
  * @param  phost: Host handle
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::Process(void)
{
    SystemState_e error        = SYS_BUSY;
    SystemState_e scsi_status  = SYS_BUSY;
    SystemState_e ready_status = SYS_BUSY;
  
    switch(m_MSC_Handle.state)
    {
        case MSC_INIT:
        {
            if(m_MSC_Handle.current_lun < m_MSC_Handle.max_lun)
            {
                m_MSC_Handle.unit[m_MSC_Handle.current_lun].error = MSC_NOT_READY;

                // Switch MSC REQ state machine
                switch(m_MSC_Handle.unit[m_MSC_Handle.current_lun].state)
                {
                    case MSC_INIT:
                    {
                        //USBH_UsrLog ("LUN #%d: ", MSC_Handle->current_lun);
                        m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_READ_INQUIRY;
                        m_MSC_Handle.timer = m_pHost->Timer;
                    }
        
                    case MSC_READ_INQUIRY:
                    {
                        scsi_status = SCSI_Inquiry(m_MSC_Handle.current_lun, &m_MSC_Handle.unit[m_MSC_Handle.current_lun].inquiry);
        
                        if(scsi_status == SYS_READY)
                        {
                            //USBH_UsrLog ("Inquiry Vendor  : %s", MSC_Handle->unit[MSC_Handle->current_lun].inquiry.vendor_id);
                            //USBH_UsrLog ("Inquiry Product : %s", MSC_Handle->unit[MSC_Handle->current_lun].inquiry.product_id);
                            //USBH_UsrLog ("Inquiry Version : %s", MSC_Handle->unit[MSC_Handle->current_lun].inquiry.revision_id);
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_TEST_UNIT_READY;
                        }

                        if(scsi_status == SYS_FAIL)
                        {
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_REQUEST_SENSE;
                        }
                        else if(scsi_status == SYS_UNRECOVERED_ERROR)
                        {
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_IDLE;
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].error = MSC_ERROR;
                        }
                    }
                    break;
        
                    case MSC_TEST_UNIT_READY:
                    {
                        ready_status = SCSI_TestUnitReady(m_MSC_Handle.current_lun);
        
                        if(ready_status == SYS_READY)
                        {
                            if(m_MSC_Handle.unit[m_MSC_Handle.current_lun].prev_ready_state != SYS_READY)
                            {
                                m_MSC_Handle.unit[m_MSC_Handle.current_lun].state_changed = 1;
                                //USBH_UsrLog ("MSC Device ready");
                            }
                            else
                            {
                                m_MSC_Handle.unit[m_MSC_Handle.current_lun].state_changed = 0;
                            }

                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_READ_CAPACITY10;
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].error = MSC_OK;
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].prev_ready_state = SYS_READY;
                        }

                        if(ready_status == SYS_FAIL)
                        {
                            // Media not ready, so try to check again during 10s
                            if(m_MSC_Handle.unit[m_MSC_Handle.current_lun].prev_ready_state != SYS_FAIL)
                            {
                                m_MSC_Handle.unit[m_MSC_Handle.current_lun].state_changed = 1;
                                //USBH_UsrLog ("MSC Device NOT ready");
                            }
                            else
                            {
                                m_MSC_Handle.unit[m_MSC_Handle.current_lun].state_changed = 0;
                            }

                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_REQUEST_SENSE;
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].error = MSC_NOT_READY;
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].prev_ready_state = SYS_FAIL;
                        }
                        else if(ready_status == SYS_UNRECOVERED_ERROR)
                        {
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_IDLE;
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].error = MSC_ERROR;
                        }
                    }
                    break;
        
                    case MSC_READ_CAPACITY10:
                    {
                        scsi_status = SCSI_ReadCapacity(m_MSC_Handle.current_lun, &m_MSC_Handle.unit[m_MSC_Handle.current_lun].capacity);
        
                        if(scsi_status == SYS_READY)
                        {
                            if(m_MSC_Handle.unit[m_MSC_Handle.current_lun].state_changed == 1)
                            {
                                //USBH_UsrLog ("MSC Device capacity : %lu Bytes", (int32_t)(m_MSC_Handle.unit[MSC_Handle->current_lun].capacity.block_nbr * m_MSC_Handle.unit[m_MSC_Handle.current_lun].capacity.block_size));
                                //USBH_UsrLog ("Block number : %lu", (int32_t)(MSC_Handle->unit[m_MSC_Handle.current_lun].capacity.block_nbr));
                                //USBH_UsrLog ("Block Size   : %lu", (int32_t)(MSC_Handle->unit[m_MSC_Handle.current_lun].capacity.block_size));
                            }

                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_IDLE;
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].error = MSC_OK;
                            m_MSC_Handle.current_lun++;
                        }
                        else if( scsi_status == SYS_FAIL)
                        {
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_REQUEST_SENSE;
                        }
                        else if(scsi_status == SYS_UNRECOVERED_ERROR)
                        {
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_IDLE;
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].error = MSC_ERROR;
                        }
                    }
                    break;
        
                    case MSC_REQUEST_SENSE:
                    {
                        scsi_status = SCSI_RequestSense(m_MSC_Handle.current_lun, &m_MSC_Handle.unit[m_MSC_Handle.current_lun].sense);
        
                        if(scsi_status == SYS_READY)
                        {
                            if((m_MSC_Handle.unit[m_MSC_Handle.current_lun].sense.key == SCSI_SENSE_KEY_UNIT_ATTENTION) ||
                               (m_MSC_Handle.unit[m_MSC_Handle.current_lun].sense.key == SCSI_SENSE_KEY_NOT_READY) )
                            {
                                if((m_pHost->Timer - m_MSC_Handle.timer) < 10000)
                                {
                                    m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_TEST_UNIT_READY;
                                    break;
                                }
                            }
          
                            //USBH_UsrLog ("Sense Key  : %x", m_MSC_Handle.unit[m_MSC_Handle.current_lun].sense.key);
                            //USBH_UsrLog ("Additional Sense Code : %x", m_MSC_Handle.unit[m_MSC_Handle.current_lun].sense.asc);
                            //USBH_UsrLog ("Additional Sense Code Qualifier: %x", m_MSC_Handle.unit[m_MSC_Handle.current_lun].sense.ascq);
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_IDLE;
                            m_MSC_Handle.current_lun++;
                        }

                        if(scsi_status == SYS_FAIL)
                        {
                            //USBH_UsrLog ("MSC Device NOT ready");
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_UNRECOVERED_ERROR;
                        }
                        else if(scsi_status == SYS_UNRECOVERED_ERROR)
                        {
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].state = MSC_IDLE;
                            m_MSC_Handle.unit[m_MSC_Handle.current_lun].error = MSC_ERROR;
                        }
                    }
                    break;
    
                    case MSC_UNRECOVERED_ERROR:
                    {
                        m_MSC_Handle.current_lun++;
                    }
                    break;
        
                    default:
                    {
                    }
                    break;
                }
      
                nOS_FlagSend(&m_pHost->HostEvent, USBH_CLASS_EVENT, USBH_CLASS_EVENT);
            }
            else
            {
                m_MSC_Handle.current_lun = 0;
                m_MSC_Handle.state = MSC_IDLE;
                nOS_FlagSend(&m_pHost->HostEvent, USBH_CLASS_EVENT, USBH_CLASS_EVENT);
                m_pHost->pUser(HOST_USER_CLASS_ACTIVE);
            }
        }
        break;

        case MSC_IDLE:
        {
            error = SYS_READY;
        }
        break;
    
        default:
        {
        }
        break;
    }

    return error;
}


/**
  * @brief  USBH_MSC_SOFProcess 
  *         The function is for SOF state
  * @param  phost: Host handle
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::SOFProcess(void)
{
    return SYS_READY;
}
/**
  * @brief  USBH_MSC_RdWrProcess 
  *         The function is for managing state machine for MSC I/O Process
  * @param  phost: Host handle
  * @param  lun: logical Unit Number
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::RdWrProcess(uint8_t lun)
{
    SystemState_e error       = SYS_BUSY;
    SystemState_e scsi_status = SYS_BUSY;
  
    // Switch MSC REQ state machine
    switch(m_MSC_Handle.unit[lun].state)
    {
        case MSC_READ:
        {
            scsi_status = SCSI_Read(lun, 0, NULL, 0);
    
            if(scsi_status == SYS_READY)
            {
                m_MSC_Handle.unit[lun].state = MSC_IDLE;
                error = SYS_READY;
            }
            else if(scsi_status == SYS_FAIL)
            {
                m_MSC_Handle.unit[lun].state = MSC_REQUEST_SENSE;
            }
            else if(scsi_status == SYS_UNRECOVERED_ERROR)
            {
                m_MSC_Handle.unit[lun].state = MSC_UNRECOVERED_ERROR;
                error = SYS_FAIL;
            }

            nOS_FlagSend(&m_pHost->HostEvent, USBH_CLASS_EVENT, USBH_CLASS_EVENT);
        }
        break;
    
        case MSC_WRITE:
        {
            scsi_status = SCSI_Write(lun, 0, NULL, 0);
    
            if(scsi_status == SYS_READY)
            {
                m_MSC_Handle.unit[lun].state = MSC_IDLE;
                error = SYS_READY;
            }
            else if(scsi_status == SYS_FAIL)
            {
                m_MSC_Handle.unit[lun].state = MSC_REQUEST_SENSE;
            }
            else if(scsi_status == SYS_UNRECOVERED_ERROR)
            {
                m_MSC_Handle.unit[lun].state = MSC_UNRECOVERED_ERROR;
                error = SYS_FAIL;
            }

            nOS_FlagSend(&m_pHost->HostEvent, USBH_CLASS_EVENT, USBH_CLASS_EVENT);
        }
        break;
  
        case MSC_REQUEST_SENSE:
        {
            scsi_status = SCSI_RequestSense(lun, &m_MSC_Handle.unit[lun].sense);
    
            if(scsi_status == SYS_READY)
            {
                //USBH_UsrLog ("Sense Key  : %x", m_MSC_Handle.unit[lun].sense.key);
                //USBH_UsrLog ("Additional Sense Code : %x", m_MSC_Handle.unit[lun].sense.asc);
                //USBH_UsrLog ("Additional Sense Code Qualifier: %x", m_MSC_Handle.unit[lun].sense.ascq);
                m_MSC_Handle.unit[lun].state = MSC_IDLE;
                m_MSC_Handle.unit[lun].error = MSC_ERROR;
                  error = SYS_FAIL;
            }
            if(scsi_status == SYS_FAIL)
            {
                //USBH_UsrLog ("MSC Device NOT ready");
            }
            else if(scsi_status == SYS_UNRECOVERED_ERROR)
            {
                m_MSC_Handle.unit[lun].state = MSC_UNRECOVERED_ERROR;
                error = SYS_FAIL;
            }

            nOS_FlagSend(&m_pHost->HostEvent, USBH_CLASS_EVENT, USBH_CLASS_EVENT);
        }
        break;
    
        default:
        {
        }
        break;
    }
    return error;
}

/**
  * @brief  USBH_MSC_IsReady 
  *         The function check if the MSC function is ready
  * @param  phost: Host handle
  * @retval USBH Status
  */
bool USB_HostMassStorage::IsReady(void)
{
    if(    m_pHost->State == HOST_CLASS)
    {
        return (m_MSC_Handle.state == MSC_IDLE) ? true :false;
    }
    else
    {
        return false;
    }
}

/**
  * @brief  USBH_MSC_GetMaxLUN 
  *         The function return the Max LUN supported
  * @param  phost: Host handle
  * @retval logical Unit Number supported
  */
int8_t USB_HostMassStorage::GetMaxLUN(void)
{
    if ((m_pHost->State == HOST_CLASS) && (m_MSC_Handle.state == MSC_IDLE))
    {
        return m_MSC_Handle.max_lun;
    }
    return 0xFF;
}

/**
  * @brief  USBH_MSC_UnitIsReady 
  *         The function check whether a LUN is ready
  * @param  phost: Host handle
  * @param  lun: logical Unit Number
  * @retval Lun Status (0: not ready / 1: ready)
  */
bool USB_HostMassStorage::UnitIsReady(uint8_t lun)
{
    if(m_pHost->State == HOST_CLASS)
    {
        return(m_MSC_Handle.unit[lun].error == MSC_OK) ? true : false;
    }
    else
    {
        return false;
    }
}
      
/**
  * @brief  USBH_MSC_GetLUNInfo 
  *         The function return a LUN information
  * @param  phost: Host handle
  * @param  lun: logical Unit Number
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::GetLUN_Info(uint8_t lun, MSC_LUN_t* info)
{
    if(m_pHost->State == HOST_CLASS)
    {
        memcpy(info, &m_MSC_Handle.unit[lun], sizeof(MSC_LUN_t));
        return SYS_READY;
    }
    else
    {
        return SYS_FAIL;
    }
}

/**
  * @brief  USBH_MSC_Read 
  *         The function performs a Read operation 
  * @param  phost: Host handle
  * @param  lun: logical Unit Number
  * @param  address: sector address
  * @param  pbuf: pointer to data
  * @param  length: number of sector to read
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::Read(uint8_t lun, uint32_t address, uint8_t *pbuf, uint32_t length)
{
    uint32_t timeout;

    if((m_pHost->Device.is_connected == 0) || (m_pHost->State != HOST_CLASS) || (m_MSC_Handle.unit[lun].state != MSC_IDLE))
    {
        return SYS_FAIL;
    }
  
    m_MSC_Handle.state = MSC_READ;
    m_MSC_Handle.unit[lun].state = MSC_READ;
    m_MSC_Handle.rw_lun = lun;
    SCSI_Read(lun, address, pbuf, length);
  
    timeout = m_pHost->Timer;
  
    while(RdWrProcess(lun) == SYS_BUSY)
    {
        if(((m_pHost->Timer - timeout) > (10000 * length)) || (m_pHost->Device.is_connected == 0))
        {
            m_MSC_Handle.state = MSC_IDLE;
            return SYS_FAIL;
        }
    }

    m_MSC_Handle.state = MSC_IDLE;
    return SYS_READY;
}

/**
  * @brief  USBH_MSC_Write 
  *         The function performs a Write operation 
  * @param  phost: Host handle
  * @param  lun: logical Unit Number
  * @param  address: sector address
  * @param  pbuf: pointer to data
  * @param  length: number of sector to write
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::Write(uint8_t lun, uint32_t address, uint8_t* pbuf, uint32_t length)
{
    uint32_t timeout;

      if((m_pHost->Device.is_connected == 0) || (m_pHost->State != HOST_CLASS) || (m_MSC_Handle.unit[lun].state != MSC_IDLE))
      {
          return SYS_FAIL;
      }
  
      m_MSC_Handle.state = MSC_WRITE;
      m_MSC_Handle.unit[lun].state = MSC_WRITE;
      m_MSC_Handle.rw_lun = lun;
      SCSI_Write(lun, address, pbuf, length);
      timeout = m_pHost->Timer;

      while(RdWrProcess(lun) == SYS_BUSY)
      {
          if(((m_pHost->Timer - timeout) > (10000 * length)) || (m_pHost->Device.is_connected == 0))
          {
              m_MSC_Handle.state = MSC_IDLE;
              return SYS_FAIL;
          }
      }

      m_MSC_Handle.state = MSC_IDLE;
      return SYS_READY;
}



/**
  * @brief  USBH_MSC_BOT_REQ_GetMaxLUN
  *         The function the MSC BOT GetMaxLUN request.
  * @param  phost: Host handle
  * @param  Maxlun: pointer to Maxlun variable
  * @retval USBH Status
  */
void USB_HostMassStorage::BOT_REQ_GetMaxLUN(uint8_t *Maxlun)
{
    m_pHost->Control.Setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;
    m_pHost->Control.Setup.b.bRequest = USB_REQ_GET_MAX_LUN;
    m_pHost->Control.Setup.b.wValue.w = 0;
    m_pHost->Control.Setup.b.wIndex.w = 0;
    m_pHost->Control.Setup.b.wLength.w = 1;
    m_pHost->pBase->ControlRequest(Maxlun , 1);
}

/**
  * @brief  USBH_MSC_SCSI_Write
  *         Issue write10 command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  address: sector address
  * @param  pbuf: pointer to data
  * @param  length: number of sector to write
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::SCSI_Write(uint8_t lun, uint32_t address, uint8_t *pbuf, uint32_t length)
{
    SystemState_e error = SYS_FAIL;
    //MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;   // ??? I will have to check every function because there is 2 possiblities

    switch(m_MSC_Handle.hbot.cmd_state)
    {
        case BOT_CMD_SEND:
        {
            // Prepare the CBW and relevent field
            m_MSC_Handle.hbot.cbw.field.DataTransferLength = length * 512;
            m_MSC_Handle.hbot.cbw.field.Flags    = USB_EP_DIR_OUT;
            m_MSC_Handle.hbot.cbw.field.CBLength = CBW_LENGTH;

            memset(m_MSC_Handle.hbot.cbw.field.CB, 0, CBW_CB_LENGTH);
            m_MSC_Handle.hbot.cbw.field.CB[0]  = OPCODE_WRITE10;

            // logical block address
            m_MSC_Handle.hbot.cbw.field.CB[2] = (((uint8_t*)&address)[3]);
            m_MSC_Handle.hbot.cbw.field.CB[3] = (((uint8_t*)&address)[2]);
            m_MSC_Handle.hbot.cbw.field.CB[4] = (((uint8_t*)&address)[1]);
            m_MSC_Handle.hbot.cbw.field.CB[5] = (((uint8_t*)&address)[0]);

            // Transfer length
            m_MSC_Handle.hbot.cbw.field.CB[7] = (((uint8_t *)&length)[1]) ;
            m_MSC_Handle.hbot.cbw.field.CB[8] = (((uint8_t *)&length)[0]) ;

            m_MSC_Handle.hbot.state     = BOT_SEND_CBW;
            m_MSC_Handle.hbot.cmd_state = BOT_CMD_WAIT;
            m_MSC_Handle.hbot.pbuf      = pbuf;
            error = SYS_BUSY;
        }
        break;

        case BOT_CMD_WAIT:
        {
            error = BOT_Process(lun);
        }
        break;

        default:
        {
        }
        break;
    }

    return error;
}

/**
  * @brief  USBH_MSC_SCSI_Read
  *         Issue Read10 command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  address: sector address
  * @param  pbuf: pointer to data
  * @param  length: number of sector to read
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::SCSI_Read(uint8_t lun, uint32_t address, uint8_t *pbuf, uint32_t length)
{
    SystemState_e Error = SYS_FAIL;

    switch(m_MSC_Handle.hbot.cmd_state)
    {
        case BOT_CMD_SEND:
        {
            // Prepare the CBW and relevent field
            m_MSC_Handle.hbot.cbw.field.DataTransferLength = length * 512;
            m_MSC_Handle.hbot.cbw.field.Flags    = USB_EP_DIR_IN;
            m_MSC_Handle.hbot.cbw.field.CBLength = CBW_LENGTH;

            memset(m_MSC_Handle.hbot.cbw.field.CB, 0, CBW_CB_LENGTH);
            m_MSC_Handle.hbot.cbw.field.CB[0]  = OPCODE_READ10;

            // logical block address
            m_MSC_Handle.hbot.cbw.field.CB[2]  = (((uint8_t*)&address)[3]);
            m_MSC_Handle.hbot.cbw.field.CB[3]  = (((uint8_t*)&address)[2]);
            m_MSC_Handle.hbot.cbw.field.CB[4]  = (((uint8_t*)&address)[1]);
            m_MSC_Handle.hbot.cbw.field.CB[5]  = (((uint8_t*)&address)[0]);

            // Transfer length
            m_MSC_Handle.hbot.cbw.field.CB[7]  = (((uint8_t *)&length)[1]) ;
            m_MSC_Handle.hbot.cbw.field.CB[8]  = (((uint8_t *)&length)[0]) ;

            m_MSC_Handle.hbot.state     = BOT_SEND_CBW;
            m_MSC_Handle.hbot.cmd_state = BOT_CMD_WAIT;
            m_MSC_Handle.hbot.pbuf      = pbuf;
            Error = SYS_BUSY;
        }
        break;

        case BOT_CMD_WAIT:
        {
            Error = BOT_Process(lun);
        }
        break;

        default:
        {
        }
        break;
    }

    return Error;
}

/**
  * @brief  USBH_MSC_BOT_Init
  *         The function Initializes the BOT protocol.
  * @param  phost: Host handle
  * @retval USBH Status
  */
void USB_HostMassStorage::BOT_Initialize(void)
{

  //MSC_HandleTypeDef *MSC_Handle =  (MSC_HandleTypeDef *) phost->pActiveClass->pData;

  m_MSC_Handle.hbot.cbw.field.Signature = BOT_CBW_SIGNATURE;
  m_MSC_Handle.hbot.cbw.field.Tag = BOT_CBW_TAG;
  m_MSC_Handle.hbot.state = BOT_SEND_CBW;
  m_MSC_Handle.hbot.cmd_state = BOT_CMD_SEND;
}

/**
  * @brief  USBH_MSC_SCSI_Inquiry
  *         Issue Inquiry command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  capacity: pointer to the inquiry structure
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::SCSI_Inquiry(uint8_t lun, SCSI_StdInquiryData_t* inquiry)
{
    SystemState_e Error = SYS_FAIL;
    //MSC_HandleTypeDef *MSC_Handle =  (MSC_HandleTypeDef *) phost->pActiveClass->pData;  ????

    switch(m_MSC_Handle.hbot.cmd_state)
    {
        case BOT_CMD_SEND:
        {
            // Prepare the CBW and relevent field
            m_MSC_Handle.hbot.cbw.field.DataTransferLength = DATA_LEN_INQUIRY;
            m_MSC_Handle.hbot.cbw.field.Flags = USB_EP_DIR_IN;
            m_MSC_Handle.hbot.cbw.field.CBLength = CBW_LENGTH;
            memset(m_MSC_Handle.hbot.cbw.field.CB, 0, CBW_LENGTH);
            m_MSC_Handle.hbot.cbw.field.CB[0]  = OPCODE_INQUIRY;
            m_MSC_Handle.hbot.cbw.field.CB[1]  = (lun << 5);
            m_MSC_Handle.hbot.cbw.field.CB[2]  = 0;
            m_MSC_Handle.hbot.cbw.field.CB[3]  = 0;
            m_MSC_Handle.hbot.cbw.field.CB[4]  = 0x24;
            m_MSC_Handle.hbot.cbw.field.CB[5]  = 0;
            m_MSC_Handle.hbot.state = BOT_SEND_CBW;
            m_MSC_Handle.hbot.cmd_state = BOT_CMD_WAIT;
            m_MSC_Handle.hbot.pbuf = (uint8_t *)m_MSC_Handle.hbot.data;
            Error = SYS_BUSY;
        }
        break;

        case BOT_CMD_WAIT:
        {
            Error = BOT_Process(lun);

            if(Error == SYS_READY)
            {
                memset(inquiry, 0, sizeof(SCSI_StdInquiryData_t));
                // Assign inquiry sdta
                inquiry->DeviceType = m_MSC_Handle.hbot.pbuf[0] & 0x1F;
                inquiry->PeripheralQualifier = m_MSC_Handle.hbot.pbuf[0] >> 5;
                inquiry->RemovableMedia = (m_MSC_Handle.hbot.pbuf[1] & 0x80)== 0x80;
                memcpy(inquiry->vendor_id, &m_MSC_Handle.hbot.pbuf[8], 8);
                memcpy(inquiry->product_id, &m_MSC_Handle.hbot.pbuf[16], 16);
                memcpy(inquiry->revision_id, &m_MSC_Handle.hbot.pbuf[32], 4);
            }
        }
        break;

        default:
        {
        }
        break;
    }

    return Error;
}

/**
  * @brief  USBH_MSC_SCSI_TestUnitReady
  *         Issue TestUnitReady command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::SCSI_TestUnitReady(uint8_t lun)
{
    SystemState_e Error = SYS_FAIL;
    //MSC_HandleTypeDef *MSC_Handle =  (MSC_HandleTypeDef *) phost->pActiveClass->pData;

    switch(m_MSC_Handle.hbot.cmd_state)
    {
        case BOT_CMD_SEND:
        {
            // Prepare the CBW and relevent field
            m_MSC_Handle.hbot.cbw.field.DataTransferLength = DATA_LEN_MODE_TEST_UNIT_READY;
            m_MSC_Handle.hbot.cbw.field.Flags              = USB_EP_DIR_OUT;
            m_MSC_Handle.hbot.cbw.field.CBLength           = CBW_LENGTH;
            memset(m_MSC_Handle.hbot.cbw.field.CB, 0, CBW_CB_LENGTH);
            m_MSC_Handle.hbot.cbw.field.CB[0]  = OPCODE_TEST_UNIT_READY;
            m_MSC_Handle.hbot.state            = BOT_SEND_CBW;
            m_MSC_Handle.hbot.cmd_state        = BOT_CMD_WAIT;
            Error = SYS_BUSY;
        }
        break;

        case BOT_CMD_WAIT:
        {
            Error = BOT_Process(lun);
        }
        break;

        default:
        {
        }
        break;
    }

    return Error;
}

/**
  * @brief  USBH_MSC_SCSI_ReadCapacity
  *         Issue Read Capacity command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  capacity: pointer to the capacity structure
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::SCSI_ReadCapacity(uint8_t lun, SCSI_Capacity_t* capacity)
{
    SystemState_e Error = SYS_BUSY;
    //MSC_HandleTypeDef *MSC_Handle =  (MSC_Handle_tf *) phost->pActiveClass->pData;

    switch(m_MSC_Handle.hbot.cmd_state)
    {
        case BOT_CMD_SEND:
        {
            // Prepare the CBW and relevent field
            m_MSC_Handle.hbot.cbw.field.DataTransferLength = DATA_LEN_READ_CAPACITY10;
            m_MSC_Handle.hbot.cbw.field.Flags              = USB_EP_DIR_IN;
            m_MSC_Handle.hbot.cbw.field.CBLength           = CBW_LENGTH;
            memset(m_MSC_Handle.hbot.cbw.field.CB, 0, CBW_CB_LENGTH);
            m_MSC_Handle.hbot.cbw.field.CB[0] = OPCODE_READ_CAPACITY10;
            m_MSC_Handle.hbot.state           = BOT_SEND_CBW;
            m_MSC_Handle.hbot.cmd_state       = BOT_CMD_WAIT;
            m_MSC_Handle.hbot.pbuf            = (uint8_t *)m_MSC_Handle.hbot.data;
            Error = SYS_BUSY;
        }
        break;

        case BOT_CMD_WAIT:
        {
            Error = BOT_Process(lun);

            if(Error == SYS_READY)
            {
                // Assign the capacity
                capacity->block_nbr =  m_MSC_Handle.hbot.pbuf[3]        |
                                      (m_MSC_Handle.hbot.pbuf[2] << 8)  |
                                      (m_MSC_Handle.hbot.pbuf[1] << 16) |
                                      (m_MSC_Handle.hbot.pbuf[0] << 24);

                // Assign the page length
                capacity->block_size =  m_MSC_Handle.hbot.pbuf[7]       |
                                       (m_MSC_Handle.hbot.pbuf[6] << 8);
            }
        }
        break;

        default:
        {
        }
        break;
    }

    return Error;
}

/**
  * @brief  USBH_MSC_SCSI_RequestSense
  *         Issue RequestSense command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  capacity: pointer to the sense data structure
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::SCSI_RequestSense(uint8_t lun, SCSI_Sense_t* sense_data)
{
    SystemState_e Error = SYS_FAIL;
    //MSC_HandleTypeDef *MSC_Handle =  (MSC_HandleTypeDef *) phost->pActiveClass->pData;

    switch(m_MSC_Handle.hbot.cmd_state)
    {
        case BOT_CMD_SEND:
        {
            // Prepare the CBW and relevent field
            m_MSC_Handle.hbot.cbw.field.DataTransferLength = DATA_LEN_REQUEST_SENSE;
            m_MSC_Handle.hbot.cbw.field.Flags = USB_EP_DIR_IN;
            m_MSC_Handle.hbot.cbw.field.CBLength = CBW_LENGTH;
            memset(m_MSC_Handle.hbot.cbw.field.CB, 0, CBW_CB_LENGTH);
            m_MSC_Handle.hbot.cbw.field.CB[0]  = OPCODE_REQUEST_SENSE;
            m_MSC_Handle.hbot.cbw.field.CB[1]  = (lun << 5);
            m_MSC_Handle.hbot.cbw.field.CB[2]  = 0;
            m_MSC_Handle.hbot.cbw.field.CB[3]  = 0;
            m_MSC_Handle.hbot.cbw.field.CB[4]  = DATA_LEN_REQUEST_SENSE;
            m_MSC_Handle.hbot.cbw.field.CB[5]  = 0;
            m_MSC_Handle.hbot.state = BOT_SEND_CBW;
            m_MSC_Handle.hbot.cmd_state = BOT_CMD_WAIT;
            m_MSC_Handle.hbot.pbuf = (uint8_t *)m_MSC_Handle.hbot.data;
            Error = SYS_BUSY;
        }
        break;

        case BOT_CMD_WAIT:
        {
            Error = BOT_Process(lun);

            if(Error == SYS_READY)
            {
                sense_data->key  = m_MSC_Handle.hbot.pbuf[2] & 0x0F;
                sense_data->asc  = m_MSC_Handle.hbot.pbuf[12];
                sense_data->ascq = m_MSC_Handle.hbot.pbuf[13];
            }
        }
        break;

        default:
        {
        }
        break;
    }

    return Error;
}

/**
  * @brief  USBH_MSC_BOT_Process
  *         The function handle the BOT protocol.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::BOT_Process(uint8_t lun)
{
    SystemState_e    Status     = SYS_BUSY;
    SystemState_e    Error      = SYS_BUSY;
    BOT_CSW_Status_e CSW_Status = BOT_CSW_CMD_FAILED;
    USBH_URB_State_e URB_Status = USBH_URB_IDLE;
    //MSC_Handle_t *m_MSC_Handle =  (MSC_HandleTypeDef *) phost->pActiveClass->pData;
    uint8_t toggle = 0;


    switch(m_MSC_Handle.hbot.state)
    {
        case BOT_SEND_CBW:
        {
            m_MSC_Handle.hbot.cbw.field.LUN = lun;
            m_MSC_Handle.hbot.state = BOT_SEND_CBW_WAIT;
            m_pHost->pUSB_App->SubmitURB(m_MSC_Handle.OutPipe,
                                         0,
                                         USBH_EP_BULK,
                                         USBH_PID_DATA,
                                         m_MSC_Handle.hbot.cbw.data,
                                         BOT_CBW_LENGTH,
                                         1);
        }
        break;

        case BOT_SEND_CBW_WAIT:
        {
            URB_Status = m_pHost->pUSB_App->GetURB_State(m_MSC_Handle.OutPipe);

            if(URB_Status == USBH_URB_DONE)
            {
                if(m_MSC_Handle.hbot.cbw.field.DataTransferLength != 0)
                {
                    // If there is data transfer stage
                    if(((m_MSC_Handle.hbot.cbw.field.Flags) & USB_REQ_DIR_MASK) == USB_D2H)
                    {
                        // Data direction IN
                        m_MSC_Handle.hbot.state = BOT_DATA_IN;
                    }
                    else
                    {
                        // Data Direction OUT
                        m_MSC_Handle.hbot.state = BOT_DATA_OUT;
                    }
                }
                else // If there is no data transfer stage
                {
                    m_MSC_Handle.hbot.state = BOT_RECEIVE_CSW;
                }

                nOS_FlagSend(&m_pHost->HostEvent, USBH_URB_EVENT, USBH_URB_EVENT);
               }
            else if(URB_Status == USBH_URB_NOTREADY)
            {
                // Re-send CBW
                m_MSC_Handle.hbot.state = BOT_SEND_CBW;
                nOS_FlagSend(&m_pHost->HostEvent, USBH_URB_EVENT, USBH_URB_EVENT);
            }
            else if(URB_Status == USBH_URB_STALL)
            {
                m_MSC_Handle.hbot.state = BOT_ERROR_OUT;
                nOS_FlagSend(&m_pHost->HostEvent, USBH_URB_EVENT, USBH_URB_EVENT);
            }
        }
        break;

        case BOT_DATA_IN:
        {
            // Send first packet
            m_pHost->pUSB_App->SubmitURB(m_MSC_Handle.InPipe, 1, USBH_EP_BULK, USBH_PID_DATA, m_MSC_Handle.hbot.pbuf, m_MSC_Handle.InEpSize, 0);
            m_MSC_Handle.hbot.state = BOT_DATA_IN_WAIT;
        }
        break;

        case BOT_DATA_IN_WAIT:
        {
            URB_Status = m_pHost->pUSB_App->GetURB_State(m_MSC_Handle.InPipe);

            if(URB_Status == USBH_URB_DONE)
            {
                // Adjust data pointer and data length
                if(m_MSC_Handle.hbot.cbw.field.DataTransferLength > m_MSC_Handle.InEpSize)
                {
                    m_MSC_Handle.hbot.pbuf += m_MSC_Handle.InEpSize;
                    m_MSC_Handle.hbot.cbw.field.DataTransferLength -= m_MSC_Handle.InEpSize;
                }
                else
                {
                    m_MSC_Handle.hbot.cbw.field.DataTransferLength = 0;
                }

                // More data to be received
                if(m_MSC_Handle.hbot.cbw.field.DataTransferLength > 0)
                {
                    // Send next packet
                    m_pHost->pUSB_App->SubmitURB(m_MSC_Handle.InPipe, 1, USBH_EP_BULK, USBH_PID_DATA, m_MSC_Handle.hbot.pbuf, m_MSC_Handle.InEpSize, 0);
                }
                else
                {
                    // If value was 0, and successful transfer, then change the state
                    m_MSC_Handle.hbot.state = BOT_RECEIVE_CSW;
                    nOS_FlagSend(&m_pHost->HostEvent, USBH_URB_EVENT, USBH_URB_EVENT);
                }
            }
            else if(URB_Status == USBH_URB_STALL)
            {
                // This is data IN stage STALL condition
                m_MSC_Handle.hbot.state = BOT_ERROR_IN;

                /* Refer to USB Mass-Storage Class : BOT (www.usb.org) 6.7.2 Host expects to receive data from the device
                         3. On a STALL condition receiving data, then: The host shall accept the data received.
                         The host shall clear the Bulk-In pipe.
                         4. The host shall attempt to receive a CSW.*/

                nOS_FlagSend(&m_pHost->HostEvent, USBH_URB_EVENT, USBH_URB_EVENT);
            }
        }
        break;

        case BOT_DATA_OUT:
        {
            m_pHost->pUSB_App->SubmitURB(m_MSC_Handle.OutPipe, 0, USBH_EP_BULK, USBH_PID_DATA, m_MSC_Handle.hbot.pbuf, m_MSC_Handle.OutEpSize, 0);
            m_MSC_Handle.hbot.state = BOT_DATA_OUT_WAIT;
        }
        break;

        case BOT_DATA_OUT_WAIT:
        {
            URB_Status = m_pHost->pUSB_App->GetURB_State(m_MSC_Handle.OutPipe);

            if(URB_Status == USBH_URB_DONE)
            {
                // Adjust Data pointer and data length
                if(m_MSC_Handle.hbot.cbw.field.DataTransferLength > m_MSC_Handle.OutEpSize)
                {
                    m_MSC_Handle.hbot.pbuf += m_MSC_Handle.OutEpSize;
                    m_MSC_Handle.hbot.cbw.field.DataTransferLength -= m_MSC_Handle.OutEpSize;
                }
                else
                {
                    m_MSC_Handle.hbot.cbw.field.DataTransferLength = 0;
                }

                // More data to be sent
                if(m_MSC_Handle.hbot.cbw.field.DataTransferLength > 0)
                {
                    m_pHost->pUSB_App->SubmitURB(m_MSC_Handle.OutPipe,
                                                 0,
                                                 USBH_EP_BULK,
                                                 USBH_PID_DATA,
                                                 m_MSC_Handle.hbot.pbuf,
                                                 m_MSC_Handle.OutEpSize,
                                                 1);
                }
                else
                {
                    // If value was 0, and successful transfered, then change the state
                    m_MSC_Handle.hbot.state = BOT_RECEIVE_CSW;
                }

                nOS_FlagSend(&m_pHost->HostEvent, USBH_URB_EVENT, USBH_URB_EVENT);
            }
            else if(URB_Status == USBH_URB_NOTREADY)
            {
                // Resend same data
                m_MSC_Handle.hbot.state  = BOT_DATA_OUT;
                nOS_FlagSend(&m_pHost->HostEvent, USBH_URB_EVENT, USBH_URB_EVENT);
            }
            else if(URB_Status == USBH_URB_STALL)
            {
                m_MSC_Handle.hbot.state = BOT_ERROR_OUT;

                /* Refer to USB Mass-Storage Class : BOT (www.usb.org) 6.7.3 Ho - Host expects to send data to the device
                   3. On a STALL condition sending data, then: The host shall clear the Bulk-Out pipe.
                   4. The host shall attempt to receive a CSW. */
                nOS_FlagSend(&m_pHost->HostEvent, USBH_URB_EVENT, USBH_URB_EVENT);
            }
        }
        break;

        case BOT_RECEIVE_CSW:
        {
            m_pHost->pUSB_App->SubmitURB(m_MSC_Handle.InPipe, 1, USBH_EP_BULK, USBH_PID_DATA, m_MSC_Handle.hbot.csw.data, BOT_CSW_LENGTH, 0);
            m_MSC_Handle.hbot.state  = BOT_RECEIVE_CSW_WAIT;
        }
        break;

        case BOT_RECEIVE_CSW_WAIT:
        {
            URB_Status = m_pHost->pUSB_App->GetURB_State(m_MSC_Handle.InPipe);

            // Decode CSW
            if(URB_Status == USBH_URB_DONE)
            {
                m_MSC_Handle.hbot.state = BOT_SEND_CBW;
                m_MSC_Handle.hbot.cmd_state = BOT_CMD_SEND;
                CSW_Status = DecodeCSW();
                Status = (CSW_Status == BOT_CSW_CMD_PASSED) ? SYS_READY : SYS_FAIL;
                nOS_FlagSend(&m_pHost->HostEvent, USBH_URB_EVENT, USBH_URB_EVENT);
            }
            else if(URB_Status == USBH_URB_STALL)
            {
                m_MSC_Handle.hbot.state = BOT_ERROR_IN;
                nOS_FlagSend(&m_pHost->HostEvent, USBH_URB_EVENT, USBH_URB_EVENT);
            }
        }
        break;

        case BOT_ERROR_IN:
        {
            Error = BOT_Abort(lun, BOT_DIR_IN);

            if(Error == SYS_READY)
            {
                m_MSC_Handle.hbot.state = BOT_RECEIVE_CSW;
            }
            else if(Error == SYS_UNRECOVERED_ERROR)
            {
                // This means that there is a STALL Error limit, Do Reset Recovery
                m_MSC_Handle.hbot.state = BOT_UNRECOVERED_ERROR;
            }
        }
        break;

        case BOT_ERROR_OUT:
        {
            Error = BOT_Abort(lun, BOT_DIR_OUT);

            if(Error == SYS_READY)
            {

                toggle = m_pHost->pUSB_App->GetToggle(m_MSC_Handle.OutPipe);
                m_pHost->pUSB_App->SetToggle(m_MSC_Handle.OutPipe, 1- toggle);
                m_pHost->pUSB_App->SetToggle(m_MSC_Handle.InPipe, 0);
                m_MSC_Handle.hbot.state = BOT_ERROR_IN;
            }
            else if(Error == SYS_UNRECOVERED_ERROR)
            {
                m_MSC_Handle.hbot.state = BOT_UNRECOVERED_ERROR;
            }
        }
        break;

        case BOT_UNRECOVERED_ERROR:
        {
            Status = BOT_REQ_Reset();

            if(Status == SYS_READY)
            {
                m_MSC_Handle.hbot.state = BOT_SEND_CBW;
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
  * @brief  USBH_MSC_BOT_Abort
  *         The function handle the BOT Abort process.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  dir: direction (0: out / 1 : in)
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::BOT_Abort(uint8_t lun, uint8_t dir)
{
    SystemState_e Status = SYS_FAIL;
    //MSC_HandleTypeDef *MSC_Handle =  (MSC_HandleTypeDef *) phost->pActiveClass->pData;

    switch(dir)
    {
        // send ClrFeture on Bulk IN endpoint
        case BOT_DIR_IN:    Status = m_pHost->pBase->ClrFeature(m_MSC_Handle.InEp);  break;
        //send ClrFeature on Bulk OUT endpoint
        case BOT_DIR_OUT:    Status = m_pHost->pBase->ClrFeature(m_MSC_Handle.OutEp); break;
    }

    return Status;
}

/**
  * @brief  USBH_MSC_BOT_REQ_Reset
  *         The function the MSC BOT Reset request.
  * @param  phost: Host handle
  * @retval USBH Status
  */
SystemState_e USB_HostMassStorage::BOT_REQ_Reset(void)
{
    m_pHost->Control.Setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;
    m_pHost->Control.Setup.b.bRequest      = USB_REQ_BOT_RESET;
    m_pHost->Control.Setup.b.wValue.w      = 0;
    m_pHost->Control.Setup.b.wIndex.w      = 0;
    m_pHost->Control.Setup.b.wLength.w     = 0;
    m_pHost->pBase->ControlRequest(0, 0);
}

BOT_CSW_Status_e USB_HostMassStorage::DecodeCSW(void)
{
//  MSC_HandleTypeDef *MSC_Handle =  (MSC_HandleTypeDef *) phost->pActiveClass->pData;

    BOT_CSW_Status_e Status = BOT_CSW_CMD_FAILED;

    // Checking if the transfer length is different than 13
    if(m_pHost->pUSB_App->GetLastXferSize(m_MSC_Handle.InPipe) != BOT_CSW_LENGTH)
    {
        Status = BOT_CSW_PHASE_ERROR;
    }
    else // CSW length is Correct
    {
        // Check validity of the CSW Signature and CSWStatus
        if(m_MSC_Handle.hbot.csw.field.Signature == BOT_CSW_SIGNATURE)                  // Check Condition 1. dCSWSignature is equal to 0x53425355
        {
            if(m_MSC_Handle.hbot.csw.field.Tag == m_MSC_Handle.hbot.cbw.field.Tag)    // Check Condition 3. dCSWTag matches the dCBWTag from the corresponding CBW
            {
                if(m_MSC_Handle.hbot.csw.field.Status == 0)
                {
                    Status = BOT_CSW_CMD_PASSED;
                }
                else if(m_MSC_Handle.hbot.csw.field.Status == 1)
                {
                    Status = BOT_CSW_CMD_FAILED;
                }
                else if(m_MSC_Handle.hbot.csw.field.Status == 2)
                {
                    Status = BOT_CSW_PHASE_ERROR;
                }
            }
        }
        else
        {
            // If the CSW Signature is not valid, We sall return the Phase Error to upper layers for reset recovery
            Status = BOT_CSW_PHASE_ERROR;
        }
    }

    return Status;
}

