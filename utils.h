/*
 * Systemy Operacyjne - Projekt - SO Protocol
 * Various utilities
 * Grzegorz Kowalski, 22i
 * version 1 | 11.2015
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
#	define ERROR(msg)		fprintf(stderr, "[%s/%s:%d]: %s\n", __FILE__, __func__, __LINE__, msg)
#else
#	define ERROR(msg)		fprintf(stderr, "%s\n", msg)
#endif
#define STDERROR()			ERROR(strerror(errno))
//-----------------------------------------------------------------------------

// constants
#define FAIL				-1
//-----------------------------------------------------------------------------

#endif /* __UTILS_H__ */