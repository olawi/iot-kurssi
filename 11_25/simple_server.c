#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {
    int sock, newsock, portno, clilength;
    char buffer[256];
    struct sockaddr_in server_address, client_address;
    int no_of_bytes;

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
    newsock = accept(sock, (struct sockaddr *) &client_address, (socklen_t *) &clilength);

    if(newsock < 0) {
        printf("Error accepting connection: %s\n", strerror(errno));
        return 1;
    }

    // loop read
    do {
            memset(buffer, 0, 256);
            no_of_bytes = read(newsock, buffer, 256);
            if(no_of_bytes > 0) {
                printf("incoming:\n---\n%s---\n", buffer);
            }
    } while(no_of_bytes > 0);

    close(newsock);
    close(sock);
    return 0;

}