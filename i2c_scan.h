/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file i2c_scan.h
 * \brief Header file for I2C bus scanning functionality.
 */
#ifndef I2C_SCAN_H
#define I2C_SCAN_H

#include "hardware/i2c.h"

/*!
 * \brief Scan the I2C bus for devices and print their addresses.
 * \details Performs a scan of the I2C bus by attempting to read a byte
 * from each possible 7-bit address (0x00 to 0x7F). For each address, it
 * checks if a device acknowledges the read request. If a device
 * acknowledges, the function prints '@'; otherwise, a dot '.'.
 * The output is formatted in a way that shows the addresses in a grid,
 * making it easier to identify which addresses are occupied by
 * devices.
 * \param i2c A pointer to the I2C instance to be scanned (e.g., i2c0 or i2c1).
 * \note Useful for debugging and identifying devices connected to the
 * I2C bus. The function assumes that the I2C bus is already properly
 * initialised and that the devices are powered on and connected
 * correctly.
 * \note Performs a blocking scan, which may take some time to complete,
 * especially if there are many addresses to check or if devices are
 * slow to respond.
 */
void i2c_scan(i2c_inst_t *i2c);

#endif /* I2C_SCAN_H */
