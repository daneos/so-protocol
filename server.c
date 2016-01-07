/*
 * Systemy Operacyjne - Projekt - SO Protocol
 * Server code
 * Grzegorz Kowalski, 22i
 * version 2 | 01.2016
 */

#include "utils.h"

int create_listener(const char *name, const char *port, int qsize, us_addrinfo **addr)
/* Create, bind and listen on listener socket */
{
	us_addrinfo hints, *info;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;        // IP version agnostic
	hints.ai_socktype = SOCK_STREAM;    // TCP socket
	hints.ai_flags = AI_PASSIVE;        // fill in local IP address

	int r;
	if((r = getaddrinfo(name, port, &hints, &info)) != 0)
	{
		ERROR(gai_strerror(r));
		return FAIL;
	}

	// loop through all the results and bind to the first we can
	us_addrinfo *p;
	int sock;
	for(p = info; p != NULL; p = p->ai_next)
	{
		if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			STDERROR();
			continue;
		}

		int yes = 1;
		if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			STDERROR();
			exit(1);
		}

		if(bind(sock, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sock);
			STDERROR();
			continue;
		}
		break;
	}

	if(p == NULL)
	{
		ERROR("failed to bind socket.");
		return FAIL;
	}

	if(listen(sock, qsize) == -1)
	{
		STDERROR();
		return FAIL;
	}

	if(addr)
		*addr = p;
	else
		freeaddrinfo(info);
	
	return sock;
}

void *getaddr(us_sockaddr *addr)
/* Get IPv4 or IPv6 address */
{
	if (addr->sa_family == AF_INET)
		return &(((us_sockaddr4*)addr)->sin_addr);
	else
		return &(((us_sockaddr6*)addr)->sin6_addr);
}

void child(int listener, int sock)
/* Actions performed by a child process */ 
{
	// prepare child process
	close(listener); // close listener to avoid keeping references to it

	//-------------------------------------------------------------------------
	// child main
	int pid = getpid();
	INFO("so-protocol[child:%d] Started.", pid);

	// receive data
	char buf[BUFLEN];
	int len;
	if((len = recv(sock, buf, BUFLEN-1, 0)) == -1)
	{
		STDERROR();
		exit(1);
	}
	len -= 2;
	buf[len] = '\0';		// since I use telnet as a client I need to trim some garbage such as newline
	INFO("so-protocol[child:%d] Got %d bytes from remote: %s", pid, len, buf);

	char cmdbuf[BUFLEN+18];		// 18 is the length of the "python ..." command 
	snprintf(cmdbuf, BUFLEN+18, "python -c \"print %s\"", buf);
	INFO("so-protocol[child:%d] Invoking <%s>.", pid, cmdbuf);
	
	memset(buf, 0, BUFLEN);
	FILE *pipe = popen(cmdbuf, "r");
	fread(buf, BUFLEN-1, sizeof(char), pipe);
	pclose(pipe);

	// send data
	if(send(sock, buf, strlen(buf), 0) == -1)
		STDERROR();
	INFO("so-protocol[child:%d] Sent answer to remote.", pid);
	//-------------------------------------------------------------------------
	
	// child cleanup
	close(sock);
	exit(0);
}

void grim_reaper(int sig)
/* Bury all dead children */
{
	if(sig == SIGCHLD)      // just in case
	{
		// waitpid can change errno
		int old_errno = errno;
		// remove ALL dead children, as we might lose track of several signals
		int pid;
		while((pid = waitpid(-1, NULL, WNOHANG)) > 0) INFO("so-protocol[grim_reaper] Reaped %d.", pid);
		// restore errno
		errno = old_errno;
	}
}

int main(int argc, char *argv[])
{
	int pid = getpid();
	INFO("so-protocol server v2");
	INFO("Grzegorz Kowalski, 22i");
	INFO("01.2016");
	INFO("PID: %d", pid);

	char *port;
	if(argc < 2)        // no port number given
		port = (char*)&"9001";
	else
		port = argv[1];

	int qsize;
	if(argc < 3)        // no queue size given
		qsize = 10;
	else
		qsize = atoi(argv[2]);

	// create listening socket
	us_addrinfo *p;
	int listener = create_listener(NULL, port, qsize, &p);
	
	struct sigaction sig;
	sig.sa_handler = grim_reaper;
	sigemptyset(&sig.sa_mask);
	if(sigaction(SIGCHLD, &sig, NULL) == -1)
	{
		STDERROR();
		exit(1);
	}

	INFO("so-protocol[main:%d]: Ready.", pid);

	us_sockaddr_s remoteaddr;       // remote end address
	socklen_t raddrsize = sizeof remoteaddr;
	char ip[INET6_ADDRSTRLEN];      // address string
	while(1)                        // server accept loop
	{
		// accept connection
		int asock = accept(listener, (us_sockaddr *)&remoteaddr, &raddrsize);
		if(asock == -1)
		{
			STDERROR();
			continue;
		}

		inet_ntop(remoteaddr.ss_family, getaddr((us_sockaddr *)&remoteaddr), ip, sizeof ip);
		INFO("so-protocol[main:%d]: Connection from %s, forking...", pid, ip);

		// fork
		if(!fork()) child(listener, asock);
		else close(asock);      // close new socket to avoid references
	}

	return 0;
}