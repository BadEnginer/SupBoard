#include "tasks/ReadData.h"


extern sSystemState SystemState;
extern osMutexId_t BlockI2CHandle; //Блокировка чтения i2c
ADS1115_Config_t configChanel[4];
ADS1115_Handle_t *pADS;
int16_t data_ch[NUM_ADC_CH][SIZE_ADC_BUFF] = {0};
osStatus_t statusMutexI2C;
uint16_t current_angle;
uint16_t arr_angle[MAX_COUNTER_ANGLE];
uint8_t counterAngle = 0;
// задача для чтения данных с ацп и магнитного энкодера
void StartReadDataTask(void *argument){
	initAllChanelADC();
	uint8_t currentChanel = 0;
	pADS = ADS1115_init(&hi2c1, ADS1115_ADR, configChanel[currentChanel]);
	ADS1115_updateConfig(pADS, configChanel[currentChanel]);
	ADS1115_startContinousMode(pADS);
	for(;;){
		// Попытка захвата мьютекса с таймаутом 1000 мс
		for(uint8_t i = 0; i < SIZE_ADC_BUFF;){ // Читаем с ацп порта 10 значений
			if(osMutexAcquire(BlockI2CHandle, 1000) == osOK){
				data_ch[currentChanel][i] = ADS1115_getData(pADS);
				arr_angle[counterAngle] = getEncoderData();
				counterAngle++;
				if(counterAngle >= MAX_COUNTER_ANGLE)
					counterAngle = 0;
				osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
				osDelay(6);
				i++;
			}
		}
		currentChanel++;
		if(currentChanel > 3){
			currentChanel = 0;
			SystemState.AdcData.chanel_0_voltage = (getAverADC(data_ch[0])* ADC_TO_V);
			SystemState.AdcData.chanel_1_voltage = (getAverADC(data_ch[1])* ADC_TO_V) - SystemState.AdcData.chanel_0_voltage ;
			SystemState.AdcData.chanel_2_voltage = (getAverADC(data_ch[2])* ADC_TO_V) - SystemState.AdcData.chanel_0_voltage;
			SystemState.AdcData.chanel_3_voltage = (getAverADC(data_ch[3])* ADC_TO_V) - SystemState.AdcData.chanel_0_voltage;
		}
		while(osMutexAcquire(BlockI2CHandle, 1000) != osOK){};
			ADS1115_updateConfig(pADS, configChanel[currentChanel]);
			osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
			osDelay(10);
	}
}


int16_t getAverADC(int16_t* data){
	int32_t temp = 0;
	for(uint8_t i = 0; i < SIZE_ADC_BUFF; i++)
		temp += data[i];
	temp /= SIZE_ADC_BUFF;
	return (int16_t)temp;
}

u_magnituda magnituda;

uint16_t raw_angle = 0;
uint16_t getEncoderData(){
	SystemState.MagnitEncoderData.EncoderState.u8MagnitState = AS5600_GetStatus();
	if(SystemState.MagnitEncoderData.EncoderState.sMagnitState.MD == ON)
		raw_angle = AS5600_GetRawAngle();
	SystemState.MagnitEncoderData.curretn_raw_angle = raw_angle;
	return 	raw_angle;
}

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
