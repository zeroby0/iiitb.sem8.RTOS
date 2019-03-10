/**

  - Written by Aravind Reddy V <aravind.reddy@iiitb.org>
  - License: Public domain

  ^ This program sends a single char to
    a unix local socket server and
    reads the reply.
**/

#include "commons.h"

extern char* SERVER_ADDRESS;

extern int server_fd;
extern int client_fd;


void connectToServer(int client_fd, const char* server_addr) {
  struct sockaddr_un address;

  address.sun_family = PF_LOCAL;
  strcpy(address.sun_path, SERVER_ADDRESS);

  gaurd_exit(
      connect(client_fd, (struct sockaddr *)&address, sizeof(address)),
      "An error occured while connecting to server"
    );
}

void shutdownEverything() {
  shutdown(client_fd, SHUT_RDWR);
  close(client_fd);
}

