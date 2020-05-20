/*
 1. there are no moves to capture an opponent’s piece;
 
 2. when a man reaches the kings row (also called
 crownhead, the farthest row forward),it does not become
 a king but, instead, can no longer be moved (thus there
 are no kings).
 */
#include <stdio.h>

//creating the checkboard and setting pieces
void initBoard(int n, int board[n][n])
{
    int i, j;
    //setting an empty checkboard
    for (i = 0; i < n; i++) {
        for(j = 0; j < n; j++)
            board[i][j] = 0;
    }
    //setting white pieces (1)
    for (i = 0; i < ((n/2) - 1); i++) {
        if (i % 2 == 0) {
            for (j = 0; j < n; j += 2)
                board[j][i] = 1;
        }
        else {
            for(j = 1; j < n; j += 2)
                board[j][i] = 1;
        }
    }
    //setting dark pieces (-1)
    for (i = n-1; i >= (n - ((n/2) - 1)); i--) {
        if (i % 2 == 1) {
            for(j = 1; j < n; j += 2)
                board[j][i] = -1;
        }
        else {
            for (j = 0; j < n; j += 2)
                board[j][i] = -1;
        }
    }
}

//the function prints the checkboard to the console
void printBoard(int n, int board[n][n])
{
    int i, j;
    for (i = n - 1; i >= 0; i--) {
        for (j = 0; j < n; j++) {
            switch (board[j][i]) {
                case 0:
                    printf("-");
                    break;

                case 1:
                    printf("W");
                    break;

                case -1:
                    printf("D");
                    break;
                default:
                    break;
            }
        }
        printf("\n");
    }
}

//the function moves the pieces and informs the user about the implications
void mymove(int n, int board[n][n], int x, int y, char direction)
{
    //informing the user that the chosen square is empty
    if (board[x][y] == 0)
        printf("There is no piece in column %i and row %i of the checkboard\n", x, y);
    //in case the chosen square contains dark piece
    else if (board[x][y] == -1) {
        if ((direction == 'L' && board[x-1][y-1] != 0) || (direction == 'R' && board[x+1][y-1] != 0))
            printf("There is already a piece in the square to which we are supposed to move\n");
        else if (y == 0)
            printf("Your piece is on the kings row");
        else if (direction == 'R' && x == n - 1)
            printf("%i is the rightmost column\n", x);
        else if (direction == 'L' && x == 0)
            printf("%i is the leftmost column\n", x);
        else {
            if (direction == 'R') {
                board[x+1][y-1] = -1;
                printf("The dark piece was moved from [%i, %i] to [%i, %i]\n", x, y, x + 1, y - 1);
            }
            else {
                board[x-1][y-1] = -1;
                printf("The dark piece was moved from [%i, %i] to [%i, %i]\n", x, y, x - 1, y - 1);
            }
            board[x][y] = 0;
        }
    }

    //in case the chosen square contains white piece
    else if (board[x][y] == 1) {
        if ((direction == 'L' && board[x-1][y+1] != 0) || (direction == 'R' && board[x+1][y+1] != 0))
            printf("There is already a piece in the square to which we are supposed to move\n");
        else if (y == n - 1)
            printf("Your piece is on the kings row");
        else if (direction == 'R' && x == n-1)
            printf("%i is the rightmost column\n", x);
        else if (direction == 'L' && x == 0)
            printf("%i is the leftmost column\n", x);
        //in case of no implications, move the piece, and free its previous square
        else {
            if (direction == 'R') {
                board[x+1][y+1] = 1;
                printf("The dark piece was moved from [%i, %i] to [%i, %i]\n", x, y, x + 1, y + 1);
            }
            else {
                board[x-1][y+1] = 1;
                printf("The dark piece was moved from [%i, %i] to [%i, %i]\n", x, y, x - 1, y + 1);
            }
            board[x][y] = 0;
        }
    }
}

//the functions prints the current state of the checkboard to file
void saveBoard(int n, int board[n][n], char file[])
{
    FILE *file_to_save;
    file_to_save = fopen(file, "w");
    int i, j;
    for (i = n - 1; i >= 0; i--) {
        for (j = 0; j < n; j++) {
            switch (board[j][i]) {
                case 0:
                    fprintf(file_to_save, "-");
                    break;
                case 1:
                    fprintf(file_to_save, "W");
                    break;
                case -1:
                    fprintf(file_to_save, "D");
                    break;
                default:
                    break;
            }
        }
        fprintf(file_to_save, "\n");
    }
    fclose(file_to_save);
}

/*
defining the struct named "Game",
which will store the number of white
and dark pieces
 */
typedef struct {
    int white;
    int dark;
} Game;

/*
 the function counts the number of
 white and dark pieces and returns
 the resultant structure "count"
 */
Game countBoard(int n, int board[n][n])
{
    Game count = {0, 0};
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (board[i][j] == 1)
                count.white++;
            else if (board[i][j] == -1)
                count.dark++;
        }
    }
    return count;
}

/*
 the functions reads file, which contains the status of the game,
 stores the status of the game in the n×n checkboard board and returns
 a resultant structure
*/
Game loadBoard(int n, int board[n][n], char file[])
{
    FILE *file_to_read;
    file_to_read = fopen(file, "r");
    Game tofile = {0, 0};
    char ch;
    int i, j;
    
    for (i = n - 1; i >= 0; i--) {
        for (j = 0; j < n; j++) {
            ch = getc(file_to_read);
            if (ch == 'W') {
                board[j][i] = 1;
                tofile.white++;
            }
            else if (ch == 'D') {
                board[j][i] = -1;
                tofile.dark++;
            }
            else if (ch == '-') {
                board[j][i] = 0;
            }
            else {
                j--;
                continue;
                }
            }
        }
    fclose(file_to_read);
    return tofile;
}

int main()
{
    //just some testing
    int n = 8;
    int cb[n][n];
    //task1
    initBoard(n, cb);
    //task2
    printBoard(n, cb);
    printf("\n");
    //task3
    mymove(n, cb, 2, 2, 'R');
    mymove(n, cb, 1, 5, 'R');
    printBoard(n, cb);
    //task4
    saveBoard(n, cb, "testing.txt");
    //task5
    Game countb = countBoard(n, cb);
    printf("Whites: %i\nDarks: %i\n\n", countb.white, countb.dark);
    //task6
    Game load = loadBoard(n, cb, "testing.txt");
    printf("Whites: %i\nDarks: %i\n", load.white, load.dark);
    printBoard(n, cb);
    return 0;
}
