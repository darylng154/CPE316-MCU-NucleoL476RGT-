/*
 * UART.h
 *
 *  Created on: Oct 23, 2022
 *      Author: Daru
 */

#ifndef INC_UART_H_
#define INC_UART_H_

//#define STRLEN 8	// UART char length range: 7-9
#define BAUD 34		//baud rate for 115.2 kpbs
//#define BAUD 416		//baud rate for 9600 kpbs

extern uint8_t aChar;

// initializes GPIO pins relating to USART2
void GPIO_init();

// initializes & enable global interrupt
void INTERRUPT_init();

// initializes USART2
void UART_init();

// print a C character
void UART_print(uint8_t character);

// interrupt handler
void TIM2_IRQHandler(void);

#endif /* INC_UART_H_ */
