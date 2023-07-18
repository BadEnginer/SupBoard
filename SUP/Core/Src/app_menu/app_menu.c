#include "app_menu/app_menu.h"

osStatus_t statusMutex;

extern sSystemState SystemState;
int currentMenuItem = 0;
static uint8_t buttonUpper = 0;  	// Кнопка вверх нажата 1
static uint8_t buttonEnable = 0; 	// Кнопка ввода нажата 1
static uint8_t button_counter = 0;  // Количество отсчётом сколько кнопка удерживалась
static uint8_t button_long = 0;		// Кнопка ввода удержана 1
static int8_t  encoderAS56 = 0;		// Положение энкодера 1 крутят в плюс -1 крутят в минус
uint8_t calibr = 1;
// Названия пунктов меню
const char* menuItems[MENU_ITEMS_COUNT+1] = {
    " 1:Buttons ",
    "   2:LED   ",
    "  3:Error  ",
	"   4:ADC   ",
	" 5:Encoder ",
	"   6:DAC   ",
	" 7:Setting ",
	" Conf Menu "
};


const char* menuButton[3] = {
    "Lon:  Ena: ",
    "EDP:  EDM: ",
	"  Button   "
};

const char* menuEncoder[3] = {
    "MAG:   MD: ",
    "ANG:   MN: ",
	"  Encoder  "
};
#define MAX_ERROR_CONF_MENU 11
const char* menuSettings[MAX_ERROR_CONF_MENU+1] = {
	"Currt:     ",
	"Bat.V:     ",
    "BatType:   ",
    "Num Cell:  ",
    "Max.V:     ",
	"Mix.V:     ",
	"Capacity:  ",
	"Err.MZ:    ",
	"Err.ADC:   ",
	"Err.Bat:   ",
	"Err.DAC:   ",
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

#define LST_MENU_POINT (START_POS_Y + SIZE_FONT_Y + 2 + 2) // под надписью главное меню

#define START_FIRST_STRING_Y (22)
#define WITH_STIRNG_MID 18
#define WITH_STIRNG_BIG 24
#define WITH_STIRNG_MIN 8
#define LEGHT_MAX_STRING 11
#define LEGHT_MAX_CHAR_MID 11
#define SPACE_PLACE 1
#define START_SECOND_STRING_Y (START_FIRST_STRING_Y + SPACE_PLACE + WITH_STIRNG)

void drawCurrentMenu(uint8_t menu_num_current, uint8_t menu_num_next, uint8_t menu_prev_num, int8_t mode){
	// Эксперементальные режимы вывода на дисплей разных шрифтов и анимации
	if(mode == 0){
		// Единственная большая строчка с текущим меню
    	ssd1306_SetCursor(START_POS_X, START_FIRST_STRING_Y + 10); // 18 + 18 = 46
			ssd1306_WriteString(menuItems[menu_num_current], Font_11x18, White);
		ssd1306_SetCursor(START_POS_X, START_FIRST_STRING_Y + 10 ); // 18 + 18 = 46
			ssd1306_WriteString("", Font_11x18, White);
		ssd1306_SetCursor(LAST_POS_X - WITH_STIRNG_MID, START_FIRST_STRING_Y + 10 ); // 18 + 18 = 46
			ssd1306_WriteString("", Font_11x18, White);
    }
    else if(mode == 1){
    	// средняя строчка по центру с сверху и снизу мелкие следующий и предыдущий пункт
       	ssd1306_SetCursor(START_POS_X + WITH_STIRNG_MIN*4, START_FIRST_STRING_Y); //
    		ssd1306_WriteString(menuItems[menu_prev_num], Font_6x8, White);
    	ssd1306_SetCursor(START_POS_X, START_FIRST_STRING_Y + WITH_STIRNG_MIN + SPACE_PLACE);
    		ssd1306_WriteString(menuItems[menu_num_current], Font_11x18, White);
        ssd1306_SetCursor(START_POS_X, START_FIRST_STRING_Y + WITH_STIRNG_MIN + SPACE_PLACE);
        	ssd1306_WriteString("", Font_11x18, White);
        ssd1306_SetCursor(LAST_POS_X - WITH_STIRNG_MID, START_FIRST_STRING_Y + WITH_STIRNG_MIN + SPACE_PLACE);
        	  ssd1306_WriteString("", Font_11x18, White);
        ssd1306_SetCursor(START_POS_X + WITH_STIRNG_MIN * 4, START_FIRST_STRING_Y + WITH_STIRNG_MIN * 2 + SPACE_PLACE * 2);
        	ssd1306_WriteString(menuItems[menu_num_next], Font_6x8, White);
    }
    else if(mode == 2){
    	// попытка в анимацию ))
    	//if(menu_num_current - menu_prev_num > 0)
    	ssd1306_SetCursor(START_POS_X, START_POS_Y); // 18 = 19 ( последний пиксель) + линия ( 20 )
    			ssd1306_WriteString(menuItems[MENU_ITEMS_COUNT], Font_11x18, White);
    	ssd1306_Line(7, 21, 120, 21, White);
    	ssd1306_Line(7, 63, 120, 63, White);

    	for(uint8_t i = 0; i <= LEGHT_MAX_STRING ; i++ ){
    		ssd1306_SetCursor(START_POS_X + i * LEGHT_MAX_CHAR_MID, START_FIRST_STRING_Y+ 10 ); //
				ssd1306_WriteString(menuItems[menu_prev_num], Font_11x18, White);
			ssd1306_SetCursor(START_POS_X + (LEGHT_MAX_STRING*LEGHT_MAX_CHAR_MID) - i * LEGHT_MAX_CHAR_MID, START_FIRST_STRING_Y + 10 ); //
				ssd1306_WriteString(menuItems[menu_num_current], Font_11x18, White);
			ssd1306_SetCursor(START_POS_X, START_FIRST_STRING_Y + 10 ); //
				ssd1306_WriteString("", Font_11x18, White);
			ssd1306_SetCursor(LAST_POS_X - WITH_STIRNG_MID, START_FIRST_STRING_Y + 10 ); //
				ssd1306_WriteString("", Font_11x18, White);
			udpateDisplay();
			osDelay(10);
    	}
    }


	ssd1306_SetCursor(START_POS_X, START_POS_Y); // 18 = 19 ( последний пиксель) + линия ( 20 )
		ssd1306_WriteString(menuItems[MENU_ITEMS_COUNT], Font_11x18, White); // В конце мы пишим название меню для вывода поверх
}

void drawMainMenu() {
	SystemState.DisplayState.prevState = SystemState.DisplayState.state;
	SystemState.DisplayState.state = MAIN_CONFIG;
	SystemState.DisplayState.mode = 2;
    uint8_t exit = 1;
    int8_t  current_item_menu = 0;
    int8_t  next_item_menu = 0;
    int8_t  prev_item = 0;
    int8_t  znak = 0;
    while(exit){ // Вывод главного меню
        ssd1306_Fill(Black);
        // Рисует линию под словом меню 7 и 120 просто MN

    	if(current_item_menu >= MENU_ITEMS_COUNT )
			current_item_menu = 0;
    	if(current_item_menu < 0 )
    	    current_item_menu = MENU_ITEMS_COUNT-1;
    	next_item_menu = current_item_menu + 1;
    	if(next_item_menu >= MENU_ITEMS_COUNT )
    		next_item_menu = 0;
    	SystemState.DisplayState.CurrentMenu = 2;
    	drawCurrentMenu(current_item_menu, next_item_menu, prev_item, SystemState.DisplayState.mode);
    	ssd1306_Line(7, 21, 120, 21, White);
        ssd1306_Line(7, 63, 120, 63, White);
        udpateDisplay();
    	buttonEnReset();
        buttonLongReset();
        encoderReset();
    	while(1){
    		osDelay(300); // Даёт время для других задач
    		if(buttonLong()){
    		// Вернуться на стартовый дисплей
    			exit = 0;
    			break;
    		}

    		if(buttonLong()){
    			// Вернуться на стартовый дисплей
    			exit = 0;
    			break;
    		}
    		if(buttonEn()){// если нажали ввод переходим в подменю
    		    switch(current_item_menu){
    		    	case 0: drawButtonMenu();	break;
    		    	case 1: drawLEDMenu(); 		break;
    		    	case 2: drawSettinMenu();	break;
    		    	case 3: drawADCMenu();		break;
    		    	case 4: drawEncodMenu();	break;
    		    	case 5: drawDACMenu();		break;
    		    	case 6: drawE_inkMenu();	break;
    		    	//default: current_item_menu = 0;
    		    }
    		    SystemState.DisplayState.prevState = SystemState.DisplayState.state;
    		    SystemState.DisplayState.state = MAIN_CONFIG;
                buttonEnReset();
                buttonLongReset();
                encoderReset();
                break;
    		}
    		if(encoderData() > 0){
    			prev_item = current_item_menu;
    			current_item_menu++;
    			encoderReset();
    			break;
    		}
    		if(encoderData() < 0){
    			prev_item = current_item_menu;
    			current_item_menu++;
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



int8_t speed = 0;
//extern int16_t data_ch[NUM_ADC_CH][SIZE_ADC_BUFF];
extern sSystemState SystemState;
uint8_t start_callibr = 0;
#define MAX_CALIBR 50
int16_t current_speed;
int16_t current_current;
int16_t current_current_raw;
int16_t current_voltage_battery;
int16_t set_zero = 0;
int16_t dis_curretn;
int8_t  mantisa = 0;
int8_t  expter = 0;
int16_t old_current_zero;
int16_t current_Vout;
uint16_t deadArea = 0;
extern int16_t delta_encoder;
void startDisplay(){
	uint8_t calibrateData = 0;
	SystemState.DisplayState.prevState = SystemState.DisplayState.state;
	SystemState.DisplayState.state = START;
	// todo сделать отдельную функцию для упрощения
	    ssd1306_Fill(Black);
	    ssd1306_Line(0, 1, 128, 1, White);
	    ssd1306_SetCursor(STD_WHITESPACE, STD_WHITESPACE); //
	    ssd1306_WriteString("SPEED:", Font_11x18, White);
	    // х-отступ для линии у-отступ линии плюс ширина текста
	 	ssd1306_SetCursor(STD_WHITESPACE, STD_WHITESPACE+SIZE_FONT_Y);
	 	ssd1306_WriteString("Bat:", Font_11x18, White);
	 	ssd1306_SetCursor(STD_WHITESPACE, STD_WHITESPACE+SIZE_FONT_Y*2); //
	 	ssd1306_WriteString("Vout:", Font_11x18, White);
	 	ssd1306_Line(0, 63, 128, 63, White);
	 	udpateDisplay();
	 	old_current_zero = 0;
	 	char symSpeed[6];
	 	char symCurrent[6];
	 	char symVout[6];
	 	osDelay(500);
		buttonLongReset();
		buttonEnReset();
		encoderReset();
	 	while(1){
	 		osDelay(300);
	 		// Блок реакции на кнопки
	 		if(buttonEn()){
	 			calibrateData = 0;
	 			// Если кнопка Enter On прервать текущий цикл
	 			break;
	 		}
	 		if(encoderData() > 0){
	 			calibrateData = 0;
	 			// Если энкодер + повысить скорость.
	 			encoderReset();
	 		 	speed++;
	 		 	if(speed >= MAX_SPEED)
	 		 		speed = MAX_SPEED;
	 		}
	 		if(encoderData() < 0){
	 			calibrateData = 0;
	 			encoderReset();
	 		 	speed--;
	 		 	if(speed <= MIN_SPEED)
	 		 		speed = MIN_SPEED;
	 		}

	 		if(SystemState.ButtonsData.SetMaxSpeed == BUTTON_ON){
	 			SystemState.ButtonsData.SetMaxSpeed = BUTTON_OFF;
	 			if(SystemState.ButtonsData.setMaxSpeed > 0)
	 				speed = MAX_SPEED;
	 			else
	 				speed = MIN_SPEED;
	 		}

	 		if(buttonLong()){
	 			// Если Back включен скорость обноляем
	 			speed = 0;
	 			calibrateData++;
	 			buttonLongReset();
	 		}

	 		if(calibrateData >= 4){
	 			calibrateData = 0;
	 			SystemState.BattaryData.calibraty = 1;
	 		}

	 		SystemState.MotorData.current_speed = speed;
	 		current_speed = speed;

	 		current_current = SystemState.BattaryData.percentCharge; //1.0*(current_current_raw * LIN_CURR_K) - LIN_CURR_B;//(current_current_raw) * multi_speed;
	 		current_Vout = SystemState.BattaryData.voltage; // Убераем последний знак.

	 		itoa(current_speed, 	symSpeed, 10);
	 		itoa(current_Vout,		symVout,  10);
	 		itoa(current_current,	symCurrent,  10);

	 		symCurrent[3] = ' ';
	 		symCurrent[3] = '%';

	 		symVout[3] = symVout[2];
	 		symVout[2] = '.';
	 		symVout[5] = 'V';

	 		// х-линейный отступ плюс слово speed: 6 символов+1символ для знака : Вывод скорости
	 		ssd1306_SetCursor(STD_WHITESPACE + SIZE_FONT_X * 6, STD_WHITESPACE );
	 			ssd1306_WriteString("       ", Font_11x18, White);
	 		ssd1306_SetCursor(STD_WHITESPACE + SIZE_FONT_X * 6, STD_WHITESPACE );
	 			ssd1306_WriteString(symSpeed, Font_11x18, White);

	 		// х-линейный отступ плюс слово cur: 4 символов+1символ для знака : Вывод заряда
	 		ssd1306_SetCursor(STD_WHITESPACE + SIZE_FONT_X * 4, STD_WHITESPACE + SIZE_FONT_Y);
	 			ssd1306_WriteString("       ", Font_11x18, White);
	 		ssd1306_SetCursor(STD_WHITESPACE + SIZE_FONT_X * 4, STD_WHITESPACE + SIZE_FONT_Y);
	 			ssd1306_WriteString(symCurrent, Font_11x18, White);
	 		ssd1306_SetCursor(STD_WHITESPACE + SIZE_FONT_X * 7, STD_WHITESPACE + SIZE_FONT_Y);
	 			ssd1306_WriteString("%", Font_11x18, White);

	 		// х-линейный отступ плюс слово Vout: 5 символов : Вывод напряжения питания
	 		ssd1306_SetCursor(STD_WHITESPACE + SIZE_FONT_X * 5, STD_WHITESPACE + SIZE_FONT_Y*2);
	 			ssd1306_WriteString("       ", Font_11x18, White);
	 		ssd1306_SetCursor(STD_WHITESPACE + SIZE_FONT_X * 5, STD_WHITESPACE + SIZE_FONT_Y*2);
	 			ssd1306_WriteString(symVout, Font_11x18, White);

	 		udpateDisplay();
	 	}
  }


// Тестовая функция для проверки кнопок  готова
void drawButtonMenu(){
	SystemState.DisplayState.prevState = SystemState.DisplayState.state;
	SystemState.DisplayState.state = BUTTON_CONFIG;
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

		itoa(butEn,  sym_butEn, 10);
		itoa(butLo,  sym_butLo, 10);
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
		osDelay(50);

		if(butLo > 2){
			if (butEn > 2)
				butEn = 0;
			SystemState.DisplayState.mode = butEn;
			break;
		}
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
	SystemState.DisplayState.prevState = SystemState.DisplayState.state;
	SystemState.DisplayState.state = ADC_CONFIG;
	uint8_t ch0 = 0;
	uint8_t ch1 = 0;
	uint8_t ch2 = 0;
	uint8_t ch3 = 0;

	char sym_ch0 [3];
	char sym_ch1 [3];
	char sym_ch2 [3];
	char sym_ch3 [3];

	buttonEnReset();
	buttonLongReset();
	encoderReset();

	ssd1306_Fill(Black);
	ssd1306_SetCursor(START_POS_X, START_POS_Y);
		ssd1306_WriteString(menuADC[2], Font_11x18, White);
	ssd1306_SetCursor(START_POS_X, START_POS_Y + SIZE_FONT_Y);
		ssd1306_WriteString(menuADC[0], Font_11x18, White);
	ssd1306_SetCursor(START_POS_X, START_POS_Y + SIZE_FONT_Y*2);
		ssd1306_WriteString(menuADC[1], Font_11x18, White);
	udpateDisplay();
	osDelay(50);
	buttonEnReset();
	buttonLongReset();
	encoderReset();
	while(1){
		ch0 = (SystemState.AdcData.chanel_0_voltage );
		ch1 = (SystemState.AdcData.chanel_1_voltage );
		ch2 = (SystemState.AdcData.chanel_2_voltage );
		ch3 = (SystemState.AdcData.chanel_3_voltage );

		itoa(ch0, sym_ch0, 10);
		itoa(ch1, sym_ch1, 10);
		itoa(ch2, sym_ch2, 10);
		itoa(ch3, sym_ch3, 10);

		ssd1306_SetCursor(SIZE_FONT_X * 3, START_POS_Y + SIZE_FONT_Y);
			ssd1306_WriteString(sym_ch0, Font_11x18, White);
		ssd1306_SetCursor(SIZE_FONT_X * 9, START_POS_Y + SIZE_FONT_Y);
			ssd1306_WriteString(sym_ch1, Font_11x18, White);
		ssd1306_SetCursor(SIZE_FONT_X * 3, START_POS_Y + SIZE_FONT_Y*2);
			ssd1306_WriteString(sym_ch2, Font_11x18, White);
		ssd1306_SetCursor(SIZE_FONT_X * 9, START_POS_Y + SIZE_FONT_Y*2);
			ssd1306_WriteString(sym_ch3, Font_11x18, White);
		udpateDisplay();

		if(buttonLong() == ON){
			buttonLongReset();
			break;
		}
		osDelay(50);
	}
}
void drawEncodMenu(){
	uint8_t exit = 0;
}
void drawDACMenu(){
	uint8_t exit = 0;
}
void drawSettinMenu(){
	SystemState.DisplayState.prevState = SystemState.DisplayState.state;
	SystemState.DisplayState.state = ERROR_CONFIG;

	int8_t current_menu = 1;
	char sym_current [4];
	char sym_vbat    [5];
	char sym_battType[3];
	char sym_numCell [2];
	char sym_maxVbat [4];
	char sym_minVbat [4];
	char sym_capacity[2];
	char sym_ErrMigZ [3];
	char sym_ErrADC  [3];
	char sym_ErrBAT  [3];
	char sym_ErrDAC  [3];


	buttonEnReset();
	buttonLongReset();
	encoderReset();

	osDelay(50);

	buttonEnReset();
	buttonLongReset();
	encoderReset();
	while(1){
		itoa(SystemState.BattaryData.current, sym_current,	10);
		itoa(SystemState.BattaryData.voltage, sym_vbat, 	10);
		itoa(SystemState.BattaryData.numCell, sym_numCell,	10);

		itoa(SystemState.BattaryData.MaxCellVoltage, sym_maxVbat, 10);
		itoa(SystemState.BattaryData.MinCellVoltage, sym_minVbat, 10);

		strcpy(sym_capacity,"00");

		if(SystemState.BattaryData.BatteryType == BATTARY_TYPE_LIPO)
			strcpy(sym_battType,"Li");
		else
			strcpy(sym_battType, "Fe");

		if(SystemState.ErrorState.ErrorMigrationZero == ZERO_OK)
			strcpy(sym_ErrMigZ, "OFF");
		else
			strcpy(sym_ErrMigZ, "ON");

		if(SystemState.ErrorState.error_ADC == DEVISE_OK)
			strcpy(sym_ErrADC,"OK");
		else if (SystemState.ErrorState.error_ADC == DEVISE_ERROR)
			strcpy(sym_ErrADC,"Err");
		else
			strcpy(sym_ErrADC,"FE!");
		if(SystemState.ErrorState.error_DAC_ADC )
		sym_ErrBAT  [3];
		sym_ErrDAC  [3];

		ssd1306_Fill(Black);
		ssd1306_SetCursor(START_POS_X, START_POS_Y);
			ssd1306_WriteString(menuSettings[current_menu-1], Font_11x18, White);
		ssd1306_SetCursor(START_POS_X, START_POS_Y + SIZE_FONT_Y);
			ssd1306_WriteString(menuSettings[current_menu], Font_11x18, White);
		ssd1306_SetCursor(START_POS_X, START_POS_Y + SIZE_FONT_Y*2);
			ssd1306_WriteString(menuSettings[current_menu+1], Font_11x18, White);
		udpateDisplay();

		if(encoderData > 0){
			current_menu ++;
			if(current_menu > MAX_ERROR_CONF_MENU - 1)
				current_menu = 1;
		}
		if(encoderData < 0){
			current_menu --;
			if(current_menu < 1)
				current_menu = MAX_ERROR_CONF_MENU - 1;
		}
		if(buttonLong() == ON){
			buttonLongReset();
			break;
		}
		osDelay(50);
	}
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
	SystemState.ButtonsData.ButtonEN= BUTTON_ON;
	SystemState.ButtonsData.ButtonENCounter++;
	buttonEnable = 1;
}
void buttonEnReset(){
	SystemState.ButtonsData.ButtonEN= BUTTON_OFF;
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
	SystemState.ButtonsData.ButtonBACK = BUTTON_ON;
	SystemState.ButtonsData.ButtonBACKCounter++;
	button_long = 1;
}
void buttonLongReset(){
	SystemState.ButtonsData.ButtonBACK = BUTTON_OFF;
	button_long = 0;
}

int8_t encoderData(){
	return encoderAS56;
}

void encoderSetUp(){
	SystemState.ButtonsData.EncoderPLUS = BUTTON_ON;
	SystemState.ButtonsData.EncoderPLUSCounter++;
	encoderAS56 = 1;
}
void setMaxSpeed(int8_t znak){
	SystemState.ButtonsData.SetMaxSpeed = BUTTON_ON;
	SystemState.ButtonsData.setMaxSpeed = znak;
}

void encoderSetDown(){
	SystemState.ButtonsData.EncoderMINUS = BUTTON_ON;
	SystemState.ButtonsData.EncoderMINUSCounter++;
	encoderAS56 = -1;
}

void encoderReset(){
	SystemState.ButtonsData.EncoderMINUS = BUTTON_OFF;
	SystemState.ButtonsData.EncoderPLUS = BUTTON_OFF;
	encoderAS56 = 0;
}


