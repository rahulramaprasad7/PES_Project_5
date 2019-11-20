/*
 * @file uartIncludes.c
 * @brief Source file that controls the UART
 *
 * This source file initialises UART registers
 * and defines the transmit and recieve functions
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 19 2019
 * @verison 1.0
 */

#include "uartIncludes.h"

enum bufErrorCode temp;
uint8_t ch, transmitTemp;

void uartInit()
{
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;					//Enable the bus clock to UART peripheral
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;					//Enabling clock for Port A

	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);				// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK; //Disabling transmitter and receiver

	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	/* Baud rate calculation
	 * Fclk = 48 MHz
	 * BaudRate = Fclk/( (OSR +1) * SBR)
	 * SBR = Fclk/((OSR +1) * BaudRate
	 */
	UART0->C4 |= UART0_C4_OSR(15);						//Oversampling ratio = 16, default
	UART0->BDH &= ~UART0_BDH_SBNS_MASK; 				//Choosing one stop bit
	UART0->BDH &= ~UART0_BDH_SBR_MASK;					//SBR[12:8] =0
	UART0->BDL |= 0x34;									//SBR[7:0] = 52 for 57600 baud rate

#if interruptEnable == 1
	NVIC_SetPriority(UART0_IRQn, 0);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
	UART0->C2 |= UART0_C2_RIE_MASK;						//Enable receiver interrupt
	UART0->C3 |= UART0_C3_ORIE_MASK | UART0_C3_FEIE_MASK | UART0_C3_NEIE_MASK;
#endif

	UART0->C2 |= UART0_C2_TE_MASK | UART0_C2_RE_MASK; 	//Enabling transmitter and receiver
	UART0->S1 &= ~UART0_S1_RDRF_MASK;
}

enum bufErrorCode transmitReady()
{
	ledOff();
	greenLED();

#if interruptEnable == 0
	while (!(UART0->S1 & UART0_S1_TDRE_MASK));		//Wait till Transmitter Data Register Empty flag is set
	temp = success;
#endif

#if interruptEnable == 1
	UART0->C2 |= UART0_C2_TIE(1);
	//	printf("CHECK\n");
	if ((UART0->S1 & UART0_S1_TDRE_MASK) ){ // transmitter interrupt enabled
		temp =  success;
		//		printf("CHECK SUCCESS\n");
	}
	else
		temp = failure;
#endif
	return temp;
}

void UART0_Transmit_Poll(uint8_t data)
{
//	ledOff();
//	greenLED();
	UART0->D = data;
	UART0->C2 |= UART0_C2_TIE(1);
}

void transmitPoll(uint8_t data)
{
//	ledOff();
//	greenLED();
#if interruptEnable == 1
	//	if (transmitReady() == success)
	UART0_Transmit_Poll(data);
#endif

#if interruptEnable == 0
	if (transmitReady() == success)
		UART0_Transmit_Poll(data);
#endif
}

enum bufErrorCode receiveReady()
{
		ledOff();
		blueLED();

#if interruptEnable == 0
	UART0->S1 |= UART0_S1_OR_MASK;
	while (!(UART0->S1 & UART0_S1_RDRF_MASK));		//Wait till Receiver Data Register full flag is set
	temp = success;
#endif

#if interruptEnable == 1
	if (UART0->S1 & UART0_S1_RDRF_MASK)
		temp =  success;
	else
		temp = failure;
#endif
	return temp;
}

uint8_t UART0_Receive_Poll(void)
{
	return UART0->D;
}

uint8_t receivePoll()
{

#if interruptEnable == 1
	if (receiveReady() == success)
		transmitTemp = UART0_Receive_Poll();
#endif

#if interruptEnable == 0
	if (receiveReady() == success)
		transmitTemp = UART0_Receive_Poll();
#endif
	return transmitTemp;
}

void echoPoll()
{
	ch = receivePoll();
	transmitPoll(ch);
}

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
