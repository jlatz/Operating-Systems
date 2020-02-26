#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
	int dimensions; 
	int interval;
	int id;

	
	dimensions = atoi(argv[1]);
	interval = atoi(argv[2]);
	id = atoi(argv[3]);

	int *array;
	array = (int*)malloc((dimensions * dimensions) * sizeof(int));
	memset(array, 0, (dimensions *dimensions) * sizeof(int));

	*(array + interval) = id;

	print_matrix(array, dimensions);
	
	int maxIndex = (dimensions * dimensions) - 1;
	int forwardPos = 1;
	int backwardPos = 1;
	int counter = 0;
	int column = 0;

	while(counter < maxIndex){
		if(*(array + interval - backwardPos) == 0){
			if(backwardPos == interval + 1){ 
			//had the problem that the array would print twice when it
			//was time to switch between the if loop and the else if loop 
				backwardPos++;
				continue;
			}
			*(array + interval - backwardPos) = id;
			backwardPos++;
			counter++;
			print_matrix(array, dimensions);
		}else{
			*(array + interval + forwardPos) = id;
			forwardPos++;
			counter++;
			print_matrix(array, dimensions);
		}
	}

	
	return 0;
}


print_matrix(int *array, int dimensions) { 

	int i,j;
	for(i = 0; i<dimensions*dimensions; i++){
		if(i % dimensions == 0){
			putchar('\n');
		}
	
		printf("%3d", array[i]);
	}
	putchar('\n');	
}
