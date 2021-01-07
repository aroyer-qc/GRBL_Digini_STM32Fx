//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_i2c_STMPE811.h
//
//*************************************************************************************************

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_typedef.h"
#include "lib_class_i2c.h"
//#include "app.h"
//#include "stm32f4xx.h"
//#include "RTOS.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

// ---- Global Interrupts definitions ----
#define IRQ_FLAG_GPIO               0x80
#define IRQ_FLAG_ADC                0x40
#define IRQ_FLAG_TEMP               0x20
#define IRQ_FLAG_FE                 0x10
#define IRQ_FLAG_FF                 0x08
#define IRQ_FLAG_FOV                0x04
#define IRQ_FLAG_FTH                0x02
#define IRQ_FLAG_TOUCH              0x01
#define IRQ_FLAG_ALL                0xFF


//-------------------------------------------------------------------------------------------------
// typedef struct(s)
//-------------------------------------------------------------------------------------------------

typedef struct TOUCH_State_s
{
  uint16_t  X;
  uint16_t  Y;
  uint16_t  Z;
  uint8_t   TouchDetected;
} TOUCH_sState;

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

// TODO: Mutex is not yet integrated ( do i need to? )

class CTouch
{
    public:

                            CTouch                  (CI2C* pI2C);
                            ~CTouch                 ();

        bool                Initialize              ();
        void                ProcessTouch            ();
        void                FifoReset               ();
        void                SetTouchCorrection      (LinearEquation_t X_Correction, LinearEquation_t Y_Correction);
        void                EnableTouchCorrection   (bool Enable);
        void                TS_GetXY                (uint16_t *X, uint16_t *Y);
        uint16_t            GetY                    ();
        void                SetIRQ_Enable           (bool State);
        bool                GetIRQ_Status           (uint8_t IRQ_Flag);
        void                ClearIRQ_Status         (uint8_t IRQ_Flag);
        bool                IsOperational           ();

    private:

        CI2C*               m_pI2C;
        I2C_DeviceInfo_t*   m_pDevice;
        uint16_t            m_IntX;
        uint16_t            m_IntY;
        uint16_t            m_X;
        uint16_t            m_Y;
        LinearEquation_t    m_X_Correction;
        LinearEquation_t    m_Y_Correction;
        bool                m_IsCalibrated;
        bool                m_IsCorrectionEnable;
};

//-------------------------------------------------------------------------------------------------
