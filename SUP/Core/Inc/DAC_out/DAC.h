#ifndef INC_DAC_OUT_DAC_H_
#define INC_DAC_OUT_DAC_H_
typedef struct sDAC dacc;
#include "main.h"
#include "System/commonStruct.h"
#include "DAC_out/DAC.h"
#include "ADC_out/AdcControl.h"
#include "DAC_out/MCP4725.h"


// 1.2v напржяние остановки
// 2.2v полный назад ( 2.0 - 1.2 = 1.0 )
// 0.4 полный вперёд ( 1.2 - 0.4 = 0.8 )

#define MAX_SPEED (20)
#define MIN_SPEED (-20)
#define STEP_VOLTAGE (FULL_VOLTAGE_SPEED / MAX_SPEED)
#define ABSOLUTE_FULL_FORWARD_VOLTAGE 0.4F
#define ABSOLUTE_FULL_BACK_VOLTAGE 2.0F
#define FULL_VOLTAGE_SPEED 0.8F
#define ZERO_SPEED_VOLTAGE 1.20F
#define KOEFF_K_SLOW 0.2F


#define STOP_MOTOR 1600
#define MAX_FORVARD_SPEED 3000
#define MAX_BACK_SPEED 200
#define REF_VOLTAGE_DAC 3.30

typedef struct {
    DeviceCommon common;
    MCP4725 myMCP4725;
    float refVoltage;
    uint16_t voltage;
} sDAC;


void initDAC(sDAC* myDAC, float refVoltage);
uint8_t turnOnDAC(sDAC* myDAC,ePowerState);
uint8_t setDACVoltage(float voltage, sDAC* myDAC);
uint16_t getDACVoltage(sDAC* myDAC);

#endif /* INC_DAC_OUT_DAC_H_ */
