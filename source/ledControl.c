/*
 * @file ledControl.c
 * @brief Source file that controls the RGB LED
 *
 * This source file turns LEDs on and off of a particular color, based
 *  on the function called
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */

#include "ledControl.h"

void Init_RGB_LEDs(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;  //Enable Port B and Port D

	PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);   //Enable the LED by selecting alternative function of the port

	PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1); //Enable the LED by selecting alternative function of the port

	PORTD->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(1);  //Enable the LED by selecting alternative function of the port

	PTB->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);
	PTD->PDDR |= MASK(BLUE_LED_POS);
//	log_message(DEBUG, __func__, "Initialising LEDs");
//	log_message(TEST, __func__, "Initialising LEDs");
}

void ledOff()
{
	PTB->PSOR = MASK(RED_LED_POS);  //Switch off RED
	PTB->PSOR = MASK(GREEN_LED_POS); //Switch off GREEN
	PTD->PSOR = MASK(BLUE_LED_POS); //Switch off BLUE
//	log_message(DEBUG, __func__, "Switching off LEDs");
}

void redLED()
{
	ledOff();
	PTB->PCOR = MASK(RED_LED_POS); //Switch on RED
}

void greenLED()
{
	ledOff();
	PTB->PCOR = MASK(GREEN_LED_POS); //Switch on GREEN
}

void blueLED()
{
	ledOff();
	PTD->PCOR = MASK(BLUE_LED_POS); //Switch on BLUE
}
