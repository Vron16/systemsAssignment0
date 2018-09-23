#ifndef SIMPLECSVSORTER_H
#define SIMPLECSVSORTER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Define our values to read and write from
#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct record{
	char *key;
	char *beginning;
	char *end;
} Record;

// Mergesort prototype headers for .c file to use
// void* parameter will be the function pointer
// to the appropriate comparator function
int mergesort(Record*, int, int, void*);
int merge(Record*, int, int, int, void*);

#endif