#include <stdio.h> 

#include <arpa/inet.h> // This includes socket and friends too

#include <pulse/simple.h>
#include <pulse/error.h>


int main(int argc, char* argv[]) {

	int socket_fd;

	if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("ERR: socket_create() -> socket()\n\
                Socket creation failed.");

        return -1;
    }

    struct sockaddr_in socket_server_address;

    socket_server_address.sin_family = AF_INET;
    socket_server_address.sin_port = htons(16000);

    if(inet_pton(AF_INET, argv[1], &socket_server_address.sin_addr) <= 0) {
        printf("\n inet_pton error occured\n");
        return 1;
    }
    
    if(connect(socket_fd, (struct sockaddr *)&socket_server_address, sizeof(socket_server_address)) == -1) {
        perror("ERR: socket_client__connect_to_server() -> connect\n\
                An error occured while connecting to server");
    }
    
    printf("Call connected.\n");

    static const pa_sample_spec sample_spec = {
		.format = PA_SAMPLE_S16LE,
		.rate = 20000,
		.channels = 1
	};

	pa_simple *sound_stream = NULL;
	int ret = 1, error;
	if (!(sound_stream = pa_simple_new(NULL, argv[0], PA_STREAM_RECORD, NULL, "record", &sample_spec, NULL, NULL, &error))) {
		fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
	}
        
	while(1){
		uint8_t audio_buffer[1024];

		if (pa_simple_read(sound_stream, audio_buffer, sizeof(audio_buffer), &error) < 0) {
		    fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
		}

		send(socket_fd, audio_buffer, sizeof(audio_buffer) , 0 );		
	}
	return 0; 

}
