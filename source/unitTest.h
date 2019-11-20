/*
 * @file unitTest.h
 * @brief Header file has all the unit tests
 *
 * This header file contains function prototypes the unit tests called,
 * which are defined in the uCUnit.h header
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 18 2019
 * @verison 1.0
 */

#ifndef FRDM_INCLUDES_UNITTEST_H_
#define FRDM_INCLUDES_UNITTEST_H_

#include "ledControl.h"
#include "circularBuffer.h"
#include "uartIncludes.h"

/*
 * @brief Performs a set of unit tests on the KL25Z board
 *
 * Performs a set of functions such as check for null, in range, size
 * of variable, is equal, etc on the freedom board, and outputs a
 * summary of test cases
 *
 * @return void
 */
void unitTest(void);

#endif /* FRDM_INCLUDES_UNITTEST_H_ */
