//-------------------------------------------------------------------------------------------------
//
//  File :  cli_cfg.h
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
// Define(s)
//-------------------------------------------------------------------------------------------------

#define CLI_USE_PASSWORD                    DEF_DISABLED              // Command password support
#define CLI_USE_EXTENDED_ERROR              DEF_ENABLED               // AT return a more informative error
#define CLI_STRING_SIZE                     32
#define CLI_NUMBER_OF_SUPPORTED_PARAM       4
#define CLI_NUMBER_OF_SECOND_FOR_STARTUP    10000                     // Startup last 10 Seconds
#define CLI_CMD_TIME_OUT                    5000                      // User has 5 seconds between character input while inputing an AT command

// Number of parameter can be expanded, as needed
#define X_CLI_CMD_DEF(X_CLI_CMD)   \
/*              ENUM_ID,        String,       Function,     Cmd Type,    Number,    Param1 -             Min1,    max1   Param2 -          Min2,   max2,     Param3 -              Min3,   max3,     Param4 -          Min4,   max4  */\
    X_CLI_CMD ( AT_HOLD,        "H",          CmdHOLD,      CLI_CMD_SP,  0,         0,                    0,       0,     0,                0,      0,        0,                    0,      0,        0,                0,      0)     \
    X_CLI_CMD ( AT_RELEASE,     "R",          CmdRELEASE,   CLI_CMD_HP,  0,         0,                    0,       0,     0,                0,      0,        0,                    0,      0,        0,                0,      0)     \
    X_CLI_CMD ( AT_VERSION,     "V",          CmdVERSION,   CLI_CMD_P,   0,         0,                    0,       0,     0,                0,      0,        0,                    0,      0,        0,                0,      0)     \
    X_CLI_CMD ( AT_DEBUG,       "DBG",        CmdDBG_LEVEL, CLI_CMD_RW,  1,         CLI_BASE_HEXADECIMAL, 0x00,    0xFF,  0,                0,      0,        0,                    0,      0,        0,                0,      0)     \
/* TODO generic command should be in the library and be enable by define */\
\
    X_CLI_CMD ( AT_INFO,        "I",          CmdINFO,      CLI_CMD_P,   0,         0,                    0,       0,     0,                0,      0,        0,                    0,      0,        0,                0,      0)     \
    X_CLI_CMD ( AT_RESET,       "RESET",      CmdRESET,     CLI_CMD_P,   0,         0,                    0,       0,     0,                0,      0,        0,                    0,      0,        0,                0,      0)     \
    X_CLI_CMD ( AT_STATUS,      "S",          CmdSTATUS,    CLI_CMD_P,   0,         0,                    0,       0,     0,                0,      0,        0,                    0,      0,        0,                0,      0)     \
    X_CLI_CMD ( AT_TEST1,       "T1",         CmdTEST1,     CLI_CMD_RW,  0,         0,                    0,       0,     0,                0,      0,        0,                    0,      0,        0,                0,      0)     \
    X_CLI_CMD ( AT_TEST2,       "T2",         CmdTEST2,     CLI_CMD_RW,  3,         CLI_BASE_DECIMAL,     0,       1,     CLI_BASE_DECIMAL, 0,      200,      CLI_BASE_DECIMAL,     10,     32000,    0,                0,      0)     \
    X_CLI_CMD ( AT_TEST3,       "T3",         CmdTEST3,     CLI_CMD_RW,  4,         CLI_BASE_DECIMAL,     0,       3,     CLI_BASE_DECIMAL, -300,   300,      CLI_BASE_HEXADECIMAL, 0x0000, 0xFFFF,   CLI_BASE_STRING,  0,      0)     \

//-------------------------------------------------------------------------------------------------




