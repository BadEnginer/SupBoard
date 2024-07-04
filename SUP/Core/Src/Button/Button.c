#include "Button/Button.h"

static uint8_t buttonUpper = 0;  	// Кнопка вверх нажата 1
static uint8_t buttonEnable = 0; 	// Кнопка ввода нажата 1
static uint8_t button_counter = 0;  // Количество отсчётом сколько кнопка удерживалась
static uint8_t button_long = 0;		// Кнопка ввода удержана 1
static int8_t  encoderAS56 = 0;		// Положение энкодера 1 крутят в плюс -1 крутят в минус


void longButton(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
	if(pinState == GPIO_PIN_RESET){
		buttonCounterSet();
	}
	else{
		buttonCounterReset();
	}

	if(buttonCounter() > 6){
		buttonLongSet();
		buttonCounterReset();
	}
}

uint8_t buttonUp(){
	return buttonUpper;
}
void buttonUpSet(){
	buttonUpper = 1;
}
void buttonUpReset(){
	buttonUpper = 0;
}

uint8_t buttonEn(){
	return buttonEnable;
}
void buttonEnSet(){
	SystemState.ButtonsData.ButtonEN= BUTTON_ON;
	SystemState.ButtonsData.ButtonENCounter++;
	buttonEnable = 1;
}
void buttonEnReset(){
	SystemState.ButtonsData.ButtonEN= BUTTON_OFF;
	buttonEnable = 0;
}

uint8_t buttonCounter(){
	return button_counter;
}
void buttonCounterSet(){
	button_counter++;
}

void buttonCounterReset(){
	button_counter = 0;
}

uint8_t buttonLong(){
	return button_long;
}
void buttonLongSet(){
	SystemState.ButtonsData.ButtonBACK = BUTTON_ON;
	SystemState.ButtonsData.ButtonBACKCounter++;
	button_long = 1;
}
void buttonLongReset(){
	SystemState.ButtonsData.ButtonBACK = BUTTON_OFF;
	button_long = 0;
}

void trueButtonLB(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
		if(pinState == GPIO_PIN_SET){
			countLB++;
		}
		else{
			countLB = 0;
		}
		if(countLB > MAX_COUNT){
			buttonLongSet();
			countLB = 0;
		}
}

void trueButtonEB(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
		if(pinState == GPIO_PIN_SET){
			countEB++;
		}
		else{
			countEB = 0;
		}
		if(countEB > MAX_COUNT){
			buttonEnSet();
			countEB = 0;
		}
}

void trueButtonEP(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	static uint8_t counterLong = OFF;
		if(pinState == GPIO_PIN_SET){
			countEP++;
		}
		else{
			countEP = 0;
			counterLong = OFF;
		}
		if((countEP > MAX_COUNT) && (counterLong == OFF)){
			//counterLong = ON;
			encoderSetUp();
			countEP = 0;
		}
		if((countEP > LONG_COUNT) && (counterLong == ON)){
			setMaxSpeed(1);// Добавил логигу работы для удержания кнопки если она продолжает удерживаться то продолжать увеличивать
			countEP = 0;
			counterLong = OFF;
		}
}

void trueButtonEM(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);

	static uint8_t counterLong = OFF;
		if(pinState == GPIO_PIN_SET){
			countEM++;
		}
		else{
			countEM = 0;
			counterLong = OFF;
		}
		if((countEM > MAX_COUNT) && (counterLong == OFF)){
			encoderSetDown();
			countEM = 0;
		}
		if((countEM > LONG_COUNT) && (counterLong == ON)){
			setMaxSpeed(-1); // Добавил логигу работы для удержания кнопки если она продолжает удерживаться то продолжать уменьшать
			countEM = 0;
			counterLong = OFF;
		}
}
