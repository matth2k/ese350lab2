

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"


char String[10] = "";

void timer1_init() {
	TCCR1B |= (1 << 0); // no prescaler
	TCCR1B |= (1 << 6); // set input capture on neg edge
	TIMSK1 |= (1 << 5); // enable interrupt on icp (TIMER1_CAPT_vect)
	TCNT1 = 0;
	
	sei();
}

void led_on() {
	PORTB |= (1 << 5);
}
void led_off() {
	PORTB &= ~(1 << 5);
}

void led_toggle() {
	PORTB ^= (1 << 5);
}

ISR (TIMER1_CAPT_vect) {
	// ICF1 flag should be cleared automatically
	led_toggle();
}

int main(void)
{
    /* Replace with your application code */
    DDRB |= (1 << 5); // set PB5 as output (led)
	DDRB &= ~(1 << 0); // set PB0 as input
	timer1_init();
	USART_init();
	led_off();
	sprintf(String, "Setup done \n");
	USART_putstring(String);
	
	while(1); // loop
	
}

