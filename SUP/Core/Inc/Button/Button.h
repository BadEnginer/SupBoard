#ifndef INC_BUTTON_BUTTON_H_
#define INC_BUTTON_BUTTON_H_
#include "main.h"

#define INVERT 1
#define NO_INVERT 0
#define ON_BUTTON  1
#define OFF_BUTTON 0
#define LONG_BUTTON_THRESHOLD 5
#define SHORT_BUTTON_THRESHOLD 2

typedef struct{
	uint8_t currentState:1;
	uint8_t previousState:1;
	uint8_t shortPress:1;
	uint8_t longPress:1;
	uint8_t invertState:4;
	uint8_t holdCounter;
	GPIO_TypeDef* GPIO;
	uint16_t Pin;
}sButton;

void updateButtonState(sButton* button);
GPIO_PinState reversPinState(GPIO_PinState state);
void init_button(sButton* button, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t invert);
void resetLongButton(sButton* button);
void resetShortButton(sButton* button);
uint8_t LongButtonState(sButton* button);
uint8_t ShortButtonState(sButton* button);
#endif /* INC_BUTTON_BUTTON_H_ */
