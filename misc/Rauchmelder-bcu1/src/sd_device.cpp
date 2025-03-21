/*  Original written for LPC922:
 *  Copyright (c) 2015-2017 Stefan Haller
 *  Copyright (c) 2013 Stefan Taferner <stefan.taferner@gmx.at>
 *
 *  Modified for LPC1115 ARM processor:
 *  Copyright (c) 2017 Oliver Stefan <o.stefan252@googlemail.com>
 *  Copyright (c) 2020 Stefan Haller
 *
 *  Refactoring and bug fixes:
 *  Copyright (c) 2023 Darthyson <darth@maptrack.de>
 *  Copyright (c) 2023 Thomas Dallmair <dev@thomas-dallmair.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <type_traits>
#include <sblib/eib/datapoint_types.h>
#include <sblib/digital_pin.h>
#include <sblib/utils.h>

#include "sd_com.h"
#include "sd_alarm.h"
#include "sd_config.h"
#include "sd_device.h"
#include "sd_errorcode.h"
#include "sd_group_objects.h"
#include "sd_shared_enums.h"

SmokeDetectorDevice::SmokeDetectorDevice(const SmokeDetectorConfig *config, const SmokeDetectorGroupObjects *groupObjects, SmokeDetectorAlarm *alarm, SmokeDetectorErrorCode *errorCode)
    : config(config),
      groupObjects(groupObjects),
      alarm(alarm),
      errorCode(errorCode),
      com(new SmokeDetectorCom(this))
{
    setSupplyVoltage(false);
    pinMode(DevicePowered.pinLed(), OUTPUT);
    digitalWrite(DevicePowered.pinLed(), false);
    pinInterruptMode(DevicePowered.pin(), INTERRUPT_EDGE_BOTH | INTERRUPT_ENABLED);
    pinMode(DevicePowered.pin(), INPUT | PULL_DOWN | HYSTERESIS); // smoke detector base plate state, pulldown configured, Pin is connected to 3.3V VCC of the RM

    state = DeviceState::initialized;
    timeout.stop();
}

void SmokeDetectorDevice::setAlarmState(RmAlarmState newState)
{
    com->setAlarmState(newState);
}

/**
 * Send command @ref cmd to smoke detector.\n
 * Receiving and processing the response from the smoke detector is done in @ref receivedMessage().
 *
 * @param cmd - Command to send
 * @return True if command was sent, otherwise false.
 */
bool SmokeDetectorDevice::sendCommand(DeviceCommand cmd)
{
    return com->sendCommand(deviceCommandToRmCommandByte(cmd));
}

void SmokeDetectorDevice::loopReceiveBytes()
{
    com->receiveBytes();
}

void SmokeDetectorDevice::loopCheckState()
{
    switch (state)
    {
        case DeviceState::initialized:
            // make sure to discharge the 12V capacitor for at least 500ms
            setSupplyVoltage(false);
            state = DeviceState::drainCapacitor;
            timeout.start(SupplyVoltageOffDelayMs);
            break;

        case DeviceState::drainCapacitor:
            if (timeout.expired())
            {
                state = DeviceState::devicePowered;
            }
            break;

        case DeviceState::devicePowered:
            checkDevicePowered();
            break;

        case DeviceState::powerUpDelay:
            if (timeout.expired())
            {
                setSupplyVoltage(true);
                state = DeviceState::fillCapacitor;
                timeout.start(SupplyVoltageOnDelayMs);
            }
            break;

        case DeviceState::fillCapacitor:
            if (timeout.expired())
            {
                // Enable communication on the smoke detector
                pinMode(CommunicationEnable.pin(), OUTPUT);
                digitalWrite(CommunicationEnable.pin(), CommunicationEnable.on());
                com->initSerialCom();
                state = DeviceState::ready;
            }
            break;

        case DeviceState::ready:
            com->loopCheckTimeouts();
            break;

        default:
            failHardInDebug();
            break;
    }
}

bool SmokeDetectorDevice::isReady() const
{
    return state == DeviceState::ready;
}

/**
 * For description see declaration in file @ref smoke_detector_com.h
 */
void SmokeDetectorDevice::receivedMessage(uint8_t *bytes, uint8_t len)
{
    uint8_t msgType;

    errorCode->communicationTimeout(false);

    msgType = bytes[0];
    const auto rmCommandByteStatus = static_cast<std::underlying_type_t<RmCommandByte>>(RmCommandByte::status);
    if (msgType == (rmCommandByteStatus | 0x80))
    {
        msgType = (rmCommandByteStatus | 0xc0); // "cast" automatic status message to "normal" status message
    }

    if ((msgType & 0xf0) != 0xc0) // check for valid response byte
    {
                // receiving an "echo" of our own command (e.g. <STX>0262<ETX>) can bring us here.
        return; // learned this by accidently touching the ARM's tx/rx pins
    }

    auto rmCommandByte = static_cast<RmCommandByte>(msgType & 0x0f);

    // Received length must match expected length.
    auto expectedLength = (rmCommandByte == RmCommandByte::numberAlarms_2) ? 3 : 5;
    if (len != expectedLength)
    {
        failHardInDebug();
        return;
    }

    ++bytes;
    switch (rmCommandByte)
    {
        case RmCommandByte::serialNumber:
            readSerialNumberMessage(bytes);
            break;

        case RmCommandByte::operatingTime:
            readOperatingTimeMessage(bytes);
            break;

        case RmCommandByte::smokeboxData:
            readSmokeboxDataMessage(bytes);
            break;

        case RmCommandByte::batteryTemperatureData:
            readBatteryAndTemperatureMessage(bytes);
            break;

        case RmCommandByte::numberAlarms_1:
            readNumberAlarms1Message(bytes);
            break;

        case RmCommandByte::numberAlarms_2:
            readNumberAlarms2Message(bytes);
            break;

        case RmCommandByte::status:
            readStatusMessage(bytes);
            break;

        default:
            failHardInDebug(); // found a new command/response => time to implement it
            break;
    }
}

/**
 * For description see declaration in file @ref smoke_detector_com.h
 */
void SmokeDetectorDevice::timedOut(std::optional<RmCommandByte> command)
{
    errorCode->communicationTimeout(true);

    if (!command.has_value())
    {
        return;
    }

    const uint32_t MinusOne = static_cast<uint32_t>(-1);

    switch (command.value())
    {
        case RmCommandByte::serialNumber:
            groupObjects->setValue(GroupObject::serialNumber, 0);
            break;

        case RmCommandByte::operatingTime:
            groupObjects->setValue(GroupObject::operatingTime, 0);
            break;

        case RmCommandByte::smokeboxData:
            groupObjects->setValue(GroupObject::smokeboxValue, MinusOne);
            groupObjects->setValue(GroupObject::countSmokeAlarm, MinusOne);
            groupObjects->setValue(GroupObject::smokeboxPollution, MinusOne);
            break;

        case RmCommandByte::batteryTemperatureData:
            groupObjects->setValue(GroupObject::batteryVoltage, 0);
            groupObjects->setValue(GroupObject::temperature, 0);
            break;

        case RmCommandByte::numberAlarms_1:
            groupObjects->setValue(GroupObject::countTemperatureAlarm, MinusOne);
            groupObjects->setValue(GroupObject::countTestAlarm, MinusOne);
            groupObjects->setValue(GroupObject::countAlarmWire, MinusOne);
            groupObjects->setValue(GroupObject::countAlarmBus, MinusOne);
            break;

        case RmCommandByte::numberAlarms_2:
            groupObjects->setValue(GroupObject::countTestAlarmWire, MinusOne);
            groupObjects->setValue(GroupObject::countTestAlarmBus, MinusOne);
            break;

        default:
            break;
    }
}

/**
 * Enable/disable the 12V supply voltage
 *
 * @param enable     Set true to enable supply voltage, false to disable it
 */
void SmokeDetectorDevice::setSupplyVoltage(bool enable)
{
    pinMode(SupportVoltage.pinLed(), OUTPUT);
    digitalWrite(SupportVoltage.pinLed(), enable); // disable/enable led first to save/drain some juice

    // Running pinMode the first time, sets it by default to low
    // which will enable the support voltage and charge the capacitor.
    // So please put nothing in between pinMode and digitalWrite.
    pinMode(SupportVoltage.pin(), OUTPUT);
    if (enable)
    {
        digitalWrite(SupportVoltage.pin(), SupportVoltage.on());
    }
    else
    {
        digitalWrite(SupportVoltage.pin(), SupportVoltage.off());
    }

    errorCode->supplyVoltageDisabled(!enable);
}

/**
 * Checks if the smoke detector is on the base plate and switches the supply voltage on
 */
void SmokeDetectorDevice::checkDevicePowered()
{
    if (!isCoverPlateAttached())
    {
        return;
    }
    // After it has been attached to the cover plate, allow some time for power-up
    state = DeviceState::powerUpDelay;
    timeout.start(DevicePowerUpDelayMs);
}

bool SmokeDetectorDevice::isCoverPlateAttached()
{
    uint32_t pin = digitalPinToBitMask(DevicePowered.pin());
    if (LPC_GPIO0->MIS & pin) // Check if the interrupt was caused by DevicePowered pin (PIO0_11)
    {
        LPC_GPIO0->IC = pin; // Clear the interrupt flag for DevicePowered pin
    }
    auto isAttached = (digitalRead(DevicePowered.pin()) == DevicePowered.on());
    digitalWrite(DevicePowered.pinLed(), isAttached);
    errorCode->coverPlateAttached(isAttached);
    return isAttached;
}

void SmokeDetectorDevice::readSerialNumberMessage(const uint8_t *bytes) const
{
    // <STX>C4214710F31F<ETX>
    groupObjects->setValue(GroupObject::serialNumber, readUInt32(bytes));
}

void SmokeDetectorDevice::readOperatingTimeMessage(const uint8_t *bytes) const
{
    // <STX>C9000047E31F<ETX>
    groupObjects->setValue(GroupObject::operatingTime, readOperatingTime(bytes));
}

void SmokeDetectorDevice::readSmokeboxDataMessage(const uint8_t *bytes) const
{
    // <STX>CB0065000111<ETX>
    groupObjects->setValue(GroupObject::smokeboxValue, floatToDpt9(readSmokeBoxValue(bytes)));
    groupObjects->setValue(GroupObject::countSmokeAlarm, bytes[2]);
    groupObjects->setValue(GroupObject::smokeboxPollution, bytes[3]);
}

void SmokeDetectorDevice::readBatteryAndTemperatureMessage(const uint8_t *bytes) const
{
    // <STX>CC000155551B<ETX>
    groupObjects->setValue(GroupObject::batteryVoltage, floatToDpt9(readVoltage(bytes)));
    groupObjects->setValue(GroupObject::temperature, floatToDpt9(readTemperature(bytes + 2)));
}

void SmokeDetectorDevice::readNumberAlarms1Message(const uint8_t *bytes) const
{
    // <STX>CD0000000007<ETX>
    groupObjects->setValue(GroupObject::countTemperatureAlarm, bytes[0]);
    groupObjects->setValue(GroupObject::countTestAlarm, bytes[1]);
    groupObjects->setValue(GroupObject::countAlarmWire, bytes[2]);
    groupObjects->setValue(GroupObject::countAlarmBus, bytes[3]);
}

void SmokeDetectorDevice::readNumberAlarms2Message(const uint8_t *bytes) const
{
    // <STX>CE000048<ETX>
    groupObjects->setValue(GroupObject::countTestAlarmWire, bytes[0]);
    groupObjects->setValue(GroupObject::countTestAlarmBus, bytes[1]);
}

void SmokeDetectorDevice::readStatusMessage(const uint8_t *bytes) const
{
    // <STX>C220000000F7<ETX>
    auto subType = bytes[0];
    auto status = bytes[1];

    // Local Alarm: Temperature alarm | smoke alarm | wired alarm
    auto hasAlarmLocal = static_cast<bool>((subType & 0x10) | (status & (0x04 | 0x08)));

    // Local test alarm: Button test alarm | wired test alarm
    auto hasTestAlarmLocal = static_cast<bool>(status & (0x20 | 0x40));

    auto hasAlarmFromBus = static_cast<bool>(status & 0x10);
    auto hasTestAlarmFromBus = static_cast<bool>(status & 0x80);

    alarm->deviceStatusUpdate(hasAlarmLocal, hasTestAlarmLocal, hasAlarmFromBus, hasTestAlarmFromBus);

    // Device button pressed
    if (subType & 0x08)
    {
        alarm->deviceButtonPressed();
    }

    // Battery low
    auto batteryLow = ((status & 0x01) == 1);
    errorCode->batteryLow(batteryLow);

    auto temperatureSensor_1_fault = false;
    auto temperatureSensor_2_fault = false;
    auto otherDeviceFault = false;

    // General smoke detector fault is indicated in 1. byte bit 1
    if (subType & 0x02)
    {
        // Details are in 4. byte
        auto faultByte = bytes[3];
        temperatureSensor_1_fault = static_cast<bool>(faultByte & 0x04); // sensor 1 state in bit 2
        temperatureSensor_2_fault = static_cast<bool>(faultByte & 0x10); // sensor 2 state in bit 4
        otherDeviceFault = static_cast<bool>(faultByte & (0x02 | 0x08)); // unknown fault states in bits 1 and 3
    }

    errorCode->temperature_1_fault(temperatureSensor_1_fault);
    errorCode->temperature_2_fault(temperatureSensor_2_fault);
    errorCode->otherDeviceFault(otherDeviceFault);

    // Smoke box fault
    auto smokeBoxFault = static_cast<bool>(subType & 0x01);
    errorCode->smokeBoxFault(smokeBoxFault);
}

uint32_t SmokeDetectorDevice::readOperatingTime(const uint8_t *bytes) const
{
    // Raw value is in quarter seconds, so divide by 4.
    auto seconds = readUInt32(bytes) >> 2;
    auto time = config->infoSendOperationTimeInHours() ? seconds / 3600 : seconds;
    return time;
}

int32_t SmokeDetectorDevice::readSmokeBoxValue(const uint8_t *bytes) const
{
    auto rawValue = static_cast<int32_t>(readUInt16(bytes));
    // Conversion: rawValue * 3.3V / 1024 * 100 [for DPT9]
    auto smokeBoxValue = (rawValue * 330) >> 10;
    return smokeBoxValue;
}

int32_t SmokeDetectorDevice::readVoltage(const uint8_t *bytes) const
{
    if ((bytes[0] == 0) && (bytes[1] == 1))
    {
        return BatteryVoltageInvalid;
    }

    auto rawVoltage = static_cast<int32_t>(readUInt16(bytes));
    // Conversion: rawVoltage * 5.7 * 3.3V / 1024 * 1000mV/V * 100 [for DPT9]
    auto voltage = (rawVoltage * 9184) / 5;
    return voltage;
}

int32_t SmokeDetectorDevice::readTemperature(const uint8_t *bytes) const
{
    // Conversion per temp sensor: (answer[x] * 0.5°C - 20°C) * 100 [for DPT9]
    auto temperature = static_cast<int32_t>(bytes[0]) + bytes[1];
    temperature *= 25; // Added two temperatures, so only half the multiplier
    temperature -= 2000;
    temperature += config->temperatureOffsetInTenthDegrees() * 10;  // Temperature offset
    return temperature;
}

void SmokeDetectorDevice::failHardInDebug()
{
#ifdef DEBUG
    fatalError();
#endif
}

RmCommandByte SmokeDetectorDevice::deviceCommandToRmCommandByte(DeviceCommand command)
{
    switch (command)
    {
        case DeviceCommand::serialNumber:
            return RmCommandByte::serialNumber;

        case DeviceCommand::status:
            return RmCommandByte::status;

        case DeviceCommand::batteryAndTemperature:
            return RmCommandByte::batteryTemperatureData;

        case DeviceCommand::operatingTime:
            return RmCommandByte::operatingTime;

        case DeviceCommand::smokeboxData:
            return RmCommandByte::smokeboxData;

        case DeviceCommand::numberAlarms1:
            return RmCommandByte::numberAlarms_1;

        case DeviceCommand::numberAlarms2:
            return RmCommandByte::numberAlarms_2;

        default:
            failHardInDebug();
            return RmCommandByte::status;
    }
}

/**
 * Read a 32-bit unsigned number from the smoke detector.
 *
 * @param bytes First byte of the smoke detector response.
 * @return The value in processor native format.
 */
uint32_t SmokeDetectorDevice::readUInt32(const uint8_t *bytes)
{
    return static_cast<uint32_t>((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
}

/**
 * Read a 16-bit unsigned number from the smoke detector.
 *
 * @param bytes First byte of the smoke detector response.
 * @return The value in processor native format.
 */
uint16_t SmokeDetectorDevice::readUInt16(const uint8_t *bytes)
{
    return static_cast<uint16_t>((bytes[0] << 8) | bytes[1]);
}

void SmokeDetectorDevice::end()
{
    com->end();
    setSupplyVoltage(false);
    pinMode(CommunicationEnable.pin(), INPUT | OPEN_DRAIN);
    state = DeviceState::initialized;
}
