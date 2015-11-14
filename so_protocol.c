/*void so_delete_packet(so_packet *p);
void so_delete_network(so_network *n);
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
	if(p->data.message_len > MESSAGE_LEN)
	{
		n.packet = NULL;
		n.len = FAIL;
		return n;
	}
	n.packet = malloc(CONSTANT_LEN+p->data.message_len);
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
	if(!strncmp(p->data.type, T_MESSAGE, T_LEN))
	{
		strncpy((char*)(n.packet+n.len), p->data.message.msg.rid, p->data.message.msg.rid_len);					// RID
		n.len+=p->data.message.msg.rid_len;
		memcpy(n.packet+n.len++, &slash, sizeof(slash));
		strncpy((char*)(n.packet+n.len), p->data.message.msg.username, p->data.message.msg.username_len);		// USERNAME
		n.len+=p->data.message.msg.username_len;
		memcpy(n.packet+n.len++, &colon, sizeof(colon));
		strncpy((char*)(n.packet+n.len), p->data.message.msg.text, p->data.message.msg.text_len);				// TEXT
		n.len+=p->data.message.msg.text_len;
	}
	else
	{
		strncpy((char*)(n.packet+n.len), p->data.message.other.text, p->data.message.other.text_len);			// TEXT
		n.len+=p->data.message.other.text_len;
	}
	return n;
}
//-----------------------------------------------------------------------------

so_packet so_unpack(so_network *p)
/* Convert network-ready packet to structured version */
{

}
//-----------------------------------------------------------------------------
void so_delete_packet(so_packet *p)
/* Free structured packet memory */
{

}
//-----------------------------------------------------------------------------

void so_delete_network(so_network *n)
/* Free network-ready packet memory */
{
	free(n->packet);
}
//-----------------------------------------------------------------------------
