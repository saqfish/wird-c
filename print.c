#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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
		width+= STATUS_MAX;
		width++;
	}

	printf("%s", HEADER_LTOP_CORNER);
	pmul(VLINE,width);
	printf("%s\n", HEADER_RTOP_CORNER);

	printf("%s %s %s ",HLINE ,STR_MAQRA, HLINE);
	if (jflag) printf("%s %s ", STR_JUZ, HLINE);
	if (pflag) printf("%-*s %s ", 7, STR_PAGES, HLINE);
	if (sflag) printf("%s %*s ", STR_STATUS, STATUS_MAX + 3, HLINE);
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
	int validx;
	char *mclr;

	validx = strlen(rstr) > 15;

	if(raw) printf("%s", rstr);
	else{
		mclr = COLOR_RESET;

		sscanf(rstr, "M%03dS%03dE%03dJ%02dX%d", &m,&s,&e,&j,&x);

		if (strlen(rstr) > 15) {
			if(x >= (86400 * 3)) mclr = COLOR_RED;
			else mclr = COLOR_GREEN;
		}

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
			char *str = gstatus(x);
			if (validx) printf(" %-*s ", strlen(STR_STATUS) + STATUS_MAX, str);
			else printf(" %*s ", strlen(STR_STATUS), " ");
			printf("%s",HLINE);
			free(str);
		} 
	}

	printf("\n");
}

void
pmul(char *str, int max){
	for(int i=0;i<max;i++)
		printf("%s",str);
}

char *
gstatus(int secs){
	int days, hrs, mins;
	int ds, hs, ms, ss;

	char *str = malloc((strlen(STR_STATUS) + STATUS_MAX) * sizeof(char *));
	days = (int) secs / 86400;
	hrs = (int) secs / 3600;
	mins = (int) secs / 60;

	ds = days > 1? 4: 3;
	hs = hrs > 1? 3: 2;
	ms = mins > 1? 4: 3;
	ss = secs > 1? 4: 3;

	if(hrs >= 24) sprintf(str, "%d %.*s", days, ds, "days", "ago");
	else if(mins >= 60) sprintf(str, "%d %.*s", hrs, hs, "hrs", "ago");
	else if(secs >= 60) sprintf(str, "%d %.*s", mins, ms,  "mins", "ago");
	else sprintf(str, "%d %.*s", secs, ss,  "secs", "ago");

	return str;
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
