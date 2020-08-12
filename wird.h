#define SIZE_JUZ 30
#define SIZE_MAQRAH 8

typedef struct {
	int number;
} Maqrah;

typedef struct {
	int *pages;
	Maqrah *maqrahs[SIZE_MAQRAH];
	int number;
} Juz;

void die();
void usage();
