#ifndef _DISPLAY_EPD_W21_H_
#define _DISPLAY_EPD_W21_H_
#include <stdint.h>

#define EPD_WIDTH   200
#define EPD_HEIGHT  200
//EPD
void EPD_W21_Init(void);
void EPD_init(void);
void EPD_full_display(const unsigned char *old_data,const unsigned char *new_data,unsigned char mode);// mode0:Refresh picture1,mode1:Refresh picture2... ,mode2��3:Clear
void EPD_partial_display(unsigned int x_start,unsigned int x_end,unsigned int y_start,unsigned int y_end ,const unsigned char *old_data,const unsigned char *new_data,unsigned char mode); //partial display
void lut(void);
void lut1(void);
void EPD_sleep(void);
void EPD_refresh(void);
void lcd_chkstatus(void);
void EPD_full_display_Clear(void);
//Display canvas function
void EPD_Display(unsigned char *Image); 

void num_to_arr(int16_t num, uint8_t* num_digital, uint8_t* sign, uint8_t* data);
void display_number(uint16_t x_start,    uint16_t y_start,
                    int16_t current_num, int16_t previous_num,
					uint8_t mode);

extern const unsigned char gImage_16x24_num_0[];
extern const unsigned char gImage_16x24_num_1[];
extern const unsigned char gImage_16x24_num_2[];
extern const unsigned char gImage_16x24_num_3[];
extern const unsigned char gImage_16x24_num_4[];
extern const unsigned char gImage_16x24_num_5[];
extern const unsigned char gImage_16x24_num_6[];
extern const unsigned char gImage_16x24_num_7[];
extern const unsigned char gImage_16x24_num_8[];
extern const unsigned char gImage_16x24_num_9[];



#endif
/***********************************************************
						end file
***********************************************************/


