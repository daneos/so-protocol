/*
 * Systemy Operacyjne - Projekt - SO Protocol
 * Server code
 * Grzegorz Kowalski, 22i
 * version 1 | 12.2015
 */

#include "utils.h"
#include "so_protocol.h"

#define BUFLEN	(2*(CONSTANT_LEN+MESSAGE_LEN))

void server_handle_packet(so_network *n)
{
	so_packet p = so_unpack(n);
	so_debug_print(&p);

	switch(switchtype(&p))
	{
		case ST_INIT:
			//start_connection(&p);
			break;

		case ST_MESSAGE:
			break;

		case ST_DATA:
			break;

		case ST_ACK:
			break;

		case ST_RETRY:
			break;

		case ST_RESET:
			break;

		case ST_END:
			break;

		default:
			ERROR("Received packet of an unknown or unsupported type. Ignoring.");
			break;
	}

	so_delete_packet(&p);
}

int main(int argc, char *argv[])
{
	int proc_num = 0;
	child *processes = malloc(ALLOCATION_STEP*sizeof(child));
	char *buffer = malloc(BUFLEN);		// enough space for two packets

	char *port;
	if(argc == 1)		// no port number given
		port = (char*)&"9001";
	else
		port = argv[1];

	int sock = create_socket(NULL, port, NULL, DO_BIND);
	INFO("Socket listening on port %s", port);

	char ip[INET_ADDRSTRLEN];

	int recvlen;
	for(;;)
	{
		so_network n;
		memset(&n, 0, sizeof(n));
		if((recvlen = recvfrom(sock, buffer, BUFLEN-1 , 0, (us_sockaddr*)(n.address.ai_addr), &n.address.ai_addrlen)) == -1)
		{
			STDERROR();
			continue;
		}
		// inet_ntop(AF_INET, &(((us_sockaddr4*)(n.address.ai_addr))->sin_addr), ip, INET_ADDRSTRLEN);
		INFO("Got packet of %d bytes from %s.", recvlen, ip);
		n.packet = malloc(recvlen);
		memcpy(n.packet, buffer, recvlen);
		server_handle_packet(&n);
		so_delete_network(&n);
	}

	close(sock);
	free(processes);
	free(buffer);
	return 0;
}