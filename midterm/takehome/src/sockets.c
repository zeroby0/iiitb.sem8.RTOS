#include "sockets.h"

int socket_create() {
    int socket_fd;

    if((socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0)) == -1) {
        perror("ERR: socket_create() -> socket()\n\
                Socket creation failed.");
    }

    return socket_fd;
}

void socket_client__connect_to_server(int client_fd, const char* server_addr) {
    struct sockaddr_un address;

    address.sun_family = PF_LOCAL;
    strcpy(address.sun_path, SOCKET_SERVER_ADDRESS);

    if(connect(socket_client_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("ERR: socket_client__connect_to_server() -> connect\n\
                An error occured while connecting to server");
    }

    
}

int socket_server__accept_connection(int socket_fd) {
    int connection_fd;

    if((connection_fd = accept(socket_fd, NULL, NULL)) == -1) {
        perror("ERR: socket_server__accept_connection() -> accept()\n\
                Error accepting connection from client.");
    }

    return connection_fd;
}

void socket_server__make_socket_reusable(int socket_fd) {
    int reuseAddress = 1;
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuseAddress,  sizeof(reuseAddress)) == -1) {
        perror("ERR: socket_server__make_socket_reusable() -> setsockopt()\n\
                An error occured when making the socket address reusable.");
    }           
}

void socket_server__bind_socket(int socket_fd, const char* socketAddress) {
    unlink(socketAddress); // Closing socket if already in use

    struct sockaddr_un address;

    address.sun_family = PF_LOCAL;
    strcpy(address.sun_path, socketAddress);

    if(bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("ERR: socket_server__bind_socket() -> bind()\n\
            An error occured while binding the socket");
    }
}

void socket_server__make_socket_listen(int socket_fd, int backlog_size) {
    if(listen(socket_fd, backlog_size) == -1) {
        perror("ERR: socket_server__make_socket_listen() -> listen()\n\
                Error listening on socket");
    }

}

int socket_server__setup_server(char* server_addr, int max_sim_clients) {
    int socket_fd = socket_create();
    
    socket_server__make_socket_reusable(socket_fd);
    socket_server__bind_socket(socket_fd, server_addr);
    socket_server__make_socket_listen(socket_fd, max_sim_clients);

    return socket_fd;
}