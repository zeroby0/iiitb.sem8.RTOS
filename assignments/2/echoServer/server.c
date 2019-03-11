/**

  - Written by: Aravind Reddy V
  - License: Public domain

  ^ This program listens for clients sending letters via sockets,
    and just echoes back whatever is received, after incrementing
    it by 1.
**/

#include "server.h"
/**
Imports from server.h

- setupSignalHandling()
- setupServer()
- acceptConnection()

**/

char* SERVER_ADDRESS =  "aabbccddeeff";
int MAX_SIMULTANEOUS_CONNECTIONS = 64;

int server_fd;
int client_fd;

int main() {

  setupSignalHandling();

  server_fd = setupServer(SERVER_ADDRESS, MAX_SIMULTANEOUS_CONNECTIONS);

  while(1) {
    client_fd = acceptConnection(server_fd);

    if(fork() == 0) { // child
        handleClient(client_fd);
    }

    close(client_fd);

  }
}

void handleClient(int client_fd) {
  char letter;

  gaurd(
      read(client_fd, &letter, sizeof(char)),
      "Error reading from client"
    );

  letter = letter + 1;

  gaurd(
      write(client_fd, &letter, sizeof(char)),
      "Error writing to client"
    );

  close(client_fd);

  exit(0);
}