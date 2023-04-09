/*
 * app_menu.h
 *
 *  Created on: Apr 7, 2023
 *      Author: BadEnginer
 */

#ifndef INC_APP_MENU_H_
#define INC_APP_MENU_H_

//#include <main.h>
#include "Display/ssd1306/ssd1306.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"

#define MENU_ITEMS_COUNT 7 	// Количество пунктов меню
#define MENU_ITEM_HEIGHT 10 // Высота пункта меню
#define MENU_ITEM_MAIN_HEIGHT 18 // Высота основного пункта меню

#define ON 1
#define OFF 0

#define ZERO_POS 2
#define LAST_POS 62

#define SIZE_FONT_X 7
#define SIZE_FONT_Y 10

#define SIZE_MAIN_FONT_X 11
#define SIZE_MAIN_FONT_Y 18

void drawMainMenu();
void drawSubMenu(uint8_t);
void drawItemNum00();
void drawItemNum01();
void drawItemNum02();
void drawItemNum03();
void drawItemNum04();
void drawItemNum05();
void drawItemNum06();
void udpateDisplay();
void drawSubMenu(uint8_t i);

uint8_t buttonUp();
void buttonUpSet();
void buttonUpReset();
uint8_t buttonEn();
void buttonEnSet();
void buttonEnReset();
uint8_t buttonCounter();
void buttonCounterSet();
void buttonCounterReset();
uint8_t buttonLong();
void buttonLongSet();
void buttonLongReset();
int8_t encoderData();
void encoderSetUp();
void encoderSetDown();
void encoderReset();


#endif /* INC_APP_MENU_H_ */
