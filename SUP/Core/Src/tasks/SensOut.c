#include "tasks/SensOut.h"

//
#define RES_TOP 9.84f
#define RES_BOT 1.002f
#define BATTERY_DEVIDER (1.0 + (RES_TOP/RES_BOT))


volatile uint16_t temp_counter_plus = 0;
volatile uint16_t temp_counter_min = 0;
uint16_t old_raw_angle = 0;
uint16_t angle = 0;
int16_t old_encoder_data = 0;
int16_t delta_encoder = 0;
uint8_t command_CMD[10] = {0};
uint8_t test_data = 0;
int16_t arr_delta_angle[MAX_DELTA];
uint8_t counter = 0;
uint8_t countLB = 0;
uint8_t countEB = 0;
uint8_t countEP = 0;
uint8_t countEM = 0;

extern sSystemState SystemState;
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
extern uint16_t current_angle;
extern uint16_t arr_angle[MAX_COUNTER_ANGLE];
char* tr_data;
char symData[8] = {'L','a','s','t','-'};
uint8_t time;
extern uint8_t calibr;
#define WAIT_ERROR_ADC_DAC 50

void error_processing(){
	// проверить все ли устройства на месте
	// Сравнить заданную скорость и какое напряжение на выходе (если рассоглосание то ошибка)
	// Если скорость 0 а ток не 0 ошибка + реакция
	//SystemState.ErrorState.
	// Проверка Достоврености работы АЦП
	static uint8_t iterErrorADC_DAC = 0;
	int16_t battary_voltage = SystemState.BattaryData.voltage;
	int16_t max_bat_voltage = SystemState.BattaryData.MaxCellVoltage * SystemState.BattaryData.numCell;
	int16_t min_bat_voltage = SystemState.BattaryData.MinCellVoltage * SystemState.BattaryData.numCell;
	int16_t difADC_DAC = SystemState.AdcData.chanel_1_voltage - (SystemState.MotorData.control_voltage * 1000);

	if(abs(SystemState.AdcData.chanel_0_voltage > 10))
		SystemState.ErrorState.ErrorMigrationZero = ZERO_MIGRATE;
	else
		SystemState.ErrorState.ErrorMigrationZero = ZERO_OK;

	if(abs(difADC_DAC) > 100)
		iterErrorADC_DAC++;
	else
		iterErrorADC_DAC = 0;
	if(iterErrorADC_DAC > WAIT_ERROR_ADC_DAC){
		SystemState.ErrorState.error_DAC = DEVISE_ERROR;
		iterErrorADC_DAC = 0;
	}

	if(battary_voltage >= max_bat_voltage)
			SystemState.ErrorState.ErrorBattary = VOLTAGE_IS_HIGH;
	else if(battary_voltage < min_bat_voltage)
			SystemState.ErrorState.ErrorBattary = VOLTAGE_IS_LOW;
	else
		SystemState.ErrorState.ErrorBattary  = BATTARY_OK;

}

int16_t dif_current;
int16_t calibrate;
// Задача для опросо кнопок, энкодера и система команд от usb и обработка ошибок
void StartSensOutTask(void *argument){
	osDelay(3000);
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
		longButton();
		calcDeltaAngle(arr_angle);
		trueButtonLB();
		trueButtonEB();
		trueButtonEP();
		trueButtonEM();

#define CUR_PARAM_1 37174,7
#define CUR_PARAM_2 (1,7317 * CUR_PARAM_1)
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

int16_t expFiltrIcurrnt(float newVal, float k) {
	  static float filVal = 0;
	  filVal += (newVal - filVal) * k;
	  return (int16_t)filVal;
}

int16_t expFiltrVbat(float newVal, float k) {
	  static float filVal = 0;
	  filVal += (newVal - filVal) * k;
	  return (int16_t)filVal;
}
int16_t expFiltrCharge(float newVal, float k) {
	  static float filVal = 0;
	  filVal += (newVal - filVal) * k;
	  return (int16_t)filVal;
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
void trueButtonLB(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
		if(pinState == GPIO_PIN_SET){
			countLB++;
		}
		else{
			countLB = 0;
		}
		if(countLB > MAX_COUNT){
			buttonLongSet();
			countLB = 0;
		}
}

void trueButtonEB(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
		if(pinState == GPIO_PIN_SET){
			countEB++;
		}
		else{
			countEB = 0;
		}
		if(countEB > MAX_COUNT){
			buttonEnSet();
			countEB = 0;
		}
}

void trueButtonEP(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	static uint8_t counterLong = OFF;
		if(pinState == GPIO_PIN_SET){
			countEP++;
		}
		else{
			countEP = 0;
			counterLong = OFF;
		}
		if((countEP > MAX_COUNT) && (counterLong == OFF)){
			//counterLong = ON;
			encoderSetUp();
			countEP = 0;
		}
		if((countEP > LONG_COUNT) && (counterLong == ON)){
			setMaxSpeed(1);// Добавил логигу работы для удержания кнопки если она продолжает удерживаться то продолжать увеличивать
			countEP = 0;
			counterLong = OFF;
		}
}

void trueButtonEM(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);

	static uint8_t counterLong = OFF;
		if(pinState == GPIO_PIN_SET){
			countEM++;
		}
		else{
			countEM = 0;
			counterLong = OFF;
		}
		if((countEM > MAX_COUNT) && (counterLong == OFF)){
			encoderSetDown();
			countEM = 0;
		}
		if((countEM > LONG_COUNT) && (counterLong == ON)){
			setMaxSpeed(-1); // Добавил логигу работы для удержания кнопки если она продолжает удерживаться то продолжать уменьшать
			countEM = 0;
			counterLong = OFF;
		}
}

void calcDeltaAngle(uint16_t* data){
	static int32_t currentAngle = 0;
	int32_t temp = currentAngle - data[28];
	if(temp > 1000 || temp < -1000){
		currentAngle = data[28];
		return;
	}
	if(currentAngle)
	if(temp > 500 ){
		currentAngle = data[28];
		encoderSetUp();
		temp_counter_plus++;
	}
	if(temp < -500){
		currentAngle = data[28];
		encoderSetDown();
		temp_counter_min++;
	}
	/*int16_t deltaAngle = 0;
	int8_t plus = 0;
	counter = 0;
	for(uint8_t i = 0; i < MAX_DELTA; i++){
		deltaAngle = data[i+1] - data[i];
		// логика простая если число в нужном диапазоне то добавить его в массив
		if( (abs(deltaAngle) > MIN_ANGLE) && (abs(deltaAngle) < MAX_ANGLE)){
			arr_delta_angle[counter] = 	deltaAngle;
			counter++;
			if(counter >=MAX_DELTA)
				counter = 0;
		}
	}
	if(counter < 20)
		return;
	for(uint8_t i = 0; i < MAX_DELTA; i++){
		if(arr_delta_angle[i] > 0)
			plus++;
		if(arr_delta_angle[i] < 0)
			plus--;
	}
	if(plus > 16){
		encoderSetUp();
		temp_counter_plus++;
	}
	if(plus < -16){
		encoderSetDown();
		temp_counter_min++;
	}
	*/
}

void longButton(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
	if(pinState == GPIO_PIN_RESET){
		buttonCounterSet();
	}
	else{
		buttonCounterReset();
	}

	if(buttonCounter() > 6){
		buttonLongSet();
		buttonCounterReset();
	}
}







