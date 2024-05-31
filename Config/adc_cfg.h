//-------------------------------------------------------------------------------------------------
//
//  File :  adc_cfg.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2021 Alain Royer.
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
// Include(s)
//-------------------------------------------------------------------------------------------------

#include "./Digini/Digini/inc/lib_define.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

// Generate code to support ADC module
#define ADC_DRIVER_SUPPORT_ADC1_CFG             DEF_ENABLED
#define ADC_DRIVER_SUPPORT_ADC2_CFG             DEF_DISABLED
#define ADC_DRIVER_SUPPORT_ADC3_CFG             DEF_DISABLED
#define ADC_COMMON_CLOCK_PRESCALER_CFG          ADC_CLOCK_SYNC_PCLK_DIV_4

//-------------------------------------------------------------------------------------------------
// define here all ADC module to be used and ADC channel to be used

#define ADC_DEF(X_ADC) \
    /*     ENUM ID of the channel,  ADCx,  RCC_APB2ENR_ADCxEN   Resolution               Conversion              Discontinuous Conversion,        Number of channel in discontinuous, Alignment,             EOC Selection,         DMA Continuous Requests          */\
    X_ADC( DRIVER_ADC1_ID,          ADC1,  RCC_APB2ENR_ADC1EN,  ADC_RESOLUTION_12_BITS,  ADC_CONVERSION_SINGLE,  ADC_DISCONTINUOUS_MODE_DISABLE,  ADC_DISCONTINUOUS_NB_CHANNEL_NONE,  ADC_DATA_ALIGN_RIGHT,  ADC_EOC_SELECT_SINGLE, ADC_DMA_CONTINUOUS_REQ_DISABLE )   \

#define ADC_CHANNEL_DEF(X_ADC_CHANNEL) \
    /*             ENUM ID of the channel,      ADC ID Module,  ADC channel number,     IO of the Channel, Sample Time              */\
    X_ADC_CHANNEL( ADC_CHANNEL_INT_VREF,        DRIVER_ADC1_ID, ADC_CHANNEL_VREFINT,    IO_NOT_DEFINED,    ADC_SAMPLE_TIME_28_CYCLES) \
    X_ADC_CHANNEL( ADC_CHANNEL_INT_VBAT,        DRIVER_ADC1_ID, ADC_CHANNEL_VBAT,       IO_NOT_DEFINED,    ADC_SAMPLE_TIME_28_CYCLES) \


//    X_ADC_CHANNEL( ADC_CHANNEL_INT_TEMP,        DRIVER_ADC1_ID, ADC_CHANNEL_TEMPSENSOR, IO_NOT_DEFINED,    ADC_SAMPLE_TIME_28_CYCLES)
//    X_ADC_CHANNEL( ADC_CHANNEL_EXT_TEMPERATURE, DRIVER_ADC1_ID, ADC_CHANNEL_1,       IO_TEMPERATURE,    ADC_SAMPLE_TIME_28_CYCLES)


//-------------------------------------------------------------------------------------------------
