#/bin/sh

# Systemy Operacyjne - Projekt - SO Protocol
# Client build script
# Grzegorz Kowalski, 22i
# version 1 | 12.2015

gcc -std=c99 -D_POSIX_SOURCE -c so_protocol.c 
gcc -std=c99 -D_POSIX_SOURCE -c client.c
gcc -o client so_protocol.o client.o