/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#ifndef DEVICECONFIG_H_
#define DEVICECONFIG_H_

#include <sblib/eib/bcu_base.h>

enum class BusSwitch : uint8_t
{
//	SPI0 = 0x01,
//	SPI1 = 0x02,
	I2C = 0x04
};

inline bool operator&(BusSwitch a, BusSwitch b)
{
	return static_cast<bool>(static_cast<int>(a) & static_cast<int>(b));
}

enum class SHTSwitch : uint8_t
{
	SHT2x = 0x80,
	SHT4x = 0x40,
	SGP4x = 0x20,
};

inline bool operator&(SHTSwitch a, SHTSwitch b)
{
	return static_cast<bool>(static_cast<int>(a) & static_cast<int>(b));
}

#pragma pack(1)
struct DeviceConfig
{
	BusSwitch BusSwitches;
	uint8_t SCK0;
	uint8_t SCK1;
	uint8_t MISO1;
	uint8_t BH1750Count;
	uint8_t CCS811Count;
	uint8_t DS3231Count;
	uint8_t iAQCoreCount;
	SHTSwitch SHTOption;
	uint8_t PCA9555DCount;
	uint8_t PortAssignment[36];
};
#pragma pack()

#endif /* DEVICECONFIG_H_ */
