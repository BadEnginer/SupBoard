
#ifndef INC_TASKS_SENSOUT_H_
#define INC_TASKS_SENSOUT_H_

#include <main.h>


#define MAX_ANGLE 600
#define MIN_ANGLE 150
#define ON 1

void longButton();
void StartSensOutTask(void *argument);
void calcDeltaAngle(uint16_t);

#endif /* INC_TASKS_SENSOUT_H_ */
