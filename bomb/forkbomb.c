#include <stdio.h>
#include <unistd.h>

int main(){
	int i = 0;
	while(i < 1){
		fork();
		printf(".");
	}

	return 0;
}
