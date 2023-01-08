/*
 * keypad.h
 *
 *  Created on: Sep 26, 2022
 *      Author: Daryl Ng
 */

#ifndef SRC_KEYPAD_H_
#define SRC_KEYPAD_H_

/* configures GPIOA & C to Keypad Pins
* R1 = [PA5		PA12] = R4			C2 = [PB10
* R2 = [PA6		PA11] = R3			B1 = [PB4		PB14] = C3
*/
void keypad_init();

// polls for single input at a time
uint8_t keypad_read();

#endif /* SRC_KEYPAD_H_ */
