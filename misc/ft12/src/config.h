/*
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 3 as
 published by the Free Software Foundation.
 ---------------------------------------------------------------------------*/

#ifndef FT12_CONFIG_H_
#define FT12_CONFIG_H_


#define LED_SERIAL_RX               PIO0_6 //!< Serial-Rx LED Pin
#define LED_KNX_RX                  PIO0_7 //!< KNX-Rx LED Pin
#define LED_ON                      false  //!< Led is turned on by setting the IO-pin to low/ground
#define LED_OFF                     true   //!< Led is turned off by setting the IO-pin to high/+3V3
#define LED_TEST_MS                 250    //!< Time in milliseconds every LED should light up on startup test

#define PIN_FT_SERIAL_TX            PIN_TX //!< Serial-Tx pin
#define PIN_FT_SERIAL_RX            PIN_RX //!< Serial-Rx pin

#define LED_KNX_RX_BLINK_TIME_MS    100   //!< Receiving KNX packets blinking timeout in milliseconds

#define FT_OWN_KNX_ADDRESS          0x11fe //!< Our own physical KNX address: 1.1.254
#define FT12_BAUD_RATE              SERIAL_BAUD_RATE_19200 //!< Ft12 baudrate

/// Use the Software UART only for debug logging! Settings are 9600 8N1!
/// Keep logging messages as short as possible to avoid interference with the KNX bus communication and
/// hardware UART.
#define SOFT_UART_ENABLE            false  //!< Enable/disable software UART
#define PIN_FT_SOFT_UART_TX         PIO1_6 //!< Software UART Tx pin
#define PIN_FT_SOFT_UART_RX         PIO1_7 //!< Software UART Rx pin
#define FT12_SOFT_UART_BAUD_RATE    SoftUART::BaudRate::Baud9600 //!< Ft12 Software UART baudrate

#endif /* FT12_CONFIG_H_ */
