
#include "tasks/OledMenu.h"
extern sSystemState SystemState;

void StartOledMenuTask(void *argument){
	// Меню для переключения между меню настроек и стартовым дисплеем
	osDelay(200);
	OutputBattaryParam();
	SystemState.DisplayState.prevState = SystemState.DisplayState.state;
	SystemState.DisplayState.state = WAIT_COMMAND;
	while(buttonEn() != ON){
		osDelay(50); // ждём нажатия кнопки старт
	}
	buttonEnReset();
	for(;;){
		osDelay(50);
		if(buttonEn() == ON){
			drawMainMenu();
		}
		startDisplay();
		udpateDisplay();
	}
}

void OutputBattaryParam(){
	SystemState.DisplayState.prevState = SystemState.DisplayState.state;
	SystemState.DisplayState.state = BATTARY_CONFIG;
	char* battTypeLip = {"BatType:LiPo"};
	char* battTypeFeP = {"BatType:LiFe"};
	char* numCell1s =   {"NumCell: 1s"};
	char* numCell2s =   {"NumCell: 2s"};
	char* numCell3s =   {"NumCell: 3s"};
	char* numCell4s =   {"NumCell: 4s"};
	char* numCell5s =   {"NumCell: 5s"};
	char* numCell6s =   {"NumCell: 6s"};
	char* currentNumCells = numCell4s;
	char* currentTypeBatt = battTypeLip;
	ssd1306_Fill(Black);
	SystemState.BattaryData.BatteryType = BATTARY_TYPE_LIPO;
	SystemState.BattaryData.numCell = NUM_CELL_4S;
	 while(1){
		ssd1306_SetCursor(2, 1);
				ssd1306_WriteString("Batt.Option", Font_11x18, White);
			ssd1306_Line(10, 21, 120, 21, White);
		 ssd1306_SetCursor(2, 25);
		 	 ssd1306_WriteString(currentTypeBatt, Font_11x18, White);
		 ssd1306_SetCursor(2,45);
			 ssd1306_WriteString(currentNumCells, Font_11x18, White);
		 ssd1306_UpdateScreen();
		 osDelay(100);
		 if(buttonEn()){
		 	 // Если кнопка Enter On прервать текущий цикл
			 break;
		 }
		 if(encoderData() > 0){
		 	 // Если энкодер + повысить скорость.
			 encoderReset();
			 if(currentTypeBatt == battTypeLip){
				 currentTypeBatt = battTypeFeP;
				 SystemState.BattaryData.BatteryType = BATTARY_TYPE_LIFE;
			 }
			 else{
				 currentTypeBatt = battTypeLip;
				 SystemState.BattaryData.BatteryType = BATTARY_TYPE_LIPO;
			 }
		 }
		 if(encoderData() < 0){
			 encoderReset();
			 if(currentNumCells == numCell1s){
				 currentNumCells = numCell2s;
				 SystemState.BattaryData.numCell = NUM_CELL_2S;
			 }
			 else if (currentNumCells == numCell2s){
				 currentNumCells = numCell3s;
				 SystemState.BattaryData.numCell = NUM_CELL_3S;
			 }
			 else if (currentNumCells == numCell3s){
				 SystemState.BattaryData.numCell = NUM_CELL_4S;
			 	 currentNumCells = numCell4s;
			 }
			 else if (currentNumCells == numCell4s){
				 SystemState.BattaryData.numCell = NUM_CELL_5S;
			 	 currentNumCells = numCell5s;
			 }
			 else if (currentNumCells == numCell5s){
			 	 currentNumCells = numCell6s;
			 	 SystemState.BattaryData.numCell = NUM_CELL_6S;
			 }
			 else if (currentNumCells == numCell6s){
			 	currentNumCells = numCell1s;
			 	SystemState.BattaryData.numCell = NUM_CELL_1S;
			 }
		 }
	 }


		if(SystemState.BattaryData.BatteryType == BATTARY_TYPE_LIPO){
			SystemState.BattaryData.MaxCellVoltage = 4200;
			SystemState.BattaryData.MinCellVoltage = 3000;
		}

		if(SystemState.BattaryData.BatteryType == BATTARY_TYPE_LIFE){
			SystemState.BattaryData.MaxCellVoltage = 3558;
			SystemState.BattaryData.MinCellVoltage = 3182;
		}
}



