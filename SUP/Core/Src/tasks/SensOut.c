#include "tasks/SensOut.h"

//

uint16_t old_raw_angle = 0;
uint16_t angle = 0;
int16_t old_encoder_data = 0;
int16_t delta_encoder = 0;
uint8_t command_CMD[10] = {0};
uint8_t test_data = 0;
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
extern uint16_t current_angle;
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
		osDelay(100);
		time++;
		if(time == 3){
			calcDeltaAngle(current_angle);
			time = 0;
		}
	}
}
int16_t arr_big[10];
int16_t arr_reght[10];
int16_t arr_left[10];
uint8_t counter_big = 0;
uint8_t couunter_right = 0;
uint8_t counter_left = 0;

void calcDeltaAngle(uint16_t current_encoder_data){
	if(current_encoder_data == old_encoder_data)
		return;
	delta_encoder = current_encoder_data - old_encoder_data;
	if(delta_encoder < 10 && delta_encoder > -10)
		return;
	old_encoder_data = current_encoder_data;
	if(delta_encoder > MAX_ANGLE || delta_encoder < (-1)*MAX_ANGLE){
		arr_big[counter_big] = delta_encoder;
		counter_big++;
		if(counter_big >= 10)
			counter_big = 0;
		delta_encoder = 0;
	}
	if(delta_encoder > MIN_ANGLE ){
		arr_reght[couunter_right] = delta_encoder;
		couunter_right++;
		if(couunter_right >= 10)
			couunter_right = 0;
		encoderSetUp();
		return;
	}
	if(delta_encoder < (-1)*MIN_ANGLE){
		arr_left[counter_left] = delta_encoder;
		counter_left++;
		if(counter_left >= 10)
			counter_left = 0;
		encoderSetDown();
	}
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







