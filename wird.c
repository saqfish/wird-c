#include <stdio.h>
#include <stdlib.h>

struct maqrah {
	int number;
};

typedef struct maqrah Maqrah;

typedef struct {
	int number;
	Maqrah **maqrahs;
} Juz;

int
main(){
	Juz **juzes;
	juzes = malloc(sizeof(Juz*) *30);		

	for(int i=0;i<30;i++){
		juzes[i] = malloc(sizeof(Juz*));		
		juzes[i]->number = i;
		juzes[i]->maqrahs = malloc(sizeof(Maqrah*) *8);		
		for(int j=0;j<8;j++){
			juzes[i]->maqrahs[j] = malloc(sizeof(Maqrah*));		
			juzes[i]->maqrahs[j]->number = j;
		}
	}

	int count;


	for(int i=0;i<30;i++){
		for(int j=0;j<8;j++){
			count++;
			free(juzes[i]->maqrahs[j]);
		}
		free(juzes[i]);
	}

	printf("Count: %d\n", count);
}
