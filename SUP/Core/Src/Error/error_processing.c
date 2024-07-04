#include "Error/error_processing.h"

void error_processing(){
	// проверить все ли устройства на месте
	// Сравнить заданную скорость и какое напряжение на выходе (если рассоглосание то ошибка)
	// Если скорость 0 а ток не 0 ошибка + реакция
	//SystemState.ErrorState.
	// Проверка Достоврености работы АЦП
	static uint8_t iterErrorADC_DAC = 0;
	int16_t battary_voltage = SystemState.BattaryData.voltage;
	int16_t max_bat_voltage = SystemState.BattaryData.MaxCellVoltage * SystemState.BattaryData.numCell;
	int16_t min_bat_voltage = SystemState.BattaryData.MinCellVoltage * SystemState.BattaryData.numCell;
	int16_t difADC_DAC = SystemState.AdcData.chanel_1_voltage - (SystemState.MotorData.control_voltage * 1000);

	if(abs(SystemState.AdcData.chanel_0_voltage > 10))
		SystemState.ErrorState.ErrorMigrationZero = ZERO_MIGRATE;
	else
		SystemState.ErrorState.ErrorMigrationZero = ZERO_OK;

	if(abs(difADC_DAC) > 100)
		iterErrorADC_DAC++;
	else
		iterErrorADC_DAC = 0;
	if(iterErrorADC_DAC > WAIT_ERROR_ADC_DAC){
		SystemState.ErrorState.error_DAC = DEVISE_ERROR;
		iterErrorADC_DAC = 0;
	}

	if(battary_voltage >= max_bat_voltage)
			SystemState.ErrorState.ErrorBattary = VOLTAGE_IS_HIGH;
	else if(battary_voltage < min_bat_voltage)
			SystemState.ErrorState.ErrorBattary = VOLTAGE_IS_LOW;
	else
		SystemState.ErrorState.ErrorBattary  = BATTARY_OK;

}
