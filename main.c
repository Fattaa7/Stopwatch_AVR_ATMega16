/*
 * main.c
 *
 *  Created on: Jan 31, 2021
 *      Author: ahmed
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

char sec_1 = 0;
char sec_2 = 0;
char min_1 = 0;
char min_2 = 0;
char hr_1 = 0;
char hr_2 = 0;

void TIMER1_INIT_CTC(void);

ISR(INT0_vect) { // Reset

	sec_1 = 0;
	sec_2 = 0;
	min_1 = 0;
	min_2 = 0;
	hr_1 = 0;
	hr_2 = 0;
	TIMER1_INIT_CTC();

}

ISR(INT1_vect) { // Pause
	TCCR1A = 0;
	TCCR1B = 0;
}

ISR(INT2_vect) { // Resume

	TCCR1A |= (1 << COM1A0) | (1 << FOC1A) | (1 << FOC1B);
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

}

ISR(TIMER1_COMPA_vect) {

	if (sec_1 == 9) {
		sec_1 = 0;

		if (sec_2 == 5) {
			sec_2 = 0;

			if (min_1 == 9) {
				min_1 = 0;

				if (min_2 == 5) {
					min_2 = 0;

					if (hr_1 == 9) {
						hr_1 = 0;
						hr_2++;

					} else {
						hr_1++;
					}
				} else {
					min_2++;
				}
			} else {
				min_1++;
			}
		} else {
			sec_2++;
		}
	} else {
		sec_1++;
	}

}

void TIMER1_INIT_CTC(void) {

	SREG |= (1 << 7);
	TCCR1A |= (1 << COM1A0) | (1 << FOC1A) | (1 << FOC1B);
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	TCNT1 = 0; /* Initial Value of Timer is 0 */
	OCR1A = 977; // Compare Value = 977
	TIMSK |= (1 << OCIE1A); // Enable Timer int

}

void SEVEN_SEG_INIT(void) {

	DDRC |= 0b1111;
	PORTC &= ~(0b1111);

	DDRA |= 0b111111;
	PORTA &= ~(0b111111);

}

void SWITCH_INIT(void) {

	DDRD &= ~(1 << PD3) | ~(1 << PD2);
	PORTD |= (1 << PD2);

	DDRB &= (1 << PB2);
	PORTB |= (1 << PB2);

	MCUCR |= (1 << ISC11) | (1 << ISC10) | (1 << ISC01);
	MCUCR &= ~(1 << ISC00);

	MCUCSR |= (1 << ISC2);

	GICR |= (1 << INT0) | (1 << INT1) | (1 << INT2);

}

int main(void) {

	TIMER1_INIT_CTC();
	SEVEN_SEG_INIT();
	SWITCH_INIT();

	while (1) {

		PORTA = (1 << PA5);
		PORTC = sec_1;

		_delay_ms(5);

		PORTA = (1 << PA4);
		PORTC = sec_2;

		_delay_ms(5);

		PORTA = (1 << PA3);
		PORTC = min_1;

		_delay_ms(5);

		PORTA = (1 << PA2);
		PORTC = min_2;

		_delay_ms(5);

		PORTA = (1 << PA1);
		PORTC = hr_1;

		_delay_ms(5);

		PORTA = (1 << PA0);
		PORTC = hr_2;

		_delay_ms(5);

	}

}
