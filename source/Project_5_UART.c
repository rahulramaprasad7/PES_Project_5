/**
 * @file    Project_5_UART.c
 * @brief   Application entry point.
 */

#include "uartIncludes.h"

bool inputReady = false;


int main(void)
{
	//	uint8_t c;

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	//	/* Init FSL debug console. */
	//	BOARD_InitDebugConsole();

	uartInit();
	//hello

	while (1)
	{
#ifdef pollingEnable
		c = UART0_Receive_Poll();
		UART0_Transmit_Poll(c);
#endif

#if interruptEnable == 1
		if (!(UART0->C2 & UART0_C2_TIE_MASK))
		{
			UART0->C2 |= UART0_C2_TIE(1);
		}
#endif
	}

}

void UART0_IRQHandler(void)
{
	uint8_t ch;

	//	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK))
	//	{
	//		// clear the error flags
	//		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | 	UART0_S1_FE_MASK | UART0_S1_PF_MASK;
	//		// read the data register to clear RDRF
	//		ch = UART0->D;
	//	}
	if (UART0->S1 & UART0_S1_RDRF_MASK)
	{
		// received a character
		ch = UART0->D;
		inputReady = true;
//		UART0->D = ch;

	}
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK) ) // transmitter interrupt enabled
	{
		// can send another character
		if(inputReady == true)
		{
			UART0->D = ch;
			inputReady = false;
		}
	}
}
