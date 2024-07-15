#include "tasks/MotorControl.h"
#include "System/SystemState.h"
extern sSystem mySystem;

/// Функция управления мотором через дак + сложные условия остановки
void StartMotorControlTask(void *argument){
	osDelay(1000);
	int8_t i = 0;
	int8_t speed = 0;
	SetSpeed(&mySystem);
	int16_t offset = 0;
	for(;;){
		osDelay(1000);
	}

}


