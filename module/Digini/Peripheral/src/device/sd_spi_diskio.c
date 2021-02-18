
#if 0

//-------------------------------------------------------------------------------------------------
//
//   Function name: SD_SPI_UpdateSocketStatus
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   Update Socket status
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
static void SD_SPI_UpdateSocketStatus(void)
{
    int inp;

    SD_SPI_SocketIsWriteProtected(&inp);
    if(inp == SD_SPI_WRITE_PROTECT)
    {
        Stat |= STA_PROTECT;                        // WP is write protected
    }
    else
    {                                                // WP is write enabled
        Stat &= ~STA_PROTECT;
    }

    SD_SPI_SocketIsEmpty(&inp);
    if(inp == SD_SPI_SOCKET_EMPTY)                  // INS = Socket empty
    {
        Stat |= (STA_NODISK | STA_NOINIT);
    }
    else
    {                                                // INS = Card inserted
        Stat &= ~STA_NODISK;
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: TakeSPI
//
//   Parameter(s):  uint16_t    speed
//   Return value:  None
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
static void TakeSPI(eSPI_Speed speed)
{
    SD_SPI_Lock();
    SD_SPI_Init(speed);
    SD_SPI_Select();
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: ReleaseSPI
//
//   Parameter(s):  None
//   Return value:  None
//
//   Description:   Deselect the card and release SPI bus
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
static void ReleaseSPI(void)
{
    SD_SPI_Deselect();
    SD_SPI_Receive(NULL);
    SD_SPI_Unlock();
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: ReceiveDataBlock
//
//   Parameter(s):    BYTE*   buff        // Data buffer to store received data
//                    UINT    btr            // Byte count (must be multiple of 4)

//   Return value:  BYTE
//
//   Description:   Receive a data packet from MMC
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
static BYTE ReceivedDataBlock(BYTE* buff, UINT btr)
{
    BYTE token;

    // Wait for data packet in timeout of 100ms
    Timer1 = (100 / SD_SPI_TICK_PERIOD);
    do
    {
        SD_SPI_Receive(&token);
    }
    while((token == 0xFF) && (Timer1 > 0));

    // Verify packet token
    if(token != SINGLE_BLOCK_START_BLOCK)
    {
        return 0;
    }

    // Receive the data block into buffer
    #ifdef SD_SPI_USE_BLOCK_ACCESS
        SD_SPI_BlockRead(buff, btr);
    #else
        for(; btr > 0; btr--)
        {
            SD_SPI_Receive(buff++);
        }
    #endif

    // Discard CRC
    SD_SPI_Receive(NULL);
    SD_SPI_Receive(NULL);

    // Return with success
    return 1;


//-------------------------------------------------------------------------------------------------
//
//   Public Functions
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Function name: SD_SPI_DiskStatus
//
//   Parameter(s):  None
//   Return value:  DSTATUS
//
//   Description:   Get Disk Status
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DSTATUS SD_SPI_DiskStatus()
{
    SD_SPI_UpdateSocketStatus();                                                                    // check socket status
    return Stat;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: SD_SPI_DiskRead
//
//   Parameter(s):  BYTE  drv                Physical drive number (0)
//                  BYTE* buff                Pointer to the data buffer to store read data
//                  WORD  sector            Start sector number (LBA)
//                    BYTE count                Sector count (1..255)
//   Return value:  DRESULT
//
//   Description:   Read Sector(s)
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DRESULT SD_SPI_DiskRead(BYTE *buff, DWORD sector, BYTE count)
{
    if(count == 0)
    {
        return RES_PARERR;
    }

    SD_SPI_UpdateSocketStatus();                                                                    // Check socket status
    if((Stat & STA_NOINIT) == STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    if((CardType & CT_BLOCK) == 0)                                                                  // Check if block addressing
    {
        sector *= 512;                                                                                // Convert to byte address if needed
    }

    TakeSPI(SPI_FAST);                                                                              // Take SPI peripheral control

    if(count == 1)                                                                                  // Single block read
    {
        SendCommand(READ_SINGLE_BLOCK, sector);
        if(response[0] == 0)
        {
            if(ReceivedDataBlock(buff, 512) > 0)
            {
                count--;
            }
        }

    }
    else                                                                                            // Multiple block read
    {

        SendCommand(READ_MULTIPLE_BLOCK, sector);                                                   // Start reception
        if(response[0] == 0)
        {
            do                                                                                      // Receive blocks
            {
                if(ReceivedDataBlock(buff, 512) == 0)
                {
                    break;                                                                            // Error status
                }
                buff += 512;
            }
            while(--count);
            SendCommand(STOP_TRANSMISSION, 0);                                                      // stop reception
        }

    }

    ReleaseSPI();

    return count ? RES_ERROR : RES_OK;

}

//-------------------------------------------------------------------------------------------------
//
//   Function name: SD_SPI_DiskWrite
//
//   Parameter(s):  const BYTE* buff            Pointer to the data to be written
//                    DWORD       sector          Start sector number (LBA)
//                    BYTE        count            Sector count (1..255)
//   Return value:  DRESULT
//
//   Description:   Write Sector(s)
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
#if _USE_WRITE == 0
DRESULT SD_SPI_DiskWrite(const BYTE *buff, DWORD sector, BYTE count)
{
    if(count == 0)
    {
        return RES_PARERR;
    }

    SD_SPI_UpdateSocketStatus();                                                                    // Check socket status
    if((Stat & STA_NOINIT) > 0)
    {
        return RES_NOTRDY;
    }

    if((Stat & STA_PROTECT) > 0)
    {
        return RES_WRPRT;
    }

    if((CardType & CT_BLOCK) == 0)                                                                  // Check if block addressing
    {
        sector *= 512;                                                                                // Convert to byte address if needed
    }

    TakeSPI(SPI_FAST);                                                                              // Take SPI peripheral control

    if(count == 1)                                                                                  // Single block write
    {
        SendCommand(WRITE_BLOCK, sector);                                                           // Write block
        if(response[0] == 0)
        {
            if(TransmitDataBlock(buff, SINGLE_BLOCK_START_BLOCK ) > 0)
            {
                count--;
            }
        }

    }
    else                                                                                            // Multiple block write
    {
      if((CardType & CT_SDC_MASK) > 0)
        {
            SendCommand(SET_WR_BLK_ERASE_COUNT_SDC, count);
        }

        send_cmd(WRITE_MULTIPLE_BLOCK, sector);                                                     // Write multiple block
        if(response[0] == 0)
        {
            do                                                                                      // Transmit blocks
            {
                if(TransmitDataBlock(buff, MULT_BLOCK_START_BLOCK) == 0)
                {
                    break;                                                                            // Error state
                }
                buff += 512;
            }
            while(--count);

            if(TransmitDataBlock(0, MULT_BLOCK_STOP_BLOCK ) == 0)                                   // STOP_TRAN token
            {
                count = 1;                                                                            // Error state
            }
        }

    }

    ReleaseSPI();

    return count ? RES_ERROR : RES_OK;

}
#endif /* _READONLY == 0 */

//-------------------------------------------------------------------------------------------------
//
//   Function name: SD_SPI_DiskIOctl
//
//   Parameter(s):  BYTE    ctrl         Control code
//                    void*   buff        Buffer to send/receive control data
//   Return value:  DRESULT
//
//   Description:   Control
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
#if _USE_IOCTL != 0
DRESULT IO_Control(BYTE ctrl, void *buff)
{
    DRESULT res;
    BYTE     n;
    DWORD    csize;
    BYTE*   ptr = buff;

    res = RES_ERROR;                                                    // pre init error

    if(ctrl == CTRL_POWER)
    {
        switch(*ptr)
        {
            case 0:                                                        // Sub control code == 0 (POWER_OFF)
                if(SD_SPI_CheckPower() == SD_SPI_CARD_PWR_ON)
                {
                    PowerOff();                                            // Power off
                }
                res = RES_OK;
                break;

            case 1:                                                        // Sub control code == 1 (POWER_ON)
                PowerOn();                                                // Power on
                res = RES_OK;
                break;

            case 2:                                                        // Sub control code == 2 (POWER_GET)
                *(ptr + 1) = (BYTE)SD_SPI_CheckPower();
                res = RES_OK;
                break;

            default:
                res = RES_PARERR;
        }

    }
    else
    {

        SD_SPI_UpdateSocketStatus();                                    // Check socket status
        if(Stat & STA_NOINIT)
        {
            return RES_NOTRDY;
        }

        TakeSPI(SPI_FAST);                                              // Take SPI peripheral control

        switch(ctrl)
        {
            case CTRL_SYNC:                                                // Make sure that no pending write process
                if(WaitReady() == 0xFF)
                {
                    res = RES_OK;
                }
                break;

            case GET_SECTOR_COUNT:                                      // Get number of sectors on the disk (DWORD)
                if(csd[0] == 0xFF)
                {
                    SendCommand(SEND_CSD, 0);
                    if(response[0] != 0x00)
                    {
                        break;
                    }
                    if(ReceivedDataBlock(&csd[0], 16) == 0)
                    {
                        break;
                    }
                }

                if((csd[0] & 0xC0) == 0x00)
                {
                    // CSD version 1
                    // SECTOR_COUNT = memory capacity / OUR_BLOCK_LEN
                    // = (BLOCKNR * BLOCK_LEN) / (512)
                    // = (((C_SIZE+1) * MULT) * (2^READ_BL_LEN)) / (2^9)
                    // = (((C_SIZE+1) * (2^(C_SIZE_MULT+2))) * (2^READ_BL_LEN)) / (2^9)
                    // = (C_SIZE+1) * 2^(C_SIZE_MULT + 2 + READ_BL_LEN - 9)
                    n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                    csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
                    *(DWORD*)buff = (DWORD)csize << (n - 9);
                    res = RES_OK;
                }
                else if((csd[0] & 0xC0) == 0x40)
                {
                    // CSD version 2
                    csize = csd[9] + ((WORD)csd[8] << 8) + 1;
                    *(DWORD*)buff = (DWORD)csize << 10;
                    res = RES_OK;
                }
                break;

            case GET_SECTOR_SIZE:                                        // Get R/W sector size (WORD)
                *(WORD*)buff = 512;
                res = RES_OK;
                break;

            case GET_BLOCK_SIZE:                                        // Get erase block size in unit of sector (DWORD)
                // SDC ver 2.00
                if((m_CardType & CT_SD2) == CT_SD2)
                {
                    if(sd_status[0] == 0xFF)
                    {
                        SendCommand(SD_STATUS_SDC, 0);
                        if(response[0] != 0x00)
                        {
                            break;
                        }
                        SD_SPI_Receive(NULL);                          // this line is necessary

                        if(ReceivedDataBlock(&sd_status[0], 15 ) == 0)
                        {
                            break;
                        }
                        for( n=49 ; n> 0; n-- ){
                            SD_SPI_Receive(NULL);                        // Purge trailing data
                        }
                    }

                    *(DWORD*)buff = 16UL << (sd_status[10] >> 4);
                    res = RES_OK;

                }
                else                                                    // SDC ver 1.XX or MMC
                {
                    if(csd[0] == 0xFF)
                    {
                        SendCommand(SEND_CSD, 0);
                        if(response[0] != 0x00)
                        {
                            break;
                        }
                        if(ReceivedDataBlock(&csd[0], 16) == 0)
                        {
                            break;
                        }
                    }

                    if(CardType & CT_SD1)                               // SDC ver 1.XX
                    {
                        *(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
                    }
                    else
                    {                                                    // MMC
                        *(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
                    }

                    res = RES_OK;
                }
                break;

            case MMC_GET_TYPE:                                            // Get card type flags (1 byte)
                *ptr = CardType;
                res = RES_OK;
                break;

            case MMC_GET_CSD:                                            // Receive CSD as a data block (16 bytes)
                if(csd[0] == 0xFF)
                {
                    SendCommand(SEND_CSD, 0);
                    if(response[0] != 0x00)
                    {
                        break;
                    }
                    if(ReceivedDataBlock(&csd[0], 16 ) == 0)
                    {
                        break;
                    }
                }

                memcpy(ptr, &csd[0], 16);
                res = RES_OK;
                break;

            case MMC_GET_CID:                                            // Receive CID as a data block (16 bytes)
                if(cid[0] == 0xFF)
                {
                    SendCommand(SEND_CID, 0);
                    if(response[0] != 0x00)
                    {
                        break;
                    }
                    if(SD_SPI_receiveDataBlock(&cid[0], 16) == 0)
                    {
                        break;
                    }
                }
                memcpy(ptr, &cid[0], 16);
                res = RES_OK;
                break;

            case MMC_GET_OCR:                                            // Receive OCR as an R3 resp (4 bytes)
                if(ocr[0] == 0xFF)
                {
                    SendCommand(READ_OCR, 0);                           // CMD58, get CCS
                    if(response[0] != 0x00 )
                    {
                        break;
                    }
                    for(n = 0; n < 4; n++)                              // Receive OCR
                    {
                          SD_SPI_Receive(&ocr[n]);
                    }
                }
                memcpy(ptr, &ocr[0], 4);
                res = RES_OK;
                break;

            case MMC_GET_SDSTAT:                                        // Receive SD status as a data block (64 bytes)
                SendCommand(SD_STATUS_SDC, 0);
                if(response[0] == 0x00)
                {
                    if(SD_SPI_receiveDataBlock(ptr, 64) > 0)
                      {
                        memcpy(&sd_status[0], ptr, 15);
                        res = RES_OK;
                    }
                }
                break;

            default:
                res = RES_PARERR;
                break;
        }

        ReleaseSPI();
    }
    return res;
}
#endif


#endif
