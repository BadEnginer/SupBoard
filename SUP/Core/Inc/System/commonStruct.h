
#ifndef INC_SYSTEM_COMMONSTRUCT_H_
#define INC_SYSTEM_COMMONSTRUCT_H_

#include <stdint.h>

typedef enum {
    STATE_DEVICE_NO_ANSWER,
	STATE_DEVICE_NO_INIT,
	STATE_DEVICE_READY,
	STATE_DEVICE_WAIT
} DeviceStatus;

typedef enum {
    NO_ERROR,
    YES_ERROR
} DeviceError;

typedef struct {
    DeviceStatus status;
    DeviceError error;
    uint8_t addressI2C;
} DeviceCommon;


typedef enum {
   DEV_OFF,
   DEV_ON
} ePowerState;
#endif /* INC_SYSTEM_COMMONSTRUCT_H_ */
