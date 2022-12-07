/* Ben Elleman and Chris Barlas*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "define.h"
#include "structs.h"
#include "plab4.h"

/* Runs a C-LOOK Disk Scheduling Algorithm Simulation */
int main()
{
	/*Cylinders represented as an array of pointers to int*/
	int **cylinders;
	/*Stores the results of the sort operation*/
	SortResult *results;

	/*Simulation setup*/	
	srand(time(NULL));
	cylinders = fillCylinders();
	printf("Contents of cylinders:\n");
	printCylinders(cylinders);

	/*Start of C-LOOK simulation*/
	results = sort(cylinders);

	/*Print results*/
	printf("Contents of sorted lower queue:\n");
	printArray(results->lower);

	printf("Contents of sorted higher queue:\n");
	printArray(results->higher);

	printf("Contents of balance: %d\n", results->balance);

	combine(results->lower, results->higher, results->balance);
	printf("Contents of sorted queue:\n");
	printArray(results->higher);

	freeCylinders(cylinders);
	freeResults(results);
	
	return 0;
}

/*Creates an array of CYLNUM number of int pointers pointing to
 * an int array of size CYLLEN
 * returns - array of int pointers*/
int **fillCylinders() {
	int **ptr_arr = malloc(sizeof(int*) * CYLNUM);
	int x, y;
	for (x = 0; x < CYLNUM; x++) {
		ptr_arr[x] = malloc(sizeof(int) * CYLLEN);
		for (y = 0; y < CYLLEN; y++) {
			ptr_arr[x][y] = rand() % MAX;
		}
	}
	return ptr_arr;
}

/*Frees all dynamically allocated memory for cylinders
 * cylinders - the cylinders to be freed*/
void freeCylinders(int **cylinders)
{
	int x, y;
	for (x = 0; x < CYLNUM; x++)
	{
		free(cylinders[x]);
	}
	free(cylinders);
}

/*Prints all the cylinders
 * arr - the cylinders to be printed*/
void printCylinders(int **arr)
{
	int x, y;
	for (x = 0; x < CYLNUM; x++)
	{
		for (y = 0; y < CYLLEN; y++) printf("%d\n", arr[x][y]);
	}
}

/*Prints a given array
 * arr - the 1-dimensional array to be printed*/
void printArray(int *arr)
{
	int i;
	for(i = 0; i < CYLNUM * CYLLEN; i++) printf("%d\n", arr[i]);
}

/*Combine the higher and lower arrays into one
 * lower - the 1-dimensional array of the lower elements
 * higher - the 1-dimensional array of the higher elements
 * balance - How many elements are already in higher*/

void combine(int lower[CYLNUM * CYLLEN], int higher[CYLNUM * CYLLEN], int balance)  {
	int z;
	for (z = 0; z + balance < CYLNUM * CYLLEN; z++){
		higher[z + balance] = lower[z];
	}
}

/*Decides based on HIGHER or LOWER SORT_TYPE whether to place elements
* from arr into buffer
* arr - the array containing all unsorted cylinders
* type - determines whether we are looking at HIGHER or LOWER values
* buffer - array to place filtered values into
* returns - number of elements moved*/
int filter(int **arr, enum SORT_TYPE type, int *buffer)
{
	int x, y;
	int buffer_index = 0;

	for (x = 0; x < CYLNUM; x++)
	{
		for(y = 0; y < CYLLEN; y++)
		{
			int currentInt = arr[x][y];
			if(type == HIGHER && currentInt >= POS)
			{
				buffer[buffer_index] = currentInt;
				buffer_index++;
			}
			else if (type == LOWER && currentInt < POS)
			{
				buffer[buffer_index] = currentInt;
				buffer_index++;
			}
		}
	}

	return buffer_index;
}

/*Stuffs a given queue so that all elements of interest will be at the front once sorted
 * arr - the array to be "stuffed"*/
void stuff(int *arr)
{
	int k;
	for(k = 0; k < CYLNUM * CYLLEN; k++) arr[k] = MAX + 1;
}

/*Swap helper function for selectionSort referenced from GeeksForGeeks
 * Source: https://www.geeksforgeeks.org/selection-sort/
 * first - the first element to be swapped
 * second - the second element to be swapped*/
void swap(int *first, int *second)
{
    int temp = *first;
    *first = *second;
    *second = temp;
}

/*Worker function to perform Selection Sort.
* Code referenced from GeeksForGeeks and extended by Ben Ellemen and Chris Barlas.
* Source: https://www.geeksforgeeks.org/selection-sort/
 * param - pointer to workerInput struct
 * returns - unused*/
void *worker(void *param)
{
	/*Variables used for selection sort*/
	int n, i, j, min_idx, index = 0;

	/*Variables used for filter operation*/
	int balance;
	int *buffer;
	int **whole_arr;
	workerInput *input;
	enum SORT_TYPE type;

	/*Create a local array and place filtered values in there*/
	input = param;
	whole_arr = input->arr;
	type = input->type;
	buffer = malloc(sizeof(int) * (CYLNUM * CYLLEN));
	stuff(buffer);
	balance = filter(whole_arr, type, buffer);
	if(input->type == HIGHER) input->result->balance = balance;

	/*Start selection sort*/
	n = CYLLEN * CYLNUM;
	/*One by one move boundary of unsorted subarray*/
	for (i = 0; i < n - 1; i++) {
		/*Find the minimum element in unsorted array*/
		min_idx = i;
		for (j = i + 1; j < n; j++) if (buffer[j] < buffer[min_idx]) min_idx = j;
		/* Swap the found minimum element with the first element*/
		swap(&buffer[min_idx], &buffer[i]);
		if (buffer[i] < POS) index++;
	}

	/*Assign correct int pointer to our sorted array*/
	if(type == LOWER) input->result->lower = buffer;
	else input->result->higher = buffer;

	free(param);
	pthread_exit(0);
}

/*High level sort method that sorted a given array using task level parallelism.
 * Code for creating threads referenced from CSE 2431 Lab 3. Author: Chris Barlas
 * arr - the array to be sorted
 * returns - pointer to SortResult containing sorted data*/
void *sort(int **arr)
{
	int i, j;
	enum SORT_TYPE type = LOWER;
	pthread_attr_t attr;
	pthread_t thread_id[THREAD_NO];

	/*Create SortResult space in memory*/
	SortResult *result = malloc(sizeof(SortResult));

	/*Setting attributes for threads*/
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setschedpolicy(&attr, SCHED_OTHER);

	/*Create threads: one for higher sort, one for lower sort*/
	for(i = 0; i < THREAD_NO; i++)
	{
		workerInput *input = malloc(sizeof(workerInput));
		input->arr = arr;
		input->type = type;
		input->result = result;
		pthread_create(&thread_id[i], &attr, worker, (void*)input);
		type = HIGHER;
	}

	/*Wait for threads to complete*/
	for(i = 0; i < THREAD_NO; i++) pthread_join(thread_id[i], NULL);

	return result;
}

/*Frees all dynamically allocated memory for results
 * ptr - The struct that holds all the data to be freed*/
void freeResults(SortResult *ptr)
{
	free(ptr->lower);
	free(ptr->higher);
	free(ptr);
}

