/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#ifndef GENERICPIN_H_
#define GENERICPIN_H_

#include <sblib/eib/com_objects.h>

class GenericPin
{
public:
	GenericPin(uint8_t firstComIndex) : firstComIndex(firstComIndex) {};
	~GenericPin() = default;

	virtual uint8_t GetState(uint32_t now, uint8_t updatedObjectNo) { return 0; };
	virtual void PutValue(uint32_t now, int val) {};

	virtual int ConfigLength() = 0;
	virtual int ComObjCount() = 0;

	static ComObjects* comObjects;

protected:
	uint8_t firstComIndex;
};

#endif /* GENERICPIN_H_ */
