#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "wird.h"
#include "util.h"

int
main(int argc, char **argv){
	opterr = 0;
	int opt;
	while ((opt = getopt(argc,argv, ":a:d:")) !=-1){
		switch (opt){
			case 'a':
				printf("%s", argv[2]);
				break;
			case 'd':
				printf("%s", argv[2]);
				break;
			default: 
				if(opt == '?'){
					fprintf(stderr, "Bad argument: -%c\n", optopt);
					die();
				} else optusage(optopt);
		}
	}

	if(!generate()){
		fprintf(stderr, "Error generating mushaf\n");
		die();
	}else printf("30/%d\n", SIZE_MAQRAH * SIZE_JUZ);

	return EXIT_SUCCESS;
}

int 
generate(){
	Juz *juzes[SIZE_JUZ];

	for(int i=0;i<SIZE_JUZ;i++){
		juzes[i] = malloc(sizeof(*juzes[i]));		
		if(juzes[i] == NULL) return 0;
		juzes[i]->number = i;
		for(int j=0;j<SIZE_MAQRAH;j++){
			juzes[i]->maqrahs[j] = malloc(sizeof(Maqrah*));		
			if(juzes[i]->maqrahs[j] == NULL) return 0;
			juzes[i]->maqrahs[j]->number = j;
		}
	}


	for(int i=0;i<SIZE_JUZ;i++){
		for(int j=0;j<SIZE_MAQRAH;j++){
			free(juzes[i]->maqrahs[j]);
		}
		free(juzes[i]);
	}

	return 1;
}

