#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

/* tehtävänantoa : */
// Aja tarpeeksi monta kertaa, että pääohjelma jää jumiin kun
// children ei mene nollaan. Tämä johtuu siitä, että SIGCHLD
// signaaleja tulee enemmän kuin kaksi kerralla. Sekä käsittelyssä
// oleva signaali, että käsittelyn aikana blokattu signaali
// on varattu ja kolmas ja siitä eteenpäin signaalit menevät "hukkaan",
// jos niitä ei ehditä käsitellä kunnolla.

/**
 * IoT-kurssi labra 2 "child signal tms"
 * @author Paavo Pietarila
 */

#define DEBUG 1
#define debug_out(msg) \
	if (DEBUG)         \
		fprintf(stderr, msg);

#define CHILD_SLEEP_TIME    1
#define CHILD_BUFFER_SIZE   32
#define CHILD_DATA_TYPE     int
#define MAX_CHILD_COUNT     10

/* Global static variables */
static int g_children = 0;
static int g_cpid_table[MAX_CHILD_COUNT];

int child_read_data(CHILD_DATA_TYPE *buffer)
/** Dummy function for child labour */
{
    int i;
    for(i=0; i < CHILD_BUFFER_SIZE; i++)
    {
        buffer[i] = rand();
    }

    /* return the amount of data written */
    return i+1;
}

void child_process_loop() 
/**
 * This is the eternal, void work loop for the child process
 */
{
    /** per-child data buffer */
	CHILD_DATA_TYPE data[CHILD_BUFFER_SIZE];
	int i;
    
    while (1)
	{
        child_read_data(data);
		fprintf(stderr, "%d ", ++i);
		sleep(CHILD_SLEEP_TIME);
	}
}

int _create_child(int pid) {
/**
 * Creates a child and puts it to work when pid is zero,
 * does nothing otherwise
 * @return pid of the child process
 */
	debug_out("ENTER create_child \n");
	if (pid == 0)
	{
		pid = fork();
		if (pid > 0)
		{
			debug_out("PARENT in create_child \n");
			// this is the parent code, return pid
			return pid;
		}
		else
		{
			debug_out("CHILD in create_child \n");
			// this is the child code, enter into eternal labour
			child_process_loop();
			// we never should get here
			debug_out("ERROR in create child code \n");
			exit(1);
		}
	}
	else
	{
		debug_out("CALLED WITH EXISTING PID in create_child \n");
		// leave things as they are
		return pid;
	}
}

int create_child() {
	/**
	 * Wrapper to create a new child if there are not too many already
	 * @return child PID, or 0 for error
	*/
	int i, pid = 0;
	int c_idx = -1;

	// first PID from the table that is zero
	i = 0;
	while((i < MAX_CHILD_COUNT) && (g_cpid_table[i] != 0)) {
		i++;
	}

	if(i < MAX_CHILD_COUNT) {
		c_idx = i;
	} else {
		debug_out("MAX NUMBER OF CHILDRED REACHED \n");
		return 0;
	}

	pid = _create_child(0);
	g_cpid_table[c_idx] = pid;
	g_children++;

	return pid;
}

int kill_child(int pid) {
/**
 * Kills and waits the child process if it exists 
 */
	int i, wstatus;
	int c_in_table = 0;
	int c_idx = -1;

	if(pid == 0) {
		// Can be called with 0 to do nothing 
		debug_out("EMPTY PID in kill_child \n");
		return 0;
	}

	// Check the pid against the global child table
	for(i=0; i < MAX_CHILD_COUNT; i++) {
		if(pid == g_cpid_table[i]) {
			c_in_table = 1;
			c_idx = i;
		}
	}

	if(!c_in_table) {
		debug_out("CHILD PID NOT EXIST in kill_child \n");
		return 0;
	}

	if (pid && c_in_table)
	// pid is non-zero and valid
	{
		debug_out("KILL in kill_child \n");
		// honey, we have a child
		kill(pid, SIGKILL);
		// let's wait for it to die
		wait(&wstatus);

		// clear the pid table entry
		g_cpid_table[c_idx] = 0;
		g_children--;
		return wstatus;
	}
	// We should never get here
	debug_out("ERROR in control \n");
	return 0;
}

int kill_all_children() {
	/** what it says */
	int i, pid, count=0;
	for(i=0; i < MAX_CHILD_COUNT; i++) {
		pid = g_cpid_table[i];
		// printf("Killing process %d \n", pid);
		if(kill_child(pid)) {
			count++;
		}
	}
	return count;
}

/* tehtävänantoa : */
// TODO: arvo luku 11-20 välillä. Printtaa ulos luku aikana, jonka lapsi nukkuu.
// Nuku arvottu aika. Printtaa ulos että lapsi kuolee ja sen jälkeen exit.
// Googleta miten C:n random toimii ellei ole tuttu


void my_sig_handler(int signum)
{
    // catch the sigchild signal and update child table and count
    g_children--;
	debug_out("* caught SIGCHLD ");
}

void init_signals()
{
	struct sigaction sa;
	sa.sa_handler = &my_sig_handler;
	sa.sa_flags = 0;
	sigfillset(&sa.sa_mask);

	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		printf("Error installing sighandler, exiting \n");
		exit(1);
	}
}

void init_() {
/** 
 * Zero buffers
 */
    int i;
    
    for(i=0; i < MAX_CHILD_COUNT; i++) {
		g_cpid_table[i] = 0;
    }

	init_signals();

}

int main(int argc, char **argv)
{

    /* tehtävänantoa : */
    // TODO: Luo sigaction struct ,  alusta mask ja flags kaikki // signaalit ja asenna signal handler SIGCHLD signaalille.

    // TODO: Forkkaa 10 lasta, lapset menevät childact() funktioon. Kasvata globaalia children arvoa
    // per luotu lapsi (valmiina arvo on 10)

    // TODO: While loopissa kunnes children arvo on taas nolla, nuku, käsittele kuollut lapsi wait komennolla, printtaa ulos tiedot lapsesta
    // Jos lapsia kuolee kolme samalla hetkellä, niin ohjelman pitäisi jäädä ikuisesti odottelemaamn

    /* testing testing */

    static CHILD_DATA_TYPE tmp_buffer[CHILD_BUFFER_SIZE];
    int pid, i;

    /* test that the buffer workx */
    int n = 0;
    n = child_read_data(tmp_buffer);

    for(i=0;i<n;i++) {
        // printf("%d ", tmp_buffer[i]);
    }

    init_();
    
    /* First create children */
 /*
    for (i=0; i <MAX_CHILD_COUNT; i++) {
		pid = _create_child(g_cpid_table[i]);
		g_cpid_table[i] = pid;
		printf("Created child process with PID %d \n", g_cpid_table[i]);
    }
*/ 

	// more testing testing
    for (i=0; i < (MAX_CHILD_COUNT + 2); i++) {
		create_child();
    }
    
	sleep(2);

	kill_all_children();
	kill_all_children();
    
}
