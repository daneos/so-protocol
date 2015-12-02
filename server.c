/*
 * Systemy Operacyjne - Projekt - SO Protocol
 * Server code
 * Grzegorz Kowalski, 22i
 * version 1 | 12.2015
 */

#include "utils.h"
#include "so_protocol.h"

#define BUFLEN	(2*(CONSTANT_LEN+MESSAGE_LEN))

int main(int argc, char *argv[])
{
	int proc_num = 0;
	child *processes = malloc(ALLOCATION_STEP*sizeof(child));
	char *buffer = malloc(BUFLEN);		// enough space for two packets

	char *port;
	if(argc == 1)		// no port number given
		port = &"9001";
	else
		port = argv[1];

	int sock = create_listener(NULL, port);
	INFO("Socket created successfully.");

	int recvlen;
	for(;;)
	{
		so_network n;
		if((recvlen = recvfrom(sock, buffer, BUFLEN-1 , 0, (us_sockaddr*)&n.address, &n.addr_len)) == -1)
		{
			STDERROR();
			continue;
		}
		n.packet = malloc(recvlen);
		strncpy(n.packet, buffer, recvlen);
		server_handle_packet(&n);
		so_delete_network(&n);
	}

	free(processes);
	free(buffer);
	return 0;
}