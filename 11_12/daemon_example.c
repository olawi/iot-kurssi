#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>

/* Esimerkki tunkkaa syslogiin tavaraa sekunnin välein kymmenen sekunnin ajan*/

#define LOG_LEVEL    LOG_ERR
#define DAEMON_NAME  "testidemoni"
#define LOG_FACILITY LOG_DAEMON

void daemonize (void) {

  pid_t pid;

  /* fork the daemon child */
  pid = fork();


  if(pid > 0) {
    /* This thread is the parent, exit to create daemon  */
    printf("Parent in daemonize, exiting! \n");
    exit(EXIT_SUCCESS);
  }

  /* only the child/daemon from here on */
  
  /* create new session */
  if(setsid() < 0) {
    printf("Error:%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  /* change working dir */
  if(chdir("/") < 0) {
    printf("Failed to cwd \n");
    exit(EXIT_FAILURE);
  }

  /* change umask to none */
  umask(0);

  /* close standard io */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
    
}

int main(int argc, char **argv) {

  int i;
  pid_t pid, ppid;

  openlog(DAEMON_NAME, LOG_PID, LOG_FACILITY);
  daemonize();

  pid = getpid();
  ppid = getppid();

  for(i=0; i<10; i++) {
    syslog(LOG_LEVEL, "Iteration %i, pid=%i, ppid=%i \n",i , pid, ppid);
    sleep(1);
  }

  syslog(LOG_LEVEL, "Daemon done");
  return 0;
}


