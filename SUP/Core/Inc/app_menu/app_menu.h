/*
 * app_menu.h
 *
 *  Created on: Apr 7, 2023
 *      Author: BadEnginer
 */

#ifndef INC_APP_MENU_H_
#define INC_APP_MENU_H_

#include "Display/ssd1306/ssd1306.h"

#define MENU_ITEMS_COUNT 7 	// Количество пунктов меню
#define MENU_ITEM_HEIGHT 18 // Высота пункта меню
#define MENU_ITEM_MAIN_HEIGHT 24 // Высота основного пункта меню

#define ON 1
#define OFF 0

void drawMainMenu();
void drawSubMenu(uint8_t);

void buttonUpSet(uint8_t);
void buttonEnSet(uint8_t);
void encoderSet(int8_t);

uint8_t buttonUp();
uint8_t buttonEn();
int8_t  encoder();

void drawItemNum00();
void drawItemNum01();
void drawItemNum02();
void drawItemNum03();
void drawItemNum04();
void drawItemNum05();
void drawItemNum06();

#endif /* INC_APP_MENU_H_ */
