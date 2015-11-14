/*
 * Systemy Operacyjne - Projekt - SO Protocol
 * Protocol specific stuff
 * Grzegorz Kowalski, 22i
 * version 1 | 11.2015
 */

#if !defined(__SO_PROTOCOL_H__)
#	define __SO_PROTOCOL_H__
//-----------------------------------------------------------------------------

#include "utils.h"
//-----------------------------------------------------------------------------

// data structures
// message
typedef struct _so_message {
	char *rid;
	char *username;
	char *text;
} so_message;

// data field
typedef struct _so_data {
	char type[3];
	so_message message;
} so_data;

// full packet data
typedef struct _so_packet {
	uint32_t cid;
	uint32_t seq_num;
	uint16_t len;
	so_data data;
} so_packet;

// packet, in a ready to send form
typedef struct _so_network {
	void *packet;
	size_t len;
} so_network;
//-----------------------------------------------------------------------------

// message types
#define T_INIT			"INI"
#define T_MESSAGE		"MSG"
#define T_ACK			"ACK"
#define T_RETRY			"RET"
#define T_RESET			"RST"
#define T_END			"END"
// type length
#define T_LEN			3
// message length
#define MESSAGE_LEN		500
// total length of members preceding data
#define CONSTANT_LEN	(2*sizeof(uint32_t) + sizeof(uint16_t))
//-----------------------------------------------------------------------------

// packet handling functions
so_network so_pack(so_packet *p);
so_packet so_unpack(so_network *n);
void so_delete_packet(so_packet *p);
void so_delete_network(so_network *n);
//-----------------------------------------------------------------------------

#endif /* __SO_PROTOCOL_H__ */
