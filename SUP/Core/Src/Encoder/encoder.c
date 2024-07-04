#include "Encoder/encoder.h"


// Структуры для магнитного энкодера
uint16_t current_angle;
uint16_t arr_angle[MAX_COUNTER_ANGLE];
uint8_t counterAngle = 0;
u_magnituda magnituda;
uint16_t raw_angle = 0;

// Переменные для подсчета смещения угла
volatile uint16_t temp_counter_plus = 0;
volatile uint16_t temp_counter_min = 0;
uint16_t old_raw_angle = 0;
uint16_t angle = 0;
int16_t old_encoder_data = 0;
int16_t delta_encoder = 0;

uint16_t getEncoderData(){
	SystemState.MagnitEncoderData.EncoderState.u8MagnitState = AS5600_GetStatus();
	if(SystemState.MagnitEncoderData.EncoderState.sMagnitState.MD == ON)
		raw_angle = AS5600_GetRawAngle();
	return 	raw_angle;
}

// todo заменить магические числа на дефайны
void calcDeltaAngle(uint16_t* data){
	static int32_t currentAngle = 0;
	int32_t temp = currentAngle - data[28];
	if(temp > 1000 || temp < -1000){
		currentAngle = data[28];
		return;
	}
	if(currentAngle)
	if(temp > 500 ){
		currentAngle = data[28];
		encoderSetUp();
		temp_counter_plus++;
	}
	if(temp < -500){
		currentAngle = data[28];
		encoderSetDown();
		temp_counter_min++;
	}
}

int8_t encoderData(){
	return encoderAS56;
}

void encoderSetUp(){
	SystemState.ButtonsData.EncoderPLUS = BUTTON_ON;
	SystemState.ButtonsData.EncoderPLUSCounter++;
	encoderAS56 = 1;
}
void setMaxSpeed(int8_t znak){
	SystemState.ButtonsData.SetMaxSpeed = BUTTON_ON;
	SystemState.ButtonsData.setMaxSpeed = znak;
}

void encoderSetDown(){
	SystemState.ButtonsData.EncoderMINUS = BUTTON_ON;
	SystemState.ButtonsData.EncoderMINUSCounter++;
	encoderAS56 = -1;
}

void encoderReset(){
	SystemState.ButtonsData.EncoderMINUS = BUTTON_OFF;
	SystemState.ButtonsData.EncoderPLUS = BUTTON_OFF;
	encoderAS56 = 0;
}
