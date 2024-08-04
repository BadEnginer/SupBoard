
#ifndef INC_TASKS_SENSCONTROL_H_
#define INC_TASKS_SENSCONTROL_H_

#include <main.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#define MAX_ANGLE 600
#define MIN_ANGLE 1
#define ON 1
#define MAX_DELTA (MAX_COUNTER_ANGLE-1)
#define MAX_COUNT 2
#define LONG_COUNT 15
#define WAIT_ERROR_ADC_DAC 50
#define MUTEX_TIMEOUT 10000

//to do понять для чего это место )
#define CUR_PARAM_1 37174,7
#define CUR_PARAM_2 (1,7317 * CUR_PARAM_1)

// определение для софт таймеров
#define BUTTON_TIMER_PERIOD_MS  300
#define ADC_TIMER_PERIOD_MS 	10
#define ENCODER_TIMER_PERIOD_MS 300

void trueButtonLB();
void trueButtonEB();
void trueButtonEP();
void trueButtonEM();

// функиця создания таймеров
uint8_t create_timers(void);

uint8_t  battaryCharge();
int16_t expFiltrCharge(float newVal, float k);
int16_t expFiltrVbat(float newVal, float k);
int16_t expFiltrIcurrnt(float newVal, float k);
//void error_processing();

void StartSensOutTask(void *argument);


// Декларация функции обратного вызова таймеров

void vButtonTimerCallback (TimerHandle_t xTimer);
void vADCTimerCallback	  (TimerHandle_t xTimer);
void vEncoderTimerCallback(TimerHandle_t xTimer);


#endif /* INC_TASKS_SENSCONTROL_H_ */
