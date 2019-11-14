#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/**
 * IoT-kurssi labra 1 "prosessimalli"
 * @author Paavo Pietarila
 */

#define DEBUG 0
#define debug_out(msg) \
	if (DEBUG)         \
		fprintf(stderr, msg);

void child_process_loop() {
/**
 * This is the eternal, void work loop for the child process
 */
	int i = 0;
	while (1)
	{
		fprintf(stderr, "\r  %d", ++i);
		sleep(1);
	}
}

int create_child(int pid) {
/**
 * Creates a child and puts it to work when pid is zero,
 * does nothing otherwise
 */
	debug_out("ENTER create_child\n");
	if (pid == 0)
	{
		pid = fork();
		if (pid > 0)
		{
			debug_out("PARENT in create_child\n");
			// this is the parent code, return pid
			return pid;
		}
		else
		{
			debug_out("CHILD in create_child\n");
			// this is the child code, enter into eternal labour
			child_process_loop();
			// we never should get here
			debug_out("ERROR in create child code\n");
			exit(1);
		}
	}
	else
	{
		debug_out("CHILD EXISTS in create_child\n");
		// leave things as they are
		return pid;
	}
}

int kill_child(int pid) {
/**
 * Kills and waits the child process if it exists 
 */
	int wstatus;
	if (pid != 0)
	{
		debug_out("KILL in kill_child\n");
		// honey, we have a child
		kill(pid, SIGKILL);
		// let's wait for it to die
		wait(&wstatus);
		return wstatus;
	}
	else
	{
		debug_out("NO CHILDREN in kill_child\n");
		// do nothing
		return 0;
	}
}

int main(int argc, char **argv) {
	int c_pid = 0; // PID of the (only) child process
	char c;
	int exit_loop = 0;

	while (!exit_loop)
	{
		printf("\r\tSelect: (s) create child (k) kill child (q) quit ");
		// read a char from STDIN
		// HOX! Also reads the newline \n
		c = getchar();

		// just flush the buffer to the end, not very pretty
		while (getchar() != '\n') { }

		switch (c)
		{
		case 's':
			// Create child process
			printf("User command \'%c\': Create child \n", c);
			c_pid = create_child(c_pid);
			break;
		case 'k':
			// Kill child process
			printf("User command \'%c\': Kill child \n", c);
			if (kill_child(c_pid))
			{
				c_pid = 0;
			}
			break;
		case 'q':
			// User command to quit
			printf("User command \'%c\': Quit \n", c);
			exit_loop = 1;
			break;
		default:
			debug_out("INVALID command in main\n");
		}
	}

	printf("Terminating child processes and exiting... \n");
	kill_child(c_pid);

	return 0;
}
