#define SERVER_PATH "./README.md"
#define PROJECT_ID 'C'
#define QUEUE_PERMISSIONS 0660

typedef struct get {
	long type;
	int qid;

	int fileID;
	char response[100];
} Get;

///////////////////

void shutdown();

