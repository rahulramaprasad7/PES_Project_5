/*
 * @file Project_5_UART.c
 * @brief Source file that circular buffer and UART
 *
 * This source file initialises the UART register,
 * runs the UART functions and circular buffers.
 *
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 19 2019
 * @verison 1.0
 */

#include "uartIncludes.h"
#include "circularBuffer.h"
#include "uCUnit.h"
#include "unitTest.h"

volatile uint8_t c;
bool inputReady = false;

volatile bool reportPrint = false;
volatile bool txBufferReady = false;

circularBuf *txBuf;
circularBuf *rxBuf;



void sendChara(char in)
{
	transmitPoll((uint8_t)in);
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

enum bufErrorCode initRxBuf(uint32_t inLength)
{
	rxBuf = (circularBuf *)malloc(sizeof(circularBuf));

	if (rxBuf == NULL)
		return failure;
	rxBuf->charArray = (uint8_t *)malloc(inLength);
	if (rxBuf->charArray == NULL) {
		free(rxBuf);
		rxBuf = NULL;
		return failure;
	}
	rxBuf->length = inLength;
	rxBuf->count = 0;

	rxBuf->head = 0;
	rxBuf->tail = 0;
	return success;
}

void endProgram(void)
{
	ledOff();
	redLED();

	log_message(NORMAL, __func__, "Critical error detected, terminating program");
	log_message(DEBUG, __func__, "Critical error detected, terminating program");
	log_message(TEST, __func__, "Critical error detected, terminating program");

	while(1);
}

int main(void)
{

	//	uint8_t c;

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

	Init_RGB_LEDs();

	//Initialize the RGB LEDs
	uartInit();
	Init_RGB_LEDs();

	ledOff();
	blueLED();

	//Initialize the systick timer to tick in 15 seconds
	Init_SysTick();

#ifndef test
	if(initTxBuf(8) == failure){
		log_message(NORMAL, __func__, "TX buffer initial allocation failed, terminating program");
		log_message(DEBUG, __func__, "TX buffer initial allocation failed, terminating program");
	}
	if(initRxBuf(256) == failure){
		log_message(NORMAL, __func__, "RX buffer initial allocation failed, terminating program");
		log_message(DEBUG, __func__, "RX buffer initial allocation failed, terminating program");
	}
#endif

#ifdef test
	unitTest();
#endif

	zeroFullBuffer(txBuf);
#if interruptEnable == 1
	if (!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}
#endif


	while (1)
	{

#if applicationMode == 1
#if interruptEnable == 1
		if(txBufferReady){
			//			printf("READY\n");
			if(transmitReady() == success){
				//				printf("READY 2\n");
				char txChar = delElement(rxBuf);
				if(txChar == 0xFE){
					txBufferReady = false;
				} else {
					//					printf("SEND\n");
					sendChara(txChar);
				}
			}
		}
#endif
#endif



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
		log_message(NORMAL, __func__, "Printing buffer report");
		log_message(DEBUG, __func__, "Printing buffer report");
		reportPrint = true;
		if(reportPrint){
			printReport();
			reportPrint = false;
		}
		return;
	}

	if (addElement(txBuf, c) == failure) {
		log_message(DEBUG, __func__, "Adding element failed, reallocating");
		START_CRITICAL;
		//			sendString("REL");

		//			printf("Buffer Full, realloc to %lu\n", txBuf->length * 2);
		txBuf->length *= 2;
		uint8_t *bufTemp = txBuf->charArray;
		txBuf->charArray = realloc(txBuf->charArray, txBuf->length);
		UCUNIT_Tracepoint(0);

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
				UCUNIT_Tracepoint(1);
				adjustElements(txBuf);
			}
		}
		//			sendString("RCD");

		if (addElement(txBuf, c) == failure)
			return;
		END_CRITICAL;
		log_message(DEBUG, __func__, "Buffer adjustment successful");
	}

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
#if applicationMode == 1
#if interruptEnable == 0
			sendString(buf);
#endif
#endif
#if applicationMode == 1
#if interruptEnable == 1
			int j = 0;
			while(buf[j] != '\0'){
				addElement(rxBuf, buf[j]);
				j++;
			}
#endif
#endif
		}
	}
	addElement(rxBuf, '\n');
	txBufferReady = true;
}

void UART0_IRQHandler(void)
{
	uint8_t i;
	//	printf("ISR\n");

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
				//				printf("ISR 1\n");
				UART0_Transmit_Poll(i);
				inputReady = false;
				//				UART0->C2 |= UART0_C2_TIE_MASK;
			}

		}
#endif

#if applicationMode == 1
		i = UART0_Receive_Poll();
		c = i;
		inputReady = true;
		txBufferReady = false;
#endif
	}
	UART0->C2 &= ~UART0_C2_TIE(1);
}

