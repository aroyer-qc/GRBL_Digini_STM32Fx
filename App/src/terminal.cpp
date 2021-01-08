//-------------------------------------------------------------------------------------------------
//
//  File : terminal.cpp
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

#include "terminal.h"
#include "Config.h"
#include "TaskIdle.h"
#include "nOS.h"
#include "lib_class_STM32F7_uart.h"
#include "lib_CQueue.h"
#include "lib_memory.h"
#include "lib_macro.h"
#include "Settings.h"
#include "task_grbl.h"
//#include "GrIP.h"
//#include "Platform.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define TERM_RX_DATA_Q_LENGTH       8
#define TERM_RX_DATA_Q_ITEM_SIZE    (sizeof(uint8_t*) + sizeof(uint16_t))   // size of TERM_QueueDataInfo_t
#define TERM_RX_BUFFER_SIZE         size_t(256)                             // This is the minimal buffer size for RX transmission

//-------------------------------------------------------------------------------------------------
// Typedef(s)
//-------------------------------------------------------------------------------------------------

struct TERM_QueueDataInfo_t
{
    uint8_t* pBuffer;
    size_t  Size;
};

//-------------------------------------------------------------------------------------------------
// Variable(s) and object(s)
//-------------------------------------------------------------------------------------------------

class CQueue    Q_RX_Data;
uint8_t         GetQueueArray[TERM_RX_DATA_Q_LENGTH * TERM_RX_DATA_Q_ITEM_SIZE];      // The array to use as the queue's storage area.

//-------------------------------------------------------------------------------------------------
//
//  Name:           TX callback to free DMA buffer
//
//  Parameter(s):   pContext                Info on buffer to free
//  Return:         void
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
static void TERM_TX_Callback(void* pContext)
{
    TERM_QueueDataInfo_t* pQ_Data;

    pQ_Data = (TERM_QueueDataInfo_t *)pContext;
    pMemory->Free((void**)&pQ_Data->pBuffer);
    pMemory->Free((void**)&pQ_Data);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           RX callback to free DMA buffer
//
//  Parameter(s):   pContext                Info on buffer to send to Q for processing
//  Return:         void
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
static void TERM_RX_Callback(void* pContext)
{
    uint8_t *            pBuffer;
    UART_Variables_t*    pVariable;
    TERM_QueueDataInfo_t Q_Data;


    VAR_UNUSED(pContext);
    pVariable = myUART_Terminal.GetInfoRX();
    Q_Data.Size    = pVariable->SizeRX;
    Q_Data.pBuffer = pVariable->pBufferRX;

    if(Q_Data.Size != 0)
    {
        if(GRBL_RealTimeCommand(*Q_Data.pBuffer) == true)
        {
            pMemory->Free((void**)&Q_Data.pBuffer);
        }
        else
        {
            if(Q_RX_Data.Send(&Q_Data) != true)
            {
                pMemory->Free((void**)&Q_Data.pBuffer);     // Discarded as the controller might stuck doing a job
            }
        }
    }

    if((pBuffer = (uint8_t*)pMemory->Alloc(TERM_RX_BUFFER_SIZE)) != nullptr)
    {
        myUART_Terminal.DMA_ConfigRX(pBuffer, TERM_RX_BUFFER_SIZE);
        return;
    }

    while(1)
    {
        __asm("nop");
    };
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize the low for printf
//
//  Parameter(s):   None
//  Return:         void
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void TERM_Initialize(void)
{
    uint8_t* pBuffer;

    // Create a queue capable of containing 10 uint64_t values.
    Q_RX_Data.CreateQueue(&GetQueueArray[0], TERM_RX_DATA_Q_LENGTH, TERM_RX_DATA_Q_ITEM_SIZE);

    // Register callback for uart RX Bytes and TX Completed
    myUART_Terminal.RegisterCallbackIdle((void*)&TERM_RX_Callback);
    myUART_Terminal.RegisterCallbackCompletedTX((void*)&TERM_TX_Callback);

    // Provide first RX buffer
    pBuffer = (uint8_t*)pMemory->Alloc(TERM_RX_BUFFER_SIZE);
    myUART_Terminal.DMA_ConfigRX(pBuffer, TERM_RX_BUFFER_SIZE);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:
//
//  Parameter(s):
//
//  Return:
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
int8_t Getc(char *c)
{
    static TERM_QueueDataInfo_t Q_DataRX;
    static bool                 Processing = false;
    static uint32_t             Pointer    = 0;

    if(Processing == false)
    {
         if(Q_RX_Data.Receive(&Q_DataRX, NOS_NO_WAIT) == true)
         {
            Pointer      = 0;
            Processing   = true;
         }
    }

    // do we have something to process?
    if(Processing == true)
    {
        *c = *(Q_DataRX.pBuffer + Pointer);
        Pointer++;

        if(Q_DataRX.Size == Pointer)     // this buffer has been extracted, free the block
        {
            Processing = false;
            pMemory->Free((void**)&Q_DataRX.pBuffer);
        }

        return 0;
    }

    return -1;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:
//
//  Parameter(s):
//
//  Return:
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
int Putc(const char c)
{
    printf("%c", c);
    return 0;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:
//
//  Parameter(s):
//
//  Return:
//
//  Description:    Convert float to string by immediately converting to a long integer, which
//                  contains  more digits than a float. Number of decimal places, which are tracked
//                  by a counter, may be set by the user. The integer is then efficiently converted
//                  to a string.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void printFloat(float n, uint8_t decimal_places)
{
    if(n < 0) {
        Putc('-');
        n = -n;
    }

    uint8_t decimals = decimal_places;

    while(decimals >= 2) { // Quickly convert values expected to be E0 to E-4.
        n *= 100;
        decimals -= 2;
    }

    if(decimals) {
        n *= 10;
    }
    n += 0.5; // Add rounding factor. Ensures carryover through entire value.

    // Generate digits backwards and store in string.
    unsigned char buf[13];
    uint8_t i = 0;
    uint32_t a = (long)n;

    while(a > 0) {
        buf[i++] = (a % 10) + '0'; // Get digit
        a /= 10;
    }

    while(i < decimal_places) {
        buf[i++] = '0'; // Fill in zeros to decimal point for (n < 1)
    }

    if(i == decimal_places) { // Fill in leading zero, if needed.
        buf[i++] = '0';
    }

    // Print the generated string.
    for(; i > 0; i--) {
        if(i == decimal_places) {
            Putc('.');
        } // Insert decimal point in right place.
        Putc(buf[i-1]);
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:
//
//  Parameter(s):
//
//  Return:
//
//  Description:        Floating value printing handlers for special variables types used in Grbl
//                      and are defined in the config.h.
//
//  Note(s):            - CoordValue: Handles all position or coordinate values in inches or mm reporting.
//                      - RateValue: Handles feed rate and current velocity in inches or mm reporting.
//
//-------------------------------------------------------------------------------------------------
void printFloat_CoordValue(float n)
{
    if(bit_istrue(settings.flags, BITFLAG_REPORT_INCHES)) {
        //PrintFloat(n*INCH_PER_MM, N_DECIMAL_COORDVALUE_INCH);
        printf("%." N_DECIMAL_COORD_INCH_STR "f", n * INCH_PER_MM);
    }
    else {
        //PrintFloat(n, N_DECIMAL_COORDVALUE_MM);
        printf("%." N_DECIMAL_COORD_MM_STR "f", n);
    }
}


void printFloat_RateValue(float n)
{
    if(bit_istrue(settings.flags, BITFLAG_REPORT_INCHES)) {
        //PrintFloat(n*INCH_PER_MM,N_DECIMAL_RATEVALUE_INCH);
        printf("%." N_DECIMAL_RATE_INCH_STR "f", n * INCH_PER_MM);
    }
    else {
        //PrintFloat(n, N_DECIMAL_RATEVALUE_MM);
        printf("%." N_DECIMAL_RATE_MM_STR "f", n);
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Redirection function to use printf().
//
//  Parameter(s):   File        Not used
//                  pBuf        To buffer to be debugged out
//                  Length      Size of data in buffer
//  Return:         0
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
#if defined(__GNUC__)
extern "C" int _write(int File, char* pBuf, int Length)
//int _write(int File, char *pBuf, int Length)
#else // defined(__ICCARM__)
__ATTRIBUTES
extern size_t __write(int File, const unsigned char *pBuf, size_t Length);
size_t __write(int File, const unsigned char *pBuf, size_t Length)
#endif // __GNUC__ or __ICCARM__
{
    TERM_QueueDataInfo_t* pQ_Data;
    bool                  Success = false;

    VAR_UNUSED(File);

  #ifdef ETH_IF
    Pdu_t data;

    data.Data = (uint8_t*)buf;
    data.Length = buf_idx;

    uint8_t ret = GrIP_Transmit(MSG_DATA_NO_RESPONSE, 0, &data);
    (void)ret;  // TODO: Handle transmit error
  #else
    if(Length != 0)
    {
        if((pQ_Data = (TERM_QueueDataInfo_t*)pMemory->Alloc(sizeof(TERM_QueueDataInfo_t))) != nullptr)
        {
            if((pQ_Data->pBuffer = (uint8_t*)pMemory->Alloc(Length)) != nullptr)
            {
                memcpy(pQ_Data->pBuffer, pBuf, Length);
                pQ_Data->Size = Length;

                while(myUART_Terminal.SendData(pQ_Data->pBuffer, &pQ_Data->Size, pQ_Data) != SYS_READY)
                {
                    nOS_Yield();
                };
                Success = true;

                // TODO add time limit to send stuff
            }

            if(Success == false)
            {
                pMemory->Free((void**)&pQ_Data->pBuffer);
                pMemory->Free((void**)&pQ_Data);
            }
        }
        else
        {
            pMemory->Free((void**)&pQ_Data);

        }

      #if DEBUG     // trap... this should not happened
        if(Success == false)
        {
            while(1)
            {
                __asm("nop");
            }
        }
      #endif
    }
  #endif

    // Return number of sent bytes
    return Length;
}

