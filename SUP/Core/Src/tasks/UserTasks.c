#include "tasks/UserTasks.h"
#include "main.h"

osThreadId_t OledMenuTaskHandle;
const osThreadAttr_t OledMenuTask_attributes = {
  .name = "OledMenuTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t LedControlTaskHandle;
const osThreadAttr_t LedControlTask_attributes = {
  .name = "LedControlTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t SensOutTaskHandle;
const osThreadAttr_t SensOutTask_attributes = {
  .name = "SensOutTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void initUserTasks(){
	OledMenuTaskHandle   = osThreadNew(StartOledMenuTask,   NULL, &OledMenuTask_attributes);
	LedControlTaskHandle = osThreadNew(StartLedControlTask, NULL, &LedControlTask_attributes);
	SensOutTaskHandle    = osThreadNew(StartSensOutTask,    NULL, &SensOutTask_attributes);
};



