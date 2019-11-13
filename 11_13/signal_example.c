#include <signal.h>
#include <stdio.h>
#include <unistd.h>


/* ks. man signal (2) */
int count = 0;

void my_sig_handler(int sig) {
  count++;
}

int main(int argc, char **argv) {
  struct sigaction sa;

  sa.sa_handler = &my_sig_handler;
  sa.sa_flags = 0;
  sigfillset(&sa.sa_mask);

  if(sigaction(SIGINT, &sa, NULL) == -1) {
    printf("Error installing sighandler, exiting \n");
    return 1;
  }
  
  while(count < 10) {
    printf("Waiting for SIGINT. Count: %d\n", count);
    sleep(10);
  }
  
  printf("Stopping listening, exit\n");
  return 0;
}
