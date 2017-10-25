/*
 * rs485.h
 *
 *  Created on: 08 Oct 2017
 *      Author: christo
 */

#ifndef RS485_H_
#define RS485_H_

#include "cmsg2.h"

#define RS485_ENABLE_DDR	DDRA
#define RS485_ENABLE_PORT	PORTA
#define RS485_ENABLE_PIN	PA1

void rs485_init();

void rs485_trasmit_message(mCMsg_t msg);
void rs485_transmit_byte(uint8_t b);
uint8_t rs485_getData();


#endif /* RS485_H_ */
