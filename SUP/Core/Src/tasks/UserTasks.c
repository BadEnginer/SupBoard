#include "tasks/UserTasks.h"
#include "main.h"

osThreadId_t ElinkDisplayTaskHandle;
const osThreadAttr_t ElinkDisplayTask_attributes = {
  .name = "ElinkDisplay",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};


osThreadId_t SensOutTaskHandle;
const osThreadAttr_t SensOutTask_attributes = {
  .name = "SensOutTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t MotorControlTaskHandle;
const osThreadAttr_t MotorControlTask_attributes = {
  .name = "MotorControlTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void initUserTasks(){
	ElinkDisplayTaskHandle  = osThreadNew(StartElinkDisplay,   	 NULL, &ElinkDisplayTask_attributes); // Задача с отображением данных на elink
	MotorControlTaskHandle  = osThreadNew(StartMotorControlTask, NULL, &MotorControlTask_attributes); // Задача управления мотором
	SensOutTaskHandle    	= osThreadNew(StartSensOutTask,    	 NULL, &SensOutTask_attributes); // Задача анализа кнопок и магнитометра и реакций
};



