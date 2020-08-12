#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wird.h"


int
main(int argc, char **argv){
	int opt;
	while ((opt = getopt(argc,argv, "a:")) !=-1){
		switch (opt){
			case 'a':
				printf("%s", argv[2]);
				break;
			default: usage();
		}
	}

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

	printf("0/%d\n", SIZE_MAQRAH * SIZE_JUZ);

	return EXIT_SUCCESS;
}

void
usage(){
	printf("Usage: -n N\n");
	die();
}

void
die(){
	printf("Bye\n");
	exit(EXIT_FAILURE);
}
