//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_i2c.h
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
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "bsp_cfg.h"
#include "stm32f4xx.h"
#include "nOS.h"
#include "lib_typedef.h"

//#include "app.h"
//#include "PortState.h"
//#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define I2C_PORT_QTY                        3
#define I2C_TICK_HOOK_TIME                    1                // in mSec

//-------------------------------------------------------------------------------------------------
// struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

enum I2C_IRQ_Number_e
{
    I2C1_HARD_PORT = 0,
    I2C2_HARD_PORT,
    I2C3_HARD_PORT,
};

// To found how many port there is
enum I2C_Port_e
{
  #define X_I2C(ENUM_ID, I2C_TYPEDEF, CLOCK, MUTEX, SPEED, PREEMP_PRIO, HARDWARE_PORT, ALTERNATE_FUNCTION, EVENT_IRQ_NB, ERROR_IRQ_NB, GPIO_SCL, PIN_SCL, SCL_PIN_SOURCE, SCL_GPIO_CLOCK, GPIO_SDA, PIN_SDA, SDA_PIN_SOURCE, SDA_GPIO_CLOCK) ENUM_ID,
    I2C_DEF
  #undef X_I2C
    I2C_NB_PORT_CONST
};

// To found how many device there is
enum I2C_Device_e
{
  #define X_I2C_DEVICE(ENUM_ID, MUTEX, PHYSICAL_ADDRESS, SIZE_OF_ADDRESS_POINTER, SPEED) ENUM_ID,
    I2C_DEVICE_DEF
  #undef X_I2C_DEVICE
    I2C_NB_DEVICE_CONST
};

struct I2C_PortInfo_t
{
    I2C_TypeDef*        pI2Cx;
    uint32_t            Clock;
    nOS_Mutex*          pMutex;
    uint32_t            Speed;
    uint8_t             PreempPrio;
    uint8_t             HardwarePort;
    uint8_t             AlternateFunction;
    IRQn_Type           EV_IRQn;
    IRQn_Type           ER_IRQn;
    GPIO_TypeDef*       pGPIO_SCL;
    uint16_t            SCL_Pin;
    uint8_t             SCL_PinSource;
    uint32_t            SCL_Clock;
    GPIO_TypeDef*       pGPIO_SDA;
    uint16_t            SDA_Pin;
    uint8_t             SDA_PinSource;
    uint32_t            SDA_Clock;
};

struct I2C_DeviceInfo_t
{
    nOS_Mutex*          pMutex;
    uint8_t             PhysicalAddress;
    size_t                MemPtrAddressLength;
    uint32_t            Speed;
};

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class CI2C
{
    public:

                        CI2C                (I2C_PortInfo_t* pPort);
                        ~CI2C                ();

        SystemState_e   LockToDevice           (I2C_DeviceInfo_t* pDevice);                                             // Set I2C to this device and lock
        SystemState_e   UnlockFromDevice     (I2C_DeviceInfo_t* pDevice);                                             // Unlock I2C from device
        SystemState_e   GetStatus           ();

        // Read function (overloaded)
        SystemState_e   Read                (uint32_t AddressInDevice, void* pDataAddress, size_t Size);
        SystemState_e   Read                (uint32_t AddressInDevice, uint8_t* Data);
        SystemState_e   Read                (uint32_t AddressInDevice, uint16_t* Data);
        SystemState_e   Read                (uint32_t AddressInDevice, uint32_t* Data);
        SystemState_e   Read                (uint32_t AddressInDevice, void* pDataAddress, size_t Size, I2C_DeviceInfo_t* pDevice);
        SystemState_e   Read                (uint32_t AddressInDevice, uint8_t* pData, I2C_DeviceInfo_t* pDevice);
        SystemState_e   Read                (uint32_t AddressInDevice, uint16_t* pData, I2C_DeviceInfo_t* pDevice);
        SystemState_e   Read                (uint32_t AddressInDevice, uint32_t* pData, I2C_DeviceInfo_t* pDevice);

        // Write function (overloaded)
        SystemState_e   Write                (uint32_t AddressInDevice, void* pDataAddress, size_t Size);
        SystemState_e   Write                (uint32_t AddressInDevice, uint8_t Data);
        SystemState_e   Write                (uint32_t AddressInDevice, uint16_t Data);
        SystemState_e   Write                (uint32_t AddressInDevice, uint32_t Data);
        SystemState_e   Write               (uint32_t AddressInDevice, void* pDataAddress, size_t Size, I2C_DeviceInfo_t* pDevice);
        SystemState_e   Write               (uint32_t AddressInDevice, uint8_t Data, I2C_DeviceInfo_t* pDevice);
        SystemState_e   Write               (uint32_t AddressInDevice, uint16_t Data, I2C_DeviceInfo_t* pDevice);
        SystemState_e   Write               (uint32_t AddressInDevice, uint32_t Data, I2C_DeviceInfo_t* pDevice);

        void            Init                 ();
        void            TickHook            ();
        void            ER_IRQHandler       ();
        void            EV_IRQHandler       ();

    private:

        void            ClearBus            ();
        uint32_t        CalculateBitMask    (uint8_t Mask, uint16_t BitConfig);
        SystemState_e   Request             (AccessRequest_e Request, uint32_t AddressInDevice, void *pDataAddress, size_t Size);
        uint32_t        GetLastEvent        ();
        void            Lock                ();
        void            Unlock              ();

        I2C_PortInfo_t*                 m_pPort;
        I2C_DeviceInfo_t*               m_pDevice;

        AccessRequest_e                    m_Request;
        volatile uint8_t*               m_pAddressInDevice;
        volatile uint8_t*               m_pDataAddress;
        volatile size_t                    m_Size;
        volatile size_t                    m_AddressLengthCount;

        volatile SystemState_e            m_Status;
        volatile uint8_t                m_Timeout;

};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef STM32F4_I2C_GLOBAL

  I2C_PortInfo_t I2C_PortInfo[I2C_NB_PORT_CONST] =
  {
    #define X_I2C(ENUM_ID, I2C_TYPEDEF, CLOCK,  MUTEX, SPEED, PREEMP_PRIO, HARDWARE_PORT, ALTERNATE_FUNCTION,            EVENT_IRQ_NB,            ERROR_IRQ_NB, GPIO_SCL, PIN_SCL, SCL_PIN_SOURCE, SCL_GPIO_CLOCK, GPIO_SDA, PIN_SDA, SDA_PIN_SOURCE, SDA_GPIO_CLOCK) \
                          {I2C_TYPEDEF, CLOCK, &MUTEX, SPEED, PREEMP_PRIO, HARDWARE_PORT, ALTERNATE_FUNCTION, (IRQn_Type)EVENT_IRQ_NB, (IRQn_Type)ERROR_IRQ_NB, GPIO_SCL, PIN_SCL, SCL_PIN_SOURCE, SCL_GPIO_CLOCK, GPIO_SDA, PIN_SDA, SDA_PIN_SOURCE, SDA_GPIO_CLOCK},
      I2C_DEF
    #undef X_I2C
  };

  I2C_DeviceInfo_t I2C_DeviceInfo[I2C_NB_DEVICE_CONST] =
  {
    #define X_I2C_DEVICE(ENUM_ID, MUTEX, PHYSICAL_ADDRESS, SIZE_OF_ADDRESS_POINTER, SPEED) {&MUTEX, PHYSICAL_ADDRESS, SIZE_OF_ADDRESS_POINTER, SPEED},
      I2C_DEVICE_DEF
    #undef X_I2C_DEVICE
  };

  CI2C I2C_Port1(&I2C_PortInfo[I2C1_PORT]);
  //CI2C I2C_Port2(&I2C_PortInfo[I2C2_PORT]);
  //CI2C I2C_Port3(&I2C_PortInfo[I2C3_PORT]);

#else

  extern I2C_PortInfo_t     I2C_PortInfo[I2C_NB_PORT_CONST];
  extern I2C_DeviceInfo_t   I2C_DeviceInfo[I2C_NB_DEVICE_CONST];
  extern CI2C I2C_Port1;
  //extern CI2C I2C_Port2;
  //extern CI2C I2C_Port3;

#endif



// from old version!!
#if 0

// To found how many port there is
enum I2C_Port_e
{
  #define X_I2C(I2C_NUMBER, CLOCK, SPEED, PREEMP_PRIO, EVENT_IRQ_NB, ERROR_IRQ_NB, GPIO_SCL, GPIO_PIN_MASK_SCL, GPIO_SCL_CLOCK, GPIO_SCL_AF, \
                                                                                   GPIO_SDA, GPIO_PIN_MASK_SDA, GPIO_SDA_CLOCK, GPIO_SDA_AF) I2C##I2C_NUMBER##_PORT,
    I2C_DEF
  #undef X_I2C
    I2C_NB_PORT_CONST
};

struct I2C_PortInfo_t
{
    I2C_TypeDef*        pI2Cx;
    uint32_t            Clock;
    uint32_t            Speed;
    uint8_t             PreempPrio;
    IRQn_Type           EV_IRQn;
    IRQn_Type           ER_IRQn;
    IO_Pin_t            SCL;
    IO_Pin_t            SDA;
};


struct I2C_DeviceInfo_t
{
    nOS_Mutex*          pMutex;
    uint8_t             PhysicalAddress;
    size_t		        MemPtrAddressLength;
    uint32_t            Speed;
};


//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class I2C
{
	public:

                        I2C				    (I2C_PortInfo_t* pPort);
                        ~I2C				();

        SystemState_e   LockToDevice   		(uint8_t Device);       // Set I2C to this device and lock
        SystemState_e   UnlockFromDevice 	(uint8_t Device);       // Unlock I2C from device
        SystemState_e   GetStatus       	(void);

        SystemState_e   ReadRegister        (uint8_t Register, void* pRxBuffer, size_t RxSize);
        SystemState_e   ReadRegister        (uint8_t Register, void* pRxBuffer, size_t RxSize, uint8_t Device);
        SystemState_e   Transfer            (void* pTxBuffer, size_t TxSize, void* pRxBuffer, size_t RxSize);
        SystemState_e   Transfer            (void* pTxBuffer, size_t TxSize, void* pRxBuffer, size_t RxSize, uint8_t Device);

		void	        Initialize 			(void);
        void            ER_IRQHandler       (void);
        void            EV_IRQHandler       (void);

	private:

        //void            ClearBus            (void);
        //uint32_t        CalculateBitMask    (uint8_t Mask, uint16_t BitConfig);
        void            Lock                (void);
        void            Unlock              (void);

        I2C_PortInfo_t*                 m_pPort;
        nOS_Mutex                       m_Mutex;
        int16_t                         m_Device;

        volatile size_t				    m_TxSize;
        volatile size_t				    m_RxSize;
        volatile uint8_t*               m_pTxBuffer;
        volatile uint8_t*               m_pRxBuffer;

        volatile uint8_t*               m_pAddressInDevice;
        volatile uint8_t*               m_pDataAddress;
        volatile size_t				    m_Size;
        volatile size_t		            m_AddressLengthCount;

        volatile SystemState_e		    m_Status;
        volatile uint8_t                m_Timeout;
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef STM32F4_I2C_GLOBAL

  I2C_PortInfo_t I2C_PortInfo[I2C_NB_PORT_CONST] =
  {
    #define X_I2C(I2C_NUMBER, CLOCK, SPEED, PREEMP_PRIO,            EVENT_IRQ_NB,            ERROR_IRQ_NB,  IO_SCL, IO_PIN_MASK_SCL, IO_SCL_CLOCK, IO_SCL_AF, \
                                                                                                            IO_SDA, IO_PIN_MASK_SDA, IO_SDA_CLOCK, IO_SDA_AF) \
            {I2C##I2C_NUMBER, CLOCK, SPEED, PREEMP_PRIO, (IRQn_Type)EVENT_IRQ_NB, (IRQn_Type)ERROR_IRQ_NB, {IO_SCL, IO_PIN_MASK_SCL, IO_SCL_CLOCK, IO_SCL_AF},\
                                                                                                           {IO_SDA, IO_PIN_MASK_SDA, IO_SDA_CLOCK, IO_SDA_AF},},
      I2C_DEF
    #undef  X_I2C
  };

  // Name of the I2C Object will be the name of the port + _Port -> Ex. I2C1 -> I2C1_Port
  // Ex. I2C I2C_Port1(&I2C_PortInfo[I2C1_PORT]);
  #define X_I2C(I2C_NUMBER, CLOCK, SPEED, PREEMP_PRIO, EVENT_IRQ_NB, ERROR_IRQ_NB,  IO_SCL, IO_PIN_MASK_SCL, IO_SCL_CLOCK, IO_SCL_AF, \
                                                                                    IO_SDA, IO_PIN_MASK_SDA, IO_SDA_CLOCK, IO_SDA_AF) \
    class I2C I2C_Port##I2C_NUMBER(&I2C_PortInfo[I2C##I2C_NUMBER##_PORT]);
    I2C_DEF
  #undef  X_I2C

#else // STM32F4_I2C_GLOBAL

  extern I2C_PortInfo_t     I2C_PortInfo[I2C_NB_PORT_CONST];

  #define X_I2C(I2C_NUMBER, CLOCK, SPEED, PREEMP_PRIO, EVENT_IRQ_NB, ERROR_IRQ_NB,  IO_SCL, IO_PIN_MASK_SCL, IO_SCL_CLOCK, IO_SCL_AF, \
                                                                                    IO_SDA, IO_PIN_MASK_SDA, IO_SDA_CLOCK, IO_SDA_AF) \
    extern class I2C I2C_Port##I2C_NUMBER;
    I2C_DEF
  #undef X_I2C

#endif // STM32F4_I2C_GLOBAL

//-------------------------------------------------------------------------------------------------
#endif
