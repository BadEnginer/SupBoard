#ifndef INC_EXPANDERGPIO_PCF8575_H_
#define INC_EXPANDERGPIO_PCF8575_H_

#include "stm32f4xx_hal.h"
#include "macros_bits.h"

extern I2C_HandleTypeDef hi2c1;

#define I2C_ADDRESS  (32 << 1)  // Адрес устройства расширения портов
#define I2C_TIMEOUT  5
#define _LCD_USE_HAL 1
typedef enum {
	PCF_PORT_0,
	PCF_PORT_1
} PCF_PORT;

typedef enum {
	PCF_PIN_0,
	PCF_PIN_1,
	PCF_PIN_2,
	PCF_PIN_3,
	PCF_PIN_4,
	PCF_PIN_5,
	PCF_PIN_6,
	PCF_PIN_7
} PCF_PIN;

typedef enum {
	P00,
	P01,
	P02,
	P03,
	P04,
	P05,
	P06,
	P07,
	P10,
	P11,
	P12,
	P13,
	P14,
	P15,
	P16,
	P17
} PCF_ALL_PIN; // Два формата сообщений

typedef enum{
  PIN_RESET = 0,
  PIN_SET
}PCF_STATE;

void PCF_GPIO_Init();
void I2C_WritePort(uint16_t data);
uint16_t I2C_ReadPort();

// Функции для работы с портами не совместимые с HAL
#if _LCD_USE_HAL==0
void PCF_GPIO_WritePin(PCF_ALL_PIN pin, PCF_STATE value);
uint8_t PCF_GPIO_ReadPin(PCF_ALL_PIN pin);
#endif
// Функции совместимые с HAL
#if _LCD_USE_HAL==1
void PCF_GPIO_WritePin(PCF_PORT port, PCF_PIN pin, PCF_STATE value);
uint8_t PCF_GPIO_ReadPin(PCF_PORT port, PCF_PIN pin);
#endif
#endif /* INC_EXPANDERGPIO_PCF8575_H_ */
