/*
 * @file circularBuffer.c
 * @brief source file containing the functions to operate on circular buffers
 *
 * This source file contains functions that can create, destroy and do operations
 * such as add to and delete from a circular buffer
 *
 * @date 16 Nov 2019
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 */


#include "circularBuffer.h"

//buffer used during testing
uint8_t inBuffer[20];

//move the elements of the circular buffer during reallocation
enum bufErrorCode adjustElements(circularBuf *inBuf)
{
//	for(uint8_t m = 0; m < txBuf->length; m++)
//		printf("ALL: %d: %c\n", m, txBuf->charArray[m]);

	//Check if the input buffer is valid, else return failure
	if(inBuf == NULL)
		return success;
	if(inBuf->charArray == NULL)
		return success;

	//A temperory variable holding the old size of the buffer
	uint32_t l = (inBuf->length / 2);

	//inform the user if the buffer has wrapped
	for (uint32_t i = 0; i < l - inBuf->head; ++i)
	{
		//move the elements of the buffer to the leftmost side, one by one
		inBuf->charArray[inBuf->head + l + i] = inBuf->charArray[inBuf->head + i];
		//set the former position of the elements to 0
		inBuf->charArray[inBuf->head + i] = 0;
	}
	//Adjust the head of the buffer to the new location
	inBuf->head = inBuf->head + l;
	//For error handling
	return success;
}

//Print and delete all elements of the buffer
enum bufErrorCode delAllElements(circularBuf *inBuf)
{
	//Check if the input buffer is valid, else return failure
	if (inBuf == NULL)
		return failure;
	if (inBuf->charArray == NULL)
		return failure;

	//Print and empty all elements of the input buffer
	uint8_t c;
	while ((c = delElement(inBuf)) != 0xFE) {
//		printf("Delete: %c\n", c);
	}
	//Error handling
	return success;

}

//Zero all the elements of the input buffer (DEBUGGING ONLY)
void zeroFullBuffer(circularBuf *inBuf)
{
	//Zero all elements from index 0 to last
	for (int i = 0; i < inBuf->length; ++i)
	{
		inBuf->charArray[i] = 0;
	}
}

//Check if the input buffer is full
enum bufErrorCode checkFull(circularBuf *inBuf)
{
	//Check if the input buffer is valid, else return failure
	if (inBuf == NULL)
		return failure;
	if (inBuf->charArray == NULL)
		return failure;

	//If the count of the buffer is equal to the length, then the buffer is full
	if (inBuf->count == inBuf->length) {
		return bufferFull;
	}
	else
		return bufferNotFull;
}

//Check if the input buffer is empty
enum bufErrorCode checkEmpty(circularBuf *inBuf)
{
	//Check if the input buffer is valid, else return failure
	if (inBuf == NULL)
		return failure;
	if (inBuf->charArray == NULL)
		return failure;

	//If the count of the buffer is 0, the buffer is empty
	if (inBuf->count == 0)
		return bufferEmpty;
	else
		return bufferNotEmpty;
}


enum bufErrorCode addElement(circularBuf *inBuf, uint8_t inData)
{
	START_CRITICAL;
	if (checkFull(inBuf) == bufferFull){
		END_CRITICAL;
		return failure;
	}

	inBuf->charArray[inBuf->tail] = inData;
	inBuf->tail = (inBuf->tail + 1) % inBuf->length;
	inBuf->count++;

	END_CRITICAL;
	return success;
}

//delete an element in the buffer
uint8_t delElement(circularBuf *inBuf)
{
	START_CRITICAL;
	//Check if the buffer is empty
	if (checkEmpty(inBuf) == bufferEmpty || checkEmpty(inBuf) == failure) {
		emptyBuffer(inBuf);
		END_CRITICAL;
		return 0xFE;
	}

	//Read the character to be deleted
	char readChar = inBuf->charArray[inBuf->head];
	//Adjust the head of the buffer, considering wrap around
	inBuf->head = (inBuf->head + 1) % inBuf->length;
	//Adjust buffer count
	inBuf->count--;
	END_CRITICAL;
	//Return the deleted element
	return readChar;
}

//Empty the buffer
enum bufErrorCode emptyBuffer(circularBuf *inBuf)
{
	//Check if the buffer is empty
	if (checkEmpty(inBuf) == bufferEmpty || checkEmpty(inBuf) == failure)
		return failure;

	//Make the tail and head of the buffer equal (zero)
	inBuf->head = 0;
	inBuf->tail = inBuf->head;
	//reset the buffer count
	inBuf->count = 0;

	//status of the operation
	return success;
}

//Destroy the buffer and its metadata
enum bufErrorCode destroyBuffer(circularBuf *inBuf)
{
	//check if the input buffer is valid
	if (inBuf == NULL)
		return failure;

	//If only the metadata is valid, free it
	if (inBuf->charArray == NULL) {
		free(inBuf);
		//set the pointer to null after freeing
		inBuf = NULL;
		return success;
	}

	//Free the buffer
	free(inBuf->charArray);
	inBuf->charArray = NULL;

	//free the metadata
	free(inBuf);
	inBuf = NULL;

	return success;
}

//Verify if the buffer metadata is valid
enum bufErrorCode verifyBuffer(circularBuf *inBuf)
{
	//Check if input buffer pointer in not null
	if (inBuf == NULL)
		return failure;

	//Check if the input buffer is not zero length
	if (inBuf->length == 0)
		return failure;

	//result of the operation
	return success;
}

//Verify if the buffer is valid
enum bufErrorCode verifyBufPointer(circularBuf *inBuf)
{
	//Check if input buffer metadata pointer in not null
	if (inBuf == NULL)
		return failure;
	//Check if the buffer pointer is not null
	if (inBuf->charArray == NULL)
		return failure;

	//result of the operation
	return success;
}

//Initialize and allocate a buffer
enum bufErrorCode initBuffer(circularBuf *inBuf, size_t inLength)
{
	//Allocate metadata of a buffer metadata
	inBuf = malloc(sizeof(circularBuf));
	//Check if the allocation failed
	if (inBuf == NULL)
		return failure;
	//Set the buffer pointer to NULL
	inBuf->charArray = NULL;

	//Set the length of the buffer
	inBuf->length = inLength;
	//Set zero elements filled
	inBuf->count = 0;
	//Allocate space for circular buffer
	inBuf->charArray = malloc(sizeof(inBuf->length * sizeof(char)));

	//Set the head and tail to initial values
	inBuf->head = 0;
	inBuf->tail = 0;

	//return the pointer if the allocation was successful
	return success;
}

void Init_SysTick(void)
{
	//Load value eq to 15 sec
	SysTick->LOAD = tenthSec;
	//Set the systick timer priority
	NVIC_SetPriority(SysTick_IRQn, 3);
	//set the value register to 0
	SysTick->VAL = 0;
	//Enable and start the timer
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

unsigned long tenth = 0;

void SysTick_Handler(void)
{
	tenth++;
}
