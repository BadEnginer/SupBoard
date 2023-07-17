
#ifndef INC_TASKS_SENSOUT_H_
#define INC_TASKS_SENSOUT_H_

#include <main.h>
#include <stdlib.h>


#define MAX_ANGLE 600
#define MIN_ANGLE 1
#define ON 1
#define MAX_DELTA (MAX_COUNTER_ANGLE-1)
#define MAX_COUNT 2
#define LONG_COUNT 15

#define CELL_4 4
#define CELL_3 3
#define BATTARY_TYPE_FE  1
#define BATTARY_TYPE_LIPO  2

#define CURREN_NUM_CELL CELL_4
#define CURRENT_BAT_TYPE BATTARY_TYPE_LIPO

void trueButtonLB();
void trueButtonEB();
void trueButtonEP();
void trueButtonEM();

uint8_t  battaryCharge(uint8_t battryType, uint8_t num_cell, uint16_t battary_voltage);
int16_t expFiltrCharge(float newVal, float k);
int16_t expFiltrVbat(float newVal, float k);
int16_t expFiltrIcurrnt(float newVal, float k);
void error_processing();
void longButton();
void StartSensOutTask(void *argument);
void calcDeltaAngle(uint16_t*);

#endif /* INC_TASKS_SENSOUT_H_ */
