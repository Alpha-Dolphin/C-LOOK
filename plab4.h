/* Chris Barlas and Ben Elleman */

void combine(int lower[CYLNUM * CYLLEN], int higher[CYLNUM * CYLLEN], int balance) ;
int **fillCylinders() ;
int filter(int **arr, enum SORT_TYPE type, int *buffer);
void freeCylinders(int **cylinders);
void freeResults(SortResult *ptr);
int main();
void printArray(int *arr);
void printCylinders(int **arr);
void *sort(int **arr);
void stuff(int *arr);
void swap(int* xp, int* yp);
void *worker(void *param);
