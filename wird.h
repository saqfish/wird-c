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

enum type {
	PAGE = 1,
	MAQRA = 2,
	JUZ = 4,
};

int generate();

Maqra *getmaqrabypage(int);
Maqra *getmaqra(int);
