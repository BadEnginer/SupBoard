#ifndef __EPAPER_H
#define __EPAPER_H
#include "stm32f4xx.h"
//250*122///////////////////////////////////////

#define MONOMSB_MODE 1
#define MONOLSB_MODE 2 
#define RED_MODE     3

#define MAX_LINE_BYTES 16  //=128/8
#define MAX_COLUMN_BYTES  296

#define ALLSCREEN_GRAGHBYTES	4736

///////////////////////////GPIO Settings//////////////////////////////////////////////////////
//HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)

#define EPD_W21_CS_0	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define EPD_W21_CS_1	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)

#define EPD_W21_DC_0	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET)
#define EPD_W21_DC_1	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET)

#define EPD_W21_RST_0	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define EPD_W21_RST_1	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

#define EPD_W21_BUSY_LEVEL 0
#define isEPD_W21_BUSY HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)


void driver_delay_xms(unsigned long xms);			
void Epaper_READBUSY(void);
void Epaper_Spi_WriteByte(unsigned char TxData);
void Epaper_Write_Command(unsigned char cmd);
void Epaper_Write_Data(unsigned char data);

void EPD_HW_Init(void); //Electronic paper initialization
void EPD_Update(void);

void EPD_DeepSleep(void);
//Display 
void EPD_WhiteScreen_ALL(const unsigned char *BW_datas,const unsigned char *R_datas);
void EPD_WhiteScreen_ALL_Clean(void);

#endif


