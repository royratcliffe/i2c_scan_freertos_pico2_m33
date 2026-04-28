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
  (void)printf("I2C%d Bus Scan\n"
               "   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n",
               I2C_NUM(i2c));
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
      rc = i2c_read_blocking(i2c, addr, data, sizeof(data), false);
    }
    (void)printf("%s%s", rc < 0 ? "." : "@", addr % 0x10U == 0xfU ? "\n" : get_core_num() == 0 ? "  " : "__");
  }
}
