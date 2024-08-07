#ifndef INC_APP_MENU_H_
#define INC_APP_MENU_H_

//#include <main.h>
#include "Display/ssd1306/ssd1306.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include <stdlib.h>
#include <tasks/SensControl.h>

#define MENU_ITEMS_COUNT 7 	// Количество пунктов меню
#define MENU_ITEM_HEIGHT 10 // Высота пункта меню
#define MENU_ITEM_MAIN_HEIGHT 18 // Высота основного пункта меню


#define STD_WHITESPACE 7 // с учётом линии по краям экрана

#define LIN_CURR_K 4.3186f
#define LIN_CURR_B (56143)

#define ON 1
#define OFF 0

#define ZERO_POS_X 2
#define ZERO_POS_Y 0


#define SIZE_FONT_X 11
#define SIZE_FONT_Y 18

#define START_POS_X 2
#define LAST_POS_X (127 - START_POS_X)

#define START_POS_Y 0

void drawMainMenu();
void drawStartMenu();
void startDisplay();

int16_t expFiltr(int16_t newVal, float k);
#define KOEFF_K 0.25
void drawLEDMenu();
void drawE_inkMenu();
void drawADCMenu();
void drawEncodMenu();
void drawDACMenu();
void drawSettinMenu();
void drawButtonMenu();
void drawCurrentMenu(uint8_t menu_num_current, uint8_t menu_num_next, uint8_t menu_prev_num, int8_t offset);
void udpateDisplay();

void setMaxSpeed(int8_t znak);
void OutputBattaryParam();

#endif /* INC_APP_MENU_H_ */
