/**
  ******************************************************************************
  * @file    BSP/Src/idd.c
  * @author  MCD Application Team
  * @brief   This example discribe how to configure the IDD current measurment 
  *          in the STM32G474E EVAL driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#if (USE_BSP_IO_CLASS == 1)

/** @addtogroup STM32G4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define IDD_VALUE_STRING_SIZE 4
#define IDD_UNIT_STRING_SIZE 2

typedef struct
{
  char value[IDD_VALUE_STRING_SIZE + 1];  /* 3 significant digit only */
  char unit[IDD_UNIT_STRING_SIZE + 1];    /* 2 letters */
  uint32_t value_na;                      /* value in nano amps */
  uint8_t error_code;                     /* 0 means no error else see mfx documentation
                                             In case of error please retry */
} Iddvalue_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

__IO uint32_t IddOnGoing = 0;

/* Private functions ---------------------------------------------------------*/
static void Idd_Convert(uint32_t Value, Iddvalue_t * idd);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int32_t Idd_demo(void)
{
  uint32_t IddValue;
  int32_t status;
  Iddvalue_t idd;
  uint32_t sav_systick;

  printf("--- Test IDD in WFI mode ---, Check JP13 is on 1-2 IDD.\n");
  printf("Expected value ~ 5 mA, please wait 3 seconds...\n");

  /*## IDD Initialization ############################*/
  if(BSP_IDD_Init(0) < 0)
  {
    Error_Handler();
  }

  if(BSP_IDD_EnableIT(0) < 0)
  {
    Error_Handler();
  }

  if(BSP_IDD_StartMeasurement(0) < 0)
  {
    Error_Handler();
  }

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Suspend HAL tick irq */
  HAL_SuspendTick();

  IddOnGoing = 1;

  __WFI();

  /* Resume HAL tick irq */
  HAL_ResumeTick();

  while(IddOnGoing == 1);

  if(BSP_IDD_ClearIT(0) < 0)
  {
    Error_Handler();
  }

  if(BSP_IDD_GetValue(0, &IddValue) < 0)
  {
    Error_Handler();
  }

  /* Fill value in Nano amps */
  idd.value_na = 10 * IddValue;

  /* Convert Idd value in order to display it on LCD glass */
  Idd_Convert(IddValue, &idd);

  printf("Measured value = %s %s\n", idd.value, idd.unit);

  if(BSP_IDD_DisableIT(0) < 0)
  {
    Error_Handler();
  }

  if(BSP_IDD_DeInit(0) < 0)
  {
    Error_Handler();
  }


  /*## IDD Config ############################*/
  printf("--- Test IDD config in RUN Mode ---\n");

  if(BSP_IDD_Init(0) < 0)
  {
    Error_Handler();
  }

  if(BSP_IDD_EnableIT(0) < 0)
  {
    Error_Handler();
  }

  for (uint32_t j= 0; j<3; j++)
  {
    for (uint32_t i= 0; i<8; i++)
    {
      BSP_IDD_Config_t IddConfig;

      switch (j)
      {
        case 0:
            IddConfig.PreDelay = 10 * i * i * i;
            IddConfig.AcquisitionDuration = 10 * i * i * i;
          break;
        case 1:
            IddConfig.PreDelay = 10 * i * i * i;
            IddConfig.AcquisitionDuration = 0;
          break;
        case 2:
            IddConfig.PreDelay = 0;
            IddConfig.AcquisitionDuration = 20 * i * i * i;
          break;
        default:
            Error_Handler();
            break;
      }

      printf("PreDelay = %04d ms; AcquisitionDuration = %04d ms ", IddConfig.PreDelay, IddConfig.AcquisitionDuration);

      status = BSP_IDD_Config(0, &IddConfig);

      if(status < 0)
      {
        if (status == BSP_ERROR_WRONG_PARAM)
        {
          printf("BSP_IDD_Config Bad parameter\n");
        }
        else
        {
          Error_Handler();
        }
      }

      HAL_Delay(200);

      sav_systick = HAL_GetTick();

      if(BSP_IDD_StartMeasurement(0) < 0)
      {
        Error_Handler();
      }

      if(status == BSP_ERROR_NONE)
      {
        IddOnGoing = 1;

        while(IddOnGoing == 1);

        if(BSP_IDD_ClearIT(0) < 0)
        {
          Error_Handler();
        }

        if(BSP_IDD_GetValue(0, &IddValue) < 0)
        {
          Error_Handler();
        }

        /* Fill value in Nano amps */
        idd.value_na = 10 * IddValue;

        /* Convert Idd value in order to display it on LCD glass */
        Idd_Convert(IddValue, &idd);

        printf("Measured value = %s %s ", idd.value, idd.unit);
        printf("Expected time = %04d ms; Elapsed Time = %04d ms\n", \
          IddConfig.PreDelay + IddConfig.AcquisitionDuration, \
          HAL_GetTick() - sav_systick \
          );
      }
    }
  }

  if(BSP_IDD_DisableIT(0) < 0)
  {
    Error_Handler();
  }

  if(BSP_IDD_DeInit(0) < 0)
  {
    Error_Handler();
  }

  printf("--- End of Test IDD ---\n\n");


  return 0;
}

/**
  * @brief  Convert value to display correct amper unit.
  * @param  None
  * @retval None
  */
static void Idd_Convert(uint32_t Value, Iddvalue_t * idd)
{
  float TempIddDisplay = 0;
  idd->value[0]=0;
  idd->unit[0]=0;

  TempIddDisplay = (float) Value * 10;

  if (TempIddDisplay < 1000){  /* Value in nano amps */
    sprintf(idd->value, "%.0f", TempIddDisplay);
    sprintf(idd->unit, "nA");
  }else{  /* Value in micro amps */
    TempIddDisplay = TempIddDisplay / 1000;
    if (TempIddDisplay < 10){
      sprintf(idd->value, "%.2f", TempIddDisplay);
      sprintf(idd->unit, "uA");
    }else if (TempIddDisplay < 100){
      sprintf(idd->value, "%.1f", TempIddDisplay);
      sprintf(idd->unit, "uA");
    }else if (TempIddDisplay < 1000){
      sprintf(idd->value, "%.0f", TempIddDisplay);
      sprintf(idd->unit, "uA");
    }else{ /* Value in milli Amp */
      TempIddDisplay = TempIddDisplay/1000;
      if (TempIddDisplay < 10){
        sprintf(idd->value, "%.2f", TempIddDisplay);
        sprintf(idd->unit, "mA");
      }else if (TempIddDisplay < 100){
        sprintf(idd->value, "%.1f", TempIddDisplay);
        sprintf(idd->unit, "mA");
      }else if (TempIddDisplay < 1000){
        sprintf(idd->value, "%.0f", TempIddDisplay);
        sprintf(idd->unit, "mA");
      }
    }
  }
}


/**
  * @brief  EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void BSP_IDD_Callback()
{
  IddOnGoing = 0;
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* #if (USE_BSP_IO_CLASS == 1) */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
