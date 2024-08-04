#include <tasks/SensControl.h>
#include "System/SystemState.h"

sSystem mySystem; // Глобальная структура где хранятся состояния всей системы
extern osMutexId_t BlockI2CHandle; //Блокировка чтения i2c
osStatus_t statusMutexI2C;

#define myADC &mySystem.adc
#define MagEnc &mySystem.magEnc
#define myButton mySystem.button
#define myDAC &mySystem.dac

/* Definitions for TestTimer */
osTimerId_t TimerReadButtonHandle;
const osTimerAttr_t ReadButtonTimer_attributes = {
  .name = "TimerReadButton"
};

osTimerId_t TimerReadADCHandle;
const osTimerAttr_t ReadADCTimer_attributes = {
  .name = "TimerReadADC"
};

osTimerId_t TimerReadEncoderHandle;
const osTimerAttr_t ReadEncoderTimer_attributes = {
  .name = "TimerReadEncoder"
};

// Дескриптор таймера
TimerHandle_t xButtonTimer;
TimerHandle_t xADCTimer;
TimerHandle_t xEncoderTimer;

uint8_t command_CMD[10] = {0};

//функция для работы usb для отладки
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
char* tr_data;
char symData[8] = {'L','a','s','t','-'};

uint8_t time;
extern uint8_t calibr;
int16_t dif_current;
int16_t calibrate;
uint8_t test_data;
int32_t current_new = 0;
int16_t supply_voltage_new = 0;
void check_error(){
	if(*myDAC.common.status == STATE_DEVICE_NO_INIT)
		mySystem.err.Critical++;
	if(*myADC.common.status == STATE_DEVICE_NO_INIT)
		mySystem.err.Critical++;
	if(*MagEnc.common.status == STATE_DEVICE_NO_INIT)
		mySystem.err.Critical++;
}
// Задача для опросо кнопок, энкодера, ацп и система команд от usb и обработка ошибок
void StartSensOutTask(void *argument){
	initAllChanelADC(myADC);
	initAllButton(myButton);
	initMagEncoder(MagEnc, I2C_ADDRESS_ENCODER);
	initDAC(myDAC, REF_VOLTAGE_DAC);
	check_error();
	osDelay(300);
	uint8_t error_timers = create_timers();
	if(error_timers > 0){while(1);}
	osDelay(3000);
	int16_t zeroCurrent = 1;
	for(;;){
		osDelay(10);
		if(osMutexAcquire(BlockI2CHandle, MUTEX_TIMEOUT) == osOK){// Попытка захвата мьютекса
			readAllChanelADC(myADC);
			osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
		}
	}
}

uint8_t create_timers(void){
	uint8_t error = 0;
	// Создание таймера для кнопок
	xButtonTimer 	= xTimerCreate(	"Button Timer",                         // Название таймера
	                             	pdMS_TO_TICKS(BUTTON_TIMER_PERIOD_MS),  // Период таймера в тиках
									pdTRUE,                                 // Повторяющийся таймер
									(void*)0,                               // Идентификатор таймера
									vButtonTimerCallback);                  // Функция обратного вызова таймера

	// Создание таймера АЦП
	xADCTimer 		= xTimerCreate("ADC Timer",                          	// Название таймера
									pdMS_TO_TICKS(ADC_TIMER_PERIOD_MS),  	// Период таймера в тиках
									pdTRUE,                              	// Повторяющийся таймер
									(void*)0,                            	// Идентификатор таймера
									vADCTimerCallback);                  	// Функция обратного вызова таймера

	// Создание таймера Энкодера
	xEncoderTimer 	= xTimerCreate("Encoder Timer",                      	// Название таймера
									pdMS_TO_TICKS(ENCODER_TIMER_PERIOD_MS), // Период таймера в тиках
									pdTRUE,                                 // Повторяющийся таймер
									(void*)0,                               // Идентификатор таймера
									vEncoderTimerCallback);                 // Функция обратного вызова таймера

	// Ошибка создания таймера
	if (xADCTimer == NULL) 		error++;
	if (xButtonTimer == NULL) 	error++;
	if (xEncoderTimer == NULL)  error++;

	if (xTimerStart(xADCTimer, 		0) != pdPASS) 	error++;
	if (xTimerStart(xButtonTimer, 	0) != pdPASS) 	error++;
	if (xTimerStart(xEncoderTimer, 	0) != pdPASS) 	error++;
	return error;
}

void vEncoderTimerCallback(TimerHandle_t xTimer){
	if(osMutexAcquire(BlockI2CHandle, MUTEX_TIMEOUT) == osOK){
		if(*MagEnc.common.status == STATE_DEVICE_NO_INIT){
			initMagEncoder(MagEnc, I2C_ADDRESS_ENCODER);
		}
		else{
			updateMagEncoder(MagEnc);
		}

		osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
	}

}

void vADCTimerCallback(TimerHandle_t xTimer){
	if(osMutexAcquire(BlockI2CHandle, MUTEX_TIMEOUT) == osOK){// Попытка захвата мьютекса
		//readAllChanelADC(myADC);
		osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
	}
}

void vButtonTimerCallback(TimerHandle_t xTimer){
	for(uint8_t i = 0; i < NUM_BUTTONS; i++){
		updateButtonState(&myButton[i]);
	}
}



