#ifndef INC_TASKS_ADCCONTROL_H_
#define INC_TASKS_ADCCONTROL_H_

#include <main.h>
#include "stm32f4xx_hal.h"
#include "ADC_out/ADS1115.h"

// Структура для хранения состояния фильтра
typedef struct {
    float filVal;
} ExpFilterState_t;

// Настройка параметров резистора для делителя
// todo надо добавить расчеты для всех плечь
#define RES_TOP 9.84f
#define RES_BOT 1.002f
#define BATTERY_DEVIDER (1.0 + (RES_TOP/RES_BOT))

#define ADS1115_ADR 72
#define ADC_CHANEL_1 0 // Test ADC GDN
#define ADC_CHANEL_2 1 // DAC
#define ADC_DAC 1
#define ADC_CHANEL_3 2 // Current
#define ADC_CURRENT 2
#define ADC_CHANEL_4 3 // Voltage
#define ADC_BATT 3
#define SIZE_ADC_BUFF 10
#define NUM_ADC_CH 4
#define NUM_ADC 16
#define MAX_ADC_RANGE_FLOTA (4.096 * 2)
#define MAX_ADC_RANGE (4096.0 * 2.0) // Расчёт для режима 4 вольта
#define MAX_DATA_ADC 65535.0
#define ADC_TO_V    	 (MAX_ADC_RANGE/MAX_DATA_ADC)
#define ADC_TO_VOLTAGE_F (MAX_ADC_RANGE_FLOTA/MAX_DATA_ADC)

#define MAX_CURRENT 70000.0F
#define FULL_RANGE_CURRENT 1500.0
#define V_TO_A (MAX_CURRENT/FULL_RANGE_CURRENT) // 21mV на 1 А
#define MAX_COUNTER_ANGLE 10

void initAllChanelADC();
void initADC(ADS1115_Config_t*);
void initChanelADC(ADS1115_Config_t*, MultiplexerConfig_t );

int16_t getAverADC(int16_t* data);




#endif /* INC_TASKS_ADCCONTROL_H_ */
