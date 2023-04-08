#include "app_menu/app_menu.h"

int currentMenuItem = 0;
static uint8_t buttonUpper = 0;
static uint8_t buttonEnable = 0;
static int8_t  encoderr = 0;

// Названия пунктов меню
const char* menuItems[MENU_ITEMS_COUNT] = {
    "1.Motor   ",
    "2.LED     ",
    "3.E-ink   ",
	"4.ADC     ",
	"5.Encod.  ",
	"6.DAC     ",
	"0.Exit    "
};

void drawSubMenu(uint8_t i){
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString(menuItems[i], Font_7x10, White);
};


void buttonUpSet(uint8_t i){
	buttonUpper = i;
}
void buttonEnSet(uint8_t i){
	buttonEnable = i;
}
void encoderSet(int8_t i){
	encoderr = i;
}

uint8_t buttonUp(){
	return buttonUpper;
}
uint8_t buttonEn(){
	return buttonEnable;
}
int8_t encoder(){
	return encoderr;
}

/*
void drawItemNum00(){
	int8_t current_speed; // текущая скорость
	int8_t exit = 1;	 // флаг выхода
	char current_sight;  // знак скорости
	char c_speed[2]; // Храним символы текущей скорости
	ssd1306_Fill(Black);// чистим экран
	drawSubMenu(0); // Рисуем шапку
	ssd1306_SetCursor(0, 11);
	ssd1306_WriteString("Speed:  ", Font_7x10, White);
	while(exit){
		ssd1306_SetCursor(7, 11);
		if(speed == 0)
			current_sight = "0";
		if(speed > 0)
			current_sight = "+";
		if(speed < 0)
			current_sight = "-";
		//itoa(current_speed)
	        ssd1306_UpdateScreen();
	        HAL_Delay(100);
	    }

}

void drawItemNum01(){

}

void drawItemNum02(){

}

void drawItemNum03(){

}

void drawItemNum04(){

}

void drawItemNum05(){

}

void drawItemNum06(){

}
*/

void drawMainMenu() {
    ssd1306_Fill(Black);
    uint8_t exit = 1;
    uint8_t current_item_menu = 0;
    uint8_t next_item_menu = current_item_menu + 1;
    uint8_t prev_item_menu = MENU_ITEMS_COUNT - 1;
    ssd1306_DrawRectangle(1, 1, 127, 63, White);

#define START_POS_X 5
#define LAST_POS_X (127 - START_POS_X)

#define START_POS_Y 5
#define LAST_POS_Y (63 - START_POS_Y)

#define SIZE_FONT_X 7
#define SIZE_FONT_Y 10

#define SIZE_MAIN_FONT_X 11
#define SIZE_MAIN_FONT_Y 18
//#define MAIN_FONT_MENU Font_11x18
//#define SECOND_FONT_MENU Font_7x10
	//ssd1306_Line(START_POS_X, START_POS_Y+SIZE_FONT_Y+1, LAST_POS_X, START_POS_Y+SIZE_FONT_Y+1, White);
	//ssd1306_Line(START_POS_X, LAST_POS_Y- SIZE_FONT_Y-1, LAST_POS_X, LAST_POS_Y- SIZE_FONT_Y-1, White);
    ssd1306_UpdateScreen();
    while( exit ){
         next_item_menu = current_item_menu + 1;
         prev_item_menu = current_item_menu - 1;
    	if(current_item_menu == (MENU_ITEMS_COUNT-1))
    		next_item_menu = 0;
    	if(current_item_menu == 0) // Для нулевого элемента предыдущий символ будет последним
    		prev_item_menu = MENU_ITEMS_COUNT - 1;

    	ssd1306_SetCursor(START_POS_X, SIZE_FONT_Y + START_POS_Y+8);// Магический номер что бы сделать по центру
    		ssd1306_WriteString(">", Font_11x18, White);
    	ssd1306_SetCursor(START_POS_X+SIZE_MAIN_FONT_X+5, SIZE_FONT_Y + START_POS_Y+8);
    		ssd1306_WriteString(menuItems[current_item_menu], Font_11x18, White);
    	ssd1306_UpdateScreen();

    	HAL_Delay(50);

    	ssd1306_SetCursor(START_POS_X, START_POS_Y); // В первой строке пишем предыдущий пункт
    		ssd1306_WriteString(menuItems[prev_item_menu], Font_7x10, White);
    		ssd1306_UpdateScreen();

        	ssd1306_SetCursor(START_POS_X, LAST_POS_Y - SIZE_FONT_Y);
        		ssd1306_WriteString(menuItems[next_item_menu], Font_7x10, White);
        	ssd1306_UpdateScreen();

    	buttonEnSet(OFF);
    	while(buttonEn() != ON){}
    		current_item_menu++;
    		HAL_Delay(300);
    		if(current_item_menu >= MENU_ITEMS_COUNT )
    			current_item_menu = 0;
    }

}
