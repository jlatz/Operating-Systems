#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h> /* used for fork */
#include <sys/types.h> /* for pid_t */
#include <errno.h> 
#include <string.h>

int main(){
	
	struct rlimit rlim;

	int resource = RLIMIT_NPROC;
	getrlimit(resource, &rlim);
	
	printf("Parent: before setrlimit: rlim_cur (%d), rlim_max(%d)\n", rlim.rlim_cur, rlim.rlim_max);
	
	rlim.rlim_cur = 7;

	setrlimit(resource, &rlim);
	//getrlimit(resource, &rlim);
	printf("Parent: after setrlimit: rlim_cur (%d), rlim_max(%d)\n", rlim.rlim_cur, rlim.rlim_max);

	int counter = 0;
	char buff[8];
	while(1){

		printf("Parent: creating child (%d)\n", counter);
		pid_t pid;
		counter++;
		if((pid=fork()) == 0){ //child process
			printf("Parent: child (%d) successfully created with pid: %d\n", counter, getpid());
			snprintf(buff, sizeof(buff), "%d", counter);
			execl("./child", "child", buff, NULL);

		}
		else if(pid < 0){
			printf("Error creating child: %s\n", strerror(errno));
			sleep(2);
			exit(0);
		}

		
		
	}
	return 0;


}


