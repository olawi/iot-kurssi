#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/**
 * IoT-kurssi labra 1 "prosessimalli"
 * @author Paavo Pietarila
 */

int main(int argc, char **argv)
{	
    int c_pid = 0; // return value from fork(); 0 for child, PID for parent
    char c;
	int exit_loop = 0;

    do
	{
		printf("Please enter command: \n");
		// read a char from STDIN
		// HOX! Also reads the newline \n
		c = getchar();

	    // jos pid == 0 lapsi printtaa printf:llä tekstin ja nukkuu
	    // sekunnin. Tämä loopissa kunnes lapsi tapetaan
	    // ns. ikuinen loop. Muista tallentaa
	    // jos c = "k" ja pid > 0, niin kill(pid,SIGKILL) ja wait().
	    // Muista myös pid = 0, kun lapsi on kuollut.

	    switch (c)
		{
		case 's':
		// Create child process 
		    printf("User command \'%c\': Create child \n", c);
		    break;
		case 'k':
		// Kill child process
		    printf("User command \'%c\': Kill child \n", c);
		    break;
		case 'q':
		// User command to quit 
		    printf("User command \'%c\': Quit \n", c);
			exit_loop = 1;
			break;
		case '\n':
		// Ignore the newline
			break;
		default:
		    printf("No such command: \'%c\' \n", c);
		}
	} while (!exit_loop);
    
    
    printf("Terminating child processes and exiting... \n");
    // tarkasta onko lapsi hengissä, jos on niin kill(pid,SIGKILL) ja
    // wait ja sitten return 0;

	return 0;
}
