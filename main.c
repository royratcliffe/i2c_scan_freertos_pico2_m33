#include "pico/stdlib.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

int main() {
  stdio_init_all();

  /*
   * Start the FreeRTOS scheduler. This will start the atomic work task
   * and allow it to run the scheduled work function. The main function
   * will not continue past this point, as the scheduler will take over
   * and run the tasks. In a real application, you might want to add
   * additional tasks or functionality before starting the scheduler.
   * For this example, we simply start the scheduler after scheduling
   * the work function.
   */
  vTaskStartScheduler();

  /*
   * If the scheduler returns, it failed to start (for example, due to
   * insufficient heap to create the idle or timer task). Do not allow
   * main to return on bare-metal; report the failure and halt here.
   */
  (void)printf("ERR: vTaskStartScheduler() returned; scheduler failed to start.\n");
  for (;;) {
    tight_loop_contents();
  }
  return 0;
}
