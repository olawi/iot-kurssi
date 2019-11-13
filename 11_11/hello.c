#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char **argv) {

  printf("Starting execution ... \n");
	
  int result;
  char *path;

  int pid;

  printf("Creating child....\n");

  pid = fork();

  if(pid == 0) {
    printf("This is the child process with fork return value 0\n");
    printf("PID: %i PPID: %i \n", getpid(), getppid());
  }
  
  if(pid > 0) {
    printf("Parent created child with %i \n", pid);
    printf("PID: %i PPID: %i \n", getpid(), getppid());

    wait(NULL);
    printf("Parent exiting now \n");
    return 0;
  }

  printf("This is the child \n");
  printf("PID: %i PPID: %i \n", getpid(), getppid());

  sleep(5);
  printf("Child exiting now \n");

  return 0;
  
}
