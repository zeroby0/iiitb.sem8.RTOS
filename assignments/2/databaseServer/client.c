#include "client.h"
#include <sys/time.h>

char* SERVER_ADDRESS =  "aabbccddeeff";

int server_fd;
int client_fd;

int main() {

  setupSignalHandling();

  client_fd = createSocket();
  connectToServer(client_fd, SERVER_ADDRESS);
    struct timeval start, stop;
    gettimeofday(&start, NULL);


    char letter[10] = "1.txt";
    char received[100];

    gaurd_exit(
        write(client_fd, letter, sizeof(letter)),
        "Error writing to server"
      );

    fprintf(stderr, "Sent: %s\n", letter);
    
    gaurd_exit(
        read(client_fd, received, sizeof(received)),
        "Error reading from server"
      );

    fprintf(stderr, "Recv: %s\n", received);
  gettimeofday(&stop, NULL);

  printf("%lu\n", (stop.tv_sec - start.tv_sec)*1000000 + (stop.tv_usec - start.tv_usec));
}