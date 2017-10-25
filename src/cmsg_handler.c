/*
 * cmsg_handler.c
 *
 *  Created on: 25 Oct 2017
 *      Author: christo
 */

#include "cmsg2.h"
#include "cmsg_handler.h"
#include "rs485.h"
#include "nvm.h"
#include "pwm.h"

mCMsg_t handlerMsg;

void cmsg_handler_init()
{
    rs485_init();
    handlerMsg.deviceId = nvm_id;
}

void cmsg_handler_run()
{
    /* no data available */
    if(!rs485_getData(&handlerMsg))
        return;

    /* not my message */
    if(handlerMsg.deviceId != nvm_id)
        return;

    switch (handlerMsg.tag) {
        case CMSG_TAG_PING:
        {
            handlerMsg.tag = CMSG_TYPE_SET;
            handlerMsg.data0 = 0xFF;
            handlerMsg.data1 = 0xFF;
            rs485_trasmit_message(handlerMsg);
//            handlerMsg = ;
        }
            break;
        case CMSG_TAG_PWM:
        {
            pwm_set(handlerMsg.data0, handlerMsg.data1);
//            handlerMsg.tag = CMSG_TYPE_SET;
//            handlerMsg.data0 = 0xFF;
//            handlerMsg.data1 = 0xFF;
//            rs485_trasmit_message(handlerMsg);
        }
                    break;
        default:
            break;
    }
}
