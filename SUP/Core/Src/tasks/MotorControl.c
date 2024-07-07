#include "tasks/MotorControl.h"

extern sSystemState SystemState;
MCP4725 myMCP4725;
/// Функция управления мотором через дак + сложные условия остановки
void StartMotorControlTask(void *argument){
	SystemState.MotorData.motorState = DEVICE_NO_INIT;
	osDelay(1000);
	myMCP4725 = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A00, 3.30);
	SystemState.MotorData.motorState = DEVICE_READY;
	SystemState.MotorData.current_speed = 0;
	int16_t offset = 0;
	MCP4725_setVoltage(&myMCP4725, 1.0, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
	for(;;){

		while(SystemState.ErrorState.error_Motor == DEVISE_FATAL_ERROR){
			//setDAC(myMCP4725, STOP_ENGINE);
			osDelay(100);
		}
		if(SystemState.MotorData.current_speed < 0)
			SystemState.MotorData.control_voltage = 0;//SystemState.MotorData.control_voltage = STOP_ENGINE - ((speed_arr_minus[abs(SystemState.MotorData.current_speed)]) / 1000.0);
		else
			SystemState.MotorData.control_voltage =0;//SystemState.MotorData.control_voltage = STOP_ENGINE + ((speed_arr_plus[abs(SystemState.MotorData.current_speed)]) / 1000.0);
		 //setDAC(myMCP4725,  expFiltrDAC(SystemState.MotorData.control_voltage, KOEFF_K_SLOW));
		osDelay(80);
	}
}


