/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EPD_CS_Pin GPIO_PIN_0
#define EPD_CS_GPIO_Port GPIOB
#define EPD_Reset_Pin GPIO_PIN_1
#define EPD_Reset_GPIO_Port GPIOB
#define EPD_Data_Control_Pin GPIO_PIN_2
#define EPD_Data_Control_GPIO_Port GPIOB
#define EPD_Busy_Pin GPIO_PIN_10
#define EPD_Busy_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define RST_Pin EPD_Reset_Pin
#define RST_GPIO_Port GPIOB
#define DC_Pin EPD_Data_Control_Pin
#define DC_GPIO_Port GPIOB
#define BUSY_Pin EPD_Busy_Pin
#define BUSY_GPIO_Port GPIOB
#define SPI_CS_Pin EPD_CS_Pin
#define SPI_CS_GPIO_Port GPIOB
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
