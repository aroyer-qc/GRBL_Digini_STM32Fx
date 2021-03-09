//-------------------------------------------------------------------------------------------------
//
//  File : lib_skin_task.cpp
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
#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#define LIB_SKIN_TASK_GLOBAL
#include "lib_grafx.h"
#undef  LIB_SKIN_TASK_GLOBAL
#include "bsp.h"
#include "lib_string.h"
#include "lib_utility.h"
#include "nOS.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define SKIN_FLAG_LOAD_NEW_SKIN               0x01
#define SKIN_FLAG_SKIN_IS_LOADED              0x02
#define SKIN_FLAG_SKIN_STATIC_IS_LOADED       0x04

//-------------------------------------------------------------------------------------------------
// Private variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef STATIC_SKIN_DEF

  #define X_STATIC_SKIN(ENUM_ID, SII) extern const StaticImageInfo_t SII;
    STATIC_SKIN_DEF
  #undef X_STATIC_SKIN

 const StaticImageInfo_t* SII_Array[NUMBER_OF_STATIC_IMAGE] =
 {
    nullptr,

   #define X_STATIC_SKIN(ENUM_ID, SII) &SII,
    STATIC_SKIN_DEF
   #undef  X_STATIC_SKIN
 };
#endif

//-------------------------------------------------------------------------------------------------
// Public pointer for forward declaration
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           SKIN_TaskWrapper
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//  Description:    C++ Wrapper for the task
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
extern "C" void SKIN_TaskWrapper(void* pvParameters)
{
    (static_cast<SKIN_myClassTask*>(pvParameters))->Run();
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Constructor
//
//  Parameter(s):   void
//
//  Description:
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SKIN_myClassTask::SKIN_myClassTask(const char* pDrive, const char* pFileName)
{
    m_pDrive = pDrive;
    snprintf(m_Path, 15, "%s%s", pDrive, pFileName);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Destructor
//
//  Parameter(s):   void
//
//  Description:
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SKIN_myClassTask::~SKIN_myClassTask(void)
{
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   void
//  Return:         nOS_Error
//
//  Description:    Initialize the task
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
nOS_Error SKIN_myClassTask::Initialize(void)
{
    nOS_Error Error;

    m_IsStaticLoaded = false;
    m_IsSkinLoaded   = false;
    m_pCallBack      = nullptr;

    if((Error = nOS_ThreadCreate(&this->m_Handle,
                                 SKIN_TaskWrapper,
                                 this,
                                 &this->m_Stack[0],
                                 SKIN_TASK_STACK_SIZE,
                                 SKIN_TASK_PRIO)) == NOS_OK)
    {
        Error = nOS_FlagCreate(&this->m_SkinFlags, 0);
    }

    return Error;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           IsSkinReloaded
//
//  Parameter(s):   void
//  Return:         nOS_Error
//
//  Description:    Initialize the task
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
bool SKIN_myClassTask::IsStaticSkinLoaded(void)
{
    return m_IsStaticLoaded;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           IsSkinReloaded
//
//  Parameter(s):   void
//  Return:         nOS_Error
//
//  Description:    Check if the Skin is reloaded
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
bool SKIN_myClassTask::IsSkinLoaded(void)
{
    return m_IsSkinLoaded;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           PercentLoader
//
//  Parameter(s):   void
//  Return:         nOS_Error
//
//  Description:    Initialize the task
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
uint16_t SKIN_myClassTask::PercentLoader(void)
{
    // TODO (Alain#5#) need to check if we are causing problem by returning 0 here...

    if(m_TotalToLoad != 0)
    {
        return (uint16_t)((m_ReadCount * 100) / m_TotalToLoad);
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           run
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//  Description:    main() for the TaskSkin
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void SKIN_myClassTask::Run(void)
{
    uint8_t*  pFreePointer;

    for(;;)
    {
        // Should wait here for signal for a new skin to load
        if(m_IsSkinLoaded == false)
        {
            m_pRawInputBuffer = (uint8_t*)GRAFX_RAW_INPUT_DATA_ADDRESS;

            m_CompxWorkMem.pDecode = new RawArray((void*)(GRAFX_DECODE_ARRAY_ADDRESS));
            m_CompxWorkMem.pAppend = new RawArray((void*)(GRAFX_APPEND_ARRAY_ADDRESS));
            m_CompxWorkMem.pPrefix = new RawArray((void*)(GRAFX_PREFIX_ARRAY_ADDRESS));
            m_pDecompress          = new DeCompression(&m_CompxWorkMem);

            // If we have to reload a new skin, then get the starting point of the previous skin
            //DB_Central.Get(&pFreePointer, GFX_FREE_RELOAD_POINTER, 0, 0);

            // Set the pointer to reload skin information
            //DB_Central.Set(&pFreePointer, GFX_FREE_SDRAM_POINTER,  0, 0);

          #ifdef STATIC_SKIN_DEF
            if(m_IsStaticLoaded == false)
            {
                this->StaticLoad();           // Initialize all static image

                // Static part of SKIN does not need to be reloaded, so set reload to actual new position
                DB_Central.Get(&pFreePointer, GFX_FREE_RAM_POINTER,  0, 0);
                DB_Central.Set(&pFreePointer, GFX_FREE_RELOAD_POINTER, 0, 0);

                m_IsStaticLoaded = true;
                nOS_Yield();
            }
          #endif

            m_pFS       = (FATFS*)   pMemory->AllocAndClear(sizeof(FATFS));
            m_pFile     = (FIL*)     pMemory->AllocAndClear(sizeof(FIL));
            m_pFileInfo = (FILINFO*) pMemory->AllocAndClear(sizeof(FILINFO));
            this->Load();
            pMemory->Free((void**)&m_pFileInfo);
            pMemory->Free((void**)&m_pFile);
            pMemory->Free((void**)&m_pFS);
            delete m_pDecompress;
            delete m_CompxWorkMem.pPrefix;
            delete m_CompxWorkMem.pDecode;
            delete m_CompxWorkMem.pAppend;

            if(this->m_pCallBack != nullptr)
            {
                this->m_pCallBack();
            }

            m_IsSkinLoaded = true;
        }
        nOS_Sleep(500);
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Load
//
//  Parameter(s):   None
//  Return:         SystemState_e
//
//  Description:    Load skin in memory
//
//  Note(s):        TO DO: add error management
//
//-------------------------------------------------------------------------------------------------
SystemState_e SKIN_myClassTask::Load(void)
{
    SystemState_e    State;
    uint32_t         NextBlock = 0;
    uint16_t         BlockTp;
    SkinBlockType_e  BlockType;

    m_ReadCount = 0;

    // Register work area for logical drive
    do
    {
        m_FResult = f_mount(m_pFS, m_pDrive, 1);
        nOS_Sleep(10);
    }
    while(m_FResult != FR_OK);

    // Open source file on the internal drive (SPI Flash)
    m_FResult = f_open(m_pFile, m_Path, FA_OPEN_EXISTING | FA_READ);
    if((m_FResult = f_stat(m_Path, m_pFileInfo)) != FR_OK) return SYS_FAIL;
    m_TotalToLoad = f_size(m_pFile);

    do
    {
        if((m_FResult = f_lseek(m_pFile, NextBlock)) != FR_OK) return SYS_FAIL; // Jump to next block (first is zero)

        if((State = Get_uint32_t(&NextBlock)) != SYS_READY) return State;       // Get now the next block
        if((State = Get_uint16_t(&BlockTp))   != SYS_READY) return State;       // Get the Block type
        BlockType = SkinBlockType_e(BlockTp);

        switch(BlockType)
        {
            case SKIN_IMAGE:
            {
                m_pCompressionMethod = (uint8_t*)GRAFX_DECOMPRESS_METHOD_ADDRESS;
                memset(m_pCompressionMethod, 0x00, sizeof(uint8_t) * DBASE_MAX_SKIN_IMAGE_QTY);
                m_pDataSize          = (uint32_t*)GRAFX_DATA_SIZE_ADDRESS;
                memset(m_pDataSize, 0x00, sizeof(uint32_t) * DBASE_MAX_SKIN_IMAGE_QTY);

                if((State = Get_uint16_t(&m_ItemCount)) == SYS_READY)
                {
                    if((State = GetImageInfo()) == SYS_READY)
                    {
                        State = DeCompressAllImage();
                    }
                }

                break;
            }

            case SKIN_FONT:
            {
                m_pCompressionMethod = (uint8_t*)GRAFX_DECOMPRESS_METHOD_ADDRESS;
                memset(m_pCompressionMethod, 0x00, sizeof(uint8_t) * DBASE_MAX_SKIN_FONT_QTY * 256);

                if((State = Get_uint16_t(&m_ItemCount)) == SYS_READY)
                {
                    if((State = GetFontInfo()) == SYS_READY)
                    {
                        State = DeCompressAllFont();
                    }
                }

                break;
            }
        }
        //nOS_Yield();
        nOS_Sleep(1);
    }
    while((NextBlock != 0) && (State == SYS_READY));

    f_close(m_pFile);

    // Unregister work area prior to discard it
    f_mount(nullptr, m_pDrive, 0);

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DeCompressAllImage
//
//  Parameter(s):   None
//  Return:         SystemState_e
//
//  Description:    Decompressed all image found in skn file
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SKIN_myClassTask::DeCompressAllImage(void)
{
    ImageInfo_t     ImageInfo;
    uint8_t*        pFreePointer;
    RawArray*       pOutput;
    RawArray*       pInput;
    SystemState_e   State;
    uint32_t        ReadCount;
    uint32_t        BlockReadSize;
    uint32_t        ReadSize;
    uint8_t*        RawPointer;

    State = SYS_READY;

    // Prepare RawArray and get free RAM pointer
    DB_Central.Get(&pFreePointer, GFX_FREE_RAM_POINTER, 0, 0);

    // Decompressed all image
    for(uint16_t i = 0; (i < m_ItemCount) && (i < DBASE_MAX_SKIN_IMAGE_QTY); i++)       // Do not go over maximum number of image
    {
        RawPointer = (uint8_t*)m_pRawInputBuffer;
        // To get index in file as save previously in GetImageInfo
        DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, i + (NUMBER_OF_STATIC_IMAGE + 1), 0);

        // Read all data for this image in the raw input buffer
        f_lseek(m_pFile, uint32_t(ImageInfo.pPointer));
        ReadSize   = uint32_t(m_pDataSize[i]);
        while(ReadSize != 0)
        {
            BlockReadSize = 512;
            if(ReadSize < 512) BlockReadSize = ReadSize;

            if((m_FResult = f_read(m_pFile, (UINT*)RawPointer, BlockReadSize, (UINT*)&ReadCount)) != FR_OK)
            {
                State = SYS_FAIL;
                goto exitDecompress;
            }
            RawPointer    += BlockReadSize;
            ReadSize      -= BlockReadSize;
            m_ReadCount   += ReadCount;
            nOS_Sleep(10);
        }

        // Align free pointer to 32 bits boundary
        LIB_AlignPointer(pFreePointer);
        // Save memory pointer for this image
        ImageInfo.pPointer = pFreePointer;
        DB_Central.Set(&ImageInfo, GFX_IMAGE_INFO, i + (NUMBER_OF_STATIC_IMAGE + 1), 0);


        pOutput    = new RawArray(pFreePointer);
        pInput     = new RawArray(m_pRawInputBuffer, m_pDataSize[i]);

        // Increment free pointer
        pFreePointer += m_pDecompress->Process(pOutput, pInput, m_pDataSize[i], m_pCompressionMethod[i]);

        delete pInput;
        delete pOutput;
    }

  exitDecompress:

    DB_Central.Set(&pFreePointer, GFX_FREE_RAM_POINTER, 0, 0);

    return State;
}

//-------------------------------------------------------------------------------------------------
SystemState_e SKIN_myClassTask::GetImageInfo(void)
{
    SystemState_e State;
    uint32_t      Dummy;
    ImageInfo_t   ImageInfo;

    // Read all image information
    for(uint16_t i = 0; (i < m_ItemCount) && ( i < DBASE_MAX_SKIN_IMAGE_QTY); i++)       // Do not go over maximum number of image
    {
        if((State = Get_uint32_t(&Dummy))                          != SYS_READY) return State;      // dummy read ID
        if((State = Get_uint32_t(&m_pDataSize[i]))                 != SYS_READY) return State;
        if((State = Get_uint8_t((uint8_t*)&ImageInfo.PixelFormat)) != SYS_READY) return State;
        if((State = Get_uint16_t(&ImageInfo.Size.Width))           != SYS_READY) return State;
        if((State = Get_uint16_t(&ImageInfo.Size.Height))          != SYS_READY) return State;
        if((State = Get_uint8_t(&m_pCompressionMethod[i]))         != SYS_READY) return State;
        if((State = Get_uint32_t((uint32_t*)&ImageInfo.pPointer))  != SYS_READY) return State;      // use memory address pointer as temporary storage for in file index
        DB_Central.Set(&ImageInfo, GFX_IMAGE_INFO, i + (NUMBER_OF_STATIC_IMAGE + 1), 0);
    }
    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
SystemState_e SKIN_myClassTask::DeCompressAllFont(void)
{
    FontDescriptor_t FontDescriptor;
    uint8_t*         pFreePointer;
    SystemState_e    State;
    uint32_t         ReadCount;
    uint32_t         BlockReadSize;
    uint32_t         ReadSize;
    uint8_t*         RawPointer;
    uint16_t         OffsetCompression;

    State = SYS_READY;

    // Prepare RawArray and get free RAM pointer
    DB_Central.Get(&pFreePointer, GFX_FREE_RAM_POINTER, 0, 0);

    // Decompressed all font
    for(uint16_t i = 0; (i < m_ItemCount) && (i < DBASE_MAX_SKIN_FONT_QTY); i++)        // Do not go over maximum number of image
    {
        for(uint16_t Character = 0; Character < 256; Character++)
        {
            DB_Central.Get(&FontDescriptor, GFX_FONT_DESC_INFO, i + NB_SYSTEM_FONTS, Character);

            if(FontDescriptor.pAddress != 0)                                            // Only extract data for font if it exist
            {
                RawArray* pInput  = new RawArray(m_pRawInputBuffer, FontDescriptor.TotalSize);
                RawArray* pOutput = new RawArray(pFreePointer);

                // Read all data for this image in the raw input buffer
                f_lseek(m_pFile, uint32_t(FontDescriptor.pAddress));
                RawPointer = m_pRawInputBuffer;
                ReadSize   = FontDescriptor.TotalSize;

                while(ReadSize != 0)
                {
                    BlockReadSize = 512;
                    if(ReadSize < 512) BlockReadSize = ReadSize;

                    if((m_FResult = f_read(m_pFile, (UINT*)RawPointer, BlockReadSize, (UINT*)&ReadCount)) != FR_OK)
                    {
                        State = SYS_FAIL;
                        goto exitDecompress;
                    }
                    RawPointer    += BlockReadSize;
                    ReadSize      -= BlockReadSize;
                    m_ReadCount   += ReadCount;
                    nOS_Sleep(10);
                }

                // Increment free pointer and decompressed data
                OffsetCompression = (i * 256) + Character;
                if(FontDescriptor.TotalSize != 0)
                {
                    // Save memory pointer
                    FontDescriptor.pAddress = pFreePointer;
                    // Decompress the data
                    pFreePointer += m_pDecompress->Process(pOutput, pInput, FontDescriptor.TotalSize, m_pCompressionMethod[OffsetCompression]);
                    // Save total size for this font
                    FontDescriptor.TotalSize = uint16_t(FontDescriptor.Size.Width) * uint16_t(FontDescriptor.Size.Height);
                    DB_Central.Set(&FontDescriptor, GFX_FONT_DESC_INFO, i + NB_SYSTEM_FONTS, Character);
/*
CLayer::SetDrawing(FOREGROUND_DISPLAY_LAYER_0);
CLayer::SetTextColor(0x00FFFFFF);

CLayer::SetColor(TRANSPARENT);
Box_t Box;
Box.Pos.X = 0;
Box.Pos.Y = 0;
Box.Size.Width = 130;
Box.Size.Height = 130;
DrawRectangle(&Box);
Cartesian_t Pos;
Pos.X = 0;
Pos.Y = 0;

DRV_PrintFont(&FontDescriptor, &Pos);
*/
                }


                delete pOutput;
                delete pInput;
            }
        }
    }

  exitDecompress:

    DB_Central.Set(&pFreePointer, GFX_FREE_RAM_POINTER, 0, 0);

    return State;
}


//-------------------------------------------------------------------------------------------------
SystemState_e SKIN_myClassTask::GetFontInfo(void)
{
    SystemState_e    State;
    uint32_t         Dummy;
    FontInfo_t       FontInfo;
    FontDescriptor_t FontDescriptor;
    uint8_t          InFontCharacterCount;
    uint8_t          Character;
    uint8_t          MaxWidth;

    // Read Font set information
    for(uint16_t i = 0; (i < m_ItemCount) && ( i < DBASE_MAX_SKIN_FONT_QTY); i++)                               // Do not go over maximum number of image
    {
        if((State = Get_uint8_t(&FontInfo.Height))    != SYS_READY) return State;                               // Read Height for each font
        if((State = Get_uint8_t(&FontInfo.Interline)) != SYS_READY) return State;                               // Read Height for each font
        DB_Central.Set(&FontInfo, GFX_FONT_INFO, i + NB_SYSTEM_FONTS, 0);
    }

    // Read all font information
    for(uint16_t i = 0; (i < m_ItemCount) && ( i < DBASE_MAX_SKIN_FONT_QTY); i++)                               // Do not go over maximum number of image
    {
        // Reset all character in font
        memset(&FontDescriptor, 0x00, sizeof(FontDescriptor_t));

        MaxWidth = 0;   // Reset max width for the font set.

        for(uint16_t Count = 0; Count < 256; Count++)
        {
            DB_Central.Set(&FontDescriptor, GFX_FONT_DESC_INFO, i + NB_SYSTEM_FONTS, Count);
        }

        if((State = Get_uint32_t(&Dummy))                                           != SYS_READY) return State; // Dummy read ID
        if((State = Get_uint8_t(&InFontCharacterCount))                             != SYS_READY) return State; // Character count for this font

        for(uint16_t Count = 0; Count < uint16_t(InFontCharacterCount); Count++)
        {
            if((State = Get_uint8_t((uint8_t*)&Character))                          != SYS_READY) return State; // Get the character
            if((State = Get_uint16_t(&FontDescriptor.TotalSize))                    != SYS_READY) return State;
            if((State = Get_uint8_t((uint8_t*)&FontDescriptor.LeftBearing))         != SYS_READY) return State;
            if((State = Get_uint8_t((uint8_t*)&FontDescriptor.RightBearing))        != SYS_READY) return State;
            if((State = Get_uint8_t(&FontDescriptor.Size.Width))                    != SYS_READY) return State;
            if((State = Get_uint8_t(&FontDescriptor.Size.Height))                   != SYS_READY) return State;
            if((State = Get_uint8_t(&FontDescriptor.Width))                         != SYS_READY) return State;
            if((State = Get_uint8_t((uint8_t*)&FontDescriptor.OffsetY))             != SYS_READY) return State;
            uint16_t OffsetCompression = (i * 256) + Character;
            if((State = Get_uint8_t(&m_pCompressionMethod[OffsetCompression]))      != SYS_READY) return State;
            if((State = Get_uint32_t((uint32_t*)&FontDescriptor.pAddress))          != SYS_READY) return State;  // Use memory address pointer as temporary storage for in file index
            DB_Central.Set(&FontDescriptor, GFX_FONT_DESC_INFO, i + NB_SYSTEM_FONTS, Character);

            if(FontDescriptor.Width > MaxWidth)
            {
                MaxWidth = FontDescriptor.Width;
            }
        }

        DB_Central.Get(&FontInfo, GFX_FONT_INFO, i + NB_SYSTEM_FONTS, 0);
        FontInfo.Width = MaxWidth;
        DB_Central.Set(&FontInfo, GFX_FONT_INFO, i + NB_SYSTEM_FONTS, 0);
    }
    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------

#ifdef STATIC_SKIN_DEF
void SKIN_myClassTask::StaticLoad(void)
{
    ImageInfo_t ImageInfo;
    RawArray*   pOutput;
    RawArray*   pInput;
    uint8_t*    pFreePointer;

    DB_Central.Get(&pFreePointer, GFX_FREE_RAM_POINTER, 0, 0);

    // load all static image, (Decompressed and store in RAM)
    for(int StaticImage = 1; StaticImage < NUMBER_OF_STATIC_IMAGE; StaticImage++)
    {
        ImageInfo.Size.Width  = SII_Array[StaticImage]->SizeX;
        ImageInfo.Size.Height = SII_Array[StaticImage]->SizeY;
        ImageInfo.PixelFormat = SII_Array[StaticImage]->PixelFormat;

        if(SII_Array[StaticImage]->Compression != COMPX_COMPRESSION_NONE)
        {
            pInput  = new RawArray(SII_Array[StaticImage]->pData);
            pOutput = new RawArray(pFreePointer);
            ImageInfo.pPointer = pFreePointer;
            pFreePointer += m_pDecompress->Process(pOutput,
                                                   pInput,
                                                   SII_Array[StaticImage]->RawSize,
                                                   SII_Array[StaticImage]->Compression);
            delete pOutput;
            delete pInput;
        }
        else // No compression, so we can work directly from flash memory
        {
            ImageInfo.pPointer = SII_Array[StaticImage]->pData;
        }

        DB_Central.Set(&ImageInfo, GFX_IMAGE_INFO, StaticImage, 0);
    }

    DB_Central.Set(&pFreePointer, GFX_FREE_RAM_POINTER, 0, 0);
}
#endif

//-------------------------------------------------------------------------------------------------

SystemState_e SKIN_myClassTask::Get_uint8_t(uint8_t* pValue)
{
    uint32_t ReadCount;

    m_FResult = f_read(m_pFile, (UINT*)pValue, sizeof(uint8_t), (UINT*)&ReadCount);
    if(ReadCount != sizeof(uint8_t)) return SYS_WRONG_SIZE;
    if(m_FResult != FR_OK)           return SYS_FAIL;

    m_ReadCount   += ReadCount;
    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------

SystemState_e SKIN_myClassTask::Get_uint16_t(uint16_t* pValue)
{
    uint32_t ReadCount;

    m_FResult = f_read(m_pFile, (UINT*)pValue, sizeof(uint16_t), (UINT*)&ReadCount);
    LIB_uint16_t_Swap(pValue);
    if(ReadCount != sizeof(uint16_t)) return SYS_WRONG_SIZE;
    if(m_FResult != FR_OK)            return SYS_FAIL;

    m_ReadCount += ReadCount;
    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------

SystemState_e SKIN_myClassTask::Get_uint32_t(uint32_t* pValue)
{
    uint32_t ReadCount;

    m_FResult = f_read(m_pFile, (UINT*)pValue, sizeof(uint32_t), (UINT*)&ReadCount);
    LIB_uint32_t_Swap(pValue);
    if(ReadCount != sizeof(uint32_t)) return SYS_WRONG_SIZE;
    if(m_FResult != FR_OK)            return SYS_FAIL;

    m_ReadCount += ReadCount;
    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           RegisterPostLoadingCallback
//
//  Parameter(s):   pCallBack       Post loading callback
//  Return:         void
//
//  Description:    Register a callback to be called after loading, before setting flag
//
//  Note(s):        Example of usage: Modifying some characteristic of font before it can be used
//
//-------------------------------------------------------------------------------------------------
void SKIN_myClassTask::RegisterPostLoadingCallback(SKIN_PostLoadCallBack_t pCallBack)
{
    m_pCallBack = pCallBack;
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
