/*
 * app_menu.h
 *
 *  Created on: Apr 7, 2023
 *      Author: BadEnginer
 */

#ifndef INC_APP_MENU_H_
#define INC_APP_MENU_H_

#define MENU_ITEMS_COUNT 7 // Количество пунктов меню
#define MENU_ITEM_HEIGHT 11 // Высота пункта меню
#define MENU_ITEM_MAIN_HEIGHT 16 // Высота основного пункта меню


void drawMainMenu();
void drawSubMenu(uint8_t i);

uint8_t iterButton();
uint8_t enterButton();
uint8_t encoderUp();
uint8_t encoderDown();

void drawItemNum00();
void drawItemNum01();
void drawItemNum02();
void drawItemNum03();
void drawItemNum04();
void drawItemNum05();
void drawItemNum06();

#endif /* INC_APP_MENU_H_ */
