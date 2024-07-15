#ifndef INC_ENCODER_ENCODER_H_
#define INC_ENCODER_ENCODER_H_

#include <main.h>
#include "stm32f4xx_hal.h"
#include "System/commonStruct.h"

#define TRUE  (1==1)
#define FALSE (1==0)
#define MIN_ANGLE_CHANGE 10

typedef struct {
	uint8_t dum:3;
	uint8_t MH:1;
	uint8_t ML:1;
	uint8_t MD:1;
	uint8_t num:2;
}s_magnit;

typedef union{
	s_magnit state;
	uint8_t raw;
}u_magnit;

typedef struct {
	DeviceCommon common;
	u_magnit magnit;	// Состояние магнитного энкодера

    int16_t  stepCount;	// Счетчик шагов энкодера
    uint8_t  turnLeft; 	// Флаг поворота влево
    uint8_t  turnRight;	// Флаг поворота вправо

    uint16_t currentPosition;	// Текущее положение энкодера
    uint16_t previousPosition;	// Предыдущее положение энкодера

    uint8_t  buttonPressed;	// Состояние кнопки энкодера (если есть)
} sEncoder;

void initMagEncoder(sEncoder *encoder);
void updateMagEncoder(sEncoder *encoder);

#endif /* INC_ENCODER_ENCODER_H_ */
