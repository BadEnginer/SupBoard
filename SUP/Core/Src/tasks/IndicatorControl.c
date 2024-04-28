
#include "tasks/OledMenu.h"
extern sSystemState SystemState;

void StartOledMenuTask(void *argument){
	// Меню для переключения между меню настроек и стартовым дисплеем
	osDelay(200);
	SystemState.DisplayState.prevState = SystemState.DisplayState.state;
	SystemState.DisplayState.state = WAIT_COMMAND;
	for(;;){
		osDelay(50);
		if(buttonEn() == ON){
			drawMainMenu();
		}
		startDisplay();
		udpateDisplay();
	}
}





