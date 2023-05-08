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

#define STOP_MOTOR 1600
#define MAX_FORVARD_SPEED 3000
#define MAX_BACK_SPEED 200
#define KOEFF_K_SLOW 0.1

uint16_t expFiltrDAC(uint16_t newVal, float k);
void StartMotorControlTask(void *argument);


#endif /* INC_TASKS_MOTORCONTROL_H_ */
