#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "pages.h"
#include "wird.h"
#include "util.h"

Juz *juzes[SIZE_JUZ];
int spawn = 0;
int verbose = 0;

int
main(int argc, char **argv){
	int opt, t;
	char *str;
	Juz *p;
	Maqra *m;

	opterr = 0;

	while ((opt = getopt(argc,argv, ":oia:d:j:m:p:h")) !=-1){
		switch (opt){
			case 'o':
				spawn = 1;
				break;
			case 'i':
				verbose = 1;
				break;
			case 'm': t = MAQRA;
				  str = optarg;
				  break;
			case 'p': t = PAGE;
				  str = optarg;
				  break;
			case 'j': t = JUZ;
				  str = optarg;
				  break;
			case 'h': 
				  usage();
				  break;
			default: 
				  if(opt == '?') 
					  vdie("Bad argument: -%c\n", optopt);
				  else optusage(optopt);
		}
	}

	if (t){
		if(!generate()) die("Error generating mushaf");

		char *chkptr;
		int page;
		long value = strtol(str, &chkptr, 10);


		if(chkptr == str)
			die("Bad input. Maqra must be 1-240");

		if(t == PAGE) {
			if(value < 1 || value > 599) 
				die("Bad input. Page must be 1-599");
			page = value;
			m = getmaqrabypage(page);
			p = juzes[m->parent];
		}else if(t == MAQRA) {
			if(value < 1 || value > 240) 
				die("Bad input. Maqra must be 1-240");
			m = getmaqra(value);
			p = juzes[m->parent];
		}else if(t == JUZ){ 
			if(value < 1 || value > 30) 
				die("Bad input. Juz must be 1-30");
			long indx = value-1;
			p = juzes[indx];
			m = p->maqras[0];
		}

		if(verbose || !spawn) printf("Juz #%d Maqra #%d Page %d-%d\n", p->number, m->number, m->start,m->end); 

		if(spawn) {
			if(fork() == 0){
				char pstr[3];
				int pchk = t == PAGE ? page : m->start;
				sprintf(pstr, "%d", pchk + offset);

				char *cmd[] = {pdfcmd[0],pdfcmd[1], pstr, pdfcmd[2], NULL};

				execvp(pdfcmd[0], cmd); 

				die("Launch failed");
			}
		}

		for(int i=0;i<SIZE_JUZ;i++){
			for(int j=0;j<SIZE_MAQRA;j++){
				free(juzes[i]->maqras[j]);
			}
			free(juzes[i]);
		}
	}else usage();

	return EXIT_SUCCESS;
}
Maqra *
getmaqra(int maqra){
	int count = 0;
	for(int i=0;i<SIZE_JUZ;i++)
		for(int j=0;j<SIZE_MAQRA;j++){
			if(count == (maqra-1)) 
				return juzes[i]->maqras[j];
			count++;
		}
	return NULL;
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
			int end = count <= SIZE_MAQRAS ? (count + 1) : count;

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

