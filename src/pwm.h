/*
 * pwm.h
 *
 *  Created on: 10 Oct 2017
 *      Author: christo
 */

#ifndef SRC_PWM_H_
#define SRC_PWM_H_

//typedef struct
//{
//	uint8_t invChan;
//	volatile uint8_t *ocra;
//	volatile uint8_t *ocrb;
//}pwm_t;

void pwm_init(uint8_t inv);
void pwm_set(uint8_t chan, uint8_t duty);
uint8_t pwm_get(uint8_t chan);

#endif /* SRC_PWM_H_ */
