/*
 * This program sums arguments from the command line using threads
 * example 5 = 5 + 4 + 3 + 2 + 1 => 15
 * John Latz 4/1/2019 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//The sum computed by the background thread
long long sum = 0;

//Thread function to generate sum of 0 to N
void* sum_runner(void* arg)
{
	long long *limit_ptr = (long long*) arg;
	long long limit = *limit_ptr;

	long long i;
	for(i = 0; i<= limit; i++){
		sum += i;
	}
	//sum is a global variable, so other threads can access
	
	pthread_exit(0);
}

int main(int argc, char **argv)
{
	if(argc < 2){
		printf("Usage: %s <num>\n", argv[0]);
		exit(-1);
	}
	//using background threads
	long long limit = atoll(argv[1]); //ascii to long long atoll
	
	//Thread ID:
	pthread_t tid;

	//Create attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_create(&tid, &attr, sum_runner, &limit); 
	//sum_runner is a function pointer so do NOT use sum_runner()

	//CAN DO OTHER STUFF HERE....
	//could spawn other threads, interact with user, compute other stuff
	
	//Wait until thread is done its work
	pthread_join(tid, NULL);
	printf("Sum is %lld\n", sum);
	
}

