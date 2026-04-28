#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>

/*
 * Run time stack overflow checking is performed if
 * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook
 * function is called if a stack overflow is detected.
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  (void)printf("ERR: Stack overflow in task %p %s\n", xTask, pcTaskName);

  /*
   * Force an assert.
   */
  taskDISABLE_INTERRUPTS();
  configASSERT((volatile void *)NULL);
  for (;;)
    ;
}
