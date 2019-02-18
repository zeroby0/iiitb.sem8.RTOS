#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	if ((server_qid = msgget(server_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) {
		perror("Error creating server queue");
		exit(-1);
	}

	printf("Server started with qid %d\n", server_qid);
	g_queueID = server_qid;

	GetRequest request;
	GetResponse response;

	int fileID = 0;
	char fileName[100];
	while(1) {
		if(msgrcv(server_qid, &request, sizeof(GetRequest), 0, 0) == -1) {
			perror("Error recieving request from client");
			closeQueue(g_queueID);
			exit(-1);
		}

		client_qid = request.client_qid;
		fileID = request.fileID;

		printf("Client #%d requested file #%d\n", client_qid, fileID);

		sprintf(fileName, "./data/%d.txt", fileID);

		printf("Corresponding file at %s\n", fileName);

		response.type = 2;
		strcpy(response.result, "jhgjkhgjkh");

		printf("%s\n", response.result);
		printf("%lu\n", sizeof(GetResponse));
		

		if(msgsnd(request.client_qid, &response, sizeof(GetResponse), 0) == -1) {
			perror("Error sending response to client");
			closeQueue(g_queueID);
			exit(-1);
		}
	}
}