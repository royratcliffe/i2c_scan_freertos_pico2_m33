#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "i2c_scan.h"

#define I2C1_SDA 6
#define I2C1_SCL 7

static void prvI2CScannerTask(void *pvParameters) {
  i2c_inst_t *i2c = (i2c_inst_t *)pvParameters;
  for (;;) {
    i2c_scan(i2c);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

int main() {
  stdio_init_all();

  i2c_init(i2c1, 400 * 1000);
  gpio_set_function(I2C1_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C1_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C1_SDA);
  gpio_pull_up(I2C1_SCL);
  BaseType_t xResult = xTaskCreate(prvI2CScannerTask, "scanI2C1", configMINIMAL_STACK_SIZE, (void *)i2c1, tskIDLE_PRIORITY, NULL);
  configASSERT(xResult != pdFAIL);

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
