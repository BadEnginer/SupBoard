#include <tasks/SensControl.h>
#include <ADC_out/AdcControl.h>
#include <Encoder/encoder.h>
#include <Button/Button.h>


extern sSystemState SystemState; // Глобальная структура где хранятся состояния всей системы
extern osMutexId_t BlockI2CHandle; //Блокировка чтения i2c
osStatus_t statusMutexI2C;

sButton  buttonA;
sButton  buttonB;
sButton  buttonC;
sButton  buttonD;
sEncoder MagEnc;

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
// Задача для опросо кнопок, энкодера, ацп и система команд от usb и обработка ошибок
void StartSensOutTask(void *argument){
	initAllChanelADC();
	init_button(&buttonA, GPIOA, GPIO_PIN_15, NO_INVERT);
	init_button(&buttonB, GPIOB, GPIO_PIN_3,  NO_INVERT);
	init_button(&buttonC, GPIOB, GPIO_PIN_4,  NO_INVERT);
	init_button(&buttonD, GPIOB, GPIO_PIN_5,  NO_INVERT);
	initMagEncoder(&MagEnc);
	osDelay(300);
	uint8_t error_timers = create_timers();
	if(error_timers > 0){while(1);}
	osDelay(3000);
	int16_t zeroCurrent = SystemState.AdcData.chanel_1_voltage;
	SystemState.BattaryData.calibraty = 1;
	SystemState.BattaryData.zeroCurrentReal = SystemState.AdcData.chanel_3_voltage;
	SystemState.BattaryData.zeroCurrentImg = 3300 / 2;
	for(;;){
		if(command_CMD[0] != 0){ // Самоя простая система команда из палок и прочего
			switch(command_CMD[0] - 48){ // преобразуем символ в число
				//case 1: buttonEnSet();    break;
				//case 2: buttonLongSet();  break;
				//case 3: encoderSetUp();   break;
				//case 4: encoderSetDown(); break;
				case 5: SystemState.BattaryData.calibraty = 1;	  break;
				//case 6: setMaxSpeed(1);	  break;
				//case 7: setMaxSpeed(-1);  break;

			}//todo добавить больше данных
			test_data = command_CMD[0];
			command_CMD[0] = 0;
		    itoa((test_data-48),symData+5,10);
		    symData[7] = '\r';
			CDC_Transmit_FS(symData, sizeof(symData));
		}
		dif_current = (SystemState.AdcData.chanel_3_voltage - SystemState.BattaryData.zeroCurrentImg);
		if(SystemState.BattaryData.calibraty == 1){
			SystemState.BattaryData.calibraty = 0;
			calibrate = dif_current;
		}
		//SystemState.BattaryData.current =expFiltrIcurrnt(((dif_current-calibrate) *42.553), 0.5);
		//SystemState.BattaryData.current = (float)((SystemState.AdcData.chanel_3_voltage - SystemState.BattaryData.zeroCurrentImg)/0.0235);
		//SystemState.BattaryData.voltage = expFiltrVbat(((1.0 * SystemState.AdcData.chanel_2_voltage * 1.0 * BATTERY_DEVIDER)), 0.2);
		//SystemState.BattaryData.percentCharge = expFiltrCharge(battaryCharge(SystemState.BattaryData.BatteryType, SystemState.BattaryData.numCell, SystemState.BattaryData.voltage), 0.2);
		if(SystemState.ErrorState.ErrorBattary == VOLTAGE_IS_HIGH)
			SystemState.BattaryData.percentCharge = 100;
		//error_processing();
		osDelay(100);
		current_new = calculateCurrent(SystemState.AdcData.chanel_1_voltage - zeroCurrent);
		supply_voltage_new = calculateVoltageSupply(SystemState.AdcData.chanel_3_voltage, BATTERY_DEVIDER);
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
	if(xButtonTimer == NULL) 	error++;
	if (xEncoderTimer == NULL)  error++;

	if (xTimerStart(xADCTimer, 		0) != pdPASS) 	error++;
	if (xTimerStart(xButtonTimer, 	0) != pdPASS) 	error++;
	if (xTimerStart(xEncoderTimer, 	0) != pdPASS) 	error++;
	return error;
}

void vEncoderTimerCallback(TimerHandle_t xTimer){
	if(osMutexAcquire(BlockI2CHandle, MUTEX_TIMEOUT) == osOK){
		updateMagEncoder(&MagEnc);
		osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
	}

}

void vADCTimerCallback(TimerHandle_t xTimer){
	if(osMutexAcquire(BlockI2CHandle, MUTEX_TIMEOUT) == osOK){// Попытка захвата мьютекса
		readAllChanelADC();
		osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
	}
}

void vButtonTimerCallback(TimerHandle_t xTimer){
	updateButtonState(&buttonA);
	updateButtonState(&buttonB);
	updateButtonState(&buttonC);
	updateButtonState(&buttonD);
}



