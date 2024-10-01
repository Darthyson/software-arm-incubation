/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#ifndef APP_TOF_VL53L4CD_CONFIG_H_
#define APP_TOF_VL53L4CD_CONFIG_H_

#include <stdint.h>
#include <sblib/io_pin_names.h>

constexpr uint16_t MinimumSendThreshold_mm = 3;
constexpr uint32_t SendPeriodicInterval_ms = 1000;

#define MANUFACTURER 0xC9   //!< Elsner
#define DEVICETYPE   0x1013 //!< KNX SO250 basic
#define APPVERSION   0x10   //!< Version 1.0

/**
 * Timing budget in milliseconds for the ToF to measure (1-200ms)
 */
constexpr uint32_t TimingBudget_ms = 200;

/**
 * Inter-measurement time in milliseconds for the ToF to pause (0 or >= @ref TimingBudget_ms)
 */
constexpr uint32_t InterMeasurement_ms = 0;

/**
 * Range sigma (standard deviation) threshold in mm.
 */
constexpr uint32_t RangeSigmaThreshold_mm = 2;

/**
 * Signal threshold in kcps (kilo counts per seconde) for the ToF signal.
 */
constexpr uint32_t SignalThreshold_kcps = 2000;

// JP24 (debug Tx/Rx gpios)
constexpr uint32_t SerialTxPin = PIN_TX;
constexpr uint32_t SerialRxPin = PIN_RX;
/*
// JP17 (ISP Tx/Rx gpios)
constexpr uint32_t SerialTxPin = PIO1_7;
constexpr uint32_t SerialRxPin = PIO1_6;
*/
constexpr uint32_t SerialBaudrate = 115200;

#if defined FUSED_IO and not defined __LPC11XX_FUSED_IO__
#   define __LPC11XX_FUSED_IO__
#endif

#endif /* APP_TOF_VL53L4CD_CONFIG_H_ */
