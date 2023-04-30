#include "tasks/SensOut.h"

//

uint16_t old_raw_angle = 0;
uint16_t angle = 0;
int16_t old_encoder_data = 0;
int16_t delta_encoder = 0;
uint8_t command_CMD[10] = {0};
uint8_t test_data = 0;
int16_t arr_delta_angle[MAX_DELTA];
uint8_t counter = 0;

extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
extern uint16_t current_angle;
extern uint16_t arr_angle[MAX_COUNTER_ANGLE];
char* tr_data;
char symData[8] = {'L','a','s','t','-'};
uint8_t time;
// Задача для опросо кнопок, энкодера и система команд от usb
void StartSensOutTask(void *argument){
	for(;;){
		if(command_CMD[0] != 0){ // Самоя простая система команда из палок и прочего
			switch(command_CMD[0] - 48){ // преобразуем символ в число
				case 1: buttonEnSet();    break;
				case 2: buttonLongSet();  break;
				case 3: encoderSetUp();   break;
				case 4: encoderSetDown(); break;
			}//todo добавить больше данных
			test_data =command_CMD[0];
			command_CMD[0] = 0;
		    itoa((test_data-48),symData+5,10);
		    symData[7] = '\r';
			CDC_Transmit_FS(symData, sizeof(symData));
		}
		longButton();
		calcDeltaAngle(arr_angle);
		osDelay(100);
	}
}

void calcDeltaAngle(uint16_t* data){
	int16_t deltaAngle = 0;
	uint8_t plus = 0;
	counter = 0;
	for(uint8_t i = 0; i < MAX_DELTA; i++){
		deltaAngle = data[i+1] - data[i];
		// логика простая если число в нужном диапазоне то добавить его в массив
		if( ((deltaAngle > MIN_ANGLE) || (deltaAngle < ((-1)*MIN_ANGLE)))
		  &&((deltaAngle < MAX_ANGLE) || (deltaAngle > ((-1)*MAX_ANGLE)))){
			arr_delta_angle[counter] = 	deltaAngle;
			counter++;
			if(counter >=MAX_DELTA)
				counter = 0;
		}
	}
	if(counter < 6)
		return;
	for(uint8_t i = 0; i < MAX_DELTA; i++){
		if(arr_delta_angle[i] > 0)
			plus++;
		if(arr_delta_angle[i] < 0)
			plus--;
	}
	if(plus > 4)
		encoderSetUp();
	if(plus < -4)
		encoderSetDown();
}

void longButton(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
	if(pinState == GPIO_PIN_RESET){
		buttonCounterSet();
	}
	else{
		buttonCounterReset();
	}

	if(buttonCounter() > 10){
		buttonLongSet();
		buttonCounterReset();
	}
}







