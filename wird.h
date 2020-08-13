#define SIZE_JUZ 30
#define SIZE_MAQRA 8

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

int generate();
Maqra *getmaqrabypage(int page);
