/*Ben Elleman*/

/*A file for all the shared functions of my two solutions*/

#include <stdio.h>
#include <stdlib.h>

#include "define.h"

#include "shared.h"

/*For-[E]ach [e]lement of the [c]ylanders
 * cylinders - The cylinders to be accessed
 * function - the function to be called for each element*/
void eEC(int cylinders[CYLNUM][CYLLEN], int(function(int))) {
	int x, y;
	for (x = 0; x < CYLNUM; x++) {
		for (y = 0; y < CYLLEN; y++) {
			cylinders[x][y] = function(cylinders[x][y]);
		}
	}
}

/*For-[E]ach [e]lement of a [q]ueue
 * queue - The queue to be accessed
 * function - the function to be called for each element*/
void eEQ(int queue[CYLNUM * CYLLEN], int(function(int))) {
	int x;
	for (x = 0; x < CYLNUM * CYLLEN; x++) {
		queue[x] = function(queue[x]);
	}
}

/*Returns a value to be filled in the cylander simulation arrays
 * unused - the value of the current entry, unused in this function but necessary
 * to maintain proper function signature
 * returns - the random value*/
int fill(int unused) {
	int result = rand() % MAX;
	return result;
}

/*Print a given element
 * element - the element to be printed
 * returns - the element printed*/
int print(int element) {
	printf("%d\n", element);
	return element;
}

/*Swap function for selectionSort 
 * xp - the first element to be swapped
 * yp - the second element to be swapped*/
void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
/*Function to perform Selection Sort. Code taken from G4G and modified by me.
 * arr[] - the array to be sorted
 * returns - the index of the first element > POS*/
int selectionSort(int arr[])
{
	int n , i, j, min_idx, index = 0;
	n = CYLNUM * CYLLEN; 
	/*One by one move boundary of unsorted subarray*/
	for (i = 0; i < n - 1; i++) {
		/*Find the minimum element in unsorted array*/
		min_idx = i;
		for (j = i + 1; j < n; j++) if (arr[j] < arr[min_idx]) min_idx = j;
		/* Swap the found minimum element with the first element*/
		swap(&arr[min_idx], &arr[i]);
		if (arr[i] < POS) index++;
	}
	return index;
}
