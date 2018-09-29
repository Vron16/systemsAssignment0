#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "simpleCSVSorter.h"

int main (char ** argv, int argc) {
	char *rec1Beginning = "2, 3, 4, ";
	int rec1Data = 45;
	char *rec1Ending = ", 5, 6, 7";

	char *rec2Beginning = "3, 4, 5, ";
	int rec2Data = 32;
	char *rec2Ending = ", 6, 7, 8";

	char *rec3Beginning = "4, 5, 6, ";
	int rec3Data = 56;
	char *rec3Ending = ", 7, 8, 9";

	char *rec4Beginning = "5, 6, 7, ";
	int rec4Data = 77;
	char *rec4Ending = ", 8, 9, 10";

	char *rec5Beginning = "6, 7, 8, ";
	int rec5Data = 17;
	char *rec5Ending = ", 9, 10, 11";

	//char *rec1BeginningPtr = &rec1Beginning;
	void *rec1DataPtr = &rec1Data;
	//char *rec1EndingPtr = &rec1Ending;

	//char *rec2BeginningPtr = &rec2Beginning;
	void *rec2DataPtr = &rec2Data;
	//char *rec2EndingPtr = &rec2Ending;

	//char *rec3BeginningPtr = &rec3Beginning;
	void *rec3DataPtr = &rec3Data;
	//char *rec3EndingPtr = &rec3Ending;

	//char *rec4BeginningPtr = &rec4Beginning;
	void *rec4DataPtr = &rec4Data;
	//char *rec4EndingPtr = &rec4Ending;

	//char *rec5BeginningPtr = &rec5Beginning;
	void *rec5DataPtr = &rec5Data;
	//char *rec5EndingPtr = &rec5Ending;

	Record record1 = {rec1DataPtr, rec1Beginning, rec1Ending};
	Record record2 = {rec2DataPtr, rec2Beginning, rec2Ending};
	Record record3 = {rec3DataPtr, rec3Beginning, rec3Ending};
	Record record4 = {rec4DataPtr, rec4Beginning, rec4Ending};
	Record record5 = {rec5DataPtr, rec5Beginning, rec5Ending};

	Record records[] = {record1, record2, record3, record4, record5};
	//int (*compPtr)(void *, void *) = intComparator;
	//mergesort(records, 0, 4, compPtr);

	int i;
	//int recordsSize = sizeof(records)/sizeof(Record);
	for (i = 0; i < 5; i++) {
		Record rec = records[i];
		int *dataPtr = (int *)(rec.ptr);
		printf("Data : %d \n", *dataPtr);
	}

}