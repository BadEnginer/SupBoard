
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
