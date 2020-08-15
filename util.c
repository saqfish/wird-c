#define UTIL_G

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

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
