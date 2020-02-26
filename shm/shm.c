/*
 * John Latz
 * Omar Rosario
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

void print_error(const char*, int);

int main(int argc, char* argv[]){

	char username[20];
	printf("please enter a username followed by a conversation starter: ");
	scanf("%s\n", username);

	int tokid = 0;
	char *filepath = "/tmp";


	key_t key;
	if ((key = ftok(filepath, tokid)) == -1)
		print_error("Can not create token", errno);

	printf("Token id: %d\n", key);


	long bufsz = sysconf(_SC_PAGESIZE);
	printf("Page size: %ld\n", bufsz);

	int shmid;
	if ((shmid = shmget(key, bufsz, IPC_CREAT | 0600)) == -1)
		print_error("Can not create shared memory", errno);

	printf("Shared memory id: %d\n", shmid);


	char *shm = NULL;
	if ((shm = shmat(shmid,0,0)) == (void *)-1)
		print_error("Unable to attach to shared memory", errno);

	
	char *temp;
	while(true){
		int shmlen = strlen(shm);
		printf("Shared memory bytes used: %d\n", shmlen);

		char message[30];
		printf("Enter message: ");
		scanf("%s", message);

		strcat(temp, username);	
		strcat(temp, " : ");
		strcat(temp, message);

		char *cbuf = temp;
		int cbuflen = strlen(cbuf);
		printf("Length of string to write: %d\n", cbuflen);


		if(shmlen + cbuflen + 1 < bufsz){
			printf("Before write (%lu): %s \n", strlen(shm), shm);

			memcpy(shm + shmlen, cbuf, cbuflen + 1);
	
			printf("After write (%lu): %s \n", strlen(shm), cbuf);

		}
		else {
			printf("Buffer full\n");
		
		}
		int i; 
		for(i = 0; i<strlen(temp); i++){
			temp[i] = '\0';
		}
		int j;
		for(j = 0; j<strlen(message); j++){
			message[j] = '\0';
		}
		sleep(8);
		

	}
	if (shmdt(shm) == -1)
		print_error("Unable to detach shared memory", errno);

	if(shmctl(shmid, IPC_RMID, 0) == -1)
		print_error("Can not validate shared memory", errno);

	exit(0);
}
	
void print_error(const char* str, int code){
	printf("%s: %s\n", str, strerror(code));
	exit(-1);
}

