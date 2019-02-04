/*
 * production.h
 *
 *  Created on: Nov 3, 2018
 *      Author: student
 */

#ifndef PRODUCTION_H_
#define PRODUCTION_H_
#include <stdio.h>
bool production(int argc, char* argv[]);
void PlayOne (unsigned int nr, unsigned int nc, char Old[][nc], char New[][nc]);
void printBoard(int rows, int cols, char board[rows][cols]);
void boardCopy(int rows, int cols, char board1[rows][cols], char board2[rows][cols]);
void getInitial(FILE *input, int rows, int cols, char output[rows][cols]);
bool boardEql(int rows, int cols, char board1[rows][cols], char board2[rows][cols]);
bool boardDead(int rows, int cols, char board[rows][cols]);
int numNeighbors(int rows, int cols, int row, int col, char board[rows][cols]);
#endif /* PRODUCTION_H_ */
