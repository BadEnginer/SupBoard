#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"
#include "tasks/OledMenu.h"
#include "tasks/SensOut.h"
#include "tasks/OledOut.h"
/**
 * @fn void initUserTasks()
 * @brief Инициализация всех пользовательских потоков
 */
void initUserTasks();


#endif /* INC_TASKS_H_ */
