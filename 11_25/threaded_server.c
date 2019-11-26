#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void *client_handler(void *arg) {
    int sock = *((int *) arg);
    char buffer[256];
    int no_of_bytes;

    do {
        memset(buffer, 0, 256);
        no_of_bytes = read(sock, buffer, 256);
        if(no_of_bytes > 0) {
            printf("Client message:\n%s\n", buffer);
        }
    } while(no_of_bytes > 0);
    close(sock);
    return 0;

}

int main(int argc, char **argv) {
    int sock, newsock, portno, clilength;
    struct sockaddr_in server_address, client_address;
    pthread_t handler_thread;

    if(argc < 2) {
        printf("Please provide a port\n");
        printf("Usage: simple_server 5000");
        return 1;
    }

    // read command-line arg
    portno = atoi(argv[1]);

    // initialize server socket    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset((char *) &server_address,0 ,sizeof(struct sockaddr_in));
    
    server_address.sin_family       = AF_INET;
    server_address.sin_port         = htons(portno);
    server_address.sin_addr.s_addr  = INADDR_ANY;

    if(bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("ERRROR binding socket:%s\n", strerror(errno));
    }

    listen(sock, 5);
    clilength = sizeof(client_address);
    printf("Now accepting connections in port %d\n", portno);

    while(1) {
        newsock = accept(sock, (struct sockaddr *) &client_address, (socklen_t *) &clilength);
        if(newsock < 0) {
            printf("Error accepting connection: %s\n", strerror(errno));
            return 1;
        }
        if(pthread_create(&handler_thread, NULL, &client_handler,(void *) &newsock) < 0) {
            printf("Failed to create thread\n");
            return 1;
        }
        pthread_detach(handler_thread);
    }

    close(sock);
    return 0;
}