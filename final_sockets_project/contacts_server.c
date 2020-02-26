/*
 * contacts_server.c
 * John Latz
 * 3/25/19
 *
 */

#include <stdbool.h>
#include <sys/types.h>  // getaddrinfo()
#include <netdb.h>      // getaddrinfo()
#include <sys/socket.h> // getaddrinfo()
#include <stdio.h>      // printf()
#include <stdlib.h>     // exit()
#include <unistd.h>     // gethostname()
#include <arpa/inet.h>  // inet_ntop()
#include <string.h>     // strerror()
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <errno.h>
#include <mysql.h>

#define QLEN 128

#ifndef HOST_NAME_MAX 
	#define HOST_NAME_MAX 256 
#endif

extern int errno;

void print_ip_addresses(struct addrinfo *ai);
void print_error(char *);


int main(int argc, char *argv[]) 
{
	char *host_name;

    	if (argc == 2) {
		host_name = argv[1];		
	}
	else {
		host_name = malloc(HOST_NAME_MAX);
		memset(host_name, 0, HOST_NAME_MAX);

		if (gethostname(host_name, HOST_NAME_MAX) < 0) {
			print_error("gethostname error");
			return(-1);
		}
	}

	printf("host name: %s\n", host_name);

	struct addrinfo 	*host_ai;
	struct addrinfo 	hint;

	hint.ai_flags = 0;
        hint.ai_family = 0;
        hint.ai_socktype = SOCK_STREAM;
        hint.ai_protocol = 0;
        hint.ai_addrlen = 0;
        hint.ai_canonname = NULL;
        hint.ai_addr = NULL;
        hint.ai_next = NULL;

	if ((getaddrinfo(host_name, "contacts", &hint, &host_ai)) != 0) { 
		print_error("getaddrinfo error");
        	exit(1);
	}

	print_ip_addresses(host_ai);

    	int host_fd;
	if ((host_fd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		print_error("unable to create socket"); 
		return(-1);
	}
	printf("socket created [%d]\n", host_fd);

        if (bind(host_fd, host_ai->ai_addr, host_ai->ai_addrlen) < 0) {
                print_error("unable to bind to socket");
                exit(1);
        }
	printf("bind returned success\n");

	freeaddrinfo(host_ai);

        if (listen(host_fd, QLEN) < 0) {
                print_error("listen failed");
                exit(1);
        }
	printf("listen returned success\n");

	int last_client_ip_int = 0;

	struct sockaddr client_sockaddr;
	socklen_t client_sockaddr_len = sizeof(client_sockaddr);
	int token = 0;
	
	for (;;) {

		printf("waiting for connection ...\n");
        	int clfd = accept(host_fd, &client_sockaddr, &client_sockaddr_len);
        	if (clfd < 0) {
			print_error("accept error");
			exit(1); 
		}
		printf("accepted connection, socket [%d]\n", clfd);

		if (client_sockaddr.sa_family != AF_INET) {
			printf("Can not onnect with IPv6 addresses\n");
			printf("Sending -1\n");

			int mssg = -1;
			int len = send(clfd, &mssg, 4, 0);
			if (len < 0) {
				print_error("error sending data");
			}
			printf("sent %d bytes\n", len);

			close(clfd);
			continue;
		}
		
		char *choice;
		choice = malloc(4);
		memset(choice , 0 , 4);

		if(recv(clfd, choice, 4, 0) < 0){
			printf("Error reciving data: %s\n", strerror(errno));
		}
		int num = atoi(choice);
		if(num == 1){
		
			char *firstName;
			char *lastName;
			char *email;
			char *charPhone;

			firstName = malloc(24);
			lastName = malloc(48);
			email = malloc(36);
			charPhone = malloc(8);
	
		
			memset(firstName, 0, 24);
			memset(lastName, 0, 48);
			memset(email, 0, 36);
			memset(charPhone, 0, 8);

	
			int count = 0;

			if(recv(clfd, firstName, 24, 0) < 0){
				printf("Error reciving data: %s\n", strerror(errno));
			}
			if(recv(clfd, lastName, 48, 0) < 0){
				printf("Error reciving data: %s\n", strerror(errno));
			}
			if(recv(clfd, email, 36, 0) < 0){
				printf("Error reciving data: %s\n", strerror(errno));
			}
			if(recv(clfd, charPhone, 8, 0) < 0){
				printf("Error reciving data: %s\n", strerror(errno));
			}


			char nullFirst[4] = "0x01";
			char nullLast[4] = "0x02";
			char nullRepeat[4] = "0x03";
			char success[4] = "0x00";
		

			MYSQL *con = mysql_init(NULL);
			char *hostname = "rds-mysql-test.cvroqekjn5qb.us-east-1.rds.amazonaws.com";
			char *username = "jlatz";
			char *password = "Penguins1";
			char *schema = "contacts";

			if (con == NULL){
				fprintf(stderr, "%s\n", mysql_error(con));
				exit(1);
  			}

  			if (mysql_real_connect(con, hostname, username, password, schema, 0, NULL, 0) == NULL){
      				fprintf(stderr, "%s\n", mysql_error(con));
      				mysql_close(con);
      				exit(1);
  			}


  			int phone = atoi(charPhone);

  			char entry[200];
  			sprintf(entry, "INSERT INTO Contacts_T VALUES('%s','%s','%s',%d)", firstName, lastName, email, phone);
		
			bool flag = false;
  			if (mysql_query(con, entry)){
      				fprintf(stderr, "%s\n", mysql_error(con));
				flag = true;
				
			}
			
  			mysql_close(con);


			if(strlen(firstName) == 0){
       		         	count = 10;
       		        }        
                	else if(strlen(lastName) == 0){
                        	count = 20;
                	}
			else if(flag == true){
				count = 30;
			}
                	else{
                        	count = 40;
			}

			switch(count){
				case 10:
					if(send(clfd, nullFirst, 4, 0) < 0){
						printf("Error sending data: %s\n", strerror(errno));
					}
					break;
				case 20:
					if(send(clfd, nullLast, 4, 0) < 0){
						printf("Error sending data: %s\n", strerror(errno));	
					}
					break;
				case 30:
					if(send(clfd, nullRepeat, 4, 0) < 0){
						printf("Error sending data: %s\n", strerror(errno));	
					}
					break;
				default:
					if(send(clfd, success, 4, 0) < 0){
						printf("Error sending data: %s\n", strerror(errno));
					}
					break;
			}
			
		}
		else if(num == 2){

			char *getContacts;
                        getContacts = malloc(4);
                        memset(getContacts, 0, 4);


			if(recv(clfd, getContacts, 4, 0) < 0){
				printf("Error reciving data: %s\n", strerror(errno));
			}

			MYSQL *con = mysql_init(NULL);
                        char *hostname = "rds-mysql-test.cvroqekjn5qb.us-east-1.rds.amazonaws.com";
                        char *username = "jlatz";
                        char *password = "Penguins1";
                        char *schema = "contacts";

                        if (con == NULL){
                                fprintf(stderr, "%s\n", mysql_error(con));
                                exit(1);
                        }

                        if (mysql_real_connect(con, hostname, username, password, schema, 0, NULL, 0) == NULL){
                                fprintf(stderr, "%s\n", mysql_error(con));
                                mysql_close(con);
                                exit(1);
                       }

			if (mysql_query(con, "SELECT * FROM Contacts_T")){
      				fprintf(stderr, "%s\n", mysql_error(con));
      				mysql_close(con);
      				exit(1);
  			}

  			MYSQL_RES *result = mysql_store_result(con);

  			if (result == NULL) {
      				fprintf(stderr, "%s\n", mysql_error(con));
      				mysql_close(con);
      				exit(1);
 			}

  			int num_fields = mysql_num_fields(result);
			
			if(send(clfd, &num_fields, 4, 0) < 0){
				printf("Error sending data: %s\n", strerror(errno));
			}

  			MYSQL_ROW row;


			int numEntries = 0;

			char *fields[100];
			int index = 0;
  			while (row = mysql_fetch_row(result)) {
      				for(int i = 0; i < num_fields; i++) {
					if(row[i] == NULL){
						fields[index] = "NULL";
					}
					else{
						fields[index] = row[i];
					}
					//fields[index] =  row[i] ? row[i] : "NULL";
					index++;
          				//printf("%s ", row[i] ? row[i] : "NULL");
      				}
				numEntries++;
      				//printf("\n");
  			}
			if(send(clfd, &numEntries, 4, 0) < 0){
				printf("Error sending data: %s\n", strerror(errno));	
			}
				
			int j = 0;
			while(j < numEntries * num_fields){
				if(send(clfd, fields[j], 4*116, 0) < 0){
					printf("Error sending data: %s\n", strerror(errno));	
				}
				j++;
			}
				
  			mysql_close(con);
			
		}


		close(clfd);
	} 
	close(host_fd);
	
}

void print_ip_addresses(struct addrinfo *host_ai)
{
	struct addrinfo	*next_ai; 
	unsigned long *ipv4_addr;
	unsigned char *ipv6_addr;

	char ip_str[INET6_ADDRSTRLEN];

	for (next_ai = host_ai; next_ai != NULL; next_ai = next_ai->ai_next) {

		memset(ip_str, '\0', INET6_ADDRSTRLEN);

		if (next_ai->ai_addr->sa_family == AF_INET) {
			printf("IPv4 ");
			ipv4_addr  = (unsigned long*) &(((struct sockaddr_in*)next_ai->ai_addr)->sin_addr);
			inet_ntop(AF_INET, ipv4_addr, ip_str, sizeof(ip_str));
		}
		else {
			printf("IPv6 ");
			ipv6_addr = (unsigned char*) &(((struct sockaddr_in6*)next_ai->ai_addr)->sin6_addr);
			inet_ntop(AF_INET6, ipv6_addr, ip_str, sizeof(ip_str));
		}

		printf("IP address: %s\n", ip_str);
	}
}

void print_error(char *str) {
	printf("%s: %s\n", str, strerror(errno));
}
