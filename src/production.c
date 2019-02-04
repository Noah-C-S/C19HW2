/*
 * production.c
 *
 *  Created on: Nov 3, 2018
 *      Author: student
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "production.h"
#include <math.h>
#define FILLEDCHAR 'x'
#define UNFILLEDCHAR 'o'
void getInitial(FILE *input, int rows, int cols, char** output);
/** Runs the game of life program
 * @param argc the amount of arguments in argv
 * @param argv the arguments passed in the command line
 * @return true if the program ran properly, false otherwise
 */
bool production(int argc, char* argv[])
{
	bool results = false;
	bool done = false;
	int nRows=-1;
	int nCols = -1;
	int gens = 0; //number of generations to play
	char inputfile[100];
	bool print = false;
	bool pause = false;
	FILE *file;
	//etc.
	//get the NR NC gens input [print] [pause], Usage as needed.

	// Must first check if there are at least 4 arguments on the command line
	if(argc < 5){
		printf("Not enough arguments. Usage: ./HW2 NR(int) NC(int) gens(int) inputfile(str) [print(y/n)] [pause(y/n)]\n");
		done = true;
		return results;
	}
	if(argc > 7){
		printf("Too many arguments. Usage: ./HW2 NR(int) NC(int) gens(int) inputfile(str) [print(y/n)] [pause(y/n)]\n");
		done = true;
		return results;
	}
	char* ptr = 0;
	nRows = (int) strtol(argv[1], &ptr, 10); //rows and columns
	nCols = (int) strtol(argv[2], &ptr, 10);
	gens = (int) strtol(argv[3], &ptr, 10); //gens
	strcpy(inputfile, argv[4]); //file. strcopy is safer than just setting equal IIRC
	if(argc > 5){
		print = argv[5][0] == 'y' || argv[5][0] == 'Y'; //technically, anything starting with y would work (such as "yellow"), but this is ok since
		if(argc > 6){			   //behavior is undefind for anything except 'y' and 'n'.
			pause = argv[6][0] == 'y' || argv[6][0] == 'Y'; //I'm too nice for unnecessary undefined behavior, so 'Y' is acceptable too.
		}
	}
	if(nRows < 1){
		printf("Usage: Rows should be greater than 0, received %d.\n",nRows);
		done = true;
	}
	if(nCols < 1){
		printf("Usage: Cols should be greater than 0, received %d.\n",nCols);
		done = true;
	}
	if(gens < 1){
		printf("Usage: gens should be greater than 0, recieved %d.\n",gens);
	}

	file = fopen(inputfile, "r");
	if(file == false){
		printf("The file \"%s\" does not exist!\n", inputfile);
		done = true;
	}
	if(done) return results; //we encountered an error, so return false;
	//inputs were all fine, so do stuff
	char arr1[nRows][nCols];
	getInitial(file, nRows, nCols, arr1);
	puts("test");
	for(int i = 0; i < nRows; i++){
		puts(arr1[i]);
	}
	return results;

}
void PlayOne (unsigned int nr, unsigned int nc, char Old[][nc], char New[][nc])
{
	int nRows = nr;
	int nCols = nc;
	for(int row = 0;row < nRows;row++)
	{
		for(int col=0; col < nCols; col++)
		{
			*( ((char*) New) + (row * nCols) + col) = *( ((char*) Old) + (row * nCols) + col);
		}
	}
}

/** Gets the formatted initial array based on the file input
 * @param input the file to search through
 * @param rows the rows in the output array
 * @param cols the columns in the output array
 * @param output the array to which the generated array should be output, should be of size rows, cols or we will get errors.
 * @return the formatted 2D array of characters to be used as the initial condition.
 */
void getInitial(FILE *input, int rows, int cols, char** output){
	puts("it's called");
	char characters[(rows + 1)* cols]; //rows+1 * cols because we may need an extra column to store the newline characters
	int newlines = 0; //newline characters, basically amount of rows
	int charsSince = 0; //characters since last new line character
	int maxCharsSince = 0; // maximum characters since last new line character, basically amount of columns
	int index = 0; //index in characters;
	int c; //int representing a character, basically a character or EOF.
	//magic numbers:
	char newlineChar = '\n'; //newline character
	char endCharacter = '\0';//indicates that we've reached the end of the array. Could be done by counting newlines, but this is more readable IMO.
	do { //EOF should be less than 0
		c = getc(input); //get the next character.
		if((c == newlineChar || c < 0 ) && charsSince > 0){ //if newline or end of file, and the line wasn't empty.
			newlines++;
			maxCharsSince = (maxCharsSince > charsSince) ? maxCharsSince : charsSince;
			charsSince = 0;
			characters[index++] = newlineChar;
		}
		else if (charsSince < cols){ //ignore any characters after columns that the array can't handle
			if(c == FILLEDCHAR){
				characters[index++] = FILLEDCHAR;
				charsSince++;
			}
			else{
				characters[index++] = UNFILLEDCHAR; //could technically be anything, but this is more fun
				charsSince++;
			}
		}
	} while (c > 0 && newlines < rows); //terminates after end of file or there are too many rows for the array to handle
	characters[index] = endCharacter; //last character will ALWAYS be newline, but we set it to endCharacter now.
	//characters is now propagated with all of the relevant characters from the file (characters that can actually be stored in the output array)
	//char output[rows][cols]; //the output array
	for(int r = 0; r < rows; r++){
		for(int c = 0; c < cols; c++){
			output[r][c] = UNFILLEDCHAR; //makNULL)e an empty board.
		}
	}
	int startingRow = (rows/2) - (newlines/2); //used to store the initial condition in the approximate middle of the array.
	int startingCol = (cols/2) - (maxCharsSince/2); //these are always 0 if the returned array should be full;
	char nextChar; //the next character in the array.
	index = 0; //reusing old index variable
	for(int r = startingRow; r < rows; r++){
		for(int c = startingCol; c < cols; c++){
			nextChar = characters[index++]; //get the next character
			if(nextChar == newlineChar) break; //everything after this in the row should be blank anyway, so go to the next row
			if(nextChar == endCharacter) return; //once we reach the end of the file, we are done. END returns the completed array.
			if(nextChar == FILLEDCHAR) output[r][c] = FILLEDCHAR; //don't need to do anything for unfilled spaces
		}
	}


}
