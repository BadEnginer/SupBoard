#include <tasks/SensControl.h>
#include <ADC_out/AdcControl.h>


extern sSystemState SystemState; // Глобальная структура где хранятся состояния всей системы
extern osMutexId_t BlockI2CHandle; //Блокировка чтения i2c
osStatus_t statusMutexI2C;



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
uint8_t test_data = 0;
int16_t arr_delta_angle[MAX_DELTA];
uint8_t counter = 0;
uint8_t countLB = 0;
uint8_t countEB = 0;
uint8_t countEP = 0;
uint8_t countEM = 0;

// Текущий канал ADC
uint8_t currentChanel = 0;

// Текущий указатель на буфер ADC
uint8_t currentAdcBlock = 0;

//функция для работы usb для отладки
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
char* tr_data;
char symData[8] = {'L','a','s','t','-'};

uint8_t time;
extern uint8_t calibr;

int16_t dif_current;
int16_t calibrate;




// Задача для опросо кнопок, энкодера, ацп и система команд от usb и обработка ошибок
void StartSensOutTask(void *argument){

	initAllChanelADC();
	pADS = ADS1115_init(&hi2c1, ADS1115_ADR, configChanel[currentChanel]);
	ADS1115_updateConfig(pADS, configChanel[currentChanel]);
	ADS1115_startContinousMode(pADS);

	osDelay(3000);

	uint8_t error_timers = create_timers();
	if(error_timers > 0){while(1);}

	SystemState.BattaryData.calibraty = 1;
	SystemState.BattaryData.zeroCurrentReal = SystemState.AdcData.chanel_3_voltage;
	SystemState.BattaryData.zeroCurrentImg = 3300 / 2;
	for(;;){
		if(command_CMD[0] != 0){ // Самоя простая система команда из палок и прочего
			switch(command_CMD[0] - 48){ // преобразуем символ в число
				case 1: buttonEnSet();    break;
				case 2: buttonLongSet();  break;
				case 3: encoderSetUp();   break;
				case 4: encoderSetDown(); break;
				case 5: SystemState.BattaryData.calibraty = 1;	  break;
				case 6: setMaxSpeed(1);	  break;
				case 7: setMaxSpeed(-1);  break;

			}//todo добавить больше данных
			test_data = command_CMD[0];
			command_CMD[0] = 0;
		    itoa((test_data-48),symData+5,10);
		    symData[7] = '\r';
			CDC_Transmit_FS(symData, sizeof(symData));
		}
		calcDeltaAngle(arr_angle);


		dif_current = (SystemState.AdcData.chanel_3_voltage - SystemState.BattaryData.zeroCurrentImg);
		if(SystemState.BattaryData.calibraty == 1){
			SystemState.BattaryData.calibraty = 0;
			calibrate = dif_current;
		}
		SystemState.BattaryData.current =expFiltrIcurrnt(((dif_current-calibrate) *42.553), 0.5);
		//SystemState.BattaryData.current = (float)((SystemState.AdcData.chanel_3_voltage - SystemState.BattaryData.zeroCurrentImg)/0.0235);
		SystemState.BattaryData.voltage = expFiltrVbat(((1.0 * SystemState.AdcData.chanel_2_voltage * 1.0 * BATTERY_DEVIDER)), 0.2);
		SystemState.BattaryData.percentCharge = expFiltrCharge(battaryCharge(SystemState.BattaryData.BatteryType, SystemState.BattaryData.numCell, SystemState.BattaryData.voltage), 0.2);
		if(SystemState.ErrorState.ErrorBattary == VOLTAGE_IS_HIGH)
			SystemState.BattaryData.percentCharge = 100;
		error_processing();
		osDelay(100);
	}
}

uint16_t charge_proc_LIPO[] = {3000, 3300, 3600, 3700, 3750, 3790, 3830, 3870, 3920, 3970, 4100, 4200};
uint16_t charge_proc_FE[]   = {3183, 3191, 3218, 3257, 3272, 3277, 3282, 3302, 3318, 3322, 3324, 3558};

float proc = 0;
uint16_t raznica1 = 0;
uint16_t raznica2 = 0;
uint8_t decides = 0;

uint8_t  battaryCharge(){
	uint16_t* arr = charge_proc_LIPO + 2 ;
	uint8_t num_cell = (uint8_t) SystemState.BattaryData.numCell;
	int16_t rez_mid;
	uint16_t battary_voltage = SystemState.BattaryData.voltage;
	if(num_cell == 0)
		return 0;
	if(SystemState.BattaryData.BatteryType == BATTARY_NO_INIT)
		return 0;
	//uint16_t battery_1C = battary_voltage/num_cell;
	if(SystemState.BattaryData.BatteryType == BATTARY_TYPE_LIFE)
		arr = charge_proc_FE + 2;
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


void readButtonState(){
	longButton();
	trueButtonLB();
	trueButtonEB();
	trueButtonEP();
	trueButtonEM();
}

void readEncoderState(){ // читаем таймером значение если порт свободен
	if(osMutexAcquire(BlockI2CHandle, 1000) == osOK){
		arr_angle[counterAngle] = getEncoderData();
		counterAngle++;
		if(counterAngle >= MAX_COUNTER_ANGLE)
			counterAngle = 0;
		osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
}

void readADCState(){
	uint8_t data_ready = 0;
	// Попытка захвата мьютекса
	if(osMutexAcquire(BlockI2CHandle, MUTEX_TIMEOUT) == osOK){
			data_ch[currentChanel][currentAdcBlock] = ADS1115_getData(pADS);
			currentAdcBlock++;
			if(currentAdcBlock >= SIZE_ADC_BUFF){
				currentAdcBlock = 0;
				currentChanel++;
				if(currentChanel >= NUM_ADC_CH){
					currentChanel = 0;
					data_ready = YES;
				}
				ADS1115_updateConfig(pADS, configChanel[currentChanel]);
			}
			osMutexRelease(BlockI2CHandle);// Освобождение мьютекса

			if(data_ready == YES){
				SystemState.AdcData.chanel_0_voltage = (getAverADC(data_ch[0])* ADC_TO_V);
				SystemState.AdcData.chanel_1_voltage = (getAverADC(data_ch[1])* ADC_TO_V) - SystemState.AdcData.chanel_0_voltage ;					SystemState.AdcData.chanel_2_voltage = (getAverADC(data_ch[2])* ADC_TO_V) - SystemState.AdcData.chanel_0_voltage;
				SystemState.AdcData.chanel_3_voltage = (getAverADC(data_ch[3])* ADC_TO_V) - SystemState.AdcData.chanel_0_voltage;
				data_ready = NO;
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
	if(xButtonTimer == NULL) 	error++;
	if (xEncoderTimer == NULL)  error++;

	if (xTimerStart(xADCTimer, 		0) != pdPASS) 	error++;
	if (xTimerStart(xButtonTimer, 	0) != pdPASS) 	error++;
	if (xTimerStart(xEncoderTimer, 	0) != pdPASS) 	error++;
}

void vEncoderTimerCallback(){
	readEncoderState();
}
void vADCTimerCallback(){

}

void vButtonTimerCallback(){

}



