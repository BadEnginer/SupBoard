
#ifndef INC_TASKS_SENSOUT_H_
#define INC_TASKS_SENSOUT_H_

//#include <main.h>
#include "stm32f4xx_hal.h"
#include "ADC_out/ADS1115.h"
#include "DAC_out/MCP4725.h"
#include "cmsis_os2.h"
#include "app_menu/app_menu.h"

extern I2C_HandleTypeDef hi2c1;

#define MAX_ANGLE 180
#define MIN_ANGLE 5
#define ON 1
#define ADS1115_ADR 72

#define ADC_CHANEL_1 0
#define ADC_CHANEL_2 1
#define ADC_CHANEL_3 2
#define ADC_CHANEL_4 3

#define SIZE_ADC_BUFF 10
#define NUM_ADC_CH 4

#define NUM_ADC 16
#define MAX_ADC_RANGE (4096 * 2) // Расчёт для режима 4 вольта
#define MAX_DATA_ADC 65536
#define ADC_TO_V (MAX_ADC_RANGE/MAX_DATA_ADC)
#define MAX_CURRENT 70
#define FULL_RANGE_CURRENT 1500
#define V_TO_A (MAX_CURRENT/FULL_RANGE_CURRENT) // 21mV на 1 А


void longButton();
void StartSensOutTask(void *argument);
void initADC(ADS1115_Config_t*);
void initChanelADC(ADS1115_Config_t*, MultiplexerConfig_t );
uint16_t getEncoderData();
void initAllChanelADC();
void calcDeltaAngle(int16_t);

typedef struct {
	uint8_t dum:3;
	uint8_t MH:1;
	uint8_t ML:1;
	uint8_t MD:1;
	uint8_t num:2;
}s_magnit;

typedef union{
	  s_magnit state_magnit;
	  uint8_t data;
}u_magnituda;

#endif /* INC_TASKS_SENSOUT_H_ */
