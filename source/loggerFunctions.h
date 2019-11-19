/*
 * @file loggerFunctions.h
 * @brief Header file that contains the logger print functions
 *
 * This header file has the function prototypes for the
 * logger print functions, which can print strings, integers
 * and data at memory locations
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */
#ifndef LOGGERFUNCTIONS_H_
#define LOGGERFUNCTIONS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//If the logger is enabled
extern bool loggerEnable;

//logger mode
enum loggerMode {DEBUG, TEST, NORMAL};

/*
 * @brief Enable the logger
 *
 * This function enables the logger function
 *
 * @return void
 */
void log_enable(void);




/*
 * @brief Disable the logger
 *
 * This function disables the logger function
 *
 * @return void
 */
void log_disable(void);


/*
 * @brief Check log status
 *
 * This function returns true if the logger is enabled
 *
 * @return bool The status of the logger
 */
bool log_status(void);


/*
 * @brief Log a memory location
 *
 * This function prints the values at the inAddress continuously
 * till the desired length i.e number of data is reached
 *
 * @param *inAddress The input address whose data is to be printed
 * @param length Of how many addresses data should be printed
 * @return void
 */
void log_data(uint32_t *inAddress, size_t length);


/*
 * @brief Print a string
 *
 * This function prints a string followed by a newLine
 *
 * @param The string that has to be printed
 * @return void
 */
void log_string(char *inString);


/*
 * @brief Print an integer
 *
 * This function prints an integer followed by a newLine
 *
 * @param The integer that has to be printed
 * @return void
 */
void log_integer(uint32_t *inAddress);


/*
 * @brief Print a formatted string
 *
 * This function prints a string formatted in a manner
 * depending on the build configuration selected
 *
 * @param The build config number, the name of func from
 * which it is being called, The message
 *
 * @return void
 */
void log_message(enum loggerMode logLevel, const char *functionName, char *message);


/*
 * @brief Print a formatted integer
 *
 * This function prints an integer formatted in a manner
 * depending on the build configuration selected
 *
 * @param The build config number, the name of func from
 * which it is being called, The message
 *
 * @return void
 */
void log_message_int(enum loggerMode logLevel, const char *functionName, char* message, int number);

void sendString(char* in);
void sendChara(char in);

#endif /* LOGGERFUNCTIONS_H_ */
