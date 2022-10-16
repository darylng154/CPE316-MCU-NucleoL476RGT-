/*
 * led.h
 *
 *  Created on: Oct 16, 2022
 *      Author: Daru
 */

#ifndef INC_LED_H_
#define INC_LED_H_

void led_init();

// Decodes keypad_read() returns into LEDs
void decodeLED(uint8_t readNum);

#endif /* INC_LED_H_ */
