/*
 * gpio_use.h
 *
 *  Created on: 20.05.2016
 *      Author: stefa
 */

#ifndef GPIO_GPIO_LOW_LEVEL_REMOTE_DEV_H_
#define GPIO_GPIO_LOW_LEVEL_REMOTE_DEV_H_

#include "event.h"

// -----------------------------------------------------------------------------
// GPIO init
// -----------------------------------------------------------------------------

void gpio_init_remote_dev(event_handler_t, event_handler_t, event_handler_t, event_handler_t, event_handler_t, event_handler_t, event_handler_t);


// -----------------------------------------------------------------------------
// LED functions
// -----------------------------------------------------------------------------

void led1_on(void);

void led1_off(void);

void led2_on(void);

void led2_off(void);

void led3_on(void);

void led3_off(void);

// for use in sx1276_driver.c, P3-ISR
extern uint16_t _sw3_event;
extern uint16_t _sw4_event;
extern uint16_t _sw5_event;


#endif /* GPIO_GPIO_LOW_LEVEL_REMOTE_DEV_H_ */
