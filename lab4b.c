/* Ben Elleman and Chris Barlas*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "define.h"
#include "shared.h"

#include "lab4b.h"

int main()
{
	/*Would've been easier to use a 1-dimensional array but that 
	 * felt disingenuous*/
	int cylinders[CYLNUM][CYLLEN];
	int queue[CYLNUM * CYLLEN];
	int index, x, y;
	
	/*Simulation setup*/
	
	srand(time(NULL));
	eEC(cylinders,&fill);
	printf("Contents of cylinders:\n");
	eEC(cylinders,&print);	

	/*Start of actual C-scan code*/

	/*Annoying that this can't cleanly fit into eEC*/
	for (x = 0; x < CYLNUM; x++) {
		for (y = 0; y < CYLLEN; y++) {
			queue[x * CYLLEN + y] = cylinders[x][y];
		}
	}

	printf("Contents of queue:\n");
	eEQ(queue,&print);
	index = selectionSort(queue);
	printf("Contents of sorted queue:\n");
	eEQ(queue,&print);
	printf("index - %d\n", index);
	printf("Contents of sorted and shifted queue:\n");
	rotate(queue, index, CYLNUM * CYLLEN);
	eEQ(queue,&print);
	
	return 0;
}

/*Rotates a given array of size n by d indices
 * arr[] - The array to be rotated
 * d - The amount of rotates
 * n - The size of arr[] */
void rotate(int arr[], int d, int n)
{
	int rotates, shifts;
	for (rotates = 0; rotates < d; rotates++) {
		int holder = arr[0];
		for (shifts = 0; shifts < n - 1; shifts++) arr[shifts] = arr[shifts+1];
		arr[n - 1] = holder;
	}
}

/*Assign a given element to a spot in the queue
 * element - the element to be assigned
 * returns - the element that was assigned*/
int assign(int element) {
	static int z = 0;
	z++;
	if (z >= CYLNUM * CYLLEN) z = 0;
	return element;
}
