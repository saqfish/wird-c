#define SIZE_JUZ 30
#define SIZE_MAQRA 8
#define SIZE_MAQRAS 240

typedef struct {
	int parent;
	int number;
	int start;
	int end;
	int status;
	unsigned long date;
} Maqra;

typedef struct {
	int *pages;
	int number;
	Maqra *maqras[SIZE_MAQRA];
} Juz;

enum type {
	PAGE = 1,
	MAQRA = 2,
	JUZ = 4,
};

int generate();
int readdb();
int writedb();

Maqra *getmaqrabypage(int);
Maqra *getmaqra(int);

void pjuzes(Juz *);
void pmaqra(Maqra *);
void pdate(Maqra *);
void plist();
int cmpms(const void *, const void *); 

void freendie(char *);
