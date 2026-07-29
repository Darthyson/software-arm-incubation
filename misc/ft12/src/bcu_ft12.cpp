/**************************************************************************//**
 * @addtogroup SBLIB_MAIN_GROUP Main Group Description
 * @defgroup SBLIB_SUB_GROUP_1 Sub Group 1 Description
 * @ingroup SBLIB_MAIN_GROUP
 * @brief   
 * @details 
 *
 *
 * @{
 *
 * @file   bcu_ft12.cpp
 * @author Darthyson <darth@maptrack.de> Copyright (c) 2022
 * @bug No known bugs.
 ******************************************************************************/

/*
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 3 as
 published by the Free Software Foundation.
 ---------------------------------------------------------------------------*/

#include "bcu_ft12.h"
#include "debug_handler.h"


BcuFt12::BcuFt12() : BcuFt12(new UserRamBCU1())
{
}

BcuFt12::BcuFt12(UserRamBCU1* userRamBcu1) :
        BcuBase(userRamBcu1, nullptr),
        layerRedirection{ EMI2_MODULE_LL, EMI2_MODULE_NL, EMI2_MODULE_TLG, EMI2_MODULE_TLC, EMI2_MODULE_TLL,
            EMI2_MODULE_AL, EMI2_MODULE_MAN, EMI2_MODULE_PEI, EMI2_MODULE_USR, EMI2_MODULE_RES }
{
}

bool BcuFt12::processApci([[maybe_unused]] ApciCommand apciCmd, [[maybe_unused]] unsigned char * telegram,
    [[maybe_unused]] uint8_t telLength, [[maybe_unused]] uint8_t * sendBuffer)
{
    return false;
}

void BcuFt12::begin()
{
    constexpr uint8_t InitBcuStatus = BCU_STATUS_LINK_LAYER | BCU_STATUS_TRANSPORT_LAYER |
        BCU_STATUS_APPLICATION_LAYER | BCU_STATUS_SERIAL_PEI;
    userRam->status() = InitBcuStatus;

    userRam->runState() = 1;
    BcuBase::_begin();
}

bool BcuFt12::processBroadCastTelegram([[maybe_unused]] ApciCommand apciCmd, [[maybe_unused]] unsigned char *telegram,
    [[maybe_unused]] uint8_t telLength)
{
    return true;
}

bool BcuFt12::processGroupAddressTelegram([[maybe_unused]] ApciCommand apciCmd, [[maybe_unused]] uint16_t groupAddress,
    [[maybe_unused]] unsigned char *telegram, [[maybe_unused]] uint8_t telLength)
{
    return true;
}

uint8_t& BcuFt12::layerStatus()
{
    return userRam->status();
}

void BcuFt12::handlePeiSwitchRequest(const uint8_t* payload, const uint8_t length)
{
    dump(softUART.print("PEI_Switch_Req (hex) ");)
    if (length < 6)
    {
        dump(softUART.println("ERROR to short ", length);)
        debugFatal();
        return;
    }

    const uint8_t newSystemStatus = payload[0];
    layerRedirection.linkLayer = static_cast<Emi2ModuleID>(payload[1] >> 4);
    layerRedirection.networkLayer = static_cast<Emi2ModuleID>(payload[1] & 0xf);
    layerRedirection.transportLayerGroupOriented = static_cast<Emi2ModuleID>(payload[2] >> 4);
    layerRedirection.transportLayerConnectionOriented = static_cast<Emi2ModuleID>(payload[2] & 0xf);
    layerRedirection.transportLayerLocal = static_cast<Emi2ModuleID>(payload[3] >> 4);
    layerRedirection.applicationLayer = static_cast<Emi2ModuleID>(payload[3] & 0xf);
    layerRedirection.managementApplicationLayer = static_cast<Emi2ModuleID>(payload[4] >> 4);
    layerRedirection.physicalExternalInterface = static_cast<Emi2ModuleID>(payload[4] & 0xf);
    layerRedirection.user = static_cast<Emi2ModuleID>(payload[5] >> 4);
    layerRedirection.reserved = static_cast<Emi2ModuleID>(payload[5] & 0xf);

    dump(
        softUART.print(newSystemStatus, HEX, 2);
        dumpLayerRedirection();
        softUART.println();
    );

    if (newSystemStatus != 0)
    {
        userRam->status() = newSystemStatus;
        dump(softUART.println("new status 0x", userRam->status(), HEX, 2);)
    }
    else
    {
        dump(
            softUART.print("WARN new status 0x", newSystemStatus, HEX, 2);
            softUART.println(" ignored. Status 0x", userRam->status(), HEX, 2);
        )
    }
}

void BcuFt12::dumpLayerRedirection() const
{
    dump(
        softUART.print(" ", layerRedirection.linkLayer, HEX);
        softUART.print(" ", layerRedirection.networkLayer, HEX);
        softUART.print(" ", layerRedirection.transportLayerGroupOriented, HEX);
        softUART.print(" ", layerRedirection.transportLayerConnectionOriented, HEX);
        softUART.print(" ", layerRedirection.transportLayerLocal, HEX);
        softUART.print(" ", layerRedirection.applicationLayer, HEX);
        softUART.print(" ", layerRedirection.managementApplicationLayer, HEX);
        softUART.print(" ", layerRedirection.physicalExternalInterface, HEX);
        softUART.print(" ", layerRedirection.user, HEX);
        softUART.print(" ", layerRedirection.reserved, HEX);
    )
}

/** @}*/
