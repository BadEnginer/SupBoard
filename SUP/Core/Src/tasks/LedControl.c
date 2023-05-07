
#include "tasks/LedControl.h"
extern uint8_t global_color;
extern int8_t speed;
/*диодов допустим 3*8 = 24 , разрядов 240 то есть если разделить число на /10 будет 24 это количество горящих диодов */


//сложная фигня получается функции говорится сколько светодиодов горит и с какой яркостью горит последний остальные обнуляем


void StartLedControlTask(void *argument){
	ARGB_Init();  // Initialization
	ARGB_Clear();
	while (ARGB_Show() != ARGB_OK);
	ARGB_SetBrightness(50);
	ARGB_Clear(); // Clear stirp
	ARGB_SetRGB(0, 100, 0, 250);
	while (ARGB_Show() != ARGB_OK);
	HAL_Delay(3000);
	SetZeroSpeed();
	int8_t old_speed = 0;
	for(;;){
		if(old_speed != speed){ // если скорость изменится выполнить
			ARGB_Clear(); // Clear stirp
			SetColorSpeed(speed);
			while (ARGB_Show() != ARGB_OK);
			old_speed = speed;
		}
		HAL_Delay(200);

	}
}
void SetZeroSpeed(){ // функция для установки нулевого значения сокрости
	ARGB_Clear(); // Clear stirp
	ARGB_SetRGB(0, 0, 255, 0);
	while (ARGB_Show() != ARGB_OK);
}

void SetColorSpeed(int8_t currentSpeed){
	uint8_t curretn_color = BLUE;
	uint8_t percentSpeed = 0;
	if(currentSpeed < 0)
		curretn_color = YELLOW;
	if(currentSpeed == 0){
		SetZeroSpeed();
		return;
	}
	percentSpeed = ((abs(currentSpeed)) * 100.0)/MAX_SPEED;
	// Зажигает количество диодов пропорционалное скорости
	step_color(curretn_color, percentSpeed);
}

void step_color(uint8_t color, uint8_t percent){
	uint8_t num_led = (percent / PRE_FOR_ONE_LED);
	// Зажигаем все предыдущие диоды максимальной яркостью кроме последнего
	for(uint8_t i = 0; i < num_led; i++){
		set_color_led(i, color, MAX_BRIGHT);
	}
	if(percent == 100){
		set_color_led(7, RED, MAX_BRIGHT);
		return;
	}
	set_color_led((num_led), color, STEP_BRIGHT *(percent%PRE_FOR_ONE_LED));
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

/*
 * 	 // uint8_t old_data;
	 // uint8_t numOnLed;
	//  ARGB_Init();  // Initialization
	//  ARGB_Clear();
	//  while (ARGB_Show() != ARGB_OK);
	//  ARGB_SetBrightness(100);
	//  ARGB_Clear(); // Clear stirp
	/*  while (ARGB_Show() != ARGB_OK);
		ARGB_SetRGB(1, 0, 50, 0); // Set LED №1 with 255 Green
		ARGB_SetRGB(2, 50, 0, 0); // Set LED №2 with 255 Green
		ARGB_SetRGB(3, 0, 0, 50); // Set LED №3 with 255 Green
	  while (ARGB_Show() != ARGB_OK);

// todo  сделать хорошо
		// если значение цвета изменилось то
		//определить сколько стало и попытаться зажечь нужноче число диодов
		/*
		if(global_color != old_data && global_color != 0 ){
			old_data = global_color;
			if(old_data > 80)
				red = 80;
			else
				red = old_data;
			if(old_data > 160)
				green = 80;
			else if(old_data > 80)
				green = old_data - 80;
			if(old_data >= 239)
				blue = 80;
			else if(old_data > 160)
				blue = old_data - 160;
			if(red > 0)
				setRedLed(red/10, red%10);
			if(green > 0)
				setGreenLed(green/10, green%10);
			if(blue > 0)
				setBlueLed(blue/10, blue%10);
			while (ARGB_Show() != ARGB_OK);
		}


void setRedLed(uint8_t num, uint8_t last){
	uint8_t i;
	if(num > MAX_LED)
		num = MAX_LED;
	for(i = 0; i < num; i++){
		ARGB_SetRGB(i, 255, 0, 0);
	}
	if(i < (MAX_LED)){
		i++;
		ARGB_SetRGB(i, 25*last, 0, 0);
	}
	if(i < (MAX_LED)){
		while(i < (MAX_LED)){
			ARGB_SetRGB(i, 0, 0, 0);
			i++;
		}
	}
}

void setGreenLed(uint8_t num, uint8_t last){
	uint8_t i;
	for(i = 0; i < num; i++){
		ARGB_SetRGB(i, 0, 250, 0);
	}
	if(i < (MAX_LED)){
		i++;
		ARGB_SetRGB(i, 0, 25*last, 0);
	}
	if(i < (MAX_LED)){
		while(i < (MAX_LED)){
			ARGB_SetRGB(i, 0, 0, 0);
			i++;
		}
	}
}

void setBlueLed(uint8_t num, uint8_t last){
	uint8_t i;
	for(i = 0; i < num; i++){
		ARGB_SetRGB(i, 0, 0, 250);
	}
	if(i < (MAX_LED)){
		i++;
		ARGB_SetRGB(i, 0, 0, 25*last);
	}
	if(i < (MAX_LED)){
		while(i < (MAX_LED)){
			ARGB_SetRGB(i, 0, 0, 0);
			i++;
		}
	}
}
*/
