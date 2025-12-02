/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#include "app_tof_vl53l4cd.h"
#include "config.h"
#include <sblib/mem_mapper.h>


APP_VERSION("VL53L4CD", "0", "01"); // Don't forget to also change the build-variable sw_version

// we need the memMapper for the range 0x4b00-4dff (size 0x300), which is not covered by UserEepromMASK0701
// Additional 0x100 bytes are for the memMapper´s allocTable
MemMapper memMapper(0xea00, 0x400);

/**
 * Application setup
 */
BcuBase* setup()
{
    memMapper.addRange(0x4b00, 0x300); // range 0x4b00-0x4dff
    bcu.setMemMapper(&memMapper);
    bcu.begin(MANUFACTURER, DEVICETYPE, APPVERSION, 0x4374);
    initApplication();
    return (&bcu);
}

/**
 * The application's main.
 */
void loop()
{
    int objno;
    // Handle updated communication objects
    while ((objno = bcu.comObjects->nextUpdatedObject()) >= 0)
    {
        objectUpdated(objno);
    }

    checkPeriodic();

    // Sleep up to 1 millisecond if there is nothing to do
    waitForInterrupt();
}

void loop_noapp()
{

}
