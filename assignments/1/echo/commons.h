/**
-
- The path used to derive ftok for the
- server message queue
-
- As this is an existing file, all programs
- can synthesize the server key and can
- message it.
-
- But Client uses IPC_private, whose key is
- sent as a part of the message
-
**/
#define SERVER_PATH "./README.md"
#define PROJECT_ID 'A'

#define QUEUE_PERMISSIONS 0660

typedef struct echo_buffer {
	long type;
	
	int client_qid;
	char text;
} Echo;



// Global variable so that closeQueues() can access them
// Will be initialised in client and server
int server_qid, client_qid; 

// Will be implemented differently by client and server
void closeQueues(); 
void sighandler(int sig_num);
