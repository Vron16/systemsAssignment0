#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "simpleCSVSorter.h"

void sortLaunch (Record *recordsStart, int right, int (*compareFcnPtr)(void *, void *)) {
	int size = right + 1;
	Record temp[size];
	int nullIndex = 0;
	int i;
	for (i = 0; i < right; i++) {
		Record rec = *(recordsStart + i);
		if (rec.ptr == NULL) {
			temp[nullIndex] = rec;
			nullIndex++;
		}
	}
	int nonNullIndex = nullIndex;
	for (i = 0; i < right; i++) {
		Record rec = *(recordsStart + i);
		if (rec.ptr != NULL) {
			temp[nonNullIndex] = rec;
			nonNullIndex++;
		}
	}
	for (i = 0; i < right; i++) {
		*(recordsStart + i) = temp[i];
	}
	mergesort(recordsStart, nullIndex, right, compareFcnPtr);
}

void mergesort (Record *recordsStart, int left, int right, int (*compareFcnPtr)(void *, void *)) {
	//as long as mergesort is working on a valid subsection of the array of records (left pointer is less than the right pointer)
	if (left < right) {
		int mid = (left + right)/2; //calculate middle index of the array of records
		mergesort(recordsStart, left, mid, compareFcnPtr); //recursively sort the left half of the array of records
		mergesort(recordsStart, mid + 1, right, compareFcnPtr); //recursively sort the right of the array of records
		merge(recordsStart, left, mid, right, compareFcnPtr); //merge the two halves of the array by using the merge helper function
	}
}

void merge (Record *recordsStart, int left, int mid, int right, int (*compareFcnPtr)(void*, void*)) {
	int leftSize = mid - left + 1;
	int rightSize = right - mid;
	Record leftHalf[leftSize];
	Record rightHalf[rightSize];
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
		Record leftTemp = leftHalf[leftIndex];
		Record rightTemp = rightHalf[rightIndex];
		int comparison = (*compareFcnPtr)(leftTemp.ptr, rightTemp.ptr);
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

}
//TODO: HANDLE NULL CASES
int intComparator (void* data0, void* data1) {
	return *((int*)data0) - *((int*)data1);
}

int strComparator (void* data0, void* data1) {
	return strcmp((const char*)data0, (const char*)data1);
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