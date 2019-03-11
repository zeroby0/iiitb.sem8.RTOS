/** ------------------------ Commons.h ------------------------ **/
/** 

  - Written by: Aravind Reddy V
  - License: Public Domain

**/


/** ------------ TO BE IMPLEMENTED BY THE INCLUDER ------------ **/

extern void shutdownEverything();


/** ----------------------------------------------------------- **/

#include <stdio.h>             // For perror, fprintf
#include <stdlib.h>            // For exit(), EXIT_FAILURE
#include <string.h>            // For strcpy
#include <unistd.h>            // For read, write, close

#include <sys/socket.h>        // For socket, setsockopt, bind, listen, shutdown
#include <sys/un.h>            // For struct sockaddr_un


// Exports
int createSocket();
void setupSignalHandling();
void gaurd();
void gaurd_exit();


/** ------------------------ Commons.c ------------------------ **/

void gaurd(int returnVal, const char* message) {
  // fprintf(stderr, "gaurd: Return val is %d\n", returnVal);
  if(returnVal == -1) {
    perror(message);
  }
}

void gaurd_exit(int returnVal, const char* message) {
  // fprintf(stderr, "gaurd_exit: Return val is %d\n", returnVal);
  if(returnVal == -1) {
    perror(message);
    shutdownEverything(EXIT_FAILURE);
  }
  
}

void sighandler(int sig_num) {
  shutdownEverything(sig_num);
}


void setupSignalHandling() {
  // fprintf(stderr, "setupSignalHandling: Setting up signal handling\n");

  signal(SIGINT,  sighandler); // Interrupt
  signal(SIGTSTP, sighandler); // Stop from keyboard
  signal(SIGTERM, sighandler); // Software term signal

  signal(SIGQUIT, sighandler); // Quit
  signal(SIGILL,  sighandler); // Illegal instruction
  signal(SIGKILL, sighandler); // kill
  signal(SIGSEGV, sighandler); // segmentation violation
}

int createSocket() {
  // fprintf(stderr, "Creating socket\n");
  int socket_fd;

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
  gaurd_exit(
      (socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0)), 
      "Socket creation failed"
    );

  // fprintf(stderr, "Socket created with fd: %d\n", socket_fd);

  return socket_fd;
}