#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define STR_MAQRA "Maqra" 
#define STR_JUZ "Juz" 
#define STR_PAGES "Pages" 
#define STR_STATUS "Time since" 
#define STATUS_MAX 6
#define MINTIME 691200 // 8 days

#define VLINE "─" 
#define HLINE "│" 
#define SPACE " " 

#define HEADER_RTOP_CORNER "┐"
#define HEADER_RBTM_CORNER "┤"
#define HEADER_LTOP_CORNER "┌"
#define HEADER_LBTM_CORNER "├"
#define FOOTER_RCORNER "┘"
#define FOOTER_LCORNER "└"

int raw, jflag, pflag, sflag;

void phdr();
void pftr();
void prstr(char *);
void pmul(char* , int);
char *gstatus(int);
void cprintf(char *, char *, ...);
