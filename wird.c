#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pages.h"
#include "wird.h"
#include "util.h"

Juz *juzes[SIZE_JUZ];

int
main(int argc, char **argv){
	opterr = 0;
	int opt;

	if(!generate()){
		fprintf(stderr, "Error generating mushaf\n");
		die();
	}

	while ((opt = getopt(argc,argv, ":a:d:p:h")) !=-1){
		switch (opt){
			case 'a':
				printf("%s", argv[2]);
				break;
			case 'd':
				printf("%s", argv[2]);
				break;
			case 'p':{
					 long value = strtol(argv[2], NULL, 10);
					 Maqrah *m = getmaqrabypage(value);
					 Juz *p = juzes[m->parent];
					 printf("Juz: %d \nMaqra: %d\nPage: %d-%d\n",p->number, m->number, m->start, m->end);
					 break;
				 }
			case 'h': 
				 usage();
				  break;
			default: 
				  if(opt == '?'){
					  fprintf(stderr, "Bad argument: -%c\n", optopt);
					  die();
				  } else optusage(optopt);
		}
	}

	for(int i=0;i<SIZE_JUZ;i++){
		for(int j=0;j<SIZE_MAQRAH;j++){
			free(juzes[i]->maqrahs[j]);
		}
		free(juzes[i]);
	}

	return EXIT_SUCCESS;
}

Maqrah *
getmaqrabypage(int page){
	for(int i=0;i<SIZE_JUZ;i++){
		for(int j=0;j<SIZE_MAQRAH;j++){
			if(page >= juzes[i]->maqrahs[j]->start && page <= juzes[i]->maqrahs[j]->end) 
				return juzes[i]->maqrahs[j];
		}
	}
	return NULL;
}

int 
generate(){

	int count = 0;
	for(int i=0;i<SIZE_JUZ;i++){
		juzes[i] = malloc(sizeof(*juzes[i]));		
		if(juzes[i] == NULL) return 0;
		juzes[i]->number = i+1;
		for(int j=0;j<SIZE_MAQRAH;j++){
			int end = count <= 240 ? (count + 1) : count;

			juzes[i]->maqrahs[j] = malloc(sizeof(Maqrah*));		
			if(juzes[i]->maqrahs[j] == NULL) return 0;

			juzes[i]->maqrahs[j]->parent = i;
			juzes[i]->maqrahs[j]->number = count+1;
			juzes[i]->maqrahs[j]->start = pages[count];
			juzes[i]->maqrahs[j]->end = pages[end];

			count++;
		}
	}



	return 1;
}

