#/bin/sh

# Systemy Operacyjne - Projekt - SO Protocol
# Server build script
# Grzegorz Kowalski, 22i
# version 2 | 01.2016
 
gcc -o server -std=c99 -ggdb -D_POSIX_SOURCE server.c