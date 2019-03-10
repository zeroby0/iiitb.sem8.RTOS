/**

  - Written by Aravind Reddy V <aravind.reddy@iiitb.org>
  - License: Public domain

  ^ This program listens for clients sending letters via sockets,
    and just echoes back whatever is received, after changing the
    case of the alphabet.
**/

#include <stdio.h>             // For perror, fprintf
#include <stdlib.h>            // For exit(), EXIT_FAILURE
#include <string.h>            // For strcpy
#include <unistd.h>            // For read, write, close

#include <sys/socket.h>        // For socket, setsockopt, bind, listen, shutdown
#include <sys/un.h>            // For struct sockaddr_un


#define SERVER_PATH "aabbccddeeffgg"
#define MAX_SIMULTANEOUS_CONNECTIONS 5

int server_fd;
int client_fd;

void shutdownServer(int errno) {
  shutdown(server_fd, SHUT_RDWR);
  close(server_fd);

  unlink(SERVER_PATH);

  exit(errno);
}

void sighandler(int sig_num) {
  shutdownServer(sig_num);
}

void setupSignalHandling() {
  signal(SIGINT,  sighandler); // Interrupt
  signal(SIGTSTP, sighandler); // Stop from keyboard
  signal(SIGTERM, sighandler); // Software term signal

  signal(SIGQUIT, sighandler); // Quit
  signal(SIGILL,  sighandler); // Illegal instruction
  signal(SIGKILL, sighandler); // kill
  signal(SIGSEGV, sighandler); // segmentation violation
}

void gaurd(int returnVal, const char* message) {
  if(returnVal == -1) {
    perror(message);
  }
}

void gaurd_exit(int returnVal, const char* message) {
  if(returnVal == -1) {
    perror(message);
  }
  shutdownServer(-1);
}

int createSocket() {
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
  if ((socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0)) == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  return socket_fd;
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
  if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuseAddress,  sizeof(reuseAddress)) == -1) {   
    perror("An error occured when making the socket address reusable.");
    sighandler(EXIT_FAILURE);  
  }


}


int main() {
  setupSignalHandling();

  unlink(SERVER_PATH); // Closing socket if already in use

  server_fd = createSocket();


 

  printf("%d\n", server_fd);



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

    ^ server_fd is the servers file descriptor.

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
  strcpy(address.sun_path, SERVER_PATH);

  if( bind(server_fd, (struct sockaddr *)&address, sizeof(address))  == -1) {
    perror("An error occured while binding the socket");
    sighandler(EXIT_FAILURE);
  }


  /**
    @ Listening on the socket.

    - int
      listen(int socket, int backlog);
    - man 2 listen

    @ Arguments and their meaning.

    ^ server_fd is the server file descriptor.
    ^ MAX_SIMULTANEOUS_CONNECTIONS is what it says.

  **/
  if ( listen(server_fd, MAX_SIMULTANEOUS_CONNECTIONS) == -1 ) {   
    perror("An error occured while listening in server");  
    sighandler(EXIT_FAILURE);    
  }


  fprintf(stderr, "Waiting for clients.");

  while(1) {

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

    if((client_fd = accept(server_fd, NULL, NULL)) == -1) {
      perror("Error accepting connection from client.");
    }

    char letter;
    if(read(client_fd, &letter, sizeof(char)) == -1) {
      perror("Error reading from client.");
    }

    printf("From client: %c\n", letter);
  }
}