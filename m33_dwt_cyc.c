/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file m33_dwt_cyc.c
 * \brief Implementation of DWT cycle counter functions for the Cortex-M33.
 */
#include "m33_dwt_cyc.h"

#include "hardware/structs/m33.h"
#include "pico/multicore.h"
#include "pico/runtime.h"

#include <stdatomic.h>

static atomic_uint_fast32_t cnt32[NUM_CORES] = {0UL, 0UL};
static atomic_uint_fast64_t cnt64[NUM_CORES] = {0ULL, 0ULL};

PICO_RUNTIME_INIT_FUNC_PER_CORE(m33_dwt_cyc_ena, "DWT_CYC");

void m33_dwt_cyc_ena(void) {
  /*
   * Enable the DWT cycle counter. This is necessary for the clock to function
   * correctly, as it relies on the DWT cycle counter to provide the tick count.
   * The DWT cycle counter is typically disabled by default, so it must be
   * explicitly enabled before using this clock.
   */
  m33_hw->demcr |= M33_DEMCR_TRCENA_BITS;          /* Enable the DWT (Data Watchpoint and Trace) unit. */
  m33_hw->dwt_cyccnt = 0;                          /* Reset the cycle counter to start counting from 0. */
  m33_hw->dwt_ctrl |= M33_DWT_CTRL_CYCCNTENA_BITS; /* Enable the cycle counter. */
}

uint32_t m33_dwt_cyc_cnt(void) {
  /*
   * Return the current value of the DWT cycle counter. This provides a
   * high-resolution tick count based on the number of CPU cycles.
   */
  return m33_hw->dwt_cyccnt;
}

uint64_t m33_dwt_cyc_cnt64(void) {
  /*
   * Ensure we're on the same core as the DWT counter, as it's per-core.
   * The DWT cycle counter is per-core, so keep separate counts for each
   * core. Use the get_core_num function to determine which core the
   * caller is running on and update the corresponding counts.
   */
  uint core = get_core_num();
  uint32_t tick = m33_dwt_cyc_cnt();
  /*
   * Update the 64-bit cycle counter for the current core. This ensures
   * that the 64-bit counter accurately reflects the total number of
   * cycles, even if the 32-bit counter wraps around. The 32-bit counter
   * will wrap around after 2^32 cycles, so keep track of how many times
   * it has wrapped around to maintain an accurate 64-bit count. The
   * total cycle count is calculated by adding the number of cycles
   * since the last update (tick - cnt32[core]) to the existing 64-bit
   * count (cnt64[core]). While updating the 64-bit count, store the
   * current tick value in cnt32[core] for the next update. This way,
   * the 64-bit counter will accurately track the total number of cycles
   * over time, even if the 32-bit counter wraps around multiple times.
   *
   * The use of atomic operations ensures that updates to the counters
   * are thread-safe, allowing for concurrent calls to this function
   * from multiple threads or cores without causing race conditions or
   * corrupting the counts. However, 64-bit updates are not atomic, so
   * the caller should ensure that this function is called atomically.
   */
  return cnt64[core] += tick - atomic_exchange(&cnt32[core], tick);
}
