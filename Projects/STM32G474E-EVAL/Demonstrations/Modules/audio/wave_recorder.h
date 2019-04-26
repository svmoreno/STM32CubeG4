/**
  ******************************************************************************
  * @file    wave_recorder.h
  * @author  MCD Application Team
  * @brief   WAVE recorder interface
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WAVE_RECORDER_H
#define __WAVE_RECORDER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
  
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void WaveRecorder_Demo(void);
void WaveRecorder_UserAction(uint8_t sel);

#ifdef __cplusplus
}
#endif

#endif /* __WAVE_RECORDER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
