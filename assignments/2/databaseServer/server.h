/** ------------------------ Server.h ------------------------- **/
/** 

  - Written by: Aravind Reddy V
  - License: Public Domain
  
**/

/** ------------ TO BE IMPLEMENTED BY THE INCLUDER ------------ **/

extern void handleClient();

extern char* SERVER_ADDRESS;
extern int MAX_SIMULTANEOUS_CONNECTIONS;

extern int server_fd;
extern int client_fd;

/** ----------------------------------------------------------- **/

#include "commons.h"

// Imports
int createSocket(); // Commons.h

// Exports
void setupSignalHandling();
int  setupServer();
int  acceptConnection();
void shutdownEverything();

// Internals
void makeSocketReusable();
void bindSocket();
void makeSocketListen();

/** ------------------------ Exports ------------------------- **/

int setupServer(char* server_addr, int max_sim_clients) {
  int socket_fd = createSocket();
  
  makeSocketReusable(socket_fd);
  bindSocket(socket_fd, server_addr);
  makeSocketListen(socket_fd, max_sim_clients);

  return socket_fd;
}


int acceptConnection(int socket_fd) {
  int connection_fd;
  /**
    @ Accepting connections on the socket.

    - int
      accept(int socket, struct sockaddr address,
         socklen_t address_len);
    - man 2 accept

    accept() extracts the first connection request on the queue
    of pending connections, creates a new socket with the same 
    properties of socket, and allocates a new file
    descriptor for the socket.

    @ Arguments and their meaning.

    ^ socket is the server's socket.

    ^ clients address is filled into the
      second argument, struct sockaddr address.

    ^ address_len has the size of the second
      argument, in bytes.

    We don't really need the clients address and such.
    Just the file descriptor is plenty. So passing NULL.
  **/
  gaurd(
      (connection_fd = accept(socket_fd, NULL, NULL)),
      "Error accepting connection from client."
    );
  return connection_fd;
}



/* ---------------------------------------- */



void shutdownEverything(int errno) {
  // fprintf(stderr, "shutdownServer: Shutting down\n");

  shutdown(server_fd, SHUT_RDWR);
  close(server_fd);

  unlink(SERVER_ADDRESS);

  exit(errno);
}

void makeSocketReusable(int socket_fd) {
  /**
    @ making the socket receive multiple connections.

    - int
      getsockopt(int socket, int level, int option_name, 
           void * option_value, socklen_t option_len);
    - man 2 setsockopt


    @ Arguments and their meaning.

    ^ server_fd is the file descriptor for our socket

    ^ SOL_SOCKET sets the level at which setsockopt should
      look for the option to change. Read man page.

    ^ SO_REUSEADDR is the option we are going to change.
      This makes the socket address reusable.
      usually, when a client tries to connect to a socket
      _already_ in use, it results in an error.

      But we need multiple clients to connect to the server,
      hence we are reusing sockets

    ^ The value we are setting to the option SO_REUSEADDR
      is 1. But I can't pass this directly, the setsockopt function
      needs it as a pointer. So making an int = 1, and passing
      that int's pointer. That's int reuseAddress.

  **/
  int reuseAddress = 1;
  gaurd_exit(
      setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuseAddress,  sizeof(reuseAddress)),
      "An error occured when making the socket address reusable."
    );

}

void bindSocket(int socket_fd, const char* socketAddress) {

  unlink(socketAddress); // Closing socket if already in use

  /** 
    @ Binding the socket to an address

    - int
      bind(int socket, const struct sockaddr *address,
         socklen_t address_len);
    - man 2 bind

    We've created a socket and it exists, but other
    programs need to know it's name to find it 
    and send data to it.

    So we give the socket a name and set it free.
    Try netstat | grep "aabbccddeeffgg"

    @ Arguments and their meaning.

    ^ server_fd is the server's file descriptor.

    ^ We use the struct sockaddr_un to send these options to bind.

        struct sockaddr_un {
          unsigned short sun_family;
          char sun_path[108];
        }

      I believe _un means unix.
      For TCP sockets, you'd send sockaddr_in. _in is Internet.
      probably.

      Similary, sun_family is socket unix family.
      It's sin_family for TCP sockets.

      Anyhow, since these are local sockets and not TCP,
      we make sun_family PF_LOCAL
      and sun_path the file we want to bind to.

  **/
  struct sockaddr_un address;

  address.sun_family = PF_LOCAL;
  strcpy(address.sun_path, socketAddress);

  gaurd_exit(
      bind(socket_fd, (struct sockaddr *)&address, sizeof(address)),
      "An error occured while binding the socket"
    );

}

void makeSocketListen(int socket_fd, int backlog_size) {
  /**
    @ Listening on the socket.

    - int
      listen(int socket, int backlog);
    - man 2 listen

    @ Arguments and their meaning.

    ^ server_fd is the server file descriptor.
    ^ MAX_SIMULTANEOUS_CONNECTIONS is what it says.

  **/
  gaurd_exit(
      listen(socket_fd, backlog_size),
      "Error listening on socket"
    );
}