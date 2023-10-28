/*
 *  Original written for LPC922:
 *  Copyright (c) 2013 Stefan Taferner <stefan.taferner@gmx.at>
 *
 *  Modified for LPC1115 ARM processor:
 *  Copyright (c) 2017-2022 Oliver Stefan <o.stefan252@googlemail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */
#ifndef rm_com_h
#define rm_com_h

#include <stdint.h>

/**
 *  Befehlbyte an den Rauchmelder
 */
enum RmCommandByte : uint8_t
{
    serialNumber           = 0x04,
    operatingTime          = 0x09,
    smokeboxData           = 0x0B,
    batteryTemperatureData = 0x0C,
    numberAlarms_1         = 0x0D,
    numberAlarms_2         = 0x0E,
    status                 = 0x02
};

/**
 * Smoke detector alarm states
 */
enum RmAlarmState : uint8_t
{
    RM_NO_ALARM,    //!< normal state, no alarm and no test alarm
    RM_ALARM,       //!< alarm state
    RM_TEST_ALARM   //!< test alarm state
};

//
// Funktionen für die Kommunikation mit dem Rauchmelder
//

bool rm_set_alarm_state(RmAlarmState newState);

/**
 * Alle Bytes über die serielle Schnittstelle vom Rauchmelder empfangen.
 * Diese Funktion muss zyklisch aus main() aufgerufen werden um zu prüfen ob Bytes empfangen wurden.
 * Wenn die empfangene Nachricht komplett ist, dann wird @ref rm_process_msg()
 * zur Verarbeitung der Nachricht aufgerufen.
 */
void rm_recv_bytes();

/**
 * Die empfangene Nachricht vom Rauchmelder verarbeiten.
 *
 * Wird von @ref rm_recv_bytes() aufgerufen wenn eine Nachricht empfangen wurde.
 * Muss vom Verwender implementiert werden.
 *
 * @param bytes - die empfangene Nachricht, ohne STX, ETX, Prüfsumme.
 * @param len - die Anzahl der empfangenen Bytes
 */
void rm_process_msg(uint8_t *bytes, int8_t len);

/**
 * Serielle Kommunikation mit dem Rauchmelder initialisieren
 */
void rm_serial_init();

/**
 * Send command @ref cmd to the smoke detector.\n
 * Receiving and processing the response from the smoke detector is done in @ref rm_process_msg(), which must be defined by the application.
 *
 * @param cmd - @ref RmCommandByte to send.
 * @return True if command was sent, otherwise false.
 */
bool rm_send_cmd(RmCommandByte cmd);

/**
 * Check if we are currently receiving bytes from the smoke detector
 *
 * @return true if receiving, otherwise false
 */
bool isReceiving();


#endif /*rm_com_h*/
