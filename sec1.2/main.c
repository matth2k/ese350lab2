

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"

unsigned int count = 70; // 16MHz / 256 / 440Hz / 2


char String[10] = "";

void timer0_init() {
	TCCR0B |= 0x04; // clk/256
	TCCR0A |= 0x40; // toggle output compare1 flag
	TIMSK0 |= (1 << 1); // enable interrupt on out compare1 (TIMER1_CAPT_vect)
	TCNT0 = 0;
	
	sei();
}

void led_on() {
	PORTB |= (1 << 5);
}
void led_off() {
	PORTB &= ~(1 << 5);
}


ISR (TIMER0_COMPA_vect) {
	
	OCR0A = TCNT0 + count;
}

int main(void)
{
	/* Replace with your application code */
	DDRD |= (1 << 6); // set PD6 as output (speaker)
	OCR0A = count;
	timer0_init();
	USART_init();

	sprintf(String, "Setup done \n");
	USART_putstring(String);
	
	while(1); // loop
	
}

