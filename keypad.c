/*
 * keypad.c
 *
 *  Created on: Sep 26, 2022
 *      Author: lcartier
 */

#include "keypad.h"

void keypadInit()
{
	  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	  //C0-3 row output, C4-7 col input, C8-11 LED output
	  GPIOC->MODER &= (0xFF000000); // Clear Bottom 24 Bits (0-11)
	  GPIOC->MODER |= (0x00550055); // 8-11 out, 4-7 in, 0-3 out

	  GPIOC->PUPDR &= (0xFFFF00FF); // Clear 4-7
	  GPIOC->PUPDR |= (0x0000AA00); // 4-7 Pull-down

	  setPadHigh();
}


uint8_t keyPressed()
{ //write to row, read from col
	uint8_t row;
	for (row = 0; row < 4; row++) { //loops over all rows
		GPIOC->BSRR = ((0xF<<(ROW_PT_OS + 16)) | (0x1<<(ROW_PT_OS + row))); //RESET and Set row #
		if (GPIOC->IDR & (0xF<<COL_PT_OS)) { // checks if row has signal
			switch (((GPIOC->IDR)>>COL_PT_OS) & 0xF) { //uses row and col to return grid#
			case 0x1:
				return row;
				break;
			case 0x2:
				return row + 4;
				break;
			case 0x4:
				return row + 8;
				break;
			case 0x8:
				return row + 12;
				break;
			}
		}
	}
	return -1; // error value, hard to tell if it's F, but all buttons return it in error state
}

void setPadHigh()
{
	GPIOC->BSRR = 0xF<<(ROW_PT_OS); //sets all high
	return;
}

uint8_t keyMap(uint8_t input)
{ // Maps keys to printed numbers
	switch (input) {
	case 0x0:
		return 0x1;
		break;
	case 0x1:
		return 0x4;
		break;
	case 0x2:
		return 0x7;
		break;
	case 0x3:
		return 0xE;
		break;
	case 0x4:
		return 0x2;
		break;
	case 0x5:
		return 0x5;
		break;
	case 0x6:
		return 0x8;
		break;
	case 0x7:
		return 0x0;
		break;
	case 0x8:
		return 0x3;
		break;
	case 0x9:
		return 0x6;
		break;
	case 0xA:
		return 0x9;
		break;
	case 0xB:
		return 0xF;
		break;
	case 0xC:
		return 0xA;
		break;
	case 0xD:
		return 0xB;
		break;
	case 0xE:
		return 0xC;
		break;
	case 0xF:
		return 0xD;
		break;
	default:
		return -1;
		break;
	}
}


// me
// Decodes keypad_read() returns into LEDs STM Pins: 9, 8, 6, 5 == 3, 2, 1, 0's place 2^#
void decodeLED(uint8_t readNum)
{

	// reset LEDs
	GPIOC->BRR = (GPIO_PIN_8);
	GPIOC->BRR = (GPIO_PIN_9);
	GPIOC->BRR = (GPIO_PIN_10);
	GPIOC->BRR = (GPIO_PIN_11);

	switch(readNum)
	{
		case 1:
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 2:
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 3:
			GPIOC->BSRR |= (GPIO_PIN_9);
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 4:
			GPIOC->BSRR |= (GPIO_PIN_10);
			break;

		case 5:
			GPIOC->BSRR |= (GPIO_PIN_10);
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 6:
			GPIOC->BSRR |= (GPIO_PIN_10);
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 7:
			GPIOC->BSRR |= (GPIO_PIN_10);
			GPIOC->BSRR |= (GPIO_PIN_9);
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 8:
			GPIOC->BSRR |= (GPIO_PIN_11);
			break;

		case 9:
			GPIOC->BSRR |= (GPIO_PIN_11);
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 10:
			GPIOC->BSRR |= (GPIO_PIN_11);
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 11:
			GPIOC->BSRR |= (GPIO_PIN_11);
			GPIOC->BSRR |= (GPIO_PIN_9);
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 12:
			GPIOC->BSRR |= (GPIO_PIN_11);
			GPIOC->BSRR |= (GPIO_PIN_10);
			break;

		case 13:
			GPIOC->BSRR |= (GPIO_PIN_11);
			GPIOC->BSRR |= (GPIO_PIN_10);
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 14:
			GPIOC->BSRR |= (GPIO_PIN_11);
			GPIOC->BSRR |= (GPIO_PIN_10);
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 15:
			GPIOC->BSRR |= (GPIO_PIN_11);
			GPIOC->BSRR |= (GPIO_PIN_10);
			GPIOC->BSRR |= (GPIO_PIN_9);
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		default:
			break;
	}
}
