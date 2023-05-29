
#ifndef INC_TASKS_SENSOUT_H_
#define INC_TASKS_SENSOUT_H_

#include <main.h>
#include <stdlib.h>


#define MAX_ANGLE 600
#define MIN_ANGLE 1
#define ON 1
#define MAX_DELTA (MAX_COUNTER_ANGLE-1)
#define MAX_COUNT 2

void trueButtonLB();
void trueButtonEB();
void trueButtonEP();
void trueButtonEM();


void longButton();
void StartSensOutTask(void *argument);
void calcDeltaAngle(uint16_t*);

#endif /* INC_TASKS_SENSOUT_H_ */
