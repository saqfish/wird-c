#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "config.h"
#include "pages.h"
#include "wird.h"
#include "util.h"

Juz *juzes[SIZE_JUZ];
Maqra wird[8];

int spawn = 0;
int info = 0;
int add = 0;
int wirdms = 0;

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
				info = 1;
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
			if(value < 1 || value > 604) 
				freendie("Bad input. Page must be 1-604");
			page = value;
			m = getmaqrabypage(page);
			pmaqra(m);
		}else if(type == MAQRA) {
			if(value < 1 || value > 240) 
				freendie("Bad input. Maqra must be 1-240");
			m = getmaqra(value);
			if(add){
				m->status++;
				m->date = (unsigned long)tme;
			}
			pmaqra(m);
		}else if(type == JUZ){ 
			if(value < 1 || value > 30) 
				freendie("Bad input. Juz must be 1-30");
			long indx = value-1;
			p = juzes[indx];
			m = p->maqras[0];
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
		if(status) wird[wirdms++]=*m;
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
pjuzes(Juz *j){
	for(int i=0;i<SIZE_MAQRA;i++){
		Maqra *m = j->maqras[i];
		printf("%03d\n", m->number+1); 
	}
}

void
pmaqra(Maqra *m){
	Juz *p = juzes[m->parent];
	if(info) printf("J%02d", p->number+1); 
	printf("M%03dS%03dE%03d", m->number+1, m->start,m->end); 
	if(m->status) pdate(m);
	printf("\n");
}

void
pdate(Maqra *m){
	struct tm tp;
	time_t datep, datec;

	datep = m->date;
	time(&datec); 

	double tdiff = difftime(datec, datep);
	int days = (int) tdiff / 86400;
	int hours = (int) tdiff / 3600;

	tp = *localtime(&datep);

	printf("\n%02d%02d%dX%d",tp.tm_mon+1, tp.tm_mday,tp.tm_year + 1900, m->status); 
	if(info)
		if (days) printf("(%dD)",days); 
		else printf("(%dH)",hours); 
}

void
plist(){
	qsort(wird, wirdms, sizeof(Maqra), cmpms);
	for(int i=0;i<wirdms;i++){
		printf("%03d %d %d\n",wird[i].number+1,wird[i].status,wird[i].date); 
	}
}

int
cmpms(const void *a, const void *b) {
	return (((Maqra *)a)->date - ((Maqra *)a)->date);
} 
