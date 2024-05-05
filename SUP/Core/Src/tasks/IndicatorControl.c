
#include "tasks/OledMenu.h"
extern sSystemState SystemState;
extern osMutexId_t BlockI2CHandle;

void StartOledMenuTask(void *argument){
	osStatus_t statusMutex;
	statusMutex = osMutexAcquire(BlockI2CHandle, 1000);
	if(statusMutex == osOK){
		lcdInit(&hi2c1, (uint8_t)0x27, (uint8_t)4, (uint8_t)20);
	  	lcdSetCursorPosition(0, 0);
		lcdPrintStr((uint8_t*)"123ABC", 6);
		osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
	}
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





