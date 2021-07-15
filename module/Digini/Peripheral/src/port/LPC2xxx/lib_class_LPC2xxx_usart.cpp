//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_LPC2xxx_uart.cpp
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
#include "lib_class_STM32F4_usart.h"
#include "stm32f4xx_usart.h"

//-------------------------------------------------------------------------------------------------

#if USE_UART_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Class: CUSART
//
//
//   Description:   Class to handle USART
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CUSART
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Initializes the USARTx peripheral according to the specified Parameters
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CUSART::CUSART(USART_PortInfo_t* pPort)
{
    m_pPort   = pPort;
    m_pDevice = nullptr;
    m_Status  = SYS_DEVICE_NOT_PRESENT;
    m_pTxFifo = new CFIFO(m_pPort->BufferSize);
    m_pRxFifo = new CFIFO(m_pPort->BufferSize);

}


//-------------------------------------------------------------------------------------------------
//
//   Destructor:   CUSART
//
//   Parameter(s):
//   Return Value:
//
//   Description:    Deinitializes the USARTx peripheral
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CUSART::~CUSART(void)
{
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Init
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Initialize this USART port
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CUSART::Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable peripheral clock for USART2 */
    RCC_APB2PeriphClockCmd(m_pPort->Clock, ENABLE);

    /* TX GPIO clock enable */
    RCC_AHB1PeriphClockCmd( m_pPort->TX_Clock, ENABLE);

    /* RX GPIO clock enable */
    RCC_AHB1PeriphClockCmd( m_pPort->RX_Clock, ENABLE);

    /*GPIO Configuration*/
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                               // the pins are configured as alternate function so the USART peripheral has access to them
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                              // this defines the output type as push pull mode (as opposed to open drain)
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                               // this activates the pullup resistors on the IO pins

    /* TX GPIO Configuration*/
    GPIO_InitStructure.GPIO_Pin   = m_pPort->TX_Pin;
    GPIO_InitStructure.GPIO_Speed = m_pPort->TX_Speed;                          // this defines the IO speed and has nothing to do with the baudrate!
    GPIO_Init(m_pPort->TX_pGPIO, &GPIO_InitStructure);

    /* RX GPIO Configuration*/
    GPIO_InitStructure.GPIO_Pin   = m_pPort->RX_Pin;
    GPIO_InitStructure.GPIO_Speed = m_pPort->RX_Speed;                          // this defines the IO speed and has nothing to do with the baudrate!
    GPIO_Init(m_pPort->RX_pGPIO, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(m_pPort->TX_pGPIO, m_pPort->TX_PinSource, m_pPort->AlternateFunction);
    GPIO_PinAFConfig(m_pPort->RX_pGPIO, m_pPort->RX_PinSource, m_pPort->AlternateFunction);

    /* USART congiguration */
    USART_InitStructure.USART_BaudRate            = m_pPort->BaudRate;
    USART_InitStructure.USART_WordLength          = m_pPort->WordLength;
    USART_InitStructure.USART_StopBits            = m_pPort->StopBits;
    USART_InitStructure.USART_Parity              = m_pPort->Parity;
    USART_InitStructure.USART_HardwareFlowControl = m_pPort->HardwareFlowControl;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(m_pPort->pUSARTn, &USART_InitStructure);

    /* Interrupt configuration */
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = m_pPort->IRQn;                            // we want to configure the USART interrupts
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = m_pPort->PreempPrio; // this sets the priority group of the USART interrupts
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = m_pPort->SubPriority;        // this sets the subpriority inside the group
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                // the USART interrupts are globally enabled
    NVIC_Init(&NVIC_InitStructure);                                                // the properties are passed to the NVIC_Init function which takes care of the low level stuff

    /* Enable the complete USART peripheral */
    USART_Cmd(m_pPort->pUSARTn, ENABLE);

    /* Enable Interrupt */
    USART_ITConfig(m_pPort->pUSARTn, USART_IT_RXNE , ENABLE); // enable the USART interrupt
    USART_ITConfig(m_pPort->pUSARTn, USART_IT_TXE , DISABLE); // disable the USART interrupt, enable when a write occur
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           LockToDevice
//
//  Parameter(s):   USART_DeviceInfo_t*   pDevice
//  Return:         SystemState_e         Status
//
//  Description:    This routine will configure the USART port to work with a specific device and
//                  lock it, so any other access to the port will be block until unlock
//
//  Note(s):        If a write without lock is executed then it will be done on the locked device
//
//-------------------------------------------------------------------------------------------------
SystemState_e CUSART::LockToDevice(USART_DeviceInfo_t* pDevice)
{
    while(nOS_MutexLock(pDevice->pMutex, NOS_WAIT_INFINITE) != NOS_OK){};

    // Now we have the mutex than keep a copy for the UnlockFromDevice()
    m_pDevice       = pDevice;
    m_Status        = SYS_READY;

    m_pPort->BaudRate             =  pDevice->BaudRate;
    m_pPort->WordLength           =  pDevice->WordLength;
    m_pPort->StopBits             =  pDevice->StopBits;
    m_pPort->Parity               =  pDevice->Parity;
    m_pPort->HardwareFlowControl  =  pDevice->HardwareFlowControl;

    Init();

    return m_Status;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           UnlockFromDevice
//
//  Parameter(s):   USART_DeviceInfo_t*   pDevice
//  Return:         SystemState_e         Status
//
//  Description:    This routine will unlock USART port from a specific device
//
//  Note(s):        If a write without lock is executed then it will be done on the locked device
//                  if lock and no write at all if not lock to a device
//
//-------------------------------------------------------------------------------------------------
SystemState_e CUSART::UnlockFromDevice(USART_DeviceInfo_t* pDevice)
{
    if(pDevice == m_pDevice)
    {
        nOS_MutexUnlock(m_pDevice->pMutex);
        pDevice  = nullptr;
        m_Status = SYS_DEVICE_NOT_PRESENT;
    }
    else
    {
        return SYS_NOT_LOCK_TO_DEVICE;
    }
    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      GetStatus
//
//   Parameter(s):
//   Return Value:
//
//   Description:    SystemState_e  Return general status of the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e CUSART::GetStatus(void)
{
    return m_Status;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Read
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Read overload function
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e CUSART::Read(uint8_t*  pData)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return Request(ACCESS_READ, pData, sizeof(uint8_t));
}
SystemState_e CUSART::Read(uint16_t* pData)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return Request(ACCESS_READ, (void*)pData, sizeof(uint16_t));

}
SystemState_e CUSART::Read(uint32_t* pData)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return Request(ACCESS_READ, (void*)pData, sizeof(uint32_t));

}
SystemState_e CUSART::Read(uint8_t* pBuffer, size_t Size)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return Request(ACCESS_READ, pBuffer, Size);
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Write
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Write overload function
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e CUSART::Write(uint8_t  Data)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return Request(ACCESS_WRITE, (void*)&Data, sizeof(uint8_t));
}

SystemState_e CUSART::Write(uint16_t Data)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return Request(ACCESS_WRITE, (void*)&Data, sizeof(uint16_t));
}

SystemState_e CUSART::Write(uint32_t Data)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return Request(ACCESS_WRITE, (void*)&Data, sizeof(uint32_t));
}

SystemState_e CUSART::Write(uint8_t* pBuffer, size_t Size)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return Request(ACCESS_WRITE, pBuffer, Size);
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Lock
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Lock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CUSART::Lock(void)
{
    while(nOS_MutexLock(m_pPort->pMutex, NOS_WAIT_INFINITE) != NOS_OK){};
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      Unlock
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Unlock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void CUSART::Unlock(void)
{
    nOS_MutexUnlock(m_pPort->pMutex);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Request
//
//  Parameter(s):   AccessRequest_e  Request
//                  void*           pBuffer
//                  size_t          Size
//  Return:         SystemState_e    Status
//
//  Description:    Read or writes data to USART device.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e CUSART::Request(AccessRequest_e Request, void* pBuffer, size_t Size)
{
    SystemState_e   Result       = SYS_FAIL;
    uint8_t*        pDataAddress = (uint8_t*)pBuffer;

    if(m_pDevice == nullptr)        return SYS_DEVICE_NOT_PRESENT;
    if(Size > m_pPort->BufferSize)  return SYS_WRONG_SIZE;

    m_Timeout = m_pDevice->TimeOut;

    if(Request == ACCESS_WRITE)
    {
        for(size_t i = 0; i < Size; i++)
        {
            OS_EnterCritical();
            if(m_pTxFifo->Push(pDataAddress[i]))
            {
                Result = SYS_READY;
                USART_ITConfig(USART1, USART_IT_TXE , ENABLE); // enable the USART interrupt
            }
            else
            {
                Result = SYS_FAIL;
                break;
            }
            OS_ExitCritical();
        }
    }
    else if(Request == ACCESS_READ)
    {
        for(size_t i = 0; i < Size; i++)
        {
            OS_EnterCritical();
            if(m_pRxFifo->Pop(&pDataAddress[i]))
                Result = SYS_READY;
            else
            {
                if(i == 0)
                    Result = NO_DATA;
                else
                    Result = SYS_FAIL;
                break;
            }
            OS_ExitCritical();
        }
    }

    OS_ExitCritical();

    return Result;
}
//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:    IRQHandler
//
//  Description:    This function handles USARTx interrupt.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CUSART::IRQHandler(void)
{
    uint8_t data;

    if(USART_GetITStatus(m_pPort->pUSARTn, USART_IT_TXE))
    {
        if(m_pTxFifo->Pop(&data))
        {
            USART_SendData( m_pPort->pUSARTn, data);
        }
        else
        {
            USART_ITConfig(m_pPort->pUSARTn, USART_IT_TXE , DISABLE);       // disable the USART TX empty interrupt, enable when a write occur
        }
    }
    else if(USART_GetITStatus(m_pPort->pUSARTn, USART_IT_RXNE))
    {
        data = USART_ReceiveData(m_pPort->pUSARTn);
        m_pRxFifo->Push(data);
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           TickHook
//
//  Parameter(s):   none
//  Return:         none
//
//  Description:    1ms state machine timeout tick hook (must be added to OS Tick hook function)
//
//-------------------------------------------------------------------------------------------------
void CUSART::TickHook(void)
{
    if(m_Timeout > 0)
    {
        m_Timeout--;

        if(m_Timeout == 0) m_Status = SYS_HUNG;
        else               m_Status = BUSY;
    }

}

//-------------------------------------------------------------------------------------------------

#endif // USE_UART_DRIVER == DEF_ENABLED
