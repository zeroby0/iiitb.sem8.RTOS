#include "sockets.h"

extern int socket_client_fd;
extern int socket_server_fd;
extern char* SOCKET_SERVER_ADDRESS;

void socket_shutdown(int arg) {

    shutdown(socket_server_fd, SHUT_RDWR);
    close(socket_server_fd);

    unlink(SOCKET_SERVER_ADDRESS);

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

void handle_client(int client_fd) {

	char fileName[100];
	int sensor_data[5];

	char message[100];

	for(int i = 0; i < 5; i++) {
		sprintf(fileName, "./sensor_data/sensor%d.txt", i+1);
		FILE* fp = fopen(fileName, "r");

		if(fp == NULL) {
			perror("ERR: handle_client() -> while -> fopen\n\
					Error while reading sensor data.");
			printf("%s\n", fileName);
		}
		fscanf(fp, "%d", &(sensor_data[i]));

		fclose(fp);
	}

	sprintf(message, "%d,%d,%d,%d,%d", sensor_data[0], sensor_data[1], sensor_data[2], sensor_data[3], sensor_data[4]);

	printf("%s\n", message);

	if(write(client_fd, message, sizeof(message)) == -1) {
    	perror("handle_client() -> write()\n\
    			Error writing to client.");
    }

	close(client_fd);

    exit(0);
}

int main() {
	setupSignalHandling();

	SOCKET_SERVER_ADDRESS = "aabbccddeeff";

	socket_server_fd = socket_server__setup_server(SOCKET_SERVER_ADDRESS, 5);

	while(1) {
		socket_client_fd = socket_server__accept_connection(socket_server_fd);

		if(fork() == 0) {
			handle_client(socket_client_fd);
		}

		close(socket_client_fd);
	}
}