
#ifndef INC_TASKS_LEDCONTROL_H_
#define INC_TASKS_LEDCONTROL_H_

#include <main.h>

#define RED 0
#define GREEN 1
#define BLUE 2
#define MAX_LED 7

void StartLedControlTask(void *argument);
void setRedLed(uint8_t num, uint8_t last);
void setGreenLed(uint8_t num, uint8_t last);
void setBlueLed(uint8_t num, uint8_t last);

#endif /* INC_TASKS_LEDCONTROL_H_ */
