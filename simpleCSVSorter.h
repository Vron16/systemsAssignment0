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
	char *line; //char pointer to the start of the line
	void *key; //void pointer to the start of the key
} Record;

typedef struct node {
	Record *data; // actual data that we need
	struct node *next; // pointer to next node in list
} Node;

// Mergesort prototype headers for .c file to use
// int* parameter will be the function pointer
// to the appropriate comparator function
void sortLaunch(Record **, int, int(*f)(void *, void *));
void mergesort(Record **, int, int, int (*f)(void *, void *));
void merge(Record **, int, int, int, int (*f)(void *, void *));

//intComparator and strComparator prototype headers for mergesort.c to use
//Pointer to intComparator is passed into mergesort function for records containing numeric values
//Pointer to strComparator is passed into mergesort function for records containing alphabetic values
//Both functions take void* paramters for the two pieces of data being compared, which are then cast in the function bodies depending on
//expected type. Allows for Record struct to contain one simplistic void*ptr that is cast as necessary when passed to the correct comparator function 
int intComparator(void *, void *);
int strComparator(void *, void *);
int doubleComparator(void *, void *);

// Prototype for trim method in our library class
char *trimwhitespace(char *);

// Prototypes for convertToArray and resize methods in our library class
Record **convertToArray(Node *, int);
int resize(char **);

#endif
