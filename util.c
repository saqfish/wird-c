#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void
usage(){
	printf("Usage: -n N\n");
	die();
}

void
optusage(char opt) {
	char *msg;
	switch (opt){
		case 'a':
			msg = "Add a maqrah";
			break;
		case 'd':
			msg = "Delete a maqrah";
			break;
	}
	fprintf(stderr,"-%c: %s\n",opt, msg);
	die();
}

void
die(){
	exit(EXIT_FAILURE);
}
