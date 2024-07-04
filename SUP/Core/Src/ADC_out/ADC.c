#include "ADC_out/AdcControl.h"

// Структуры для работы АЦП
ADS1115_Config_t configChanel[4];
ADS1115_Handle_t *pADS;
int16_t data_ch[NUM_ADC_CH][SIZE_ADC_BUFF] = {0};

void initAllChanelADC(){
	initADC(&configChanel[ADC_CHANEL_1]);
	initADC(&configChanel[ADC_CHANEL_2]);
	initADC(&configChanel[ADC_CHANEL_3]);
	initADC(&configChanel[ADC_CHANEL_4]);

	initChanelADC(&configChanel[ADC_CHANEL_1], CHANNEL_AIN0_GND);
	initChanelADC(&configChanel[ADC_CHANEL_2], CHANNEL_AIN1_GND);
	initChanelADC(&configChanel[ADC_CHANEL_3], CHANNEL_AIN2_GND);
	initChanelADC(&configChanel[ADC_CHANEL_4], CHANNEL_AIN3_GND);
}

void initADC(ADS1115_Config_t* configReg){
	configReg->pgaConfig 		= PGA_4_096;
	configReg->operatingMode 	= MODE_CONTINOUS;
	configReg->dataRate 		= DRATE_250;
	configReg->compareMode 		= COMP_WINDOW;
	configReg->polarityMode 	= POLARITY_ACTIVE_LOW;
	configReg->latchingMode  	= LATCHING_NONE;
	configReg->queueComparator 	= QUEUE_FOUR; 			// ПОднять флаг через сколько измерений
}

void initChanelADC(ADS1115_Config_t* configReg, MultiplexerConfig_t chanel){
	configReg->channel = chanel;
}

int16_t getAverADC(int16_t* data){
	int32_t temp = 0;
	for(uint8_t i = 0; i < SIZE_ADC_BUFF; i++)
		temp += data[i];
	temp /= SIZE_ADC_BUFF;
	return (int16_t)temp;
}


// Универсальная фильтрующая функция
int16_t expFilter(float newVal, float k, ExpFilterState_t *filterState) {
    filterState->filVal += (newVal - filterState->filVal) * k;
    return (int16_t)(filterState->filVal);
}

// Примеры использования
int16_t expFiltrIcurrnt(float newVal, float k, ExpFilterState_t *filterState) {
    return expFilter(newVal, k, filterState);
}

int16_t expFiltrVbat(float newVal, float k, ExpFilterState_t *filterState) {
    return expFilter(newVal, k, filterState);
}

int16_t expFiltrCharge(float newVal, float k, ExpFilterState_t *filterState) {
    return expFilter(newVal, k, filterState);
}
