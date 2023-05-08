#include "tasks/MotorControl.h"

extern uint16_t global_DAC;
extern osMutexId_t BlockI2CHandle;
osStatus_t statusMutexMotor;

/// Функция управления мотором через дак + сложные условия остановки
void StartMotorControlTask(void *argument){
	uint16_t old_global_DAC;
	 global_DAC = STOP_MOTOR;// это вроде 1.2 вольт todo уточнить
	 MCP4725 myMCP4725 = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A00, 3.30);
	 setDAC(myMCP4725,  global_DAC);
	for(;;){
		if(global_DAC > MAX_FORVARD_SPEED)
			global_DAC = MAX_FORVARD_SPEED;
		if(global_DAC < MAX_BACK_SPEED)
			global_DAC = MAX_BACK_SPEED;
		setDAC(myMCP4725,  expFiltrDAC(global_DAC, KOEFF_K_SLOW));
		osDelay(200);
	}
}

void setDAC(MCP4725 myMCP4725, uint16_t Vout){
	statusMutexMotor = osMutexAcquire(BlockI2CHandle, 1000);
	if(statusMutexMotor == osOK)
		MCP4725_setValue(&myMCP4725, Vout, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
	osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
}

#define KOEFF_K_SLOW 0.1
uint16_t expFiltrDAC(uint16_t newVal, float k) {
	  static uint16_t filVal = STOP_MOTOR;
	  filVal += (newVal - filVal) * k;
	  return filVal;
}
