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
int add = 0;

int
main(int argc, char **argv){
	int opt, t;
	char *str;
	Juz *p;
	Maqra *m;

	opterr = 0;
	

	while ((opt = getopt(argc,argv, ":oiahj:m:p:")) !=-1){
		switch (opt){
			case 'a':
				add = 1;
				break;
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

		if(!readdb()) die("Couldn't read db");

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
			if(add){
				m->status= 1;
				m->date[0] = 8;
				m->date[1] = 13;
				m->date[2] = 20;
			}
		}else if(t == JUZ){ 
			if(value < 1 || value > 30) 
				die("Bad input. Juz must be 1-30");
			long indx = value-1;
			p = juzes[indx];
			m = p->maqras[0];
		}

		if(verbose || !spawn) {
				printf("Status: %d\n", m->status); 
				printf("Date: %d/%d/%d\n", m->date[0],m->date[0],m->date[2]); 
			printf("Juz #%d Maqra #%d Page %d-%d\n", p->number, m->number, m->start,m->end); 
		}

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

		if(add) if(!addtodb()) die("Couldn't read db");

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

int
readdb(){
	FILE *fd;
	int maqra, mon, day, yr, status;
	fd = fopen("db", "r+");
	if(fd == NULL) return 0;

	while(!feof(fd)){
		int chk, jindx, mindx;
		chk=fscanf(fd, "%d:%d/%d/%d:%d", &maqra, &mon, &day, &yr, &status);
		if(chk!=5) break;

		jindx = (int) maqra / 8;
		mindx = maqra % 8;
		
		juzes[jindx]->maqras[mindx]->status = status;
		juzes[jindx]->maqras[mindx]->date[0] = mon;
		juzes[jindx]->maqras[mindx]->date[1] = day;
		juzes[jindx]->maqras[mindx]->date[2] = yr;

	}
	fclose(fd);
	return 1;
}
int
addtodb(){
	FILE *fd;
	int maqra, mon, day, yr, status;
	fd = fopen("db", "w+");
	if(fd == NULL) return 0;

	
	for(int i=0;i<SIZE_JUZ;i++)
		for(int j=0;j<SIZE_MAQRA;j++){
			fprintf(fd, "%d:%d/%d/%d:%d\n", juzes[i]->maqras[j]->number - 1, juzes[i]->maqras[j]->date[0], juzes[i]->maqras[j]->date[1], juzes[i]->maqras[j]->date[2], juzes[i]->maqras[j]->status);
		}
	fclose(fd);

	return 1;

}
