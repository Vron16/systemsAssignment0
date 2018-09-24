#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "simpleCSVSorter.h"

// Current errors that will later need to be addressed:
// - does not account for trailing white space, only leading
int main (char ** argv, int argc) {

	// First, we need to check to make sure that our input is all valid  (expecting 3 inputs
	// and the -c flag)
	if (argc != 3){
		char *errorMessage = "Missing command line arguments.  Aborting program.\n";
		write(STDERR, errorMessage, sizeof(errorMessage);
		return -1;
	}

	char *sort = argv[1];

	if (strcmp(sort,"-c") == 0){
		char *errorMessage = "Unrecognized value.  Expected \"-c\".  Aborting program.\n";
		write(STDERR, errorMessage, sizeof(errorMessage);
		return -1;
	}

	// We have one more error check to do with the value of the column string passed to us
	char *column = argv[2];

	// We will read through each line character by character and build the string that way
	int endOfLine = 0; //boolean value to represent when we hit the end of the first line
	int found = 0; // boolean value to represent when we find the correct column name
	int numOfCommas = 0; // represents number of commas until we hit our key column value (1 representing the first column)
	int numCols = 0; // represents the number of columns to sort upon

	while (endOfLine == 0){

		char *strBuilder = (char *)malloc(sizeof(char)*100); // assume we won't get more than 100 characters in a single string

		// Memory check
		if (strBuilder == NULL){
			char *errorMessage = "Error upon allocating memory to strBuilder for column headings.  Aborting program.\n";
			write(STDERR, errorMessage, sizeof(errorMessage));
			return -1;
		}

		int endOfStr = 0; // boolean value to represent when we hit the end of a column name
		int midOfStr = 0; // boolean value to represent if we have already read at least one char for the string (to remove whitespace)

		while (endOfStr == 0){
			char *newChar = (char *)malloc(sizeof(char)*2);

			if (newChar == NULL){
				char *errorMessage = "Error allocating memory to newChar in column headings.  Aborting program.\n";
				write(STDERR, errorMessage, sizeof(errorMessage));
				return -1;
			}

			read(STDIN, newChar, sizeof(char)); // gets a single character from STDIN
			newChar[1] = '\0';
		
			switch(newChar[0]){
				case '\n':
					endOfLine = 1;
					endOfStr = 1;
					strcat(strBuilder, newChar);
					break;
				case ',':
					endOfStr = 1;
					if (found == 0)
						numOfCommas += 1;
					numCols += 1;
					break;
				case ' ':
					if (midOfStr == 1){
						strcat(strBuilder, newChar);
					}
					break;
				case '\t':
					if (midOfStr == 1){
						strcat(strBuilder, newChar);
					}
					break;
				default:
					midOfStr = 1;
					strcat(strBuilder, newChar); // add our new character to the string
					break;
			}

			free(newChar);
		}

		if (strcmp(column, strBuilder) == 0){
			found = 1;
			numOfCommas -= 1; // will make our lives easier in the next part
		}

		free(strBuilder);
	}
	
	if (found == 0){
		char *errorMessage = "Column heading not found.  Cannot complete sort.  Aborting program.\n");
		write(STDERR, errorMessage, sizeof(errorMessage));
		return -1;
	}

	//*****************************************************************************************************
	// Now we can actually start setting up our array of structs.  This array will later be passed to out
	// mergesort so that we can sort the records in an efficient manner.
	//*****************************************************************************************************

	Record records[1000]; // place-holder until we can figure out how to dynamically allocate this

	// We will read through each line character by character and build the string that way
	int endOfFile = 0; // boolean value to represent when we hit the end of the file
	int lineNum = 0; // represents the number line we are on (excluding the header)

	while (endOfFile == 0){

		int endOfLine = 0; // boolean value to represent when we hit the end of a column name
		int midOfStr = 0; // boolean value to represent if we have already read at least one char for the string (to remove whitespace)
		int count = 0; // running sum of number of commas we encounter

		char *strBuilder = (char *)malloc(sizeof(char)*100);

		// Memory check
		if (strEnd == NULL){
			char *errorMessage = "Error upon allocating memory to strBuilder for records.  Aborting program.\n";
			write(STDERR, errorMessage, sizeof(errorMessage));
			return -1;
		}

		Record myRecord;

		while (endOfLine == 0){
			char *newChar = (char *)malloc(sizeof(char)*2);

			if (newChar == NULL){
				char *errorMessage = "Error allocating memory to newChar in records.  Aborting program.\n";
				write(STDERR, errorMessage, sizeof(errorMessage));
				return -1;
			}

			read(STDIN, newChar, sizeof(char)); // gets a single character from STDIN
			newChar[1] = '\0';
		
			switch(newChar[0]){
				case '\n':
					endOfLine = 1;

					if (numOfCommas == count){
						strcpy(myRecord.key, strBuilder);
						strcpy(myRecord.end, "\n");
					} else if (count < numOfCommas || count > numCols){
						char *errorMessage = "Invalid number of entries in a record.  Aborting program.\n";
						write(STDERR, errorMessage, sizeof(errorMessage));
						return -1;
					} else {
						strcat(strBuilder, newChar);
						strcpy(myRecord.end, strBuilder);
					}
					break;
				case ',':
					if (numOfCommas == count){
						strcpy(myRecord.key, strBuilder);
						strBuilder = (char *)realloc(sizeof(char)*100);
						strcat(strBuilder, ",");
					} else {
						strcat(strBuilder, newChar);
					}
					count++;
	
					// Check after the increment, setting up for key value
					if (numOfCommas == count){
						strcpy(myRecord.beginning, strBuilder);
						strBuilder = (char *)realloc(sizeof(char)*100);
					}
					break;
				case ' ':
					break;
				case '\t':
					break;
				default:
					midOfStr = 1;
					strcat(strBuilder, newChar);
					break;
			}

			free(newChar);
		}

		records[lineNum] = myRecord;

		free(strBuilder);

		lineNum++;
	}

	// If we get to this point, then we have inserted all of the Records properly into the array and can begin sorting
	//int result = mergesort(records, 0, 1000, comparator);
	
	return 0;
}
