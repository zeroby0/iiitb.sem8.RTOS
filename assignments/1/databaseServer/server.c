#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>

#include "commons.h"

int server_qid, client_qid;
FILE *outfile, *fp;

void closeFiles() {
	fclose(fp);
	fclose(outfile);
}

void closeQueues() {
	if( msgctl(server_qid, IPC_RMID, NULL) == -1) {
		perror("Failed to remove queue");
	}
}

void shutdown() {
	closeFiles();
	closeQueues();
	fprintf(stderr, "Shutting down\n");
	exit(0);
}

void sighandler(int sig_num) {shutdown();}

int main(int argc, char* argv[]) {
	// # server: Second argument is the file to which
	// # results should be written

	// # First argument is used to create server key
	outfile = fopen(argv[2], "w");

	signal(SIGINT, sighandler);  // Interrupt
	signal(SIGTSTP, sighandler); // Stop from keyboard
	signal(SIGTERM, sighandler); // Software term signal

	signal(SIGQUIT, sighandler); // Quit
	signal(SIGILL, sighandler);  // Illegal instruction
	signal(SIGKILL, sighandler); // kill
	signal(SIGSEGV, sighandler); // segmentation violation

	key_t server_queue_key;

	if( (server_queue_key = ftok(SERVER_PATH, atoi(argv[1])) ) == -1) {
		perror("Error generating server key");
		shutdown();
	}

	if ((server_qid = msgget(server_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) {
		perror("Error creating server queue");
		shutdown();
	}

	// printf("Server started with qid %d\n", server_qid);

	Get get;

	while(1) {
		struct timeval start, stop;

		if(msgrcv(server_qid, &get, sizeof(get), 0, 0) == -1) {
			perror("Error recieving from client");
			shutdown();
		}

		gettimeofday(&start, NULL);

		// Not doing the file number
		// validation.
		char fileName[100];
		sprintf(fileName, "./data/%d.txt", get.fileID);

		fp = fopen(fileName, "r");
		if(!fp) {
			printf("File EON!\n");
			printf("%s\n", fileName);
		}
		fscanf(fp, "%s\n", get.response);

		fclose(fp);

		if(msgsnd(get.qid, &get, sizeof(get), 0) == -1) {
			perror("Error replying to client");
			shutdown();
		}

		gettimeofday(&stop, NULL);
		fprintf(outfile, "%lu\n", (stop.tv_sec - start.tv_sec)*1000000 + (stop.tv_usec - start.tv_usec));

	}



}