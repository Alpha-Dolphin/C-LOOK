enum SORT_TYPE { LOWER, HIGHER };

typedef struct 
{
	int *lower;
	int *higher;
	int balance;
} SortResult;

typedef struct
{
	int **arr;
	enum SORT_TYPE type;
	SortResult *result;
} workerInput;