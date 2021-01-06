//-------------------------------------------------------------------------------------------------
//
//  File : assert.cpp
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

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>

#include "assert.h"
#include "lib_macro.h"
#include "lib_typedef.h"

//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------

uint32_t*   Assert_pFaultStackAddress;
uint32_t    Assert_r0;
uint32_t    Assert_r1;
uint32_t    Assert_r2;
uint32_t    Assert_r3;
uint32_t    Assert_r12;
uint32_t    Assert_lr;      // Link register.
uint32_t    Assert_pc;      // Program counter.
uint32_t    Assert_psr;     // Program status register.

//-------------------------------------------------------------------------------------------------
// Prototype(s)
//-------------------------------------------------------------------------------------------------

void HardFault_Handler      (void);
void HardFault_Handler_C    (uint32_t* FpaultStackAddress);

//-------------------------------------------------------------------------------------------------
//
// Name:           assert_failed
// Parameter(s):   const char* file    Pointer to the source file name
//                 int line            Assert_param error line source number
//
// Return:         void
//
// Description:    Reports the name of the source file and the source line number
//                 Where the assert_param error has occurred.
//
// Note(s):
//
//-------------------------------------------------------------------------------------------------
//#if defined( DEBUG ) && defined( USE_FULL_ASSERT )
void assert_failed(const char* file, int line)
{
    static char* pFile;
    static int Line;

    pFile = (char*)file;
    Line  = line;

    // User can add his own implementation to report the file name and line number
    // ex: printf("Wrong Parameters value: file %s on line %u\r\n", file, line)
    // printf("Failed in file %s on line %lu\r\n", file, line);

    VAR_UNUSED(*pFile);
    VAR_UNUSED(Line);

     // Infinite loop
    while(1) {};
}
//#endif

//-------------------------------------------------------------------------------------------------

void HardFault_Handler(void)
{
    __asm volatile
    (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word HardFault_Handler_C         \n"
    );
}

//-------------------------------------------------------------------------------------------------

void HardFault_Handler_C(uint32_t* pFaultStackAddress)
{
    Assert_pFaultStackAddress = pFaultStackAddress;
    Assert_r0  = pFaultStackAddress[0];
    Assert_r1  = pFaultStackAddress[1];
    Assert_r2  = pFaultStackAddress[2];
    Assert_r3  = pFaultStackAddress[3];
    Assert_r12 = pFaultStackAddress[4];
    Assert_lr  = pFaultStackAddress[5];
    Assert_pc  = pFaultStackAddress[6];
    Assert_psr = pFaultStackAddress[7];

    // When the following line is hit, the variables contain the register values.
    for( ;; )
    {
        __asm("nop");
    };
}

//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
