
#include "tasks/OledMenu.h"

void StartOledMenuTask(void *argument){

	while(buttonEn() != ON){
		osDelay(50);
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
