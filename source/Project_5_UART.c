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

//Character input from UART
volatile uint8_t c;

//Is the input from UART rx?
bool inputReady = false;

//Print the character count report
volatile bool reportPrint = false;

//Is the buffer ready for transmission?
volatile bool txBufferReady = false;

//The transmitter metadata and buffer
circularBuf *txBuf;

//The receiver metadata and buffer
circularBuf *rxBuf;


//Send a single character
void sendChara(char in)
{
	transmitPoll((uint8_t)in);
}

//Send a string
void sendString(char* in)
{
	int i = 0;
	//send a string till null reached
	while (*in){
		sendChara(*in++);
		i++;
	}
}

//Initialize the transmitter buffer
enum bufErrorCode initTxBuf(uint32_t inLength)
{
	//Allocate the metadata
	txBuf = (circularBuf *)malloc(sizeof(circularBuf));

	//Error handling if allocation fails
	if (txBuf == NULL)
		return failure;

	//Allocate the character buffer
	txBuf->charArray = (uint8_t *)malloc(inLength);

	//Error handling
	if (txBuf->charArray == NULL) {
		free(txBuf);
		txBuf = NULL;
		return failure;
	}
	//Update metadata
	txBuf->length = inLength;
	txBuf->count = 0;

	//Reset head and tail of the buffer
	txBuf->head = 0;
	txBuf->tail = 0;

	return success;
}

//Initialize the receiver buffer
enum bufErrorCode initRxBuf(uint32_t inLength)
{
	//Allocate the metadata
	rxBuf = (circularBuf *)malloc(sizeof(circularBuf));

	//Error handling if allocation fails
	if (rxBuf == NULL)
		return failure;

	//Allocate the character buffer
	rxBuf->charArray = (uint8_t *)malloc(inLength);

	//Error handling
	if (rxBuf->charArray == NULL) {
		free(rxBuf);
		rxBuf = NULL;
		return failure;
	}
	//Update metadata
	rxBuf->length = inLength;
	rxBuf->count = 0;

	//Reset head and tail of the buffer
	rxBuf->head = 0;
	rxBuf->tail = 0;
	return success;
}

void endProgram(void)
{
	//Change LED status to RED
	ledOff();
	redLED();

	//inform user
	log_message(NORMAL, __func__, "Critical error detected, terminating program");
	log_message(DEBUG, __func__, "Critical error detected, terminating program");
	log_message(TEST, __func__, "Critical error detected, terminating program");

	//end program
	while(1);
}

int main(void)
{

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

	//Initialize the RGB leds
	Init_RGB_LEDs();

	//Initialize the UART
	uartInit();
	Init_RGB_LEDs();

	//Initial blue LED status
	ledOff();
	blueLED();

	//Initialize the systick timer to tick in 0.1 seconds
	Init_SysTick();

	//End program if initial allocation fails
#ifndef test
	if(initTxBuf(8) == failure){
		log_message(NORMAL, __func__, "TX buffer initial allocation failed, terminating program");
		log_message(DEBUG, __func__, "TX buffer initial allocation failed, terminating program");
		endProgram();
	}
	if(initRxBuf(256) == failure){
		log_message(NORMAL, __func__, "RX buffer initial allocation failed, terminating program");
		log_message(DEBUG, __func__, "RX buffer initial allocation failed, terminating program");
		endProgram();
	}
#endif

	//Execute the uCUnit test if test mode is selected
#ifdef test
	unitTest();
#endif

	//Zero the contents of the buffer
	zeroFullBuffer(txBuf);
#if interruptEnable == 1
	if (!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}
#endif


	while (1)
	{

		//If transmitter is free, send the elements of the
		//UART buffer one at a time
#if applicationMode == 1
#if interruptEnable == 1
		if(txBufferReady){
			if(transmitReady() == success){
				char txChar = delElement(rxBuf);
				if(txChar == 0xFE){
					txBufferReady = false;
				} else {
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
		//receive one character from UART
		c = (uint8_t)receivePoll();
		//run the character count application
		application();
#endif

		//Interrupt mode
#if interruptEnable == 1

		if (inputReady == true)
		{
			//If input is ready from the buffer, run application
			inputReady = false;
			application();
		}
#endif
#endif
	}
}

//The character count application
void application(void)
{
	//Print the count of the buffer on the input character '.'
	if (c == '.') {
		log_message(NORMAL, __func__, "Printing buffer report");
		log_message(DEBUG, __func__, "Printing buffer report");
		reportPrint = true;
		//Print/ready the report
		if(reportPrint){
			printReport();
			reportPrint = false;
		}
		return;
	}

	//If a new element cant be added to the input buffer, try to
	//increase the buffer size
	if (addElement(txBuf, c) == failure) {
		log_message(DEBUG, __func__, "Adding element failed, reallocating");
		//Start the critical section of the code
		START_CRITICAL;
		//Double the length of the buffer
		txBuf->length *= 2;
		//save the buffer in the temperory variable before reallocating
		uint8_t *bufTemp = txBuf->charArray;
		//reallocate the buffer
		txBuf->charArray = realloc(txBuf->charArray, txBuf->length);
		//for uC unit
		UCUNIT_Tracepoint(0);

		//in case reallocation fails
		if (txBuf->charArray == NULL) {
			//restore the buffer pointer
			txBuf->charArray = bufTemp;
			//Restore the previous length
			txBuf->length /= 2;
			//Print the report (empty out the buffer)
			reportPrint = true;
			if(reportPrint){
				sendChara('\n');
				sendString("Buffer full, dumping elements");
				sendChara('\n');
				printReport();
				reportPrint = false;
			}
		} else {
			//If reallocation was successful, move elements if the head and tail
			//pointers overlap
			if (txBuf->tail == txBuf->head) {
				UCUNIT_Tracepoint(1);
				adjustElements(txBuf);
			}
		}


		if (addElement(txBuf, c) == failure){
			log_message(NORMAL, __func__, "Buffer data corrupt, terminating program");
			log_message(DEBUG, __func__, "Buffer data corrupt, terminating program");
			endProgram();
			return;
		}
		END_CRITICAL;
		log_message(DEBUG, __func__, "Buffer adjustment successful");
	}

	//Print the report
	if(reportPrint){
		printReport();
		reportPrint = false;
	}
}

//Print the character count
void printReport(void)
{
	sendChara('\n');
	uint8_t inChar;
	//Temp buffer to save character
	unsigned int report[95] = {0};
	char buf[10];
	//Till the buffer empties out, increment the position (with an offset)
	while(((inChar = delElement(txBuf)) != 0xFE) && reportPrint){
		report[inChar - 32]++;
	}
	//Generate the report string
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
			//Add element to the transmission buffer
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

