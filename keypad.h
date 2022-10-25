/*
 * keypad.h
 *
 *  Created on: Sep 28, 2022
 *      Author: lcartier
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "main.h"

#define ROW_PINS 4
#define COL_PINS 4

#define ROW_PT_OS 0
#define COL_PT_OS (ROW_PT_OS + 4)
#define LED_PT_OS (COL_PT_OS + 4)

void keypadInit();
uint8_t keyPressed();
void setPadHigh();
uint8_t keyMap(uint8_t);

// me
void decodeLED(uint8_t readNum);


#endif /* INC_KEYPAD_H_ */
