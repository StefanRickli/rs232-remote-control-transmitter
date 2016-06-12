/*
 * gpio_use.c
 *
 *  Created on: 20.05.2016
 *      Author: Stefan Rickli
 */

#include "gpio_low_level_remote_dev.h"
#include <msp430.h>
#include "driverlib.h"
#include "event.h"

// TODO maybe remove wurx from a later version
#include "wurx.h"

// -----------------------------------------------------------------------------
// GPIO init
// -----------------------------------------------------------------------------
// RIC: PINASSIGN
#define SW1_PORT	GPIO_PORT_P4
#define SW1_PIN		GPIO_PIN5
#define SW2_PORT	GPIO_PORT_P1
#define SW2_PIN		GPIO_PIN1
#define SW3_PORT	GPIO_PORT_P3
#define SW3_PIN		GPIO_PIN4
#define SW4_PORT	GPIO_PORT_P3
#define SW4_PIN		GPIO_PIN5
#define SW5_PORT	GPIO_PORT_P3
#define SW5_PIN		GPIO_PIN6
/*
#define SW6_PORT	GPIO_PORT_P1
#define SW6_PIN		GPIO_PIN3
#define SW7_PORT	GPIO_PORT_P1
#define SW7_PIN		GPIO_PIN4
*/

#define LED1_PORT	GPIO_PORT_P4
#define LED1_PIN	GPIO_PIN6
#define LED2_PORT	GPIO_PORT_P1
#define LED2_PIN	GPIO_PIN0
#define LED3_PORT	GPIO_PORT_P1
#define LED3_PIN	GPIO_PIN2

// Event IDs associated with the buttons
static uint16_t _sw1_event;
static uint16_t _sw2_event;
uint16_t _sw3_event;
uint16_t _sw4_event;
uint16_t _sw5_event;
/*
static uint16_t _sw6_event;
static uint16_t _sw7_event;
*/

// -----------------------------------------------------------------------------
// GPIO init
// -----------------------------------------------------------------------------

void gpio_init_remote_dev(event_handler_t sw1_handler, event_handler_t sw2_handler, event_handler_t sw3_handler, event_handler_t sw4_handler, event_handler_t sw5_handler, event_handler_t sw6_handler, event_handler_t sw7_handler) {
	// Switch init
	GPIO_setAsInputPinWithPullUpResistor(SW1_PORT, SW1_PIN);
	GPIO_setAsInputPinWithPullUpResistor(SW2_PORT, SW2_PIN);
	GPIO_setAsInputPinWithPullUpResistor(SW3_PORT, SW3_PIN);
	GPIO_setAsInputPinWithPullUpResistor(SW4_PORT, SW4_PIN);
	GPIO_setAsInputPinWithPullUpResistor(SW5_PORT, SW5_PIN);
	/*
	GPIO_setAsInputPinWithPullUpResistor(SW6_PORT, SW6_PIN);
	GPIO_setAsInputPinWithPullUpResistor(SW7_PORT, SW7_PIN);
	*/
	GPIO_selectInterruptEdge(SW1_PORT, SW1_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_selectInterruptEdge(SW2_PORT, SW2_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_selectInterruptEdge(SW3_PORT, SW3_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_selectInterruptEdge(SW4_PORT, SW4_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_selectInterruptEdge(SW5_PORT, SW5_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	/*
	GPIO_selectInterruptEdge(SW6_PORT, SW6_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_selectInterruptEdge(SW7_PORT, SW7_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	*/
	GPIO_clearInterrupt(SW1_PORT, SW1_PIN);
	GPIO_clearInterrupt(SW2_PORT, SW2_PIN);
	GPIO_clearInterrupt(SW3_PORT, SW3_PIN);
	GPIO_clearInterrupt(SW4_PORT, SW4_PIN);
	GPIO_clearInterrupt(SW5_PORT, SW5_PIN);
	/*
	GPIO_clearInterrupt(SW6_PORT, SW6_PIN);
	GPIO_clearInterrupt(SW7_PORT, SW7_PIN);
	*/
	GPIO_enableInterrupt(SW1_PORT, SW1_PIN);
	GPIO_enableInterrupt(SW2_PORT, SW2_PIN);
	GPIO_enableInterrupt(SW3_PORT, SW3_PIN);
	GPIO_enableInterrupt(SW4_PORT, SW4_PIN);
	GPIO_enableInterrupt(SW5_PORT, SW5_PIN);
	/*
	GPIO_enableInterrupt(SW6_PORT, SW6_PIN);
	GPIO_enableInterrupt(SW7_PORT, SW7_PIN);
	*/

	_sw1_event = event_add(sw1_handler);
	_sw2_event = event_add(sw2_handler);
	_sw3_event = event_add(sw3_handler);
	_sw4_event = event_add(sw4_handler);
	_sw5_event = event_add(sw5_handler);
	/*
	_sw6_event = event_add(sw6_handler);
	_sw7_event = event_add(sw7_handler);
	*/

	// LED init
	GPIO_setAsOutputPin(LED1_PORT, LED1_PIN);
	GPIO_setAsOutputPin(LED2_PORT, LED2_PIN);
	GPIO_setAsOutputPin(LED3_PORT, LED3_PIN);

	GPIO_setOutputLowOnPin(LED1_PORT, LED1_PIN);
	GPIO_setOutputLowOnPin(LED2_PORT, LED2_PIN);
	GPIO_setOutputLowOnPin(LED3_PORT, LED3_PIN);
}

// -----------------------------------------------------------------------------
// LED functions
// -----------------------------------------------------------------------------

/**
 * LED1 on: STOP
 */
inline void led1_on(void) {
	GPIO_setOutputHighOnPin(LED1_PORT, LED1_PIN);
}

/**
 * LED1 off: STOP
 */
inline void led1_off(void) {
	GPIO_setOutputLowOnPin(LED1_PORT, LED1_PIN);
}

/**
 * LED1 on: START
 */
inline void led2_on(void) {
	GPIO_setOutputHighOnPin(LED2_PORT, LED2_PIN);
}

/**
 * LED2 off: START
 */
inline void led2_off(void) {
	GPIO_setOutputLowOnPin(LED2_PORT, LED2_PIN);
}

/**
 * LED3 on: READY
 */
inline void led3_on(void) {
	GPIO_setOutputHighOnPin(LED3_PORT, LED3_PIN);
}

/**
 * LED3 off: READY
 */
inline void led3_off(void) {
	GPIO_setOutputLowOnPin(LED3_PORT, LED3_PIN);
}

// -----------------------------------------------------------------------------
// GPIO ISRs
// -----------------------------------------------------------------------------

#pragma vector=PORT1_VECTOR
__interrupt void isr_gpio_p1(void)
{
	switch (__even_in_range(P1IV, P1IV_P1IFG7)) {
	case P1IV_NONE:
		break;
	case P1IV_P1IFG0:
		break;
	case P1IV_P1IFG1:
		__delay_cycles(200);
		while(!(P1IN & BIT1));
		__delay_cycles(200);
		P1IFG &= ~BIT1;
		EVENT_SIGNAL_ISR(_sw2_event);
		break;
	case P1IV_P1IFG2:
		break;
	case P1IV_P1IFG3:
		/*
		__delay_cycles(200);
		while(!(P1IN & BIT3));
		__delay_cycles(200);
		P1IFG &= ~BIT3;
		EVENT_SIGNAL_ISR(_sw6_event);
		*/
		break;
	case P1IV_P1IFG4:
		/*
		__delay_cycles(200);
		while(!(P1IN & BIT4));
		__delay_cycles(200);
		P1IFG &= ~BIT4;
		EVENT_SIGNAL_ISR(_sw7_event);
		*/
		break;
	case P1IV_P1IFG5:
		EVENT_SIGNAL_ISR(_wurx_ev);
		P1IFG &= ~BIT5;
		break;
	case P1IV_P1IFG6:
		break;
	case P1IV_P1IFG7:
		break;
	}
}

/**
 * P3 ISR has been moved to sx1276_driver.c
 */

#pragma vector=PORT4_VECTOR
__interrupt void isr_gpio_p4(void)
{
	switch (__even_in_range(P4IV, P4IV_P4IFG7)) {
	case P4IV_NONE:
		break;
	case P4IV_P4IFG0:
		break;
	case P4IV_P4IFG1:
		break;
	case P4IV_P4IFG2:
		break;
	case P4IV_P4IFG3:
		break;
	case P4IV_P4IFG4:
		break;
	case P4IV_P4IFG5:
		__delay_cycles(200);
		while(!(P4IN & BIT5));
		__delay_cycles(200);
		P4IFG &= ~BIT5;
		EVENT_SIGNAL_ISR(_sw1_event);
		break;
	case P4IV_P4IFG6:
		break;
	case P4IV_P4IFG7:
		break;
	}
}
