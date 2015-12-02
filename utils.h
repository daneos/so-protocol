/*
 * Systemy Operacyjne - Projekt - SO Protocol
 * Various utilities
 * Grzegorz Kowalski, 22i
 * version 1 | 12.2015
 */

#if !defined(__UTILS_H__)
#	define __UTILS_H__
//-----------------------------------------------------------------------------

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>

#if defined(_WIN32)
#	include <winsock2.h>
#	include <ws2tcpip.h>
#else
#	include <unistd.h>
 
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <netdb.h>
#	include <arpa/inet.h>
#endif
//-----------------------------------------------------------------------------

// renaming unix socket structs
typedef struct addrinfo 		us_addrinfo;
typedef struct sockaddr			us_sockaddr;
typedef struct sockaddr_storage	us_sockaddr_s;
typedef struct sockaddr_in		us_sockaddr4;
typedef struct sockaddr_in6		us_sockaddr6;
//typedef fd_set					sockset;
//-----------------------------------------------------------------------------

// macros
#if defined(__GNUC__) && (__STDC_VERSION__ >= 199901L)
	// i'm assuming that GNU C is running on linux terminal that supports colors
	// it's probably wrong, but i'm too lazy to actually check it
#	define ERROR(msg, ...)		({ \
									fprintf(stderr, "\x1b[1;31m<%d> [%s/%s():%d]: ", (int)time(NULL), __FILE__, __func__, __LINE__); \
									fprintf(stderr, msg, ## __VA_ARGS__); \
									fprintf(stderr, "\x1b[0m\n"); \
								})
#	define INFO(msg, ...)		({ \
									printf("\x1b[1;94m<%d> [%s/%s():%d]: ", (int)time(NULL), __FILE__, __func__, __LINE__); \
									printf(msg, ## __VA_ARGS__); \
									printf("\x1b[0m\n"); \
								})
#else
#	define ERROR(msg, ...)		({ \
									fprintf(stderr, "<%d> ", (int)time(NULL)); \
									fprintf(stderr, msg, ## __VA_ARGS__); \
									fprintf(stderr, "\n"); \
								})
#	define INFO(msg, ...)		({ \
									printf("<%d> ", (int)time(NULL)); \
									printf(msg, ## __VA_ARGS__); \
									printf("\n"); \
								})
#endif
#define STDERROR()			ERROR(strerror(errno))
//-----------------------------------------------------------------------------

// constants
#define FAIL				-1
// bind or not to bind, that is the question...
#define NO_BIND				0
#define	DO_BIND				1
//-----------------------------------------------------------------------------

#endif /* __UTILS_H__ */