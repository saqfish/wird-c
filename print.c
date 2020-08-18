#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "print.h"

int width;

void
phdr(){
	int mlen, jlen, plen, slen;
	int spacing;

	spacing = 2;

	mlen = strlen(STR_MAQRA) + spacing;
	width+= mlen;

	if(jflag) {
		jlen = strlen(STR_JUZ) + spacing;
		width+= jlen;
		width++;
	}

	if(pflag) {
		plen = strlen(STR_PAGES) < 7 ? 7 : strlen(STR_PAGES);
		plen+= spacing;
		width+= plen;
		width++;
	}

	if(sflag) {
		slen = strlen(STR_STATUS) + spacing;
		width+= slen;
		width++;
	}

	printf("%s", HEADER_LTOP_CORNER);
	pmul(VLINE,width);
	printf("%s\n", HEADER_RTOP_CORNER);

	printf("%s %s %s ",HLINE ,STR_MAQRA, HLINE);
	if (jflag) printf("%s %s ", STR_JUZ, HLINE);
	if (pflag) printf("%-*s %s ", 7, STR_PAGES, HLINE);
	if (sflag) printf("%s %s ", STR_STATUS, HLINE);
	printf("\n");


	printf("%s",HEADER_LBTM_CORNER);
	pmul(VLINE,width);
	printf("%s\n",HEADER_RBTM_CORNER);
}

void
pftr(){
	printf("%s", FOOTER_LCORNER);
	pmul(VLINE,width);
	printf("%s\n", FOOTER_RCORNER);
}

void 
prstr(char *rstr){
	int m,s,e,j,x;
	char *mclr;

	if(raw) printf("%s", rstr);
	else{
		mclr = COLOR_RESET;

		sscanf(rstr, "M%03dS%03dE%03dJ%02dX%d", &m,&s,&e,&j,&x);

		if (strlen(rstr) > 15) mclr = COLOR_GREEN;

		printf("%s",HLINE);
		cprintf(mclr, " %03d ", m);

		pmul(SPACE, strlen(STR_MAQRA)-3);
		printf("%s",HLINE);

		if(jflag) {
			cprintf(COLOR_YELLOW, " %02d ", j);
			printf(" %s",HLINE);
		} 
		if(pflag) {
			cprintf(COLOR_YELLOW, " %03d-%03d ",s,e);
			printf("%s",HLINE);
		} 
		if(sflag) {
			printf(" %s ", STR_STATUS);
			printf("%s",HLINE);
		} 
	}

	printf("\n");
}

void
pmul(char *str, int max){
	for(int i=0;i<max;i++)
		printf("%s",str);
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
