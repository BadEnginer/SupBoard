
#include "tasks/OledMenu.h"
extern sSystemState SystemState;
extern osMutexId_t BlockI2CHandle;

void StartOledMenuTask(void *argument){
	osStatus_t statusMutex;
	char num[3] = {0};
	uint8_t name[] = "Hello JetPro!";
	statusMutex = osMutexAcquire(BlockI2CHandle, 1000);
	if(statusMutex == osOK){
		lcdInit(&hi2c1, (uint8_t)0x27, (uint8_t)4, (uint8_t)20);
		osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
	}
	// Меню для переключения между меню настроек и стартовым дисплеем
	osDelay(200);
	SystemState.DisplayState.prevState = SystemState.DisplayState.state;
	SystemState.DisplayState.state = WAIT_COMMAND;
	for(;;){
		for(uint8_t i = 0; i < 13; i++){
			statusMutex = osMutexAcquire(BlockI2CHandle, 1000);
			if(statusMutex == osOK){
				lcdDisplayClear() ;
				lcdSetCursorPosition(0,0);
				lcdPrintStr(name, i);
				osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
				osDelay(180);
			}
		}
		for(uint8_t i = 0; i < 13; i++){
			statusMutex = osMutexAcquire(BlockI2CHandle, 1000);
			if(statusMutex == osOK){
				lcdDisplayClear() ;
				lcdSetCursorPosition(80-i,1);
				lcdPrintStr(name, 12);
				osMutexRelease(BlockI2CHandle);// Освобождение мьютекса
				osDelay(300);
			}
		}

		osDelay(50);
		if(buttonEn() == ON){
			drawMainMenu();
		}
		startDisplay();
		udpateDisplay();
	}
}





