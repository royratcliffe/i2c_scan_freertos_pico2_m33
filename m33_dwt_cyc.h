/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file m33_dwt_cyc.h
 * \brief Header file for DWT cycle counter functions for the Cortex-M33.
 */
#ifndef M33_DWT_CYC_H
#define M33_DWT_CYC_H

#include <stdint.h>

/*!
 * \brief Enables the DWT cycle counter.
 * \details Must be called before using the \c m33_dwt_cyc_cnt function.
 * The DWT cycle counter is a hardware feature of the Cortex-M series that
 * provides high-resolution timing based on CPU cycles, suitable for performance
 * measurements and precise timing applications.
 */
void m33_dwt_cyc_ena(void);

/*!
 * \brief Returns the current value of the DWT cycle counter.
 * \return The current value of the DWT cycle counter.
 */
uint32_t m33_dwt_cyc_cnt(void);

/*!
 * \brief Returns a 64-bit cycle count based on the 32-bit DWT cycle counter.
 * \details Extends the 32-bit DWT cycle counter to 64 bits by keeping
 * track of overflows. It should be called atomically to ensure that the
 * overflow count is updated correctly, especially in long-running
 * applications.
 * \return The current 64-bit cycle count.
 */
uint64_t m33_dwt_cyc_cnt64(void);

#endif /* M33_DWT_CYC_H */
