/*
 * rs485.c
 *
 *  Created on: 08 Oct 2017
 *      Author: christo
 */

#include "rs485.h"
#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include <avr/interrupt.h>

#include "hdlc_framer.h"

#define BAUD 9600
#define MYUBBR ((F_CPU / (BAUD * 8UL))-1)

bool dataReady = 0;
mCMsg_t rxMessage;

/*-- Private Functions ------------------------------------*/
void rs485_handleByte(uint8_t byte)
{

    uint8_t len = hdlc_framer_pack(byte);
    if (len)
    {
        if (len == CMSG_LENGTH)
        {
            memcpy(&rxMessage, hdlc_framer_buffer(), CMSG_LENGTH);
            dataReady = true;
        }
    }
}

/*-- Public Functions -------------------------------------*/
void rs485_init()
{
    hdlc_framer_init(16);

    dataReady = false;

    RS485_ENABLE_DDR |= _BV(RS485_ENABLE_PIN);   //output

    //set up the BAUD Rate
    UBRRH = (unsigned char) (MYUBBR >> 8);
    UBRRL = (unsigned char) (MYUBBR);

    //double transmission speed
    UCSRA |= _BV(U2X);
    //set the character size to 8-bit
    UCSRC = _BV(UCSZ0) | _BV(UCSZ1);
    //Enable RX | TX interrupt
    UCSRB = _BV(TXEN) | _BV(RXEN) | _BV(RXCIE) | _BV(TXCIE);

    //disable the enable / listen
    RS485_ENABLE_PORT &= ~_BV(RS485_ENABLE_PIN);
}

void rs485_trasmit_message(mCMsg_t msg)
{
    uint8_t buffer[16];
    uint8_t bufflen = 0;
    mCMsg_t tmsg = msg;

    uint8_t msgBuff[CMSG_LENGTH];
    memcpy(&msgBuff, &tmsg, CMSG_LENGTH);

    hdlc_framer_frame(msgBuff, CMSG_LENGTH, buffer, &bufflen);

    for (uint8_t idx = 0; idx < bufflen; idx++)
        rs485_transmit_byte(buffer[idx]);
}

void rs485_transmit_byte(uint8_t b)
{
    //set transmit enable
    RS485_ENABLE_PORT |= _BV(RS485_ENABLE_PIN);

    //wait until the transmitter is ready
    while (!(UCSRA & (1 << UDRE)))
        ;

    //write the data to the uart buffer
    UDR = b;

    //clear the transfer complete flag
    UCSRA |= (1 << TXC);
}

uint8_t rs485_getData(mCMsg_t *msg)
{
    if (!dataReady)
        return 0;

    *msg = rxMessage;
    dataReady = false;
    return 1;
}

ISR(USART_RX_vect)
{
    rs485_handleByte(UDR);
}

ISR(USART_TX_vect)
{
    RS485_ENABLE_PORT &= ~_BV(RS485_ENABLE_PIN);
}
