//-------------------------------------------------------------------------------------------------
//
//  File : lib_cli_user_command.cpp
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
//  Note: User CLI Command Function: This is the user space for the user CLI command set.
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_digini.h"

//-------------------------------------------------------------------------------------------------

#if (DIGINI_USE_CMD_LINE == DEF_ENABLED)

//-------------------------------------------------------------------------------------------------
//
//  Name:           CmdTEST1
//
//  Parameter(s):   pArg                Not used
//  Return:         SystemState_e
//
//  Description:    Example of a command
//
//  Note(s):        For this command examples.
//
//                    - This command is enable at all time (no check of flag CLI_IsItOnHold).
//                    - Read return a specific user information.
//                    - Write is not supported in this example.
//
//-------------------------------------------------------------------------------------------------
SystemState_e CommandLine::CmdTEST1(void* pArg)
{
    SystemState_e   Error;
    char            Response[11];
    static uint64_t Count = 1234;

    VAR_UNUSED(pArg);

    if(m_ReadCommand == true)
    {
        Count++;
        snprintf(Response, 11, "0x%08llX", Count);
        SendAnswer(CLI_TEST1,  SYS_OK_READ, Response);
        Error = SYS_OK_SILENT;
    }
    else
    {
        Error = SYS_CMD_NO_WRITE_SUPPORT;       // No write on this command
    }

    return Error;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           CmdTEST2
//
//  Parameter(s):   pArg                Not used
//  Return:         SystemState_e
//
//  Description:    Example of a command
//
//  Note(s):        For this command examples, you can write 3 news parameters, on read the
//                  parameter. It show how to check for parameter validity.
//
//                    - This command are enable only when on hold (check of flag CLI_IsItOnHold).
//                    - Read return application data or application information.
//                    - Write new value for application data or function.
//
//-------------------------------------------------------------------------------------------------
SystemState_e CommandLine::CmdTEST2(void* pArg)
{
    SystemState_e       Error;
    char                Response[20];
    static uint8_t      State_1;                                // Here value are static because for the example, you can read them back
    static uint8_t      Value_1;
    static int16_t      Value_2;

    VAR_UNUSED(pArg);

    if(m_IsItOnHold == true)                                  // This command will be allowed only if system is on hold
    {
        if((m_ReadCommand == true) || (m_PlainCommand == true)) // Process also a plain command has a read
        {
            snprintf(Response, 20, "%d,%d,%d", State_1, Value_1, Value_2);

            SendAnswer(CLI_TEST2, SYS_OK_READ, Response);
            Error = SYS_OK_SILENT;
        }
        else
        {
            State_1 = m_ParamValue[0];
            Value_1 = m_ParamValue[1];
            Value_2 = m_ParamValue[2];
            Error   = SYS_READY;
        }
    }
    else
    {
        Error = SYS_OK_DENIED;               // Access to this command is forbidden in run mode
    }

    return Error;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           CmdTEST3
//
//  Parameter(s):   pArg                Not used
//  Return:         SystemState_e
//
//  Description:    Example of a command
//
//-------------------------------------------------------------------------------------------------
SystemState_e CommandLine::CmdTEST3(void* pArg)
{
    SystemState_e            Error;
    char                     Response[64];
    static char              Test[CLI_STRING_SIZE];
    static int16_t           Test1;
    static uint16_t          Test2;
    static int32_t           Test3;

    VAR_UNUSED(pArg);

    if(m_IsItOnHold == true)
    {
        if(m_ReadCommand == true)
        {
            snprintf(Response, 64, "%d,%d,0x%04lX,\"%s\"", Test1, Test2, Test3, Test);
            SendAnswer(CLI_TEST3, SYS_OK_READ, Response);
            Error = SYS_OK_SILENT;
        }
        else
        {
            Test1 = m_ParamValue[0];
            Test2 = m_ParamValue[1];
            Test3 = m_ParamValue[2];
            memcpy(&Test[0], m_pParamStr[3], CLI_STRING_SIZE);
            pMemoryPool->Free((void**)&m_pParamStr[3]);
            Error = SYS_READY;
        }
    }
    else
    {
        Error = SYS_OK_DENIED;               // Access to this command is forbidden in run mode
    }

    return Error;
}

//-------------------------------------------------------------------------------------------------

#endif // (DIGINI_USE_CMD_LINE == DEF_ENABLED)
