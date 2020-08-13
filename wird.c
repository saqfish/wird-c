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

	if(!generate()) die("Error generating mushaf");

	while ((opt = getopt(argc,argv, ":a:d:j:m:p:h")) !=-1){
		switch (opt){
			case 'a':
				printf("%s", optarg);
				break;
			case 'd':
				printf("%s", optarg);
				break;
			case 'j':
				juzinfo(optarg);
				break;
			case 'm':
				maqrainfo(optarg);
				break;
			case 'p':
				pageinfo(optarg);
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

	for(int i=0;i<SIZE_JUZ;i++){
		for(int j=0;j<SIZE_MAQRA;j++){
			free(juzes[i]->maqras[j]);
		}
		free(juzes[i]);
	}

	return EXIT_SUCCESS;
}
void
maqrainfo(char *str){
	Maqra *m;
	Juz *p;
	char *chkptr;

	long value = strtol(str, &chkptr, 10);

	if(chkptr == str || (value < 1 || value > 599)) 
		die("Bad input. Maqra must be 1-240");

	m = getmaqra(value);
	p = juzes[m->parent];

	printf("Maqra #%d:\nJuz #%d Pages: %d-%d\n", m->number,p->number, m->start, m->end);
}

void
juzinfo(char *str)
{
	Juz *p;
	char *chkptr;

	long value = strtol(str, &chkptr, 10);

	if(chkptr == str || (value < 1 || value > 30)) 
		die("Bad input. Juz must be 1-30");

	long indx = value-1;

	printf("Juz #%d: \n", juzes[indx]->number);

	for(int j=0;j<SIZE_MAQRA;j++)
		printf("Maqra #%d Pages: %d-%d \n", juzes[indx]->maqras[j]->number, juzes[indx]->maqras[j]->start, juzes[indx]->maqras[j]->end);
}

void
pageinfo(char *str){
	Maqra *m;
	Juz *p;
	char *chkptr;

	long value = strtol(str, &chkptr, 10);

	if(chkptr == str || (value < 1 || value > 30)) 
		die("Bad input. Page must be 1-599");

	if((m = getmaqrabypage(value)) == NULL) die("Problem");
	p = juzes[m->parent];

	printf("Page #%d:\nJuz #%d Maqra #%d\n", value ,p->number, m->number);
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

