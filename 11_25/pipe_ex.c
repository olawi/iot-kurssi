#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int child_action(int pipefd) {
    // write to pipe

    int m;
    char buffer[256];
    memset(buffer, 0, 256);

    printf("Reading stdin\n");
    // read from stdin to buffer
    fgets(buffer,255,stdin);

    printf("Child sending message\n");

    // write to pipe
    m = write(pipefd, buffer, strlen(buffer));

}

int main (int argc, char **argv) {
    int pipefd[2], n;
    int pid;
    char buffer[256];

    n = pipe(pipefd);
    if(n < 0) {
        printf("Error creating pipe. Error:%s\n",strerror(errno));
        return 1;
    }
    pid = fork();
    if(pid == 0) {
        child_action(pipefd[1]);
        printf("Child process exits now.\n");
        return 0;
    }
    printf("Waiting for message\n");
    // clear the buffer and read pipe
    memset(buffer, 0 ,256);
    n = read(pipefd[0], buffer, 256);

    if(n > 0) {
        printf("Message from child:\n%s\n", buffer);
    }

    printf("Parent process exits now.\n");
    return 0;
}