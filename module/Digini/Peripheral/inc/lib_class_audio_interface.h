//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_audio_driver.h
//
//*************************************************************************************************

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

// Codec POWER DOWN modes
#define CODEC_PDWN_HW                 		1
#define CODEC_PDWN_SW                 		2

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class AudioDriverInterface
{
    public:

		virtual SystemState_e 		Initialize				(void*    pArgControl,
				                                             void*    pArgData,
															 uint16_t InOutDevice,
															 uint8_t  InitialVolume)            = 0;
		//virtual SystemState_e 	DeInitialize			(void)                				= 0;
		virtual uint16_t			ReadID					(void)                				= 0;
		virtual SystemState_e		Reset					(void)                				= 0;
		virtual SystemState_e 		Play					(uint16_t* pBuffer, uint16_t Size)  = 0;
		virtual SystemState_e 		Stop					(uint32_t CodecPdwnMode)            = 0;
		virtual SystemState_e 		Pause					(void)                				= 0;
		virtual SystemState_e 		Resume					(void)                				= 0;
		virtual SystemState_e 		SetVolume				(uint8_t Volume)                	= 0;
		virtual SystemState_e 		SetFrequency			(uint32_t AudioFrequency)           = 0;
		virtual SystemState_e 		SetMute					(uint32_t Command)                	= 0;
		virtual SystemState_e 		SetOutputMode			(uint8_t Mode)                		= 0;
		virtual void         		DataPortAccessFunction	(int Function, void* pData)         = 0;
};

//-------------------------------------------------------------------------------------------------

