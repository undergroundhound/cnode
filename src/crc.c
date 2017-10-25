/*
 * crc.c
 *
 *  Created on: 22 Oct 2017
 *      Author: christo
 */

#include "crc.h"

uint8_t crc8_update(uint8_t data, uint8_t crc)
{
    uint8_t i;

    crc = crc ^ data;
    for (i = 0; i < 8; i++)
    {
        if (crc & 0x01)
            crc = (crc >> 1) ^ 0x8C;
        else
            crc >>= 1;
    }

    return crc;
}

uint8_t crc8(uint8_t * data_ptr, uint32_t len)
{
    uint8_t crc = 0;
    while (len--)
    {
        crc = crc8_update(*data_ptr, crc);
        data_ptr++;
    }
    return crc;
}

