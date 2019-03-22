#include <stdio.h>             // For perror, fprintf
#include <stdlib.h>            // For exit(), EXIT_FAILURE
#include <string.h>            // For strcpy
#include <unistd.h>            // For read, write, close

#include <sys/socket.h>        // For socket, setsockopt, bind, listen, shutdown
#include <sys/un.h>            // For struct sockaddr_un



#ifndef SOCKETS_H
#define SOCKETS_H

char* SOCKET_SERVER_ADDRESS;

int socket_server_fd;
int socket_client_fd;


extern void socket_shutdown(int arg); // You need to implement this


int socket_create();

int socket_server__accept_connection(int socket_fd);
void socket_server__make_socket_reusable(int socket_fd);
void socket_server__bind_socket(int socket_fd, const char* socketAddress);
void socket_server__make_socket_listen(int socket_fd, int backlog_size);
int socket_server__setup_server(char* server_addr, int max_sim_clients);


void socket_client__connect_to_server(int client_fd, const char* server_addr);






#endif