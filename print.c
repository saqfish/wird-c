#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "print.h"

#define STR_MAQRA "Maqra" 
#define STR_JUZ "Juz" 
#define STR_PAGES "Pages" 


#define WIDTH 7
#define WIDTH_INFO 31

#define VLINE "─" 
#define HLINE "│" 
#define SPACE " " 

#define HEADER_RTOP_CORNER "┐"
#define HEADER_RBTM_CORNER "┤"
#define HEADER_LTOP_CORNER "┌"
#define HEADER_LBTM_CORNER "├"
#define FOOTER_RCORNER "┘"
#define FOOTER_LCORNER "└"

int lens = strlen(STR_JUZ) + strlen(STR_MAQRA) + strlen(STR_PAGES);

void
phdr(){
	int max = info ? WIDTH_INFO: WIDTH;
	int padding = 9;
	int nmax = max - lens -padding;

	printf("%s", HEADER_LTOP_CORNER);
	pmul(VLINE,max);
	printf("%s\n", HEADER_RTOP_CORNER);

	printf("%s %s %s",HLINE ,STR_MAQRA, HLINE);
	if (info) {

		printf(" %s %s %s ", STR_JUZ, HLINE, STR_PAGES);
		pmul(SPACE,nmax);
		printf(" %s\n",HLINE);
	} else printf("\n");


	printf("%s",HEADER_LBTM_CORNER);
	pmul(VLINE,max);
	printf("%s\n",HEADER_RBTM_CORNER);
}

void
pftr(){
	int max = info ? WIDTH_INFO: WIDTH;
	printf("%s", FOOTER_LCORNER);
	pmul(VLINE,max);
	printf("%s\n", FOOTER_RCORNER);
}
void 
prstr(char *rstr){
	int m,s,e,j,x;
	int nlen;

	int nmax, padding;

	if(raw) printf("%s", rstr);
	else{
		sscanf(rstr, "M%03dS%03dE%03dJ%02dX%d", &m,&s,&e,&j,&x);
		printf("%s",HLINE);
		cprintf(COLOR_YELLOW, " %03d ", m);
		// if (strlen(rstr) > 16) cprintf(COLOR_BLUE, " %03d", x);
		nlen = strlen(STR_MAQRA)-3;
		pmul(SPACE,nlen);
		printf("%s",HLINE);
		if(info) {

			cprintf(COLOR_YELLOW, " %02d ", j);

			nlen = strlen(STR_JUZ)-2;
			pmul(SPACE,nlen);
			printf("%s",HLINE);

			cprintf(COLOR_YELLOW, " %03d-%03d ",s,e);
			
			nmax = (lens - strlen(STR_PAGES));
			pmul(SPACE,nmax);
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
