/*
 * MotorControl.h
 *
 *  Created on: Apr 9, 2023
 *      Author: BadEnginer
 */

#ifndef INC_TASKS_MOTORCONTROL_H_
#define INC_TASKS_MOTORCONTROL_H_

#include<main.h>
#include "stm32f4xx_hal.h"
#include "DAC_out/MCP4725.h"

extern I2C_HandleTypeDef hi2c1;

void StartMotorControlTask(void *argument);


#endif /* INC_TASKS_MOTORCONTROL_H_ */
