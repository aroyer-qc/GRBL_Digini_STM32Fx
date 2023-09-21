//-------------------------------------------------------------------------------------------------
//
//  File : FatFs_cfg.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2023 Alain Royer.
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
// Note(s)
//
//              1 - Include header for each used driver
//              2 - Define DIGINI_FATFS_USE_.....  DEF_ENABLED      all driver to be used
//              3 - User need to call the object method xxx.Configure(...) before the disk can be
//                  use.
//              4 - User can add any custom driver to the list.
//                  Only use tis format:   X_DRIVE( DISK_USER_XX, CFatFS_User_Disk_xx, USER_Disk)
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define FATFS_CFG_MAX_PATH_NAME_LEN                         16
#define FATFS_CFG_MAX_FILENAME_SIZE                         9
#define FATFS_CFG_MAX_EXT_SIZE                              5
#define FATFS_CFG_MAX_FULL_FILENAME_SIZE                    13
#define FATFS_CFG_MAX_FULL_FILENAME_SIZE_PADDING            14

#define _USE_WRITE                                          1       // 1: Enable FatFS disk_write function
#define _USE_IOCTL                                          1       // 1: Enable FatFs disk_ioctl function

//-------------------------------------------------------------------------------------------------
