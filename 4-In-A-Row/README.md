This programm is a 4 In a row game, Where the main focus is on working with 2d arrays.

This game is a 2-player game where each player at is turn inserts a coin into a specific column and the first player with 4 consecutive coins in row col or diagonal wins the game.

The programm provides additional feature such as:
1. Validate a given board for a valid sequenct of plays:

    A valid board is that resulted by some sequence of valid playes.

2. Encode a board to a given string:
 
    From a given board and a string the programm returns the board encoded into the string

For example the string Encoded board: 'H /H /H /H /H /BACBBAD /'

~~~~~~~~~~~~~~~
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
|A|B|B|A| | | |
~~~~~~~~~~~~~~~

3. Decode a string into the board. Its the reverse action of Decoding.

To run the game simply run:
> gcc 4InARow.c

and then:

> ./a.out

To test the additional features copy the following code into the file:

```
int main() {

    char board[ROWS][COLS];
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    makeMove(board, ROWS, COLS, NUM_PLAYERS, 'A', 3);
    makeMove(board, ROWS, COLS, NUM_PLAYERS, 'B', 3);
    makeMove(board, ROWS, COLS, NUM_PLAYERS, 'A', 0);
    undoMove(board, ROWS, COLS, 3);

    int status = getStatus(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
        
    if (status == 1) {
        printf("Game-over\n");
    } else if (status == 0) {
        printf("Tie\n");
    } else if (status == -1){
        printf("Game is in progress\n");
    } else {
        printf("Invalid return value from getStatus(...)\n");
    }
    char winner = getWinner(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
    if (winner == -1) {
        printf("No winner\n");
    } else {
        printf("Winner is '%c'\n", winner);
    }
    int valid = isValidBoard(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
    if (valid) {
        printf("Board is valid\n");
    } else {
        printf("Board is invalid\n");
    }
        char code[ROWS * COLS + 1];
        encode(board, ROWS, COLS, code);
        printf("Encoded board: '%s'\n", code);
        char newBoard[ROWS][COLS];
        decode(code, newBoard);
        printBoard(newBoard, ROWS, COLS);
}

```
