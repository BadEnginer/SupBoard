#include "tasks/MotorControl.h"

extern uint16_t global_DAC;


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
		if(global_DAC != old_global_DAC){
			old_global_DAC = global_DAC;
			setDAC(myMCP4725,  global_DAC);
		}
		osDelay(50);
	}
}


void setDAC(MCP4725 myMCP4725, uint16_t Vout){
	MCP4725_setValue(&myMCP4725, Vout, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
}
