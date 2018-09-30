CC = gcc

all : mergesort simpleCSVSorter

mergesort : mergesort.c
	$(CC) -c  mergesort.c

simpleCSVSorter : simpleCSVSorter.c
	$(CC)  simpleCSVSorter.c mergesort.o -o simpleCSVSorter

clean : 
	rm -f simpleCSVSorter mergesort.o
