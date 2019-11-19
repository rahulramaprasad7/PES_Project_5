/*
 * @file    Project_5_UART.c
 * @brief   Application entry point.
 */

#include "uartIncludes.h"
#include "circularBuffer.h"

uint8_t c;
bool inputReady = false;

volatile bool reportPrint = false;

circularBuf *txBuf;
circularBuf *rxBuf;


void sendChara(char in)
{
	//Send 1 char
	//	while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
//	UART0_Transmit_Poll((uint8_t) in);
	transmitPoll((uint8_t)in);
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
#if applicationMode == 0
#if interruptEnable == 0
		echoPoll();
#endif
#endif
#if applicationMode == 1
#if interruptEnable == 0
		c = (uint8_t)receivePoll();
		application();
#endif

#if interruptEnable == 1
		if (inputReady == true)
		{
			inputReady = false;
			application();
		}
#endif
#endif
	}
}

void application(void)
{
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
		return;
	}

	if (addElement(txBuf, c) == failure) {
		START_CRITICAL;
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
			txBuf->length /= 2;
			reportPrint = true;
			if(reportPrint){
				sendChara('\n');
				sendString("Buffer full, dumping elements");
				sendChara('\n');
				printReport();
				reportPrint = false;
			}
		} else {
			if (txBuf->tail == txBuf->head) {
				//				printf("Buffer Wrapped, moving elements\n");
				adjustElements(txBuf);
			}
		}
		//			sendString("RCD");

		if (addElement(txBuf, c) == failure)
			return;
		END_CRITICAL;
	}

	//		for(uint8_t i = 0; i < txBuf->length; i++)
	//			printf("ALL: %d: %c\n", i, txBuf->charArray[i]);

	if(reportPrint){
		printReport();
		reportPrint = false;
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

	if (UART0->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK))
	{
		// clear the error flags
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | UART0_S1_FE_MASK;
		// read the data register to clear RDRF
		i = UART0->D;
	}
	if (receiveReady() == success)
	{
#if applicationMode == 0
		// received a character
		i = UART0_Receive_Poll();
		inputReady = true;

		if ( transmitReady() == success) // transmitter interrupt enabled
		{
			if (inputReady == true)
			{
				UART0_Transmit_Poll(i);
				inputReady = false;
			}

		}
#endif

#if applicationMode == 1
		i = UART0_Receive_Poll();
		c = i;
		inputReady = true;
#endif
	}
}
