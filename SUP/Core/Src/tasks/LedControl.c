
#include "tasks/LedControl.h"

void StartLedControlTask(void *argument){
	  ARGB_Init();  // Initialization
	  ARGB_Clear();
	  while (ARGB_Show() != ARGB_OK);
	  ARGB_SetBrightness(80);
	for(;;){
		osDelay(100);
		}
}
