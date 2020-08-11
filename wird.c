#include <stdio.h>
#include <stdlib.h>
#include "wird.h"

int
main(){
	Juz *juzes[SIZE_JUZ];
	for(int i=0;i<SIZE_JUZ;i++){
		juzes[i] = malloc(sizeof(*juzes[i]));		
		juzes[i]->number = i;
		for(int j=0;j<SIZE_MAQRAH;j++){
			juzes[i]->maqrahs[j] = malloc(SIZE_MAQRAH * sizeof(Maqrah*));		
			juzes[i]->maqrahs[j]->number = j;
		}
	}


	for(int i=0;i<SIZE_JUZ;i++){
		for(int j=0;j<SIZE_MAQRAH;j++){
			free(juzes[i]->maqrahs[j]);
		}
		free(juzes[i]);
	}

	printf("Count: %d\n", SIZE_MAQRAH * SIZE_JUZ);

	return 0;
}
