/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file i2c_scan.c
 * \brief Implementation of I2C bus scanning functionality.
 */
#include "i2c_scan.h"

#include "pico/multicore.h"
#include <stdio.h>

void i2c_scan(i2c_inst_t *i2c) {
  (void)printf("I2C%u Bus Scan\n"
               "   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n",
               i2c_get_index(i2c));
  for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
    if (addr % 0x10U == 0) {
      (void)printf("%02x ", addr);
    }
    int rc;
    /*
     * Address 0x00-0x07 and 0x78-0x7F are reserved for general call,
     * start byte, and high-speed mode and hence are not valid for
     * normal devices, so skip them in the scan to avoid false
     * positives.
     */
    if ((addr & 0x78U) == 0x00U || (addr & 0x78U) == 0x78U) {
      rc = PICO_ERROR_GENERIC;
    } else {
      uint8_t data[1];
      /*
       * Use a short timeout to avoid long delays if there is no device
       * at this address. The i2c_read_timeout_us function will return
       * PICO_ERROR_TIMEOUT if the device does not respond within the
       * specified timeout period, which allows the scan to continue
       * quickly to the next address without hanging.
       *
       * Note that some devices may not respond to a read request if
       * they require a specific command or register address to be sent
       * first, so this scan may not detect all devices on the bus.
       * However, it will provide a quick way to identify which
       * addresses are responding and which are not, which can be useful
       * for troubleshooting and device discovery on the I2C bus.
       */
      rc = i2c_read_timeout_us(i2c, addr, data, sizeof(data), false, 100000U);
    }
    (void)printf("%s%s",
                 /*
                  * Print the result of the I2C scan for this address.
                  * "." indicates a reserved address, or an address that is not responding.
                  * "!" indicates a timeout error.
                  * "@" indicates a valid device.
                  */
                 rc < 0 ? (rc == PICO_ERROR_GENERIC ? "." : "!") : "@",
                 /*
                  * Print a newline character at the end of each row of the I2C scan.
                  * If the current core is 0, print two spaces between addresses.
                  * Otherwise, print two underscores between addresses.
                  */
                 addr % 0x10U == 0xfU ? "\n" : (get_core_num() == 0 ? "  " : "__"));
  }
}
