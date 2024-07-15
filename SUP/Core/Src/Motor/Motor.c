#include "Motor/Motor.h"
#include "System/SystemState.h"
extern sSystem mySystem;

ExpFilterState_t smoothSpeed;

int8_t SetSpeed(sSystem* s){ // делаем рост скорости плавным
	int8_t Speed = s->motor.speed;
	if(Speed > MAX_SPEED) // установка пределов скорости
		Speed = MAX_SPEED;
	if(Speed < MIN_SPEED)
		Speed = MIN_SPEED;
	// Расчёт напряжения эквивалентной скорости
	s->motor.normalazy_voltage = (STEP_VOLTAGE * Speed);
	s->motor.smooth_voltage = ZERO_SPEED_VOLTAGE + expFilter(s->motor.normalazy_voltage, KOEFF_K_SLOW, &smoothSpeed);
	return setDACVoltage(s->motor.smooth_voltage, &(s->dac));
}

