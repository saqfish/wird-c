#define SIZE_JUZ 30
#define SIZE_MAQRA 8
#define SIZE_MAQRAS 240

typedef struct {
	int parent;
	int number;
	int start;
	int end;
} Maqra;

typedef struct {
	int *pages;
	int number;
	Maqra *maqras[SIZE_MAQRA];
} Juz;

void maqrainfo(char *);
void juzinfo(char *);
void pageinfo(char *);

int generate();

Maqra *getmaqrabypage(int);
Maqra *getmaqra(int);
