#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include <msp430.h>
#include <driverlib.h>

#include <config.h>
#include <event.h>
#include <transmit.h>
#include <ringbuffer.h>
#include <timers.h>

#include <main_handlers_remote_dev.h>
#include <gpio_low_level_remote_dev.h>
#include <gpio_handler_remote_dev.h>
#include <states_remote_dev.h>
#include <ascii_comm_functions_remote_dev.h>

/*
 * Initialize the clock
 *
 * DCO set at 1 MHz
 *
 * MCLK and SMCLK set at 8 MHz and sources from DCO without div /1, and thus run at 1 MHz
 * ACLK is sources from LFXT (32 kHz) with div / 1
 */
static void init_clock(void) {
	// Set the DCO at 1 MHz
	CSCTL0 = CSKEY;
	CSCTL1 = DCOFSEL_0;
	CSCTL2 = SELM__DCOCLK | SELS__DCOCLK | SELA__LFXTCLK;
	CSCTL3 = DIVM__1 | DIVS__1 | DIVA__1;
}

#define MAX_CMD_ACK_BACKOFF 4095	// 1s - MAX random backoff for cmd ack



// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
void main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

	init_clock();
	event_init();
	timers_init();
	srand(NODE_ADDRESS);

	debug_uart_init(debug_uart_cmdline_handler);
	comm_uart_init(comm_uart_cmdline_handler);

	gpio_init_remote_dev(remote_dev_sw1_handler, remote_dev_sw2_handler,
			remote_dev_sw3_handler, remote_dev_sw4_handler,
			remote_dev_sw5_handler, remote_dev_sw6_handler,
			remote_dev_sw7_handler);

	lora_init();



	// Enabling global interrupts
	_EINT();
	debug_uart_sendstr("Remote Dev \r\n");
	// Starting the event loop
	event_loop();
}
