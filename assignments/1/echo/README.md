# Echo server

A common server queue is used to send messages to the server. Clients have their own queues to receive messages. Server queue key is generated using this readme file. Client uses `IPC_PRIVATE`, so it's queue id must be sent to the server.

Client queue id is embedded in the message and server replies to this queue. 
```
typedef struct echo_buffer {
	long type;
	
	int client_qid;
	char text;
} Echo;
```

`IPC_PRIVATE` ensures that all clients have different qids and that no client knows other client's qid. Also, this allows us to spin up new clients without creating new files to ensure uniqueness. This makes benchmarking scripts easier.

commons.h has the common components used by both client and server.