/*
 * @file unitTest.c
 * @brief Source file has all the unit tests
 *
 * This source file contains all the unit tests called, which are defined
 * in the uCUnit.h header
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 18 2019
 * @verison 1.0
 */

#include "uCUnit.h"
#include <stdint.h>
#include "unitTest.h"
#include "circularBuffer.h"

int ucunit_checks_failed = 0; /* Number of failed checks */
int ucunit_checks_passed = 0; /* Number of passed checks */

int ucunit_testcases_failed = 0; /* Number of failed test cases */
int ucunit_testcases_passed = 0; /* Number of passed test cases */
int ucunit_testcases_failed_checks = 0; /* Number of failed checks in a testcase */
int ucunit_checklist_failed_checks = 0; /* Number of failed checks in a checklist */
int ucunit_action = UCUNIT_ACTION_WARNING; /* Action to take if a check fails */
int ucunit_checkpoints[UCUNIT_MAX_TRACEPOINTS]; /* Max. number of tracepoints */
int ucunit_index = 0; /* Tracepoint index */

void unitTest(void)
{
	//Initialize testing
	UCUNIT_Init();
	UCUNIT_TestcaseBegin("Unit test on circular buffers and UART");


	//Checking if the struct pointer and buffer pointer are vaild
	UCUNIT_WriteString("Checking if the struct pointer and buffer pointer are vaild\n");
	initTxBuf(4);
	UCUNIT_CheckIsNull(txBuf);
	UCUNIT_CheckIsNull(txBuf->charArray);

	//add and read element from a buffer
	UCUNIT_WriteString("Adding and reading element from a buffer\n");
	addElement(txBuf, 5);
	uint8_t temp = delElement(txBuf);
	UCUNIT_CheckIsEqual(temp, 5);

	//Verify if buffer can add element when wrapped
	UCUNIT_WriteString("Verify if buffer can add element when wrapped\n");
	while(addElement(txBuf, 5) != failure);
	UCUNIT_CheckIsEqual(txBuf->head, txBuf->tail);

	//Verify if buffer removes element when wrapped
	UCUNIT_WriteString("Verify if buffer removes element when wrapped\n");
	while(delElement(txBuf) != 0xFE);
	UCUNIT_CheckIsEqual(txBuf->head, txBuf->tail);

	//Verify if buffer returns error on deletion when empty
	UCUNIT_WriteString("Verify if buffer returns error on deletion when empty\n");
	UCUNIT_CheckIsEqual(delElement(txBuf), 0xFE);

	//Verify if buffer returns error on addition when full
	UCUNIT_WriteString("Verify if buffer returns error on addition when full\n");
	while(addElement(txBuf, 5) != failure);
	UCUNIT_CheckIsEqual(addElement(txBuf, 5), failure);

	//Check if buffer reallocation attempt happens (tracepoint)
	UCUNIT_WriteString("Check if buffer reallocation attempt happens (tracepoint)\n");
	c = 5;
	application();
	UCUNIT_CheckTracepointCoverage(0);

	//Check if the reallocation returns null
	UCUNIT_WriteString("Check if the reallocation returns null\n");
	UCUNIT_CheckIsNull(txBuf);
	UCUNIT_CheckIsNull(txBuf->charArray);

	//Check if buffer shifts elements if wrapped (tracepoints)
	UCUNIT_WriteString("Check if buffer shifts elements if wrapped (tracepoints)\n");
	UCUNIT_CheckTracepointCoverage(1);

	//Check if the buffer is destroyed
	UCUNIT_WriteString("Check if the buffer is destroyed\n");
	destroyBuffer(txBuf);
	UCUNIT_CheckIsNull(txBuf);
	UCUNIT_CheckIsNull(txBuf->charArray);

	//End testing and write summary
	UCUNIT_WriteSummary();

	UCUNIT_Shutdown();
}
