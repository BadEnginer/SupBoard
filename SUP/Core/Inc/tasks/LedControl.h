
#ifndef INC_TASKS_LEDCONTROL_H_
#define INC_TASKS_LEDCONTROL_H_

#include <main.h>

void StartLedControlTask(void *argument);
static void setRedLed(uint8_t num, uint8_t last);
static void setGreenLed(uint8_t num, uint8_t last);
static void setBlueLed(uint8_t num, uint8_t last);

#endif /* INC_TASKS_LEDCONTROL_H_ */
