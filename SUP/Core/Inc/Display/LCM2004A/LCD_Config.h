
#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H 

#include "ExpanderGPIO/PCF8575.h"

#define _LCD_USE_FREERTOS 0
#define _LCD_USE_MENU_LIB 0

#define _LCD_COLS         16
#define _LCD_ROWS         2

#define _LCD_RS_PORT      PCF_PORT_0
#define _LCD_RS_PIN       PCF_PIN_1

#define _LCD_E_PORT       PCF_PORT_0
#define _LCD_E_PIN        PCF_PIN_3

#define _LCD_RW_PORT      PCF_PORT_0
#define _LCD_RW_PIN       PCF_PIN_2

#define _LCD_D0_PORT      PCF_PORT_0
#define _LCD_D0_PIN       PCF_PIN_4

#define _LCD_D1_PORT      PCF_PORT_0
#define _LCD_D1_PIN       PCF_PIN_5

#define _LCD_D2_PORT      PCF_PORT_0
#define _LCD_D2_PIN       PCF_PIN_6

#define _LCD_D3_PORT      PCF_PORT_0
#define _LCD_D3_PIN       PCF_PIN_7

#define _LCD_D4_PORT      PCF_PORT_1
#define _LCD_D4_PIN		  PCF_PIN_0

#define _LCD_D5_PORT      PCF_PORT_1
#define _LCD_D5_PIN       PCF_PIN_1

#define _LCD_D6_PORT      PCF_PORT_1
#define _LCD_D6_PIN       PCF_PIN_2

#define _LCD_D7_PORT      PCF_PORT_1
#define _LCD_D7_PIN       PCF_PIN_3

#endif

