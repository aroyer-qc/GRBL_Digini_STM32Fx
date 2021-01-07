//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_usart.h
//
//
//  Notes: Callback for TickHook will be reinitialize when LockToDevice() or UnlockFromDevice()
//         is called
//
//*************************************************************************************************

#ifndef __STM32F4_USART_H__
#define __STM32F4_USART_H__

#ifdef STM32F4_USART_GLOBAL
    #define STM32F4_USART_EXTERN
#else
    #define STM32F4_USART_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "stm32f4xx.h"
#include "nOS.h"
#include "lib_digini.h"
//#include "PortState.h"
#include "lib_class_STM32F4_gpio.h"
#include "lib_fifo.h"
#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

// To found how many port ther is
enum USART_PortQty_e
{
    #define X_USART( ENUM_ID, USART_TYPEDEF, USART_CLOCK, MUTEX,  BUFFER_SIZE, PREEMP_PRIO, SUB_PRIORITY, ALTERNATE_FUNCTION, IRQ_NB, BAUDRATE, WORD_LENGHT, STOP_BITS, PARITY, HARDWARE_FLOW_CONTROL, TX_PORT, TX_PIN, TX_PIN_SOURCE, TX_SPEED, TX_CLOCK, RX_PORT, RX_PIN, RX_PIN_SOURCE, RX_SPEED, RX_CLOCK) ENUM_ID,
    USART_DEF
    #undef X_USART
    USART_NB_PORT_CONST
};

// To found how many device there is
enum USART_DeviceQty_e
{
    #ifdef USART_DEVICE_DEF
    #define X_USART_DEVICE(ENUM_ID, DEFINITION, MUTEX, TIMEOUT, BAUDRATE, WORD_LENGHT, STOP_BITS, PARITY, HARDWARE_FLOW_CONTROL)  ENUM_ID,
    USART_DEVICE_DEF
    #undef X_USART_DEVICE
    #endif

    USART_NB_DEVICE_CONST
};

struct USART_PortInfo_t
{
    USART_TypeDef*      pUSARTn;
    uint32_t            Clock;
    nOS_Mutex*          pMutex;
    uint16_t            BufferSize;

    uint8_t             PreempPrio;
    uint8_t             SubPriority;
    uint8_t             AlternateFunction;
    IRQn_Type           IRQn;

    uint32_t            BaudRate;
    uint16_t            WordLength;
    uint16_t            StopBits;
    uint16_t            Parity;
    uint16_t            HardwareFlowControl;

    GPIO_TypeDef*       TX_pGPIO;
    uint16_t            TX_Pin;
    uint8_t             TX_PinSource;
    GPIOSpeed_TypeDef   TX_Speed;
    uint32_t            TX_Clock;

    GPIO_TypeDef*       RX_pGPIO;
    uint16_t            RX_Pin;
    uint8_t             RX_PinSource;
    GPIOSpeed_TypeDef   RX_Speed;
    uint32_t            RX_Clock;
};

struct USART_DeviceInfo_t
{
    nOS_Mutex*          pMutex;
    uint8_t             TimeOut;
    uint32_t            BaudRate;
    uint16_t            WordLength;
    uint16_t            StopBits;
    uint16_t            Parity;
    uint16_t            HardwareFlowControl;
};

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class CUSART
{
	public:

                        CUSART				    (USART_PortInfo_t* pPort);
                        ~CUSART				    ();
        void            Init                    ();
        SystemState_e   LockToDevice   		    (USART_DeviceInfo_t* pDevice);                                             // Set USART to this device and lock
        SystemState_e   UnlockFromDevice 	    (USART_DeviceInfo_t* pDevice);                                             // Unlock USART from device
        SystemState_e   GetStatus       	    ();

		// Read function (overloaded)
		SystemState_e   Read				    (uint8_t*  pData);
		SystemState_e   Read				    (uint16_t* pData);
		SystemState_e   Read				    (uint32_t* pData);
        SystemState_e   Read				    (uint8_t* pBuffer, size_t Size);

		// Write function (overloaded)
		SystemState_e   Write				    (uint8_t  Data);
		SystemState_e   Write				    (uint16_t Data);
		SystemState_e   Write				    (uint32_t Data);
        SystemState_e   Write				    (uint8_t* pBuffer, size_t Size);

        SystemState_e   Request                 (AccessRequest_e Request, void* pBuffer, size_t Size);
        void            IRQHandler              ();
        void            TickHook                ();

	private:

        void            Lock                    ();
        void            Unlock                  ();

        USART_PortInfo_t*                       m_pPort;
        USART_DeviceInfo_t*                     m_pDevice;
        volatile SystemState_e		            m_Status;
        volatile uint8_t                        m_Timeout;

        CFIFO*                                  m_pTxFifo;
        CFIFO*                                  m_pRxFifo;
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------
#ifdef STM32F4_USART_GLOBAL

    USART_PortInfo_t USART_PortInfo[USART_NB_PORT_CONST] =
    {

    #define X_USART( ENUM_ID, USART_TYPEDEF, USART_CLOCK,  MUTEX, BUFFER_SIZE, PREEMP_PRIO, SUB_PRIORITY, ALTERNATE_FUNCTION, IRQ_NB, BAUDRATE, WORD_LENGHT, STOP_BITS, PARITY, HARDWARE_FLOW_CONTROL, TX_PORT, TX_PIN, TX_PIN_SOURCE, TX_SPEED, TX_CLOCK, RX_PORT, RX_PIN, RX_PIN_SOURCE, RX_SPEED, RX_CLOCK) \
                     {        USART_TYPEDEF, USART_CLOCK, &MUTEX, BUFFER_SIZE, PREEMP_PRIO, SUB_PRIORITY, ALTERNATE_FUNCTION, IRQ_NB, BAUDRATE, WORD_LENGHT, STOP_BITS, PARITY, HARDWARE_FLOW_CONTROL, TX_PORT, TX_PIN, TX_PIN_SOURCE, TX_SPEED, TX_CLOCK, RX_PORT, RX_PIN, RX_PIN_SOURCE, RX_SPEED, RX_CLOCK },

    USART_DEF
    #undef X_USART
    };

    #ifdef USART_DEVICE_DEF
    USART_DeviceInfo_t USART_DeviceInfo[USART_NB_DEVICE_CONST] =
    {
        #define X_USART_DEVICE(ENUM_ID, DEFINITION,  MUTEX, TIMEOUT, BAUDRATE, WORD_LENGHT, STOP_BITS, PARITY, HARDWARE_FLOW_CONTROL) \
                               {                    &MUTEX, TIMEOUT, BAUDRATE, WORD_LENGHT, STOP_BITS, PARITY, HARDWARE_FLOW_CONTROL },
        USART_DEVICE_DEF
        #undef X_USART_DEVICE
    };
    #endif

   CUSART USART_Port1(&USART_PortInfo[USART1_PORT]);

#else // STM32F4_USART_GLOBAL

  extern USART_PortInfo_t USART_PortInfo[USART_NB_PORT_CONST];
 #ifdef USART_DEVICE_DEF
  extern USART_DeviceInfo_t USART_DeviceInfo[USART_NB_DEVICE_CONST];
 #endif
  extern CUSART USART_Port1;

#endif // STM32F4_USART_GLOBAL

#endif //__STM32F4_USART_H__




