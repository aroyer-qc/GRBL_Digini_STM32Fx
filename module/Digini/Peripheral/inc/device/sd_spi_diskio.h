

#define SD_SPI_TICK_PERIOD                                1                        // in ms
//#define SD_SPI_PORT                                        SPI.SPI_Port1


//-------------------------------------
// Socket's Card-Present Pin
// Socket's Write-Protection Pin
//-------------------------------------
#define SD_SPI_WR_PROTECT                                HAL_GPIO_ACT
#define SD_SPI_WRITEABLE                                HAL_GPIO_INACT
#define SD_SPI_SOCKET_EMPTY                                HAL_GPIO_INACT
#define SD_SPI_CARD_INSERTED                            HAL_GPIO_ACT

#define SD_SPI_SocketIsEmpty(ptr)                        //hal_GpioGetPinState( HAL_GPIO_SDCARD_DET, ptr )
#define SD_SPI_SocketIsWriteProtected(ptr)                //(*ptr = SD_SPI_WRITEABLE)


#define SD_SPI_Init(speed)                                   // hal_spiInit( SD_SPI_PORT, HAL_SPI_8b, HAL_SPI_Low, HAL_SPI_1Edge, HAL_SPI_SW, speed, HAL_SPI_MSB, 100 )
#define SD_SPI_Transmit(dat)                            // hal_spiReadWrite( SD_SPI_PORT, dat, NULL )
#define SD_SPI_Receive(ptr)                                // hal_spiReadWrite( SD_SPI_PORT, 0xFF, ptr )
#define    SD_SPI_USE_BLOCK_ACCESS
#define SD_SPI_BlockRead(ptr, len)                        // hal_spiReadBlock( SD_SPI_PORT, (unsigned short)(len), (unsigned char*)(ptr) )
#define SD_SPI_BlockWrite(ptr, len)                        // hal_spiWriteBlock( SD_SPI_PORT, (unsigned short)(len), (unsigned char*)(ptr) )



#define SD_SPI_CardPower()
#define SD_SPI_CheckPower()                                //SD_SPI_CARD_PWR_ON        //hal_GpioGetPinState( /* TBD */ , /* TBD */ )



// data response token
#define DATA_RESP_TOKEN_MASK                        0x1F
#define DATA_ACCEPTED                                0x05
#define DATA_REJECTED_CRC_ERR                        0x0B
#define DATA_REJECTED_WRT_ERR                        0x0D

// single block tokens
#define SINGLE_BLOCK_START_BLOCK                    0xFE

// multiple block
#define MULT_BLOCK_START_BLOCK                        0xFC
#define MULT_BLOCK_STOP_BLOCK                        0xFD


//-------------------------------------------------------------------------------------------------
// Enum(s)
//-------------------------------------------------------------------------------------------------

typedef enum SPI_Speed_e
{
    SPI_SLOW,
    SPI_FAST,
} eSPI_Speed;


//-------------------------------------------------------------------------------------------------
// Type definition(s) and structure(s)
//-------------------------------------------------------------------------------------------------

// data error token
typedef struct //__attribute__ ((__packed__))
{
    BYTE error                : 1;
    BYTE cc_error            : 1;
    BYTE card_ecc_failed    : 1;
    BYTE out_of_range        : 1;
    BYTE pad                : 4;
} data_err_token_t;



//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

DSTATUS SD_SPI_DiskInitialize      ();
DSTATUS SD_SPI_DiskStatus          ();
DRESULT SD_SPI_DiskRead            (BYTE*buff, DWORD sector, UINT count);
DRESULT SD_SPI_DiskWrite           (const BYTE* buff, DWORD sector, UINT count);
DRESULT SD_SPI_DiskIOctl           (BYTE cmd, void* buff);
void    SD_SPI_TickHook            ();
//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif //__SD_SPI_DISKIO_DEFINED__

//-------------------------------------------------------------------------------------------------
//EOF
