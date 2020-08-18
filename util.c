#define UTIL_G

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void
usage(){
	printf("%s\n", usgstr);
	exit(EXIT_FAILURE);
}
void
help(){
	printf("%s\n", usgstr);
	printf("%s\n", hlpstr);
	exit(EXIT_SUCCESS);
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
	if(str) fprintf(stderr, "%s\n", str);
	exit(EXIT_FAILURE);
}
