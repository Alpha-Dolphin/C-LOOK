/*Chris Barlas and Ben Elleman*/

/*To distinguish whether we are sorting cylinders less than POS or greater than POS*/
enum SORT_TYPE { LOWER, HIGHER };

/*Struct to hold the results of sort operation
 * lower - holds the sorted array of cylinders less than POS
 * higher - holds the sorted array of cylinders greater than POS
 * balance - reports how many elements are in higher for use of combine function
 */
typedef struct 
{
	int *lower;
	int *higher;
	int balance;
} SortResult;

/*Struct to give every worker the data needed to start sorting the unsorted cylinders
 * arr - pointer to the array of pointers to ints (the cylinders)
 * type - whether we should do LOWER or HIGHER sort
 * result - where the threads should put the resulting sorted array
 */
typedef struct
{
	int **arr;
	enum SORT_TYPE type;
	SortResult *result;
} workerInput;