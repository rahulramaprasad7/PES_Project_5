/**
 * @file    Project_5_UART.c
 * @brief   Application entry point.
 */

#include "uartIncludes.h"
#include "circularBuffer.h"

bool inputReady = false;

//circularBuf *txBuf;
circularBuf *rxBuf;


void sendChara(char in)
{
	//Send 1 char
	//	while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
	UART0_Transmit_Poll((uint8_t) in);
	//	UART0->D = (uint8_t)in;
}


void sendString(char* in)
{
	int i = 0;
	//send a string till null reached
	while (*in){
		sendChara(*in++);
		i++;
	}
}
uint8_t *charArray1;

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

	//	if((initBuffer(txBuf, 32)) == failure)
	//		return 1;
	//	if((initBuffer(rxBuf, 32)) == failure)
	//		return 1;

	circularBuf *txBuf = (circularBuf *)malloc(sizeof(circularBuf));

	if (txBuf == NULL)
		return 1;
	txBuf->charArray = NULL;

	txBuf->length = 32;
	txBuf->count = 0;
	charArray1 = (uint8_t *)malloc(32);
	if (charArray1 == NULL) {
		free(txBuf);
		txBuf = NULL;
		return 1;
	}

	txBuf->head = 0;
	txBuf->tail = 0;

//	printf("%p %lu %lu %lu %lu\n", charArray1[1], txBuf->head, txBuf->tail, txBuf->length, txBuf->count);


	int il = 0;
	while (addElement(txBuf, il) != failure) {
		printf("Added %d\n", il);
		printf("Chararray: %d\n", charArray1[il]);
		il++;
	}
	printf("DONE\n");
//	i = 0;

	for(int i = 0; i < txBuf->length; i++){
		printf("Buffer: %d\n", charArray1[i]);
		char tempHolder[15];
		sprintf(tempHolder, "Buffer: %c\n", charArray1[i]);
		sendString(tempHolder);
	}

	while (1)
	{
#ifdef pollingEnable
		char c = UART0_Receive_Poll();
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
	uint8_t i;

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
		i = UART0->D;
		inputReady = true;
		UART0->D = i;

	}
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK) ) // transmitter interrupt enabled
	{
		// can send another character
		if(inputReady == true)
		{
			UART0->D = i;
			inputReady = false;
		}
	}
}
