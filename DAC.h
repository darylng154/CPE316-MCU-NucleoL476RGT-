/*
 * DAC.h
 *
 *  Created on: Oct 10, 2022
 *      Author: Daru
 */

#ifndef INC_DAC_H_
#define INC_DAC_H_

// initialize the SPI peripheral to communicate with the DAC
void DAC_init();

// write a 12-bit value to the DAC
void DAC_write(uint16_t digital);		// NOTE: 4-bit configuration is sent => 16-bit package

// calibrates the DAC to output closer to keypad's inputed voltage
// actual_v = voltage we calculated | desired_v = voltage keypad inputed
uint16_t DAC_calibrate(uint16_t actual_v, uint16_t desired_v);

// convert a voltage value into a 12-bit value to control the DAC
uint16_t DAC_volt_conv(uint16_t analog);

#endif /* INC_DAC_H_ */
