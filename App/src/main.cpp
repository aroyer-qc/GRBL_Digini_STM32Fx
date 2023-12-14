//-------------------------------------------------------------------------------------------------
//
//  File : main.cpp
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

#include "lib_digini.h"
#include "bsp.h"
#include "Task_grbl.h"
#include "Task_loading.h"
#include "Task_network.h"
#include "Task_comm.h"

static void MPU_Config(void);

//-------------------------------------------------------------------------------------------------
//
// Name:           main
// Parameter(s):   void
// Return:         int
//
// Description:    main() what more can be said
//
// Note(s):        Here we create the task that will start all the other
//
//-------------------------------------------------------------------------------------------------
int main()
{
    Language_e Language;

    // Configure the MPU attributes as Device memory for ETH DMA descriptors and buffer
    MPU_Config();


  #if (DIGINI_USE_STACKTISTIC == DEF_ENABLED)
    myStacktistic.Initialize();                             // Initialize the Stack Check process before any task
  #endif

    nOS_Init();
    BSP_Initialize();                                       // All hardware and system initialization

    nOS_Start();
    BSP_PostOS_Initialize();
    pTaskCOMM->Initialize();

    //pTaskLoading->Initialize();

  #if (DIGINI_USE_ETHERNET == DEF_ENABLED)
    pLWIP_App->Initialize();
    //pTaskNetwork->Initialize();
  #endif
   // pTaskGRBL->Initialize();

    DB_Central.Get(&Language, SYSTEM_LANGUAGE, 0, 0);
    // = LANG_FRENCH;
    myLabel.SetLanguage(Language);


  #ifdef DEBUG
    DateAndTime_t DateTime;

    DateTime.Date.Day    = 31;
    DateTime.Date.Month  = 7;
    DateTime.Date.Year   = 2023;
    DateTime.Time.Hour   = 15;
    DateTime.Time.Minute = 30;
    DateTime.Time.Second = 1;
    LIB_SetDateAndTime(&DateTime);
  #endif

    for(;;)                                 // It is the idle task..
    {
        pTaskCOMM->Process(); // should move this to own task! so option to run as a task or this as a process
      #if (DIGINI_USE_ETHERNET == DEF_ENABLED)
        pLWIP_App->Process();
        //pTaskNetwork->Process();
      #endif

        //nOS_Yield();
    }

    return 0;                               // will never return
}

//-------------------------------------------------------------------------------------------------


typedef struct
{
  uint8_t                Enable;                /*!< Specifies the status of the region.
                                                     This parameter can be a value of @ref CORTEX_MPU_Region_Enable                 */
  uint8_t                Number;                /*!< Specifies the number of the region to protect.
                                                     This parameter can be a value of @ref CORTEX_MPU_Region_Number                 */
  uint32_t               BaseAddress;           /*!< Specifies the base address of the region to protect.                           */
  uint8_t                Size;                  /*!< Specifies the size of the region to protect.
                                                     This parameter can be a value of @ref CORTEX_MPU_Region_Size                   */
  uint8_t                SubRegionDisable;      /*!< Specifies the number of the subregion protection to disable.
                                                     This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF    */
  uint8_t                TypeExtField;          /*!< Specifies the TEX field level.
                                                     This parameter can be a value of @ref CORTEX_MPU_TEX_Levels                    */
  uint8_t                AccessPermission;      /*!< Specifies the region access permission type.
                                                     This parameter can be a value of @ref CORTEX_MPU_Region_Permission_Attributes  */
  uint8_t                DisableExec;           /*!< Specifies the instruction access status.
                                                     This parameter can be a value of @ref CORTEX_MPU_Instruction_Access            */
  uint8_t                IsShareable;           /*!< Specifies the shareability status of the protected region.
                                                     This parameter can be a value of @ref CORTEX_MPU_Access_Shareable              */
  uint8_t                IsCacheable;           /*!< Specifies the cacheable status of the region protected.
                                                     This parameter can be a value of @ref CORTEX_MPU_Access_Cacheable              */
  uint8_t                IsBufferable;          /*!< Specifies the bufferable status of the protected region.
                                                     This parameter can be a value of @ref CORTEX_MPU_Access_Bufferable             */
}MPU_Region_InitTypeDef;


void HAL_MPU_ConfigRegion(MPU_Region_InitTypeDef *MPU_Init)
{
  /* Set the Region number */
  MPU->RNR = MPU_Init->Number;

  if((MPU_Init->Enable) != RESET)
  {

    MPU->RBAR = MPU_Init->BaseAddress;
    MPU->RASR = ((uint32_t)MPU_Init->DisableExec             << MPU_RASR_XN_Pos)   |
                ((uint32_t)MPU_Init->AccessPermission        << MPU_RASR_AP_Pos)   |
                ((uint32_t)MPU_Init->TypeExtField            << MPU_RASR_TEX_Pos)  |
                ((uint32_t)MPU_Init->IsShareable             << MPU_RASR_S_Pos)    |
                ((uint32_t)MPU_Init->IsCacheable             << MPU_RASR_C_Pos)    |
                ((uint32_t)MPU_Init->IsBufferable            << MPU_RASR_B_Pos)    |
                ((uint32_t)MPU_Init->SubRegionDisable        << MPU_RASR_SRD_Pos)  |
                ((uint32_t)MPU_Init->Size                    << MPU_RASR_SIZE_Pos) |
                ((uint32_t)MPU_Init->Enable                  << MPU_RASR_ENABLE_Pos);
  }
  else
  {
    MPU->RBAR = 0x00;
    MPU->RASR = 0x00;
  }
}

/** @defgroup CORTEX_SysTick_clock_source CORTEX _SysTick clock source
  * @{
  */
#define SYSTICK_CLKSOURCE_HCLK_DIV8    ((uint32_t)0x00000000U)
#define SYSTICK_CLKSOURCE_HCLK         ((uint32_t)0x00000004U)

/**
  * @}
  */

//#if (__MPU_PRESENT == 1)
/** @defgroup CORTEX_MPU_HFNMI_PRIVDEF_Control MPU HFNMI and PRIVILEGED Access control
  * @{
  */
#define  MPU_HFNMI_PRIVDEF_NONE      ((uint32_t)0x00000000U)
#define  MPU_HARDFAULT_NMI           ((uint32_t)0x00000002U)
#define  MPU_PRIVILEGED_DEFAULT      ((uint32_t)0x00000004U)
#define  MPU_HFNMI_PRIVDEF           ((uint32_t)0x00000006U)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Enable CORTEX MPU Region Enable
  * @{
  */
#define  MPU_REGION_ENABLE     ((uint8_t)0x01U)
#define  MPU_REGION_DISABLE    ((uint8_t)0x00U)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Instruction_Access CORTEX MPU Instruction Access
  * @{
  */
#define  MPU_INSTRUCTION_ACCESS_ENABLE      ((uint8_t)0x00U)
#define  MPU_INSTRUCTION_ACCESS_DISABLE     ((uint8_t)0x01U)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Access_Shareable CORTEX MPU Instruction Access Shareable
  * @{
  */
#define  MPU_ACCESS_SHAREABLE        ((uint8_t)0x01U)
#define  MPU_ACCESS_NOT_SHAREABLE    ((uint8_t)0x00U)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Access_Cacheable CORTEX MPU Instruction Access Cacheable
  * @{
  */
#define  MPU_ACCESS_CACHEABLE         ((uint8_t)0x01U)
#define  MPU_ACCESS_NOT_CACHEABLE     ((uint8_t)0x00U)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Access_Bufferable CORTEX MPU Instruction Access Bufferable
  * @{
  */
#define  MPU_ACCESS_BUFFERABLE         ((uint8_t)0x01U)
#define  MPU_ACCESS_NOT_BUFFERABLE     ((uint8_t)0x00U)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_TEX_Levels MPU TEX Levels
  * @{
  */
#define  MPU_TEX_LEVEL0    ((uint8_t)0x00U)
#define  MPU_TEX_LEVEL1    ((uint8_t)0x01U)
#define  MPU_TEX_LEVEL2    ((uint8_t)0x02U)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Size CORTEX MPU Region Size
  * @{
  */
#define   MPU_REGION_SIZE_32B      ((uint8_t)0x04U)
#define   MPU_REGION_SIZE_64B      ((uint8_t)0x05U)
#define   MPU_REGION_SIZE_128B     ((uint8_t)0x06U)
#define   MPU_REGION_SIZE_256B     ((uint8_t)0x07U)
#define   MPU_REGION_SIZE_512B     ((uint8_t)0x08U)
#define   MPU_REGION_SIZE_1KB      ((uint8_t)0x09U)
#define   MPU_REGION_SIZE_2KB      ((uint8_t)0x0AU)
#define   MPU_REGION_SIZE_4KB      ((uint8_t)0x0BU)
#define   MPU_REGION_SIZE_8KB      ((uint8_t)0x0CU)
#define   MPU_REGION_SIZE_16KB     ((uint8_t)0x0DU)
#define   MPU_REGION_SIZE_32KB     ((uint8_t)0x0EU)
#define   MPU_REGION_SIZE_64KB     ((uint8_t)0x0FU)
#define   MPU_REGION_SIZE_128KB    ((uint8_t)0x10U)
#define   MPU_REGION_SIZE_256KB    ((uint8_t)0x11U)
#define   MPU_REGION_SIZE_512KB    ((uint8_t)0x12U)
#define   MPU_REGION_SIZE_1MB      ((uint8_t)0x13U)
#define   MPU_REGION_SIZE_2MB      ((uint8_t)0x14U)
#define   MPU_REGION_SIZE_4MB      ((uint8_t)0x15U)
#define   MPU_REGION_SIZE_8MB      ((uint8_t)0x16U)
#define   MPU_REGION_SIZE_16MB     ((uint8_t)0x17U)
#define   MPU_REGION_SIZE_32MB     ((uint8_t)0x18U)
#define   MPU_REGION_SIZE_64MB     ((uint8_t)0x19U)
#define   MPU_REGION_SIZE_128MB    ((uint8_t)0x1AU)
#define   MPU_REGION_SIZE_256MB    ((uint8_t)0x1BU)
#define   MPU_REGION_SIZE_512MB    ((uint8_t)0x1CU)
#define   MPU_REGION_SIZE_1GB      ((uint8_t)0x1DU)
#define   MPU_REGION_SIZE_2GB      ((uint8_t)0x1EU)
#define   MPU_REGION_SIZE_4GB      ((uint8_t)0x1FU)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Permission_Attributes CORTEX MPU Region Permission Attributes
  * @{
  */
#define  MPU_REGION_NO_ACCESS      ((uint8_t)0x00U)
#define  MPU_REGION_PRIV_RW        ((uint8_t)0x01U)
#define  MPU_REGION_PRIV_RW_URO    ((uint8_t)0x02U)
#define  MPU_REGION_FULL_ACCESS    ((uint8_t)0x03U)
#define  MPU_REGION_PRIV_RO        ((uint8_t)0x05U)
#define  MPU_REGION_PRIV_RO_URO    ((uint8_t)0x06U)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Number CORTEX MPU Region Number
  * @{
  */
#define  MPU_REGION_NUMBER0    ((uint8_t)0x00U)
#define  MPU_REGION_NUMBER1    ((uint8_t)0x01U)
#define  MPU_REGION_NUMBER2    ((uint8_t)0x02U)
#define  MPU_REGION_NUMBER3    ((uint8_t)0x03U)
#define  MPU_REGION_NUMBER4    ((uint8_t)0x04U)
#define  MPU_REGION_NUMBER5    ((uint8_t)0x05U)
#define  MPU_REGION_NUMBER6    ((uint8_t)0x06U)
#define  MPU_REGION_NUMBER7    ((uint8_t)0x07U)


//  Configure the MPU attributes
static void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

// Disable the MPU
  //  HAL_MPU_Disable();
    /* Make sure outstanding transfers are done */
  __DMB();

  /* Disable fault exceptions */
  SCB->SHCSR &= ~SCB_SHCSR_MEMFAULTENA_Msk;

  /* Disable the MPU and clear the control register*/
  MPU->CTRL = 0;





    // Configure the MPU as Strongly ordered for not defined regions
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x00;
    MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x87;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU as Device for Ethernet Descriptors at the end of SRAM1 */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x2004BE00;
    MPU_InitStruct.Size = MPU_REGION_SIZE_512B;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    // Configure the MPU as Normal Non Cacheable for Ethernet Buffers in the SRAM2
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x2004C000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU attributes as WT for the SDRAM */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0xC0000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER3;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU attributes FMC control registers */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0xA0000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_8KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER4;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x0;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Enable the MPU */
  //  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
  /* Enable the MPU */
  MPU->CTRL = MPU_PRIVILEGED_DEFAULT | MPU_CTRL_ENABLE_Msk;

  /* Enable fault exceptions */
  SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

  /* Ensure MPU setting take effects */
  __DSB();
  __ISB();



}
