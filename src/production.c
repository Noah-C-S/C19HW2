/*
 * production.c
 *
 *  Created on: Nov 3, 2018
 *      Author: Noah Capucilli-Shatan
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "production.h"
#include <math.h>
#include <string.h>
#define FILLEDCHAR 'x'
#define UNFILLEDCHAR 'o'

/**
 *  Runs the game of life program
 * @param argc the amount of arguments in argv
 * @param argv the arguments passed in the command line
 * @return true if the program ran properly, false otherwise
 */
bool production(int argc, char* argv[])
{
	bool results = false; //what to return
	bool done = false; //true when finished
	int nRows=-1; //amount of rows for the board
	int nCols = -1; //amount of columns for the board
	int gens = 0; //number of generations to play
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
	if(argc > 5){
		print = argv[5][0] == 'y' || argv[5][0] == 'Y'; //technically, anything starting with y would work (such as "yellow"), but this is ok since
		if(argc > 6){			   //behavior is undefind for anything except 'y' and 'n'.
			pause = argv[6][0] == 'y' || argv[6][0] == 'Y'; //I'm too nice for unnecessary undefined behavior, so 'Y' is acceptable too.
		}
	}
	//Error messages. We may want to print multiple error messages, so these don't terminate the function immediately.
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

	file = fopen(argv[4], "r"); //open the file
	if(file == false){
		printf("The file \"%s\" does not exist!\n", argv[4]);
		done = true;
	}
	if(done) return results; //we encountered an error, so return false;
	//inputs were all fine, so do stuff
	char old[nRows][nCols], new[nRows][nCols], standby[nRows][nCols]; //the board arrays. old = old array, new = newest array, standby = 2 generations ago
	getInitial(file, nRows, nCols, old); //store the initial condition in old
	boardCopy(nRows, nCols, old, new); //copy old in to new (they must be equal before running playOne
	puts("Generation 0");
	printBoard(nRows, nCols, old); //print out the initial board
	int currentGens = 0;
	//run the actual simulation
	while(!done){
		PlayOne(nRows, nCols, old, new);//run playOne on old and new
		if(print){
			printf("Generation %i\n", currentGens + 1);
			printBoard(nRows, nCols, new); //print the board if print is true
		}
		//check if there is an oscillation or steady-state, and exit loop if there is.
		if(boardEql(nRows, nCols, old, new) || (currentGens > 0  && boardEql(nRows, nCols, standby, new))){ //standby is garbage on the first pass
			done = true;
			puts("Oscillation or steady-state detected!");
			results = true;
		}
		if(boardDead(nRows, nCols, new)){
			done = true;
			puts("Everything on the board is dead! ;_;");
			results = true;
		}
		if(++currentGens >= gens){
			puts("Simulation complete!");
			done = true;
			results = true;
		}
		if(pause){
			puts("Press enter to continue.");
			getchar();
		}
		boardCopy(nRows, nCols, old, standby); //copy old in to standby and new in to old
		boardCopy(nRows, nCols, new, old); //we don't need standby (would be 3 gens ago) anymore and we don't care what new is (it will be overwritten in the next gen)
	}
	if(!print) printBoard(nRows, nCols, old); //print the final board. If print is true, then the final board was already printed.
	return results;

}

/**
 * Plays one turn of the simulation
 * @param nr number of rows
 * @param nc number of columns
 * @param old the old array
 * @param new where to put the new array
 */
void PlayOne (unsigned int nr, unsigned int nc, char Old[][nc], char New[][nc])
{
	int nRows = nr;
	int nCols = nc;
	for(int row = 0;row < nRows;row++)
	{
		for(int col=0; col < nCols; col++)
		{
			int neighbors = numNeighbors(nRows, nCols, row, col, Old); //get the amount of neighbors
			if(Old[row][col] == FILLEDCHAR){ //death if occupied space's neighbors is less than 2 or more than 3
				if(neighbors < 2 || neighbors > 3){
					New[row][col] = UNFILLEDCHAR;
				}
			}
			else{ //birth if unoccupied space has exactly 3 neighbors
				if(neighbors == 3){
					New[row][col] = FILLEDCHAR;
				}
			}
		}
	}
}

/**
 * Gets the formatted initial array based on the file input
 * @param input the file to search through
 * @param rows the rows in the output array
 * @param cols the columns in the output array
 * @param output the array to which the generated array should be output, should be of size rows, cols or we will get errors.
 */
void getInitial(FILE *input, int rows, int cols, char output[rows][cols]){
	char characters[(rows + 1)* cols]; //rows+1 * cols because we may need an extra column to store the newline characters. We won't be storing any characters that won't fit on the grid.
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
		else if (c > 0 && charsSince < cols){ //ignore any characters after columns that the array can't handle
			if(c == FILLEDCHAR){
				characters[index++] = FILLEDCHAR;
				charsSince++;
			}
			else if (c == UNFILLEDCHAR){
				characters[index++] = UNFILLEDCHAR; //could technically be anything, but this is more fun
				charsSince++;
			}
		}
	} while (c > 0 && newlines < rows); //terminates after end of file or there are too many rows for the array to handle
	characters[index-1] = endCharacter; //last character will ALWAYS be newline, but we set it to endCharacter now.
	//characters is now propagated with all of the relevant characters from the file (characters that can actually be stored in the output array)
	for(int r = 0; r < rows; r++){
		for(int col = 0; col < cols; col++){
			output[r][col] = UNFILLEDCHAR; //make an empty board.
		}
	}
	int startingRow = (rows/2) - (newlines/2); //used to store the initial condition in the approximate middle of the array.
	int startingCol = (cols/2) - (maxCharsSince/2); //these are always 0 if the returned array should be full;
	if(startingRow < 0) startingRow = 0;
	if(startingCol < 0) startingCol = 0;
	int nextChar; //the next character in the array.
	index = 0; //reusing old index variable
	for(int r = startingRow; r < rows; r++){
		for(int col = startingCol; col < cols; col++){
			nextChar = characters[index++]; //get the next character
			if(nextChar == newlineChar){
				index--; //put the index back before the Newline so it doesn't get skipped twice
				break; //everything after this in the row should be blank anyway, so go to the next row
			}
			if(nextChar == endCharacter) return; //once we reach the end of the file, we are done.
			if(nextChar == FILLEDCHAR) output[r][col] = FILLEDCHAR; //don't need to do anything for unfilled spaces
		}
		index++; //skip the newline character
	}


}

/**
 *  Prints out the board passed in
 * @param rows the rows in the input array
 * @param cols the columns in the input array
 * @param board the board to print
 */
void printBoard(int rows, int cols, char board[rows][cols]){
	char c;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			c = board[i][j];
			if (c == FILLEDCHAR) printf("%c", FILLEDCHAR);
			else printf(" ");
		}
		printf("\n");
	}
}

/**
 * copies the board from board1 in to board2
 * @param rows the rows in the input arrays
 * @param cols the columns in the input arrays
 * @param board1 the array to be copied in to board2
 * @param board2 the array in which board1's contents will be copied
 */
void boardCopy(int rows, int cols, char board1[rows][cols], char board2[rows][cols]){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			board2[i][j] = board1[i][j];
		}
	}
}

/**
 * Compares 2 boards for inequality
 * @param rows the rows in the input arrays
 * @param cols the columns in the input arrays
 * @param board1 first array to compare
 * @param board2 2nd array to compare
 * @return true if the boards are equal, false otherwise
 */
bool boardEql(int rows, int cols, char board1[rows][cols], char board2[rows][cols]){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(board1[i][j] != board2[i][j]) return false;
		}
	}
	return true;
}
/**
 * Figures out if a board is dead (has no organisms remaining)
 * @param rows amount of rows in the board
 * @param cols amount of columns in the board
 * @param board the board that should be checked
 * @return True if the board is dead, false otherwise
 */
bool boardDead(int rows, int cols, char board[rows][cols]){
	bool dead = true;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < rows; j++){
			if(board[i][j] == FILLEDCHAR) dead = false;
		}
	}
	return dead;
}

/**
 * Gets the amount of neighbors (adjacent occupied spaces) of a particular space in a board
 * @param rows the amount of rows in the board
 * @param cols the amount of columns in the board
 * @param row the row of the space to check
 * @param col the column of the space to check
 * @param board the board to check
 * @return the number of neighbors the space at (row, col) has (1..8)
 */
int numNeighbors(int rows, int cols, int row, int col, char board[rows][cols]){
	int neighbors = 0;
	for(int i = -1; i <=1; i++){
		for(int j = -1; j <= 1; j++){
			if(j != 0 || i !=0){ //no need to check the space itself
				//if the space (row + i, col + j) is on the board and the space (row + i, col + j) is occupied
				if((row + i) >= 0 && (row + i) < rows && (col + j) >= 0 && (col + j) < cols && board[row+i][col+j] == FILLEDCHAR){
					neighbors++;
				}
			}
		}
	}
	return neighbors;
}
