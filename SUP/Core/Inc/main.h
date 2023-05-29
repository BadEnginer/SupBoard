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
#include "Display/e-ink/epaper.h"
#include "Display/ws2812/ARGB.h"
#include "Display/ssd1306/ssd1306.h"
#include "ADC_out/ADS1115.h"
#include "DAC_out/MCP4725.h"
#include "Encoder/as5600.h"
#include "tasks/UserTasks.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
void startDisplay();
// Блок для опредления устройств на линии
#define DIS_ADRESS 60
#define ADC_ADRESS 72
#define ENC_ADRESS 54
#define DAC_ADRESS 96

typedef struct {
	uint8_t encoder_dev:1;
	uint8_t display_dev:1;
	uint8_t ADC_dev:1;
	uint8_t DAC_dev:1;
	uint8_t unknown_dev:4;
}s_devise_i2c_tree;

uint8_t test_i2c_dev();


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
#define EXT_button_on_Pin GPIO_PIN_14
#define EXT_button_on_GPIO_Port GPIOB
#define EXT_button_on_EXTI_IRQn EXTI15_10_IRQn
#define PB3_LB_Pin GPIO_PIN_3
#define PB3_LB_GPIO_Port GPIOB
#define PB4_EB_Pin GPIO_PIN_4
#define PB4_EB_GPIO_Port GPIOB
#define PB4_EP_Pin GPIO_PIN_5
#define PB4_EP_GPIO_Port GPIOB
#define PB8_EM_Pin GPIO_PIN_8
#define PB8_EM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/*
#define RST_Pin GPIO_PIN_1
#define RST_GPIO_Port GPIOB
#define DC_Pin GPIO_PIN_2
#define DC_GPIO_Port GPIOB
#define BUSY_Pin GPIO_PIN_10
#define BUSY_GPIO_Port GPIOB
#define SPI_CS_Pin GPIO_PIN_0
#define SPI_CS_GPIO_Port GPIOB
*/

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
