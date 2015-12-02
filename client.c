/*
 * Systemy Operacyjne - Projekt - SO Protocol
 * Client code
 * Grzegorz Kowalski, 22i
 * version 1 | 12.2015
 */

#include "utils.h"
#include "so_protocol.h"

#define BUFLEN	(2*(CONSTANT_LEN+MESSAGE_LEN))

int main(int argc, char *argv[])
{
	char *port;
	if(argc == 2)		// no port number given
		port = (char*)&"9001";
	else
		port = argv[2];

	us_addrinfo *info;
	int sock = create_socket(argv[1], port, &info, NO_BIND);
	INFO("Socket created successfully.");

	char text[] = "hello world!";
	so_packet p;
	//p.address = *info;
	p.cid = 0xdeadbeef;
	p.seq_num = 0xcafeface;
	SET_TYPE(&p, T_DATA);
	p.data.message.text = text;
	so_network n = so_pack(&p);

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(((us_sockaddr4*)info->ai_addr)->sin_addr), ip, INET_ADDRSTRLEN);

	int sentlen;
	for(;;)
	{
		sleep(1);
		if((sentlen = sendto(sock, n.packet, n.len, 0, info->ai_addr, info->ai_addrlen)) == -1)
		{
			STDERROR();
			continue;
		}
		INFO("Sent packet of %d bytes to %s.", sentlen, ip);
	}

	close(sock);
	so_delete_network(&n);
	return 0;
}