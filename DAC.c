/*
 * DAC.c
 *
 *  Created on: Oct 10, 2022
 *      Author: Daru
 *
 *  Notes:
 *  	Flow: Keypad = analog value -> digital -> DAC_write(digital) -> DAC outputs digital value to Vout
 *
 *
 *  	SPI1_NSS = PA4
 *  	SPI1_SCK = PA5
 *  	SPI1_MISO = PA6 (not used)
 *		SPI1_MOSI = PA7
 */

#include "main.h"
#include <stdio.h>

#define VREF 3300	// Vref = 3.3V
#define numBits 4096	// 2^12
#define numKeyPresses 3
#define SPICONFIGBITS 0x3000
#define MASK 0xFFF
#define PERCENT 100

// initialize the SPI peripheral to communicate with the DAC
void DAC_init()
{
	// enable clock on GPIOA and SPI1
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;		// SPI1 uses PA pins

	// configure GPIO pins Mode
	GPIOA->MODER &= ~(GPIO_MODER_MODE4 | GPIO_MODER_MODE5 | GPIO_MODER_MODE7);
	GPIOA->MODER |= (GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1 | GPIO_MODER_MODE7_1);

	// SPI Alt function select pins
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL4_Pos | GPIO_AFRL_AFSEL5_Pos | GPIO_AFRL_AFSEL7_Pos);
	GPIOA->AFR[0] |= ((5 << GPIO_AFRL_AFSEL4_Pos) | (5 << GPIO_AFRL_AFSEL5_Pos) | (5 << GPIO_AFRL_AFSEL7_Pos));

	// configure SPI1 CR1 = controller
	SPI1->CR1 |= (SPI_CR1_MSTR);

	// configure CR2 = CS signal	DS = 1111 for 16 bits
	SPI1->CR2 |= (SPI_CR2_SSOE | SPI_CR2_NSSP | (0xF << SPI_CR2_DS_Pos));

	// enable SPI1
	SPI1->CR1 |= SPI_CR1_SPE;
}

// calibrates the DAC to output closer to keypad's inputed voltage
// actual_v = voltage we calculated | desired_v = voltage keypad inputed
uint16_t DAC_calibrate(uint16_t actual_v, uint16_t desired_v)
{
	// calculate percent error
	uint16_t percent_error = (actual_v - desired_v) / desired_v * PERCENT;
	// get +- voltage
	uint16_t calibNum = percent_error * actual_v;

	if(actual_v < desired_v)
		actual_v += calibNum;
	else
		actual_v -= calibNum;

	return actual_v;
}

// write a 12-bit value to the DAC
void DAC_write(uint16_t digital)		// NOTE: 4-bit configuration is sent => 16-bit package
{
	// send data
	// if transmit buffer not empty
	while(!(SPI1->SR & SPI_SR_TXE));	// do nothing if true
//	SPI1->DR &= ~(0x0);
	// scrub digital value and add 4-bit DAC config
	SPI1->DR = ( SPICONFIGBITS | (MASK & digital) );
	uint16_t DR = SPI1->DR;
}

// convert a 12-bit digital value into an Analog value
// Note: 1. Keypad gives us an analog value
//		 2. Use formula for analog value -> digital
uint16_t DAC_volt_conv(uint16_t analog)
{
	// converts into single integer while in mV i.e. 3.3 V => 3300 mV
	uint16_t digital = 0;

	// max voltage = 3.3 V or 330 from 3 keys from keypad
	if(digital > VREF)
	{
		digital = VREF;
	}

	// analog -> digital
	digital = analog*numBits/VREF;

	return digital;
}
