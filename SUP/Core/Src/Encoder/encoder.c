#include "Encoder/encoder.h"


// Структуры для магнитного энкодера



void initMagEncoder(sEncoder *encoder){
	encoder->currentPosition = 0;
}


void updateMagEncoder(sEncoder *encoder){
	encoder->magnituda.data = AS5600_GetStatus(); // Если все флаги верные то сделать расчеты
	if( encoder->magnituda.magnit.MD == TRUE  &&
		encoder->magnituda.magnit.MH == FALSE &&
		encoder->magnituda.magnit.ML == FALSE){
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





