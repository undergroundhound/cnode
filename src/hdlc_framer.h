/*
 * hdlc_framer.h
 *
 *  Created on: 22 Oct 2017
 *      Author: christo
 */

#ifndef SRC_HDLC_FRAMER_H_
#define SRC_HDLC_FRAMER_H_

#include <stdint-gcc.h>
#include <avr/io.h>

void hdlc_framer_init(uint8_t frame_len);

void hdlc_framer_frame(uint8_t * payload_ptr,
                        uint8_t payload_length,
                        uint8_t * frame_ptr,
                        uint8_t * frame_length);

uint8_t hdlc_framer_pack(uint8_t byte);

uint8_t* hdlc_framer_buffer();

#endif /* SRC_HDLC_FRAMER_H_ */
