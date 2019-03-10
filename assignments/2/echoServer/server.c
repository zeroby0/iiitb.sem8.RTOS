/**

- Written by Aravind Reddy V <aravind.reddy@iiitb.org>
- License: Public domain

^ This program listens for clients sending letters via sockets,
  and just echoes back whatever is received, after changing the
  case of the alphabet.

^ Usually, we'd spawn a new thread whenever a new client
  starts messaging, and handle the client in that thread.
  
  However, there's overhead attached to spawning threads,
  and it quickly gets complex and unscalable.
  
  Fortunately for us, Unix provides a nifty function called
  select(2). select let's us listen to many
  file descriptors and see which become active.
  
  see man 2 select
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

void sighandler(int sig_num) {
    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);

    unlink(SERVER_PATH);

    exit(sig_num);
}


int main() {

    signal(SIGINT,  sighandler); // Interrupt
    signal(SIGTSTP, sighandler); // Stop from keyboard
    signal(SIGTERM, sighandler); // Software term signal

    signal(SIGQUIT, sighandler); // Quit
    signal(SIGILL,  sighandler); // Illegal instruction
    signal(SIGKILL, sighandler); // kill
    signal(SIGSEGV, sighandler); // segmentation violation

    unlink(SERVER_PATH); // Closing socket if already in use

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
    if ((server_fd = socket(PF_LOCAL, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("%d\n", server_fd);


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
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuseAddress,  sizeof(reuseAddress)) == -1) {   
        perror("An error occured when making the socket address reusable.");

        sighandler(EXIT_FAILURE);  
    }  

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

    /**
    ^ I really like logging debug info to stderr.

      That way, I can turn it off with just
      ./a.out 2> /dev/null
      and can pipe debug info and actual
      output to different files.

    **/
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
        int client_fd;
        if((client_fd = accept(server_fd, NULL, NULL)) == -1) {
            perror("Error accepting connection from client.");
            // exit(EXIT_FAILURE);
            // Not exiting because there might be other connections.
        }

        char letter;
        if(read(client_fd, &letter, sizeof(char)) == -1) {
            perror("Error reading from client.");
        }

        printf("From client: %c\n", letter);
    }
}