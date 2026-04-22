/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#ifndef DHTPIN_H_
#define DHTPIN_H_

#include <sblib/eib/bcu_base.h>
#include <sblib/sensors/dht.h>
#include <sblib/digital_pin.h>

#include <GenericPin.h>
#include <DHTConfig.h>

class DHTPin : public GenericPin
{
public:
	DHTPin(int port, uint8_t firstComIndex, DHTConfig* config, bool dht11, uint16_t& objRamPointer);
	~DHTPin() = default;

	uint8_t GetState(uint32_t now, uint8_t updatedObjectNo);

	int ConfigLength() { return sizeof(DHTConfig); }
	int ComObjCount() { return 4; }

protected:
	uint32_t nextAction = 0;
	uint8_t state = 0;
	DHT dht;
	DHTConfig* config;
	uint8_t retries;
};


#endif /* DHTPIN_H_ */
