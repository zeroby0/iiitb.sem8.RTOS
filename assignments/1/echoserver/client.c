#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "commons.h"



int main(int argc, char* argv[]) {
	key_t server_queue_key;
	int server_qid, client_qid;

	Message to_server, from_server;

	// Create queue to recieve messages
	if ((client_qid = msgget(IPC_PRIVATE, QUEUE_PERMISSIONS)) == -1) {
		perror("Error creating client queue");
		exit(-1);
	}

	if((server_queue_key = ftok(SERVER_PATH, PROJECT_ID)) == -1) {
		perror("Error generating server key");
		exit(-1);
	}

	if((server_qid = msgget(server_queue_key, 0)) == -1) {
		perror("Error connecting to server queue");
		exit(-1);
	}

	to_server.type = 1;
	to_server.content.client_qid = client_qid;

	printf("Client started with qid %d\nConnected to server %d\n", client_qid, server_qid);

	printf("\nMessage: ");

	// while(getc(&to_server.content.text)) {
	while(scanf(" %c", &to_server.content.text)) {
		if(msgsnd(server_qid, &to_server, sizeof(Message), 0) == -1) {
			perror("Failed to send message to server.");
			exit(-1);
		}

		if(msgrcv(client_qid, &from_server, sizeof(Message), 0, 0) == -1) {
			perror("Failed to recieve message from server");
			exit(-1);
		}

		printf("From server: %c\n\nMessage: ", from_server.content.text);
	}

	if(msgctl(client_qid, IPC_RMID, NULL) == -1) {
		perror("Failed to remove client queue");
		exit(-1);
	}

	exit(0);
}