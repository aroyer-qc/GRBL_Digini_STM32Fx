//-------------------------------------------------------------------------------------------------
//
//  File : terminal.cpp
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

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "terminal.h"
#include "Config.h"
#include "./Digini/lib_digini.h"
#include "Settings.h"
#include "task_grbl.h"

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

class CQueue    Q_RX_Data;
uint8_t         GetQueueArray[TERM_RX_DATA_Q_LENGTH * TERM_RX_DATA_Q_ITEM_SIZE];      // The array to use as the queue's storage area.

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
// todo do my own version incorporated into digini and format.. if necessary

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
    if(BIT_IS_TRUE(Settings.flags, BITFLAG_REPORT_INCHES)) {
        //PrintFloat(n*INCH_PER_MM, N_DECIMAL_COORDVALUE_INCH);
        printf("%.4f", n * INCH_PER_MM);
    }
    else {
        //PrintFloat(n, N_DECIMAL_COORDVALUE_MM);
        printf("%.3f", n);
    }
}


void printFloat_RateValue(float n)
{
    if(BIT_IS_TRUE(Settings.flags, BITFLAG_REPORT_INCHES)) {
        //PrintFloat(n*INCH_PER_MM,N_DECIMAL_RATEVALUE_INCH);
        printf("%.4f", n * INCH_PER_MM);
    }
    else {
        //PrintFloat(n, N_DECIMAL_RATEVALUE_MM);
        printf("%.3f", n);
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

// use digini console child process instead
#if 0
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
        if((pQ_Data = (TERM_QueueDataInfo_t*)pMemoryPool->Alloc(sizeof(TERM_QueueDataInfo_t))) != nullptr)
        {
            if((pQ_Data->pBuffer = (uint8_t*)pMemoryPool->Alloc(Length)) != nullptr)
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
                pMemoryPool->Free((void**)&pQ_Data->pBuffer);
                pMemoryPool->Free((void**)&pQ_Data);
            }
        }
        else
        {
            pMemoryPool->Free((void**)&pQ_Data);

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
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GRBL_Serial::Initialize(void)
{
    uint8_t* pBuffer;

    // Create a queue capable of containing 10 uint64_t values.
    Q_RX_Data.CreateQueue(&GetQueueArray[0], TERM_RX_DATA_Q_LENGTH, TERM_RX_DATA_Q_ITEM_SIZE);

    // Register callback for uart RX Bytes and TX Completed
    // TODO match with new class register type or something   myUART_Terminal.RegisterCallbackIdle((void*)&TERM_RX_Callback);
  // myUART_Terminal.RegisterCallbackCompletedTX((void*)&TERM_TX_Callback);

    // Provide first RX buffer
    pBuffer = (uint8_t*)pMemoryPool->Alloc(TERM_RX_BUFFER_SIZE);
    myUART_Terminal.DMA_ConfigRX(pBuffer, TERM_RX_BUFFER_SIZE);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ProcessRX
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GRBL_Serial::ProcessRX(void)
{
    uint8_t *            pBuffer;
    UART_Variables_t*    pVariable;
    TERM_QueueDataInfo_t Q_Data;

    // See cli or vt100 for example

    pVariable = myUART_Terminal.GetInfoRX();
    Q_Data.Size    = pVariable->SizeRX;
    Q_Data.pBuffer = pVariable->pBufferRX;

    if(Q_Data.Size != 0)
    {
        if(GRBL_RealTimeCommand(*Q_Data.pBuffer) == true)
        {
            pMemoryPool->Free((void**)&Q_Data.pBuffer);
        }
        else
        {
            if(Q_RX_Data.Send(&Q_Data) != true)
            {
                pMemoryPool->Free((void**)&Q_Data.pBuffer);     // Discarded as the controller might stuck doing a job
            }
        }
    }

    if((pBuffer = (uint8_t*)pMemoryPool->Alloc(TERM_RX_BUFFER_SIZE)) != nullptr)
    {
        myUART_Terminal.DMA_ConfigRX(pBuffer, TERM_RX_BUFFER_SIZE);
        return;
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           RealTimeCmd
//
//  Parameter(s):   RealTimeCommand             Character to be check
//  Return:         IsItProcessed               - true      If character was a real time command
//                                              - false     If not a valid real time command
//
//  Description:    Interpreter for GRBL real time command
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
bool GRBL_Serial::RealTimeCmd(char RealTimeCommand)
{
    bool IsItRealTime = true;

    switch(RealTimeCommand)
    {
        case CMD_RESET:         MC_Reset();                                  break; // Call motion control reset routine.
        case CMD_STATUS_REPORT: System_SetExecStateFlag(EXEC_STATUS_REPORT); break; // Set as true
        case CMD_CYCLE_START:   System_SetExecStateFlag(EXEC_CYCLE_START);   break; // Set as true
        case CMD_FEED_HOLD:     System_SetExecStateFlag(EXEC_FEED_HOLD);     break; // Set as true

        // Real-time control characters extended ASCII only.
        case CMD_SAFETY_DOOR:   System_SetExecStateFlag(EXEC_SAFETY_DOOR);   break; // Set as true
        case CMD_JOG_CANCEL:
        {
            if(System.State & STATE_JOG)    // Block all other states from invoking motion cancel.
            {
                System_SetExecStateFlag(EXEC_MOTION_CANCEL);
            }
        }
        break;

        case CMD_FEED_OVR_RESET:            System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_RESET);              break;
        case CMD_FEED_OVR_COARSE_PLUS:      System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_COARSE_PLUS);        break;
        case CMD_FEED_OVR_COARSE_MINUS:     System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_COARSE_MINUS);       break;
        case CMD_FEED_OVR_FINE_PLUS:        System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_FINE_PLUS);          break;
        case CMD_FEED_OVR_FINE_MINUS:       System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_FINE_MINUS);         break;
        case CMD_RAPID_OVR_RESET:           System_SetExecMotionOverrideFlag(EXEC_RAPID_OVR_RESET);             break;
        case CMD_RAPID_OVR_MEDIUM:          System_SetExecMotionOverrideFlag(EXEC_RAPID_OVR_MEDIUM);            break;
        case CMD_RAPID_OVR_LOW:             System_SetExecMotionOverrideFlag(EXEC_RAPID_OVR_LOW);               break;
        case CMD_SPINDLE_OVR_RESET:         System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_RESET);        break;
        case CMD_SPINDLE_OVR_COARSE_PLUS:   System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_COARSE_PLUS);  break;
        case CMD_SPINDLE_OVR_COARSE_MINUS:  System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_COARSE_MINUS); break;
        case CMD_SPINDLE_OVR_FINE_PLUS:     System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_FINE_PLUS);    break;
        case CMD_SPINDLE_OVR_FINE_MINUS:    System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_FINE_MINUS);   break;
        case CMD_SPINDLE_OVR_STOP:          System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_STOP);         break;
        case CMD_COOLANT_FLOOD_OVR_TOGGLE:  System_SetExecAccessoryOverrideFlag(EXEC_COOLANT_FLOOD_OVR_TOGGLE); break;
        case CMD_COOLANT_MIST_OVR_TOGGLE:
        {
           if(Config.M7_Enable == true)
           {
               System_SetExecAccessoryOverrideFlag(EXEC_COOLANT_MIST_OVR_TOGGLE);  break;
           }

           // No break; we continue onto the default since M7 is not enabled
        }

        default:
        {
            if(RealTimeCommand < 0x80)
            {
                // Throw away any unfound extended-ASCII character by not passing it to the serial buffer.
                // keeping only character that are not extended
                IsItRealTime = false;
            }
        }
        break;
    }

    return IsItRealTime;
}
