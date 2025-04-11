#include "ex_4.h"

#define EMPTY_POS ' '
#define INVALID_BOARD 0
#define VALID_BOARD 1
int getButtomEmptyPos(char board[ROWS][COLS],int rows, int column);
int validatePositions (int col, int row, int columns, int rows, int connect,int action);
int getNumOfConnects(char board[ROWS][COLS], int connect, int rows, int columns, int countArr[], int col, int row,char\
                     player, int directions,int originalConnect, int toValidate);
int checkForConnect(char board[ROWS][COLS], int connect, int rows, int columns,int validate,int player);
int checkForFullBoard(char board[ROWS][COLS], int columns);
int isValidPlayer (int players, int player);
int getNumOfOccurences(char board[ROWS][COLS], int rows, int columns, char player);
int validatePlayTimes(char board[ROWS][COLS], int rows, int columns, int players);
int validatePlayMoves(char board[ROWS][COLS], int rows, int columns, int lastPlayed, int numOfOccur, int players);
int validatePlays(char board[ROWS][COLS], int rows, int columns, int players);
int get64BaseAsInteger(char input);


/**
 @brief function to convert charcter player to its int form
 @param player the player in character form
 @return the player as integer
 */
int getPlayerAsInt(char player) {
    return player - 'A' + 1;
}

/**
 @brief function to conver integer player to its character form
 @param player the player in integer form
 @return the player as character
 */
char getPlayerAsChar(int player) {
    return player + 'A' - 1;
}

/**
 @brief function to init the board
 the function runs double loop and initializing the board with spaces
 @param board the given board to init
 @param rows the rows of the board
 @param columns the columns of the board
 */
void initBoard(char board[ROWS][COLS], int rows, int columns) {
    
    //run for every row
    for (int i = 0; i < rows; i++) {
        //run for every column
        for (int j = 0; j < columns; j++){
            board[i][j] = EMPTY_POS;
        }
    }
}

/**
 @brief function to print the board
 the function runs double loop and prints the board
 @param board the given board to init
 @param rows the rows of the board
 @param columns the columns of the board
 */
void printBoard(char board[ROWS][COLS], int rows, int columns) {
    
    //run for every row
    for (int i = 0; i < rows + 2; i++) {
        //run for every column
        for (int j = 0; j < 2 * columns + 1; j++) {
            //if we on the first or last row print
            if (i == 0 || i == rows + 1) {
                printf("~");
                //if we on the ramge of the board print
            } else if (i < rows + 1 && j < columns) {
                printf("|%c", board[i - 1][j]);
                //if we at the edge of the board print
            } else if (j == columns) {
                printf("|");
            }
        }
        //go line down
        printf("\n");
    }
}
/**
 @brief function to check if agiven board is a valid board
 the function check for invalid wins and for invalid plays on the board
 @param board the given board to init
 @param rows the rows of the board
 @param columns the columns of the board
 @param players the number of player playing
 @param connect the number that the players needs to connect to win
 @return 0 if the board is invalid and 1 otherwise
 */
int isValidBoard(char board[ROWS][COLS], int rows, int columns, int players, int connect) {

    const int NO_WINS = 0;
    const int INVALID_WIN = -1;
    const int MAX_ALLOWED_WINS = 1;
    //init the board decode string so we can restore it to its last position
    char str[2 * rows * columns + rows + 1];
    char *code = str;
    
    //we encode the board
    encode(board, rows, columns, code);
    
    //says to the function checkForConnect that we want to validate the wins
    int validate = 1;
    
    //if we found two wins for a single player that are not connected we return invalid
    int playerWins = 0;
    
    //check for every player on the board for invalid wins
    for (int player = 1; player <= players; player++) {
        
        //we check for the connect made player by player
        int isValidWin =  checkForConnect(board, connect, rows, columns, validate, player);

        //check if we find a valid win for the player and increment its total wins
        if (isValidWin > NO_WINS) {
            playerWins++;
            //two wins for the same player its not ok
            if (playerWins > MAX_ALLOWED_WINS) {
                return INVALID_BOARD;
            }
        }
        
        //if the function found invalid win return
        if (isValidWin == INVALID_WIN) {
            return INVALID_BOARD;
        }
    }

    //we restore the board for the next check
    decode(code, board);
    
    //we check for invalid plays made by the users
    int result = validatePlays(board, rows, columns, players);
    
    //decode back the array
    decode(code, board);
    
    //return the result
    return result;
}

/**
 @brief function to validate the board in term of turns played legal
 @param board the play board
 @param rows the board rows
 @param columns the board columns
 @param players total players
 @return 0 if the board is invalid otherwise 1
 */
int validatePlays(char board[ROWS][COLS], int rows, int columns, int players) {
    
    //get the last played player while validate the moves
    int lastPlayed = validatePlayTimes(board, rows, columns, players);
    
    
    //not a valid played board
    if (!lastPlayed) {
        return INVALID_BOARD;
    }
    
    //get total num of occurencec in the board
    int numOfOccur = getNumOfOccurences(board, rows, columns, EMPTY_POS);
    
    //validate the board layout for valid moves
    int res = validatePlayMoves(board, rows, columns, lastPlayed, numOfOccur, players);
    
    //the board is not valid
    if (!res) {
        return INVALID_BOARD;
    }

    //the board is valid
    return VALID_BOARD;
}

/**
 @brief function to check if a player is in a given row
 the function gets a row and runs throw if returns the col if it found
 @param row the guven row to check
 @param player the player to check
 @param rows the total row on the board
 @return the col of the given player -1 if not found
 */
int getPlayerCol (char board[ROWS][COLS], int row, int columns, int player, int rows) {
        
    const int NOT_FOUND =  -1;
    const int FLOATING_DISK = -2;
    //init the player as cher
    char playerAsChar =  getPlayerAsChar(player);
    
    //find the player in the row
    for (int i = 0; i < columns; i++){
        //if the position in not empty but its position below is
        if (board[row][i] != EMPTY_POS && row + 1 < rows && board[row + 1][i] == EMPTY_POS){
        //check for the next row for floating disk
            return FLOATING_DISK;
        }
        
        //check if found in the row and return column
        if (board[row][i] == playerAsChar) {
            return i;
        }
    }
    
    //not found return -1
    return NOT_FOUND;
}

/**
 @brief funciton to validate if the moves that was made on the board are legal
 the function will check recursively for the last played player occurence in the first row of some column and do that
 for the previous player and so on
 @param board the play board
 @param rows the rows on the board
 @param columns the columns on the board
 @param lastPlayed the last played player on the board
 @param numOfOccur the number of total occurences on the board
 @param players the total number of players
 @return 1 if the board is vaild 0 otherwise
 */
int validatePlayMoves(char board[ROWS][COLS], int rows, int columns, int lastPlayed, int numOfOccur, int players) {
    
    const int FLOATING_DISK = -2;
    const int MIN_ROW = 0;
    //if no occurences left on the board return
    if (numOfOccur == 0) {
        return VALID_BOARD;
    }
    
    //run for every row
    for (int i = 0; i < rows; i++) {
        
        //look for the player in the row
        int playerCol = getPlayerCol(board, i, columns, lastPlayed, rows);

        //in case you found a player but the player was floating
        if (playerCol == FLOATING_DISK) {
            return INVALID_BOARD;
        }
        
        //if found a player
        if (playerCol >= MIN_ROW) {
            
            //go back to the prevoius player i.e subtract one from the current player
            lastPlayed = lastPlayed - 1 == 0? players : (lastPlayed - 1);
            
            //if there isnt no one above him and also we arent on the first row
            if ((i != 0 && board[i - 1][playerCol] == EMPTY_POS) || i == 0) {
                //clear the current player spot so we can check for it's player below
                board[i][playerCol] = EMPTY_POS;
                //break because you found what you wanted
                break;
            }
        }
        
        //if not player was found on non of the rows the board is invalid return
        if (i == rows) {
            return INVALID_BOARD;
        }
    }


    //call itself subtracting the number of occurences on the board
    return validatePlayMoves(board, rows, columns, lastPlayed, numOfOccur - 1, players);
}

/**
 @brief helper function to validatePlays
 the function checks for invalid occurences in the board and illegel plays
 @param board the play board
 @param rows the number of rows in the board
 @param columns the number of columns in the board
 @param players the number of players
 @return 0 if the board is illegal the last played user if the board is ok
 */
int validatePlayTimes(char board[ROWS][COLS], int rows, int columns, int players) {
    
    const int MAX_DIFF_ALLOWD = 2;
    
    //init
    int max = 0;
    int lastPlayedPlayer = 0;
    
    //check for every player
    for (int i = 0; i < players - 1; i++) {
        //init player as int
        char player = 'A' + i;
        
        //get the player and the next player occurences on the board
        int currCount = getNumOfOccurences(board , rows, columns, player);
        int nextCount = getNumOfOccurences(board, rows, columns, player + 1);
        
        //if the next player made a move then the board is illegal
        if (currCount < nextCount) {
            return INVALID_BOARD;
        }
        //store the last played player because well need it for later
        if (currCount >= nextCount) {
            lastPlayedPlayer = i + 2;
        }
        //store max
        if (currCount > max) {
            max = currCount;
        }
     
        //if player played 2 times then the board is invalid
        if (max - currCount >= MAX_DIFF_ALLOWD || max - nextCount >= MAX_DIFF_ALLOWD) {
            return INVALID_BOARD;
        }
        
    }
    
    //return last player whos played
    return lastPlayedPlayer;
}

/**
 @brief function to get the number of occurences of a player in the board or the total occurences on the board
 if the function recieved player as input she will check for the player occurences else for the total board
 @param board the play board
 @param rows the board rows
 @param columns the obard columns
 @param player the player to check
 @return the count of occurences
 */
int getNumOfOccurences(char board[ROWS][COLS], int rows, int columns, char player) {
    
    //init
    int count = 0;
    
    //run for every row
    for (int i = 0; i < rows; i++) {
        //run for every column
        for (int j = 0; j < columns; j++) {
            
            //if player was given to the function then check for its occurences
            if (player != EMPTY_POS) {
                if (board[i][j] == player) {
                    count++;
                }
                //else check for the total board
            } else if (board[i][j] != EMPTY_POS){
                count++;
            }
       
        }
    }
    
    //return
    return count;
}

/**
 @brief function to get the winner on the board
 the function will call checkForConnect
 @param board the play board
 @param rows the board rows
 @param columns the obard columns
 @param players the player to check
 @param connect the number of connect that the players need for win
 @return the winner as character if winner in found  else -1
 */
char getWinner(char board[ROWS][COLS], int rows, int columns, int players, int connect) {
    
    const int NO_WINNER = -1;
    //assume the board is valid
    int validate = 0;
    
    //run for every player
    for (int pl = 1; pl < players; pl++) {
        //check for its wins
        int winner = checkForConnect(board, connect, rows, columns, validate, pl);
        
        //if winner we returned return the player charcter
        if (winner) {
            return getPlayerAsChar(pl);
        }
    }
   
    return NO_WINNER;
}


/**
 @brief function to check if a given player is between the max players allowed to play
 @param players the number of players playing the game
 @param player the player to check
 @return 0 if its not a valid player and 1 otherwise
 */
int isValidPlayer (int players, int player) {
    const int MIN_INVLAID_PL = 0;
    return player > MIN_INVLAID_PL && player <= players;
}

/**
 @brief function to check the current status of a givan board
 the fuction first check for a winner and then if a winner wasn't found it checks if the board is full
 @param board the play board
 @param rows the board rows
 @param columns the obard columns
 @param players the player to check
 @param connect the number of connect that the players need for win
 @return 1 if there is a winner 0 if the board is full then its a tie and -1 if the game is still on
 */
int getStatus(char board[ROWS][COLS], int rows, int columns, int players, int connect) {
    
    const int GAME_WINNED = 1;
    const int WINNDER_NOT_FOUND = -1;
    const int GAME_IS_ON = -1;
    const int TIE = 0;
    //if there is a winner
    char winner = getWinner(board, rows, columns, players, connect);
    
    //found a winner return
    if ((char) winner != WINNDER_NOT_FOUND) {
        return GAME_WINNED;
    }

    //else if the board is full
    int isFullBoard = checkForFullBoard(board,columns);
    if (isFullBoard) {
        return TIE;
    }
    
    //the game is still on
    return GAME_IS_ON;
}


/**
 @brief function to check if a given board is full
 @param board the play board
 @param columns the obard columns
 @return 1 if the board is full 0 otherwise
 */
int checkForFullBoard(char board[ROWS][COLS], int columns) {
    
    const int HAS_SPACE = 0;
    const int NO_SPACE = 1;
    //check for the first row
    for (int i = 0; i < columns; i++) {
        //if we founded empty position
        if (board[0][i] == EMPTY_POS) {
            return HAS_SPACE;
        }
    }
    
    return NO_SPACE;
}

/**
 @brief function to check for a win on the board
 the function calls getNumOfConnects to check the win of a given player, the function recieves player and check for its wins
 @param board the board to check
 @param connect how much in a row count for a win
 @param rows the number of rows in the board
 @param columns the number of columns in the border
 @param validate paramter the determine whether odr not to actually validate the board
 @return the serial number of the winner 0 if there is no winner -1 if there is invalid win
 */
int checkForConnect(char board[ROWS][COLS], int connect, int rows, int columns, int validate, int player) {

    const int INVALID_WIN = 1;
    const int INVALID_WIN_RETURN = -1;
    const int VALID_WIN = 1;
    const int ALLOWED_WINS = 1;
    const int NO_WINS = 0;
    
    //intialize 8 direction array and array size
    int directions = 8;
    int arr[8] = {0};
    int wins = 0;
    char playerAsChar = getPlayerAsChar(player);
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (board[row][col] == playerAsChar) {
                //get if there was a win or invalid win if validate flag is on
                int isWin = getNumOfConnects(board, connect - 1, rows, columns, arr,col, row, playerAsChar,\
                                             directions, connect, validate);
                
                //if validate flag is on validate the win
                if (validate) {
                    if (isWin == INVALID_WIN) {
                        return INVALID_WIN_RETURN;
                    } else if (isWin > INVALID_WIN) {
                        wins++;
                        //more than one win its not ok return
                        if (wins > ALLOWED_WINS) {
                            return INVALID_WIN_RETURN;
                        }
                    }
                    //if it's valid meaning its a valid win
                } else if (isWin > INVALID_WIN) {
                    return VALID_WIN;
                }
   
                //reset the directions array
                for (int i = 0; i < directions; i++) {
                    arr[i] = 0;
                }
            }
        }
    }
    
    //we have a winner
    if (wins) {
        return VALID_WIN;
    }

    return NO_WINS;
}

/**
 @brief function to check in a given win disks sequence if the disk is a possible win disk
 i.e i its in the first row or its the first in its column
 @param board the play board
 @param row the row to check
 @param col the columns to check
 @return 1 if the position is a win disk 0 otherwise
 */
int isValidWin(char board[ROWS][COLS], int row, int col) {
    
    const int VALID_WIN_DISK = 1;
    const char POSSIBLE_WIN = '+';
    const int FIRST_ROW = 0;
    
     //if we on the first row or there isnt anyone above
    if (row == FIRST_ROW || board[row - 1][col] == EMPTY_POS) {
        
        //mark the position in order to check for two invalid wins
        board[row][col] = POSSIBLE_WIN;
        
        //return 1 since its a valid win spot
        return VALID_WIN_DISK;
    }
    
    //not a win disk
    return !VALID_WIN_DISK;
}

/**
 @brief function to checks if at a given position lays player then she increment given array at a given index
 the function checks on the board whethere the position is valid and check for the player occurence and increment if both came positive
 @param board the board to check
 @param row the row to check
 @param col the coulmn to check
 @param player the player to check
 @param countArr the array to store the values in
 @param pos pos to store inside  the array
 @param rows the number of rows in the board
 @param columns the number of columns in the border
 @return 0 if the operation wes not succesfull 1 otherwise
 */
int storeCount(char board[ROWS][COLS],int row, int col, char player, int countArr[],int pos
               ,int rows, int columns) {
    
    const int OP_SUCCESS = 1;
    //check for the boundries
    if (row < rows && col < columns && col >= 0 && row >= 0) {
        //check for player at pos
        if (board[row][col] == player) {
            //increment giving array at giving index
            countArr[pos]++;
            //return
            return OP_SUCCESS;
        }
    }

    //return no success
    return !OP_SUCCESS;
}


/**
 @brief function to validate a given board for its wins
 the fuction recives array and calls isValidWin to check for a win in a certain direction if that win is valid
 meaning at least one of the disks on the connect has no one above him
 @param board the board to check
 @param arr the given arr to check the direction made in getNumOfConnects function
 @param originalConnect the CONNECT paramter
 @param connect given the connect at a certain position put by the getNumOfConnects function
 @param row the row to check the player single move
 @return 1 if found a disk as described 0 otherwise
 */
int validateWin(char board[ROWS][COLS], int arr[],int originalConnect, int connect, int row, int col) {
    
    const int VALID_WIN = 1;
    int res = 0;
    //check the row to the buttom
    if (arr[0] == originalConnect) {
        res += isValidWin(board, row+connect, col);
    }
    //check the row to the top
    if (arr[1] == originalConnect) {
        res += isValidWin(board, row - connect, col);
    }
    //check for the col to the right
    if (arr[2] == originalConnect) {
        res += isValidWin(board, row, col + connect);
    }
    //check for the col to the left
    if (arr[3] == originalConnect) {
        res += isValidWin(board, row, col - connect);
        
    }
    //check the right buttom diagonel
    if (arr[4] == originalConnect) {
        res += isValidWin(board, row + connect, col + connect);
    }
    //check the left buttom diagonal
    if (arr[5] == originalConnect) {
        res += isValidWin(board, row+connect, col - connect);
    }
    //check the left top diagonal
    if (arr[6] == originalConnect) {
        res += isValidWin(board, row - connect, col + connect);
    }
    //check the right top diagonal
    if (arr[7] == originalConnect) {
        res += isValidWin(board, row - connect, col - connect);
    }
    
    if (res > 0) {
        return VALID_WIN;
    }
    
    return !VALID_WIN;
}

/**
 @brief function that gets ra position and check for its connects around the position
 the function does it recursively checking for all eight direction and returns the CONNECT + 1 variable if there was a connect
 and because it does so we can check and validate the win by checking every disk in the win seqence to find at least one disk that is a
 "possible win disk" i.e no other disk is above him by decremening the return when we dont find "possible win disk" and  returning the result
 @param board the board to check
 @Param connect the connect
 @param rows the board rows
 @param columns the board columns
 @param countArr array that stores the the disks sequence times
 @param col the column to check
 @Param row the row to check
 @param player the player on the board
 @param size the array size
 @param originalConnect the connect that we do not chenge recusively
 @return number > 1 if there wes a valid win 0 if no wins 1 if there was invalid win
 */
int getNumOfConnects(char board[ROWS][COLS], int connect, int rows, int columns,\
                     int countArr[], int col, int row, char player, int size,
                     int originalConnect, int toValidate) {
    const int NO_WINS = 0;
    
    //exit condition we decrement connect with every recursive call
    if (connect == -1) {
        //check the wins array to find a win
        for (int i = 0; i < size; i++) {
            if (countArr[i] == originalConnect) {
                return originalConnect + 1;
            }
        }
        //no wins return
        return NO_WINS;
    }

    //init
    int exitFlag = 0;
    //check the row to the buttom
    exitFlag += storeCount(board , row+connect, col, player, countArr, 0, rows, columns);
    //check the row to the top
    exitFlag += storeCount(board, row - connect, col, player, countArr, 1, rows, columns);
    //check for the col to the right
    exitFlag += storeCount(board, row, col + connect, player, countArr, 2, rows, columns);
    //check for the col to the left
    exitFlag += storeCount(board, row, col - connect, player, countArr, 3, rows, columns);
    //check the right buttom diagonel
    exitFlag += storeCount(board, row + connect, col + connect, player, countArr, 4, rows, columns);
    //check the left buttom diagonal
    exitFlag += storeCount(board, row + connect, col - connect, player, countArr,5, rows, columns);
    //check the left top diagonal
    exitFlag += storeCount(board, row - connect, col - connect, player, countArr,6, rows, columns);
    //check the right top diagonal
    exitFlag += storeCount(board, row - connect, col + connect, player, countArr, 7, rows, columns);

    //if no matching plate was found on eather direction
    if (!exitFlag) {
        return NO_WINS;
    }
    
    
    //call with connect - 1 to get close the the end position
    int result = getNumOfConnects(board, connect - 1, rows, columns, countArr, col,\
                               row, player, size, originalConnect, toValidate);
   
    //if there was a win and we need to validate and we still in the range of the original connnect
    if (connect < originalConnect && result && toValidate) {
        //check if the win is valid
        int validate = validateWin(board, countArr, originalConnect, connect, row, col);
       
        //decrement result by 1 if its not possible win disk
        if (!validate) {
            return result - 1;
        } else {
            //its a win disk the just return result
            return result;
        }
    }

    //if there is any win return
    return result;
}

/**
 @brief function to undo move
 the function will validate the column and check if the column in empty and if so will remove from it
 @param board the board to check
 @param rows the board rows
 @param columns the board columns
 @param column the column to undo the move
 @return 0 if the undo was unseccesfull 1 otherwise
 */
int undoMove(char board[ROWS][COLS], int rows, int columns, int column) {
    
    const int UNDO_SUCCESFULL = 1;
    const int MIN_COL = 0;
    //validate the column in aspect of the board
    if (column < MIN_COL || column >= columns) {
        return !UNDO_SUCCESFULL;
    }
    
    //get the last empty row
    int emptyRow = getButtomEmptyPos(board, rows, column);
    
    
    //check if the column is completly empty
    if (emptyRow == rows - 1) {
        return !UNDO_SUCCESFULL;
    }
    
    //clear the board at pos
    board[emptyRow + 1][column] = EMPTY_POS;
    return UNDO_SUCCESFULL;
}


/**
 @brief function to make move on the board
 the function will validate the colums  and validate the player and check if the column is full
 @param board the board to check
 @param rows the board rows
 @param columns the board columns
 @param players the number of players
 @param player the player on the board
 @param column coulmn to insert
 @return 0 if the move wes unseccessfull 1 otherwise
 */
int makeMove(char board[ROWS][COLS], int rows, int columns, int players, char player, int column) {
    
    const int MOVE_SUCCESSFULL = 1;
    const int MIN_ROW_COL = 0;
    //validate the column in aspect of the board
    if (column < MIN_ROW_COL || column >= columns) {
        return !MOVE_SUCCESSFULL;
    }
    
    //init the player as cher
    int intgerPlayer = getPlayerAsInt(player);
    
    //validate the player
    if (!isValidPlayer(players, intgerPlayer)) {
        return !MOVE_SUCCESSFULL;
    }

    //get the last empty row
    int emptyRow = getButtomEmptyPos(board, rows, column);
    
    //check for a full column
    if (emptyRow < MIN_ROW_COL) {
        return !MOVE_SUCCESSFULL;
    }
    
    //set the player at the column
    board[emptyRow][column] = player;
    
    return MOVE_SUCCESSFULL;
}


/**
 @brief function to recieve the most buttom empty position in a given column
 the function checks the value in the board from buttom to top for empty position
 @param rows number of rows in the board
 @param column the given column to check
 @returns -1 if totally full column and row if that  row is empty at that column
 */
int getButtomEmptyPos(char board[ROWS][COLS], int rows, int column) {
    
    const int POS_NOT_FOUND = -1;
    //rum from buttom to top
    for (int row = rows - 1; row >= 0 ; row--) {
        //search for empty position and return the row if found
        if (board[row][column] == EMPTY_POS) {
            return row;
        }
    }
    //return not found
    return POS_NOT_FOUND;
}


/**
 @brief function to get 64 base number from an integer
 the function will convert the number
 @param input the number to convetr
 @return character in 64 or 0 if there wan't
 */
char getIntegerAs64Base(int input) {
    
    const int NOT_SUCCESS = -1;
    //init consts
    const int A_64_BASE = 0;
    const int LETTERS_COUNT = 26;
    const int SMALL_A_64_BASE = 26;
    const int ZERO_64_BASE = 52;
    const int NUMBER_COUNT = 9;
    const int PLUS_64BASE = 62;

    //if the input is in range of big charcter in 64 base
    if (input >= A_64_BASE && input <= A_64_BASE + LETTERS_COUNT - 1) {
        return 'A' + (input - A_64_BASE);
    }
    //if the input is in range of small charcter in 64 base
    if (input >= SMALL_A_64_BASE && input <= SMALL_A_64_BASE + LETTERS_COUNT - 1) {
        return 'a' + (input  - SMALL_A_64_BASE) ;
    }
    //if the input is in range of digit charcter in 64 base
    if (input >= ZERO_64_BASE && input <= ZERO_64_BASE + NUMBER_COUNT - 1) {
        return '0' + (input - ZERO_64_BASE);
    }
    //if the input is one of the 64base signs
    if (input == PLUS_64BASE) {
        return '+';
    } else {
        return '/';
    }
    
    //not succesfull return
    return NOT_SUCCESS;
}


/**
 @brief function to get integer from 64 base character
 @param input the charcter to convert
 @return interger represante the 64base input
 */
int get64BaseAsInteger(char input) {
    
    //init consts
    const char PLUS = '+';
    const int SMALL_A_BASE_DIFF = 26;
    const int ZERO_BASE_DIFF = 52;
    const int PLUS_64_BASE = 62;
    const int SLASH_64_BASE = 63;
    
    //if the char is in range og a big letter just subrtact A from it
    if (input >= 'A' && input <= 'Z') {
           return input -= 'A';
        //if the input is in range of small letter subtract from it the diff from the two
       } else if (input >= 'a' && input <= 'z') {
           return input -= ('a' - SMALL_A_BASE_DIFF);
           //if the input is a digit the same as small latter
       } else if (input >='0' && input <= '9') {
           return input -= ('0' - ZERO_BASE_DIFF);
           //if it is a special char
       } else if (input == PLUS) {
           return PLUS_64_BASE;
       }
    //no other option
    return SLASH_64_BASE;
}

/**
 @brief function to insert 64base character into the end of a string
 the function users pointer arithmetic to insert characters
 @param code the string pointer
 @param timesOfOccurences of a given chacrter on the board
 @param theChar the charcter to insert
 @return pointer to the new string
 */
char * insertBase64CharIntoString(char *code, char timesOfOccurences, char theChar) {
    
    //set the charcter time of occurences in 64 base and
    *code = getIntegerAs64Base(timesOfOccurences);
    //increment the pointer to the next char
    code++;
    //store the char
    *code = theChar;
    //increment again
    code++;
    
    //return the updated pointer
    return code;
}


/**
 @brief function that decodes a given board
 the function will check for end of string and store valued into the board
 @param board the board
 @param code the pointer
 */
void decode(const char *code, char board[ROWS][COLS]) {
    
    
    const char END_OF_LINE = '/';
    int row = 0;
    int col = 0;
    //while decode
    while (*code != '\0') {
        
        //get the number of chars to insert as interger
        int cols = get64BaseAsInteger(*code);
        
        //increment code to the next char
        code++;
        //get the current char to insert
        char currChar = *code;
        //increment again to check for end of row
        code++;
        
        //init i as 0..cols and subtracting cols..0
        for (int i = col ; cols > 0; cols--) {
            //store the col so we start next time from the same position
            col = i;
            board[row][i++] = currChar;
        }
        //go next
        col++;
        //end of line
        if (*code == END_OF_LINE) {
            //increment row
            row++;
            //set col to 0
            col = 0;
            //go next
            code++;
        }
    }
}


/**
 @brief function the encode a given board
 the function will encode charctres into a given board
 @param board the board
 @param code the pointer
 */
void encode(const char board[ROWS][COLS], int rows, int columns, char *code) {
    
    const char END_OF_ROW = '/';
    const char END_OF_STRING = '\0';
    const int RIGHT_BOUNDRY = columns - 2;
    int counter = 1;
    //run for evey cell in the board
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            
            //be carefull not to step out of array boundry
            if (col < RIGHT_BOUNDRY) {
                //check for the next charcter with the current one for equality
                if (board[row][col] == board[row][col + 1]) {
                    //increment the counter of the character
                    counter++;
                    //if it not equal
                } else {
                    //get the current store it as it is in the string
                    code = insertBase64CharIntoString(code, counter, board[row][col]);
                    //reset counter
                    counter = 1;
                }
            } else {
                //we reach to the end of the column
                if (board[row][col] == board[row][col + 1]) {
                    //if we founde we increment and storing it
                    counter++;
                    code = insertBase64CharIntoString(code, counter, board[row][col]);
                    //else we store the current and the next seperatly
                } else {
                    code = insertBase64CharIntoString(code, counter, board[row][col]);
                    code = insertBase64CharIntoString(code, 1, board[row][col + 1]);
            
                }
                //reset counter
                counter = 1;
                //break for the end of the row
                break;
            }
        }
        //store end of the row character
        *code = END_OF_ROW;
        code++;
    }
    //finish the string with backslash 0
    *code = END_OF_STRING;
}

void run() {

    char board[ROWS][COLS];
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);

    char winner = -1;
    char status = -1;
    int turn = 1;
    int col;
    char pl = 'A';

    while (winner == -1 && status == -1) {
        printf("Enter a column: ");
        scanf("%d", &col);

        if (turn) {
            pl = 'A';
        } else {
            pl = 'B';
        }

        if (!makeMove(board, ROWS, COLS, NUM_PLAYERS, pl, col)) {
            printf("invalid column\n");
            continue;
        } else {
            turn = !turn;
            printBoard(board, ROWS, COLS);
        }

        winner = getWinner(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
        status = getStatus(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
    }

    if (status == 1) {
        printf("Game-over\n");
    } else if (status == 0) {
        printf("Tie\n");
    } 

    if (winner != -1) {
        printf("The winner is %c\n", winner);
    }
}


int main() {
    run();
}

