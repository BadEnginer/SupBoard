#include "tasks/MotorControl.h"

extern uint16_t global_DAC;
extern osMutexId_t BlockI2CHandle;
extern sSystemState SystemState;
osStatus_t statusMutexMotor;

// 1.21v напржяние остановки
// 2.2v полный назад
// 0.4 полный вперёд
#define FULL_FORWARD 0.4F
#define FULL_BACK 2.2F
#define STOP_ENGINE 1.21F
#define KOEFF_INLINE_DAC 37 // Убераем рассолгасовние между ацп и дак

/// Функция управления мотором через дак + сложные условия остановки
void StartMotorControlTask(void *argument){
	HAL_Delay(1000);
	 global_DAC = STOP_MOTOR;// это вроде 1.2 вольт todo уточнить
	 MCP4725 myMCP4725 = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A00, 3.20);
	 setDAC(myMCP4725,  global_DAC);
	for(;;){
		if(global_DAC > MAX_FORVARD_SPEED)
			global_DAC = MAX_FORVARD_SPEED;
		if(global_DAC < MAX_BACK_SPEED)
			global_DAC = MAX_BACK_SPEED;
		setDAC(myMCP4725,  expFiltrDAC(global_DAC, KOEFF_K_SLOW));
	}
}

void setDAC(MCP4725 myMCP4725, uint16_t Vout){
	float Vvoltage = (Vout+KOEFF_INLINE_DAC)/1000.0; //
	if(Vvoltage < FULL_FORWARD)
		Vvoltage =  FULL_FORWARD;
 	if(Vvoltage > FULL_BACK)
		Vvoltage = FULL_BACK;
	statusMutexMotor = osMutexAcquire(BlockI2CHandle, 1000);
	if(statusMutexMotor == osOK)
		//MCP4725_setValue(&myMCP4725, Vout, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
		MCP4725_setVoltage(&myMCP4725, Vvoltage, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
	osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
}

#define KOEFF_K_SLOW 0.1
uint16_t expFiltrDAC(uint16_t newVal, float k) {
	  static uint16_t filVal = STOP_MOTOR;
	  filVal += (newVal - filVal) * k;
	  return filVal;
}
