#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "simpleCSVSorter.h"
	
int main  (int argc, char **argv) {
	// First, we need to check to make sure that our input is all valid  (expecting 3 inputs
	// and the -c flag)
	if (argc != 3){
		char *errorMessage = "Missing command line arguments.  Aborting program.\n";
		write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
		return -1;
	}

	char *sort = argv[1];

	if (strcmp(sort,"-c") != 0){
		char *errorMessage = "Unrecognized value.  Expected \"-c\".  Aborting program.\n";
		write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
		return -1;
	}

	// We have one more error check to do with the value of the column string passed to us
	char *column = argv[2];

	// We will read through each line character by character and build the string that way
	int endOfLine = 0; //boolean value to represent when we hit the end of the first line
	int found = 0; // boolean value to represent when we find the correct column name
	int numOfCommas = 0; // represents number of commas acting as separators in line
	int numCols = 0; // represents the number of columns each record should have
	int first = 1; //represents that we are at the start of the input file
	char *line = (char *)malloc(sizeof(char)*100); // holds the entire line for the column headings to be printed at end

	// Memory check
	if (line == NULL){
		char *errorMessage = "Error upon allocating memory to line for column headings.  Aborting program.\n";
		write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
		return -1;
	}

	int lineBufferSize = 100; // initial buffer size to be used for comparisons
	int comma = 0; // represents the number of commas needed to hit the key value (0 being the first column)

	// For reading in the input, we will currently assume that most of the edge cases we need to account for in reading in
	// each record will be nonexistent; can be updated later if we have time
	
	while (endOfLine == 0) {
		char *strBuilder = (char *)malloc(sizeof(char)*100);
		int strBuilderBufferSize = 100; // initial buffer size to be used for comparisons

		// Memory check
		if (strBuilder == NULL){
			char *errorMessage = "Error upon allocating memory to strBuilder for column headings.  Aborting program.\n";
			write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
			return -1;
		}

		int endOfStr = 0; // boolean value to represent when we hit the end of a column name

		while (endOfStr == 0){
			char *newChar = (char *)malloc(sizeof(char)*2);

			if (newChar == NULL){
				char *errorMessage = "Error allocating memory to newChar in column headings.  Aborting program.\n";
				write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
				return -1;
			}

			// Resizing checks
			if (strlen(line) == lineBufferSize){
				resize(&line); // quadruple our buffer size (didn't work with doubling)
				lineBufferSize = lineBufferSize * 4;
			}

			if (strlen(strBuilder) == strBuilderBufferSize){ // 1 character offset for null terminating character
				resize(&strBuilder);  // quadruple our buffer size
				strBuilderBufferSize = strBuilderBufferSize * 4;
			}

			read(STDIN, newChar, sizeof(char)); // gets a single character from STDIN
			if (first == 1) {
				if (strlen(newChar) == 0) {
					char *errorMessage = "Error, no input was provided to the sorter. Aborting program.\n";
					write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
					return -1;
				}
				else {
					first = 0;
				}
			}
				
			strcat(line, newChar); // append the character to the line for output
		
			switch(newChar[0]){
				case '\n':
					endOfLine = 1;
					strcat(line, "\0");  // terminate the line
					strcat(strBuilder, "\0"); // terminate the builder
					endOfStr = 1;
					break;
				case ',':
					endOfStr = 1;
					strcat(strBuilder, "\0"); // terminate the builder
					numOfCommas = numOfCommas + 1;
					numCols = numCols + 1;
					break;
				case '\r':
					endOfLine = 1;
					strcat(line, "\n\0"); // terminate the line
					strcat(strBuilder, "\0"); // terminate the builder
					read(STDIN, newChar, sizeof(char)); // removes the annoying newline
					endOfStr = 1;
					break;
				default: // is a valid alphanumeric character or space
					strcat(strBuilder, newChar);
					break;
			}

			free(newChar);
		}

		strBuilder = trimwhitespace(strBuilder); // remove leading and trailing whitespace

		// Check to see if the string we finished building  is our compare value
		if (strcmp(column, strBuilder) == 0){
			found = 1;
			if (endOfLine != 1)
				comma = numOfCommas - 1; // will make our lives easier in the next part
			else
				comma = numOfCommas;
		}
		
		free(strBuilder);
	}
	
	if (found == 0){
		char *errorMessage = "Column heading not found.  Cannot complete sort.  Aborting program.\n";
		write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
		return -1;
	}

	//*****************************************************************************************************
	// Now we can actually start setting up our array of structs.  This array will later be passed to out
	// mergesort so that we can sort the records in an efficient manner.  To create the array, we first
	// create a LinkedList that will then later be converted into an array once we get the number of 
	// records in our CSV so that we can allocate the appropriate space for our array.
	//*****************************************************************************************************

	Node *head = (Node *)malloc(sizeof(Node));
	Node *current = head;

	// We will read through each line character by character and build the string that way
	int endOfFile = 0; // boolean value to represent when we hit the end of the file
	int lineNum = 0; // represents the number line we are on (excluding the header)
	
	int strOrNumeric = 0; // If we don't ever find a character in our key values, then we assume the column is of a numeric type
	int potentialDoubles = 0; // If we find a period in our key values, and no characters, we might be comparing doubles

	while (endOfFile == 0){

		int endOfLine = 0; // boolean value to represent when we hit the end of a column name
		int count = 0; // running sum of number of commas we encounter

		char *strBuilder = (char *)malloc(sizeof(char)*100);
		int strBuilderBufferSize = 100; // same as above for reading in columns

		// Memory check
		if (strBuilder == NULL){
			char *errorMessage = "Error upon allocating memory to strBuilder for records.  Aborting program.\n";
			write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
			return -1;
		}

		char *keyBuilder = (char *)malloc(sizeof(char)*100);
		int keyBuilderBufferSize = 100; // same as above for reading in columns

		// Memory check
		if (keyBuilder == NULL){
			char *errorMessage = "Error upon allocating memory to strBuilder for records.  Aborting program.\n";
			write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
			return -1;
		}

		// Used to check for edge cases
		int foundChars = 0; // only important for building key
		int quotes = 0; // used to indicate that we are in Quotes mode

		while (endOfLine == 0){
			char *newChar = (char *)malloc(sizeof(char)*2);
			
			// Memory check
			if (newChar == NULL){
				char *errorMessage = "Error allocating memory to newChar in records.  Aborting program.\n";
				write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
				return -1;
			}

			// Resizing checks
			if (strlen(strBuilder) == strBuilderBufferSize){ // 1 character offset for null terminating character
				resize(&strBuilder);  // quadruple our buffer size
				strBuilderBufferSize = strBuilderBufferSize * 4;
			}

			if (keyBuilder != NULL && strlen(keyBuilder) == keyBuilderBufferSize){
				resize(&keyBuilder); // quadruple our buffer size
				keyBuilderBufferSize = keyBuilderBufferSize * 4;
			}

			read(STDIN, newChar, sizeof(char)); // gets a single character from STDIN

			// If the newchar has a size of 0, then we have reached the end of the file
			if (strlen(newChar) == 0){
				endOfFile = 1;
				break;
			}

			newChar[1] = '\0'; // append to end of string for strcat

			strcat(strBuilder, newChar); // append newChar to line being built
		
			switch(newChar[0]){
				case '\n':
					endOfLine = 1;

					if (comma == count){
						strcat(strBuilder, "\0"); // terminate the line

						if (foundChars == 1){
							strcat(keyBuilder, "\0"); // terminate the key string
							keyBuilder = trimwhitespace(keyBuilder); // trim the string
						} else {
							keyBuilder = NULL; // will help with mergesort
						}
					} else if (count < numOfCommas || count > numCols){
						char *errorMessage = "Invalid number of entries in a record.  Aborting program.\n";
						write(STDERR, errorMessage, sizeof(char)*strlen(errorMessage));
						return -1;
					} else {
						strcat(strBuilder, "\0"); // terminate the string
					}

					break;
				case ',':
					if (quotes == 0){ // quotes mode off
						if (comma == count){ // building our key
							if (foundChars == 1){
								strcat(keyBuilder, "\0"); // terminate the key string
								keyBuilder = trimwhitespace(keyBuilder);
							} else {
								keyBuilder = NULL; // will help with mergesort
							}
						}

						count++;
					}

					break;
				case ' ':
					if (comma == count){
						strcat(keyBuilder, newChar); // will be removed later
					}
					break;

				case '\t':
					if (comma == count){
						strcat(keyBuilder, newChar); // will be removed later
					}
					break;
				case '"':
					if (quotes == 0){
						quotes = 1; // Quotes mode ON
					} else {
						quotes = 0; // Quotes mode OFF
					}
					break;
				case '.':
					if (comma == count){
						strcat(keyBuilder, newChar);
						potentialDoubles = 1; // columns might be numeric double values
					}
				case '\r': // carriage return escape character, ignore
					break;
				default:
					if (comma == count){
						foundChars = 1; // our key is nonempty!
						strcat(keyBuilder, newChar);
						if (isalpha(newChar[0])){ // is our character alphabetic?
							strOrNumeric = 1;
						}
					}
					break;
			}

			//free(newChar);
		}

		if (endOfFile == 0){
			// Construct the record entry
			// Finish current node and construct next node
			current->data = (Record *)malloc(sizeof(Record));

			if (keyBuilder != NULL)
				current->data->key = strdup(keyBuilder);	
			else
				current->data->key = NULL;

			current->data->line = strdup(strBuilder);
			current->next = (Node *)malloc(sizeof(Node));
			current = current->next;

			lineNum++;
		}	
		
		//free(strBuilder);
		//if (keyBuilder != NULL)
		//	free(keyBuilder);
	}
	
	// Converts the linkedlist into an appropriately sized records array
	Record **converted = convertToArray(head, lineNum);

	// If we get to this point, then we have inserted all of the Records properly into the array and can begin sorting
	int (*comparePtr)(void *, void *) = 0;

	if (strOrNumeric == 1){
		comparePtr = strComparator;
	} else if (potentialDoubles == 1){
		comparePtr = doubleComparator;
	} else {
		comparePtr = intComparator;
	}
	
	sortLaunch(converted, lineNum, comparePtr); // launches our MergeSort

	//****************************************************************************************************************
	// OUTPUT
	//****************************************************************************************************************

	write(STDOUT, line, sizeof(char)*strlen(line)); // column headings;
	
	int i;
	for (i = 0; i < lineNum; i++){
		write(STDOUT, converted[i]->line, sizeof(char)*strlen(converted[i]->line));
	}
		
	//****************************************************************************************************************
	// Garbage Collection: freeing everything that remains
	//****************************************************************************************************************
	
	// Assume that the implemented convertToArray method will handle freeing of Nodes
	free(line);
	
	//for (i = 0; i < lineNum; i++){
	//	if (converted[lineNum].key != NULL)
	//		free(converted[lineNum].key);
	//	free(converted[lineNum].line);
	//}

	//free(converted);

	return 0;
}
