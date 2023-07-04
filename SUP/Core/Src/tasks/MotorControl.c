#include "tasks/MotorControl.h"

extern uint16_t global_DAC;
extern osMutexId_t BlockI2CHandle;
extern sSystemState SystemState;
osStatus_t statusMutexMotor;
void setDAC(MCP4725, float);
// 1.21v напржяние остановки
// 2.2v полный назад
// 0.4 полный вперёд
#define FULL_FORWARD 0.3F
#define FULL_BACK 2.1F
#define STOP_ENGINE 1.20F
#define KOEFF_INLINE_DAC 37 // Убераем рассолгасовние между ацп и дак

//uint8_t speed_arr[] = {0,200,50,60,70,80,90,100,120,130}; // step mV for lean speed
uint16_t speed_arr[] = {0,200,250,310,380,460,550,650,770,900}; // step mV for lean speed
/// Функция управления мотором через дак + сложные условия остановки
void StartMotorControlTask(void *argument){
	SystemState.MotorData.motorState = DEVICE_NO_INIT;
	HAL_Delay(1000);
	SystemState.MotorData.motorState = DEVICE_READY;
	SystemState.MotorData.current_speed = 0;
	MCP4725 myMCP4725 = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A00, 3.20);
	SystemState.MotorData.max_speed = MAX_SPEED;
	int16_t offset = 0;
	int16_t test_ofsett[30] = {};
	float TEST = 0.5;
	uint8_t i = 0;
	for(;;){

		while(1){
			MCP4725_setVoltage(&myMCP4725, TEST, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
			HAL_Delay(3000);
			test_ofsett[i] = (1000 * TEST) - SystemState.AdcData.chanel_1_voltage;
			i++;
			if(i > 29)
				i = 0;
			TEST = TEST + 0.1;
			if(TEST > 2.0)
				TEST = 0.5;
		}

		while(SystemState.ErrorState.error_Motor == DEVISE_FATAL_ERROR){
			setDAC(myMCP4725, STOP_ENGINE);
			HAL_Delay(100);
		}
		offset = (speed_arr[abs(SystemState.MotorData.current_speed)]);
		if(SystemState.MotorData.current_speed < 0)
			SystemState.MotorData.control_voltage = STOP_ENGINE - (offset / 1000.0);
		else
			SystemState.MotorData.control_voltage = STOP_ENGINE + (offset / 1000.0);
		setDAC(myMCP4725,  expFiltrDAC(SystemState.MotorData.control_voltage, KOEFF_K_SLOW));
		HAL_Delay(50);
	}
}

void setDAC(MCP4725 myMCP4725, float Vout){
	if(Vout < FULL_FORWARD)
		Vout =  FULL_FORWARD;
 	if(Vout > FULL_BACK)
 		Vout = FULL_BACK;
	statusMutexMotor = osMutexAcquire(BlockI2CHandle, 1000);
	if(statusMutexMotor == osOK)
		MCP4725_setVoltage(&myMCP4725, Vout, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
	osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
}

#define KOEFF_K_SLOW 0.1
float expFiltrDAC(float newVal, float k) {
	  static float filVal = 0;
	  filVal += (newVal - filVal) * k;
	  return filVal;
}
