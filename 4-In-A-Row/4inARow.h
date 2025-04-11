/**
 * @file 4InARow.h
 * @brief Header file for 4InARow.c
 * @note Do not modify this file or submit it, we will use our own copy of it.
 */
#include <stdio.h>


#define ROWS 6
#define COLS 7
#define NUM_PLAYERS 2
#define CONNECT 4

void initBoard(char board[ROWS][COLS], int rows, int columns);
void printBoard(char board[ROWS][COLS], int rows, int columns);
int makeMove(char board[ROWS][COLS], int rows, int columns, int players, char player, int column);
int undoMove(char board[ROWS][COLS], int rows, int columns, int column);
int getStatus(char board[ROWS][COLS], int rows, int columns, int players, int connect);
char getWinner(char board[ROWS][COLS], int rows, int columns, int players, int connect);
int isValidBoard(char board[ROWS][COLS], int rows, int columns, int players, int connect);
void encode(const char board[ROWS][COLS], int rows, int cols, char *code);
void decode(const char *code, char board[ROWS][COLS]);
