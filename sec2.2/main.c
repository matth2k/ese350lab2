#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"

#define PULSES 4
#define DIV 8
#define FCPU 16000000
#define FTIM FCPU / DIV
#define FTRIG 100000


unsigned timeHigh = (FTIM / FTRIG); // high time of the pulses to be created
unsigned timeLow = (FTIM / FTRIG) * 19 * 100; // low time of the pulses to be created

// number of pulses to be created
char high; // flag to choose

volatile unsigned int pos;
volatile unsigned int neg;
volatile unsigned int dist;

char String[32] = "";

void timer0_init() {
	TCCR0B |= 0x01; // clk/1
	TCNT0 = 0;
}

void timer1_config() {
	DDRB |= 0x02; // set PB1 to output for OC
	TCCR1A |= 0x40; // set output compare flag to toggle mode
	TCCR1B |= 0x02; // clk/8, 2MHz

	OCR1A = TCNT1 + 16; // set PB1 soon
}

void trigger_start() {
	
	timer1_config();
	high = 0;
	TIMSK1 |= 0x02; // enable COMPA ISR

	sei();
}

ISR (TIMER1_COMPA_vect) {
	if (high) {
		OCR1A = TCNT1 + timeLow;
		high = 0;
		// enable Input Compare on pos edge
		TIMSK1 |= (1 << 5);
		TCCR1B |= (1 << 6);
	} else {
		OCR1A = TCNT1 + timeHigh;
		high = 1;
	}


}

ISR (TIMER1_CAPT_vect) {
	if (TCCR1B & (1 << 6)) {
		TCCR1B &= ~(1 << 6);
		TCNT1 = 0;
		pos = 0;
		} else {
		
		neg = TCNT1;
		dist = neg - pos;
		high = 0;
		TIMSK1 &= ~(1 << 5);
		TIMSK1 |= 0x02; // enable COMPA ISR
		TCCR1A |= 0x40; // enable toggle
		OCR1A = TCNT1 + 16;
	}
}

int main(void) {
	USART_init();
	trigger_start();
	sprintf(String, "Setup done!\n");
	USART_putstring(String);
	while (1) {
		sprintf(String, "%d, OC: %d ICAPT: %d, RSE: %d \n", dist, (TIMSK1 & 0x02) >> 1, (TIMSK1 & (1 << 5) >> 5), (TCCR1B && (1 << 6)) >> 6);
		USART_putstring(String);
	}
}