#ifndef INC_ENCODER_ENCODER_H_
#define INC_ENCODER_ENCODER_H_

#include <main.h>
#include "stm32f4xx_hal.h"

extern sSystemState SystemState; // Глобальная структура где хранятся состояния всей системы

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

uint16_t getEncoderData();
void calcDeltaAngle(uint16_t*);

int8_t encoderData();
void encoderSetUp();
void encoderSetDown();
void encoderReset();

#endif /* INC_ENCODER_ENCODER_H_ */
