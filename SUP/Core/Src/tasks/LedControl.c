
#include "tasks/LedControl.h"

void StartLedControlTask(void *argument){
	  ARGB_Init();  // Initialization
	  ARGB_Clear();
	  while (ARGB_Show() != ARGB_OK);
	  ARGB_SetBrightness(100);
	  ARGB_Clear(); // Clear stirp
	  while (ARGB_Show() != ARGB_OK);
		ARGB_SetRGB(1, 0, 50, 0); // Set LED №1 with 255 Green
		ARGB_SetRGB(2, 50, 0, 0); // Set LED №2 with 255 Green
		ARGB_SetRGB(3, 0, 0, 50); // Set LED №3 with 255 Green
	  while (ARGB_Show() != ARGB_OK);

	for(;;){

		osDelay(25);
		}
}
