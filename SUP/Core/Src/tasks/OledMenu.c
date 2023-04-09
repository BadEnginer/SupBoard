
#include "tasks/OledMenu.h"

void StartOledMenuTask(void *argument){
	for(;;){
		if(buttonEn() == ON){
			drawMainMenu();
		}
	osDelay(100);
	}
}
