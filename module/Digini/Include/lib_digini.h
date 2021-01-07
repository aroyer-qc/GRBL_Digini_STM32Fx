//-------------------------------------------------------------------------------------------------
//
//  File : lib_digini.h
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

#include <stdint.h>
#include <stdbool.h>
#include "digini_cfg.h"

#include "lib_typedef.h"
#include "assert.h"
#include "lib_io.h"

//#ifdef DIGINI_USE_RTC
//#include "lib_class_rtc.h"
//#endif

//#ifdef DIGINI_USE_USB
//#include "lib_class_usb.h"
//#endif

//#ifdef DIGINI_USE_FATFS
//#include "ff.h"
//#include "lib_fatfs_disk.h"
//#endif

#ifdef DIGINI_USE_QSPI
 #include "lib_class_qspi.h"
#endif


#include "lib_memory.h"
#include "lib_label.h"

//-------------------------------------------------------------------------------------------------
// Functions prototypes for service call processing
//
// Note: function available if grafx is enable
//-------------------------------------------------------------------------------------------------

SystemState_e       DIGINI_Initialize       (void);
SystemState_e       DIGINI_PostInitialize   (void);

//-------------------------------------------------------------------------------------------------
