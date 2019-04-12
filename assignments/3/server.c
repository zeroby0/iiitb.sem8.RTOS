#include <stdio.h> 
#include <unistd.h> // read()
#include <string.h> // strerror()
#include <errno.h>  // errno()

#include <arpa/inet.h> // This includes socket and friends too

#include <pulse/simple.h>
#include <pulse/error.h>

int main(int argc, char* argv[]) {

    int socket_fd__listen;

    if((socket_fd__listen = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("ERR: socket_create() -> socket()\n\
                Socket creation failed.");

        return -1;
    }

    struct sockaddr_in socket_server_address;

    socket_server_address.sin_family = AF_INET;
    socket_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_server_address.sin_port = htons(16000);

    int reuseAddress = 1;
    if(setsockopt(socket_fd__listen, SOL_SOCKET, SO_REUSEADDR, (void *)&reuseAddress,  sizeof(reuseAddress)) == -1) {
        perror("ERR: socket_server__make_socket_reusable() -> setsockopt()\n\
                An error occured when making the socket address reusable.");
    }  

    if(bind(socket_fd__listen, (struct sockaddr *)&socket_server_address, sizeof(socket_server_address)) == -1) {
        perror("ERR: socket_server__bind_socket() -> bind()\n\
            An error occured while binding the socket");
    }

    if(listen(socket_fd__listen, 5) == -1) {
        perror("ERR: socket_server__make_socket_listen() -> listen()\n\
                Error listening on socket");
    }


    static const pa_sample_spec sample_spec = {
        .format = PA_SAMPLE_S16LE,
        .rate = 20000,
        .channels = 1
    };
    pa_simple *sound_stream = NULL;
    int ret = 1;
    int error;
    
    /* Create a new playback stream */
    if (!(sound_stream = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &sample_spec, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
    }

    int socket_fd__incoming_call;
    while(1) {

        if((socket_fd__incoming_call = accept(socket_fd__listen, NULL, NULL)) == -1) {
            perror("ERR: socket_server__accept_connection() -> accept()\n\
                    Error accepting connection from client.");
        }
        
        printf("Call started.\n");

        uint8_t voice_buffer[1024];
        int incoming_data_size;

        if ((incoming_data_size = read(socket_fd__incoming_call, voice_buffer, sizeof(voice_buffer))) <= 0) {
            if (incoming_data_size == 0) /* EOF */
                break;

            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
        }

        if (pa_simple_write(sound_stream, voice_buffer, (size_t)incoming_data_size, &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
        }

    }

}
