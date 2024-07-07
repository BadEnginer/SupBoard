#ifndef INC_DAC_OUT_DAC_H_
#define INC_DAC_OUT_DAC_H_
#include "main.h"
#include "DAC_out/MCP4725.h"

// 1.21v напржяние остановки
// 2.2v полный назад
// 0.4 полный вперёд
extern osMutexId_t BlockI2CHandle;

#define MAX_SPEED 10
#define MIN_SPEED (-10)
#define FULL_FORWARD 0.3F
#define FULL_BACK 2.1F
#define STOP_ENGINE 1.20F
#define KOEFF_INLINE_DAC 37 // Убераем рассолгасовние между ацп и дак
#define KOEFF_K_SLOW 0.02

extern osMutexId_t BlockI2CHandle;
extern I2C_HandleTypeDef hi2c1;

#define STOP_MOTOR 1600
#define MAX_FORVARD_SPEED 3000
#define MAX_BACK_SPEED 200
#define KOEFF_K_SLOW 0.1

float expFiltrDAC(float newVal, float k);
//void initDAC(MCP4725* _MCP4725);
void SetSpeed(int16_t speed);
void SlowStop();
void FastStop();

#endif /* INC_DAC_OUT_DAC_H_ */
