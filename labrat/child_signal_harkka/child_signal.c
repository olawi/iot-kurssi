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
		fprintf(stderr, "\r  %d", ++i);
		sleep(CHILD_SLEEP_TIME);
	}
}

    /* tehtävänantoa : */
    // TODO: arvo luku 11-20 välillä. Printtaa ulos luku aikana, jonka lapsi nukkuu.
    // Nuku arvottu aika. Printtaa ulos että lapsi kuolee ja sen jälkeen exit.
    // Googleta miten C:n random toimii ellei ole tuttu


/* Global static variables */
static int g_children = 0;

void myHandler(int signum)
{
    // TODO: vähennä globaalia children arvoa yhdellä.
    g_children--;
}

int main(int argc, char **argv)
{
    //struct sigaction sa;

    /* tehtävänantoa : */
    // TODO: Luo sigaction struct ,  alusta mask ja flags kaikki // signaalit ja asenna signal handler SIGCHLD signaalille.

    // TODO: Forkkaa 10 lasta, lapset menevät childact() funktioon. Kasvata globaalia children arvoa
    // per luotu lapsi (valmiina arvo on 10)

    // TODO: While loopissa kunnes children arvo on taas nolla, nuku, käsittele kuollut lapsi wait komennolla, printtaa ulos tiedot lapsesta
    // Jos lapsia kuolee kolme samalla hetkellä, niin ohjelman pitäisi jäädä ikuisesti odottelemaamn

    /* testing testing */

    static CHILD_DATA_TYPE tmp_buffer[CHILD_BUFFER_SIZE];

    /* test that the buffer workx */
    int i;
    int n = 0;
    n = child_read_data(tmp_buffer);

    for(i=0;i<n;i++) {
        printf("%d ", tmp_buffer[i]);
    }

}