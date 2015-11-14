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
// MSG type message
typedef struct _so_msg {
	char *rid;
	int rid_len;
	char *username;
	int username_len;
	char *text;
	int text_len;
} so_msg;

// other message
typedef struct _so_other {
	char *text;
	int text_len;
} so_other;

//	message
typedef union _so_message {
	so_msg msg;
	so_other other;
} so_message;

// data field
typedef struct _so_data {
	char type[3];
	so_message message;
	int message_len;
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
// total length of members preceding message
#define CONSTANT_LEN	(11+T_LEN)	// 4-cid, 4-seq_num, 2-len, 1-'/'
//-----------------------------------------------------------------------------

// packet handling functions
so_network so_pack(so_packet *p);
so_packet so_unpack(so_network *n);
void so_delete_packet(so_packet *p);
void so_delete_network(so_network *n);
//-----------------------------------------------------------------------------

#endif /* __SO_PROTOCOL_H__ */
