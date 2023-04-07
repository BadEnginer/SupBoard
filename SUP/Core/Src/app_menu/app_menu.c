#include "app_menu/app_menu.h"

int currentMenuItem = 0;
static uint8_t buttonUpper = 0;
static uint8_t buttonEnable = 0;
static int8_t  encoderr = 0;

// Названия пунктов меню
const char* menuItems[MENU_ITEMS_COUNT] = {
    "00:Motor",
    "01:LED  ",
    "02:E-ink",
	"03:ADC  ",
	"04:Encod",
	"05:DAC  ",
	"06:Exit "
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
    while( exit ){
         next_item_menu = current_item_menu + 1;
         prev_item_menu = current_item_menu - 1;
    	if(current_item_menu == (MENU_ITEMS_COUNT-1))
    		next_item_menu = 0;
    	if(current_item_menu == 0) // Для нулевого элемента предыдущий символ будет последним
    		prev_item_menu = MENU_ITEMS_COUNT - 1;

    	ssd1306_SetCursor(0, 0);
    	ssd1306_WriteString(menuItems[prev_item_menu], Font_11x18, White);

    	ssd1306_SetCursor(0, MENU_ITEM_HEIGHT);
    	ssd1306_WriteString("> ", Font_16x24, White);
    	ssd1306_SetCursor(17, MENU_ITEM_HEIGHT);
    		ssd1306_WriteString(menuItems[current_item_menu], Font_16x24, White);

    	ssd1306_SetCursor(0, MENU_ITEM_MAIN_HEIGHT+MENU_ITEM_HEIGHT);
    	ssd1306_WriteString(menuItems[next_item_menu], Font_11x18, White);

    	ssd1306_UpdateScreen();
    	buttonEnSet(OFF);
    	while(buttonEn() != ON){}
    		current_item_menu++;
    		if(current_item_menu >= MENU_ITEMS_COUNT )
    			current_item_menu = 0;
    }

}
