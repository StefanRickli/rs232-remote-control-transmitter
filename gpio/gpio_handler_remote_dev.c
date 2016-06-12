/*
 * gpio_handler_remote_dev.c
 *
 *  Created on: 22.05.2016
 *      Author: stefa
 */

#include <gpio_handler_remote_dev.h>

#include <ascii_comm_functions_remote_dev.h>
#include "config.h"
#include "mac_layer.h"

/**
 * SW1: STOP
 */
void remote_dev_sw1_handler(void)
{
	debug_uart_sendstr(">SW1 STOP\r");
	debug_uart_sendstr(">10\r");

	comm_uart_sendstr("lstop\r");

	struct Packet_t packet;
	packet.type = PLAYER_COMMAND;
	packet.command_no = 0x10;

	mac_lora_tx(BROADCAST_ADDRESS, (char*)(&packet), sizeof(struct Packet_t));
}

/**
 * SW2: START
 */
void remote_dev_sw2_handler(void)
{
	debug_uart_sendstr(">SW2 START\r");
	debug_uart_sendstr(">12\r");

	comm_uart_sendstr("lstart\r");

	struct Packet_t packet;
	packet.type = PLAYER_COMMAND;
	packet.command_no = 0x12;

	mac_lora_tx(BROADCAST_ADDRESS, (char*)(&packet), sizeof(struct Packet_t));
}

/**
 * SW3: READY
 */
void remote_dev_sw3_handler(void)
{
	debug_uart_sendstr(">SW3 READY\r");
	debug_uart_sendstr(">1401\r");

	comm_uart_sendstr("lready\r");

	struct Packet_t packet;
	packet.type = PLAYER_COMMAND;
	packet.command_no = 0x1401;

	mac_lora_tx(BROADCAST_ADDRESS, (char*)(&packet), sizeof(struct Packet_t));

}

/**
 * SW4: PREV
 */
void remote_dev_sw4_handler(void)
{
	debug_uart_sendstr(">SW4 SKIP PREV\r");
	debug_uart_sendstr(">1A01\r");

	comm_uart_sendstr("lprev\r");

	struct Packet_t packet;
	packet.type = PLAYER_COMMAND;
	packet.command_no = 0x1A01;

	mac_lora_tx(BROADCAST_ADDRESS, (char*)(&packet), sizeof(struct Packet_t));
}

/**
 * SW5: NEXT
 */
void remote_dev_sw5_handler(void)
{
	debug_uart_sendstr(">SW5 SKIP NEXT\r");
	debug_uart_sendstr(">1A00\r");

	comm_uart_sendstr("lnext\r");

	struct Packet_t packet;
	packet.type = PLAYER_COMMAND;
	packet.command_no = 0x1A00;

	mac_lora_tx(BROADCAST_ADDRESS, (char*)(&packet), sizeof(struct Packet_t));

}

/**
 * SW6: TRACK
 */
void remote_dev_sw6_handler(void)
{
	debug_uart_sendstr(">SW6 TRACK INFO\r");
	debug_uart_sendstr(">cstm-1\r");

	struct Packet_t packet;
	packet.type = CUSTOM_COMMAND;
	packet.command_no = 0x1;

	mac_lora_tx(BROADCAST_ADDRESS, (char*)(&packet), sizeof(struct Packet_t));
}

/**
 * SW7: STATUS
 */
void remote_dev_sw7_handler(void)
{
	debug_uart_sendstr(">SW7 STATUS\r");
	debug_uart_sendstr(">cstm-0\r");

	struct Packet_t packet;
	packet.type = CUSTOM_COMMAND;
	packet.command_no = 0x0;

	mac_lora_tx(BROADCAST_ADDRESS, (char*)(&packet), sizeof(struct Packet_t));

}
