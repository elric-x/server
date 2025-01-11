CC=gcc
CFLAGS= -Wall -Wextra

all: libwsServer.tar 

libwsServer.tar: libwsServer.o
	tar -cf libwsServer.tar libwsServer.o server.h
	rm libwsServer.o

libwsServer.o: server.c server.h 
	$(CC) -c server.c -o $@

