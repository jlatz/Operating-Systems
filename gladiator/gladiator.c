/***************
 * 
 * gladiator.c 
 *
 * Eric McGregor 
 ****************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sem_helper.h"

void print_matrix(int arena_len, int *array);

int main(int argc, char* argv[]) 
{
	int semid;
        int gladiator_id;
	int arena_len;

        if (argc != 4) {
                printf("usage: %s semid gladiator_id arena_len\n", argv[0]);
                return 0;
        }

        semid = atoi(argv[1]); //what index to start with
        gladiator_id = atoi(argv[2]); //What number to use
	arena_len = atoi(argv[3]); //Dimensions
	/* Battle */
	printf("semid: %d\n",semid);
	int startIndex = 0;

	printf("gladiator id is: %d\n", gladiator_id);	
        short storage[arena_len * arena_len];
        short *array = storage;
	getAllSemaphores(semid, arena_len, array);

	while(startIndex < (arena_len * arena_len)){
		if(gladiator_id == array[startIndex++]){		
			break;
		}
		
	}//will terminate when we find what value we want to start at

	printf("start index is: %d\n", startIndex);
        
	//*(array + startIndex) = gladiator_id;

       	int x = startIndex;
	while(1){
		int j;
		int counter = 0;
		for(j = 0; j < arena_len * arena_len; j++){
			if(gladiator_id == array[j]){
				counter++;
			}
		}
		if(counter == 0){
			printf("I submit\n");
			exit(0);
		}
        	while(x > 0){
                	if((x-arena_len) >= 0){
				setSemaphore(semid,x-arena_len, gladiator_id);
                        	x = x-arena_len;
                	}else{
				setSemaphore(semid,x-1, gladiator_id);
                        	x = x-1;
                	}	
        	}

        	int y;
        	for(y=0; y<arena_len*arena_len; y++){
				setSemaphore(semid, y, gladiator_id);
                
        	}
	}
		return 0;
}
//  END OF FILE
