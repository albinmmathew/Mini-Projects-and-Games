#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 8
#define MINES 10

// ---- DATA STRUCTURES ----
typedef struct {
    int isMine;
    int revealed;
    int flagged;
    int neighborMines;
} Cell;

typedef struct MineNode {
    int r, c;
    struct MineNode *next;
} MineNode;

typedef struct {
    int r, c;
    char action; // 'R' for reveal, 'F' for flag
} Move;

// ---- GLOBALS ----
Cell board[SIZE][SIZE];
MineNode *mineHead = NULL;
Move moveStack[SIZE * SIZE];
int top = -1;
int flagCount = 0; // track how many flags are placed


// ---- STACK FUNCTIONS ----
void pushMove(int r, int c, char action) {
    if (top < SIZE * SIZE - 1)
        moveStack[++top] = (Move){r, c, action};
}

Move popMove() {
    if (top >= 0)
        return moveStack[top--];
    else
        return (Move){-1, -1, 'X'};
}

// ---- LINKED LIST FUNCTIONS ----
void addMine(int r, int c) {
    MineNode *newNode = (MineNode *)malloc(sizeof(MineNode));
    newNode->r = r;
    newNode->c = c;
    newNode->next = mineHead;
    mineHead = newNode;
}

// ---- COUNT NEIGHBORS ----
int countMines(int r, int c) {
    int i, j, count = 0;
    for (i = -1; i <= 1; i++)
        for (j = -1; j <= 1; j++) {
            int nr = r + i, nc = c + j;
            if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE && board[nr][nc].isMine)
                count++;
        }
    return count;
}

// ---- REVEAL FUNCTION ----
void reveal(int r, int c) {
    if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || board[r][c].revealed || board[r][c].flagged)
        return;
    board[r][c].revealed = 1;
    pushMove(r, c, 'R');

	int i, j;
    if (board[r][c].neighborMines == 0)
        for (i = -1; i <= 1; i++)
            for (j = -1; j <= 1; j++)
                if (!(i == 0 && j == 0))
                    reveal(r + i, c + j);
}

// ---- PRINT BOARD ----
void printBoard(int showMines) {
	int i, j;
    printf("\n   ");
    for (i = 0; i < SIZE; i++)
        printf("%d ", i);
    printf("\n");

    for (i = 0; i < SIZE; i++) {
        printf("%d  ", i);
        for (j = 0; j < SIZE; j++) {
            if (board[i][j].flagged)
                printf("F ");
            else if (board[i][j].revealed)
                printf("%d ", board[i][j].neighborMines);
            else if (showMines && board[i][j].isMine)
                printf("* ");
            else
                printf(". ");
        }
        printf("\n");
    }
}

// ---- CHECK WIN ----
int checkWin() {
    int safeCells = SIZE * SIZE - MINES;
    int revealedCells = 0;
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            if (board[i][j].revealed && !board[i][j].isMine)
                revealedCells++;
    return revealedCells == safeCells;
}

// ---- MAIN ----
int main() {
    srand(time(0));
    int i, j;

    // Initialize board
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            board[i][j] = (Cell){0, 0, 0, 0};

    // Place mines dynamically
    int placed = 0;
    while (placed < MINES) {
        int r = rand() % SIZE, c = rand() % SIZE;
        if (!board[r][c].isMine) {
            board[r][c].isMine = 1;
            addMine(r, c);
            placed++;
        }
    }

    // Calculate neighbors
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            if (!board[i][j].isMine)
                board[i][j].neighborMines = countMines(i, j);

    printf("\n?? MINESWEEPER GAME ??\nCommands:\n");
    printf("R row col -> Reveal cell\n");
    printf("F row col -> Flag/Unflag cell\n");
    printf("U -> Undo last move\n");
    printf("Q -> Quit game\n");

    // ---- GAME LOOP ----
    char cmd;
    while (1) {
        printBoard(0);
        printf("\nEnter command: ");
        scanf(" %c", &cmd);

        if (cmd == 'Q' || cmd == 'q') {
            printf("\nExiting game. Goodbye!\n");
            break;
        }

        if (cmd == 'U' || cmd == 'u') {
            Move last = popMove();
            if (last.action == 'R')
                board[last.r][last.c].revealed = 0;
            else if (last.action == 'F')
                board[last.r][last.c].flagged = !board[last.r][last.c].flagged;
            continue;
        }

        int r, c;
        scanf("%d %d", &r, &c);

        if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) {
            printf("Invalid input!\n");
            continue;
        }

        if (cmd == 'F' || cmd == 'f') {
    		if (!board[r][c].flagged) { // trying to place a flag
        		if (flagCount >= MINES) {
            		printf("??  You’ve already used all %d flags!\n", MINES);
            		continue;
        		}
        		board[r][c].flagged = 1;
        		flagCount++;
        		printf("?? Flag placed! Flags used: %d/%d\n", flagCount, MINES);
    		} else { // removing a flag
        		board[r][c].flagged = 0;
        		flagCount--;
        		printf("?? Flag removed! Flags used: %d/%d\n", flagCount, MINES);
    		}
    		pushMove(r, c, 'F');
    		continue;
}

        if (cmd == 'R' || cmd == 'r') {
            if (board[r][c].isMine) {
                printf("\n?? Boom! You hit a mine!\n");
                printBoard(1);
                break;
            }
            reveal(r, c);
        }

        if (checkWin()) {
            printf("\n?? Congratulations! You cleared all safe cells!\n");
            printBoard(1);
            break;
        }
    }

    return 0;
}
