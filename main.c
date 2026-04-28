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

  /*
   * Initialise I2C1 with a baud rate of 400 kHz and configure the GPIO
   * pins. Launch a FreeRTOS task to perform the I2C scanning. The task
   * will run indefinitely, scanning the I2C bus every second and
   * printing the results.
   */
  i2c_init(i2c1, 400 * 1000);
  gpio_set_function(I2C1_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C1_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C1_SDA);
  gpio_pull_up(I2C1_SCL);
  BaseType_t xResult = xTaskCreate(prvI2CScannerTask, "scanI2C1", configMINIMAL_STACK_SIZE, (void *)i2c1, tskIDLE_PRIORITY, NULL);
  configASSERT(xResult != pdFAIL);

  /*
   * Start the FreeRTOS scheduler. This hands control to the scheduler,
   * which will begin running the tasks created above, including the
   * I2C scanner task. The main function should not continue past this
   * point unless the scheduler fails to start.
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
