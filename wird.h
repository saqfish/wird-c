#define SIZE_JUZ 30
#define SIZE_MAQRAH 8

typedef struct {
	int parent;
	int number;
	int start;
	int end;
} Maqra;

typedef struct {
	int *pages;
	int number;
	Maqra *maqras[SIZE_MAQRAH];
} Juz;

int generate();
Maqra *getmaqrabypage(int page);
