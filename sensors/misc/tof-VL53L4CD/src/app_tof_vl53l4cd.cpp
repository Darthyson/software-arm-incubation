/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#include "app_tof_vl53l4cd.h"
#include "config.h"
#include <sblib/timeout.h>
#include <sblib/serial.h>
#include <sblib/i2c/tof/VL53L4CD_api.h>
#include <sblib/i2c/tof/VL53L4CD_calibration.h>
#include <cmath> // for fabsf


MASK0701 bcu;

constexpr uint8_t COMOBJ_1_MESSWERT_IN_METER_AUSGANG = 3;
constexpr uint8_t COMOBJ_6_SENSORSTOERUNG_AUSGANG = 6;
constexpr uint8_t COMOBJ_56_SOFTWARE_VERSION_AUSGANG = 56;

constexpr uint8_t VL53L4CD_DEFAULT_I2C_ADDRESS = 0x29;

Timeout sendPeriodicTimer; ///\todo This is just for testing, Timeout Timer to send all objects periodically

void failHardInDebug(VL53L4CD_Error status)
{
#ifndef DEBUG
    return;
#endif

    if (status != VL53L4CD_ERROR_NONE)
    {
        fatalError();
    }
}

void initSerialDebug()
{
#ifndef DEBUG
    //return; ///\todo enable on release
#endif
    serial.setRxPin(SerialRxPin);
    serial.setTxPin(SerialTxPin);
    serial.begin(SerialBaudrate);
    serial.println("Selfbus I2C VL53L4CD Time of Flight (ToF) sensor example");
    serial.println("state;distance (mm);sigma (mm);ambient (kcps);ambient (kcps/SPAD);target (kcps);target (kcps/SPAD);#SPADs active");
}

void sendSerialResult(const VL53L4CD_ResultsData_t *result)
{
#ifndef DEBUG
    //return; ///\todo enable on release
#endif
    serial.print(result->range_status, DEC, 3);
    serial.print(";", result->distance_mm, DEC, 5);
    serial.print(";", result->sigma_mm, DEC, 5);
    serial.print(";", result->ambient_rate_kcps, DEC, 5);
    serial.print(";", result->ambient_per_spad_kcps, DEC, 5);
    serial.print(";", result->signal_rate_kcps, DEC, 5);
    serial.print(";", result->signal_per_spad_kcps, DEC, 5);
    serial.print(";", result->number_of_spad, DEC, 5);
    serial.println();
}

void calibrateOffset()
{
    int16_t measuredOffset;
    failHardInDebug(VL53L4CD_CalibrateOffset(VL53L4CD_DEFAULT_I2C_ADDRESS, 100, &measuredOffset, 255));
    //serial.println("calibrateOffset measuredOffset=", measuredOffset, DEC, 6);
}

bool sendStatusIfChanged(VL53L4CD_Error newStatus)
{
    VL53L4CD_Error currentStatus = bcu.comObjects->objectRead(COMOBJ_6_SENSORSTOERUNG_AUSGANG);
    if (currentStatus == newStatus)
    {
        return false;
    }
    bcu.comObjects->objectWrite(COMOBJ_6_SENSORSTOERUNG_AUSGANG, (newStatus != VL53L4CD_ERROR_NONE));
    bcu.comObjects->objectWrite(COMOBJ_56_SOFTWARE_VERSION_AUSGANG, static_cast<uint16_t>(newStatus));
//    serial.println("COMOBJ_56_SOFTWARE_VERSION_AUSGANG: ", newStatus);
    return true;
}

bool sendRangeIfChanged(float newRange, float delta)
{
    float currentRange = dpt14ToFloat(bcu.comObjects->objectRead(COMOBJ_1_MESSWERT_IN_METER_AUSGANG));
    if (fabsf(currentRange - newRange) < delta)
    {
        return false;
    }
    bcu.comObjects->objectWrite(COMOBJ_1_MESSWERT_IN_METER_AUSGANG, floatToDpt14(newRange));
/*
    serial.print("COMOBJ_1_MESSWERT_IN_METER_AUSGANG: ", newRange, 1);
    serial.print(" (DPT14) ", static_cast<unsigned int>(floatToDpt14(newRange)), HEX, 8);
    serial.println();
*/
    return true;
}

void objectUpdated(int objno)
{
    /*
    if (objno == )
    {
        calibrateOffset();
    }
    */
}

void processSensorResults()
{
    uint8_t dataReady = 0;
    VL53L4CD_Error status = VL53L4CD_CheckForDataReady(VL53L4CD_DEFAULT_I2C_ADDRESS, &dataReady);
    if (status != VL53L4CD_ERROR_NONE)
    {
        return;
    }

    if (dataReady == 0)
    {
        return;
    }

    VL53L4CD_ResultsData_t result;
    failHardInDebug(VL53L4CD_GetResult(VL53L4CD_DEFAULT_I2C_ADDRESS, &result));
    failHardInDebug(VL53L4CD_ClearInterrupt(VL53L4CD_DEFAULT_I2C_ADDRESS));

    sendSerialResult(&result);
    digitalWrite(PIN_INFO, !digitalRead(PIN_INFO));

    bool comObjectsUpdated = sendStatusIfChanged(result.range_status);
    if (result.range_status == VL53L4CD_ERROR_NONE)
    {
        comObjectsUpdated |= sendRangeIfChanged(result.distance_mm, MinimumSendThreshold_mm);
    }

    if (comObjectsUpdated)
    {
        return;
    }
    sendPeriodicTimer.start(SendPeriodicInterval_ms);
}

void checkPeriodic(void)
{
    processSensorResults();

    // TODO this is just for testing
    if (!sendPeriodicTimer.expired())
    {
        return;
    }
    //bcu.comObjects->objectWrite(OBJ_FILL_LEVEL_CALIBRATED_m3, rcParamMsg.level_m3_CalibratedToDPT());
    sendPeriodicTimer.start(SendPeriodicInterval_ms);
}

void initApplication(void)
{
    // LED Initialize
    pinMode(PIN_INFO, OUTPUT);
    digitalWrite(PIN_INFO, false);

    initSerialDebug();

    bcu.comObjects->objectWrite(COMOBJ_1_MESSWERT_IN_METER_AUSGANG, 0U);
    bcu.comObjects->objectWrite(COMOBJ_6_SENSORSTOERUNG_AUSGANG, true);
    bcu.comObjects->objectWrite(COMOBJ_56_SOFTWARE_VERSION_AUSGANG, VL53L4CD_ERROR_TIMEOUT);

    failHardInDebug(VL53L4CD_SensorInit(VL53L4CD_DEFAULT_I2C_ADDRESS));
    failHardInDebug(VL53L4CD_SetRangeTiming(VL53L4CD_DEFAULT_I2C_ADDRESS, TimingBudget_ms, InterMeasurement_ms));
    failHardInDebug(VL53L4CD_SetSigmaThreshold(VL53L4CD_DEFAULT_I2C_ADDRESS, RangeSigmaThreshold_mm));
    failHardInDebug(VL53L4CD_SetSignalThreshold(VL53L4CD_DEFAULT_I2C_ADDRESS, SignalThreshold_kcps));
    failHardInDebug(VL53L4CD_ClearInterrupt(VL53L4CD_DEFAULT_I2C_ADDRESS));
    //calibrateOffset();
    failHardInDebug(VL53L4CD_StartRanging(VL53L4CD_DEFAULT_I2C_ADDRESS));
    failHardInDebug(VL53L4CD_StartTemperatureUpdate(VL53L4CD_DEFAULT_I2C_ADDRESS));
    failHardInDebug(VL53L4CD_StartRanging(VL53L4CD_DEFAULT_I2C_ADDRESS));
    sendPeriodicTimer.start(SendPeriodicInterval_ms);
}
