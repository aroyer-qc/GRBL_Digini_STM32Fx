//-------------------------------------------------------------------------------------------------
//
//  File :  crc_cfg.h
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

//#pragma once

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define DIGINI_USE_CRC_8_TO_32_BITS         DEF_ENABLED
#define DIGINI_USE_CRC_MORE_THAN_32_BITS    DEF_DISABLED

// Will be available only if DIGINI_USE_CRC_8_TO_32_BITS is DEF_ENABLED
#define DIGINI_USE_CRC_8                    DEF_DISABLED
#define DIGINI_USE_CRC_8_AUTOSAR            DEF_DISABLED
#define DIGINI_USE_CRC_8_BLUETOOTH          DEF_DISABLED
#define DIGINI_USE_CRC_8_CDMA2000           DEF_DISABLED
#define DIGINI_USE_CRC_8_DARC               DEF_DISABLED
#define DIGINI_USE_CRC_8_DVB_S2             DEF_DISABLED
#define DIGINI_USE_CRC_8_EBU                DEF_DISABLED
#define DIGINI_USE_CRC_8_GSM_A              DEF_DISABLED
#define DIGINI_USE_CRC_8_GSM_B              DEF_DISABLED
#define DIGINI_USE_CRC_8_HITAG              DEF_DISABLED
#define DIGINI_USE_CRC_8_I_CODE             DEF_DISABLED
#define DIGINI_USE_CRC_8_I_432_1            DEF_DISABLED
#define DIGINI_USE_CRC_8_ITU                DEF_DISABLED
#define DIGINI_USE_CRC_8_LTE                DEF_DISABLED
#define DIGINI_USE_CRC_8_MAXIM              DEF_DISABLED
#define DIGINI_USE_CRC_8_MIFARE             DEF_DISABLED
#define DIGINI_USE_CRC_8_NRSC_5             DEF_DISABLED
#define DIGINI_USE_CRC_8_OPENSAFETY         DEF_DISABLED
#define DIGINI_USE_CRC_8_ROHC               DEF_DISABLED
#define DIGINI_USE_CRC_8_SAE_J1850          DEF_ENABLED
#define DIGINI_USE_CRC_8_SMBUS              DEF_DISABLED
#define DIGINI_USE_CRC_8_TECH_3250          DEF_DISABLED
#define DIGINI_USE_CRC_8_WCDMA              DEF_DISABLED
#define DIGINI_USE_CRC_10_ATM               DEF_DISABLED
#define DIGINI_USE_CRC_10_CDMA2000          DEF_DISABLED
#define DIGINI_USE_CRC_10_GSM               DEF_DISABLED
#define DIGINI_USE_CRC_11_FLEXRAY           DEF_DISABLED
#define DIGINI_USE_CRC_11_UMTS              DEF_DISABLED
#define DIGINI_USE_CRC_12_CDMA2000          DEF_DISABLED
#define DIGINI_USE_CRC_12_DECT              DEF_DISABLED
#define DIGINI_USE_CRC_12_GSM               DEF_DISABLED
#define DIGINI_USE_CRC_12_UMTS              DEF_DISABLED
#define DIGINI_USE_CRC_13_BBC               DEF_DISABLED
#define DIGINI_USE_CRC_14_DARC              DEF_DISABLED
#define DIGINI_USE_CRC_14_GSM               DEF_DISABLED
#define DIGINI_USE_CRC_15_CAN               DEF_DISABLED
#define DIGINI_USE_CRC_15_MPT1327           DEF_DISABLED
#define DIGINI_USE_CRC_16_ARC               DEF_DISABLED
#define DIGINI_USE_CRC_16_AUG_CCITT         DEF_DISABLED
#define DIGINI_USE_CRC_16_BUYPASS           DEF_DISABLED
#define DIGINI_USE_CRC_16_CCITT_FALSE       DEF_ENABLED
#define DIGINI_USE_CRC_16_CDMA2000          DEF_DISABLED
#define DIGINI_USE_CRC_16_CMS               DEF_DISABLED
#define DIGINI_USE_CRC_16_DDS_110           DEF_DISABLED
#define DIGINI_USE_CRC_16_DECT_R            DEF_DISABLED
#define DIGINI_USE_CRC_16_DECT_X            DEF_DISABLED
#define DIGINI_USE_CRC_16_DNP               DEF_DISABLED
#define DIGINI_USE_CRC_16_EN_13757          DEF_DISABLED
#define DIGINI_USE_CRC_16_GENIBUS           DEF_DISABLED
#define DIGINI_USE_CRC_16_KERMIT            DEF_DISABLED
#define DIGINI_USE_CRC_16_MAXIM             DEF_DISABLED
#define DIGINI_USE_CRC_16_MCRF4XX           DEF_DISABLED
#define DIGINI_USE_CRC_16_MODBUS            DEF_DISABLED
#define DIGINI_USE_CRC_16_RIELLO            DEF_DISABLED
#define DIGINI_USE_CRC_16_T10_DIF           DEF_DISABLED
#define DIGINI_USE_CRC_16_TELEDISK          DEF_DISABLED
#define DIGINI_USE_CRC_16_TMS37157          DEF_DISABLED
#define DIGINI_USE_CRC_16_USB               DEF_DISABLED
#define DIGINI_USE_CRC_16_X25               DEF_DISABLED
#define DIGINI_USE_CRC_16_XMODEM            DEF_DISABLED
#define DIGINI_USE_CRC_16_A                 DEF_DISABLED
#define DIGINI_USE_CRC_17_CAN_FD            DEF_DISABLED
#define DIGINI_USE_CRC_21_CAN_FD            DEF_DISABLED
#define DIGINI_USE_CRC_24_BLE               DEF_DISABLED
#define DIGINI_USE_CRC_24_FLEXRAY_A         DEF_DISABLED
#define DIGINI_USE_CRC_24_FLEXRAY_B         DEF_DISABLED
#define DIGINI_USE_CRC_24_INTERLAKEN        DEF_DISABLED
#define DIGINI_USE_CRC_24_LTE_A             DEF_DISABLED
#define DIGINI_USE_CRC_24_LTE_B             DEF_DISABLED
#define DIGINI_USE_CRC_24_OPENPGP           DEF_DISABLED
#define DIGINI_USE_CRC_24_OS_9              DEF_DISABLED
#define DIGINI_USE_CRC_30_CDMA              DEF_DISABLED
#define DIGINI_USE_CRC_31_PHILIPS           DEF_DISABLED
#define DIGINI_USE_CRC_32                   DEF_ENABLED
#define DIGINI_USE_CRC_32_AUTOSAR           DEF_DISABLED
#define DIGINI_USE_CRC_32_BZIP2             DEF_DISABLED
#define DIGINI_USE_CRC_32_C                 DEF_DISABLED
#define DIGINI_USE_CRC_32_CD_ROM_EDC        DEF_DISABLED
#define DIGINI_USE_CRC_32_D                 DEF_DISABLED
#define DIGINI_USE_CRC_32_ISO_HDLC          DEF_DISABLED
#define DIGINI_USE_CRC_32_JAMCRC            DEF_DISABLED
#define DIGINI_USE_CRC_32_MEF               DEF_DISABLED
#define DIGINI_USE_CRC_32_MPEG_2            DEF_DISABLED
#define DIGINI_USE_CRC_32_POSIX             DEF_DISABLED
#define DIGINI_USE_CRC_32_Q                 DEF_DISABLED
#define DIGINI_USE_CRC_32_SATA              DEF_DISABLED
#define DIGINI_USE_CRC_32_XFER              DEF_DISABLED

// Will be available only if DIGINI_USE_CRC_MORE_THAN_32_BITS is DEF_ENABLED
#define DIGINI_USE_CRC_40_GSM               DEF_DISABLED
#define DIGINI_USE_CRC_64_ECMA_182          DEF_DISABLED
#define DIGINI_USE_CRC_64_GO_ISO            DEF_DISABLED
#define DIGINI_USE_CRC_64_MS                DEF_DISABLED
#define DIGINI_USE_CRC_64_WE                DEF_DISABLED
#define DIGINI_USE_CRC_64_XZ                DEF_DISABLED

//-------------------------------------------------------------------------------------------------




