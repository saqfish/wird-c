#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "config.h"
#include "pages.h"
#include "print.h"
#include "wird.h"
#include "util.h"

Juz *juzes[SIZE_JUZ];
Maqra *wird;

char *rstr;
int spawn, add, wirdms;

int
main(int argc, char **argv){
	Juz *p;
	Maqra *m;
	int opt, type, page;
	long value;
	char *chkptr, *str;
	time_t tme = time(NULL);

	type = 0;

	while ((opt = getopt(argc,argv, "oihraj:m:p:")) !=-1){
		switch (opt){
			case 'a':
				add = 1;
				tme = time(NULL);		
				break;
			case 'o':
				spawn = 1;
				break;
			case 'i':
				jflag = 1;
				sflag = 1;
				pflag = 1;
				break;
			case 'r':
				raw = 1;
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
				  help();
				  break;
			default: usage();
		}
	}

	if(!generate()) die("Error generating");
	if(!readdb()) freendie("Couldn't read db");

	if(type) value = strtol(str, &chkptr, 10);

	switch (type){
		case PAGE:
			if(value < 1 || value > 604) 
				freendie("Bad input. Page must be 1-604");
			page = value;
			m = getmaqrabypage(page);
			rstr = pmaqra(m);
			if(m->status) sflag=1;
			if(!raw) phdr();
			prstr(rstr);
			if(!raw) pftr();
			break;
		case MAQRA:
			if(value < 1 || value > 240) 
				freendie("Bad input. Maqra must be 1-240");
			m = getmaqra(value);
			if(add){
				m->status++;
				m->date = (unsigned long)tme;
			}
			if(m->status) sflag=1;
			rstr = pmaqra(m);
			if(!raw) phdr();
			prstr(rstr);
			if(!raw) pftr();
			break;
		case JUZ:
			if(value < 1 || value > 30) 
				freendie("Bad input. Juz must be 1-30");
			long indx = value-1;
			p = juzes[indx];
			m = p->maqras[0];
			if(!raw) phdr();
			pjuzes(p);
			if(!raw) pftr();
			break;
		default: plist();
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
	Maqra *m = NULL;
	Maqra *m2 = NULL;
	int jindx;

	jindx = (int) page / 20;
	if(jindx>0) jindx--;

	for(int i=jindx;i<SIZE_JUZ;i++){
		p = juzes[i];
		for(int j=0;j<SIZE_MAQRA;j++){
			m = p->maqras[j];
			if(page >= m->start && page <= m->end)
				m2 = m;	
		}
		if(m2 != NULL) return m2;
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
		if(status) {
			wirdms++;

			if(wirdms) wird = realloc(wird, wirdms * sizeof(*wird));
			else wird = malloc(sizeof(Maqra*));

			wird[wirdms-1] = *m;
		}
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
	free(rstr);
	die(str);
}

void
pjuzes(Juz *j){
	for(int i=0;i<SIZE_MAQRA;i++){
		char *str;

		Maqra *m = j->maqras[i];
		str = pmaqra(m);

		prstr(str);
		free(str);
	}
}

char *
pmaqra(Maqra *m){
	char *str, *tstr;
	int limit = 16;

	Juz *p = juzes[m->parent];

	str = malloc(limit * sizeof(char));
	snprintf(str, limit, "M%03dS%03dE%03dJ%02d", m->number+1, m->start, m->end, p->number+1); 


	if(m->status){
		tstr = pdate(m);
		str = realloc(str, (limit+sizeof(tstr) * sizeof(char)));
		strcat(str, tstr);
		free(tstr);
	}
	return str;
}

char *
pdate(Maqra *m){
	char *tstr = malloc(20*sizeof(char));
	time_t datep, datec;

	datep = m->date;
	time(&datec); 

	int tdiff = difftime(datec, datep);
	snprintf(tstr, 100, "X%d" ,tdiff);
	return tstr;
}

void
plist(){
	int min = wirdms > 8 ? 8 : wirdms; 
	qsort(wird, wirdms, sizeof(Maqra), cmpms);
	sflag = 1;
	if(!raw) phdr();
	for(int i=0;i<wirdms && i<min;i++){
		char *str;

		Maqra m = wird[i];
		str = pmaqra(&m);

		prstr(str);
		free(str);
	}
	free(wird);
	if(!raw) pftr();
}

int
cmpms(const void *a, const void *b) {
	Maqra *ma = (Maqra *)a;
	Maqra *mb = (Maqra *)b;
	return (ma->date > mb->date) - (ma->date < mb->date);
} 
