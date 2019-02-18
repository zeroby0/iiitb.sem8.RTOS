#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy
#include <ctype.h> // For tolower and all

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "commons.h"

int server_qid, client_qid;

void closeQueues() {
	if( msgctl(server_qid, IPC_RMID, NULL) == -1) {
		perror("Failed to remove queue");
	}
}

void sighandler(int sig_num) {
	closeQueues();
	exit(0);
}

int main(int argc, char* argv[]) {
	
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

	if ((server_qid = msgget(server_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) {
		perror("Error creating server queue");
		exit(-1);
	}

	printf("Server started with qid %d\n", server_qid);

	Echo echo;

	while(1) {
		if(msgrcv(server_qid, &echo, sizeof(Echo), 0, 0) == -1) {
			perror("Error recieving from client");
			exit(-1);
		}

		printf("\nMessage from client #%d\n", echo.client_qid);
		printf("%c\n", echo.text);

		if(islower(echo.text)) {
			echo.text = toupper(echo.text);
		} else {
			echo.text = tolower(echo.text);
		}

		if(msgsnd(echo.client_qid, &echo, sizeof(Echo), 0) == -1) {
			perror("Error replying to client");
			exit(-1);
		}
	}
}