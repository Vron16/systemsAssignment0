#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<float.h>
#include "simpleCSVSorter.h"

//called by the main file and launches the process of sorting
//creates temp array and iterates through array of pointers and moves nulls to start
//copies all non-null entries after the null entries and calls mergesort on non-null portion
void sortLaunch (Record **recordsStart, int right, int (*compareFcnPtr)(void *, void *)) {
	Record *temp[right];
	int nullIndex = 0;
	int i;
	for (i = 0; i < right; i++) {
		Record *rec = *(recordsStart + i);
		if (rec->key == NULL) {
			temp[nullIndex] = rec;
			nullIndex++;
		}
	}
	int nonNullIndex = nullIndex;
	for (i = 0; i < right; i++) {
		Record *rec = *(recordsStart + i);
		if (rec->key != NULL) {
			temp[nonNullIndex] = rec;
			nonNullIndex++;
		}
	}
	for (i = 0; i < right; i++) {
		*(recordsStart + i) = temp[i];
		//write(STDOUT,temp[i]->line, sizeof(char)*strlen(temp[i]->line));
		//write(STDOUT, recordsStart[i]->line, sizeof(char)*strlen(recordsStart[i]->line));
	}

	mergesort(recordsStart, nullIndex, right-1, compareFcnPtr);
}
//recursive mergesort function that sorts between provided indices
//does not sort if left index is the same as the right index (one-element array)
void mergesort (Record **recordsStart, int left, int right, int (*compareFcnPtr)(void *, void *)) {
	//as long as mergesort is working on a valid subsection of the array of records (left pointer is less than the right pointer)
	if (left < right) {
		int mid = (left + right)/2; //calculate middle index of the array of records
		mergesort(recordsStart, left, mid, compareFcnPtr); //recursively sort the left half of the array of records
		mergesort(recordsStart, mid + 1, right, compareFcnPtr); //recursively sort the right of the array of records
		merge(recordsStart, left, mid, right, compareFcnPtr); //merge the two halves of the array by using the merge helper function
	}
}
//called by mergesort function to merge two sorted subarrays
//creates two temp arrays to contain elements on the two sorted halves
//then merges into the original array by comparing elements from each half with each other
//if one array is traversed before the other, contents of the nontraversed array simply copied to original in order they appear
void merge (Record **recordsStart, int left, int mid, int right, int (*compareFcnPtr)(void*, void*)) {
	int leftSize = mid - left + 1;
	int rightSize = right - mid;
	Record *leftHalf[leftSize];
	Record *rightHalf[rightSize];
	int i;
	for (i = 0; i < leftSize; i++) {
		leftHalf[i] = *(recordsStart + left + i);
	}
	for (i = 0; i < rightSize; i++) {
		rightHalf[i] = *(recordsStart+mid+i+1); 
	}
	int leftIndex = 0;
	int rightIndex = 0;
	int mergedIndex = left;

	while (leftIndex < leftSize && rightIndex < rightSize) {
		Record *leftTemp = leftHalf[leftIndex];
		Record *rightTemp = rightHalf[rightIndex];
		//write(STDOUT,rightTemp->key,sizeof(char)*strlen(rightTemp->key));
		//write(STDOUT,leftTemp->key,sizeof(char)*strlen(leftTemp->key));
		//if (compareFcnPtr == NULL)
		//	write(STDERR,"AH!",sizeof(char)*3);
		int comparison = compareFcnPtr(leftTemp->key, rightTemp->key);
		//write(STDOUT,&comparison,sizeof(*int));
		if (comparison > 0) {
			*(recordsStart + mergedIndex) = rightTemp;
			//records[mergedIndex] = rightTemp;
			mergedIndex++;
			rightIndex++;
		}
		else if (comparison < 0) {
			*(recordsStart + mergedIndex) = leftTemp;
			//records[mergedIndex] = leftTemp;
			mergedIndex++;
			leftIndex++;
		}
		else {
			*(recordsStart + mergedIndex) = leftTemp;
			//records[mergedIndex] = leftTemp;
			mergedIndex++;
			leftIndex++;
			*(recordsStart + mergedIndex) = rightTemp;
			//records[mergedIndex] = rightTemp;
			mergedIndex++;
			rightIndex++;
		}
	}

	while (leftIndex < leftSize) {
		*(recordsStart + mergedIndex) = leftHalf[leftIndex];
		//records[mergedIndex] = leftHalf[leftIndex];
		mergedIndex++;
		leftIndex++;
	}

	while (rightIndex < rightSize) {
		*(recordsStart + mergedIndex) = rightHalf[rightIndex];
		//records[mergedIndex] = rightHalf[rightIndex];
		mergedIndex++;
		rightIndex++;
	}

	//write(STDOUT, recordsStart[0]->line,sizeof(char)*strlen(recordsStart[0]->line));
	//write(STDOUT, recordsStart[1]->line,sizeof(char)*strlen(recordsStart[1]->line));

}

/*int intComparator (void* data0, void* data1) {
	return (atoi(data0) - atoi(data1));
}*/
//called for any situation where the keys to be sorted are ints
//takes in two void data pointers and converts them to longs
//then returns 1 if first arg is bigger, -1 if vice versa, 0 if equal
int intComparator (void *data0, void* data1) {
	long one = atol(data0);
	long two = atol(data1);

	if (one > two)
		return 1;
	else if (one < two) 
		return -1;
	else
		return 0;
}
//called for any situation where the keys to be sorted are strings
//takes in two void data pointers and calls strcmp on them
//returns the output of strcmp, which is positive if first arg is bigger than second, negative if vice versa, 0 if equal
int strComparator (void* data0, void* data1) {
	return strcmp((const char*)data0, (const char*)data1);
}
//called for any situation where keys to be sorted are doubles
//takes in two void data pointers and converts them into doubles
//if difference is less than machine epsilon (floating-point error), treated as equal and return 0
//if difference is less than 0 return -1
//otherwise return 1, meaning first arg is larger than second
int doubleComparator(void* data0, void* data1) {
	double one = atof(data0);
	double two = atof(data1);
	
	if (-0.00001 < one - two && one - two < 0.00001){ // represents the machine epsilon
		return 0;
	} else if (one - two < 0){
		return -1;
	} else {
		return 1;
	}
}

// Note: this code was grabbed from StackOverflow, by user Adam Rosenfield.
// While we did not create this code, we understand how it works and will
// be utilizing it to handle the generalized case of the program
// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return NULL; // This we changed, to allow us to sort on NULL values instead of an empty string

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

// Takes a LinkedList and converts it into an array of Records
// Also frees the pointers during creation
Record **convertToArray(Node *head, int numEntries){
	Record **myRecords = (Record **)malloc(sizeof(Record *)*numEntries);
	Node *current = head;

	int i;
	for (i = 0; i < numEntries; i++){
		myRecords[i] = (Record *)malloc(sizeof(Record));
		myRecords[i] = current->data;
		//Node *prev = current;
		current = current->next;
		//free(prev); // garbage collection
	}

	return myRecords;
}

// Resizes a string buffer by reallocating double the size
int resize(char **buffer){
	*buffer  = realloc(*buffer, strlen(*buffer)*sizeof(char)*4);
	return 0;
}
