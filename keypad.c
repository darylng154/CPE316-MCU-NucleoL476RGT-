/*
 * keypad.c
 *
 *  Created on: Sep 26, 2022
 *      Author: Daryl Ng
 *
 *		HARD KEY Physical Keypad Pins: C1|C2|C3|C4|R1|R2|R3|R4
 *		MEMBRANE Physical Keypad Pins: R1|R2|R3|R4|C1|C2|C3|C4
 *
 *      Code to MEMBRANE Keypad Board Layout:
 * R1 = [PH0		PA1] = C1
 * R2 = [PH1		PA4] = C2
 * 		[			   ]
 * R3 = [PC2		PC1] = C3
 * R4 = [PC3		PC0] = C4
 *
*/

#include "main.h"
#include "keypad.h"
#include <stdint.h>

#define NUMCOLUMNS 4
#define NUMPORTS 16

#define LASTROW 3
#define LASTCOLUMN 4

void enable_clk()
{
	// enable clock to used GPIOs
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOCEN);
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOHEN);
}

// configures GPIOC to Keypad Pins
void keypad_init()
{
	// configure ROW PINS to Input
	GPIOH->MODER &= ~(GPIO_MODER_MODE0);

	GPIOH->MODER &= ~(GPIO_MODER_MODE1);

	GPIOC->MODER &= ~(GPIO_MODER_MODE2);

	GPIOC->MODER &= ~(GPIO_MODER_MODE3);

	// configure COLUMN PINS to output
	GPIOA->MODER &= ~(GPIO_MODER_MODE1);
	GPIOA->MODER |= (GPIO_MODER_MODE1_0);

	GPIOA->MODER &= ~(GPIO_MODER_MODE4);
	GPIOA->MODER |= (GPIO_MODER_MODE4_0);

	GPIOC->MODER &= ~(GPIO_MODER_MODE1);
	GPIOC->MODER |= (GPIO_MODER_MODE1_0);

	GPIOC->MODER &= ~(GPIO_MODER_MODE0);
	GPIOC->MODER |= (GPIO_MODER_MODE0_0);

	// enable pull-down resistors on ROW PINS (PUPDR)
	GPIOH->PUPDR &= ~(GPIO_PUPDR_PUPD0);
	GPIOH->PUPDR |= (GPIO_PUPDR_PUPD0_1);

	GPIOH->PUPDR &= ~(GPIO_PUPDR_PUPD1);
	GPIOH->PUPDR |= (GPIO_PUPDR_PUPD1_1);

	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD2);
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPD2_1);

	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD3);
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPD3_1);
}

uint8_t mapKeys(uint8_t onColumn, uint8_t onRow)
{
	if(onRow == 0 && onColumn == 4)
	{
		return 10;
	}
	else if(onRow == 1)
	{
		switch(onColumn)
		{
			case 1:
				return 4;
				break;

			case 2:
				return 5;
				break;

			case 3:
				return 6;
				break;

			case 4:
				return 11;
				break;

			default:
				break;
		}
	}
	else if(onRow == 2)
	{
		switch(onColumn)
		{
			case 1:
				return 7;
				break;

			case 2:
				return 8;
				break;

			case 3:
				return 9;
				break;

			case 4:
				return 12;
				break;

			default:
				break;
		}
	}
	else if(onRow == 3)
	{
		switch(onColumn)
		{
			case 1:
				return 14;
				break;

			case 2:
				return 0;
				break;

			case 3:
				return 15;
				break;

			case 4:
				return 13;
				break;

			default:
				break;
		}
	}
	else
		return onColumn + (onRow*NUMCOLUMNS);

	return -1;
}

uint8_t isKeyPressed()
{
	// checks if any rows are activated
	if( (GPIOH->IDR & GPIO_PIN_0) || (GPIOH->IDR & GPIO_PIN_1) ||
		(GPIOC->IDR & GPIO_PIN_2) || (GPIOC->IDR & GPIO_PIN_3) )
		return 1;
	else
		return 0;
}

/*
 *	a. Detect any key
 *		Set all columns to high value
 *		Read all rows
 *
 *	b. Detect specific key
 *		(1) Turn off all columns
 *		(2) Cycle through each column
 *		(3) Turning one on at a time and read rows
 *	Line of thought:
 *		Columns are always HIGH. When button is pushed, that Row will be HIGH => Key pressed.
 *		We always know which Column is pushed.
 *
 */
// polls for single input at a time
// onRows starts @ 0, onColumn starts @ 1
uint8_t keypad_read()
{
	uint8_t onColumn = 0;
	uint8_t onRow = 0;
	uint8_t i = 0;

	while(1)
	{
		// (2)
		for(i = 1; i <= NUMCOLUMNS; i++)
		{
			// (1)
			GPIOA->BRR = (GPIO_PIN_1);
			GPIOA->BRR = (GPIO_PIN_4);
			GPIOC->BRR = (GPIO_PIN_1);
			GPIOC->BRR = (GPIO_PIN_0);

			onColumn = i;
			onRow = 0;

			//	(3)
			// Turns ON one Column at a time
			switch(i)
			{
				case 1:
					GPIOA->ODR |= (GPIO_PIN_1);
					break;

				case 2:
					GPIOA->ODR |= (GPIO_PIN_4);
					break;

				case 3:
					GPIOC->ODR |= (GPIO_PIN_1);
					break;

				case 4:
					GPIOC->ODR |= (GPIO_PIN_0);
					break;

				default:
					break;
			}

			// read Rows
			if(GPIOH->IDR & GPIO_PIN_0)
			{
				onRow = 0;
			}
			else if(GPIOH->IDR & GPIO_PIN_1)
			{
				onRow = 1;
			}
			else if(GPIOC->IDR & GPIO_PIN_2)
			{
				onRow = 2;
			}
			else if(GPIOC->IDR & GPIO_PIN_3)
			{
				onRow = 3;
			}

			if(isKeyPressed())
				return mapKeys(onColumn, onRow);
		}
	}
	return -1;
}
