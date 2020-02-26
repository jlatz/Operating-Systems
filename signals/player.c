/* 
 * Owner: John Latz
 * Date: 2/2/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

//global variable
int health_level = 3;

void decrease_health(){
	health_level--;
	printf("HEALTH: %d\n", health_level);
	if(health_level == 0){
		printf("Player killed: %d\n", getpid());
		if(kill(getpid(), SIGKILL) < 0){
			printf("Error registering SIGKILL: %s\n", strerror(errno));
		}
		exit(0);
	}
}

void increase_health(){
	if(health_level < 5){
		health_level = health_level + 1;
		printf("HEALTH: %d\n", health_level);
	}
	else{
		printf("HEALTH is MAX\n");
	}
}
	
int main(){

	if (signal(SIGUSR1, decrease_health) == SIG_ERR ) {
		printf("Error registering handler: %s\n", strerror(errno));
	}
	if (signal(SIGUSR2, increase_health) == SIG_ERR){
		printf("Error registering handler: %s\n", strerror(errno));
	}

	while(1){
		pause();
	}
	return 0;
}
