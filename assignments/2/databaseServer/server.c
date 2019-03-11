/**

  - Written by: Aravind Reddy V
  - License: Public domain
  
**/

#include "server.h" // For setupSignalHandling(), setupServer(), acceptConnection()

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
  char fileName[10];
  char filePath[100];
  char fileContent[100];

  gaurd(
      read(client_fd, &fileName, sizeof(fileName)),
      "Error reading from client"
    );

  sprintf(filePath, "./data/%s", fileName);

  FILE* fp = fopen(filePath, "r");
  if(!fp) {
    fprintf(stderr, "File not found: %s\n", filePath);
    close(client_fd);
  }

  fscanf(fp, "%s\n", fileContent);
  fclose(fp);


  gaurd(
      write(client_fd, fileContent, sizeof(fileContent)),
      "Error writing to client"
    );

  close(client_fd);

  exit(0);
}