CC=gcc
CFLAGS= -Wall -Wextra

all: libwsServer.tar 

libwsServer.tar: libwsServer.o
	ar rcs libwsServer.a libwsServer.o
	tar -cf libwsServer.tar libwsServer.a server.h

libwsServer.o: server.c server.h 
	$(CC) -c server.c -o $@

