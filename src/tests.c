/*
 * tests.c
 *
 *  Created on: Nov 3, 2018
 *      Author: Noah Capucilli-Shatan
 */
#include <stdbool.h>
#include "tests.h"
#include "production.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strncmp

/**
 * Runs all the tests functions
 * @return true if all the test functions return true, false otherwise
 */
bool tests(void)
{
	bool results=false;
	puts("During tests");
	bool ok1 = testReadInput();
	if(ok1)puts("Found and read the file.");
	bool ok2 = testMakeArrays();
	if(ok2)puts("Was able to allocate the arrays ok.");
	bool ok3 = testPlayOne();
	if(ok3)puts("playOne is ok.");
	bool ok4 = testNeighbors();
	if(ok4) puts("Neighbors is ok");
	puts("end of tests");
	results = ok1 && ok2 && ok3;
	return results;
}
/**
 * Tests the ability to read inputs
 * @return true if able to read inputs
 */
bool testReadInput(void)
{
	//the plan is:
	//create a file
	//make sure it can be opened.
	//try a file that should not exist
	//check that we notice it does not exist

	bool results = false;
	bool ok1 = false;
	bool ok2 = false;
	bool ok3 = false;
	//
	//Here goes: create a file
	FILE* fp;
	fp = fopen("/tmp/LifeInput.txt", "w"); //we write it first for later reading
	if (fp != false)
	{//it opened, yay!
		//file containing a sequence of lines,
		//each consisting of a sequence of 'x' and 'o' characters,
		//indicating the occupied and unoccupied cells
		//of the initial configuration, e.g.
		//oxo
		//xox
		//xox
		//oxo
		int w1 = fprintf(fp, "oxo\n");//returns the number of chars excluding null
		int w2 = fprintf(fp, "xox\n");
		int w3 = fprintf(fp, "xox\n");
		int w4 = fprintf(fp, "oxo\n");
		if ((w1 == w2) && (w2 == w3) && (w3 == w4) && (w4 == 4))
		{
			ok1 = true;
		}
		fclose(fp);
	}
	fp = fopen("/tmp/LifeInput.txt", "r"); //we try to read it
	if (fp != false)
	{//it opened, yay!
		ok2 = true;

		ok3 = true;
		//can we read the correct data?
		char oRow[4]; //test case, we know a string of 3 chars terminated by null
		//int count =  fscanf(fp, "%s", oRow);
		fscanf(fp, "%s", oRow);
		//printf("First row is %s\n", oRow);
		if(0 != strncmp(oRow, "oxo", 4))//0 means match
		{//got the right data
			ok3 = false;
		}
		fscanf(fp, "%s", oRow);
		//printf("Second row is %s\n", oRow);
		if(0 != strncmp(oRow, "xox", 4))
		{//got the right data
			ok3 = false;
		}
		fscanf(fp, "%s", oRow);
		//printf("Third row is %s\n", oRow);
		if(0 != strncmp(oRow, "xox", 4))
		{//got the right data
			ok3 = false;
		}
		fscanf(fp, "%s", oRow);
		//printf("Fourth row is %s\n", oRow);
		if(0 != strncmp(oRow, "oxo", 4))
		{//got the right data
			ok3 = false;
		}
	}

	// Still need to try opening a non-existent file for reading.

	results = ok1 && ok2 && ok3;
	return results;
}
/**
 * Tests the ability to make arrays
 * @return true if able to make arrays
 */
bool testMakeArrays(void)
{
	bool results = false;
	bool ok1 = false;
	//we need to allocate and populate arrays
	//In this test case we set our arrays to 4 rows x3 columns
	int nRows = 4;
	int nCols = 4;
	char boardBefore[4][4]={
			{'o','x','o','\0'},
			{'x','o','x','\0'},
			{'x','o','x','\0'},
			{'o','x','o','\0'}
	};

	char boardAfter[nRows][nCols];
	//here's the test
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			boardAfter[row][col]=boardBefore[row][col];
		}
	}
	ok1 = true;
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if (boardAfter[row][col]!=boardBefore[row][col])
			{
				ok1=false;
			}
		}
	}

	results = ok1;
	return results;
}
/**
 * Tests the aPlayOne function
 * @return true if the playOne function passes all tests, false otherwise
 */
bool testPlayOne(void)
{
	bool results = false;
	bool ok1 = false;
	bool ok2 = false;


	int nRows = 4;
	int nCols = 4;
	//all strings end in "\0", but a "2D array of chars" isn't the same as "an array of strings"
	char boardBefore[4][4]={
			{'o','x','o','o'},
			{'o','x','o','o'},
			{'o','x','o','o'},
			{'o','o','o','o'}
	};
	char correctBoardAfter[4][4]={
			{'o','o','o','o'},
			{'x','x','x','o'},
			{'o','o','o','o'},
			{'o','o','o','o'}
	};
	char boardAfter[nRows][nCols];
	boardCopy(4,4,boardBefore,boardAfter); //playOne requires both arrays to be the same before running to work properly
	//here's the invocation
	PlayOne(nRows, nCols, boardBefore, boardAfter);
	//here's the check
	ok1 = boardEql(nRows, nCols, boardAfter, correctBoardAfter);

	char boardBefore2[4][5]={
			{'o','o','x','o','o'},
			{'x','o','x','o','o'},
			{'o','x','x','o','o'},
			{'o','o','o','o','o'}
	};
	char correctBoardAfter2[4][5]={
			{'o','x','o','o','o'},
			{'o','o','x','x','o'},
			{'o','x','x','o','o'},
			{'o','o','o','o','o'}
	};

	char boardAfter2[4][5];
	boardCopy(4,5,boardBefore2,boardAfter2);
	//here's the invocation
	PlayOne(4, 5, boardBefore2, boardAfter2);
	//here's the check
	ok2 = boardEql(4, 5, boardAfter2, correctBoardAfter2);
	results = ok1 && ok2;
	return results;
}

/**
 * Tests the numNeighbors function
 * @return true if numNeighbors passes all tests, false otherwise
 */
bool testNeighbors(void){
	char board[4][4] ={
	{'x','o','x','o'},
	{'x','o','x','o'},
	{'o','x','x','x'},
	{'o','o','o','x'}
	};
	bool ok1 = (numNeighbors(4,4,0,0,board) == 1);
	bool ok2 = (numNeighbors(4,4,3,3,board) == 2);
	bool ok3 = (numNeighbors(4,4,2,2,board) == 4);
	bool ok4 = (numNeighbors(4,4,0,2,board) == 1);
	return ok1 && ok2 && ok3 && ok4;
}
