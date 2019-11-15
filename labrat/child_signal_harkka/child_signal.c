#include <signal.h>
#include <stdio.h>	
#include <sys/types.h>	
#include <unistd.h>	
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
// Aja tarpeeksi monta kertaa, että pääohjelma jää jumiin kun
// children ei mene nollaan. Tämä johtuu siitä, että SIGCHLD 
// signaaleja tulee enemmän kuin kaksi kerralla. Sekä käsittelyssä
// oleva signaali, että käsittelyn aikana blokattu signaali
// on varattu ja kolmas ja siitä eteenpäin signaalit menevät "hukkaan",
// jos niitä ei ehditä käsitellä kunnolla.
static int children = 0;

void myHandler(int signum) {
	// TODO: vähennä globaalia children arvoa yhdellä.
}

int childact(int nro) {
	// TODO: arvo luku 11-20 välillä. Printtaa ulos luku aikana, jonka lapsi nukkuu.
	// Nuku arvottu aika. Printtaa ulos että lapsi kuolee ja sen jälkeen exit.
	// Googleta miten C:n random toimii ellei ole tuttu
}	

int main(int argc, char** argv) {
	struct sigaction sa;

	// TODO: Luo sigaction struct ,  alusta mask ja flags kaikki // signaalit ja asenna signal handler SIGCHLD signaalille.
	

	// TODO: Forkkaa 10 lasta, lapset menevät childact() funktioon. Kasvata globaalia children arvoa
	// per luotu lapsi (valmiina arvo on 10)

	// TODO: While loopissa kunnes children arvo on taas nolla, nuku, käsittele kuollut lapsi wait komennolla, printtaa ulos tiedot lapsesta 
	// Jos lapsia kuolee kolme samalla hetkellä, niin ohjelman pitäisi jäädä ikuisesti odottelemaamn
}