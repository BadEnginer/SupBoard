#include "DAC_out/DAC.h"


uint16_t speed_arr_plus[] =  {0,250,300,330,380,460,550,650,770,900}; // step mV for lean speed
uint16_t speed_arr_minus[] = {0,180,250,310,380,460,550,650,770,900}; // step mV for lean speed
/*
void initDAC(MCP4725* myMCP4725);
void SetSpeed(MCP4725* myMCP4725, int16_t speed);
void SlowStop();
void FastStop();
*/
void initDAC(MCP4725* myMCP4725){
	*myMCP4725 = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A00, 3.20);
}

/*
void SetSpeed(MCP4725* myMCP4725, int16_t setSpeed){
	//MCP4725_setVoltage(myMCP4725, setSpeed, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
}


float expFiltrDAC(float newVal, float k) {
	  static float filVal = STOP_ENGINE;
	  filVal += (newVal - filVal) * k;
	  return filVal;
}

*/
