/*
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 3 as
 published by the Free Software Foundation.
 ---------------------------------------------------------------------------*/

#ifndef FT12_DEBUG_HANDLER_H_
#define FT12_DEBUG_HANDLER_H_

#include "config.h"


#if (SOFT_UART_ENABLE==true)
#   include <sblib/soft_uart.h>
#endif


#if (SOFT_UART_ENABLE==true)
#  define dump(code) { code; }
#else
#  define dump(code)
#endif


void debugFatal();

#if (SOFT_UART_ENABLE==true)
    extern SoftUART softUART; // declared in debug_handler.cpp
#endif

#endif /* FT12_DEBUG_HANDLER_H_ */
