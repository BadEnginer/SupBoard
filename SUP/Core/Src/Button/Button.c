#include "Button/Button.h"

void init_button(sButton* button, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t invert, ButtonID id){
	button->GPIO = GPIOx;
	button->Pin  = GPIO_Pin;
	button->id = id;
	button->invertState = invert;
	button->currentState  = 0;
	button->previousState = 0;
	button->longPress  = 0;
	button->shortPress = 0;
	button->currentState = 0;
}

GPIO_PinState reversPinState(GPIO_PinState state){
	return (state == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
}

void updateButtonState(sButton* button){
	GPIO_PinState state = HAL_GPIO_ReadPin(button->GPIO, button->Pin);
	if((button->invertState == ON_BUTTON)) // инвертирует состояние если соответствущий бит включен
		state = reversPinState(state);

	button->previousState = button->currentState;
	if(state == GPIO_PIN_SET){
		button->currentState = ON_BUTTON;
		button->shortPress = ON_BUTTON;
		if(button->previousState == ON_BUTTON)
			button->holdCounter++;
	}
	else{
		button->currentState = OFF_BUTTON;
		button->holdCounter = 0;
	}
	if(button->holdCounter >= LONG_BUTTON_THRESHOLD){
		button->holdCounter = 0;
		button->longPress = ON_BUTTON;
	}
}

void resetLongButton(sButton* button){
	return button->longPress = OFF_BUTTON;
}

void resetShortButton(sButton* button){
	return button->longPress = OFF_BUTTON;
}

uint8_t LongButtonState(sButton* button){
	return button->longPress;
}

uint8_t ShortButtonState(sButton* button){
	return button->shortPress;
}

void initAllButton(sButton* button){
	init_button(&button[A], GPIOA, GPIO_PIN_15, NO_INVERT, A);
	init_button(&button[B], GPIOB, GPIO_PIN_3,  NO_INVERT, B);
	init_button(&button[C], GPIOB, GPIO_PIN_4,  NO_INVERT, C);
	init_button(&button[D], GPIOB, GPIO_PIN_5,  NO_INVERT, D);
}
