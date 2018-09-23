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

#endif