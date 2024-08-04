#include "ADC_out/AdcControl.h"

// Структуры для работы АЦП
/*
ADS1115_Config_t configChanel[4];
ADS1115_Handle_t *pADS;
*/
int16_t data_ch[NUM_ADC_CH][SIZE_ADC_BUFF] = {0}; // Фильтрующий буфер
// Текущий канал ADC
uint8_t currentAdcBlock = 0;



void initAllChanelADC(sADC* myADC){
	myADC->common.addressI2C = ADS1115_ADR;
	if(ADC_i2cReady()){
		myADC->common.error = NO_ERROR;
		myADC->common.status = STATE_DEVICE_READY;
	}
	else{
		myADC->common.error = YES_ERROR;
		myADC->common.status = STATE_DEVICE_NO_INIT;
	}
	initADC(&myADC->config[ADC_CHANEL_0], CHANNEL_AIN0_GND);
	initADC(&myADC->config[ADC_CHANEL_2], CHANNEL_AIN2_GND);
	initADC(&myADC->config[ADC_CHANEL_3], CHANNEL_AIN3_GND);
	initADC(&myADC->config[ADC_CHANEL_1], CHANNEL_AIN1_GND);

	myADC->currentChanel = ADC_CHANEL_0;

	myADC->data[ADC_CHANEL_0].coef_to_V = ADC_TO_V;
	myADC->data[ADC_CHANEL_1].coef_to_V = ADC_TO_V;
	myADC->data[ADC_CHANEL_2].coef_to_V = ADC_TO_V;
	myADC->data[ADC_CHANEL_3].coef_to_V = ADC_TO_V;

	myADC->handle = ADS1115_init(&hi2c1, ADS1115_ADR, myADC->config[myADC->currentChanel]);
	updapteConfig(myADC);
	ADS1115_startContinousMode(myADC->handle);
}

uint8_t ADC_i2cReady(){
	HAL_StatusTypeDef state = HAL_I2C_IsDeviceReady(&hi2c1, (ADS1115_ADR << 1), 2, 5);
	if(state == HAL_OK)
		return 1;
	else
		return 0;
}
void updapteConfig(sADC* myADC){
	ADS1115_updateConfig(myADC->handle, myADC->config[myADC->currentChanel]);
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

int16_t getAverADC(int16_t* data){
	int32_t temp = 0;
	for(uint8_t i = 0; i < SIZE_ADC_BUFF; i++)
		temp += data[i];
	temp /= SIZE_ADC_BUFF;
	return (int16_t)temp;
}

// Универсальная фильтрующая функция
float expFilter(float newVal, float k, ExpFilterState_t *filterState) {
    filterState->filVal += (newVal - filterState->filVal) * k;
    return (filterState->filVal);
}

void readAllChanelADC(sADC* myADC){ // Чтение всех каналов ацп и преобразоавние их в напряжение
	data_ch[myADC->currentChanel][currentAdcBlock] = ADS1115_getData(myADC->handle);
	currentAdcBlock++;
	if(currentAdcBlock >= (SIZE_ADC_BUFF)){ // Если заполнил первый массив
		currentAdcBlock = 0;
		myADC->currentChanel++;
		if(myADC->currentChanel >= NUM_ADC_CH){ // Если весь ацп массив обновился сделать расчеты
			myADC->currentChanel = 0;
			updateAllData(myADC);
		}
		updapteConfig(myADC);
	}
}


int32_t calculateCurrent(int16_t voltageDifference){

int32_t current = (voltageDifference  / mV_TO_A)*1000.0;
return current;
}

uint16_t calculateVoltageSupply(int16_t voltageADC, float divede){
	return (((float)(voltageADC) * (float)divede));
}

void updateData(sADC* myADC,uint8_t chanel){
	myADC->data[chanel].adc_data =   getAverADC(data_ch[chanel]);
	myADC->data[chanel].voltage =    myADC->data[chanel].adc_data * myADC->data[chanel].coef_to_V;

}

void updateAllData(sADC* myADC){
	updateData(myADC, ADC_CHANEL_0);
	updateData(myADC, ADC_CHANEL_1);
	updateData(myADC, ADC_CHANEL_2);
	updateData(myADC, ADC_CHANEL_3);
}
