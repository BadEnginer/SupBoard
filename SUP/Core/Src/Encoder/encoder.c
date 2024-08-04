#include "Encoder/encoder.h"


// Структуры для магнитного энкодера
extern I2C_HandleTypeDef hi2c1;


void initMagEncoder(sEncoder *encoder, uint8_t adr){
	encoder->common.addressI2C = adr;

	if(AS5600_i2cReady()){
		encoder->common.error = NO_ERROR;
		encoder->common.status = STATE_DEVICE_READY;
	}
	else{
		encoder->common.error = YES_ERROR;
		encoder->common.status = STATE_DEVICE_NO_INIT;
	}
	encoder->currentPosition = 0;
}


void updateMagEncoder(sEncoder *encoder){
	encoder->magnit.raw = AS5600_GetStatus();
	if( (encoder->magnit.state.MD == TRUE)  &&
		(encoder->magnit.state.MH == FALSE) &&
		(encoder->magnit.state.ML == FALSE)   )// Если все флаги верные то сделать расчеты
	{
			encoder->common.error = NO_ERROR;
			encoder->previousPosition = encoder->currentPosition;
			encoder->currentPosition = AS5600_GetRawAngle();
			encoder->stepCount = encoder->currentPosition - encoder->previousPosition;
			if(encoder->stepCount > MIN_ANGLE_CHANGE){
				encoder->turnLeft = 1;
				encoder->turnRight = 0;
			}
			if(encoder->stepCount < (-MIN_ANGLE_CHANGE)){
				encoder->turnLeft = 0;
				encoder->turnRight = 1;
			}
	}
}





