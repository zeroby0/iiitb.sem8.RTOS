/**
-
- The path used to derive ftok for the
- message queue
-
**/
#define SERVER_PATH "./README.md"

/**
- User defined key for ftok
-
**/
#define PROJECT_ID 'A'

#define QUEUE_PERMISSIONS 0660

bool verbose_mode = false;

/**
-
- This struct encapsulates the message
- Includes client id too so that the
- server can reply
-
**/
typedef struct message_content {
	int client_qid;
	char text[1024];
} Message_content;

/**
-
- Wrapper around message content
**/
typedef struct message {
	long type;
	Message_content content;
} Message;

