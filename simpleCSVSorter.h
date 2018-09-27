#ifndef SIMPLECSVSORTER_H
#define SIMPLECSVSORTER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Define our values to read and write from
#define STDIN 0
#define STDOUT 1
#define STDERR 2

/*typedef union valuePtr {
	int *intKey;
	char *charKey;
} Key;*/
typedef struct record {
	void *ptr; //void ptr to the data that mergesort.c utilizes to sort by (will be cast to int or char depending on the comparator function specified)
	char *beginning; //char pointer to the start of the row of characters
	char *end; //char pointer to the start of the remainder of the characters that occur after the data we are sorting on in the original row
} Record;

// Mergesort prototype headers for .c file to use
// int* parameter will be the function pointer
// to the appropriate comparator function
int mergesort(Record*, int, int, int*);
int merge(Record*, int, int, int, int*);

//intComparator and strComparator prototype headers for mergesort.c to use
//Pointer to intComparator is passed into mergesort function for records containing numeric values
//Pointer to strComparator is passed into mergesort function for records containing alphabetic values
//Both functions take void* paramters for the two pieces of data being compared, which are then cast in the function bodies depending on
//expected type. Allows for Record struct to contain one simplistic void*ptr that is cast as necessary when passed to the correct comparator function 
int intComparator(void*, void*);
int strComparator(void*, void*);

#endif