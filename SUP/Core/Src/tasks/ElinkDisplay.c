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

unsigned char BlackImage[5000];//Define canvas space
void StartElinkDisplay(void *argument){
	osDelay(1000);

	EPD_init(); //EPD init
	EPD_full_display(gImage_main_display, gImage_main_display, 0);//EPD_picture1
	EPD_sleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!

		uint8_t x = 24, y = 16;

		uint8_t x_start, x_end;
		uint8_t y_start, y_end;

		uint8_t x0_start = 64, x0_end = x + x0_start;
		uint8_t y0_start = 72, y0_end = y + y0_start;

		uint8_t x1_start = 64,  x1_end = x + x1_start;
		uint8_t y1_start = 96, y1_end = y + y1_start;

		uint8_t x2_start = 64, x2_end = x + x2_start;
		uint8_t y2_start = 120, y2_end = y + y2_start;

		display_number(x0_start, x0_end, y0_start, y0_end, 0, 0, 0);
		display_number(x1_start, x1_end, y1_start, y1_end, 0, 0, 0);
		display_number(x2_start, x2_end, y2_start, y2_end, 0, 0, 0);

		x_start = x0_start; x_end = x0_end;
		y_start = y0_start; y_end = y0_end;
	for(;;){

	}
}
