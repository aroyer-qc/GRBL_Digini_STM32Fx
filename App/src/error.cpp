//-------------------------------------------------------------------------------------------------
//
//  File : error.cpp
//
//  By   : Alain Royer
//
//  On   : January 7, 2014
//
//-------------------------------------------------------------------------------------------------
//------ Revision And History ---------------------------------------------------------------------
//
//  Build  Date           Author            Description
//  -----  -------------  ----------------  -------------------------------------------------------
//  000    Jan 7,   2014  Alain Royer       New code
//
//
//*************************************************************************************************

//------ Note(s) ----------------------------------------------------------------------------------
//
//
//
//
//
//
//*************************************************************************************************

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------
#define ERROR_GLOBAL
#include "error.h"
#undef ERROR_GLOBAL
#include "lib_macro.h"

//-------------------------------------------------------------------------------------------------
//
//  Name:           ERR_GetRegistersFromStack
//
//  Parameter(s):   uint32_t* pulFaultStackAddress
//  Return:         none
//
//  Description:    Get a peak at the register from stack when the hard fault occured
//
//  Date            Author              Description
//  -------------   ----------------    -----------------------------------------------------------
//  Jan 7,   2014   Alain Royer         First draft
//
//-------------------------------------------------------------------------------------------------
/*
extern "C" void ERR_GetRegistersFromStack(uint32_t* pullFaultStackAddress)
{
    static volatile uint32_t r0;
    static volatile uint32_t r1;
    static volatile uint32_t r2;
    static volatile uint32_t r3;
    static volatile uint32_t r12;
    static volatile uint32_t lr;       // Link register.
    static volatile uint32_t pc;       // Program counter.
    static volatile uint32_t psr;      // Program status register.

    r0  = pullFaultStackAddress[0];
    r1  = pullFaultStackAddress[1];
    r2  = pullFaultStackAddress[2];
    r3  = pullFaultStackAddress[3];
    r12 = pullFaultStackAddress[4];
    lr  = pullFaultStackAddress[5];
    pc  = pullFaultStackAddress[6];
    psr = pullFaultStackAddress[7];

    VAR_UNUSED(r0);
    VAR_UNUSED(r1);
    VAR_UNUSED(r2);
    VAR_UNUSED(r3);
    VAR_UNUSED(r12);
    VAR_UNUSED(lr);
    VAR_UNUSED(pc);
    VAR_UNUSED(psr);

    // When the following line is hit, the variables contain the register values.
    for(;;);
}
*/

//-------------------------------------------------------------------------------------------------
//
//  Name:           ERR_Message
//
//  Parameter(s):   uint8_t     Type
//                  char*       pBuffer
//
//  Return:         void
//
//  Description:    Display a critical error screen
//                  When no other choice are possible this is called (SKIN do not exist) basic
//                  graphic info only can be used with explaining text
//
//  Date            Author              Description
//  -------------   ----------------    -----------------------------------------------------------
//  Jan 7,   2014   Alain Royer         First draft
//
//-------------------------------------------------------------------------------------------------
//void ERR_Screen(uint8_t Type, char* pBuffer)
//{
//}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ERR_LogInFileIfErrorCoordinate
//
//  Parameter(s):   uint16_t    wX
//                  uint16_t    wY
//                  uint16_t    wSizeX
//                  uint16_t    wSizeY
//                  uint32_t   dwAddressSource
//                  uint32_t   dwAddressDest
//                  uint8_t    byErrorID
//  Return:         void
//
//  Description:    Check coordinate of a access X,Y, size X and Y and log error if out of
//                  range
//
//  Date            Author              Description
//  -------------   ----------------    -----------------------------------------------------------
//  Jan 7,   2014   Alain Royer         First draft
//-------------------------------------------------------------------------------------------------
/*
uint8_t ERR_LogInFileIfErrorCoordinate(char* pMsg,
                                    uint16_t wX,
                                    uint16_t wY,
                                    uint16_t wSizeX,
                                    uint16_t wSizeY,
                                    uint16_t* pAddressSource,
                                    uint16_t* pAddressDest,
                                    uint8_t byErrorID)
{
    FIL*        pFile;
    char*       pBuffer1;
    FRESULT     fResult;
    uint8_t        byError;

    if((wX >= GLCD_SIZE_X)           ||
       (wY >= GLCD_SIZE_Y)           ||
       (wSizeX == 0)                 ||
       (wSizeY == 0)                 ||
       (wSizeX > GLCD_SIZE_X)        ||
       (wSizeY > GLCD_SIZE_Y)        ||
       ((wX + wSizeX) > GLCD_SIZE_X) ||
       ((wY + wSizeY) > GLCD_SIZE_Y)   )
    {
        OSSemPend(Semaphore.Names.pErrorLogFile, 0, &byError);

        MNTC_ERR_MemoryVerification(byErrorID);
        ERR_TimeStampLogFile(ERR_TYPE_WARNING);

        pFile    = pMemory->Alloc(sizeof(FIL));
        pBuffer1 = pMemory->Alloc(256);

        fResult = ERR_OpenLogFile(pFile);
        if(fResult == FR_OK)
        {
            uint16_t wLenght;

            f_write(pFile, pMsg, LIB_strlen_B(pMsg), &wLenght);
            f_write(pFile, caCarriageReturn, LIB_strlen_B(caCarriageReturn), &wLenght);
            LIB_Sprintf_B(pBuffer1,
                        "wX = %d, wY = %d, wWidth = %d, wHeight = %d, Source = %ld; Destination = %ld",
                        wX,
                        wY,
                        wSizeX,
                        wSizeY,
                        (uint32_t)pAddressSource,
                        (uint32_t)pAddressDest);
            f_write(pFile, pBuffer1, LIB_strlen_B(pBuffer1), &wLenght);
            f_write(pFile, caCarriageReturn, LIB_strlen_B(caCarriageReturn), &wLenght);
            f_write(pFile, caCarriageReturn, LIB_strlen_B(caCarriageReturn), &wLenght);

            #if (DBG_LOG_FILE_PROTECTION == DEF_DISABLED)
            f_close(pFile);
            #else
            ERR_ValidateLogFile(pFile);
            #endif
        }

        OSSemPost(Semaphore.Names.pErrorLogFile);
        pMemory->Free(&pFile);
        pMemory->Free(&pBuffer1);

        return ERR_OUT_OF_RANGE;
    }
    return ERR_NONE;
}
*/
//-------------------------------------------------------------------------------------------------
//
//  Name:           ERR_PutMessageInLogFile
//
//  Parameter(s):   char*   pMessage
//                  uint16_t    wErrorID    Error ID so we can find where erro append
//                  uint8_t    byMODE      ERR_TYPE_WARNING or ERR_TYPE_CRITICAL
//  Return:         void
//
//  Description:    Log this message in log error file
//
//  Date            Author              Description
//  -------------   ----------------    -----------------------------------------------------------
//  Jan 7,   2014   Alain Royer         First draft
//-------------------------------------------------------------------------------------------------
/*
void ERR_PutMessageInLogFile(char* pMessage, uint16_t wErrorID, uint8_t byMode)
{
    FIL*        pFile;
    char*       pBuffer1;
    FRESULT     fResult;

    ERR_TimeStampLogFile(byMode);

    if(byMode == ERR_TYPE_CRITICAL)
    {
        pFile    = (FIL*)  &AppMemGroup4Partition[APP_MEM_NUMBER_BLOCK_GROUP_4 - 2][0];     // Since the system is failed, we take the last 3 block address
        pBuffer1 = (char*) &AppMemGroup4Partition[APP_MEM_NUMBER_BLOCK_GROUP_4 - 1][0];     // of the MemBlock4 to work
    }
    else
    {
        pFile    = pMemory->Alloc(sizeof(FIL));
        pBuffer1 = pMemory->Alloc(128);
    }

    fResult = ERR_OpenLogFile(pFile);
    if(fResult == FR_OK)
    {
        uint16_t            wLenght;

        f_write(pFile, pMessage, LIB_strlen_B(pMessage), &wLenght);
        LIB_Sprintf_B(pBuffer1, " Error ID: %4d", wErrorID);
        f_write(pFile, pBuffer1, LIB_strlen_B(pBuffer1), &wLenght);
        f_write(pFile, caCarriageReturn, LIB_strlen_B(caCarriageReturn), &wLenght);
        f_write(pFile, caCarriageReturn, LIB_strlen_B(caCarriageReturn), &wLenght);

        #if (DBG_LOG_FILE_PROTECTION == DEF_DISABLED)
        f_close(pFile);
        #else
        ERR_ValidateLogFile(pFile);
        #endif
    }

    if(byMode != ERR_TYPE_CRITICAL)
    {
        pMemory->Free(&pFile);
        pMemory->Free(&pBuffer1);
    }
}
*/
//-------------------------------------------------------------------------------------------------
//
//  Name:           ERR_PutAllocTableInLogFile
//
//  Parameter(s):   char*   pMessage
//  Return:         void
//
//  Description:    Log this message in log error file
//
//  Date            Author              Description
//  -------------   ----------------    -----------------------------------------------------------
//  Jan 7,   2014   Alain Royer         First draft
//-------------------------------------------------------------------------------------------------
/*
void ERR_PutAllocTableInLogFile(uint16_t wErrorID)
{
    FIL*        pFile;
    char*       pBuffer1;
    char*       pBuffer2;
    FRESULT     fResult;
    uint8_t        byError;

    OSSemPend(Semaphore.Names.pErrorLogFile, 0, &byError);

    ERR_TimeStampLogFile(ERR_TYPE_CRITICAL);

    pFile    = (FIL*)  &AppMemGroup4Partition[APP_MEM_NUMBER_BLOCK_GROUP_4 - 3][0];     // Since the system is failed, we take the last 3 block address
    pBuffer1 = (char*) &AppMemGroup4Partition[APP_MEM_NUMBER_BLOCK_GROUP_4 - 2][0]; // of the MemBlock4 to work
    pBuffer2 = (char*) &AppMemGroup4Partition[APP_MEM_NUMBER_BLOCK_GROUP_4 - 1][0];

    fResult = ERR_OpenLogFile(pFile);
    if(fResult == FR_OK)
    {
        uint16_t            wLenght;
        uint8_t            byGroup;
        uint16_t            wBlock;
        uint8_t            i;

        LIB_Sprintf_B(pBuffer1, " Error ID: %4d", wErrorID);
        f_write(pFile, pBuffer1, LIB_strlen_B(pBuffer1), &wLenght);
        f_write(pFile, caCarriageReturn, LIB_strlen_B(caCarriageReturn), &wLenght);
        f_write(pFile, caCarriageReturn, LIB_strlen_B(caCarriageReturn), &wLenght);

        for(byGroup = 0; byGroup < APP_MEM_NB_BLOCK_GROUP; byGroup++)
        {
            wBlock = 0;

            LIB_Sprintf_B(pBuffer1, ERR_MEM_ALL_Header1, (byGroup + 1), cAppMemGroupInfo[byGroup].wBlockSize, cAppMemGroupInfo[byGroup].wNbBlock);
            LIB_strcat_B(pBuffer1, ERR_MEM_ALL_Header2);

            if(f_write(pFile, pBuffer1, LIB_strlen_B(pBuffer1), &wLenght) == FR_OK)
            {
                while(wBlock < cAppMemGroupInfo[byGroup].wNbBlock)
                {
                    LIB_Sprintf_B(pBuffer1, ERR_MEM_ALL_BlockNbr, wBlock);

                    for(i = 0; (i < 8 && wBlock < cAppMemGroupInfo[byGroup].wNbBlock); i++)
                    {
                        LIB_Sprintf_B(pBuffer2, ERR_MEM_ALL_ErrID, *(uint16_t*)(cErrorTableAdd[byGroup] + wBlock));
                        wBlock++;
                        LIB_strcat_B(pBuffer1, pBuffer2);
                    }
                    LIB_strcat_B(pBuffer1, caCarriageReturn);

                    if(f_write(pFile, pBuffer1, LIB_strlen_B(pBuffer1), &wLenght) != FR_OK) break;
                }
            }
        }
        LIB_strcpy_B(pBuffer1, caCarriageReturn);
        LIB_strcat_B(pBuffer1, caCarriageReturn);
        f_write(pFile, pBuffer1, LIB_strlen_B(pBuffer1), &wLenght);

        #if (DBG_LOG_FILE_PROTECTION == DEF_DISABLED)
        f_close(pFile);
        #else
        ERR_ValidateLogFile(pFile);
        #endif
    }

    OSSemPost(Semaphore.Names.pErrorLogFile);
}
*/
//-------------------------------------------------------------------------------------------------
//
//  Name:           ERR_TimeStampLogFile
//
//  Parameter(s):   uint8_t    byMode      ERR_TYPE_WARNING or ERR_TYPE_CRITICAL
//  Return:         void
//
//  Description:    Log this message in log error file
//
//  Date            Author              Description
//  -------------   ----------------    -----------------------------------------------------------
//  Jan 7,   2014   Alain Royer
//-------------------------------------------------------------------------------------------------
/*
void ERR_TimeStampLogFile(uint8_t byMode)
{
    FIL*        pFile;
    char*       pBuffer1;
    char*       pBuffer2;
    FRESULT     fResult;

    if(byMode == ERR_TYPE_CRITICAL)
    {
        pFile    = (FIL*) &AppMemGroup4Partition[APP_MEM_NUMBER_BLOCK_GROUP_4 - 3][0];      // Since the system is failed, we take the last 3 block address
        pBuffer1 = (char*)&AppMemGroup4Partition[APP_MEM_NUMBER_BLOCK_GROUP_4 - 2][0];      // of the MemBlock4 to work
        pBuffer2 = (char*)&AppMemGroup4Partition[APP_MEM_NUMBER_BLOCK_GROUP_4 - 1][0];
    }
    else
    {
        pFile    = pMemory->Alloc(sizeof(FIL));
        pBuffer1 = pMemory->Alloc(128);
        pBuffer2 = pMemory->Alloc(128);
    }

    fResult = ERR_OpenLogFile(pFile);
    if(fResult == FR_OK)
    {
        sTextLabelInfo  TextInfo;

        RAM_ReadRecord(&TextInfo, RAM_MONTH_FULL_NAME, (uint8_t)(DateTime.d.byMonth - 1), 0);
        RAM_ReadMemory(pBuffer2, TextInfo.pAddress, TextInfo.wLenght);
        pBuffer2[TextInfo.wLenght] = '\0';
        LIB_Sprintf_B(pBuffer1, g_SERV_TITL_Date_4, DateTime.d.byDay, pBuffer2);              // print Day & Month of type %02d %s
        LIB_Sprintf_B(pBuffer2, " %4d ", (DateTime.d.byYear + 2000));                         // print Year
        LIB_strcat_B(pBuffer1, pBuffer2);
        LIB_Sprintf_B(pBuffer2, ERR_MEM_UTSER_Time, DateTime.t.byHour, DateTime.t.byMinute);  // print time %2d:%02d
        LIB_strcat_B(pBuffer1, pBuffer2);
        LIB_strcat_B(pBuffer1, caCarriageReturn);
        f_write(pFile, pBuffer1, LIB_strlen_B(pBuffer1), &TextInfo.wLenght);

        #if (DBG_LOG_FILE_PROTECTION == DEF_DISABLED)
        f_close(pFile);
        #else
        ERR_ValidateLogFile(pFile);
        #endif
    }

    if(byMode != ERR_TYPE_CRITICAL)
    {
        pMemory->Free(&pFile);
        pMemory->Free(&pBuffer1);
        pMemory->Free(&pBuffer2);
    }
}
*/
//-------------------------------------------------------------------------------------------------
//
//  Name:           ERR_OpenLogFile
//
//  Parameter(s):   FIL* pFile
//  Return:         FRESULT
//
//  Description:    Open the Log File
//
//  Date            Author              Description
//  -------------   ----------------    -----------------------------------------------------------
//  Jan 7,   2014   Alain Royer
//
//-------------------------------------------------------------------------------------------------
/*
FRESULT ERR_OpenLogFile(FIL * pFile)
{
    FRESULT fResult;

    #if (DBG_LOG_FILE_PROTECTION == DEF_DISABLED)
    fResult = f_open(pFile, caMemAllErrLog, FA_WRITE);
    if(fResult == FR_NO_FILE)
    {
        fResult = f_open(pFile, caMemAllErrLog, FA_WRITE | FA_CREATE_ALWAYS);
    }
    #else
    fResult = f_open(pFile, caMemAllErrLog[ERR_byLogFileNbr], FA_WRITE);
    if(fResult == FR_NO_FILE)
    {
        fResult = f_open(pFile, caMemAllErrLog[ERR_byLogFileNbr], FA_WRITE | FA_CREATE_ALWAYS);
    }
    else if(fResult == FR_OK)
    {
        if(pFile->fsize >= ERR_LOG_FILE_MAX_SIZE)                   // If file is full, close file and open next file
        {
            f_close(pFile);
            if(ERR_byLogFileNbr == 0)
            {
                ERR_byLogFileNbr = 1;
            }
            else
            {
                ERR_byLogFileNbr = 0;
            }

            fResult = f_open(pFile, caMemAllErrLog[ERR_byLogFileNbr], FA_WRITE);
            if(fResult == FR_NO_FILE)
            {
                fResult = f_open(pFile, caMemAllErrLog[ERR_byLogFileNbr], FA_WRITE | FA_CREATE_ALWAYS);
            }
        }
    }
    #endif
    if(fResult == FR_OK)
    {
        fResult = f_lseek(pFile, pFile->fsize);
        if(fResult != FR_OK)
        {
            f_close(pFile);
        }
    }

    return fResult;
}
*/
//-------------------------------------------------------------------------------------------------
//
//  Name:           ERR_ValidateLogFile
//
//  Parameter(s):   FIL *   pFile
//                  uint8_t    byFileNbr
//  Return:
//
//  Description:    Open the Log File
//
//  Date            Author              Description
//  -------------   ----------------    -----------------------------------------------------------
//  Jan 7,   2014   Alain Royer
//
//-------------------------------------------------------------------------------------------------
/*
#if (DBG_LOG_FILE_PROTECTION == DEF_ENABLED)
void ERR_ValidateLogFile(FIL * pFile)
{
    if(pFile->fsize >= ERR_LOG_FILE_MAX_SIZE)
    {
        if(ERR_byLogFileNbr == 0)
        {
            ERR_byLogFileNbr = 1;
        }
        else // if(ERR_byLogFileNbr == 1)
        {
            ERR_byLogFileNbr = 0;
        }

        f_unlink(caMemAllErrLog[ERR_byLogFileNbr]);
    }

    f_close(pFile);
}
#endif
*/
