#include "tasks/UserTasks.h"
#include "main.h"

osThreadId_t OledMenuTaskHandle;
const osThreadAttr_t OledMenuTask_attributes = {
  .name = "OledMenuTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};

osThreadId_t OledOutTaskHandle;
const osThreadAttr_t OledOutTask_attributes = {
  .name = "OledOutTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};

osThreadId_t SensOutTaskHandle;
const osThreadAttr_t SensOutTask_attributes = {
  .name = "SensOutTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};

void initUserTasks(){
	OledMenuTaskHandle = osThreadNew(StartOledMenuTask, NULL, &OledMenuTask_attributes);
	//OledOutTaskHandle  = osThreadNew(StartOledOutTask,  NULL, &OledOutTask_attributes);
	SensOutTaskHandle  = osThreadNew(StartSensOutTask,  NULL, &SensOutTask_attributes);
};



