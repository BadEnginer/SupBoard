#include "DAC_out/DAC.h"

extern osMutexId_t BlockI2CHandle;
extern I2C_HandleTypeDef hi2c1;

uint16_t speed_arr_plus[] =  {0,250,300,330,380,460,550,650,770,900}; // step mV for lean speed
uint16_t speed_arr_minus[] = {0,180,250,310,380,460,550,650,770,900}; // step mV for lean speed

void initDAC(sDAC* myDAC, float refVoltage){
	myDAC->common.addressI2C = MCP4725A0_ADDR_A00;
		if(ADC_i2cReady()){
			myDAC->common.error = NO_ERROR;
			myDAC->common.status = STATE_DEVICE_READY;
		}
		else{
			myDAC->common.error = YES_ERROR;
			myDAC->common.status = STATE_DEVICE_NO_INIT;
		}
	myDAC->refVoltage = refVoltage;
	myDAC->myMCP4725 = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A00, refVoltage);
	turnOnDAC(myDAC, DEV_ON);
}
uint8_t DAC_i2cReady(){
	HAL_StatusTypeDef state = HAL_I2C_IsDeviceReady(&hi2c1, (MCP4725A0_ADDR_A00 << 1), 2, 5);
	if(state == HAL_OK)
		return 1;
	else
		return 0;
}

uint8_t setDACVoltage(float voltage, sDAC* myDAC){
	myDAC->voltage = (uint16_t)(voltage*1000);
	return MCP4725_setVoltage(&(myDAC->myMCP4725), voltage, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
}

uint16_t getDACVoltage(sDAC* myDAC){
	return myDAC->voltage;
}

uint8_t turnOnDAC(sDAC* myDAC,ePowerState turn){
	if(turn == DEV_OFF){
		myDAC->common.status = STATE_DEVICE_WAIT;
		return MCP4725_setVoltage(&(myDAC->myMCP4725), ZERO_SPEED_VOLTAGE, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_100KOHM);
	}
	else
		myDAC->common.status = STATE_DEVICE_READY;
	return MCP4725_setVoltage(&(myDAC->myMCP4725), ZERO_SPEED_VOLTAGE, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
}

