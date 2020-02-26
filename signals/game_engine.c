/*
 * Owner: John Latz
 * Date: 2/3/19
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>


void updatelist();
void removechild(int pid);

int child[20];

int main(){
	int choice;
	int count = 0;
	pid_t pid;
	while(1){
		printf("1.Spawn player \n2.Injure player \n3.Heal Player \n4.List players \n5.Quit\n");
		scanf("%d", &choice);
		switch(choice){	
			case 1:
				pid = fork();
				if(pid == 0){ //child process
					printf("Spawned player %d\n", getpid());
					execl("./player", "player", NULL);
				
				}
				else if(pid == -1){
					printf("Error creating child: %s\n", strerror(errno));
					exit(0);
				}
				else{
					if(signal(SIGCHLD, updatelist) == SIG_ERR){
						printf("Error registering handler: %s\n", strerror(errno));
					}
				}
				child[count] = pid;
				count++;
					
				break;
			
			case 2: ;//cannot put a label before a declaration ';' fixes this issue
				
				int i;
				for(i = 0; i<count; i++){
					if(child[i] != 0){
						printf("%d\n", child[i]);
					}
				}
				int injure;
				printf("Enter a player to injure: ");
				scanf("%d", &injure);
				if(kill(injure, SIGUSR1) < 0){
					printf("Error in kill function: %s\n", strerror(errno));
				}
				break;
			
			case 3: ;
				int j;
				for(j = 0; j<count; j++){
					if(child[j] != 0){
						printf("%d\n", child[j]);
					}
				}
				int heal;
				printf("Enter a player to heal: ");
				scanf("%d", &heal);
				if(kill(heal, SIGUSR2) < 0){
					printf("Error in heal function: %s\n",strerror(errno));
				}
				break;
			
			case 4: ;
				int k;
				for(k = 0; k<count; k++){
					if(child[k] != 0){
						printf("%d\n", child[k]);
					}
				}
					 
				break;
			
			case 5:
				printf("GAME OVER\n");
				int n;
				for(n = 0; n<count; n++){
					if(kill(child[n], SIGKILL) < 0){
						printf("Error in SIGKILL signal: %s\n", strerror(errno));
					}
				}
				exit(0);
				break;
				
			default:
				printf("Enter a number 1 to 5\n");
		
		}	
	}

	return 0;
}

void updatelist(){
	pid_t pid;
	pid = wait(NULL);
	int m;
	for(m=0;m<20;m++){
		if(child[m] == pid){
			child[m] = '\0';
		}
	}	
}



