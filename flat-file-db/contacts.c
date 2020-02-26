#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAXADDRLEN 256
#define BUFLEN 128

extern int errno;

void print_error(char *);

int main(int argc, char *argv[]) {
	struct addrinfo *host_ai;
	struct addrinfo hint;
	int sockfd, err;
    
	if (argc != 2)
        	print_error("usage: client hostname");
    
	hint.ai_flags = 0;
    	hint.ai_family = AF_INET;
    	hint.ai_socktype = SOCK_STREAM;
    	hint.ai_protocol = 0;
    	hint.ai_addrlen = 0;
    	hint.ai_canonname = NULL;
    	hint.ai_addr = NULL;
    	hint.ai_next = NULL;

	if ((err = getaddrinfo(argv[1], "flatfiledb", &hint, &host_ai)) != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(err));
		return 0;
	}

	printf("creating socket\n");
	if ((sockfd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		print_error("Error creating socket");
	}
	printf("socket created\n");

	printf("attempting Connection\n");
	if (connect(sockfd, host_ai->ai_addr, host_ai->ai_addrlen) != 0) {
		printf("can't connect to %s\n", argv[1]);
		print_error("Error connecting to server");
	}
	printf("connection made...\n"); 

	char choice[4];
	int num;
	while(1){
		printf("1. Insert New Contact \n2. Exit\n");
		
		fgets(choice, 4, stdin);
		choice[strlen(choice) -1] = '\0';
		num = atoi(choice);
	
		switch(num){
			case 1: ; 
				char firstName[24];
				char lastName[48];
				char email[36];
				char phone[8];
				int intPhone;
 
				printf("Enter a first name: ");
				fgets(firstName, 24, stdin);
				firstName[strlen(firstName) - 1] = '\0';
		
				printf("Enter a last name: ");
				fgets(lastName, 48, stdin);
				lastName[strlen(lastName) - 1] = '\0';

				printf("Enter a email: ");
				fgets(email, 36, stdin);
				email[strlen(email) - 1] = '\0';

				printf("Enter a phone number: ");
				fgets(phone, 8, stdin);
				phone[strlen(phone) - 1] = '\0';

				fflush(stdin); //flushes from the keyboard, refreashes for each iteration of the while loop
				
				if(send(sockfd, firstName, 24, 0) < 0){
					printf("Error sending data: %s\n", strerror(errno));
				}
				if(send(sockfd, lastName, 48, 0) < 0){
					printf("Error sending data: %s\n", strerror(errno));
				}
				if(send(sockfd, email, 36, 0) < 0){
					printf("Error sending data: %s\n", strerror(errno));
				}
				if(send(sockfd, phone, 8, 0) < 0){
					printf("Error sending data: %s\n", strerror(errno));
				}
			
				char result[4];
				if(recv(sockfd, result, 4, 0) < 0){
					printf("Error reciving data: %s\n", strerror(errno));
				}
				const char* from = result;
  				char *subResult = (char*) malloc(5);
  				strncpy(subResult, from, 4);
				printf("%s\n", subResult); //get a subset of just the return code
	
				break;
		
			case 2:
				printf("EXIT\n");
				exit(1);
			default:
				printf("Enter a number between 1 and 2\n");
				break;
		}	
	}

	freeaddrinfo(host_ai);
	close(sockfd);
	
    	exit(1);
}


void print_error(char *str) {
	printf("%s: %s\n", str, strerror(errno));
	exit(1);
}

// END OF FILE
