#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
	Juz *p;
	Maqra *m;
	int opt, type, page;
	char *chkptr, *str;
	time_t tme = time(NULL);

	type = 0;
	opterr = 0;

	while ((opt = getopt(argc,argv, "oihaj:m:p:")) !=-1){
		switch (opt){
			case 'a':
				add = 1;
				tme = time(NULL);		
				break;
			case 'o':
				spawn = 1;
				break;
			case 'i':
				verbose = 1;
				break;
			case 'm': type = MAQRA;
				  str = optarg;
				  break;
			case 'p': type = PAGE;
				  str = optarg;
				  break;
			case 'j': type = JUZ;
				  str = optarg;
				  break;
			case 'h': 
				  die(hlpstr);
				  break;
			default: die(usgstr);
		}
	}

	if(!generate()) die("Error generating");
	if(!readdb()) freendie("Couldn't read db");

	if (type){

		long value = strtol(str, &chkptr, 10);

		if(type == PAGE) {
			if(value < 1 || value > 599) 
				freendie("Bad input. Page must be 1-599");
			page = value;
			m = getmaqrabypage(page);
			p = juzes[m->parent];
			ppage(m, page);
		}else if(type == MAQRA) {
			if(value < 1 || value > 240) 
				freendie("Bad input. Maqra must be 1-240");
			m = getmaqra(value);
			p = juzes[m->parent];
			if(add){
				m->status = 1;
				m->date = (unsigned long)tme;
			}
			pmaqra(m);
			if(m->status) pdate(m);
		}else if(type == JUZ){ 
			if(value < 1 || value > 30) 
				freendie("Bad input. Juz must be 1-30");
			long indx = value-1;
			p = juzes[indx];
			pjuzes(p);
		}


		if(spawn) {
			if(fork() == 0){
				char pstr[3];
				int pchk = type == PAGE ? page : m->start;
				sprintf(pstr, "%d", pchk + offset);

				char *cmd[] = {pdfcmd[0],pdfcmd[1], pstr, pdfcmd[2], NULL};

				execvp(pdfcmd[0], cmd); 

				freendie("Launch failed");
			}
		}

		if(add) if(!writedb()) freendie("Couldn't write to db");

	}else {
		plist();
	}
	freendie(NULL);
	return EXIT_SUCCESS;
}

Maqra *
getmaqra(int maqra){
	Juz *p;
	Maqra *m;
	int jindx, mindx;

	int mzero = maqra-1;

	jindx = (int) mzero / SIZE_MAQRA;
	mindx = mzero % SIZE_MAQRA;
	p = juzes[jindx];
	m = p->maqras[mindx];

	if(m->number == mzero) return m;

	return NULL;
}

Maqra *
getmaqrabypage(int page){
	Juz *p;
	Maqra *m;
	int jindx;

	jindx = (int) page / 20;
	if(jindx>0) jindx--;

	p = juzes[jindx];

	for(int j=0;j<SIZE_MAQRA;j++){
		m = p->maqras[j];
		if(page >= m->start && page <= m->end) 
			return m;
	}
	return NULL;
}

int 
generate(){
	Juz *p;
	Maqra *m;
	int count = 0;

	for(int i=0;i<SIZE_JUZ;i++){
		juzes[i] = malloc(sizeof(*juzes[i]));		
		if(juzes[i] == NULL) return 0;

		p = juzes[i];
		p->number=i;

		for(int j=0;j<SIZE_MAQRA;j++){
			int end = count <= SIZE_MAQRAS ? (count + 1) : count;

			juzes[i]->maqras[j] = malloc(sizeof(*juzes[i]->maqras[j]));

			m = p->maqras[j];
			m->parent = p->number;
			m->number = count;
			m->start = pages[count];
			m->end = pages[end];
			count++;
		}
	}
	return 1;
}

int
readdb(){
	FILE *fd;
	Juz *p;
	Maqra *m;
	int maqra, status;
	unsigned long date;

	fd = fopen(dbfile, "r+");
	if(fd == NULL) return 0;

	while(!feof(fd)){
		int chk, jindx, mindx;
		chk=fscanf(fd, "%d:%lu:%d", &maqra, &date, &status);

		if(chk!=3) break;

		jindx = (int) maqra / SIZE_MAQRA;
		mindx = maqra % SIZE_MAQRA;
		p = juzes[jindx];
		m = p->maqras[mindx];

		m->status = status;
		m->date = date;
	}
	fclose(fd);
	return 1;
}

int
writedb(){
	FILE *fd;
	Juz *p;
	Maqra *m;

	fd = fopen(dbfile, "w+");
	if(fd == NULL) return 0;

	for(int i=0;i<SIZE_JUZ;i++)
		for(int j=0;j<SIZE_MAQRA;j++){
			p = juzes[i];
			m = p->maqras[j];
			fprintf(fd, "%d:%lu:%d\n", m->number, m->date, m->status);
		}
	fclose(fd);
	return 1;
}

void
freendie(char *str){
	for(int i=0;i<SIZE_JUZ;i++){
		for(int j=0;j<SIZE_MAQRA;j++){
			free(juzes[i]->maqras[j]);
		}
		free(juzes[i]);
	}
	die(str);
}

void
ppage(Maqra *m, int page){
	printf("Page %03d: \n", page); 
	pmaqra(m);
	if(m->status) pdate(m);
}

void
pjuzes(Juz *j){
	printf("Juz %02d: \n", j->number+1); 
	for(int i=0;i<SIZE_MAQRA;i++){
		Maqra *m = j->maqras[i];
		printf("Maqra #%03d (pages %03d-%03d) ", m->number+1, m->start,m->end); 
		if(m->status) pdate(m);
		else printf("\n");
	}
}

void
pmaqra(Maqra *m){
	Juz *p = juzes[m->parent];
	printf("Maqra #%03d (juz %02d) Pages %03d-%03d\n", m->number+1, p->number+1, m->start,m->end); 
}

void
pdate(Maqra *m){
	struct tm t;
	time_t date = m->date;
	t = *localtime(&date);
	printf("Completed: %d/%d/%d\n",t.tm_mon, t.tm_mday,t.tm_year + 1900); 
}

void
plist(){

	for(int i=0;i<SIZE_JUZ;i++){
		Juz *p = juzes[i];
		for(int j=0;j<SIZE_MAQRA;j++){
			Maqra *m = p->maqras[j];
			if(m->status){
				printf("Maqra #%d ",m->number+1); 
				pdate(m);
			}
		}
	}
}
