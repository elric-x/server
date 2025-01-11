#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../include/server.h"

struct server *server_create(void){
		server_t *new_server = calloc(1, sizeof(server_t));
		if(new_server == NULL)
				fprintf(stderr,"%s: %d\n\tMemory allocation error: Unable to allocate new server\n", __FILE__, __LINE__);
		return new_server;
}

//TODO: correct bug line 44
server_t* server_init( int type, int protocol, char *address, int port ){
#include <arpa/inet.h>
		if(!address){
				fprintf(stderr, "%s:%d\n\tInvalid params passed to server_init function\n", __FILE__, __LINE__);
				return NULL;
		}
		//Create the server
		server_t *server = server_create();
		if(server == NULL){
				fprintf(stderr, "%s:%d\n\tFailed to initialize server\n", __FILE__, __LINE__);
				return NULL;
		}
		
		server->address = strndup(address, strlen(address));
		if(server->address == NULL){
				free(server);
				fprintf(stderr,"%s: %d\n\tFailed to initialize server\n", __FILE__, __LINE__);
				return NULL;
		}
		server->port = port;
		server->socket_fd = socket(AF_INET, type, protocol);
		if(server->socket_fd == -1){
				free(server->address);
				free(server);
				fprintf(stderr, "Failed to initialize server: %s\n", strerror(errno));
				return NULL;
		}
		server->addr.sin_family = AF_INET;
		server->addr.sin_port = htons(port);
		//server->addr.sin_addr = inet_addr(address);
		server->addr_len = sizeof(server->addr);

		return server;
}

int server_bind(server_t *server){
		if(!server){
				fprintf(stderr, "%s:%d\n\tserver_bind: Invalid params\n", __FILE__, __LINE__);
				return 1;
		}
		if(bind(server->socket_fd, (struct sockaddr *)&server->addr, server->addr_len) == -1){
				fprintf(stderr,"%s: %d\n\tFailed to bind server: %s\n", __FILE__, __LINE__, strerror(errno));
				return 1;
		}
		return 0;
}

int server_listen(int socket_fd, int backlog){
		if(listen(socket_fd, backlog) == -1 ){
				fprintf(stderr,"%s: %d\n\tFailed to start listenning: %s\n", __FILE__, __LINE__, strerror(errno));
			return 1;
		}
		return 0;
}

int server_accept(
				server_t *server, int (*on_accept)(int client_fd, buffer_t *buffer, void *data),
				buffer_t *buffer,void *data
){
		if(!server){
				fprintf(stderr, "%s:%d\n\tserver_accept: Invalid params\n", __FILE__, __LINE__);
				return 1;
		}

		int client_fd = 0;
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = 0;

		while(1){
				client_fd = accept(server->socket_fd, (struct sockaddr*)&client_addr, &client_addr_len);
				if(client_fd == -1){
						fprintf(stderr, "%s:%d\n\tUnable to accept client: %s\n",__FILE__, __LINE__ ,strerror(errno));
						continue;
				}
				int handle_res = on_accept(client_fd, buffer, data);
				if(handle_res != 0)
						fprintf(stderr, "%s:%d\n\tUnable to client: %s\n",__FILE__, __LINE__ ,strerror(errno));
		}
		return 0;
}

int server_start(
				server_t *server, 
				int (*on_accept)(int client_fd, buffer_t *buffer, void *data),
				buffer_t *buffer,void *data
){
		if(!server || !on_accept){
				fprintf(stderr, "%s:%d\n\tserver_start: Invalid params \n", __FILE__, __LINE__);
				return 1;
		}

		if(server_bind(server) != 0)
				return 1;
		
		if(server_listen(server->socket_fd, LISTEN_BACKLOG) !=0)
				return 1;

		server_accept(server, on_accept, buffer, data);
		return 0;
};


void server_close(server_t *server){
		if(!server)
				return;
		if(server->address)
				free(server->address);
		close(server->socket_fd);
		free(server);
		return;
}
