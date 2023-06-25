
#include "tasks/OledMenu.h"
extern sSystemState SystemState;

void StartOledMenuTask(void *argument){
	HAL_Delay(200);
	if (SystemState.ErrorState.errorCounter > 0){
		SystemState.DisplayState.prevState = SystemState.DisplayState.state;
		SystemState.DisplayState.state = ERROR_STATE_I2C;
		ssd1306_Fill(Black);
		OutputErrorI2C();
	}

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
