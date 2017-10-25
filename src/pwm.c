/*
 * pwm.c
 *
 *  Created on: 10 Oct 2017
 *      Author: christo
 */

#include <avr/io.h>

#include "pwm.h"


void pwm_init(uint8_t inv)
{
	/* set PD5 & PD6 as output */
	DDRD |= _BV(5);
	DDRB |= _BV(2);

	if (inv == 1)
	{
		// inverting mode A & B
		TCCR0A |= _BV(COM0A1);
		TCCR0A |= _BV(COM0B1);
	} else
	{
		// non-inverting mode A & B
		TCCR0A |= _BV(COM0A1) | _BV(COM0A0);
		TCCR0A |= _BV(COM0B1) | _BV(COM0B0);
	}

	// set Waveform as phase correct PWM / 8 bit
	TCCR0A |=_BV(WGM00);
	// set the prescaler as clk/64
	TCCR0B |= _BV(CS01);
}

void pwm_set(uint8_t chan, uint8_t duty)
{
	if(chan > 1)
		return;

	if(chan == 0)
		OCR0A = duty;
	else
		OCR0B = duty;
}


uint8_t pwm_get(uint8_t chan)
{
	if(chan > 1)
		return 0;

	if(chan == 0)
		return OCR0A;

	return OCR0B;
}
