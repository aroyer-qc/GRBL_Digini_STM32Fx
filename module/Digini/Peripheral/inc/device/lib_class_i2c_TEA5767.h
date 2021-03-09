//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_i2c_TEA5767.h
//
//  Library header file for the radio library to control the TEA5767 radio chip.
//
//*************************************************************************************************

#pragma once

//-------------------------------------------------------------------------------------------------
// device definition(s)
//-------------------------------------------------------------------------------------------------

#ifdef CFG_DEVICE_DEFINITION

#define TEA5767_DEFINITION X_I2C_DEVICE(I2C_DEVICE_TEA5767, "Radio", MUTEX, 0x60, sizeof(uint8_t), 100000)

#else // CFG_DEVICE_DEFINITION

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
//#include <stdint.h>
//#include <stdbool.h>
#include "lib_class_i2c.h"


//-------------------------------------------------------------------------------------------------
// typedef(s)
//-------------------------------------------------------------------------------------------------

// Frequency data type.
typedef uint16_t TEA5767_Freq_t;        // Only 16 bits are used for any frequency value (not the real one)

// A structure that contains information about the radio features from the chip.
struct TEA5767_Info_t
{
    bool    Active;                     // Receiving is active.
    uint8_t RSSI;                       // Radio Station Strength Information.
    uint8_t SNR;                        // Signal Noise Ratio.
    bool    RDS;                        // RDS information is available.
    bool    Tuned;                      // A stable frequency is tuned.
    bool    Mono;                       // Mono mode is on.
    bool    Stereo;                     // Stereo audio is available
    bool    Mute;
};

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class TEA5767
{
    public:

                            TEA5767                    (CI2C* pI2C);
                            ~TEA5767                ();

        SystemState_e       Initialize                (void);                     // Initialize the chip.
        void                SetMono                 (bool SwitchOn);            // Control mono/stereo mode of the radio chip
        void                SetMute                 (bool SwitchOn);            // Control the mute function of the radio chip
        void                SetFrequency            (TEA5767_Freq_t newF);
        TEA5767_Freq_t        GetFrequency            (void);
        void                SeekUp                  (bool toNextSender){};      // Start seek mode upwards
        void                SeekDown                (bool toNextSender){};      // Start seek mode downwards
        void                CheckRDS                (void);                     // Read RDS data from the current station and process when data available.
        void                GetRadioInfo            (TEA5767_Info_t *info);

    private:

        void                  ReadRegisters            (void);                      // Read all status & data registers
        void                  SaveRegisters            (void);                      // Save writable registers back to the chip


        CI2C*               m_pI2C;
        I2C_DeviceInfo_t*   m_pDevice;

        uint8_t             m_Registers[5];                                     // Store the current values of the registers for controlling the radio chip.
        uint8_t             m_Status[5];                                        // Registers with the current status of the radio chip.
        bool                m_Mono;
        bool                m_Mute;
        TEA5767_Freq_t      m_Freq;

        // const
        const uint16_t        m_freqLow     = 8700;
        const uint16_t       m_freqHigh     = 10800;
        const uint16_t       m_freqSteps = 10;
};



//void  TEA5767_debugScan();               // Scan all frequencies and report a status

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

extern class   TEA5767                     RADIO_TEA5767;

#ifdef LIB_TEA5767_GLOBAL
 class   TEA5767                        RADIO_TEA5767;
#endif

//-------------------------------------------------------------------------------------------------



