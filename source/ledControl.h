/*
 * @file ledControl.h
 * @brief Header file for controlling the RGB LED
 *
 * This header file has prototypes for functions that
 * turns LEDs on and off of a particular color, based
 * on the function called
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */

#ifndef LEDCONTROL_H_
#define LEDCONTROL_H_

#include "../CMSIS/MKL25Z4.h"
#include "loggerFunctions.h"

#define RED_LED_POS (18)  //Define port number for Red LED
#define GREEN_LED_POS (19) //Define port number for Green LED
#define BLUE_LED_POS (1) //Define port number for Blue LED

#define MASK(x) (1UL << (x))

/*
 * @brief Initializes the LED pins
 *
 * Enables the clocks, sets pins as outputs, etc for the LEDs
 * using the bare metal functions
 *
 * @return void
 */
void Init_RGB_LEDs(void);

/*
 * @brief Turns off all LEDs
 *
 * Turns off all RGB leds
 *
 * @return void
 */
void ledOff();

/*
 * @brief Change LED color
 *
 * Change LED color to red
 *
 * @return void
 */
void redLED();

/*
 * @brief Change LED color
 *
 * Change LED color to green
 *
 * @return void
 */
void greenLED();

/*
 * @brief Change LED color
 *
 * Change LED color to blue
 *
 * @return void
 */
void blueLED();

#endif /* LEDCONTROL_H_ */
