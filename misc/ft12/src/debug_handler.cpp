/*
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 3 as
 published by the Free Software Foundation.
 ---------------------------------------------------------------------------*/

#include "debug_handler.h"
#include "bcu_ft12.h"
#include <sblib/timer.h>
#include <sblib/utils.h>


#if (SOFT_UART_ENABLE==true)
    SoftUART softUART(PIN_FT_SOFT_UART_RX, PIN_FT_SOFT_UART_TX,
            timer16_0, FT12_SOFT_UART_BAUD_RATE, SystemCoreClock);
#endif

void debugFatal()
{
#ifdef DEBUG
    extern BcuFt12 bcuFt12;
    bcuFt12.setProgPin(LED_KNX_RX);
    fatalError();
#endif
#if (SOFT_UART_ENABLE==true)
    dump(softUART.println("fatal"))
#endif
}

#if (SOFT_UART_ENABLE==true)
extern "C" void TIMER16_0_IRQHandler()
{
    softUART.timerInterruptHandler();
}
extern "C" void PIOINT1_IRQHandler()
{
    softUART.handleGpioInterrupt();
}
#endif
