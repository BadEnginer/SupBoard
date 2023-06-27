
#ifndef INC_TASKS_LEDCONTROL_H_
#define INC_TASKS_LEDCONTROL_H_

#include "Display/ws2812/ARGB.h"

#define RED 0
#define GREEN 1
#define BLUE 2
#define YELLOW 3
#define WHITE 4
#define MAGENTA 5
#define CYAN 6
#define MAX_LED 8

#define START_LED_PLUS 4
#define FINISH_LED_PLUS 7

#define START_LED_MINUS 1
#define FINISH_LED_MINUS 0

#define PRE_FOR_ONE_LED (100/MAX_LED)
#define MAX_BRIGHT 250
#define STEP_BRIGHT (MAX_BRIGHT/PRE_FOR_ONE_LED)

void SetColorSpeed(int8_t currentSpeed, uint8_t max_speed, uint8_t start, uint8_t finish);
void step_color(uint8_t color, uint8_t percent, uint8_t start, uint8_t finish);
void set_color_led(uint8_t numLed, uint8_t color, uint8_t bright);
void inlineBright();

void StartLedControlTask(void *argument);
void setRedLed(uint8_t num, uint8_t last);
void setGreenLed(uint8_t num, uint8_t last);
void setBlueLed(uint8_t num, uint8_t last);

#endif /* INC_TASKS_LEDCONTROL_H_ */
