#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "print.h"

void 
prstr(char *rstr){
	int m,s,e,j,x;
	

	if(raw) printf("%s", rstr);
	else{
		sscanf(rstr, "M%03dS%03dE%03dJ%02dX%d", &m,&s,&e,&j,&x);
		if (strlen(rstr) > 16) cprintf(COLOR_BLUE, "%03d ", x);
		cprintf(COLOR_GREEN, "Maqra: ");
		cprintf(COLOR_YELLOW, "%03d\n", m);
		cprintf(COLOR_GREEN, "Juz: ");
		cprintf(COLOR_YELLOW, "%02d\n", j);
		cprintf(COLOR_GREEN, "Pages: ");
		cprintf(COLOR_YELLOW, "%03d-%03d\n", s,e);
	}
	if(raw) printf("\n");
}

void
cprintf(char *color, char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);

	printf(color);
	vprintf(fmt, ap);
	printf(COLOR_RESET);

	va_end(ap);
}
