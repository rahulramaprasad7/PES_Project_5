/*
 * @file uartIncludes.h
 * @brief Header file containing the functions prototypes to operate on
 *			UART functions
 *
 * This header file contains functions prototypes that can send, receive
 * characters between the PC and KL25Z. It also has separate function
 * prototypes for interrupt and polling modes.
 * @date 16 Nov 2019
 * @authors Rahul Ramaprasad, Prayag Milan Desai
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
#include <stdbool.h>
#include "circularBuffer.h"

/*
 * @brief Initialises the UART0 registers
 *
 * This function initialises the baud rate and tx & rx for UART
 *
 * @return void
 */
void uartInit();

/*
 * @brief Transmit one byte of data
 *
 * This function transmits one byte of data
 *
 * @param The data to be transmitted
 * @return void
 */
void UART0_Transmit_Poll(uint8_t data);

/*
 * @brief CHeck if it is ready for transmission
 *
 * This function checks if the KL25Z is ready for transmission
 *
 * @return Success or failure
 */
enum bufErrorCode transmitReady();

/*
 * @brief Transmit one byte of data
 *
 * This function transmits one byte of data after the
 * KL25Z is ready for transmission
 *
 * @param The data to be transmitted
 * @return void
 */
void transmitPoll(uint8_t data);

/*
 * @brief Checks if KL25Z is ready to receive a byte
 *
 * This function checks if the receiver is ready to receive
 *
 * @return success or failure
 */
enum bufErrorCode receiveReady();

/*
 * @brief Receives one byte of data
 *
 * This function receives one byte of data
 *
 * @return the byte received
 */
uint8_t UART0_Receive_Poll(void);

/*
 * @brief Receives one byte of data
 *
 * This function receives one byte of data after
 * checking if the KL25Z is ready to receive
 *
 * @return the byte received
 */
uint8_t receivePoll();

/*
 * @brief Echoes a byte
 *
 * This function receives a byte and transmits the
 * same byte
 *
 * @return void
 */
void echoPoll();

#define interruptEnable 1
#define applicationMode 0
#endif /* UARTINCLUDES_H_ */
