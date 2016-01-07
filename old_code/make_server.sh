#/bin/sh

# Systemy Operacyjne - Projekt - SO Protocol
# Server build script
# Grzegorz Kowalski, 22i
# version 1 | 12.2015

gcc -std=c99 -ggdb -D_POSIX_SOURCE -c so_protocol.c 
gcc -std=c99 -ggdb -D_POSIX_SOURCE -c server.c
gcc -o server so_protocol.o server.o