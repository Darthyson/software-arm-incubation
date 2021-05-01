/*
 *  app_raincenter.cpp
 *
 *  Created on: 11.09.2020
 *  Copyright (c) 2020 Darthyson <darth@maptrack.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#include <sblib/internal/iap.h>
#include <sblib/eib.h>
#include <sblib/timeout.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "app_heliovent.h"

#ifdef HELIOS
#   include "heliosvent.h"
#else
#   include "mdtled.h"
#endif

void objectUpdated(int objno)
{
#ifdef HELIOS
    int switchObject = COMOBJ_LUEFTUNGSGERAET_HAUPTSCHALTER_SCHALTEN;
    int rmObject = COMOBJ_LUEFTUNGSGERAET_HAUPTSCHALTER_STATUS;
#else
    int switchObject = - 1;
    int rmObject = -1;
    switch (objno)
    {
        case COMOBJ_0_KANAL_A_SCHALTEN:
            switchObject = COMOBJ_0_KANAL_A_SCHALTEN;
            rmObject = COMOBJ_4_KANAL_A_STATUS_EIN_AUS;
            break;
        case COMOBJ_16_KANAL_B_SCHALTEN:
            switchObject = COMOBJ_16_KANAL_B_SCHALTEN;
            rmObject = COMOBJ_20_KANAL_B_STATUS_EIN_AUS;
            break;
        case COMOBJ_32_KANAL_C_SCHALTEN:
            switchObject = COMOBJ_32_KANAL_C_SCHALTEN;
            rmObject = COMOBJ_36_KANAL_C_STATUS_EIN_AUS;
            break;
        case COMOBJ_48_KANAL_D_SCHALTEN:
            switchObject = COMOBJ_48_KANAL_D_SCHALTEN;
            rmObject = COMOBJ_52_KANAL_D_STATUS_EIN_AUS;
            break;
        default:
            break;
    }
#endif


    if ((objno >= 0) && (objno == switchObject))
    {
        unsigned int status = objectRead(switchObject);
        objectWrite(rmObject, status);
        digitalWrite(PIN_INFO, status);
    }
}

void checkPeriodic(void)
{

}

void initApplication(void)
{
    pinMode(PIN_RUN, OUTPUT);
    pinMode(PIN_INFO, OUTPUT);
    digitalWrite(PIN_RUN, 1);
    digitalWrite(PIN_INFO, 1);
    delay(300);
    digitalWrite(PIN_INFO, 0);

#ifdef HELIOS
    objectWrite(COMOBJ_LUEFTUNGSGERAET_HAUPTSCHALTER_STATUS, (unsigned int) 0);
#else
    objectWrite(COMOBJ_4_KANAL_A_STATUS_EIN_AUS, (unsigned int) 0);
    objectWrite(COMOBJ_20_KANAL_B_STATUS_EIN_AUS, (unsigned int) 1);
    objectWrite(COMOBJ_36_KANAL_C_STATUS_EIN_AUS, (unsigned int) 1);
    objectWrite(COMOBJ_52_KANAL_D_STATUS_EIN_AUS, (unsigned int) 0);
#endif
}
