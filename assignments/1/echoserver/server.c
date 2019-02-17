#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy
#include <ctype.h> // For tolower and all

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "commons.h"

int main(int argc, char* argv[]) {
	key_t server_queue_key;
	int server_qid, client_qid;

	Message from_client;

	if((server_queue_key = ftok(SERVER_PATH, PROJECT_ID)) == -1) {
		perror("Error generating server key");
		exit(-1);
	}

	if ((server_qid = msgget(server_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) {
		perror("Error creating server queue");
		exit(-1);
	}

	printf("Server started with qid %d\n", server_qid);

	while(1) {
		if(msgrcv(server_qid, &from_client, sizeof(Message), 0, 0) == -1) {
			perror("Error recieving from client");
			exit(-1);
		}

		printf("\nMessage from client #%d\n", from_client.content.client_qid);
		printf("%c\n", from_client.content.text);

		if(islower(from_client.content.text)) {
			from_client.content.text = toupper(from_client.content.text);
		} else {
			from_client.content.text = tolower(from_client.content.text);
		}

		if(msgsnd(from_client.content.client_qid, &from_client, sizeof(Message), 0) == -1) {
			perror("Error replying to client");
			exit(-1);
		}
	}


}