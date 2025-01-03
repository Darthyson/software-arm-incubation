/*
 *  Copyright (c) 2016-2021 Oliver Stefan
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */


#include <sblib/core.h>
#include <sblib/timeout.h>
#include "app_rtr.h"
#include "inputs.h"
#include "params.h"

unsigned int debounceTime;
Debouncer inputDebouncer[NUM_CHANNELS];
uint8_t inputChangedMem[NUM_CHANNELS];
unsigned int TasterPressTime[NUM_CHANNELS];

void init_inputs(void){

	// Configure the input pins and initialize the debouncers with the current
    // value of the pin.
    for (int channel = 0; channel < NUM_CHANNELS; ++channel)
    {

    	pinInterruptMode(inputPins[channel], INTERRUPT_EDGE_BOTH);
    	pinMode(inputPins[channel], INPUT | HYSTERESIS | PULL_UP);
    	pinEnableInterrupt(inputPins[channel]);

    	inputDebouncer[channel].init(digitalRead(inputPins[channel]));
    }

	// Enable the timer interrupt
	enableInterrupt(EINT1_IRQn);
	enableInterrupt(EINT2_IRQn);

	// set Timer priority lower than normal, because sblib interrupts have to be served with highest priority (prio = 0)
	NVIC_SetPriority(EINT1_IRQn, 1);
	NVIC_SetPriority(EINT2_IRQn, 1);
}

// Interrupt für PIO1_7
extern "C" void PIOINT1_IRQHandler(void)
{
	unsigned int source = LPC_GPIO1->MIS;
	LPC_GPIO1->IC |= source;
	if(source == (1<<7)){ //PIO1_7
		bool inputLevel = digitalRead(inputPins[1]);
		if(inputLevel == 0){	//Taste gedrückt
			TasterPressTime[0] = millis();
			timeout[0].start(LONGPRESSTIME);
		}else{		//Taste losgelassen
			timeout[0].stop();
			unsigned int timeDiff = millis() - TasterPressTime[0];
			if(timeDiff > LONGPRESSTIME){
//				inputChangedMem[1] = LONG_PRESS;
			}else if(timeDiff > debounceTime && timeDiff < LONGPRESSTIME){
					inputChangedMem[0] = SHORT_PRESS;
			}
		}
	}
}

//Interrupt für PIO 2_10
extern "C" void PIOINT2_IRQHandler(void)
{
	unsigned int source = LPC_GPIO2->MIS;
	LPC_GPIO2->IC |= source;
	if(source == (1<<10)){ //PIO2_10
		bool inputLevel = digitalRead(inputPins[0]);
		if(inputLevel == 0){	//Taste gedrückt
			TasterPressTime[1] = millis();
			timeout[1].start(LONGPRESSTIME);
		}else{		//Taste losgelassen
			timeout[1].stop();
			unsigned int timeDiff = millis() - TasterPressTime[1];
			if(timeDiff > LONGPRESSTIME){
//				inputChangedMem[1] = LONG_PRESS;
			}else if(timeDiff > debounceTime && timeDiff < LONGPRESSTIME){
					inputChangedMem[1] = SHORT_PRESS;
			}
		}
	}
}

void handlePeriodicInputs(){
	int channel, lastValue = 0;

	for (channel = 0; channel < NUM_CHANNELS; ++channel)
	{
		if(timeout[channel].started() && timeout[channel].expired()){
		   	if(timeout[channel ^ 1].started()){ //Channel 0 oder 1 Longpress und der andere auch gedrückt
		   		rebootApplication(); //RESET und Neuinitialisierung aller Komponenten
		   	}else{
		   		inputChanged(channel, lastValue, LONG_PRESS);
		   	}
		}
			if(inputChangedMem[channel] != NO_PRESS){
			inputChanged(channel, 1, inputChangedMem[channel]);
			inputChangedMem[channel] = NO_PRESS;
		}
	}
}

