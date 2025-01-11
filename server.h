#ifndef __SERVER__
#define __SERVER__

#include "../lib/buffer.h"
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LISTEN_BACKLOG 10

typedef struct server{
		int port;
		char *address;
		int socket_fd;
		struct sockaddr_in addr;
		socklen_t addr_len;
}server_t;

struct server *server_create(void);
server_t* server_init(int type, int protocol, char *address, int port);
int server_start(
				server_t *server, 
				int (*on_accept)(int client_fd, struct buffer *buffer, void *data),
				buffer_t *buffer, void *data
);
void server_close(server_t *server);
#endif
