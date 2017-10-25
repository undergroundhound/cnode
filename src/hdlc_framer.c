/*
 * hdlc_framer.c
 *
 *  Created on: 22 Oct 2017
 *      Author: christo
 */

#include "hdlc_framer.h"

#include <stdbool.h>
#include <string.h>

#include "crc.h"

uint8_t mBuffer[16];
uint8_t mLength;
uint8_t mFrameIdx;
bool mValidFrame;
bool mEscapeChar;

const uint8_t escapedChars[] =
{ 0x7E, 0x7D, 0x08, 0x7F };

void hldc_framer_addByte(uint8_t byte, uint8_t * frame_ptr,
        uint8_t * frame_length)
{
    bool to_escape = false;

    for (uint8_t k = 0; k < sizeof(escapedChars); k++)
    {
        if (byte == escapedChars[k])
        {
            to_escape = true;
        }
    }

    if (to_escape == true)
    {
        // Insert escape sequence
        frame_ptr[*frame_length] = 0x7D;
        (*frame_length)++;
        frame_ptr[*frame_length] = 0x20 ^ byte;
        (*frame_length)++;
    }
    else
    {
        frame_ptr[*frame_length] = byte;
        (*frame_length)++;
    }
}

void hdlc_framer_init(uint8_t buff_len)
{
    memset(&mBuffer, 0x00, 16);
//    mBuffer = (uint8_t*) malloc(buff_len);
    mLength = buff_len;
    mValidFrame = false;
    mEscapeChar = false;
    mFrameIdx = 0;
}

void hdlc_framer_frame(uint8_t * payload_ptr, uint8_t payload_length,
        uint8_t * frame_ptr, uint8_t * frame_length)
{
    // Perform CRC on payload
    uint8_t crc;
    crc = crc8(payload_ptr, payload_length);
//    crc ^= 0xFF;

// Opening flag
    frame_ptr[0] = 0x7E;
    *frame_length = 1;

    // Add each byte of the payload
    for (uint8_t cntr = 0; cntr < payload_length; cntr++)
        hldc_framer_addByte(*payload_ptr++, frame_ptr, frame_length);

    // Add FCS and flag bytes
    hldc_framer_addByte(crc, frame_ptr, frame_length);
//    hldc_framer_addByte((crc >> 8) & 0xFF, frame_ptr, frame_length);

// Closing flag
    frame_ptr[*frame_length] = 0x7E;
    (*frame_length) += 1;
}

uint8_t hdlc_framer_pack(uint8_t byte)
{
    uint8_t packedFrame = 0;

    if (!mValidFrame && mFrameIdx == 0)  //first byte
    {
        if (byte == 0x7E)  //frame start
            mValidFrame = true;

    }
    else //middle or last bytes
    {
        if (byte == 0x7E) // && var->frameLen > 2)  //frame end
        {
            if (mFrameIdx < 3) //start of a new frame
                return 0;

            //diag_printf("Data received %d\n",frameLen);
            uint16_t calc_crc = crc8(mBuffer, mFrameIdx);

            if (calc_crc == 0)
            {
                //dataFrame Received
                packedFrame = mFrameIdx - 1;
            }
            else
            {
                mFrameIdx = 0;
                mValidFrame = false;
                mEscapeChar = false;

                return -1;
            }
            mFrameIdx = 0;
            mValidFrame = false;
            mEscapeChar = false;
        }
        else if (byte == 0x7D)
        {
            mEscapeChar = true;  //escape next byte
        }
        else if (mEscapeChar)
        {
            mBuffer[mFrameIdx++] = byte ^ 0x20; //xor to get original byte
            mEscapeChar = false;
        }
        else
        {
            mBuffer[mFrameIdx++] = byte;

            if (mFrameIdx >= mLength)
            {
                mFrameIdx = 0;
                mValidFrame = false;
            }
        }
    }

    return packedFrame;
}

uint8_t* hdlc_framer_buffer()
{
    return mBuffer;
}
