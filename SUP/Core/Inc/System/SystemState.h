#ifndef INC_SYSTEM_SYSTEMSTATE_H_
#define INC_SYSTEM_SYSTEMSTATE_H_
#include "main.h"


typedef struct {
    DeviceCommon common;
    int32_t current; // mA
} CurrentSensor;

typedef struct {
    DeviceCommon common;
    int16_t speed;
    int16_t direction;
    float smooth_voltage;
    float normalazy_voltage;
} sThruster;

typedef struct {
	sBattery battery;
	sEncoder magEnc;
    sADC adc;
    CurrentSensor curSens;
    sDAC dac;
    sThruster motor;
    sButton button[4];
} sSystem;

#endif /* INC_SYSTEM_SYSTEMSTATE_H_ */
