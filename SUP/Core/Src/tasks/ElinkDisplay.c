#include "tasks/ElinkDisplay.h"
//EPD
#include "Display/e-ink/EPD/Display_EPD_W21_spi.h"
#include "Display/e-ink/EPD/Display_EPD_W21.h"
#include "Display/e-ink/pic/logo_v2.h"
#include "Display/e-ink/pic/butt_ok.h"
#include "Display/e-ink/pic/butt_stop.h"
#include "Display/e-ink/pic/logo_v1.h"
#include "Display/e-ink/pic/main_display.h"
//GUI
#include "Display/e-ink/GUI/GUI_Paint.h"
#include "Display/e-ink/Fonts/fonts.h"
#include "System/SystemState.h"

extern sSystem mySystem;

unsigned char BlackImage[5000];//Define canvas space
void StartElinkDisplay(void *argument){
	osDelay(1000);
	uint8_t est = 0;
	EPD_init(); //EPD init
	EPD_full_display(gImage_main_display, gImage_main_display, 0);//EPD_picture1
	EPD_sleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!

	// центр32 x - 104 y
	uint16_t test = 0;
	uint16_t prev_test = 0;

	prev_test = test;
	test = mySystem.magEnc.currentPosition;
	display_number(48, 104, test, prev_test,0);
	for(;;){
		prev_test = test;
		test = mySystem.magEnc.currentPosition;
		display_number(48, 104, test, prev_test,1);
		osDelay(100);

	}
}
