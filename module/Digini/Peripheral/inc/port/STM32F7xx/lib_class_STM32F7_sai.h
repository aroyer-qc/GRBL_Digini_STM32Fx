//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_sai.h
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
#include "stm32f7xx.h"
#include "nOS.h"
#include "lib_typedef.h"
#include "lib_class_gpio.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define SAIx_CLK_SOURCE_PLLI2S                        0
#define SAIx_CLK_SOURCE_PLLSAI                        1

// SAI CR1 reset value
#define SAI_RESET_VALUE                                ((uint32_t)0x00000000)

// SAI Block Mode
#define SAI_CR1_MODE_MASTER_TX                         ((uint32_t)0x00000000)
#define SAI_CR1_MODE_MASTER_RX                         ((uint32_t)SAI_xCR1_MODE_0)
#define SAI_CR1_MODE_SLAVE_TX                          ((uint32_t)SAI_xCR1_MODE_1)
#define SAI_CR1_MODE_SLAVE_RX                          ((uint32_t)(SAI_xCR1_MODE_1 | SAI_xCR1_MODE_0))

// SAI Block Protocol
#define SAI_CR1_FREE_PROTOCOL                         ((uint32_t)0x00000000)
#define SAI_CR1_SPDIF_PROTOCOL                        ((uint32_t)SAI_xCR1_PRTCFG_0)
#define SAI_CR1_AC97_PROTOCOL                         ((uint32_t)SAI_xCR1_PRTCFG_1)

// SAI Block Data Size
#define SAI_CR1_DATA_SIZE_8                         ((uint32_t)SAI_xCR1_DS_1)
#define SAI_CR1_DATA_SIZE_10                             ((uint32_t)(SAI_xCR1_DS_1 | SAI_xCR1_DS_0))
#define SAI_CR1_DATA_SIZE_16                        ((uint32_t)SAI_xCR1_DS_2)
#define SAI_CR1_DATA_SIZE_20                        ((uint32_t)(SAI_xCR1_DS_2 | SAI_xCR1_DS_0))
#define SAI_CR1_DATA_SIZE_24                        ((uint32_t)(SAI_xCR1_DS_2 | SAI_xCR1_DS_1))
#define SAI_CR1_DATA_SIZE_32                        ((uint32_t)(SAI_xCR1_DS_2 | SAI_xCR1_DS_1 | SAI_xCR1_DS_0))

// SAI Block MSB LSB transmission
#define SAI_CR1_FIRST_BIT_MSB                          ((uint32_t)0x00000000)
#define SAI_CR1_FIRST_BIT_LSB                          ((uint32_t)SAI_xCR1_LSBFIRST)

// SAI Block Clock Strobing
#define SAI_CR1_CLOCK_STROBING_RISING_EDGE          ((uint32_t)0x00000000)
#define SAI_CR1_CLOCK_STROBING_FALLING_EDGE         ((uint32_t)SAI_xCR1_CKSTR)

// SAI Block Synchronization
#define SAI_CR1_ASYNCHRONOUS                        ((uint32_t)0x00000000)
#define SAI_CR1_SYNCHRONOUS                         ((uint32_t)SAI_xCR1_SYNCEN_0)
#define SAI_CR1_SYNCHRONOUS_EXT_SAI1                ((uint32_t)SAI_xCR1_SYNCEN_1)
#define SAI_CR1_SYNCHRONOUS_EXT_SAI2                    ((uint32_t)SAI_xCR1_SYNCEN_1)

//SAI Mono Stereo Mode
#define SAI_CR1_STEREO_MODE                           ((uint32_t)0x00000000)
#define SAI_CR1_MONO_MODE                             ((uint32_t)SAI_xCR1_MONO)

// SAI Block Output Drive
#define SAI_CR1_OUTPUT_DRIVE_DISABLE                  ((uint32_t)0x00000000)
#define SAI_CR1_OUTPUT_DRIVE_ENABLE                   ((uint32_t)SAI_xCR1_OUTDRIV)

// SAI Block NoDivider
#define SAI_CR1_MASTER_DIVIDER_ENABLE                 ((uint32_t)0x00000000)
#define SAI_CR1_MASTER_DIVIDER_DISABLE               ((uint32_t)SAI_xCR1_NODIV)

// SAI Block Fifo Threshold
#define SAI_CR2_FIFO_THRESHOLD_EMPTY                  ((uint32_t)0x00000000)
#define SAI_CR2_FIFO_THRESHOLD_1QF                    ((uint32_t)(SAI_xCR2_FTH_0))
#define SAI_CR2_FIFO_THRESHOLD_HF                     ((uint32_t)(SAI_xCR2_FTH_1))
#define SAI_CR2_FIFO_THRESHOLD_3QF                    ((uint32_t)(SAI_xCR2_FTH_1 | SAI_xCR2_FTH_0))
#define SAI_CR2_FIFO_THRESHOLD_FULL                   ((uint32_t)(SAI_xCR2_FTH_2))

// SAI Block Fifo Flush
#define SAI_CR2_FIFO_NO_FLUSH                        ((uint32_t)0x00000000)
#define SAI_CR2_FIFO_FLUSH                          ((uint32_t)(SAI_xCR2_FFLUSH))

// SAI Block Companding mode
#define SAI_CR2_NO_COMPANDING                        ((uint32_t)0x00000000)
#define SAI_CR2_COMPANDING_U_LAW_1CPL               ((uint32_t)SAI_xCR2_COMP_1)
#define SAI_CR2_COMPANDING_A_LAW_1CPL                ((uint32_t)SAI_xCR2_COMP_1 | SAI_xCR2_COMP_0)
#define SAI_CR2_COMPANDING_U_LAW_2CPL                 ((uint32_t)SAI_xCR2_COMP_1 | SAI_xCR2_CPL)
#define SAI_CR2_COMPANDING_A_LAW_2CPLG                 ((uint32_t)SAI_xCR2_COMP_1 | SAI_xCR2_COMP_0 | SAI_xCR2_CPL)

// SAI Block companding mode
#define SAI_CR2_SD_OUTPUT_DRIVEN                    ((uint32_t)0x00000000)
#define SAI_CR2_SD_OUTPUT_RELEASE                   ((uint32_t)SAI_xCR2_TRIS)

// SAI Block FS Definition
#define SAI_FRCR_FS_START_FRAME                     ((uint32_t)0x00000000)
#define SAI_FRCR_FS_CHANNEL_IDENTIFICATION          ((uint32_t)SAI_xFRCR_FSDEF)

// SAI Block FS Polarity
#define SAI_FRCR_FS_ACTIVE_LOW                      ((uint32_t)0x00000000)
#define SAI_FRCR_FS_ACTIVE_HIGH                     ((uint32_t)SAI_xFRCR_FSPOL)

// SAI Block FS Offset
#define SAI_FRCR_FS_FIRST_BIT                       ((uint32_t)0x00000000)
#define SAI_FRCR_FS_BEFORE_FIRST_BIT                ((uint32_t)SAI_xFRCR_FSOFF)

//e SAI Block Slot Size
#define SAI_SLOTR_SLOT_SIZE_DATA_SIZE                  ((uint32_t)0x00000000)
#define SAI_SLOTR_SLOT_SIZE_16B                        ((uint32_t)SAI_xSLOTR_SLOTSZ_0)
#define SAI_SLOTR_SLOT_SIZE_32B                        ((uint32_t)SAI_xSLOTR_SLOTSZ_1)


// SAI Audio Frequency
#define SAI_AUDIO_FREQUENCY_192K                  ((uint32_t)192000)
#define SAI_AUDIO_FREQUENCY_96K                   ((uint32_t)96000)
#define SAI_AUDIO_FREQUENCY_48K                   ((uint32_t)48000)
#define SAI_AUDIO_FREQUENCY_44K                   ((uint32_t)44100)
#define SAI_AUDIO_FREQUENCY_32K                   ((uint32_t)32000)
#define SAI_AUDIO_FREQUENCY_22K                   ((uint32_t)22050)
#define SAI_AUDIO_FREQUENCY_16K                   ((uint32_t)16000)
#define SAI_AUDIO_FREQUENCY_11K                   ((uint32_t)11025)
#define SAI_AUDIO_FREQUENCY_8K                    ((uint32_t)8000)
#define SAI_AUDIO_FREQUENCY_MCKDIV                ((uint32_t)0)


// SAI External synchronization
#define SAI_CR1_EXTERNAL_SYNC_DISABLE                  0
#define SAI_CR1_EXTERNAL_SYNC_OUT_BLOCK_A_ENABLE     1
#define SAI_CR1_EXTERNAL_SYNC_OUT_BLOCK_B_ENABLE    2


// SAI Block Slot Active
#define SAI_SLOT_NOT_ACTIVE                       ((uint32_t)0x00000000)
#define SAI_SLOT_ACTIVE_0                         ((uint32_t)0x00010000)
#define SAI_SLOT_ACTIVE_1                         ((uint32_t)0x00020000)
#define SAI_SLOT_ACTIVE_2                         ((uint32_t)0x00040000)
#define SAI_SLOT_ACTIVE_3                         ((uint32_t)0x00080000)
#define SAI_SLOT_ACTIVE_4                         ((uint32_t)0x00100000)
#define SAI_SLOT_ACTIVE_5                         ((uint32_t)0x00200000)
#define SAI_SLOT_ACTIVE_6                         ((uint32_t)0x00400000)
#define SAI_SLOT_ACTIVE_7                         ((uint32_t)0x00800000)
#define SAI_SLOT_ACTIVE_8                         ((uint32_t)0x01000000)
#define SAI_SLOT_ACTIVE_9                         ((uint32_t)0x02000000)
#define SAI_SLOT_ACTIVE_10                        ((uint32_t)0x04000000)
#define SAI_SLOT_ACTIVE_11                        ((uint32_t)0x08000000)
#define SAI_SLOT_ACTIVE_12                        ((uint32_t)0x10000000)
#define SAI_SLOT_ACTIVE_13                        ((uint32_t)0x20000000)
#define SAI_SLOT_ACTIVE_14                        ((uint32_t)0x40000000)
#define SAI_SLOT_ACTIVE_15                        ((uint32_t)0x80000000)
#define SAI_SLOT_ACTIVE_ALL                       ((uint32_t)0xFFFF0000)

// To have 2 separate audio stream in Both headphone and speaker the 4 slot must be activated
#define CODEC_AUDIO_FRAME_SLOT_0123               SAI_SLOT_ACTIVE_0 | SAI_SLOT_ACTIVE_1 | SAI_SLOT_ACTIVE_2 | SAI_SLOT_ACTIVE_3
// To have an audio stream in headphone only SAI Slot 0 and Slot 2 must be activated
#define CODEC_AUDIO_FRAME_SLOT_02                 SAI_SLOT_ACTIVE_0 | SAI_SLOT_ACTIVE_2
// To have an audio stream in speaker only SAI Slot 1 and Slot 3 must be activated
#define CODEC_AUDIO_FRAME_SLOT_13                 SAI_SLOT_ACTIVE_1 | SAI_SLOT_ACTIVE_3


#define SAI_FUNCTION_SET_AUDIO_FRAME_SLOT        0x00

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

typedef struct
{
    uint32_t             AudioMode;
    uint32_t            Synchro;
    uint32_t            ExternalSynchro;
    uint32_t             OutputDrive;    
    uint32_t             NoDivider;      
    uint32_t             FIFOThreshold;  
    uint32_t             AudioFrequency; 
    uint32_t             Mckdiv;        
    uint32_t             MonoStereoMode;
    uint32_t             CompandingMode;
    uint32_t             TriState;      
    uint32_t             Protocol;      
    uint32_t             DataSize;      
    uint32_t             FirstBit;      
    uint32_t             ClockStrobing;

    // Frame config
    uint32_t             FrameLength;
    uint32_t             ActiveFrameLength;
      uint32_t             FSDefinition;
    uint32_t             FSPolarity;
    uint32_t             FSOffset;

    // Slot config
    uint32_t             FirstBitOffset;
    uint32_t             SlotSize;
    uint32_t             SlotNumber;
    uint32_t             SlotActive;

} SAI_PortConfig_t;

typedef struct
{
    SAI_TypeDef*            pSAIx;
    SAI_Block_TypeDef*      pSAI_BlockOut;
//    SAI_Block_TypeDef*      pSAI_BlockIn;
    DMA_Stream_TypeDef*     pDMA;
   // uint32_t                Clock;
} SAI_PortInfo_t;

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class SAI_Driver
{
    public:

                          SAI_Driver            (const SAI_PortInfo_t* pPort);

        SystemState_e     GetStatus               (void);
        void              SetFrequency          (uint32_t AudioFrequency);
        uint32_t          GetFrequency          (void);
        void              SetAudioFrameSlot        (uint32_t AudioFrameSlot);
        SystemState_e     Transmit_DMA          (uint8_t* pData, uint16_t Size);
        void               StopDMA                (void);

    private:

        SAI_PortInfo_t*                 m_pPort;
        SystemState_e                    m_Status;
        uint32_t                        m_AudioFrequency;
};
//-------------------------------------------------------------------------------------------------



