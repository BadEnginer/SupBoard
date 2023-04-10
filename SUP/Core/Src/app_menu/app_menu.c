#include "app_menu/app_menu.h"

osStatus_t statusMutex;

int currentMenuItem = 0;
static uint8_t buttonUpper = 0;  	// Кнопка вверх нажата 1
static uint8_t buttonEnable = 0; 	// Кнопка ввода нажата 1
static uint8_t button_counter = 0;  // Количество отсчётом сколько кнопка удерживалась
static uint8_t button_long = 0;		// Кнопка ввода удержана 1
static int8_t  encoderAS56 = 0;		// Положение энкодера 1 крутят в плюс -1 крутят в минус

// Названия пунктов меню
const char* menuItems[MENU_ITEMS_COUNT] = {
    "1:Button  ",
    "2:LED     ",
    "3:E-ink   ",
	"4:ADC     ",
	"5:Encod.  ",
	"6:DAC     ",
	"0:Exit    "
};

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
    udpateDisplay();
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

    	udpateDisplay();
    	HAL_Delay(50);

    	ssd1306_SetCursor(START_POS_X, START_POS_Y); // В первой строке пишем предыдущий пункт
    		ssd1306_WriteString(menuItems[prev_item_menu], Font_7x10, White);
    	udpateDisplay();

        	ssd1306_SetCursor(START_POS_X, LAST_POS_Y - SIZE_FONT_Y);
        		ssd1306_WriteString(menuItems[next_item_menu], Font_7x10, White);
        udpateDisplay();
        HAL_Delay(500);
        buttonEnReset();
        buttonLongReset();
        encoderReset();
    	while(1){
    		if(encoderData() > 0){
    			current_item_menu++;
    			encoderReset();
    			break;
    		}
    		if(encoderData() < 0){
    			current_item_menu--;
    			encoderReset();
    			break;
    		}
    		if(buttonLong()){
    			// Вернуться на стартовый дисплей
    			current_item_menu = 0;
    			break;
    		}
    		if(buttonEn()){
    			switch(current_item_menu){
    				case 0: drawItemNum01(); break;
    				case 1: drawItemNum02(); break;
    				case 2: drawItemNum03(); break;
    				case 3: drawItemNum04(); break;
    				case 4: drawItemNum05(); break;
    				case 5: drawItemNum06(); break;
    			}
    		}
    		if(current_item_menu >= MENU_ITEMS_COUNT )
    			current_item_menu = 0;
    	}
    }

}

extern osMutexId_t BlockI2CHandle;
void udpateDisplay(){
	statusMutex = osMutexAcquire(BlockI2CHandle, 1000);
	if(statusMutex == osOK)
		ssd1306_UpdateScreen();
	osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
}

void drawSubMenu(uint8_t i){
	ssd1306_SetCursor(10, 10);
	ssd1306_WriteString(menuItems[i], Font_7x10, White);
}

uint8_t buttonUp(){
	return buttonUpper;
}
void buttonUpSet(){
	buttonUpper = 1;
}
void buttonUpReset(){
	buttonUpper = 0;
}

uint8_t buttonEn(){
	return buttonEnable;
}
void buttonEnSet(){
	buttonEnable = 1;
}
void buttonEnReset(){
	buttonEnable = 0;
}

uint8_t buttonCounter(){
	return button_counter;
}
void buttonCounterSet(){
	button_counter++;
}

void buttonCounterReset(){
	button_counter = 0;
}

uint8_t buttonLong(){
	return button_long;
}
void buttonLongSet(){
	button_long = 1;
}
void buttonLongReset(){
	button_long = 0;
}

int8_t encoderData(){
	return encoderAS56;
}
void encoderSetUp(){
	encoderAS56 = 1;
}

void encoderSetDown(){
	encoderAS56 = -1;
}

void encoderReset(){
	encoderAS56 = 0;
}


/*
 *  	  char sym[3];
  	  itoa(raw_angle,sym,10);
		  	  ssd1306_SetCursor(67, 0);
		      ssd1306_WriteString(sym, Font_11x18, White);
		      ssd1306_SetCursor(67, 19);
		      ssd1306_UpdateScreen();
		      HAL_Delay(40);
 */
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
*/
void drawItemNum01(){
	uint8_t butEn = 0;
	uint8_t butLo = 0;
	 int8_t encod = 0;
	char sym_butEn[3];
	char sym_butLo[3];
	char sym_encod[3];



	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(1, 1, 127, 63, White);
	drawSubMenu(0);
	ssd1306_SetCursor(10, 29);
	ssd1306_WriteString("ButEn:", Font_7x10, White);
	ssd1306_SetCursor(10, 40);
	ssd1306_WriteString("ButLo:", Font_7x10, White);
	ssd1306_SetCursor(10, 51);
	ssd1306_WriteString("Encod:", Font_7x10, White);
	while(1){
		if(buttonEn() == ON){
			buttonEnReset();
			butEn++;
		}
		if(buttonLong() == ON){
			buttonLongReset();
			butLo++;
		}
		if(encoderData() > 0){
			encoderReset();
			encod++;
		}
		if(encoderData() < 0){
			encoderReset();
			encod--;
		}
		itoa(butEn, sym_butEn, 10);
		itoa(butLo, sym_butLo, 10);
		itoa(encod, sym_encod, 10);
		ssd1306_SetCursor(10, 29);
		ssd1306_WriteString("ButEn:", Font_7x10, White);
		ssd1306_SetCursor(10, 40);
		ssd1306_SetCursor(10, 40);
		udpateDisplay();
		HAL_Delay(80);
	}
}

void drawItemNum02(){
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(1, 1, 127, 63, White);
	drawSubMenu(1);
	udpateDisplay();
}

void drawItemNum03(){
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(1, 1, 127, 63, White);
	drawSubMenu(2);
	udpateDisplay();
}

void drawItemNum04(){
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(1, 1, 127, 63, White);
	drawSubMenu(3);
	udpateDisplay();
}

void drawItemNum05(){
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(1, 1, 127, 63, White);
	drawSubMenu(4);
	udpateDisplay();
}

void drawItemNum06(){
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(1, 1, 127, 63, White);
	drawSubMenu(5);
	udpateDisplay();
}

