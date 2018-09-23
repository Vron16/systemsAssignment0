#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void mergesort (Record records[], int left, int right, void *compare) {
	//as long as mergesort is working on a valid subsection of the array of records (left pointer is less than the right pointer)
	if (left < right) {
		int mid = (left + right)/2; //calculate middle index of the array of records
		mergesort(records, left, mid, *compare); //recursively sort the left half of the array of records
		mergesort(records, mid + 1, right, *compare); //recursively sort the right of the array of records
		merge(records, left, mid, right, *compare); //merge the two halves of the array by using the merge helper function
	}
}


void merge (Record records[], int left, int mid, int right, void *compare) {
	int leftSize = mid - left + 1;
	int rightSize right - mid;
	Record leftHalf[leftSize];
	Record rightHalf[rightSize];
	for (int i = 0; i < leftSize; i++) {
		leftHalf[i] = records[left + i];
	}
	for (int i = 0; i < rightSize; i++) {
		rightHalf[i] = records[mid + i + 1];
	}
	int leftIndex = 0;
	int rightIndex = 0;
	int mergedIndex = left;

	while (leftIndex < leftSize && rightIndex < rightSize) {
		Record leftTemp = leftHalf[leftIndex];
		Record rightTemp = rightHalf[rightIndex];
	}


}