#include "ADC_out/AdcControl.h"

// Структуры для работы АЦП
ADS1115_Config_t configChanel[4];
ADS1115_Handle_t *pADS;
int16_t data_ch[NUM_ADC_CH][SIZE_ADC_BUFF] = {0};
// Текущий канал ADC
uint8_t currentChanel = 0;
uint8_t currentAdcBlock = 0;



void initAllChanelADC(){
	initADC(&configChanel[ADC_CHANEL_1], CHANNEL_AIN0_GND);
	initADC(&configChanel[ADC_CHANEL_2], CHANNEL_AIN1_GND);
	initADC(&configChanel[ADC_CHANEL_3], CHANNEL_AIN2_GND);
	initADC(&configChanel[ADC_CHANEL_4], CHANNEL_AIN3_GND);

	pADS = ADS1115_init(&hi2c1, ADS1115_ADR, configChanel[currentChanel]);
	ADS1115_updateConfig(pADS, configChanel[0]);
	ADS1115_startContinousMode(pADS);
}

void initADC(ADS1115_Config_t* configReg, MultiplexerConfig_t chanel){
	configReg->pgaConfig 		= PGA_4_096;
	configReg->operatingMode 	= MODE_CONTINOUS;
	configReg->dataRate 		= DRATE_250;
	configReg->compareMode 		= COMP_WINDOW;
	configReg->polarityMode 	= POLARITY_ACTIVE_LOW;
	configReg->latchingMode  	= LATCHING_NONE;
	configReg->queueComparator 	= QUEUE_FOUR; 			// ПОднять флаг через сколько измерений
	configReg->channel = chanel;
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

void readAllChanelADC(){ // Чтение всех каналов ацп и преобразоавние их в напряжение
	data_ch[currentChanel][currentAdcBlock] = ADS1115_getData(pADS);
	currentAdcBlock++;
	if(currentAdcBlock >= (SIZE_ADC_BUFF)){ // Если заполнил первый массив
		currentAdcBlock = 0;
		currentChanel++;
		if(currentChanel >= NUM_ADC_CH){ // Если весь ацп массив обновился сделать расчеты
			currentChanel = 0;
			adc_to_voltage();
		}
		ADS1115_updateConfig(pADS, configChanel[currentChanel]);
	}
}


int32_t calculateCurrent(int16_t voltageDifference){

int32_t current = (voltageDifference  / mV_TO_A)*1000.0;
return current;
}

uint16_t calculateVoltageSupply(int16_t voltageADC, float divede){
	return (((float)(voltageADC) * (float)divede));
}

void adc_to_voltage(){
	SystemState.AdcData.chanel_0_voltage = (getAverADC(data_ch[0])* ADC_TO_V);
	SystemState.AdcData.chanel_1_voltage = (getAverADC(data_ch[1])* ADC_TO_V);
	SystemState.AdcData.chanel_2_voltage = (getAverADC(data_ch[2])* ADC_TO_V);
	SystemState.AdcData.chanel_3_voltage = (getAverADC(data_ch[3])* ADC_TO_V);
}
