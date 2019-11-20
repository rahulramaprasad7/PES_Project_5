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

