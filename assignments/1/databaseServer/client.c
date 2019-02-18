#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>

#include "commons.h"

int server_qid, client_qid;

void shutdown() {
	if( msgctl(client_qid, IPC_RMID, NULL) == -1) {
		perror("Failed to remove queue");
		exit(-1);
	}

	exit(0);
}

void sighandler(int sig_num) {shutdown();}

int main(int argc, char* argv[]) {

	// # Second argument is the file that
	// # will be requested

	// # First is to be used for
	// # creating server queue key

	signal(SIGINT, sighandler);
	signal(SIGTSTP, sighandler);

	key_t server_queue_key;
	
	if( (server_queue_key = ftok(SERVER_PATH, atoi(argv[1])) ) == -1) {
		perror("Error generating server key");
		exit(-1);
	}

	if((server_qid = msgget(server_queue_key, 0)) == -1) {
		perror("Error connecting to server queue");
		exit(-1);
	}

	if ((client_qid = msgget(IPC_PRIVATE, QUEUE_PERMISSIONS)) == -1) {
		perror("Error creating client queue");
		exit(-1);
	}

	Get get;

	get.type = 10;
	get.qid  = client_qid;
	get.fileID = atoi(argv[2]);

	struct timeval start, stop;
	
	if(msgsnd(server_qid, &get, sizeof(get), 0) == -1) {
		perror("Failed to send message to server.");
		shutdown();
	}

	gettimeofday(&start, NULL);

	if(msgrcv(client_qid, &get, sizeof(get), 0, 0) == -1) {
		perror("Failed to recieve message from server");
		shutdown();
	}

	gettimeofday(&stop, NULL);

	printf("%lu\n", (stop.tv_sec - start.tv_sec)*1000000 + (stop.tv_usec - start.tv_usec) );

	shutdown();
}