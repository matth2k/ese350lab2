

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"

unsigned int count = 70; // 16MHz / 256 / 440Hz / 2


char String[10] = "";

void timer0_init() {
	OCR0A = count; // desired count for 440Hz
	TCCR0A |= 0x42; // set CTC mode
	TCCR0B |= 0x04; // clk/256
	TCNT0 = 0;
	
	sei();
	
}

void led_on() {
	PORTB |= (1 << 5);
}
void led_off() {
	PORTB &= ~(1 << 5);
}

int main(void)
{
	/* Replace with your application code */
	DDRD |= (1 << 6); // set PD6 as output (speaker)

	timer0_init();
	USART_init();

	sprintf(String, "Setup done \n");
	USART_putstring(String);
	
	while(1); // loop
	
}

