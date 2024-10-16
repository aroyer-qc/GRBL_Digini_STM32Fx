//-------------------------------------------------------------------------------------------------
//
//  File : terminal.h
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

#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

void            printFloat              (float n, uint8_t decimal_places);
int8_t          Getc                    (char *c);
int             Putc                    (const char c);
void            printFloat_CoordValue   (float n);
void            printFloat_RateValue    (float n);
extern "C" int  _write                  (int File, char* pBuf, int Length);


class GRBL_Serial : public ChildProcessInterface
{
    public:

   public:
                            GRBL_Serial                 () {};

        void                IF_Process                  (void);
        nOS_Error           Initialize                  (Console* pConsole);
        bool                RealTimeCommand             (char RealTimeCmd);

    private:

        void                ProcessRX                   (void);


};

//-------------------------------------------------------------------------------------------------
