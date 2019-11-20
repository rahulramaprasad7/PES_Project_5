/*
 * @file circularBuffer.h
 * @brief Header file containing the functions prototypes to operate on
 *			circular buffers
 *
 * This header file contains functions prototypes that can create, destroy
 * and do operations such as add to and delete from a circular buffer, and it also
 * contains supporting elements such as function metadata and the enumerations
 * used for error handling
 *
 * @date 16 Nov 2019
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 */


#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ledControl.h"
#include "loggerFunctions.h"
#include "MKL25Z4.h"

#define START_CRITICAL __disable_irq()
#define END_CRITICAL __enable_irq()

#define tenthSec 286102UL


//Enumeration defining all error codes that the circular buffer functions can return
enum bufErrorCode {bufferFull = 1, bufferNotFull, bufferEmpty, bufferNotEmpty, success, failure};

//The structure containing the circular buffer and the metadata
typedef struct circularBuf {
	uint8_t *charArray;	//The buffer
	uint32_t head;		//The head, points to the oldest element in the buffer
	uint32_t tail;		//The tail, points to the latest element in the buffer
	uint32_t length;		//The total length of the buffer
	uint32_t count;		//The number of elements currently in the buffer
} circularBuf;

//A temperory buffer for testing
extern uint8_t inBuffer[20];

//transmit queue circular buffer
extern circularBuf *txBuf;
//receive queue circular buffer
extern circularBuf *rxBuf;
extern uint8_t *charArray1;
extern unsigned long tenth;
extern volatile bool txBufferReady;
extern volatile uint8_t c;

/*
 * @brief check if the buffer is full
 *
 * This function checks if the input circular buffer is full
 *
 * @param The pointer to the circular buffer and metadata
 * @return The enum indicating if the buffer is full/not full
 */
enum bufErrorCode checkFull(circularBuf *inBuf);

/*
 * @brief check if the buffer is empty
 *
 * This function checks if the input circular buffer is empty
 *
 * @param The pointer to the circular buffer and metadata
 * @return The enum indicating if the buffer is empty/not empty
 */
enum bufErrorCode checkEmpty(circularBuf *inBuf);

/*
 * @brief Add an element to the buffer
 *
 * This function adds an element to the input circular buffer
 *
 * @param The pointer to the circular buffer and metadata
 * @param The data to be added
 * @return The result of the addition attempt (success/failure)
 */
enum bufErrorCode addElement(circularBuf *inBuf, uint8_t inData);


/*
 * @brief Delete an element from the buffer
 *
 * This function deletes an element from the input circular buffer
 *
 * @param The pointer to the circular buffer and metadata
 * @return The element which was removed (0xFE in case of an error)
 */
uint8_t delElement(circularBuf *inBuf);


/*
 * @brief Empty the input buffer
 *
 * This function prints all the elements of the input
 * buffer and emptys it
 *
 * @param The pointer to the circular buffer and metadata
 * @return The result of the print attempt (success/failure)
 */
enum bufErrorCode emptyBuffer(circularBuf *inBuf);


/*
 * @brief Initialize the circular buffer and its metadata
 *
 * This function allocates the metadata for a circular buffer and
 * the buffer itself. Returns the pointer to the allocation, and NULL
 * if it fails
 *
 * @param The length of the buffer to be allocated
 * @return The pointer to the allocated buffer, NULL if allocation failed
 */
enum bufErrorCode initBuffer(circularBuf *inBuf, size_t inLength);

/*
 * @brief Delete a circular buffer and its metadata
 *
 * This function deallocates the metadata for a circular buffer and
 * the buffer itself.
 *
 * @param The pointer to the circular buffer and metadata to be destroyed
 * @return The result of the operation (success/failure)
 */
enum bufErrorCode destroyBuffer(circularBuf *inBuf);

/*
 * @brief Verify if a buffer metadata is allocated/valid
 *
 * This function checks if the metadata of a circular buffer is valid
 *
 * @param The pointer to the circular buffer metadata to be checked
 * @return The result of the check (success/failure)
 */
enum bufErrorCode verifyBuffer(circularBuf *inBuf);

/*
 * @brief Verify if the circular buffer is allocated/valid
 *
 * This function checks if the circular buffer is valid
 *
 * @param The pointer to the circular buffer to be checked
 * @return The result of the check (success/failure)
 */
enum bufErrorCode verifyBufPointer(circularBuf *inBuf);

/*
 * @brief Zeros all the elements in a buffer (DEBUGGING ONLY)
 *
 * This function Zeros all the elements of the buffer from zero
 * to last index
 *
 * @param The pointer to the circular buffer metadata to be zeroed
 * @return void
 */
void zeroFullBuffer(circularBuf *inBuf);

/*
 * @brief Delete all the elements in a buffer
 *
 * This function prints all the elements of the buffer and deletes them
 *
 * @param The pointer to the circular buffer metadata to be deleted
 * @return The result of the deletion (success/failure)
 */
enum bufErrorCode delAllElements(circularBuf *inBuf);

/*
 * @brief Adjusts the position of elements of a circular buffer
 *
 * In case the circular buffer elements are wrapped around, and the buffer
 * is reallocated, the elements needs to be moved, which this function does
 *
 * @param The pointer to the circular buffer to be moved
 * @return The result of the adjustment (success/failure)
 */
enum bufErrorCode adjustElements(circularBuf *inBuf);

/*
 * @brief Initialize the transmitter buffer
 *
 * Initializes the transmitter metadata structure and the
 * buffer, by allocating memory to it. It also fills the metadata
 * with appropriate values
 *
 * @param The length of the buffer to be allocated
 * @return The result of the operation
 */
enum bufErrorCode initTxBuf(uint32_t inLength);

/*
 * @brief Initialize the receiver buffer
 *
 * Initializes the receiver metadata structure and the
 * buffer, by allocating memory to it. It also fills the metadata
 * with appropriate values
 *
 * @param The length of the buffer to be allocated
 * @return The result of the operation
 */
void printReport(void);

/*
 * @brief runs the application that counts character
 *
 * This function contains the application that counts the number
 * of character input, and prints a report of the character count
 * when the character '.' is entered
 *
 * @return void
 */
void application(void);

/*
 * @brief Initializes the systick counter
 *
 * Initializes the systick counter to interrupt in 1/10 of
 * a second
 *
 * @return void
 */
void Init_SysTick(void);

/*
 * @brief Terminate the program
 *
 * This function informs the user if the program is being
 * terminated, turns the LED to red, and enters an infinite loop
 *
 * @return void
 */
void endProgram(void);

#endif
