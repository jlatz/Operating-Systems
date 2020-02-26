//Brie and John

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <setjmp.h>
#include <fcntl.h>
#include <setjmp.h>
#include <errno.h>

#define MAX_SIZE_BYTES 4000000
jmp_buf buf;

void function() {
	printf("In function\n");
	
	printf("jumping to point set by setjmp\n");
	if(execl("./prog2", "prog2", NULL) == -1) {
		printf("%s\n", strerror(errno));
	}
	longjmp(buf, 1);
}
int main() {
//creating new virtual memory area
									
	char *filepath = "./prog2";
	int fd = open(filepath, O_RDWR);
	char *addr;
													 
	if((addr = mmap(NULL, MAX_SIZE_BYTES, PROT_READ | PROT_EXEC, MAP_FILE | MAP_SHARED, fd, 0)) == MAP_FAILED) {
		printf("MAP_FAILED");
		exit(0);
	}
													 							
	printf("%p\n", addr);
	//we are convinced you cannot do this as you can read the article below
	//https://stackoverflow.com/questions/12409908/invoking-a-function-main-from-a-binary-file-in-c
	if(setjmp(buf)) {
		printf("returned to initial position set by setjmp\n");
	}
	else {
		printf("calling function\n");
		function();
	}
		
}
