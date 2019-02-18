#include <stdio.h>    // For perror()
#include <stdlib.h>   // For signals and exit()
#include <sys/msg.h>  // For message queues

#include "commons.h"

void closeQueues() {
	if( msgctl(client_qid, IPC_RMID, NULL) == -1) {
		perror("Failed to remove client queue");
	}
}

void sighandler(int sig_num) {
	closeQueues();
	exit(0);
}

int main() {

	signal(SIGINT,  sighandler); // Interrupt
	signal(SIGTSTP, sighandler); // Stop from keyboard
	signal(SIGTERM, sighandler); // Software term signal

	signal(SIGQUIT, sighandler); // Quit
	signal(SIGILL,  sighandler); // Illegal instruction
	signal(SIGKILL, sighandler); // kill
	signal(SIGSEGV, sighandler); // segmentation violation

	key_t server_queue_key;

	if((server_queue_key = ftok(SERVER_PATH, PROJECT_ID)) == -1) {
		perror("Error generating server key");
		exit(-1);
	}

	if((server_qid = msgget(server_queue_key, 0)) == -1) {
		perror("Error connecting to server queue");
		fprintf(stderr, "Check if the server is running.\n");
		exit(-1);
	}

	// Create queue to recieve messages
	if ((client_qid = msgget(IPC_PRIVATE, QUEUE_PERMISSIONS)) == -1) {
		perror("Error creating client queue");
		exit(-1);
	}

	fprintf(stderr, "Client started with qid %d\nConnected to server %d\n", client_qid, server_qid);
	fprintf(stderr, "Press ctrl-z to stop.\n");
	//------


	Echo echo;
	echo.type = 1;
	echo.client_qid = client_qid;	

	printf("\nMessage: ");
	while(scanf(" %c", &echo.text)) {
		if(msgsnd(server_qid, &echo, sizeof(Echo), 0) == -1) {
			perror("Failed to send message to server.");
			closeQueues();
			exit(-1);
		}

		if(msgrcv(client_qid, &echo, sizeof(Echo), 0, 0) == -1) {
			perror("Failed to recieve message from server");
			closeQueues();
			exit(-1);
		}

		printf("From server: %c\n\nMessage: ", echo.text);
	}
	
}