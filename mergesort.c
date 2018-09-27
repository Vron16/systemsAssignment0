#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void mergesort (Record records[], int left, int right, int *compareFcnPtr(void*, void*)) {
	//as long as mergesort is working on a valid subsection of the array of records (left pointer is less than the right pointer)
	if (left < right) {
		int mid = (left + right)/2; //calculate middle index of the array of records
		mergesort(records, left, mid, compareFcnPtr); //recursively sort the left half of the array of records
		mergesort(records, mid + 1, right, compareFcnPtr); //recursively sort the right of the array of records
		merge(records, left, mid, right, compareFcnPtr); //merge the two halves of the array by using the merge helper function
	}
}


void merge (Record records[], int left, int mid, int right, int *compareFcnPtr(void*, void*)) {
	int leftSize = mid - left + 1;
	int rightSize right - mid;
	Record leftHalf[leftSize];
	Record rightHalf[rightSize];
	int i;
	for (i = 0; i < leftSize; i++) {
		leftHalf[i] = records[left + i];
	}
	for (i = 0; i < rightSize; i++) {
		rightHalf[i] = records[mid + i + 1];
	}
	int leftIndex = 0;
	int rightIndex = 0;
	int mergedIndex = left;

	while (leftIndex < leftSize && rightIndex < rightSize) {
		Record leftTemp = leftHalf[leftIndex];
		Record rightTemp = rightHalf[rightIndex];
		int comparison = *compareFcnPtr(leftTemp->ptr, rightTemp->ptr);
		if (comparison > 0) {
			records[mergedIndex] = rightTemp;
			mergedIndex++;
			rightIndex++;
		}
		else if (comparison < 0) {
			records[mergedIndex] = leftTemp;
			mergedIndex++;
			leftIndex++;
		}
		else {
			records[mergedIndex] = leftTemp;
			mergedIndex++;
			leftIndex++;
			records[mergedIndex] = rightTemp;
			mergedIndex++;
			rightIndex++;
		}
	}

	while (leftIndex < leftSize) {
		records[mergedIndex] = leftHalf[leftIndex];
		mergedIndex++;
		leftIndex++;
	}

	while (rightIndex < rightSize) {
		records[mergedIndex] = rightHalf[rightIndex];
		mergedIndex++;
		rightIndex++;
	}

}
//TODO: HANDLE NULL CASES
int intComparator (void* data0, void* data1) {
	return *((int*)data0) - *((int*)data1);
}

int strComparator (void* data0, void* data1) {
	return strcmp(*((char*)data1), *((char*)data0));
}