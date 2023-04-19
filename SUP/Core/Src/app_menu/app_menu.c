#include "app_menu/app_menu.h"

osStatus_t statusMutex;

int currentMenuItem = 0;
static uint8_t buttonUpper = 0;  	// Кнопка вверх нажата 1
static uint8_t buttonEnable = 0; 	// Кнопка ввода нажата 1
static uint8_t button_counter = 0;  // Количество отсчётом сколько кнопка удерживалась
static uint8_t button_long = 0;		// Кнопка ввода удержана 1
static int8_t  encoderAS56 = 0;		// Положение энкодера 1 крутят в плюс -1 крутят в минус

// Названия пунктов меню
const char* menuItems[MENU_ITEMS_COUNT+1] = {
    "1:Button   ",
    "2:LED      ",
    "3:E-ink    ",
	"4:ADC      ",
	"5:Encod.   ",
	"6:DAC      ",
	"7:Settings ",
	"   Menu    "
};


const char* menuButton[3] = {
    "Lon:  Enc: ",
    "Ena:       ",
	"  Button   "
};

const char* menuEncoder[3] = {
    "MAG:   MD: ",
    "ANG:   MN: ",
	"  Encoder  "
};
const char* menuSettings[7] = {
    "Default:   ",
    "Сell:      ",
	"Сell min:  ",
    "Max.V:     ",
	"Mix.V:     ",
	"Capacity:  ",
	" Settings  "
};

const char* menuDAC[3] = {
    "Lon:  Ena: ",
    "Encod:     ",
	"    DAC    "
};

const char* menuE_inc[3] = {
    "Pic.1 Pic.2",
	"Pic.3 Pic.4",
	"   E-ink   "
};

const char* menuLED[3] = {
    "Num:  GRN: ",
    "RED:  BLU: ",
	"    LED    "
};

const char* menuADC[3] = {
    "C1:   C3:  ",
    "C2:   C4:  ",
	"    ADC    "
};

void drawMainMenu() {
    ssd1306_Fill(Black);
    uint8_t exit = 1;
    int8_t  current_item_menu = 0;
    ssd1306_DrawRectangle(1, 1, 127, 63, White);
    udpateDisplay();

    while(1){ // Вывод главного меню
    	exit = ON;
    	if(current_item_menu >= MENU_ITEMS_COUNT )
			current_item_menu = 0;
    	if(current_item_menu <= 0 )
    	    current_item_menu = 0;
    	ssd1306_SetCursor(START_POS_X, START_POS_Y);
    		ssd1306_WriteString(menuItems[MENU_ITEMS_COUNT+1], Font_11x18, White);
    	ssd1306_SetCursor(START_POS_X, START_POS_Y + SIZE_FONT_Y + 5);
    		ssd1306_WriteString(menuItems[current_item_menu], Font_11x18, White);
    	udpateDisplay();
        buttonEnReset();
        buttonLongReset();
        encoderReset();
        HAL_Delay(800);
    	while(exit){
    		if(buttonLong()){
    			// Вернуться на стартовый дисплей
    			current_item_menu = 0;
    			break;
    		}
    		if(buttonEn()){
    		    			switch(current_item_menu){
    		    				case 0: drawButtonMenu();	break;
    		    				case 1: drawLEDMenu(); 		break;
    		    				case 2: drawE_inkMenu();	break;
    		    				case 3: drawADCMenu();		break;
    		    				case 4: drawEncodMenu();	break;
    		    				case 5: drawDACMenu();		break;
    		    				case 6: drawSettinMenu();	break;
    		    				default: current_item_menu = 0;
    		    			}
    		}
    		if(encoderData() > 0){
    			current_item_menu++;
    			exit = 0;
    			encoderReset();
    			break;
    		}
    		if(encoderData() < 0){
    			current_item_menu--;
    			exit = 0;
    			encoderReset();
    			break;
    		}
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
#define LED_NUM 8
struct sLedData{
	uint8_t numLED	[LED_NUM];
	uint8_t on_off	[LED_NUM];
	uint8_t red		[LED_NUM];
	uint8_t grn		[LED_NUM];
	uint8_t blu		[LED_NUM];
}LedData;

void drawButtonMenu(){
	uint8_t butEn = 0;
	uint8_t butLo = 0;
	 int8_t encod = 0;
	char sym_butEn[1];
	char sym_butLo[1];
	char sym_encod[2];

	ssd1306_Fill(Black);
	ssd1306_SetCursor(START_POS_X, START_POS_Y);
		ssd1306_WriteString(menuButton[3], Font_11x18, White);
	ssd1306_SetCursor(START_POS_X, START_POS_Y + SIZE_FONT_Y);
		ssd1306_WriteString(menuButton[1], Font_11x18, White);
	ssd1306_SetCursor(START_POS_X, START_POS_Y + SIZE_FONT_Y*2);
		ssd1306_WriteString(menuButton[2], Font_11x18, White);

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
		butEn = butEn % 10;
		butLo = butLo % 10;
		encod = encod % 10;

		itoa(butEn, sym_butEn, 10);
		itoa(butLo, sym_butLo, 10);
		itoa(encod, sym_encod, 10);

		ssd1306_SetCursor(SIZE_FONT_X * 4, START_POS_Y + SIZE_FONT_Y);
			ssd1306_WriteString(sym_butEn, Font_11x18, White);
		ssd1306_SetCursor(SIZE_FONT_X * 10, START_POS_Y + SIZE_FONT_Y);
			ssd1306_WriteString(sym_butLo, Font_11x18, White);
		ssd1306_SetCursor(SIZE_FONT_X * 5, START_POS_Y + SIZE_FONT_Y*2);
			ssd1306_WriteString(sym_encod, Font_11x18, White);

		udpateDisplay();
		HAL_Delay(50);
	}
}

void drawLEDMenu(){
	uint8_t currentLed = 0;
	uint8_t red,grn,blu;
}

void drawE_inkMenu(){
	uint8_t exit = 0;
}

void drawADCMenu(){
	uint8_t exit = 0;
}
void drawEncodMenu(){
	uint8_t exit = 0;
}
void drawDACMenu(){
	uint8_t exit = 0;
}
void drawSettinMenu(){
	uint8_t exit = 0;
}

/*
const char* menuButton[3] = {
	"  Button   "
    "Lon:  Ena: ",
    "Enc:       ",

};

const char* menuEncoder[3] = {
    "MAG:   MD: ",
    "ANG:   MN: ",
	"  Encoder  "
};
const char* menuSettings[7] = {
    "Default:   ",
    "Сell:      ",
	"Сell min:  ",
    "Max.V:     ",
	"Mix.V:     ",
	"Capacity:  ",
	" Settings  "
};

const char* menuDAC[3] = {
    "Lon:  Ena: ",
    "Encod:     ",
	"    DAC    "
};

const char* menuE_inc[3] = {
    "Pic.1 Pic.2",
	"Pic.3 Pic.4",
	"   E-ink   "
};

const char* menuLED[3] = {
    "Num:  GRN: ",
    "RED:  BLU: ",
	"    LED    "
};

const char* menuADC[3] = {
    "C1:   C3:  ",
    "C2:   C4:  ",
	"    ADC    "
};

*/
