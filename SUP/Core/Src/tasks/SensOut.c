#include "tasks/SensOut.h"

//
extern osMutexId_t BlockI2CHandle;

uint16_t raw_angle = 0;
uint16_t old_raw_angle = 0;
uint16_t angle = 0;

int16_t old_encoder_data = 0;
int16_t delta_encoder = 0;

ADS1115_Config_t configChanel[4];
#define ADC_CHANEL_1 0
#define ADC_CHANEL_2 1
#define ADC_CHANEL_3 2
#define ADC_CHANEL_4 3
//ADS1115_Config_t configChanel2;
//ADS1115_Config_t configChanel3;
//ADS1115_Config_t configChanel4;
ADS1115_Handle_t *pADS;

osStatus_t statusMutexI2C;
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
uint8_t command_CMD[10] = {0};
int16_t data_ch[4][10] = {0};
extern uint16_t global_DAC;
uint8_t aver_mass(int16_t* data){
	uint16_t temp = 0;
	for(uint8_t i = 0; i < 9; i++)
		temp += data[i];
	temp /= 10;
	temp /= 8;
	return (uint8_t)temp;
}
// Задача для опросо кнопок ADC и энкодера и система команд от usb
void StartSensOutTask(void *argument){
	uint8_t currentChanel = 0;
	initAllChanelADC(); // todo сделать новую задачу для ацп
    pADS = ADS1115_init(&hi2c1, ADS1115_ADR, configChanel[currentChanel]);
    ADS1115_updateConfig(pADS, configChanel[currentChanel]);
    ADS1115_startContinousMode(pADS);
    uint8_t buffer[] = {0};
    MCP4725 myMCP4725 = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A00, 3.30);
    global_DAC = 1244;// это вроде 1 вольт
    setDAC(myMCP4725,  global_DAC);

	for(;;){
		if(command_CMD[0] != 0){ // Самоя простая система команда из палок и прочего
			switch(command_CMD[0] - 48){
				case 1: buttonEnSet();    break;
				case 2: buttonLongSet();  break;
				case 3: encoderSetUp();   break;
				case 4: encoderSetDown(); break;
			}
			buffer[0] = command_CMD[0];
			buffer[1] = aver_mass(data_ch[0]);
			buffer[2] = aver_mass(data_ch[1]);
			buffer[3] = aver_mass(data_ch[2]);
			buffer[4] = aver_mass(data_ch[3]);
			buffer[5] = (int16_t)"\n";
			command_CMD[0] = 0;
			CDC_Transmit_FS(buffer, sizeof(buffer));
		}
		// Попытка захвата мьютекса с таймаутом 1000 мс
		statusMutexI2C = osMutexAcquire(BlockI2CHandle, 1000);
		if(statusMutexI2C == osOK){
			calcDeltaAngle((int16_t)getEncoderData()); // Расчитываем смещение энкодера
			osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
		}
		for(uint8_t i = 0; i < 10;){ // Читаем с ацп порта 10 значений
			if(osMutexAcquire(BlockI2CHandle, 1000) == osOK){
				data_ch[currentChanel][i] = ADS1115_getData(pADS);
				osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
				osDelay(6);
				i++;
			}
		 }
				// меняем канал после завершения цикла
				currentChanel++;
				if(currentChanel > 3)
					currentChanel = 0;
				while(osMutexAcquire(BlockI2CHandle, 1000) != osOK){};
					ADS1115_updateConfig(pADS, configChanel[currentChanel]);
					osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
					osDelay(10);
		setDAC(myMCP4725,  global_DAC);
		longButton();
		//osDelay(50);
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
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
	if(pinState == GPIO_PIN_RESET){
		buttonCounterSet();
	}
	else{
		buttonCounterReset();
	}

	if(buttonCounter() > 50){
		buttonLongSet();
		buttonCounterReset();
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
	configReg->compareMode 		= COMP_HYSTERESIS;
	configReg->polarityMode 	= POLARITY_ACTIVE_LOW;
	configReg->latchingMode  	= LATCHING_NONE;
	configReg->queueComparator 	= QUEUE_ONE; 			// ПОднять флаг через сколько измерений
}

void initChanelADC(ADS1115_Config_t* configReg, MultiplexerConfig_t chanel){
	configReg->channel = chanel;
}

void setDAC(MCP4725 myMCP4725, uint16_t Vout){
	MCP4725_setValue(&myMCP4725, Vout, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
}






