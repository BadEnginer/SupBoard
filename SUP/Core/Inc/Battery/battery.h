#ifndef INC_BATTERY_BATTERY_H_
#define INC_BATTERY_BATTERY_H_

#include "stm32f4xx_hal.h"
#include "System/commonStruct.h"

typedef enum eTypeBattary{
	BATTARY_NO_INIT,
	LIFE,
	LIPO
} TypeBattary;

// Структуры для батари со всеми её показателями
typedef struct {
	DeviceCommon common;
    TypeBattary batteryType;

    uint16_t maxCellVoltage; // mV
    uint16_t minCellVoltage;
    uint8_t  numCell;
    uint16_t currentCharge; // mA*h

    uint16_t voltage; // mV
    int32_t  current; // mA
    uint8_t  chargeLevel; // %

    uint16_t workTime; // sec
    uint16_t estimatedTime;// sec
} sBattery;

#endif /* INC_BATTERY_BATTERY_H_ */
