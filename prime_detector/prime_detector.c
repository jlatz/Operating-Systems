/*
 * This program confirms if a number is prime and has a time limit to confirm
 * John Latz/Omar Rosario 4/9/2019 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

int flag = 0;
void* isOne(void* arg){
	long long *prime_ptr = (long long*) arg;
	long long primeNum = *prime_ptr;

	if(primeNum == 1){
		flag = 1;
		printf("confirmed composite\n");
	}
	pthread_exit(0);
}
void* isTwo(void* arg){
        long long *prime_ptr = (long long*) arg;
        long long primeNum = *prime_ptr;

        if(primeNum == 2){
		flag = 1;
                printf("confirmed composite\n");
        }
        pthread_exit(0);
}
void* isEven(void* arg){
        long long *prime_ptr = (long long*) arg;
        long long primeNum = *prime_ptr;

        if(primeNum%2 == 0 && primeNum != 2){
		flag = 1;
                printf("confirmed composite\n");
        }
        pthread_exit(0);
}

long long mulmod(long long a, long long b, long long mod)
{
    long long x = 0,y = a % mod;
    while (b > 0){
        if (b % 2 == 1){    
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}
/* 
 *  * modular exponentiation
 *   */
long long modulo(long long base, long long exponent, long long mod)
{
    long long x = 1;
    long long y = base;
    while (exponent > 0){
        if (exponent % 2 == 1){
            x = (x * y) % mod;
	}
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}
 
/*
 *  * Miller-Rabin Primality test, iteration signifies the accuracy
 *   */
int Miller(long long p,int iteration, time_t beginning, int seconds)
{
 
    int i;
    long long s;
    if (p < 2){
        return 0;
    }
    if (p != 2 && p % 2==0){
        return 0;
    }
    s = p - 1;
    while (s % 2 == 0){
        s /= 2;
    }
    for (i = 0; i < iteration; i++){
        long long a = rand() % (p - 1) + 1, temp = s;
        long long mod = modulo(a, temp, p);

	time_t check;
	time(&check);
	if((check - beginning) >= seconds){
		printf("unknown\n");
		exit(0);
	}
        while (temp != p - 1 && mod != 1 && mod != p - 1){
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0){
            return 0;
        }
    }
    return 1;
}
int main(int argc, char **argv)
{
	time_t beginning;
	time(&beginning);

	if(argc < 2){
		printf("Usage: %s <num1> <num2>\n", argv[0]);
		exit(-1);
	}
	//int num_args = argc - 1;
	int iteration = 10;
	long long prime = atoll(argv[1]);

	int seconds = 10000000; 
	if (argc > 2){
		seconds = atol(argv[2]);
	}
	//printf("max time available is: %d\n", seconds);
	
	pthread_t tid[4];
	int i;
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_create(&tid[0], &attr, isOne, &prime);
	pthread_create(&tid[1], &attr, isTwo, &prime);
	pthread_create(&tid[2], &attr, isEven, &prime);
	

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);

	time_t end;
	if(flag == 1){
		time(&end);
		printf("program ran for %ld seconds\n", end - beginning);	
		exit(0);
	}

	if(Miller(prime, iteration, beginning, seconds)){
		time(&end);
		printf("confirmed prime\n");
		printf("program ran for %ld seconds\n", end - beginning);
	}
	else{
		time(&end);
		printf("confirmed composite\n");
		printf("program ran for %ld seconds\n", end - beginning);
	}
	return 0;
}
