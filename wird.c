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

	while ((opt = getopt(argc,argv, ":a:d:j:m:p:h")) !=-1){
		switch (opt){
			case 'a':
				printf("%s", argv[2]);
				break;
			case 'd':
				printf("%s", argv[2]);
				break;
			case 'j':
				printf("%s", argv[2]);
				break;
			case 'm':
				printf("%s", argv[2]);
				break;
			case 'p':{
					 long value = strtol(argv[2], NULL, 10);
					 Maqra *m = getmaqrabypage(value);
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
		for(int j=0;j<SIZE_MAQRA;j++){
			free(juzes[i]->maqras[j]);
		}
		free(juzes[i]);
	}

	return EXIT_SUCCESS;
}

Maqra *
getmaqrabypage(int page){
	for(int i=0;i<SIZE_JUZ;i++){
		for(int j=0;j<SIZE_MAQRA;j++){
			if(page >= juzes[i]->maqras[j]->start && page <= juzes[i]->maqras[j]->end) 
				return juzes[i]->maqras[j];
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
		for(int j=0;j<SIZE_MAQRA;j++){
			int end = count <= SIZE_MAQRA ? (count + 1) : count;

			juzes[i]->maqras[j] = malloc(sizeof(*juzes[i]->maqras[j]));

			juzes[i]->maqras[j]->parent = i;
			juzes[i]->maqras[j]->number = count+1;
			juzes[i]->maqras[j]->start = pages[count];
			juzes[i]->maqras[j]->end = pages[end];

			count++;
		}
	}



	return 1;
}

