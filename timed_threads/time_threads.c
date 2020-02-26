/*
 * This program sums integer from the command line using threads 
 * John Latz 4/1/2019 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct sum_runner_struct {
	long long limit; 
	long long answer;
};

//Thread function to generate sum of 0 to N
void* sum_runner(void* arg)
{
	struct sum_runner_struct *arg_struct = (struct sum_runner_struct*) arg;
	//type casting the void* into a pointer to the sum_runner_struct

	long long sum = 0;
	long long i;
	for(i = 0; i<= arg_struct->limit; i++){ 
		//(arg_struct->limit): 
		//access an element in the struct that im pointing to 
		sum += i;
	}
	arg_struct->answer = sum;
	pthread_exit(0);
}

int main(int argc, char **argv)
{
	time_t beginning;
	time(&beginning);

	if(argc < 2){
		printf("Usage: %s <num1> <num2> ... <num n>\n", argv[0]);
		exit(-1);
	}
	int num_args = argc - 1;	

	struct sum_runner_struct args[num_args];
	
	//Launch thread
	pthread_t tid[num_args];
	int i;
	for(i = 0; i <num_args; i++) {
		args[i].limit = atoll(argv[i + 1]); //ascii to long long atoll

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid[i], &attr, sum_runner, &args[i]);
	} 
	//sum_runner is a function pointer so do NOT use sum_runner()

	//Wait until thread is done its work
	for(i = 0; i< num_args; i++) { 
		pthread_join(tid[i], NULL);
		printf("Sum is %lld\n", args[i].answer);
	}
	time_t end;
	time(&end);
	time_t totalTime = end - beginning;
	printf("total program time is: %ld\n", totalTime);
}

