#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {

    struct addrinfo hints, *res;
    int n, sock, err;
    char buffer[256];

    if(argc < 3) {
        printf("Please provide server address and port\n");
        printf("ex: simple_client localhost 5000\n");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family  = AF_INET;
    err = getaddrinfo(argv[1], argv[2], &hints, &res);
    if(err != 0 ) {
        printf("Error getting address:%s\n", gai_strerror(err));
        return 1;
    }

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if(connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
        printf("Cannot connect to server:%s\n", strerror(errno));
        return 1;
    }

    do {
        memset(buffer, 0, 256);
        printf("Enter message:\n");
        fgets(buffer, 256, stdin);
        n = write(sock, buffer, strlen(buffer));
    } while(n>0);
    return 0;

}