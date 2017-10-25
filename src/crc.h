/*
 * crc.h
 *
 *  Created on: 22 Oct 2017
 *      Author: christo
 */

#ifndef SRC_CRC_H_
#define SRC_CRC_H_

#include <stdint-gcc.h>

uint8_t crc8(uint8_t * data, uint32_t len);
uint8_t crc8_update(uint8_t data, uint8_t crc);

#endif /* SRC_CRC_H_ */
