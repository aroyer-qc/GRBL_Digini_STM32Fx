//-------------------------------------------------------------------------------------------------
//
//  File :  sdio_var.h
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
//
//  Notes : SDIO variables for STM32F7xx
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef SDIO_DRIVER_GLOBAL

#if (USE_SDIO_DRIVER == DEF_ENABLED)
SDIO_Info_t SDIO_Info =
{
    IO_SD_D0,               // SDIO Data 0 to 3
    IO_SD_D1,
    IO_SD_D2,
    IO_SD_D3,
    IO_SD_CLK,              // SDIO CLK
    IO_SD_CMD,				// Command line to SDIO
    IO_DETECT_SD_CARD,      // Card detect line
};
#endif

class SDIO_Driver mySDIO(&SDIO_Info);

#else // SDIO_DRIVER_GLOBAL

extern       SDIO_Info_t    SDIO_Info;
extern class SDIO_Driver    mySDIO;

#endif // SDIO_DRIVER_GLOBAL
