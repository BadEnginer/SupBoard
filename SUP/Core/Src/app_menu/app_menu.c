/*
 * app_menu.c
 *
 *  Created on: Apr 7, 2023
 *      Author: BadEnginer
 */


int currentMenuItem = 0;
// Названия пунктов меню
const char* menuItems[MENU_ITEMS_COUNT] = {
    "00: Motor",
    "01: LED",
    "02: E-ink",
	"03: ADC",
	"04: Enc",
	"05: DAC",
	"06: Exit"
};

void drawSubMenu(uint8_t i){
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString(menuItems[i], Font_7x10, White);
};

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

void drawMainMenu() {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_SetCursor(0, MENU_ITEM_HEIGHT);
    for (int i = 0; i < MENU_ITEMS_COUNT; i++) {
        if (i == currentMenuItem) {
            ssd1306_WriteString("> ", Font_7x10, White);
        }
        ssd1306_WriteString(menuItems[i], Font_7x10, White);
        ssd1306_SetCursor(0, (i + 2) * MENU_ITEM_HEIGHT);
    }
    ssd1306_UpdateScreen();
}
