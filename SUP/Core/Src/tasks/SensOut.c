#include "tasks/SensOut.h"

//
extern osMutexId_t BlockI2CHandle;

uint16_t raw_angle = 0;
uint16_t old_raw_angle = 0;
uint16_t angle = 0;

int16_t old_encoder_data = 0;
int16_t delta_encoder = 0;

ADS1115_Config_t configChanel1;
ADS1115_Config_t configChanel2;
ADS1115_Config_t configChanel3;
ADS1115_Config_t configChanel4;
ADS1115_Handle_t *pADS;

osStatus_t statusMutexI2C;

// Задача для опросо кнопок ADC и энкодера
void StartSensOutTask(void *argument){
	initAllChanelADC();
    pADS = ADS1115_init(&hi2c1, ADS1115_ADR, configChanel1);
    ADS1115_updateConfig(pADS, configChanel1);
    ADS1115_startContinousMode(pADS);
	for(;;){
		// Попытка захвата мьютекса с таймаутом 1000 мс
		statusMutexI2C = osMutexAcquire(BlockI2CHandle, 1000);
		if(statusMutexI2C == osOK){
			calcDeltaAngle((int16_t)getEncoderData()); // Расчитываем смещение энкодера
			osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
		}
		longButton();
		osDelay(50);
	}
}

void calcDeltaAngle(int16_t current_encoder_data){
	delta_encoder = current_encoder_data - old_encoder_data;
	old_encoder_data = current_encoder_data;
	if(delta_encoder > MAX_ANGLE || delta_encoder < (-1)*MAX_ANGLE){
		delta_encoder = 0;
	}
	if(delta_encoder > MIN_ANGLE )
		encoderSetUp();
	if(delta_encoder < (-1)*MIN_ANGLE)
		encoderSetDown();
}
void longButton(){
	if(buttonUp() == OFF){
		buttonCounterReset();
	}
	if(buttonUp() == ON){
		buttonCounterSet();
		buttonUpReset();
	}
	if(buttonCounter() > 10){
		buttonLongSet();
		buttonCounterReset();
		buttonUpReset();
	}
}

u_magnituda magnituda;
uint16_t getEncoderData(){
	magnituda.data = AS5600_GetStatus();
	if(magnituda.state_magnit.MD == ON)
		raw_angle = AS5600_GetRawAngle();
	return 	raw_angle;
}

void initAllChanelADC(){
	initADC(&configChanel1);
	initADC(&configChanel2);
	initADC(&configChanel3);
	initADC(&configChanel4);

	initChanelADC(&configChanel1, CHANNEL_AIN0_GND);
	initChanelADC(&configChanel2, CHANNEL_AIN1_GND);
	initChanelADC(&configChanel3, CHANNEL_AIN2_GND);
	initChanelADC(&configChanel4, CHANNEL_AIN3_GND);
}

void initADC(ADS1115_Config_t* configReg){
	configReg->pgaConfig 		= PGA_4_096;
	configReg->operatingMode 	= MODE_CONTINOUS;
	configReg->dataRate 		= DRATE_250;
	configReg->compareMode 		= COMP_HYSTERESIS;
	configReg->polarityMode 	= POLARITY_ACTIVE_LOW;
	configReg->latchingMode  	= LATCHING_NONE;
	configReg->queueComparator 	= QUEUE_ONE; 			// ПОднять флаг через сколько измерений
}

void initChanelADC(ADS1115_Config_t* configReg, MultiplexerConfig_t chanel){
	configReg->channel = chanel;
}
