/*
 * @file    Project_5_UART.c
 * @brief   Application entry point.
 */

#include "uartIncludes.h"
#include "circularBuffer.h"

bool inputReady = false;

volatile bool reportPrint = false;

circularBuf *txBuf;
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

enum bufErrorCode initTxBuf(uint32_t inLength)
{
	txBuf = (circularBuf *)malloc(sizeof(circularBuf));

	if (txBuf == NULL)
		return failure;
	txBuf->charArray = (uint8_t *)malloc(inLength);
	if (txBuf->charArray == NULL) {
		free(txBuf);
		txBuf = NULL;
		return failure;
	}
	txBuf->length = inLength;
	txBuf->count = 0;

	txBuf->head = 0;
	txBuf->tail = 0;
	return success;
}

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

	initTxBuf(8);

	int il = 0;
	//	while (addElement(txBuf, il) != failure) {
	//		printf("Added %d\n", il);
	//		printf("Chararray: %d\n", txBuf->charArray[il]);
	//		il++;
	//	}
	//	printf("DONE\n");

	for(int i = 0; i < txBuf->length; i++){
		printf("Buffer: %d\n", txBuf->charArray[i]);
		char tempHolder[15];
		sprintf(tempHolder, "Buffer: %c\n", txBuf->charArray[i]);
		sendString(tempHolder);
	}

	zeroFullBuffer(txBuf);
	while (1)
	{
#ifdef pollingEnable
		uint8_t c = (uint8_t)UART0_Receive_Poll();
		//		UART0_Transmit_Poll((char)c);
#endif

#if interruptEnable == 1
		if (!(UART0->C2 & UART0_C2_TIE_MASK))
		{
			UART0->C2 |= UART0_C2_TIE(1);
		}
#endif


		uint8_t temp;
		//		scanf("%c", &temp);


		if (c == '.') {
			//			printf(". detected, dumping all elements\n");
			//			for(uint8_t i = 0; i < txBuf->length; i++)
			//				printf("ALL: %d: %c\n", i, txBuf->charArray[i]);
			//			delAllElements(txBuf);
//			sendString("DONE");
			reportPrint = true;
			if(reportPrint){
				printReport();
				reportPrint = false;
			}
			continue;
		}

		if (addElement(txBuf, c) == failure) {
//			sendString("REL");

			//			printf("Buffer Full, realloc to %lu\n", txBuf->length * 2);
			txBuf->length *= 2;
			uint8_t *bufTemp = txBuf->charArray;
			//			uint32_t tempSize = txBuf->length;
			//			uint8_t tempSizeByte = uint8_t()
			txBuf->charArray = realloc(txBuf->charArray, txBuf->length);

			if (txBuf->charArray == NULL) {
				//				printf("Realloc failed\nDumping all elements");
				txBuf->charArray = bufTemp;

				delAllElements(txBuf);
			}

			if (txBuf->tail == txBuf->head) {
				//				printf("Buffer Wrapped, moving elements\n");
				adjustElements(txBuf);
			}
//			sendString("RCD");

			if (addElement(txBuf, c) == failure)
				return 1;
		}

		//		for(uint8_t i = 0; i < txBuf->length; i++)
		//			printf("ALL: %d: %c\n", i, txBuf->charArray[i]);

		if(reportPrint){
			printReport();
			reportPrint = false;
		}
	}

}

void printReport(void)
{
	sendChara('\n');
	uint8_t inChar;
	unsigned int report[95] = {0};
	char buf[10];
	while(((inChar = delElement(txBuf)) != 0xFE) && reportPrint){
		report[inChar - 32]++;
	}
	for(int i = 0; (i < 95) && reportPrint; i++){
		if(report[i] != 0){
			sprintf (buf, "%c: %d; ", i + 32, report[i]);
			sendString(buf);
		}
	}
//	sendString(buf);
	sendChara('\n');
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
