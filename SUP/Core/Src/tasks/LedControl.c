
#include "tasks/LedControl.h"
extern uint8_t global_color;

/*диодов допустим 3*8 = 24 , разрядов 240 то есть если разделить число на /10 будет 24 это количество горящих диодов */

#define RED 0
#define GREEN 1
#define BLUE 2


#define MAX_LED 7

//сложная фигня получается функции говорится сколько светодиодов горит и с какой яркостью горит последний остальные обнуляем


void StartLedControlTask(void *argument){
	uint8_t red,green,blue;
	  uint8_t old_data;
	  uint8_t numOnLed;
	  ARGB_Init();  // Initialization
	  ARGB_Clear();
	  while (ARGB_Show() != ARGB_OK);
	  ARGB_SetBrightness(100);
	  ARGB_Clear(); // Clear stirp
	/*  while (ARGB_Show() != ARGB_OK);
		ARGB_SetRGB(1, 0, 50, 0); // Set LED №1 with 255 Green
		ARGB_SetRGB(2, 50, 0, 0); // Set LED №2 with 255 Green
		ARGB_SetRGB(3, 0, 0, 50); // Set LED №3 with 255 Green
	  while (ARGB_Show() != ARGB_OK);
*/
	for(;;){
		HAL_Delay(50);
		// todo  сделать хорошо
		// если значение цвета изменилось то
		//определить сколько стало и попытаться зажечь нужноче число диодов
		if(global_color != old_data){
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

	}
}


static void setRedLed(uint8_t num, uint8_t last){
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

static void setGreenLed(uint8_t num, uint8_t last){
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

static void setBlueLed(uint8_t num, uint8_t last){
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
