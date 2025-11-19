/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#include <sblib/eib/bcu_base.h>
#include <sblib/i2c.h>
#include <sblib/timer.h>

#include <SHT4xItem.h>
#include <HelperFunctions.h>

extern int portPins[32];

SHT4xItem::SHT4xItem(byte firstComIndex, TempHumSensorConfig *config, GenericItem* nextItem, uint16_t& objRamPointer) : GenericItem(firstComIndex, nextItem), config(config), sht4x(SHT4xClass()), nextAction(0)
{
	sht4x.init();
	offset = dpt9ToFloat(config->Offset) * 0.01f;

	HelperFunctions::setComObjPtr(comObjects, firstComIndex, BIT_1, objRamPointer);
	HelperFunctions::setComObjPtr(comObjects, firstComIndex + 1, BYTE_2, objRamPointer);
	HelperFunctions::setComObjPtr(comObjects, firstComIndex + 2, BYTE_4, objRamPointer);
	HelperFunctions::setComObjPtr(comObjects, firstComIndex + 3, BYTE_2, objRamPointer);
}

void SHT4xItem::Loop(uint32_t now, int updatedObjectNo)
{
	if (now >= nextAction || (now + (config->PreFan + config->PreMeasure + config->Delay) * 1000) < nextAction)
	{
		switch (state)
		{
		case 0:
			if (config->PreFan > 0)
			{
				comObjects->objectWrite(firstComIndex, 1);
				nextAction = now + (config->PreFan * 1000);
			}
			else
			{
				nextAction = now;
			}
			state++;
			break;
		case 1:
			if (config->PreFan > 0)
			{
				comObjects->objectWrite(firstComIndex, (int)0);
				nextAction = now + (config->PreMeasure * 1000);
			}
			else
			{
				nextAction = now;
			}
			state++;
			break;
		default:
			if (sht4x.measureHighPrecision())
			{
				float fhum = sht4x.getHumidity();
				float ftemp = sht4x.getTemperature() + offset;
				int16_t temp = (int16_t)(ftemp * 100);
				uint16_t hum = (uint16_t)(fhum * 100);
				comObjects->objectWriteFloat(firstComIndex + 1, temp);
				comObjects->objectWrite(firstComIndex + 2, (byte*)&ftemp);
				comObjects->objectWriteFloat(firstComIndex + 3, hum);
			}
			else
			{
				sht4x.init();
			}
			nextAction = now + (config->Delay * 1000);
			state = 0;
			break;
		}

		// Überlauf würde zu Dauermessung führen bis SystemTime auch überläuft
		if (nextAction < now)
		{
			nextAction = 0xFFFFFFFF;
		}
	}
	return;
}

