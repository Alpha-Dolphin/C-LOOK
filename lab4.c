/* Ben Elleman and Chris Barlas*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "define.h"
#include "shared.h"

#include "lab4.h"

int main()
{
	/*Would've been easier to use a 1-dimensional array but that felt disingenuous*/
	int cylinders[CYLNUM][CYLLEN];
	int lower[CYLNUM * CYLLEN];
	int higher[CYLNUM * CYLLEN];
	int balance, x, y;
	/*Simulation setup*/
	
	srand(time(NULL));
	eEQ(lower,&stuff);
	eEQ(higher,&stuff);
	eEC(cylinders, &fill);
	printf("Contents of cylinders:\n");
	eEC(cylinders, &print);	

	/*Start of actual C-scan code*/

	/*Annoying that this can't cleanly fit into eEC*/
	for (x = 0; x < CYLNUM; x++) {
		for (y = 0; y < CYLLEN; y++) {
			balance += assign(cylinders[x][y], lower, higher);
		}
	}
	selectionSort(lower);
	printf("Contents of sorted lower queue:\n");
	eEQ(lower, &print);
	selectionSort(higher);
	printf("Contents of sorted higher queue:\n");
	eEQ(higher, &print);
	combine(lower, higher, balance);
	printf("Contents of sorted queue:\n");
	eEQ(higher, &print);
	
	return 0;
}

/*Stuffs a given queue so that all elements of interest will be at the front once sorted
 * unused - the value of the current entry, unused in this function but necessary
 * to maintain proper function signature
 * returns - the stuffing value*/
int stuff(int unused) {
	return MAX + 1;
}

/*Assign a given element to a spot in the appropiate queue
 * element - the element to be assigned
 * returns - the element that was assigned*/
int assign(int element, int lower[CYLNUM * CYLLEN], int higher[CYLNUM * CYLLEN]) {
	static int h = 0, l = 0;
	if (element >= POS) {
		higher[h] = element;
		h++;
		return 1;
	} else {
		lower[l] = element;
		l++;
	}
	return 0;
	/*if (z >= sizeof cylinders / sizeof cylinders[0][0]) z = 0;*/
}

/*Combine the higher and lower arrays into one*/
void combine(int lower[CYLNUM * CYLLEN], int higher[CYLNUM * CYLLEN], int balance)  {
	int z;
	for (z = 0; z + balance < CYLNUM * CYLLEN; z++){
		higher[z + balance] = lower[z];
	}
}

