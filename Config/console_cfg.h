//-------------------------------------------------------------------------------------------------
//
//  File :  console_cfg.h
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

#define CON_CHILD_PROCESS_PUSH_POP_LEVEL        2   // Cascading number of child process.


// This define if enabled will trap every line starting with CON_TRAP_COMMENT_CHARACTER so the peer
// module connected to the serial port can send information for watching user or debugging purpose.
// By enabling the feature the code will not parse a line with CON_TRAP_COMMENT_CHARACTER until the
// CON_TRAP_COMMENT_END_OF_LINE_MARKER is detected.
#define CON_TRAP_COMMENT_INCOMING_LINE          DEF_ENABLED
#define CON_TRAP_COMMENT_CHARACTER              '#'
#define CON_TRAP_COMMENT_END_OF_LINE_MARKER     '\n'
#define CON_TRAP_COMMENT_TIME_OUT               50

// TODO is it the best place to put this....???

//#define CON_DEBUG_LEVEL_1 to 16
#define CON_DEBUG_LEVEL_SYSTEM_STATUS           CON_DEBUG_LEVEL_1
#define CON_DEBUG_LEVEL_SYSTEM_ACTION           CON_DEBUG_LEVEL_2
#define CON_DEBUG_LEVEL_SYSTEM_HEALTH           CON_DEBUG_LEVEL_3
#define CON_DEBUG_LEVEL_MONITOR_LOGS            CON_DEBUG_LEVEL_4
#define CON_DEBUG_LEVEL_STACK_WARNING           CON_DEBUG_LEVEL_5
#define CON_DEBUG_LEVEL_ETHERNET                CON_DEBUG_LEVEL_6
#define CON_DEBUG_LEVEL_LWIP                    CON_DEBUG_LEVEL_7
#define CON_DEBUG_LEVEL_MEMORY_POOL             CON_DEBUG_LEVEL_8

//-------------------------------------------------------------------------------------------------




