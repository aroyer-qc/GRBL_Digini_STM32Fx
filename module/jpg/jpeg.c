/*
 * jpeg.c
 *
 *  Created on: 19.05.2014
 *      Author: Florian
 */

/*---------------------------------------------------------------------*
 *  include files                                                      *
 *---------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*---------------------------------------------------------------------*
 *  local definitions                                                  *
 *---------------------------------------------------------------------*/

#define M_SOF0              0xc0
#define M_DHT               0xc4
#define M_EOI               0xd9
#define M_SOS               0xda
#define M_DQT               0xdb
#define M_DRI               0xdd
#define M_APP0              0xe0

#define W1                  2841
#define W2                  2676
#define W3                  2408
#define W5                  1609
#define W6                  1108
#define W7                  565

#define MAKEWORD(a, b)      ((uint16_t)(((uint8_t)(a)) | ((uint16_t)((uint8_t)(b))) << 8))



#define FUNC_OK             0
#define FUNC_FORMAT_ERROR   3


typedef struct
{
    //int32_t     CurX;
    //int32_t     CurY;
    uint32_t    ImgWidth;
    uint32_t    ImgHeight;
    size_t      ImgSize;
    int16_t     SampRate_Y_H;
    int16_t     SampRate_Y_V;
    int16_t     SampRate_U_H;
    int16_t     SampRate_U_V;
    int16_t     SampRate_V_H;
    int16_t     SampRate_V_V;
    int16_t     H_YtoU;
    int16_t     V_YtoU;
    int16_t     H_YtoV;
    int16_t     V_YtoV;
    int16_t     Y_in_MCU;
    int16_t     U_in_MCU;
    int16_t     V_in_MCU;  // notwendig ??
    uint8_t*    lpJpegBuf;
    uint8_t*    lp;
    int16_t     qt_table[3][64];
    int16_t     comp_num;
    uint8_t     comp_index[3];
    uint8_t     YDcIndex;
    uint8_t     YAcIndex;
    uint8_t     UVDcIndex;
    uint8_t     UVAcIndex;
    uint8_t     HufTabIndex;
    int16_t*    YQtTable;
    int16_t*    UQtTable;
    int16_t*    VQtTable;
    int16_t     code_pos_table[4][16];
    int16_t     code_len_table[4][16];
    uint16_t    code_value_table[4][256];
    uint16_t    huf_max_value[4][16];
    uint16_t    huf_min_value[4][16];
    int16_t     BitPos;
    int16_t     CurByte;
    int16_t     rrun;
    int16_t     vvalue;
    int16_t     MCUBuffer[10 * 64];
    int16_t     QtZzMCUBuffer[10 * 64];
    int16_t     BlockBuffer[64];
    int16_t     ycoef;
    int16_t     ucoef;
    int16_t     vcoef;
    uint8_t     IntervalFlag;
    int16_t     interval;
    int16_t     Y[4 * 64];
    int16_t     U[4 * 64];
    int16_t     V[4 * 64];
    uint32_t    sizei;
    uint32_t    sizej;
    int16_t     restart;
    int32_t     iclip[1024];
    int32_t*    iclp;
} JPEG_Info_t;

static JPEG_Info_t JPEG_Info;

static int      InitTag                 (void);
static int      Decode                  (uint8_t* buffer);
static void     GetYUV                  (int16_t flag);
static size_t   StoreBuffer             (uint8_t* buffer);
static int      DecodeMCUBlock          (void);
static int      HufBlock                (uint8_t dchufindex, uint8_t achufindex);
static int      DecodeElement           (void);
static void     IQtIZzMCUComponent      (int16_t flag);
static void     IQtIZzBlock             (int16_t* s, int16_t* d, int16_t flag);
static void     Fast_IDCT               (int* block);
static uint8_t  ReadByte                (void);
static void     Initialize_Fast_IDCT    (void);
static void     idctrow                 (int* blk);
static void     idctcol                 (int* blk);

/*---------------------------------------------------------------------*
 *  external declarations                                              *
 *---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*
 *  public data                                                        *
 *---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*
 *  private data                                                       *
 *---------------------------------------------------------------------*/

static const int JPEG_Info_ZZ[8][8] =
{
    {  0,  1,  5,  6, 14, 15, 27, 28 },
    {  2,  4,  7, 13, 16, 26, 29, 42 },
    {  3,  8, 12, 17, 25, 30, 41, 43 },
    {  9, 11, 18, 24, 37, 40, 44, 53 },
    { 10, 19, 23, 32, 39, 45, 52, 54 },
    { 20, 22, 33, 38, 46, 51, 55, 60 },
    { 21, 34, 37, 47, 50, 56, 59, 61 },
    { 35, 36, 48, 49, 57, 58, 62, 63 }
};

static const uint8_t JPEG_Info_AND[9] =
{
    0, 1, 3, 7, 0xf, 0x1f, 0x3f, 0x7f, 0xff
};

/*---------------------------------------------------------------------*
 *  private functions                                                  *
 *---------------------------------------------------------------------*/

static int InitTag(void)
{
    uint8_t  finish = 0;
    uint8_t  id;
    int16_t  llength;
    int16_t  i, j, k;
    int16_t  huftab1, huftab2;
    int16_t  huftabindex;
    uint8_t  hf_table_index;
    uint8_t  qt_table_index;
    uint8_t  comnum;
    uint8_t* lptemp;
    int16_t  colorount;

    JPEG_Info.lp = JPEG_Info.lpJpegBuf + 2;

    while(finish == false)
    {
        id = *(JPEG_Info.lp + 1);
        JPEG_Info.lp += 2;
        switch(id)
        {
            case M_APP0:
                llength = MAKEWORD(*(JPEG_Info.lp + 1), *JPEG_Info.lp);
                JPEG_Info.lp += llength;
                break;

            case M_DQT:
                llength = MAKEWORD(*(JPEG_Info.lp + 1), *JPEG_Info.lp);
                qt_table_index = (*(JPEG_Info.lp + 2)) & 0x0f;
                lptemp = JPEG_Info.lp + 3;

                if(llength < 80)
                {
                    for(i = 0; i < 64; i++)
                    {
                        JPEG_Info.qt_table[qt_table_index][i] = (int16_t) *(lptemp++);
                    }
                }
                else
                {
                    for(i = 0; i < 64; i++)
                    {
                        JPEG_Info.qt_table[qt_table_index][i] = (int16_t) *(lptemp++);
                    }

                    qt_table_index = (*(lptemp++)) & 0x0f;

                    for(i = 0; i < 64; i++)
                    {
                        JPEG_Info.qt_table[qt_table_index][i] = (int16_t) *(lptemp++);
                    }
                }

                JPEG_Info.lp += llength;
                break;

            case M_SOF0:
                llength = MAKEWORD(*(JPEG_Info.lp + 1), *JPEG_Info.lp);
                JPEG_Info.ImgHeight = MAKEWORD(*(JPEG_Info.lp + 4), *(JPEG_Info.lp + 3));
                JPEG_Info.ImgWidth  = MAKEWORD(*(JPEG_Info.lp + 6), *(JPEG_Info.lp + 5));
                JPEG_Info.comp_num  = *(JPEG_Info.lp + 7);

                if((JPEG_Info.comp_num != 1) && (JPEG_Info.comp_num != 3))
                {
                    return FUNC_FORMAT_ERROR;
                }

                if(JPEG_Info.comp_num == 3)
                {
                    JPEG_Info.comp_index[0] = *(JPEG_Info.lp + 8);
                    JPEG_Info.SampRate_Y_H = (*(JPEG_Info.lp + 9)) >> 4;
                    JPEG_Info.SampRate_Y_V = (*(JPEG_Info.lp + 9)) & 0x0f;
                    JPEG_Info.YQtTable = (int16_t *) JPEG_Info.qt_table[*(JPEG_Info.lp + 10)];
                    JPEG_Info.comp_index[1] = *(JPEG_Info.lp + 11);
                    JPEG_Info.SampRate_U_H = (*(JPEG_Info.lp + 12)) >> 4;
                    JPEG_Info.SampRate_U_V = (*(JPEG_Info.lp + 12)) & 0x0f;
                    JPEG_Info.UQtTable =     (int16_t *) JPEG_Info.qt_table[*(JPEG_Info.lp + 13)];
                    JPEG_Info.comp_index[2] = *(JPEG_Info.lp + 14);
                    JPEG_Info.SampRate_V_H = (*(JPEG_Info.lp + 15)) >> 4;
                    JPEG_Info.SampRate_V_V = (*(JPEG_Info.lp + 15)) & 0x0f;
                    JPEG_Info.VQtTable = (int16_t *) JPEG_Info.qt_table[*(JPEG_Info.lp + 16)];
                }
                else
                {
                    JPEG_Info.comp_index[0] = *(JPEG_Info.lp + 8);
                    JPEG_Info.SampRate_Y_H  = (*(JPEG_Info.lp + 9)) >> 4;
                    JPEG_Info.SampRate_Y_V  = (*(JPEG_Info.lp + 9)) & 0x0f;
                    JPEG_Info.YQtTable      = (int16_t *) JPEG_Info.qt_table[*(JPEG_Info.lp + 10)];
                    JPEG_Info.comp_index[1] = *(JPEG_Info.lp + 8);
                    JPEG_Info.SampRate_U_H  = 1;
                    JPEG_Info.SampRate_U_V  = 1;
                    JPEG_Info.UQtTable      = (int16_t *) JPEG_Info.qt_table[*(JPEG_Info.lp + 10)];
                    JPEG_Info.comp_index[2] = *(JPEG_Info.lp + 8);
                    JPEG_Info.SampRate_V_H  = 1;
                    JPEG_Info.SampRate_V_V  = 1;
                    JPEG_Info.VQtTable      = (int16_t *) JPEG_Info.qt_table[*(JPEG_Info.lp + 10)];
                }

                JPEG_Info.lp += llength;
                break;

            case M_DHT:
                llength = MAKEWORD(*(JPEG_Info.lp + 1), *JPEG_Info.lp);
                if(llength < 0xd0)
                {
                    huftab1 = (int16_t) (*(JPEG_Info.lp + 2)) >> 4;
                    huftab2 = (int16_t) (*(JPEG_Info.lp + 2)) & 0x0f;
                    huftabindex = huftab1 * 2 + huftab2;
                    lptemp = JPEG_Info.lp + 3;

                    for(i = 0; i < 16; i++)
                    {
                        JPEG_Info.code_len_table[huftabindex][i] = (int16_t)(*(lptemp++));
                    }

                    j = 0;

                    for(i = 0; i < 16; i++)
                    {
                        if(JPEG_Info.code_len_table[huftabindex][i] != 0)
                        {
                            k = 0;
                            while(k < JPEG_Info.code_len_table[huftabindex][i])
                            {
                                JPEG_Info.code_value_table[huftabindex][k + j] = (int16_t)(*(lptemp++));
                                k++;
                            }
                            j += k;
                        }
                    }

                    i = 0;

                    while(JPEG_Info.code_len_table[huftabindex][i] == 0)
                    {
                        i++;
                    }

                    for(j = 0; j < i; j++)
                    {
                        JPEG_Info.huf_min_value[huftabindex][j] = 0;
                        JPEG_Info.huf_max_value[huftabindex][j] = 0;
                    }

                    JPEG_Info.huf_min_value[huftabindex][i] = 0;
                    JPEG_Info.huf_max_value[huftabindex][i] =    JPEG_Info.code_len_table[huftabindex][i] - 1;

                    for(j = i + 1; j < 16; j++)
                    {
                        JPEG_Info.huf_min_value[huftabindex][j] = (JPEG_Info.huf_max_value[huftabindex][j - 1] + 1) << 1;
                        JPEG_Info.huf_max_value[huftabindex][j] =    JPEG_Info.huf_min_value[huftabindex][j] + JPEG_Info.code_len_table[huftabindex][j] - 1;
                    }

                    JPEG_Info.code_pos_table[huftabindex][0] = 0;

                    for(j = 1; j < 16; j++)
                    {
                        JPEG_Info.code_pos_table[huftabindex][j] = JPEG_Info.code_len_table[huftabindex][j - 1]    + JPEG_Info.code_pos_table[huftabindex][j    - 1];
                    }

                    JPEG_Info.lp += llength;
                }
                else
                {
                    hf_table_index = *(JPEG_Info.lp + 2);
                    JPEG_Info.lp += 2;
                    while(hf_table_index != 0xff)
                    {
                        huftab1 = (int16_t) hf_table_index >> 4;
                        huftab2 = (int16_t) hf_table_index & 0x0f;
                        huftabindex = huftab1 * 2 + huftab2;
                        lptemp = JPEG_Info.lp + 1;
                        colorount = 0;

                        for(i = 0; i < 16; i++)
                        {
                            JPEG_Info.code_len_table[huftabindex][i] = (int16_t) (*(lptemp++));
                            colorount += JPEG_Info.code_len_table[huftabindex][i];
                        }

                        colorount += 17;
                        j = 0;

                        for(i = 0; i < 16; i++)
                        {
                            if(JPEG_Info.code_len_table[huftabindex][i] != 0)
                            {
                                k = 0;

                                while(k    < JPEG_Info.code_len_table[huftabindex][i])
                                {
                                    JPEG_Info.code_value_table[huftabindex][k + j] = (int16_t) (*(lptemp++));
                                    k++;
                                }

                                j += k;
                            }
                        }

                        i = 0;

                        while(JPEG_Info.code_len_table[huftabindex][i] == 0)
                        {
                            i++;
                        }

                        for(j = 0; j < i; j++)
                        {
                            JPEG_Info.huf_min_value[huftabindex][j] = 0;
                            JPEG_Info.huf_max_value[huftabindex][j] = 0;
                        }

                        JPEG_Info.huf_min_value[huftabindex][i] = 0;
                        JPEG_Info.huf_max_value[huftabindex][i] =    JPEG_Info.code_len_table[huftabindex][i] - 1;

                        for(j = i + 1; j < 16; j++)
                        {
                            JPEG_Info.huf_min_value[huftabindex][j] =    (JPEG_Info.huf_max_value[huftabindex][j - 1] + 1) << 1;
                            JPEG_Info.huf_max_value[huftabindex][j] = JPEG_Info.huf_min_value[huftabindex][j] + JPEG_Info.code_len_table[huftabindex][j] - 1;
                        }

                        JPEG_Info.code_pos_table[huftabindex][0] = 0;

                        for(j = 1; j < 16; j++)
                        {
                            JPEG_Info.code_pos_table[huftabindex][j] = JPEG_Info.code_len_table[huftabindex][j - 1]    + JPEG_Info.code_pos_table[huftabindex][j    - 1];
                        }

                        JPEG_Info.lp += colorount;
                        hf_table_index = *JPEG_Info.lp;
                    }
                }
                break;

            case M_DRI:
                llength = MAKEWORD(*(JPEG_Info.lp + 1), *JPEG_Info.lp);
                JPEG_Info.restart = MAKEWORD(*(JPEG_Info.lp + 3), *(JPEG_Info.lp + 2));
                JPEG_Info.lp += llength;
                break;

            case M_SOS:
                llength = MAKEWORD(*(JPEG_Info.lp + 1), *JPEG_Info.lp);
                comnum = *(JPEG_Info.lp + 2);
                if(comnum != JPEG_Info.comp_num) return FUNC_FORMAT_ERROR;
                lptemp = JPEG_Info.lp + 3;

                for(i = 0; i < JPEG_Info.comp_num; i++)
                {
                    if(*lptemp == JPEG_Info.comp_index[0])
                    {
                        JPEG_Info.YDcIndex = (*(lptemp + 1)) >> 4;
                        JPEG_Info.YAcIndex = ((*(lptemp + 1)) & 0x0f) + 2;
                    }
                    else
                    {
                        JPEG_Info.UVDcIndex = (*(lptemp + 1)) >> 4;
                        JPEG_Info.UVAcIndex = ((*(lptemp + 1)) & 0x0f) + 2;
                    }
                    lptemp += 2;
                }

                JPEG_Info.lp += llength;
                finish = 1;
                break;

            case M_EOI:
                return FUNC_FORMAT_ERROR;
                break;

            default:
                if((id & 0xf0) != 0xd0)
                {
                    llength = MAKEWORD(*(JPEG_Info.lp + 1), *JPEG_Info.lp);
                    JPEG_Info.lp += llength;
                }
                else
                {
                    JPEG_Info.lp += 2;
                }
                break;
        }
    }
    return FUNC_OK;
}


static int Decode(uint8_t* buffer)
{
    int      funcret;

    JPEG_Info.Y_in_MCU = JPEG_Info.SampRate_Y_H * JPEG_Info.SampRate_Y_V;
    JPEG_Info.U_in_MCU = JPEG_Info.SampRate_U_H * JPEG_Info.SampRate_U_V;
    JPEG_Info.V_in_MCU = JPEG_Info.SampRate_V_H * JPEG_Info.SampRate_V_V;
    JPEG_Info.H_YtoU   = JPEG_Info.SampRate_Y_H / JPEG_Info.SampRate_U_H;
    JPEG_Info.V_YtoU   = JPEG_Info.SampRate_Y_V / JPEG_Info.SampRate_U_V;
    JPEG_Info.H_YtoV   = JPEG_Info.SampRate_Y_H / JPEG_Info.SampRate_V_H;
    JPEG_Info.V_YtoV   = JPEG_Info.SampRate_Y_V / JPEG_Info.SampRate_V_V;

    Initialize_Fast_IDCT();

    while((funcret = DecodeMCUBlock()) == FUNC_OK)
    {
        JPEG_Info.interval++;

        if((JPEG_Info.restart) && (JPEG_Info.interval % JPEG_Info.restart == 0))
        {
            JPEG_Info.IntervalFlag = 1;
        }
        else
        {
            JPEG_Info.IntervalFlag = 0;
        }

        IQtIZzMCUComponent(0);
        IQtIZzMCUComponent(1);
        IQtIZzMCUComponent(2);
        GetYUV(0);
        GetYUV(1);
        GetYUV(2);
        JPEG_Info.ImgSize = StoreBuffer(buffer);
        JPEG_Info.sizej += JPEG_Info.SampRate_Y_H * 8;

        if(JPEG_Info.sizej >= JPEG_Info.ImgWidth)
        {
            JPEG_Info.sizej = 0;
            JPEG_Info.sizei += JPEG_Info.SampRate_Y_V * 8;
        }

        if((JPEG_Info.sizej == 0) && (JPEG_Info.sizei >= JPEG_Info.ImgHeight))
        {
            break;
        }
    }

    return funcret;
}

static void GetYUV(int16_t flag)
{
    int16_t H, VV;
    int16_t i, j, k, h;
    int16_t *buf;
    int16_t *pQtZzMCU;

    switch(flag)
    {
        case 0:
            H = JPEG_Info.SampRate_Y_H;
            VV = JPEG_Info.SampRate_Y_V;
            buf = JPEG_Info.Y;
            pQtZzMCU = JPEG_Info.QtZzMCUBuffer;
            break;

        case 1:
            H = JPEG_Info.SampRate_U_H;
            VV = JPEG_Info.SampRate_U_V;
            buf = JPEG_Info.U;
            pQtZzMCU = JPEG_Info.QtZzMCUBuffer + JPEG_Info.Y_in_MCU * 64;
            break;

        case 2:
            H = JPEG_Info.SampRate_V_H;
            VV = JPEG_Info.SampRate_V_V;
            buf = JPEG_Info.V;
            pQtZzMCU = JPEG_Info.QtZzMCUBuffer + (JPEG_Info.Y_in_MCU + JPEG_Info.U_in_MCU) * 64;
            break;

        default:
            return;
    }

    for(i = 0; i < VV; i++)
    {
        for(j = 0; j < H; j++)
        {
            for(k = 0; k < 8; k++)
            {
                for(h = 0; h < 8; h++)
                {
                    buf[(i * 8 + k) * JPEG_Info.SampRate_Y_H * 8 + j * 8 + h] = *pQtZzMCU++;
                }
            }
        }
    }
}

static size_t StoreBuffer(uint8_t* buffer)
{
    int16_t     i = 0, j = 0;
    uint8_t     R, G, B;
    int         y, u, v, rr, gg, bb;
    int32_t     color;
    uint32_t    offset;

    for(i = 0; i < JPEG_Info.SampRate_Y_V * 8; i++)
    {
        if((JPEG_Info.sizei + i) < JPEG_Info.ImgHeight)
        {
            for(j = 0; j < JPEG_Info.SampRate_Y_H * 8; j++)
            {
                if((JPEG_Info.sizej + j) < JPEG_Info.ImgWidth)
                {
                    y = JPEG_Info.Y[i * 8 * JPEG_Info.SampRate_Y_H + j];
                    u = JPEG_Info.U[(i / JPEG_Info.V_YtoU) * 8 * JPEG_Info.SampRate_Y_H + j / JPEG_Info.H_YtoU];
                    v = JPEG_Info.V[(i / JPEG_Info.V_YtoV) * 8 * JPEG_Info.SampRate_Y_H+ j / JPEG_Info.H_YtoV];

                    rr = ((y << 8) + 18 * u + 367 * v) >> 8;
                    gg = ((y << 8) - 159 * u - 220 * v) >> 8;
                    bb = ((y << 8) + 411 * u - 29 * v) >> 8;

                    R = (uint8_t) rr;
                    G = (uint8_t) gg;
                    B = (uint8_t) bb;

                    if(rr & 0xffffff00)
                    {
                        if(rr > 255)     R = 255;
                        else if (rr < 0) R = 0;
                    }

                    if(gg & 0xffffff00)
                    {
                        if (gg > 255)    G = 255;
                        else if (gg < 0) G = 0;
                    }

                    if(bb & 0xffffff00)
                    {
                        if(bb > 255)       B = 255;
                        else if (bb < 0) B = 0;
                    }

                    color  = R >> 3;
                    color  = color << 6;
                    color |= (G >> 2);
                    color  = color << 5;
                    color |= (B >> 3);

                    offset = (JPEG_Info.sizei + i) * JPEG_Info.ImgWidth + JPEG_Info.sizej + j;

                    buffer[offset * 2] = color & 0xFF;
                    buffer[offset * 2 + 1] = color >> 8;
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }

    return (size_t)((offset + 1)* 2);
}

static int DecodeMCUBlock(void)
{
    int16_t* lpMCUBuffer;
    int16_t  i, j;
    int      funcret;

    if(JPEG_Info.IntervalFlag == 1)
    {
        JPEG_Info.lp     += 2;
        JPEG_Info.ycoef   = 0;
        JPEG_Info.ucoef   = 0;
        JPEG_Info.vcoef   = 0;
        JPEG_Info.BitPos  = 0;
        JPEG_Info.CurByte = 0;
    }

    switch(JPEG_Info.comp_num)
    {
        case 3:
            lpMCUBuffer = JPEG_Info.MCUBuffer;
            for(i = 0; i < JPEG_Info.SampRate_Y_H * JPEG_Info.SampRate_Y_V; i++)
            {
                funcret = HufBlock (JPEG_Info.YDcIndex, JPEG_Info.YAcIndex);
                if(funcret != FUNC_OK) return funcret;
                JPEG_Info.BlockBuffer[0] = JPEG_Info.BlockBuffer[0]    + JPEG_Info.ycoef;
                JPEG_Info.ycoef = JPEG_Info.BlockBuffer[0];

                for(j = 0; j < 64; j++)
                {
                    *lpMCUBuffer++ = JPEG_Info.BlockBuffer[j];
                }
            }

            for(i = 0; i < JPEG_Info.SampRate_U_H * JPEG_Info.SampRate_U_V;    i++)
            {
                funcret = HufBlock (JPEG_Info.UVDcIndex, JPEG_Info.UVAcIndex);
                if (funcret != FUNC_OK)    return funcret;
                JPEG_Info.BlockBuffer[0] = JPEG_Info.BlockBuffer[0]    + JPEG_Info.ucoef;
                JPEG_Info.ucoef = JPEG_Info.BlockBuffer[0];

                for(j = 0; j < 64; j++)
                {
                    *lpMCUBuffer++ = JPEG_Info.BlockBuffer[j];
                }
            }

            for(i = 0; i < JPEG_Info.SampRate_V_H * JPEG_Info.SampRate_V_V;    i++)
            {
                funcret = HufBlock (JPEG_Info.UVDcIndex, JPEG_Info.UVAcIndex);
                if(funcret != FUNC_OK) return funcret;
                JPEG_Info.BlockBuffer[0] = JPEG_Info.BlockBuffer[0]    + JPEG_Info.vcoef;
                JPEG_Info.vcoef = JPEG_Info.BlockBuffer[0];

                for(j = 0; j < 64; j++)
                {
                    *lpMCUBuffer++ = JPEG_Info.BlockBuffer[j];
                }
            }
            break;

        case 1:
            lpMCUBuffer = JPEG_Info.MCUBuffer;
            funcret = HufBlock (JPEG_Info.YDcIndex, JPEG_Info.YAcIndex);
            if(funcret != FUNC_OK) return funcret;
            JPEG_Info.BlockBuffer[0] = JPEG_Info.BlockBuffer[0]    + JPEG_Info.ycoef;
            JPEG_Info.ycoef = JPEG_Info.BlockBuffer[0];

            for(j = 0; j < 64; j++)
            {
                *lpMCUBuffer++ = JPEG_Info.BlockBuffer[j];
            }

            for(i = 0; i < 128; i++)
            {
                *lpMCUBuffer++ = 0;
            }
            break;

        default:
            return FUNC_FORMAT_ERROR;
    }

    return FUNC_OK;
}

static int HufBlock(uint8_t dchufindex, uint8_t achufindex)
{
    int16_t count = 0;
    int16_t i;
    int     funcret;

    JPEG_Info.HufTabIndex = dchufindex;
    funcret = DecodeElement ();

    if(funcret != FUNC_OK) return funcret;

    JPEG_Info.BlockBuffer[count++] = JPEG_Info.vvalue;
    JPEG_Info.HufTabIndex = achufindex;

    while(count < 64)
    {
        funcret = DecodeElement();

        if(funcret != FUNC_OK) return funcret;

        if((JPEG_Info.rrun == 0) && (JPEG_Info.vvalue == 0))
        {
            for(i = count; i < 64; i++)
            {
                JPEG_Info.BlockBuffer[i] = 0;
            }
            count = 64;
        }
        else
        {
            for(i = 0; i < JPEG_Info.rrun; i++)
            {
                JPEG_Info.BlockBuffer[count++] = 0;
            }
            JPEG_Info.BlockBuffer[count++] = JPEG_Info.vvalue;
        }
    }

    return FUNC_OK;
}

static int DecodeElement(void)
{
    int      thiscode, tempcode;
    uint16_t temp, valueex;
    int16_t  codelen;
    uint8_t  hufexbyte, runsize, tempsize, sign;
    uint8_t  newbyte, lastbyte;

    if(JPEG_Info.BitPos >= 1)
    {
        JPEG_Info.BitPos--;
        thiscode = (uint8_t) JPEG_Info.CurByte >> JPEG_Info.BitPos;
        JPEG_Info.CurByte = JPEG_Info.CurByte & JPEG_Info_AND[JPEG_Info.BitPos];
    }
    else
    {
        lastbyte = ReadByte ();
        JPEG_Info.BitPos--;
        newbyte = JPEG_Info.CurByte & JPEG_Info_AND[JPEG_Info.BitPos];
        thiscode = lastbyte >> 7;
        JPEG_Info.CurByte = newbyte;
    }
    codelen = 1;

    while((thiscode < JPEG_Info.huf_min_value[JPEG_Info.HufTabIndex][codelen - 1])    ||
          (JPEG_Info.code_len_table[JPEG_Info.HufTabIndex][codelen - 1] == 0)         ||
          (thiscode > JPEG_Info.huf_max_value[JPEG_Info.HufTabIndex][codelen - 1]))
    {
        if(JPEG_Info.BitPos >= 1)
        {
            JPEG_Info.BitPos--;
            tempcode = (uint8_t) JPEG_Info.CurByte >> JPEG_Info.BitPos;
            JPEG_Info.CurByte = JPEG_Info.CurByte & JPEG_Info_AND[JPEG_Info.BitPos];
        }
        else
        {
            lastbyte = ReadByte ();
            JPEG_Info.BitPos--;
            newbyte = JPEG_Info.CurByte & JPEG_Info_AND[JPEG_Info.BitPos];
            tempcode = (uint8_t) lastbyte >> 7;
            JPEG_Info.CurByte = newbyte;
        }

        thiscode = (thiscode << 1) + tempcode;
        codelen++;
        if(codelen > 16) return FUNC_FORMAT_ERROR;
    }

    temp = thiscode    - JPEG_Info.huf_min_value[JPEG_Info.HufTabIndex][codelen - 1] + JPEG_Info.code_pos_table[JPEG_Info.HufTabIndex][codelen - 1];
    hufexbyte =    (uint8_t) JPEG_Info.code_value_table[JPEG_Info.HufTabIndex][temp];
    JPEG_Info.rrun = (int16_t) (hufexbyte >> 4);
    runsize = hufexbyte & 0x0f;

    if(runsize == 0)
    {
        JPEG_Info.vvalue = 0;
        return FUNC_OK;
    }

    tempsize = runsize;
    if(JPEG_Info.BitPos >= runsize)
    {
        JPEG_Info.BitPos -= runsize;
        valueex = (uint8_t) JPEG_Info.CurByte >> JPEG_Info.BitPos;
        JPEG_Info.CurByte = JPEG_Info.CurByte & JPEG_Info_AND[JPEG_Info.BitPos];
    }
    else
    {
        valueex = JPEG_Info.CurByte;
        tempsize -= JPEG_Info.BitPos;
        while (tempsize > 8)
        {
            lastbyte = ReadByte ();
            valueex = (valueex << 8) + (uint8_t) lastbyte;
            tempsize -= 8;
        }
        lastbyte = ReadByte ();
        JPEG_Info.BitPos -= tempsize;
        valueex = (valueex << tempsize) + (lastbyte >> JPEG_Info.BitPos);
        JPEG_Info.CurByte = lastbyte & JPEG_Info_AND[JPEG_Info.BitPos];
    }
    sign = valueex >> (runsize - 1);
    if(sign != 0)
    {
        JPEG_Info.vvalue = valueex;
    }
    else
    {
        valueex = valueex ^ 0xffff;
        temp = 0xffff << runsize;
        JPEG_Info.vvalue = -(int16_t) (valueex ^ temp);
    }

    return FUNC_OK;
}

static void IQtIZzMCUComponent(int16_t flag)
{
    int16_t  H, VV;
    int16_t  i, j;
    int16_t* pQtZzMCUBuffer;
    int16_t* pMCUBuffer;

    switch(flag)
    {
        case 0:
            H = JPEG_Info.SampRate_Y_H;
            VV = JPEG_Info.SampRate_Y_V;
            pMCUBuffer = JPEG_Info.MCUBuffer;
            pQtZzMCUBuffer = JPEG_Info.QtZzMCUBuffer;
            break;

        case 1:
            H = JPEG_Info.SampRate_U_H;
            VV = JPEG_Info.SampRate_U_V;
            pMCUBuffer = JPEG_Info.MCUBuffer + JPEG_Info.Y_in_MCU * 64;
            pQtZzMCUBuffer = JPEG_Info.QtZzMCUBuffer + JPEG_Info.Y_in_MCU * 64;
            break;

        case 2:
            H = JPEG_Info.SampRate_V_H;
            VV = JPEG_Info.SampRate_V_V;
            pMCUBuffer = JPEG_Info.MCUBuffer + (JPEG_Info.Y_in_MCU + JPEG_Info.U_in_MCU) * 64;
            pQtZzMCUBuffer = JPEG_Info.QtZzMCUBuffer + (JPEG_Info.Y_in_MCU + JPEG_Info.U_in_MCU) * 64;
            break;

        default:
            return;
    }

    for(i = 0; i < VV; i++)
    {
        for(j = 0; j < H; j++)
        {
            IQtIZzBlock(pMCUBuffer + (i * H + j) * 64,    pQtZzMCUBuffer + (i * H + j) * 64, flag);
        }
    }
}

static void IQtIZzBlock(int16_t* s, int16_t* d, int16_t flag)
{
    int16_t  i, j;
    int16_t  tag;
    int16_t* pQt;
    int      buffer2[8][8];
    int*     buffer1;
    int16_t  offset;

    switch(flag)
    {
        case 0:
            pQt = JPEG_Info.YQtTable;
            offset = 128;
            break;

        case 1:
            pQt = JPEG_Info.UQtTable;
            offset = 0;
            break;

        case 2:
            pQt = JPEG_Info.VQtTable;
            offset = 0;
            break;

        default:
            return;
    }

    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            tag = JPEG_Info_ZZ[i][j];
            buffer2[i][j] = (int) s[tag] * (int) pQt[tag];
        }
    }

    buffer1 = (int *) buffer2;
    Fast_IDCT (buffer1);

    for(i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            d[i * 8 + j] = buffer2[i][j] + offset;
        }
    }
}

static void Fast_IDCT(int* block)
{
    int16_t i;

    for(i = 0; i < 8; i++) idctrow(block + 8 * i);
    for(i = 0; i < 8; i++) idctcol(block + i);
}

static uint8_t ReadByte(void)
{
    uint8_t i;

    i = *(JPEG_Info.lp++);
    if(i == 0xff) JPEG_Info.lp++;
    JPEG_Info.BitPos = 8;
    JPEG_Info.CurByte = i;

    return i;
}

static void Initialize_Fast_IDCT(void)
{
    int16_t i;

    JPEG_Info.iclp = JPEG_Info.iclip + 512;

    for(i = -512; i < -256; i++)    JPEG_Info.iclp[i] = -256;
    for(i = -256; i < 256; i++)        JPEG_Info.iclp[i] = i;
    for(i = 256; i < 512; i++)        JPEG_Info.iclp[i] = 512;
}

static void idctrow(int* blk)
{
    int x0, x1, x2, x3, x4, x5, x6, x7, x8;

    if(!((x1 = blk[4] << 11) | (x2 = blk[6]) | (x3 = blk[2]) | (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
    {
        blk[0] = blk[1] = blk[2] = blk[3] = blk[4] = blk[5] = blk[6] = blk[7] = blk[0] << 3;
        return;
    }

    x0 = (blk[0] << 11) + 128;

    x8 = W7 * (x4 + x5);
    x4 = x8 + (W1 - W7) * x4;
    x5 = x8 - (W1 + W7) * x5;
    x8 = W3 * (x6 + x7);
    x6 = x8 - (W3 - W5) * x6;
    x7 = x8 - (W3 + W5) * x7;

    x8 = x0 + x1;
    x0 -= x1;
    x1 = W6 * (x3 + x2);
    x2 = x1 - (W2 + W6) * x2;
    x3 = x1 + (W2 - W6) * x3;
    x1 = x4 + x6;
    x4 -= x6;
    x6 = x5 + x7;
    x5 -= x7;

    x7 = x8 + x3;
    x8 -= x3;
    x3 = x0 + x2;
    x0 -= x2;
    x2 = (181 * (x4 + x5) + 128) >> 8;
    x4 = (181 * (x4 - x5) + 128) >> 8;

    blk[0] = (x7 + x1) >> 8;
    blk[1] = (x3 + x2) >> 8;
    blk[2] = (x0 + x4) >> 8;
    blk[3] = (x8 + x6) >> 8;
    blk[4] = (x8 - x6) >> 8;
    blk[5] = (x0 - x4) >> 8;
    blk[6] = (x3 - x2) >> 8;
    blk[7] = (x7 - x1) >> 8;
}

static void idctcol(int* blk)
{
    int x0, x1, x2, x3, x4, x5, x6, x7, x8;

    if(!((x1 = (blk[8 * 4] << 8)) | (x2 = blk[8 * 6]) | (x3 = blk[8 * 2]) | (x4 = blk[8 * 1]) | (x5 = blk[8 * 7]) | (x6 = blk[8 * 5]) | (x7 = blk[8 * 3])))
    {
        blk[8 * 0] = blk[8 * 1] = blk[8 * 2] = blk[8 * 3] = blk[8 * 4] = blk[8 * 5] = blk[8 * 6] = blk[8 * 7] = JPEG_Info.iclp[(blk[8 * 0] + 32) >> 6];
        return;
    }

    x0 = (blk[8 * 0] << 8) + 8192;

    x8 = W7 * (x4 + x5) + 4;
    x4 = (x8 + (W1 - W7) * x4) >> 3;
    x5 = (x8 - (W1 + W7) * x5) >> 3;
    x8 = W3 * (x6 + x7) + 4;
    x6 = (x8 - (W3 - W5) * x6) >> 3;
    x7 = (x8 - (W3 + W5) * x7) >> 3;

    x8 = x0 + x1;
    x0 -= x1;
    x1 = W6 * (x3 + x2) + 4;
    x2 = (x1 - (W2 + W6) * x2) >> 3;
    x3 = (x1 + (W2 - W6) * x3) >> 3;
    x1 = x4 + x6;
    x4 -= x6;
    x6 = x5 + x7;
    x5 -= x7;

    x7 = x8 + x3;
    x8 -= x3;
    x3 = x0 + x2;
    x0 -= x2;
    x2 = (181 * (x4 + x5) + 128) >> 8;
    x4 = (181 * (x4 - x5) + 128) >> 8;

    blk[8 * 0] = JPEG_Info.iclp[(x7 + x1) >> 14];
    blk[8 * 1] = JPEG_Info.iclp[(x3 + x2) >> 14];
    blk[8 * 2] = JPEG_Info.iclp[(x0 + x4) >> 14];
    blk[8 * 3] = JPEG_Info.iclp[(x8 + x6) >> 14];
    blk[8 * 4] = JPEG_Info.iclp[(x8 - x6) >> 14];
    blk[8 * 5] = JPEG_Info.iclp[(x0 - x4) >> 14];
    blk[8 * 6] = JPEG_Info.iclp[(x3 - x2) >> 14];
    blk[8 * 7] = JPEG_Info.iclp[(x7 - x1) >> 14];
}

/*---------------------------------------------------------------------*
 *  public functions                                                   *
 *---------------------------------------------------------------------*/

bool JPEG_Decode(uint8_t* JPG, uint8_t* Decoded, size_t* pImageSize/*, uint16_t xPos, uint16_t yPos*/)
{
    memset(&JPEG_Info, 0x00, sizeof(JPEG_Info));

    //JPEG_Info.CurX = xPos;
    //JPEG_Info.CurY = yPos;

    JPEG_Info.lpJpegBuf = (uint8_t*)JPG;
    if(pImageSize != NULL)
    {
        *pImageSize = 0;
    }

    if(InitTag() != FUNC_OK)
    {
        return false;
    }

    if((JPEG_Info.SampRate_Y_H == 0 ) || (JPEG_Info.SampRate_Y_V == 0))
    {
        return false;
    }

    if(Decode(Decoded) != FUNC_OK)
    {
        return false;
    }

    if(pImageSize != NULL)
    {
        *pImageSize = JPEG_Info.ImgSize;
    }

    return true;
}

/*---------------------------------------------------------------------*
 *  eof                                                                *
 *---------------------------------------------------------------------*/
