
#include "tasks/LedControl.h"
extern uint8_t global_color;
extern int8_t speed;
/*диодов допустим 3*8 = 24 , разрядов 240 то есть если разделить число на /10 будет 24 это количество горящих диодов */


//сложная фигня получается функции говорится сколько светодиодов горит и с какой яркостью горит последний остальные обнуляем


void setAllColor(uint8_t color,uint8_t bright, uint16_t delay){
	for(uint8_t i = 0; i < MAX_LED; i++){ // Загрузка
		switch(color){
			case BLUE: 	  ARGB_SetRGB(i, 0, 0, bright); break;
			case RED: 	  ARGB_SetRGB(i, bright, 0, 0); break;
			case GREEN:   ARGB_SetRGB(i, 0, bright, 0); break;
			case YELLOW:  ARGB_SetRGB(i, bright/2, bright/2, 0); break;
			case MAGENTA: ARGB_SetRGB(i, bright/2, 0, bright/2); break;
			case WHITE:   ARGB_SetRGB(i, bright/3, bright/3, bright/3); break;
			case CYAN:    ARGB_SetRGB(i, 0, bright/2, bright/2); break;
			default: 	  ARGB_SetRGB(i, bright/3, bright/3, bright/3);
		}
	if(delay > 0) HAL_Delay(delay);
	}
	while (ARGB_Show() != ARGB_OK);
}

void blink_led(uint8_t color,uint8_t times, uint8_t delay){
	for(uint8_t i = 0; i < times; i++){
		setAllColor(color, 255, 0);// Загрузка
		HAL_Delay(delay);
		ARGB_Clear();
		while (ARGB_Show() != ARGB_OK);
		HAL_Delay(delay);
	}
}

void StartLedControlTask(void *argument){
	uint8_t set_mode = 0;
	ARGB_Init();  // Initialization
	ARGB_Clear();
	while (ARGB_Show() != ARGB_OK);
	ARGB_SetBrightness(100); // Максимальная яркость 255
	ARGB_Clear(); // Clear stirp

	setAllColor(GREEN, 255, 300);// Загрузка
	blink_led(GREEN, 3, 200);

	for(;;){
		SetChargeLed(SystemState.BattaryData.percentCharge);
		SetSpeedLed(SystemState.MotorData.current_speed);
		HAL_Delay(200);
	}
}

void SetChargeLed(uint8_t charge_proc){

}
void SetSpeedLed(int8_t speed){
	if(speed == 0){
		SetSpeedLedPlus(0);
		SetSpeedLedMinus(0);
		return;
	}
	if(speed > 0){
		SetSpeedLedPlus(speed);
		SetSpeedLedMinus(0);
		return;
	}
	if(speed < 0){
		SetSpeedLedPlus(0);
		SetSpeedLedMinus(speed);
		return;
	}
}
void SetSpeedLedPlus(int8_t speed){
	SetColorSpeed(speed, SystemState.MotorData.max_speed, START_LED_PLUS, FINISH_LED_PLUS);
}
void SetSpeedLedMinus(speed){
	SetColorSpeed(speed, SystemState.MotorData.max_speed, START_LED_MINUS, FINISH_LED_MINUS);
}

void SetZeroSpeed(){ // функция для установки нулевого значения сокрости
	ARGB_Clear(); // Clear stirp
	ARGB_SetRGB(0, 0, 255, 0);
	while (ARGB_Show() != ARGB_OK);
}



void SetColorSpeed(int8_t currentSpeed, uint8_t max_speed, uint8_t start, uint8_t finish ){
	uint8_t curretn_color;
	if(currentSpeed = 0)
		curretn_color = WHITE;
	if(currentSpeed > 0){
		curretn_color = GREEN;
	}
	else if(currentSpeed < 0){
		curretn_color = BLUE;
	}
	percentSpeed = ((abs(currentSpeed)) * 100.0)/max_speed;
	// Зажигает количество диодов пропорционалное скорости
	step_color(curretn_color, percentSpeed, start, finish);
}

void step_color(uint8_t color, uint8_t percent, uint8_t start, uint8_t finish){
	uint8_t all_diod_ready = (abs(finish - start))+1;
	uint8_t i = start;
	uint16_t lenght_for_per = (all_diod_ready * 250)/100;// количество диодов умножаем на максимальную яркость и делим на 100 процентов будет яркость на 1 процент
	int16_t lenght = percent * lenght_for_per;
	uint8_t bright = 0;
	// хочу сделать инверсию красивую для перевернутых чисел
	//for(uint8_t i = start; i <= finish; i++){
	while(all_diod_ready){
		if(lenght > 250){
			set_color_led(i, color, MAX_BRIGHT);
			lenght-=250;
		}
		else if( lenght > 0)
				set_color_led(i, color, lenght);
			else
				set_color_led(i, color, 0);
		if((finish - start) > 0)
			i++;
		else
			i--;
		all_diod_ready--;
	}
}

void set_color_led(uint8_t numLed, uint8_t color, uint8_t bright){
	if(color == BLUE){
		ARGB_SetRGB(numLed, 0, 0, bright);
		return;
	}
	if(color == RED){
		ARGB_SetRGB(numLed, bright, 0, 0);
		return;
	}
	if(color == GREEN){
		ARGB_SetRGB(numLed, 0, bright, 0);
		return;
	}
	if(color == YELLOW){
		ARGB_SetRGB(numLed, bright/2, bright/2, 0);
		return;
	}
	if(color == MAGENTA){
		ARGB_SetRGB(numLed, bright/2, 0, bright/2);
		return;
	}
	if(color == WHITE){
		ARGB_SetRGB(numLed, bright/3, bright/3, bright/3);
		return;
	}
	if(color == CYAN){
		ARGB_SetRGB(numLed, 0, bright/2, bright/2);
		return;
	}
	ARGB_SetRGB(numLed, bright/3, bright/3, bright/3);
	return;
}


