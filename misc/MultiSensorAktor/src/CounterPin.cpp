/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#include <sblib/eib/bcu_base.h>

#include <CounterPin.h>
#include <math.h>

CounterPin::CounterPin(uint8_t firstComIndex, CounterPinConfig *config) : GenericPin(firstComIndex), config(config)
{
   	comObjects->requestObjectRead(firstComIndex);
}

void CounterPin::PutValue(uint32_t now, int val)
{
	int value = debouncer.value();
	if (value != lastValue && !value)
	{
		float val = comObjects->objectRead(firstComIndex);
		val += config->Increment;
		val = roundFloat(val, config->Decimals);
		comObjects->objectWrite(firstComIndex, (uint8_t*)&val);

		if (lastTime < now)
		{
			comObjects->objectWrite(firstComIndex + 1, now - lastTime);
			float freq = 3600000.0f / (now - lastTime);
			freq = roundFloat(freq, config->DecimalsFreq);
			comObjects->objectWrite(firstComIndex + 2, (uint8_t*)&freq);
		}
	}
}

float CounterPin::roundFloat(float val, int decimals)
{
	return floor(val * pow(10,decimals)) / pow(10,decimals);
}
