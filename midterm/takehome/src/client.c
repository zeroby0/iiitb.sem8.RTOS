#include "sockets.h"
#include "time.h"

extern int socket_client_fd;
extern int socket_server_fd;
extern char* SOCKET_SERVER_ADDRESS;

void socket_shutdown(int arg) {
	shutdown(socket_client_fd, SHUT_RDWR);
	close(socket_client_fd);
	exit(0);
}

void setupSignalHandling() {
    // fprintf(stderr, "setupSignalHandling: Setting up signal handling\n");

    signal(SIGINT,  socket_shutdown); // Interrupt
    signal(SIGTSTP, socket_shutdown); // Stop from keyboard
    signal(SIGTERM, socket_shutdown); // Software term signal

    signal(SIGQUIT, socket_shutdown); // Quit
    signal(SIGILL,  socket_shutdown); // Illegal instruction
    signal(SIGKILL, socket_shutdown); // kill
    signal(SIGSEGV, socket_shutdown); // segmentation violation
}

void append_to_file(char* fileName, char* data) {
	FILE* fp = fopen(fileName, "a");

	if(fp == NULL) {
		perror("append_to_file() -> fopen()\n\
				File open failed.");
	}

	fprintf(fp, "%lu,%s\n", (unsigned)time(NULL), data);

	fclose(fp);
}

void query_server(char* last_received) {
	socket_client_fd = socket_create();
	socket_client__connect_to_server(socket_client_fd, SOCKET_SERVER_ADDRESS);

	int size_recieved;

	char received[1000];
	memset(received, 0, sizeof(received));

	size_recieved = read(socket_client_fd, received, sizeof(received));

	if(size_recieved == -1) {
		perror("ERR: main() -> while(); -> read()\n\
				Error reading from server.");
		exit(1);
	}

	if(size_recieved == 0) {
		shutdown(socket_client_fd, SHUT_RDWR);
		close(socket_client_fd);
		return;
	}

	printf("%s\n", received);

	if(strcmp(received, last_received)) {
		append_to_file("./results", received);
	}

	strcpy(last_received, received);
}

int main() {
	setupSignalHandling();

	char last_received[1000];

	SOCKET_SERVER_ADDRESS = "aabbccddeeff";

	while(1) {
		query_server(last_received);
		sleep(1);
	}
}