CC=gcc
CFLAGS= -Wall -Wextra

all: libwserver.tar 

libserver.tar: libwsServer.o
	ar rcs libserver.a libserver.o
	tar -cf libserver.tar libserver.a server.h

libserver.o: server.c server.h 
	$(CC) -c server.c -o $@

