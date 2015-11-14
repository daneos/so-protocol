/*
 * Systemy Operacyjne - Projekt - SO Protocol
 * Protocol specific stuff
 * Grzegorz Kowalski, 22i
 * version 1 | 11.2015
 */

#include "so_protocol.h"
//-----------------------------------------------------------------------------

so_network so_pack(so_packet *p)
/* Convert structured packet to network-ready version */
{
	so_network n;
	
	int message_len, rid_len, username_len, text_len;
	short is_message = !strncmp(p->data.type, T_MESSAGE, T_LEN);
	if(is_message)
	{
		rid_len = strlen(p->data.message.msg.rid);
		username_len = strlen(p->data.message.msg.username);
		text_len = strlen(p->data.message.msg.text);
		message_len = rid_len + username_len + text_len + 2;
	}
	else
		message_len = strlen(p->data.message.other.text);
	
	if(message_len > MESSAGE_LEN)
	{
		n.packet = NULL;
		n.len = FAIL;
		return n;
	}

	p->len = T_LEN + message_len + 1;
	
	n.packet = malloc(CONSTANT_LEN + p->len);
	n.len = 0;

	uint32_t cid = htonl(p->cid);
	uint32_t seq_num = htonl(p->seq_num);
	uint16_t len = htons(p->len);
	const char slash = '/';
	const char colon = ':';

	memcpy(n.packet+n.len, &cid, sizeof(cid));								// CID
	n.len += sizeof(cid);
	memcpy(n.packet+n.len, &seq_num, sizeof(seq_num));						// SEQ_NUM
	n.len += sizeof(seq_num);
	memcpy(n.packet+n.len, &len, sizeof(len));								// LEN
	n.len += sizeof(len);

	strncpy((char*)(n.packet+n.len), p->data.type, T_LEN);	n.len+=T_LEN;	// TYPE
	memcpy(n.packet+n.len++, &slash, sizeof(slash));
	
	if(is_message)
	{
		strncpy((char*)(n.packet+n.len), p->data.message.msg.rid, rid_len);					// RID
		n.len += rid_len;
		memcpy(n.packet+n.len++, &slash, sizeof(slash));

		strncpy((char*)(n.packet+n.len), p->data.message.msg.username, username_len);		// USERNAME
		n.len += username_len;
		memcpy(n.packet+n.len++, &colon, sizeof(colon));
		
		strncpy((char*)(n.packet+n.len), p->data.message.msg.text, text_len);				// TEXT
		n.len += text_len;
	}
	else
	{
		strncpy((char*)(n.packet+n.len), p->data.message.other.text, text_len);			// TEXT
		n.len += text_len;
	}
	
	return n;
}
//-----------------------------------------------------------------------------

so_packet so_unpack(so_network *n)
/* Convert network-ready packet to structured version */
{
	so_packet p;
	uint32_t cid;
	uint32_t seq_num;
	uint16_t len;
	int current = 0;
	
	memcpy(&cid, n->packet+current, sizeof(cid));							// CID
	current += sizeof(cid);
	memcpy(&seq_num, n->packet+current, sizeof(seq_num));					// SEQ_NUM
	current += sizeof(seq_num);
	memcpy(&len, n->packet+current, sizeof(len));							// LEN
	current += sizeof(len);

	p.cid = ntohl(cid);
	p.seq_num = ntohl(seq_num);
	p.len = ntohs(len);

	strncpy(p.data.type, (char*)(n->packet+current), T_LEN);				// TYPE
	current += T_LEN+1;		// slash after type

	int message_len = 0;
	if(!strncmp(p.data.type, T_MESSAGE, T_LEN))
	{
		int rid_len = 0;
		while(*((char*)n->packet + current + rid_len) != '/') rid_len++;
		p.data.message.msg.rid = (char*)malloc(rid_len+1);
		strncpy(p.data.message.msg.rid, (char*)(n->packet+current), rid_len);				// RID
		p.data.message.msg.rid[rid_len] = '\0';
		current += rid_len+1;	// slash after rid

		int username_len = 0;
		while(*((char*)n->packet + current + username_len) != ':') username_len++;
		p.data.message.msg.username = (char*)malloc(username_len+1);
		strncpy(p.data.message.msg.username, (char*)(n->packet+current), username_len);		// USERNAME
		p.data.message.msg.username[username_len] = '\0';
		current += username_len+1;	// colon after username

		int text_len = p.len - current + CONSTANT_LEN;
		p.data.message.msg.text = (char*)malloc(text_len+1);
		strncpy(p.data.message.msg.text, (char*)(n->packet+current), text_len);				// TEXT
		p.data.message.msg.text[text_len] = '\0';
		current += text_len;

		message_len = rid_len + username_len + text_len + 2;
	}
	else
	{
		int text_len = p.len - current + CONSTANT_LEN;
		p.data.message.other.text = (char*)malloc(text_len+1);
		strncpy(p.data.message.other.text, (char*)(n->packet+current), text_len);			// TEXT
		p.data.message.other.text[text_len] = '\0';
		message_len = text_len;
	}

	return p;
}
//-----------------------------------------------------------------------------

void so_delete_packet(so_packet *p)
/* Free structured packet memory */
{
	if(!strncmp(p->data.type, T_MESSAGE, T_LEN))
	{
		free(p->data.message.msg.rid);
		free(p->data.message.msg.username);
		free(p->data.message.msg.text);
		p->data.message.msg.rid = NULL;
		p->data.message.msg.username = NULL;
		p->data.message.msg.text = NULL;
	}
	else
	{
		free(p->data.message.other.text);
		p->data.message.other.text = NULL;
	}
}
//-----------------------------------------------------------------------------

void so_delete_network(so_network *n)
/* Free network-ready packet memory */
{
	free(n->packet);
	n->packet = NULL;
	n->len = FAIL;
}
//-----------------------------------------------------------------------------
