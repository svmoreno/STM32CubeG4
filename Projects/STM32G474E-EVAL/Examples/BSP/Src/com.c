/**
  ******************************************************************************
  * @file    BSP/Src/com.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the com feature in the
  *          STM32G474E EVAL driver
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
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32G4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Com demo
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Com_demo(void)
{
  int32_t         result = 0;
#if (USE_BSP_COM_FEATURE > 0 )
  COM_InitTypeDef COM_Init;

  /* Initialize COM init structure */
  COM_Init.BaudRate   = 38400;
  COM_Init.WordLength = UART_WORDLENGTH_8B;
  COM_Init.StopBits   = COM_STOPBITS_1;
  COM_Init.Parity     = COM_PARITY_NONE;
  COM_Init.HwFlowCtl  = COM_HWCONTROL_NONE;

  /****************/
  /* Test of COM1 */
  /****************/
  /* Initialize COM */
  if (BSP_COM_Init(COM1, &COM_Init) != BSP_ERROR_NONE) result--;

#if (USE_COM_LOG == 1)
  /* Set COM used for log */
  if (BSP_COM_SelectLogPort(COM1) != BSP_ERROR_NONE) result--;
#endif

  /* Print data on COM */
  printf("TEST OF COM1\n");

  /* De-initialize COM */
  if (BSP_COM_DeInit(COM1) != BSP_ERROR_NONE) result--;
#endif /* USE_BSP_COM_FEATURE */
  return result;
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
