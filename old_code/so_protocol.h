/*
 * Systemy Operacyjne - Projekt - SO Protocol
 * Protocol specific stuff
 * Grzegorz Kowalski, 22i
 * version 1 | 12.2015
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
	us_addrinfo address;
} so_packet;

// packet, in a ready to send form
typedef struct _so_network {
	void *packet;
	size_t len;
	us_addrinfo address;
} so_network;

// // output packet queue
// typedef struct _qe {
// 	so_packet *p;
// 	struct _qe *next;
// } qe;

// // random connection data
// typedef struct _rcd {
// 	uint32_t current_seq;
// 	qe *head;
// 	qe *current;
// 	qe *tail;
// } rcd;

// // low-level connection structure
// typedef struct _connection {
// 	int sock;
// 	us_sockaddr address;
// 	uint32_t cid;
// 	pid_t process;
// 	rcd data;
// 	cflags flags;
// } connection;

// child process info
typedef struct _child {
	pid_t pid;
	uint32_t cid;	
} child;
//-----------------------------------------------------------------------------

// message types
#define T_INIT			"INI"
#define T_MESSAGE		"MSG"
#define T_DATA			"DAT"		// any text data not conforming to MSG specification, added for math server implementation 
#define T_ACK			"ACK"
#define T_RETRY			"RET"
#define T_RESET			"RST"
#define T_END			"END"

// message types for switch statement
#define ST_INIT			0
#define ST_MESSAGE		1
#define ST_DATA			2
#define ST_ACK			3
#define ST_RETRY		4
#define ST_RESET		5
#define ST_END			6

// type length
#define T_LEN			3
// message length
#define MESSAGE_LEN		500
// total length of members preceding data
#define CONSTANT_LEN	(2*sizeof(uint32_t) + sizeof(uint16_t))

// useful macros
#define IS_TYPE(p,t)	(!strncmp((p)->data.type, t, T_LEN))
#define SET_TYPE(p,t)	(strncpy((p)->data.type, t, T_LEN))

// child process management
#define ALLOCATION_STEP	100
//-----------------------------------------------------------------------------

// packet handling functions
so_network so_pack(so_packet *p);
so_packet so_unpack(so_network *n);
void so_delete_packet(so_packet *p);
void so_delete_network(so_network *n);
void so_debug_print(so_packet *p);
int switchtype(so_packet *p);
int create_socket(const char *name, const char* port, us_addrinfo **addr, short if_bind);
//-----------------------------------------------------------------------------

#endif /* __SO_PROTOCOL_H__ */
