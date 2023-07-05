#include "tasks/SensOut.h"

//
#define RES_TOP 9.92f
#define RES_BOT 1.001f
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
void error_processing(){
	// проверить все ли устройства на месте
	// Сравнить заданную скорость и какое напряжение на выходе (если рассоглосание то ошибка)
	// Если скорость 0 а ток не 0 ошибка + реакция
	//SystemState.ErrorState.

}
#define CELL_4 4
#define CELL_3 3
#define BATTARY_TYPE_FE  1
#define BATTARY_TYPE_LIPO  2
// Задача для опросо кнопок, энкодера и система команд от usb и обработка ошибок
void StartSensOutTask(void *argument){
	for(;;){
		//data_right++;
		if(command_CMD[0] != 0){ // Самоя простая система команда из палок и прочего
			switch(command_CMD[0] - 48){ // преобразуем символ в число
				case 1: buttonEnSet();    break;
				case 2: buttonLongSet();  break;
				case 3: encoderSetUp();   break;
				case 4: encoderSetDown(); break;
				case 5: calibr = ON;	  break;
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

		SystemState.BattaryData.current = SystemState.AdcData.chanel_1_voltage;
		SystemState.BattaryData.voltage = expFiltrVbat(((1.0 * SystemState.AdcData.chanel_3_voltage * 1.0 * BATTERY_DEVIDER)), 0.1);
		SystemState.BattaryData.percentCharge = expFiltrCharge(battaryCharge(BATTARY_TYPE_LIPO, CELL_4, SystemState.BattaryData.voltage), 0.2);
		osDelay(100);
	}
}

int16_t expFiltrVbat(int16_t newVal, float k) {
	  static int16_t filVal = 0;
	  filVal += (newVal - filVal) * k;
	  return filVal;
}
int16_t expFiltrCharge(int16_t newVal, float k) {
	  static int16_t filVal = 0;
	  filVal += (newVal - filVal) * k;
	  return filVal;
}



uint16_t charge_proc_LIPO[] = {3000, 3300, 3600, 3700, 3750, 3790, 3830, 3870, 3920, 3970, 4100, 4200};
uint16_t charge_proc_FE[]   = {3183, 3191, 3218, 3257, 3272, 3277, 3282, 3302, 3318, 3322, 3324, 3558};

float proc = 0;
uint16_t raznica1 = 0;
uint16_t raznica2 = 0;
uint8_t decides = 0;

uint8_t  battaryCharge(uint8_t battryType, uint8_t num_cell, uint16_t battary_voltage){
	uint16_t* arr = charge_proc_LIPO + 2 ;
	if(num_cell == 0)
		num_cell = 1;
	//uint16_t battery_1C = battary_voltage/num_cell;
	if(battryType == BATTARY_TYPE_FE)
		arr = charge_proc_FE + 2;
	if(battary_voltage < (arr[0]*num_cell)) // Минимальное значение и меньше будет 0
		return 0;
	if(battary_voltage >= (arr[9]*num_cell)) // Максимальное значение и выше будет 1
		return 100;
	for(uint8_t i = 1; i < 9; i++){
		if(battary_voltage > (arr[i]*num_cell))
			continue;
		else{
			decides =  i*10;
			raznica1 = (battary_voltage-(arr[i-1]*num_cell));
			raznica2 = ((arr[i]*num_cell) - (arr[i-1]*num_cell));
			proc = ((float)raznica1/(float)raznica2)*100.0;
			return (decides+ proc/10);
		}
	}
}
void trueButtonLB(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
		if(pinState == GPIO_PIN_RESET){
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
		if(pinState == GPIO_PIN_RESET){
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
		if(pinState == GPIO_PIN_RESET){
			countEP++;
		}
		else{
			countEP = 0;
			counterLong = OFF;
		}
		if((countEP > MAX_COUNT) && (counterLong == OFF)){
			encoderSetUp();
			countEP = 0;
		}
		if((countEP > LONG_COUNT) && (counterLong == ON)){
			encoderSetUp();// Добавил логигу работы для удержания кнопки если она продолжает удерживаться то продолжать увеличивать
			countEP = 0;
		}
}

void trueButtonEM(){
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
	static uint8_t counterLong = OFF;
		if(pinState == GPIO_PIN_RESET){
			countEM++;
		}
		else{
			countEM = 0;
			counterLong = OFF;
		}
		if((countEM > MAX_COUNT) && (counterLong == OFF)){
			counterLong = ON;
			encoderSetDown();
			countEM = 0;
		}
		if((countEM > LONG_COUNT) && (counterLong == ON)){
			encoderSetDown(); // Добавил логигу работы для удержания кнопки если она продолжает удерживаться то продолжать уменьшать
			countEM = 0;
		}
}

void calcDeltaAngle(uint16_t* data){
	int16_t deltaAngle = 0;
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







