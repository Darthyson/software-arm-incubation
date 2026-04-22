/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#ifndef INPUTPINCONFIG_H_
#define INPUTPINCONFIG_H_

#include <sblib/eib/bcu_base.h>

enum PortInAction : uint8_t
{
	PortInNothing,
	PortInOff,
	PortInOn,
	PortInToggle
};

#pragma pack(1)
struct InputPinConfig
{
    uint8_t DebounceTime;
	PortInAction Hi;
	PortInAction Lo;
	PortInAction HiLong;
	PortInAction LoLong;
	bool ShortOnLong;
	uint16_t LongTime;
	uint8_t LockFlags;
};
#pragma pack()


#endif /* INPUTPINCONFIG_H_ */
