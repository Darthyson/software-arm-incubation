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
 * @file   bcu_ft12.h
 * @author Darthyson <darth@maptrack.de> Copyright (c) 2022
 * @bug No known bugs.
 ******************************************************************************/

/*
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 3 as
 published by the Free Software Foundation.
 ---------------------------------------------------------------------------*/

#ifndef BCU_FT12_H_
#define BCU_FT12_H_

#include "ft12_protocol.h"
#include <sblib/eib/userRamBCU1.h>
#include <sblib/eib/bcu_base.h>


class BcuFt12: public BcuBase
{
public:
    BcuFt12();
    explicit BcuFt12(UserRamBCU1* userRamBcu1);
    ~BcuFt12() override = default;
    void begin();
    [[nodiscard]] bool applicationRunning() const override { return enabled; }
    uint8_t& layerStatus() override;

    /**
     * @brief Handles the PEI switch request.
     * @param payload  The buffer holding the payload of the PEI switch request, starting with System Status (octet 2)
     * @param length   The length of the payload buffer. Must be 6 for a PEI_Switch.req.
     * @note  KNX Spec. 3.0 3/6/3 3.3.9.7 p.57ff
     */
    void handlePeiSwitchRequest(const uint8_t* payload, uint8_t length);
protected:
    bool processApci(ApciCommand apciCmd, unsigned char * telegram, uint8_t telLength, uint8_t * sendBuffer) override;
    bool processGroupAddressTelegram(ApciCommand apciCmd, uint16_t groupAddress, unsigned char *telegram, uint8_t telLength) override;
    bool processBroadCastTelegram(ApciCommand apciCmd, unsigned char *telegram, uint8_t telLength) override;
private:
    void dumpLayerRedirection() const;

    Emi2LayerRedirection layerRedirection;
};



#endif /* BCU_FT12_H_ */
/** @}*/
