#include "Battery/battery.h"

uint16_t charge_proc_LIPO[] = {3000, 3300, 3600, 3700, 3750, 3790, 3830, 3870, 3920, 3970, 4100, 4200};
uint16_t charge_proc_FE[]   = {3183, 3191, 3218, 3257, 3272, 3277, 3282, 3302, 3318, 3322, 3324, 3558};

float proc = 0;
uint16_t raznica1 = 0;
uint16_t raznica2 = 0;
uint8_t decides = 0;

#define BATTARY_TYPE_LIFE 1

uint8_t  battaryCharge(){
	uint16_t* arr = charge_proc_LIPO + 2 ;
	uint8_t num_cell = 2;//(uint8_t) SystemState.BattaryData.numCell;
	int16_t rez_mid;
	uint16_t battary_voltage = 1;//SystemState.BattaryData.voltage;
	if(num_cell == 0)
		return 0;
	//uint16_t battery_1C = battary_voltage/num_cell;

	if(battary_voltage < (arr[0]*num_cell)) // Минимальное значение и меньше будет 0
		return 0;
	if(battary_voltage >= (arr[9]*num_cell)) // Максимальное значение и выше будет 1
		return 100;
	for(uint8_t i = 1; i <= 9; i++){
		if(battary_voltage > (arr[i]*num_cell))
			continue;
		else{
			decides =  i*10;
			raznica1 = (battary_voltage-(arr[i-1]*num_cell));
			raznica2 = ((arr[i]*num_cell) - (arr[i-1]*num_cell));
			proc = ((float)raznica1/(float)raznica2)*100.0;
			rez_mid = (decides+ (proc/10));
			return rez_mid;
		}
	}
}
