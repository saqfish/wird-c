#ifndef UTIL_G

#else

char *usgstr = "usage: wird [-aoir] [-p page] [-m maqra] [-j juz]";
char *hlpstr = 
"\t-a add maqra entry to db\n"
"\t-o open in mushaf\n"
"\t-i display extra information\n"
"\t-r raw data instead of table\n"
"\t-p [page] select maqra by page\n"
"\t-m [maqra] select maqra\n"
"\t-j [juz] select maqras by juz";

#endif /*UTIL_G_*/

void usage();
void help();
void vdie(char *, ...);
void die(char *);

