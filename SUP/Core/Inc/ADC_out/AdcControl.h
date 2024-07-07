#ifndef INC_TASKS_ADCCONTROL_H_
#define INC_TASKS_ADCCONTROL_H_

#include <main.h>
#include "stm32f4xx_hal.h"
#include "ADC_out/ADS1115.h"

// Структура для хранения состояния фильтра
typedef struct {
    float filVal;
} ExpFilterState_t;

extern sSystemState SystemState;
extern I2C_HandleTypeDef hi2c1;
// Настройка параметров резистора для делителя
// todo надо добавить расчеты для всех плечь
#define RES_TOP 115.00f
#define RES_BOT 9.946f
#define BATTERY_DEVIDER (1+(RES_TOP/RES_BOT))

#define ADS1115_ADR 72
#define ADC_DAC 1
#define ADC_CURRENT 2
#define ADC_BATT 3

#define ADC_CHANEL_1 0 //
#define ADC_CHANEL_2 1 //
#define ADC_CHANEL_3 2 //
#define ADC_CHANEL_4 3 // Voltage

#define SIZE_ADC_BUFF 10
#define NUM_ADC_CH 4

#define NUM_ADC 16
#define MAX_ADC_RANGE_FLOTA (4.096 * 2)
#define MAX_ADC_RANGE (4096.0 * 2.0) // Расчёт для режима 4 вольта
#define MAX_DATA_ADC 65535.0
#define ADC_TO_V    	 (MAX_ADC_RANGE/MAX_DATA_ADC)
#define ADC_TO_VOLTAGE_F (MAX_ADC_RANGE_FLOTA/MAX_DATA_ADC)

#define DIV_POWER_SUPPLY
#define MAX_CURRENT 70000.0F
#define FULL_RANGE_CURRENT 2200.0 // 2500 ( половина от 5 Вольт - 0.3
#define mV_TO_A (MAX_CURRENT/FULL_RANGE_CURRENT) // 33mV на 1 А ( из даташиита)

#define MAX_COUNTER_ANGLE 10

void initAllChanelADC();
void readAllChanelADC();
void initADC(ADS1115_Config_t* configReg, MultiplexerConfig_t chanel);
void initChanelADC(ADS1115_Config_t*, MultiplexerConfig_t );
int16_t getAverADC(int16_t* data);
void adc_to_voltage();
int32_t calculateCurrent(int16_t voltageDifference);
uint16_t calculateVoltageSupply(int16_t voltageADC, float divede);

#endif /* INC_TASKS_ADCCONTROL_H_ */