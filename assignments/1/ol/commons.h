#define SERVER_PATH "./README.md"

#define PROJECT_ID 'B'

#define QUEUE_PERMISSIONS 0660

#define MAX_INDEX 999

typedef struct getRequest {
	long type;

	int client_qid;
	int fileID;
} GetRequest;

typedef struct getResponse {
	long type;

	char result[100000];
} GetResponse;



int closeQueue(int qid) {
	if(msgctl(qid, IPC_RMID, NULL) == -1) {
		perror("Failed to remove queue");
		return -1;
	}
	return 0;
}

int g_queueID = 0;
void sighandler(int sig_num) 
{ 
    // Reset handler to catch SIGTSTP next time 
    printf("Shutting down process.\n");
    printf("Closing queues\n");
    closeQueue(g_queueID);

    exit(0);
} 