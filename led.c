/*
 * led.c
 *
 *  Created on: Oct 16, 2022
 *      Author: Daru
 */

void led_init()
{
	// configure LED PINS
	GPIOC->MODER &= ~(GPIO_MODER_MODE9);
	GPIOC->MODER |= (GPIO_MODER_MODE9_0);

	GPIOC->MODER &= ~(GPIO_MODER_MODE8);
	GPIOC->MODER |= (GPIO_MODER_MODE8_0);

	GPIOB->MODER &= ~(GPIO_MODER_MODE8);
	GPIOB->MODER |= (GPIO_MODER_MODE8_0);

	GPIOC->MODER &= ~(GPIO_MODER_MODE6);
	GPIOC->MODER |= (GPIO_MODER_MODE6_0);
}

// Note: if used all GPIOC PINS, could of set the bits in BSRR instead of a manual switch code
//			=> didnt due to board's pin layout (only 3 were next to each other, then the last is another GPIO)
//			=> and how I wanted them next to each other for easy pin setup
// i.e. GPIOC->BSRR |= (readNum & 0xFFFF0000);	// mask reset @ upper 16-bits of BSRR
// Decodes keypad_read() returns into LEDs STM Pins: LED3, 2, 1, 0's place 2^#
void decodeLED(uint8_t readNum)
{
	// reset LEDs
	GPIOC->BRR = (GPIO_PIN_9);
	GPIOC->BRR = (GPIO_PIN_8);
	GPIOB->BRR = (GPIO_PIN_8);
	GPIOC->BRR = (GPIO_PIN_6);

	switch(readNum)
	{
		case 1:
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 2:
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 3:
			GPIOC->BSRR |= (GPIO_PIN_8);
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 4:
			GPIOB->BSRR |= (GPIO_PIN_8);
			break;

		case 5:
			GPIOB->BSRR |= (GPIO_PIN_8);
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 6:
			GPIOB->BSRR |= (GPIO_PIN_8);
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 7:
			GPIOB->BSRR |= (GPIO_PIN_8);
			GPIOC->BSRR |= (GPIO_PIN_8);
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 8:
			GPIOC->BSRR |= (GPIO_PIN_6);
			break;

		case 9:
			GPIOC->BSRR |= (GPIO_PIN_6);
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 10:
			GPIOC->BSRR |= (GPIO_PIN_6);
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 11:
			GPIOC->BSRR |= (GPIO_PIN_6);
			GPIOC->BSRR |= (GPIO_PIN_8);
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 12:
			GPIOC->BSRR |= (GPIO_PIN_6);
			GPIOB->BSRR |= (GPIO_PIN_8);
			break;

		case 13:
			GPIOC->BSRR |= (GPIO_PIN_6);
			GPIOB->BSRR |= (GPIO_PIN_8);
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 14:
			GPIOC->BSRR |= (GPIO_PIN_6);
			GPIOB->BSRR |= (GPIO_PIN_8);
			GPIOC->BSRR |= (GPIO_PIN_8);
			break;

		case 15:
			GPIOC->BSRR |= (GPIO_PIN_6);
			GPIOB->BSRR |= (GPIO_PIN_8);
			GPIOC->BSRR |= (GPIO_PIN_8);
			GPIOC->BSRR |= (GPIO_PIN_9);
			break;

		case 0:
			GPIOC->BRR = (GPIO_PIN_9);
			GPIOC->BRR = (GPIO_PIN_8);
			GPIOB->BRR = (GPIO_PIN_8);
			GPIOC->BRR = (GPIO_PIN_6);
			break;

		default:
			break;
	}
}

