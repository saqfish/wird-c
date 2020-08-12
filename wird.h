#define SIZE_JUZ 30
#define SIZE_MAQRAH 8

typedef struct {
	int parent;
	int number;
	int start;
	int end;
} Maqrah;

typedef struct {
	int *pages;
	int number;
	Maqrah *maqrahs[SIZE_MAQRAH];
} Juz;

int generate();
Maqrah *getmaqrabypage(int page);
