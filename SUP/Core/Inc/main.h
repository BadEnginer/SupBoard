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

void test_i2c_dev();

typedef enum{
	DEVICE_NO_INIT,
	DEVICE_NO_ANSWER,
	DEVICE_READY
} eDeviceState;

typedef enum{
	UNKNOWN_PLACE,
	LOAD,
	START,
	MAIN_MENU,
	MAIN_CONFIG,
	LED_CONFIG,
	BUTTON_CONFIG,
	ADC_CONFIG,
	DAC_CONFIG,
	SYSTEM_CONFIG,
	ERROR_STATE,
	ERROR_STATE_I2C,
	WAIT_COMMAND

} eDisplayState;

typedef struct{
eDisplayState prevState;
eDisplayState state;
uint8_t i2c_addres;
eDeviceState readyDISPLAY;
uint8_t mode;
int8_t CurrentMenu;
} sDisplayState;


typedef enum eStateButton{
	BUTTON_OFF,
	BUTTON_ON
} StateButton;

typedef struct{
	StateButton ButtonEN;
		uint8_t ButtonENCounter;
	StateButton ButtonBACK;
		uint8_t ButtonBACKCounter;
	StateButton EncoderPLUS;
		uint8_t EncoderPLUSCounter;
	StateButton EncoderMINUS;
		uint8_t EncoderMINUSCounter;
} sButtonData;



typedef struct{
	float current_voltage;
	float set_voltage;
	float calc_voltage;
	uint16_t stop_cod;
	uint16_t current_code;
	uint8_t i2c_addres;
	eDeviceState readyDAC;
} sDacData;

typedef struct{
	eDeviceState readyADC;
	uint8_t i2c_addres;
	int16_t chanel_0_voltage;
	int16_t chanel_1_voltage;
	int16_t chanel_2_voltage;
	int16_t chanel_3_voltage;
} sAdcData;

typedef struct{
	eDeviceState readyBattary;
	uint8_t percentCharge;
	uint8_t MaxCellVoltage;
	uint8_t MinCellVoltage;
	uint8_t numCell;
	uint16_t voltage;
	uint16_t current; // mA
	uint16_t sumCharge;
	uint16_t workTime;
	uint16_t estimatedTime;
} sBattaryData;

typedef struct{

} sLED_STATE;

typedef struct {
	uint8_t dum:3;
	uint8_t MH: 1; // Magnit too strong
	uint8_t ML: 1; // Magnit too weak
	uint8_t MD: 1; // Magnit detected
	uint8_t num:2;
} sEncoderState;

typedef union{
	sEncoderState sMagnitState;
	uint8_t u8MagnitState;
}uEncoderState;

typedef struct{
	uEncoderState EncoderState;
	uint16_t curretn_raw_angle;
	eDeviceState readyENCODER;
	uint8_t i2c_adres;
} sMagnitEncoderData;

typedef enum {
	DEVISE_NO_ANSWER,
	DEVISE_WORK,
	MAGNIT_NO_DETECT,
	MAGNIT_TOO_STRONG,
	MAGNIT_TOO_WEAK
}eErrorEncoder;

typedef enum {
	DEVISE_OK,
	DEVISE_ERROR,
	DEVISE_FATAL_ERROR
} eStandartError;

typedef enum {
	ZERO_OK,
	ZERO_MIGRATE
} eErrorMigrationZero;

typedef struct{
	uint8_t error_mismatch_current; // когда код управления 0 а ток не 0
	uint8_t error_DAC_ADC; // DAC has one state but ADC have other state;
	uint8_t counterEncoderError;
	uint8_t errorCounter;
	eErrorEncoder error_encoder;
	eStandartError error_DISPLAY;
	eStandartError error_DAC;
	eStandartError error_ADC;
	eErrorMigrationZero ErrorMigrationZero;
	eStandartError error_LED;
	eStandartError error_Motor;
}sErrorState;

typedef struct{
	eDeviceState motorState;
	float current;
	float control_voltage;
	int8_t current_speed;
	uint8_t max_speed;
} sMotorData;

typedef struct{
	sBattaryData BattaryData;
	sDisplayState DisplayState;
	sButtonData ButtonsData;
	sMotorData MotorData;
	sErrorState ErrorState;
	sMagnitEncoderData MagnitEncoderData;
	sAdcData AdcData;
	sDacData DacData;
} sSystemState;

void ResetDacError(sSystemState* System);
void ResetAdcError(sSystemState* System);
void ResetLedError(sSystemState* System);
void ResetMotorError(sSystemState* System);
void ResetEncoderError(sSystemState* System);
void ResetAllError(sSystemState* System);
void ResetDisplayError(sSystemState* System);
void SetMotorSpeed(sSystemState* System);
void OutputErrorI2C();
float GetAdcDataVoltage(sSystemState* System, uint8_t channel_num);

uint8_t GerError(sSystemState* System);
uint8_t GetEncoderState(sSystemState* System);

uint16_t GetAdcDataMiliVoltage(sSystemState* System, uint8_t channel_num);

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
#define PB5_EP_Pin GPIO_PIN_5
#define PB5_EP_GPIO_Port GPIOB
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
