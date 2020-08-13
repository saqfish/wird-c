#define UTIL_G

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void
usage(){
	die(usgstr);
}

void
optusage(char opt) {
	char *msg;
	switch (opt){
		case 'j':
			msg = "number: set juz";
			break;
		case 'm':
			msg = "number: set maqra";
			break;
		case 'p':
			msg = "number: set page";
			break;
	}
	vdie("-%c %s\n",opt, msg);
}

void
vdie(char *fmt, ...){

	va_list ap;
	va_start(ap,fmt);

	vfprintf(stderr, fmt, ap);

	va_end(ap);
	exit(EXIT_FAILURE);
}

void
die(char *str){
	fprintf(stderr, "%s\n", str);
	exit(EXIT_FAILURE);
}
