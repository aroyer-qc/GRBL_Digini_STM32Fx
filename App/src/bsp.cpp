//-------------------------------------------------------------------------------------------------
//
//  File : bsp.cpp
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

//------ Note(s) ----------------------------------------------------------------------------------
//
//  BSP - Board support package for STM32F746G-DISCO
//
//  this board has 320K RAM in CPU
//  and            8M RAM in extended DRAM
//
//  CPU RAM
//  256K for the code
//  64K  for data base ram
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#define BSP_GLOBAL
#include "bsp.h"
#undef  BSP_GLOBAL

#include "Task_grbl.h"
#include "Task_loading.h"
#include "Task_network.h"

#if (USE_ETH_DRIVER == DEF_ENABLED) && (DIGINI_USE_ETHERNET == DEF_ENABLED)
IP_Manager myIP_Manager;
#endif // (DIGINI_USE_ETHERNET == DEF_ENABLED)

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

//#define SDRAM_SIZE                                  0x800000

// see "stm32fxx_II_fmc.h" for definition.. imported here for clarity
#define FMC_SDRAM_BANK1                             ((uint32_t)0x00000000U)
#define FMC_SDRAM_COLUMN_BITS_NUM_8                 ((uint32_t)0x00000000U)
#define FMC_SDRAM_ROW_BITS_NUM_12                   ((uint32_t)0x00000004U)
#define FMC_SDRAM_MEM_BUS_WIDTH_16                  ((uint32_t)0x00000010U)
#define FMC_SDRAM_INTERN_BANKS_NUM_4                ((uint32_t)0x00000040U)
#define FMC_SDRAM_CAS_LATENCY_2                     ((uint32_t)0x00000100U)
#define FMC_SDRAM_WRITE_PROTECTION_DISABLE          ((uint32_t)0x00000000U)
#define FMC_SDRAM_CLOCK_PERIOD_2                    ((uint32_t)0x00000800U)
#define FMC_SDRAM_RBURST_ENABLE                     ((uint32_t)0x00001000U)
#define FMC_SDRAM_RPIPE_DELAY_0                     ((uint32_t)0x00000000U)
#define FMC_SDRAM_CMD_CLK_ENABLE                    ((uint32_t)0x00000001U)
#define FMC_SDRAM_CMD_PALL                          ((uint32_t)0x00000002U)
#define FMC_SDRAM_CMD_TARGET_BANK1                  FMC_SDCMR_CTB1
#define FMC_SDRAM_CMD_AUTOREFRESH_MODE              ((uint32_t)0x00000003U)
#define FMC_SDRAM_CMD_LOAD_MODE                     ((uint32_t)0x00000004U)

#define SDRAM_MODEREG_BURST_LENGTH_1                ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2                ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4                ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8                ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL         ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED        ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2                 ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3                 ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD       ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE        ((uint16_t)0x0200)

#define SDRAM_REFRESH_COUNT                         ((uint32_t)0x0603)   // SDRAM refresh counter (100Mhz SD clock)

//-------------------------------------------------------------------------------------------------
// Const(s)
//-------------------------------------------------------------------------------------------------

#if (DIGINI_USE_GRAFX == DEF_ENABLED)
// Offset to negative so right alignment work with size modification (Erasing zone handling fix)
// Since i cannot change the DIGITAL font from windows, I fix the offset here.
const Font_e  DigitalFont[5]        =  { FT_DIGITAL_16, FT_DIGITAL_24, FT_DIGITAL_36, FT_DIGITAL_48, FT_DIGITAL_64};
const uint8_t DigitalWidth[5]       =  { 10,            17,            22,            31,            40           };
const uint8_t DigitalDotWidth[5]    =  { 4,             4,             8,             10,            12           };
const uint8_t DigitalChar[13]       =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '.', '-'};
const int8_t  DigitalBearing[5][13] = {{ -2,   5,  -2,  -1,  -1,  -1,  -2,   1,  -2,   0,   0,  -2,   0 },   // 16
                                       { -1,   9,  -1,   0,   0,   0,  -1,   2,  -1,   1,   0,  -2,   0 },   // 24
                                       { -2,   13, -2,   0,   0,   0,  -2,   2,  -2,   0,   0,  -2,   0 },   // 36
                                       {  0,   20,  0,   2,   2,   2,   0,   6,   0,   2,   0,  -1,   0 },   // 48
                                       {  0,   26,  0,   2,   3,   2,   0,   5,   0,   2,   0,   0,   0 }};  // 64
#endif

//-------------------------------------------------------------------------------------------------
// Forward Declaration(s)
//-------------------------------------------------------------------------------------------------

static void             BSP_SDRAM_Initialize            (void);
#ifdef ARM_MATH_CM7
// static void             BSP_CPU_CacheEnable             (void);
#endif

//-------------------------------------------------------------------------------------------------
// Function(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           BSP_Initialize
//  Parameter(s):   void
//  Return:         void
//
//  Description:    This function should be called by your application before anything else
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void BSP_Initialize(void)
{
    // Enable the FLASH Adaptive Real-Time memory accelerator.
    // Note:  The ART accelerator is available only for flash access on ITCM interface.
    FLASH->ACR |= FLASH_ACR_ARTEN;

    SysTick_Config(SYSTEM_CORE_CLOCK / CFG_SYSTICK_RATE);

  #ifdef ARM_MATH_CM7
    //BSP_CPU_CacheEnable();
  #endif

    ISR_Initialize();
    BSP_SDRAM_Initialize();
    LED_Init(IO_LED1);
    IO_TogglePin(IO_LED1);
    DIGINI_Initialize();
    myI2C_External.Initialize();
    myI2C_Control.Initialize();
    myADC1_Driver.Initialize();
    RNG_Initialize();
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           BSP_PostLoadingSkinFontPatch
//  Parameter(s):   void
//  Return:         None
//
//  Description:    This function will be called after the skin is loaded in memory
//
//  Note(s):        This is a patch
//                      DS_Digital FONT does not handle fixed font properly, this patch override
//                      position for each digit from 0 to 9, space and minus sign.
//                      Width is reset to fix value for all character.
//
//-------------------------------------------------------------------------------------------------
#if (DIGINI_USE_GRAFX == DEF_ENABLED)
void BSP_PostLoadingSkinFontPatch(void)
{
    FontDescriptor_t FontDescriptor;

    for(int Font = 0; Font < 5; Font++)
    {
        for(int Character = 0; Character < 13; Character++)
        {
            DB_Central.Get(&FontDescriptor, GFX_FONT_DESC_INFO, DigitalFont[Font], DigitalChar[Character]);
            FontDescriptor.Width = (DigitalChar[Character] == '.') ? DigitalDotWidth[Font] : DigitalWidth[Font];
            FontDescriptor.LeftBearing = DigitalBearing[Font][Character];
            DB_Central.Set(&FontDescriptor, GFX_FONT_DESC_INFO, DigitalFont[Font], DigitalChar[Character]);
        }
    }
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           BSP_PostOS_Initialize
//  Parameter(s):   void
//  Return:         SystemState_e       SystemState
//
//  Description:    This function should be called by your application After OS has being started
//
//  Note(s):        Example: class or driver using Semaphore
//
//-------------------------------------------------------------------------------------------------
SystemState_e BSP_PostOS_Initialize(void)
{
    SystemState_e State = SYS_READY;

    MCP23008_Input1_8.Initialize  (&myI2C_External, MCP23008_INPUT_1_8_IOEXP_I2C_SLAVE_ADDRESS,  MCP23008_DEVICE);
    MCP23008_Input9_16.Initialize (&myI2C_External, MCP23008_INPUT_8_16_IOEXP_I2C_SLAVE_ADDRESS, MCP23008_DEVICE);
    MCP23017_IO.Initialize        (&myI2C_External, MCP23017_IO_EXPANDER_I2C_SLAVE_ADDRESS,      MCP23017_DEVICE);

    MCP23008_Input1_8.PinInitInput (IO_PIN_MASK_ALL_LOW, MCP230xx_INPUT_NO_PULL);   // All 8 pin are inputs
    MCP23008_Input9_16.PinInitInput(IO_PIN_MASK_ALL_LOW, MCP230xx_INPUT_NO_PULL);   // All 8 pin are inputs
    MCP23017_IO.PinInitOutput      (IO_PIN_MASK_ALL_LOW | IO_PIN_MASK_8, MCP230xx_IO_PUSH_PULL, MCP230xx_IO_LOW); // IO 0 to 8 are output set to 0
    MCP23017_IO.PinInitInput       (IO_PIN_MASK_14 | IO_PIN_MASK_15, MCP230xx_INPUT_NO_PULL);                     // IO 14 to 15 are input

    //QSPI.Initialize();
    State = DIGINI_PostInitialize();
  #if (DIGINI_USE_GRAFX == DEF_ENABLED)
    SKIN_pTask->RegisterPostLoadingCallback(BSP_PostLoadingSkinFontPatch);
  #endif

  #if (DIGINI_USE_COMM_MODULE == DEF_ENABLED)
    pTaskCOMM->Initialize();
  #endif

    pTaskLoading->Initialize();

  #if (DIGINI_USE_ETHERNET == DEF_ENABLED)

// MAC address configuration using GUID of the CPU.
//#define MAC_ADDR0	                            (((char *)0x1FF0F420)[0])
//#define MAC_ADDR1	                            (((char *)0x1FF0F420)[2])
//#define MAC_ADDR2	                            (((char *)0x1FF0F420)[4])
//#define MAC_ADDR3	                            (((char *)0x1FF0F420)[6])
//#define MAC_ADDR4	                            (((char *)0x1FF0F420)[8])
//#define MAC_ADDR5	                            (((char *)0x1FF0F420)[10])
    // to debug
    IP_MAC_Address_t MAC = {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5};
    myIP_Manager.Initialize(ETH_IF_GRBL, &MAC);

    pTaskNetwork->Initialize();
  #endif
   // pTaskGRBL->Initialize();

    Language_e Language;

// move this to label ????? myLabel should read language by itself
    DB_Central.Get(&Language, SYSTEM_LANGUAGE, 0, 0);
    // = LANG_FRENCH;
    myLabel.SetLanguage(Language);


  #ifdef DEBUG
    DateAndTime_t DateTime;

    DateTime.Date.Day    = 25;
    DateTime.Date.Month  = 6;
    DateTime.Date.Year   = 2024;
    DateTime.Time.Hour   = 15;
    DateTime.Time.Minute = 30;
    DateTime.Time.Second = 1;
    LIB_SetDateAndTime(&DateTime);
  #endif

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           BSP_SDRAM_Initialize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Setup the external memory controller.
//
//  Note(s):        Called in BSP_Initialize before jump to main.
//                  This function configures the external SDRAM.
//                  This SDRAM can be used as program data memory (including heap and stack).
//
//-------------------------------------------------------------------------------------------------
static void BSP_SDRAM_Initialize(void)
{
    uint32_t        ModeRegisterDefinition;
    uint32_t        AutoRefresh;

    // ---- FMC Reset ----
    RCC->AHB3RSTR |=  RCC_AHB3RSTR_FMCRST;
    RCC->AHB3RSTR &= ~RCC_AHB3RSTR_FMCRST;

    // ---- FMC Configuration ----
    RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;

    // ---- GPIOs Configuration ----
    //
    // Configures all SDRAM memory I/Os pins.
    //+--------------------+--------------------+---------------------+---------------------+
    //+                                SDRAM pins assignment                                +
    //+--------------------+--------------------+---------------------+---------------------+
    //| PD0  <-> FMC_D2    | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0     | PG0  <-> FMC_A10    |
    //| PD1  <-> FMC_D3    | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1     | PG1  <-> FMC_A11    |
    //| PD8  <-> FMC_D13   | PE7  <-> FMC_D4    | PF2  <-> FMC_A2     | PG4  <-> FMC_BA0    |
    //| PD9  <-> FMC_D14   | PE8  <-> FMC_D5    | PF3  <-> FMC_A3     | PG5  <-> FMC_BA1    |
    //| PD10 <-> FMC_D15   | PE9  <-> FMC_D6    | PF4  <-> FMC_A4     | PG8  <-> FMC_SDCLK  |
    //| PD14 <-> FMC_D0    | PE10 <-> FMC_D7    | PF5  <-> FMC_A5     | PG15 <-> FMC_SDNCAS |
    //| PD15 <-> FMC_D1    | PE11 <-> FMC_D8    | PF11 <-> FMC_SDNRAS |---------------------+
    //+--------------------| PE12 <-> FMC_D9    | PF12 <-> FMC_A6     |
    //                     | PE13 <-> FMC_D10   | PF13 <-> FMC_A7     |
    //                     | PE14 <-> FMC_D11   | PF14 <-> FMC_A8     |
    //                     | PE15 <-> FMC_D12   | PF15 <-> FMC_A9     |
    //+--------------------+--------------------+---------------------+
    //| PC3 <-> FMC_SDCKE0 |
    //| PH3 <-> FMC_SDNE0  |
    //| PH5 <-> FMC_SDNWE  |
    //+--------------------+
    for(uint32_t IO_Id = uint32_t(IO_FMC_D0); IO_Id <= uint32_t(IO_FMC_SDNWE); IO_Id++)
    {
        IO_PinInit(IO_ID_e(IO_Id));
    }

    // FMC SDRAM Bank configuration
    // Timing configuration for 100Mhz as SD clock frequency (System clock is up to 200Mhz)
    // Set SDRAM bank configuration parameters

    // Set NC, NR, MWID, NB, CAS, WP, SDCLK, RBURST, and RPIPE bits
    FMC_Bank5_6->SDCR[FMC_SDRAM_BANK1] = (uint32_t)(FMC_SDRAM_COLUMN_BITS_NUM_8        |
                                                    FMC_SDRAM_ROW_BITS_NUM_12          |
                                                    FMC_SDRAM_MEM_BUS_WIDTH_16         |
                                                    FMC_SDRAM_INTERN_BANKS_NUM_4       |
                                                    FMC_SDRAM_CAS_LATENCY_2            |
                                                    FMC_SDRAM_WRITE_PROTECTION_DISABLE |
                                                    FMC_SDRAM_CLOCK_PERIOD_2           |
                                                    FMC_SDRAM_RBURST_ENABLE            |
                                                    FMC_SDRAM_RPIPE_DELAY_0);

    // Set TMRD, TXSR, TRAS, TRC, TWR, TRP and TRCD bits
    FMC_Bank5_6->SDTR[FMC_SDRAM_BANK1] = (uint32_t)(((2)-1)                         | // TMRD: 2 Clock cycles
                                                    (((7)-1) << FMC_SDTR1_TXSR_Pos) | // TXSR: min = 70ns (7x11.11ns)
                                                    (((4)-1) << FMC_SDTR1_TRAS_Pos) | // TRAS: min = 42ns (4x11.11ns) max=120k (ns)
                                                    (((7)-1) << FMC_SDTR1_TRC_Pos)  | // TRC:  min = 63 (6x11.11ns)
                                                    (((2)-1) << FMC_SDTR1_TWR_Pos)  | // TWR:  2 Clock cycles
                                                    (((3)-1) << FMC_SDTR1_TRP_Pos)  | // TRP:  15ns => 2x11.11ns
                                                    (((3)-1) << FMC_SDTR1_TRCD_Pos)); // TRCD: 15ns => 2x11.11ns

    // --- FMC SDRAM device initialization sequence ---

    ModeRegisterDefinition = 0;
    AutoRefresh = 0;

    // Configure a clock configuration enable command
    FMC_Bank5_6->SDCMR = (FMC_SDRAM_CMD_CLK_ENABLE   |
                          FMC_SDRAM_CMD_TARGET_BANK1 |
                          AutoRefresh                |
                          ModeRegisterDefinition);

    LIB_Delay_uSec(1000);                                                                               // Insert 1 mSec delay

    // Configure a PALL (precharge all) command
    FMC_Bank5_6->SDCMR = (FMC_SDRAM_CMD_PALL         |
                          FMC_SDRAM_CMD_TARGET_BANK1 |
                          AutoRefresh                |
                          ModeRegisterDefinition);

    // Configure a Auto-Refresh command
    AutoRefresh = (8 << FMC_SDCMR_NRFS_Pos);
    FMC_Bank5_6->SDCMR = (FMC_SDRAM_CMD_AUTOREFRESH_MODE |
                          FMC_SDRAM_CMD_TARGET_BANK1     |
                          AutoRefresh                    |
                          ModeRegisterDefinition);

    // Program the external memory mode register
    // Configure a load Mode register command
    ModeRegisterDefinition = (SDRAM_MODEREG_BURST_LENGTH_1          |
                              SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                              SDRAM_MODEREG_CAS_LATENCY_2           |
                              SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                              SDRAM_MODEREG_WRITEBURST_MODE_SINGLE) << FMC_SDCMR_MRD_Pos;
    AutoRefresh = 0;
    FMC_Bank5_6->SDCMR = (FMC_SDRAM_CMD_LOAD_MODE    |
                          FMC_SDRAM_CMD_TARGET_BANK1 |
                          AutoRefresh                |
                          ModeRegisterDefinition);

    FMC_Bank5_6->SDRTR |= (SDRAM_REFRESH_COUNT << 1);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           BSP_CPU_CacheEnable
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    CPU L1-Cache enable.
//
//-------------------------------------------------------------------------------------------------
#ifdef ARM_MATH_CM7
#if 0
static void BSP_CPU_CacheEnable(void)
{
  (*(uint32_t *) 0xE000ED94) &= ~0x5;
  (*(uint32_t *) 0xE000ED98) = 0x0;                 // MPU->RNR
  (*(uint32_t *) 0xE000ED9C) = 0x20010000 |1<<4;    // MPU->RBAR
  (*(uint32_t *) 0xE000EDA0) = 0<<28 | 3 <<24 | 0<<19 | 0<<18 | 1<<17 | 0<<16 | 0<<8 | 30<<1 | 1<<0 ; // MPU->RASE  WT
  (*(uint32_t *) 0xE000ED94) = 0x5;

  // Invalidate I-Cache : ICIALLU register
  SCB_InvalidateICache();

  // Enable branch prediction
  SCB->CCR |= (1 <<18);
  __DSB();

  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_InvalidateDCache();
  SCB_EnableDCache();
}
#endif
#endif

//-------------------------------------------------------------------------------------------------

