/*
 * uartIncludes.h
 *
 *  Created on: Nov 16, 2019
 *      Author: rahul
 */

#ifndef UARTINCLUDES_H_
#define UARTINCLUDES_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

void uartInit();

void UART0_Transmit_Poll(uint8_t data);

uint8_t UART0_Receive_Poll(void);

#endif /* UARTINCLUDES_H_ */
