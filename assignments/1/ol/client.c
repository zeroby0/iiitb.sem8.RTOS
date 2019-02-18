#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "commons.h"

int main() {
	signal(SIGINT, sighandler);
	signal(SIGTSTP, sighandler);

	key_t server_queue_key;
	int server_qid, client_qid;

	if((server_queue_key = ftok(SERVER_PATH, PROJECT_ID)) == -1) {
		perror("Error generating server key");
		exit(-1);
	}

	if((server_qid = msgget(server_queue_key, 0)) == -1) {
		perror("Error connecting to server queue");
		exit(-1);
	}

	// Create queue to recieve messages
	if ((client_qid = msgget(IPC_PRIVATE, QUEUE_PERMISSIONS)) == -1) {
		perror("Error creating client queue");
		exit(-1);
	}

	GetRequest request;
	GetResponse response;

	printf("Client started with qid: %d\n", client_qid);
	printf("Connected to server with qid: %d\n", server_qid);

	g_queueID = client_qid;

	printf("Which file would you like to retrieve?\n(0<= int <1000): ");

	int fileID = 0;
	while(scanf(" %d", &fileID)) {
		request.type = 1;
		request.client_qid = client_qid;
		request.fileID = fileID;

		if(msgsnd(server_qid, &request, sizeof(GetRequest), 0) == -1) {
			perror("Failed to send request to server.");
			closeQueue(g_queueID);
			exit(-1);
		}

		if(msgrcv(client_qid, &response, sizeof(GetResponse), 0, 0) == -1) {
			perror("Failed to recieve response from server");
			closeQueue(g_queueID);
			exit(-1);
		}

		// printf("%s\n", response.fileID);
		printf("%s\n", response.result);

		printf("\nWhich file would you like to retrieve?\n(0<= int <1000): ");
	}



}