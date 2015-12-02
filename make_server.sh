#/bin/sh

gcc -std=c99 -D_POSIX_SOURCE -c so_protocol.c 
gcc -std=c99 -D_POSIX_SOURCE -c server.c
gcc -o server so_protocol.o server.o