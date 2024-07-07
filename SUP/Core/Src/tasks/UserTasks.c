#include "tasks/UserTasks.h"
#include "main.h"

osThreadId_t OledMenuTaskHandle;
const osThreadAttr_t OledMenuTask_attributes = {
  .name = "OledMenuTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t ElinkMenuTaskHandle;
const osThreadAttr_t ElinkMenuTask_attributes = {
  .name = "ElinkMenuTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t LedControlTaskHandle;
const osThreadAttr_t LedControlTask_attributes = {
  .name = "LedControlTask",
  .stack_size = 256 * 10,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t ReadDataTaskHandle;
const osThreadAttr_t ReadDataTask_attributes = {
  .name = "ReadDataTask",
  .stack_size = 256 * 6,
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
	//OledMenuTaskHandle   	= osThreadNew(StartOledMenuTask,   	 NULL, &OledMenuTask_attributes); // Задача с отображением данных на OLED
	//LedControlTaskHandle 	= osThreadNew(StartLedControlTask,   NULL, &LedControlTask_attributes); // Задача управления умными светодиодами
	MotorControlTaskHandle  = osThreadNew(StartMotorControlTask, NULL, &MotorControlTask_attributes); // Задача управления мотором
	//ReadDataTaskHandle  	= osThreadNew(StartReadDataTask,   	 NULL, &ReadDataTask_attributes); // Задача чтения ацп и датчика холла
	SensOutTaskHandle    	= osThreadNew(StartSensOutTask,    	 NULL, &SensOutTask_attributes); // Задача анализа кнопок и магнитометра и реакций
};



