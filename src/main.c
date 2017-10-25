/*
 * main.c
 *
 *  Created on: 07 Oct 2017
 *      Author: christo
 */

#include <avr/interrupt.h>

#include "cmsg2.h"
#include "cmsg_handler.h"
#include "pwm.h"

int main( void )
{
	sei();

	cmsg_handler_init();
	pwm_init(0);
	pwm_set(0, 0);

	while(1)
	{
	    cmsg_handler_run();
	}
	return 0;
}
