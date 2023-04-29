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
    "Lon:  Ena: ",
    "EPC:  EMC: ",
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

    uint8_t exit = 1;
    int8_t  current_item_menu = 0;
    ssd1306_Fill(Black);
    ssd1306_DrawRectangle(1, 1, 127, 63, White);
    udpateDisplay();

    while(exit){ // Вывод главного меню
        ssd1306_Fill(Black);
        ssd1306_DrawRectangle(1, 1, 127, 63, White);
        udpateDisplay();
    	if(current_item_menu >= MENU_ITEMS_COUNT )
			current_item_menu = 0;
    	if(current_item_menu <= 0 )
    	    current_item_menu = 0;
    	ssd1306_SetCursor(START_POS_X, START_POS_Y+6);
    		ssd1306_WriteString(menuItems[MENU_ITEMS_COUNT], Font_11x18, White);
    	ssd1306_SetCursor(START_POS_X, START_POS_Y + SIZE_FONT_Y + 5+6);
    		ssd1306_WriteString(menuItems[current_item_menu], Font_11x18, White);
    	udpateDisplay();
        buttonEnReset();
        buttonLongReset();
        encoderReset();
        HAL_Delay(500);
    	while(1){
    		if(buttonLong()){
    			// Вернуться на стартовый дисплей
    			exit = 0;
    			break;
    		}
    		if(buttonEn()){// если нажали ввод переходим в подменю
    			HAL_Delay(600);
        		if(buttonLong()){ // если нажата кнопка подтверждения ждем что это не выход
        			// Вернуться на стартовый дисплей
        			exit = 0;
        			break;
        		}
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
                buttonEnReset();
                buttonLongReset();
                encoderReset();
                break;
    		}
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


#define SPEED_STEP 30
#define MAX_SPEED  (1500/SPEED_STEP)
#define MIN_SPEED ((-1) * MAX_SPEED)
#define STD_WHITESPACE 7 // с учётом линии по краям экрана
int8_t speed = 0;
extern int16_t data_ch[NUM_ADC_CH][SIZE_ADC_BUFF];
extern uint16_t global_DAC;

void startDisplay(){
	// todo сделать отдельную функцию для упрощения
	    ssd1306_Fill(Black);
	    ssd1306_Line(0, 1, 128, 1, White);
	    ssd1306_SetCursor(STD_WHITESPACE, STD_WHITESPACE); //
	    ssd1306_WriteString("SPEED:", Font_11x18, White);
	    // х-отступ для линии у-отступ линии плюс ширина текста
	 	ssd1306_SetCursor(STD_WHITESPACE, STD_WHITESPACE+SIZE_FONT_Y);
	 	ssd1306_WriteString("CUR:", Font_11x18, White);
	 	ssd1306_SetCursor(STD_WHITESPACE, STD_WHITESPACE+SIZE_FONT_Y*2); //
	 	ssd1306_WriteString("Vout:", Font_11x18, White);
	 	ssd1306_Line(0, 63, 128, 63, White);
	 	udpateDisplay();
	 	uint16_t current_speed;
	 	int16_t current_current;
	 	uint16_t current_Vout;
	 	char symSpeed[5];
	 	char symCurrent[5];
	 	char symVout[5];
	 	while(1){
	 		HAL_Delay(350);
	 		if(buttonLong()){
	 			speed = 0;
	 			buttonLongReset();
	 		}
	 		if(encoderData() > 0){
	 			encoderReset();
	 		 	speed++;
	 		 	if(speed >= MAX_SPEED)
	 		 		speed = MAX_SPEED;
	 		}
	 		if(encoderData() < 0){
	 			encoderReset();
	 		 	speed--;
	 		 	if(speed <= MIN_SPEED)
	 		 		speed = MIN_SPEED;
	 		}
	 		global_DAC = 1500 + (SPEED_STEP*speed);

	 		itoa(current_speed  , symSpeed  , 10);
	 		itoa(current_current, symCurrent, 10);
	 		itoa(current_Vout   , symVout   , 10);
	 		current_speed = speed;
	 		current_current = (((aver_mass(data_ch[2])* ADC_TO_V)-1500)*V_TO_A)-3900;
	 		current_Vout = aver_mass(data_ch[1]) * ADC_TO_V;
	 		// х-линейный отступ плюс слово speed: 6 символов+1символ для знака : Вывод скорости
	 		ssd1306_SetCursor(STD_WHITESPACE + SIZE_FONT_X * 7, STD_WHITESPACE );
	 			ssd1306_WriteString(symSpeed, Font_11x18, White);
	 		// х-линейный отступ плюс слово cur: 4 символов+1символ для знака : Вывод тока
	 		ssd1306_SetCursor(STD_WHITESPACE + SIZE_FONT_X * 5, STD_WHITESPACE + SIZE_FONT_Y);
	 			ssd1306_WriteString(symCurrent, Font_11x18, White);
	 		// х-линейный отступ плюс слово Vout: 5 символов : Вывод напряжения управления
	 		ssd1306_SetCursor(STD_WHITESPACE + SIZE_FONT_X * 5, STD_WHITESPACE + SIZE_FONT_Y*2);
	 			ssd1306_WriteString(symVout, Font_11x18, White);
	 		udpateDisplay();
	 	}
  }

// Тестовая функция для проверки кнопок  готова
void drawButtonMenu(){
	uint8_t butEn = 0;
	uint8_t butLo = 0;
	uint8_t encodP = 0;
	uint8_t encodM = 0;

	char sym_butEn[1];
	char sym_butLo[1];
	char sym_encP[1];
	char sym_encM[1];
	buttonEnReset();
	buttonLongReset();
	encoderReset();

	ssd1306_Fill(Black);
	ssd1306_SetCursor(START_POS_X, START_POS_Y);
		ssd1306_WriteString(menuButton[2], Font_11x18, White);
	ssd1306_SetCursor(START_POS_X, START_POS_Y + SIZE_FONT_Y);
		ssd1306_WriteString(menuButton[0], Font_11x18, White);
	ssd1306_SetCursor(START_POS_X, START_POS_Y + SIZE_FONT_Y*2);
		ssd1306_WriteString(menuButton[1], Font_11x18, White);

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
			encodP++;
		}
		if(encoderData() < 0){
			encoderReset();
			encodM++;
		}
		encodM %=10;
		encodP %=10;

		butEn = butEn % 10;
		butLo = butLo % 10;

		itoa(butEn, sym_butEn, 10);
		itoa(butLo, sym_butLo, 10);
		itoa(encodM, sym_encM, 10);
		itoa(encodP, sym_encP, 10);

		ssd1306_SetCursor(SIZE_FONT_X * 4, START_POS_Y + SIZE_FONT_Y);
			ssd1306_WriteString(sym_butLo, Font_11x18, White);
		ssd1306_SetCursor(SIZE_FONT_X * 10, START_POS_Y + SIZE_FONT_Y);
			ssd1306_WriteString(sym_butEn, Font_11x18, White);
		ssd1306_SetCursor(SIZE_FONT_X * 4, START_POS_Y + SIZE_FONT_Y*2);
			ssd1306_WriteString(sym_encM, Font_11x18, White);
		ssd1306_SetCursor(SIZE_FONT_X * 10, START_POS_Y + SIZE_FONT_Y*2);
			ssd1306_WriteString(sym_encP, Font_11x18, White);

		udpateDisplay();
		HAL_Delay(50);
		if(butLo > 2)
			break;
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
extern uint16_t global_DAC;


