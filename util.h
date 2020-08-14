#ifndef UTIL_G

char *usgstr = "usage: wird [-aoi] [-p page] [-m maqra] [-j juz]";
char *hlpstr = 
"-a add maqra entry to db\n"
"-o open in mushaf\n"
"-i display maqra information\n"
"-p page: select maqra by page\n"
"-m maqra: select maqra\n"
"-j juz: select select the first maqra in juz";

#else

#endif /*UTIL_G_*/

void vdie(char *, ...);
void die(char *);

