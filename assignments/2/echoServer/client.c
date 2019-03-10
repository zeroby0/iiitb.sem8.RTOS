/**

  - Written by Aravind Reddy V <aravind.reddy@iiitb.org>
  - License: Public domain

  ^ This program sends a single char to
    a unix local socket server and
    reads the reply.
**/

#include <stdio.h>             // For perror, fprintf
#include <stdlib.h>            // For exit(), EXIT_FAILURE
#include <string.h>            // For strcpy
#include <unistd.h>            // For read, write, close

#include <sys/socket.h>        // For socket, setsockopt, bind, listen, shutdown
#include <sys/un.h>            // For struct sockaddr_un


#define SERVER_PATH "aabbccddeeffgg"

int client_fd;

int main() {

  /**
    @ Creating a socket.

    - int
      socket(int domain, int type, int protocol);
    - man 2 socket

    @ Arguments and their meaning.

    ^ PF_LOCAL means the socket is for local communication,
      and is not a TCP socket for external communication.

    ^ SOCK_STREAM makes the socket type 'stream'
      other types are datagram (udp) and
      raw (full control over internals)
      read the man pages

    ^ The last argument is for protocol.
      There is _usually_ just one protocol.
      0 is the first one. Arrays start with 0, right?

    ^ Works on Macos (BSD, System V)
      May have to change the arguments for other systems.
      for example, AF_LOCAL. See man pages.

  **/
  if ( (client_fd = socket(PF_LOCAL, SOCK_STREAM, 0)) == -1 ) {
    perror("Socket creation failed.");
    exit(EXIT_FAILURE);
  }


  struct sockaddr_un address;

  address.sun_family = PF_LOCAL;
  strcpy(address.sun_path, SERVER_PATH);

  if(connect(client_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
    perror("An error occured while connecting to server.");
    exit(EXIT_FAILURE);
  }

  char letter = 'A';
  if(write(client_fd, &letter, sizeof(char)) == -1) {
    perror("Error writing to server.");
    exit(EXIT_FAILURE);
  }

  shutdown(client_fd, SHUT_RDWR);
  close(client_fd);
}