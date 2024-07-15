#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include <tasks/SensControl.h>
#include "cmsis_os.h"
//#include "tasks/OledMenu.h"
//#include "tasks/LedControl.h"
#include "tasks/ElinkDisplay.h"
#include "tasks/MotorControl.h"

/**
 * @fn void initUserTasks()
 * @brief Инициализация всех пользовательских потоков
 */
void initUserTasks();


#endif /* INC_TASKS_H_ */
