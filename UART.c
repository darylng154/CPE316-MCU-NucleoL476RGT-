/*
 * UART.c
 *
 *  Created on: Oct 23, 2022
 *      Author: Daru
 *
 *	TX = PA2 = output/transmitter	RX = PA3 = input/receiver
 *	PA8 = TX test pin		PA10 = RX test pin
 */

#include "main.h"
#include <stdio.h>
#include "UART.h"

// initializes GPIO pins relating to USART2
void GPIO_init()
{
	// enable clock on GPIOA
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	// configure TX = PA2 = 10	RX = PA3 = 10
	GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	// set to alt function
	GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);

	// USART Alt Function 7 pins configuration
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2_Pos | GPIO_AFRL_AFSEL3_Pos);
	// enable AF7 pins:
	GPIOA->AFR[0] |= ((7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos));
}

// initializes & enable global interrupt
void INTERRUPT_init()
{
	// enable USART2 interrupt vector in vector table
	NVIC->ISER[1] |= (1 << (USART2_IRQn & 0x1F));	// shift 6 times
	//Note: mask 0x1F dude to indexing into ISER[1]

	// enable USART interrupts for RX
	USART2->CR1 |= (USART_CR1_RXNEIE);

	__enable_irq();		// enable global interrupt
}

// initializes USART2
void UART_init()
{
	// initializes GPIO pins relating to USART2
	GPIO_init();

	// enable clock for USART2
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	// configure USART2:

	// set word length 			NOTE: can only be config'ed if UE = 0 (Disabled)
	// 1 Start bit, 8 data bits, n stop bits = [00]
	USART2->CR1 &= ~( USART2->CR1 & (USART_CR1_M1 | USART_CR1_M0));

	// set stop bit = 1 bit	= [00]	NOTE: can only be config'ed if UE = 0 (Disabled)
	USART2->CR2 &= ~(USART_CR2_STOP);

	USART2->BRR = BAUD;

	// initializes & enable USART2 & global interrupt
	INTERRUPT_init();

	USART2->CR1 |= USART_CR1_UE;		// enable device (UE)
	// enable USART2 RX, TX
	USART2->CR1 |= (USART_CR1_RE | USART_CR1_TE);
}

// print a C character
void UART_print(uint8_t character)
{
	// busy loop to check if there is data to transfer by TX flag = 1
	while( !(USART2->ISR & USART_ISR_TXE) );

	USART2->TDR = character;
}

// interrupt handler
void USART2_IRQHandler(void)
{
	// check if there is data to read by RX flag = 1
	if(USART2->ISR & USART_ISR_RXNE)
	{
		aChar = USART2->RDR;
	}
}
