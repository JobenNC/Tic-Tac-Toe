/* 
 * File:   tic.c
 * Author: Joseph M Jarriel
 *
 * Play a game of Tic Tac Toe against another person or a computer.
 * Or, if you'd prefer, watch as two hyper-intelligent super computers
 * battle for your amusement!  
 *
 * Usage: Run tic, follow the prompts to seed the number generator
 * and choose the AI level for each player.
 * 
 */

/* DO NOT CHANGE THIS CODE */

#include <stdio.h>
#include <stdlib.h>

/* Public-domain pseudorandom number generator -- code at end of this file
 * To use -- 
 * (1) call ranf_start with a seed, like srand.  Don't worry 
 *     about the "long" -- just use an int value.  
 * (2) When you want a random integer between 0 and n-1, call ranf_next(n). 
 */
void ranf_start(long seed);  /* seed generator */
int ranf_next(int limit);    /* get random int between 0 and limit-1, inclusive */
void ranf_test();            /* basic test code -- sanity check */

/* Declarations for the functions you will implement */

void playGame(int first, int xLevel, int oLevel); 
/* first: 1=X, zero=O; level: 0=novice, 1=intermediate, 2=advanced, 3=human */

int gameCheck(const int board[]);
/* returns 1 if X wins, zero if O wins, 2 if tie, -1 if not over */

/* in move functions, player = one (X) or zero (O) */
void noviceMove(int board[], int player);
void interMove(int board[], int player);
void advMove(int board[], int player);
void humanMove(int board[], int player);

void printBoard(const int board[]);


/* GLOBAL VARIABLES ARE NOT ALLOWED FOR THIS ASSIGNMENT */

/* If you write additional functions, put their declarations AFTER
 * the main() function.  */



#ifndef TESTING  /* DO NOT CHANGE THIS LINE, OR THE MAIN FUNCTION  */

int main() {
    
    int seed;
    int levelX = -1, levelO = -1;
    int first;  /* if 1, X goes first; if 0, O goes first */
 
    /***********************************************************************/
    /* Here we seed and call ranf_test 10 times, to make sure that 
     * you are getting the same sequence of random numbers as your graders.  
     * Verify that the 10 numbers you see match exactly the 10 numbers
     * in the program spec. 
     * 
     */
    ranf_start(1013); 
    ranf_test();  
    /***********************************************************************/
    
    printf("TIC TAC TOE\n\n");
    
    /* Seed the pseudorandom number generator, using user input */
    printf("Enter a seed: ");
    scanf("%d", &seed);
    ranf_start(seed);
    
    /* Get level if X player (repeat until legal) */
    while ((levelX < 0) || (levelX > 3)) {
        printf("Level of X (0=novice,1=intermediate,2=advanced,3=human): ");
        scanf("%d",&levelX);
    }
    /* Get level of O player (repeat until legal) */
    while ((levelO < 0) || (levelO > 3)) {
        printf("Level of O (0=novice,1=intermediate,2=advanced,3=human): ");
        scanf("%d",&levelO);
    }
    
    /* choose first player and play game */
    printf("Flipping a coin... ");
    first = ranf_next(2);
    if (first) printf("X goes first\n\n");
    else printf("O goes first\n\n");
    
    playGame(first, levelX, levelO);

    return EXIT_SUCCESS;
}

#endif  

/*************************************************************************/
/*   DO NOT CHANGE ANYTHING ABOVE THIS LINE,                             */
/*   EXCEPT FOR THE PROGRAM HEADER                                       */
/*************************************************************************/

/* If you write additional functions, put their declarations here.  */
void printSquare(int squareNum);

int almostCheck(const int board[], int player);

void printChoice();

/* PUT YOUR FUNCTION DEFINITIONS HERE */
void printChoice(){
    /*input: None
     *output: Void
     *
     *Print out a representation of the board that shows
     *the user which numbers correspond with which space
    */
    int i;
    int j;

    j = 0;

    for (i = 0; i < 9; i++){
        /*newline every 3 chars*/
        if (((i % 3) == 0) || (i == 0)) printf("\n");
        printf(" %d ", i);
        if (j == 2) j = 0;
        /*seperate with '|'*/
        else{
            printf("|");
            j++;
        }
    }
    printf("\n");
}

void printSquare(int squareNum){
    /*input: array representation of the board
     *output: Void
     *
     *Called for each square on the board by
     *printScreen to determine which character
     *gets printed
    */
    if (squareNum == -1) printf(" - ");
    else if (squareNum == 0) printf(" O ");
    else if (squareNum == 1) printf(" X ");
}

int almostCheck(const int board[], int player){
    /*input: array repr of the board, player number
     *output: number representing a square to play, or neg 1
     *
     *Check board to see if input player has a potential
     *win in any direction.  This function is called for the 
     *opposing player to check for a potential win block.
    */
    int count;
    int open;
    int i;

    count = 0;
    open = -1;

    /*Check verticaly for a potential win*/
    for (i = 0; i < 3; i++){
       if (board[i] == player) count = count + 1; 
       else if (board[i] == -1) open = i; 
       if (board[i + 3] == player) count = count + 1; 
       else if (board[i + 3] == -1) open = i + 3; 
       if (board[i + 6] == player) count = count + 1; 
       else if (board[i + 6] == -1) open = i + 6; 
       if ((count == 2) && (open != -1)) return open;
       count = 0;
       open = -1;
    }

    /*Check horizontally for a potential win*/
    for (i = 0; i < 7; i = i + 3){
       if (board[i] == player) count = count + 1; 
       else if (board[i] == -1) open = i; 
       if (board[i + 1] == player) count = count + 1; 
       else if (board[i + 1] == -1) open = i + 1; 
       if (board[i + 2] == player) count = count + 1; 
       else if (board[i + 2] == -1) open = i + 2; 
       if ((count == 2) && (open != -1)) return open;
      count = 0;
      open = -1;
    }

    /*Check horizontally from top left to bottom right*/
    if (board[0] == player) count = count + 1; 
    else if (board[0] == -1) open = 0; 
    if (board[4] == player) count = count + 1; 
    else if (board[4] == -1) open = 4; 
    if (board[8] == player) count = count + 1; 
    else if (board[8] == -1) open = 8; 

    if ((count == 2) && (open != -1)) return open;

    count = 0;
    open = -1;

    /*Check horizontally from top right to bottom left*/
    if (board[2] == player) count = count + 1; 
    else if (board[2] == -1) open = 2; 
    if (board[4] == player) count = count + 1; 
    else if (board[4] == -1) open = 4; 
    if (board[6] == player) count = count + 1; 
    else if (board[6] == -1) open = 6; 

    if ((count == 2) && (open != -1)) return open;

    /*return neg 1 if no potential win*/
    return -1;
}

void playGame(int first, int xLevel, int oLevel){
    /*input: first player, x player ai level, y player ai level
     *outpu: Void
     *
     *first: 1=X, 0=O; 
     *level: 0=novice, 1=intermediate, 2=advanced, 3=human
     *
     *Main loop will continue until game is over
    */

    int over;
    int player;
    int newPlayer;
    int level;
    /*Board is initialized as empty*/
    int board[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    /*Set the coin toss winner*/
    player = first;

    over = -1;

    /*Main gameplay loop*/
    while (over == -1){

        if (player == 1){
            level = xLevel;
            newPlayer = 0;
            printf("X's turn-----\n");
        }
        else{
            level = oLevel;
            newPlayer = 1;
            printf("O's turn-----\n");
        }

        /*nicely formatted copy of board with squares labeled*/
        printChoice();

        if (level == 0) noviceMove(board, player);
        else if (level == 1) interMove(board, player);
        else if (level == 2) advMove(board, player);
        else if (level == 3) humanMove(board, player);

        printBoard(board);

        /*Switch players*/
        player = newPlayer;

        over = gameCheck(board);

    }

    /*Declare the victor, or lack thereof*/
    if (over == 1) printf("Mighty X is victorious!\n");
    else if (over == 0) printf("Mighty O is victorious!\n");
    else printf("Nobody wins!\n");

    return;
}

int gameCheck(const int board[]){
    /*input: array representation of the board, will not be changed
     *output: number representing the game state
     *
     *returns 1 if X wins, 0 if O wins, 2 if tie, -1 if not over
    */
    int i;
    int j;
    
    j = 0;

    /*check for a vertical match*/
    for (i = 0; i < 3; i++){
        if ((board[i] == 1) && (board[i+3] == 1) && (board[i+6] == 1)) return 1; 
        if ((board[i] == 0) && (board[i+3] == 0) && (board[i+6] == 0)) return 0;
    }
    /*check for a horizontal match*/
    for (i = 0; i < 8; i = i + 3){
        if((board[i] == 1) && (board[i+1] == 1) && (board[i+2] == 1)) return 1;
        else if((board[i] == 0) && (board[i+1] == 0) && (board[i+2] == 0)) return 0;
    }

    /*Check for a diag match, top left, bottom right*/
    if((board[0] == 1) && (board[4] == 1) && (board[8] == 1)) return 1;
    else if((board[0] == 0) && (board[4] == 0) && (board[8] == 0)) return 0;

    /*Check for a diag match, top right, bottom left*/
    if((board[2] == 1) && (board[4] == 1) && (board[6] == 1)) return 1;
    else if((board[2] == 0) && (board[4] == 0) && (board[6] == 0)) return 0;

    /*If board is full and no one has one, tie*/
    for (i=0; i<9; i++){
        if (board[i] != -1) j = j + 1; 
    }
    if (j == 9) return 2;

    /*play on!*/
    return -1;
}

void noviceMove(int board[], int player){
    /*input: array repr of the board, player
     *output: Void
     *
     *Makes a random move, doens't consider board
     *changes the board
    */
    int square;

    square = ranf_next(9);

    /*if occupied square is randomly chosen, keep at it*/
    while (board[square] != -1){
        square = ranf_next(9);
    }

    board[square] = player;

    return;
}
void interMove(int board[], int player){
    /*input: array repr of board, player
     *output: Void
     *
     *first, look for opportunity for the player to win, if found, do it
     *second, look for opportunity for other player to win, if found, block
     *otherwise, make a random move
     *changes board
    */
    int square;
    int oppPlayer;
    int check;

    square = -1;

    if (player == 1) oppPlayer = 0;
    if (player == 0) oppPlayer = 1;
    
    /*first check for potential win by player*/
    check = almostCheck(board, player);
    if (check != -1) square = check;
    /*then, check for potential win by opposing player*/
    else{
        check = almostCheck(board, oppPlayer);
        if (check != -1) square = check;
    }
   
    /*otherwise, play like the novice*/
    if (square == -1) square = ranf_next(9);

    while (board[square] != -1){
        square = ranf_next(9);
    }

    board[square] = player;

    return;
}
void advMove(int board[], int player){
    /*input: array repr of board, player
     *output: Void
     *
     *just like intermediate, with one exception
     *first move, choose either 1)center square or 2)top right square
     *That order of preference
     *For every other move, acts just like intermediate
    */
    int square;
    int oppPlayer;
    int check;
    int i;
    int tally;

    /*same as intMove*/
    square = -1;

    if (player == 1) oppPlayer = 0;
    if (player == 0) oppPlayer = 1;
    
    check = almostCheck(board, player);
    if (check != -1) square = check;
    else{
        check = almostCheck(board, oppPlayer);
        if (check != -1) square = check;
    }

    /*Here's where this function differs from intMove. if it's the first move,
    *choose square 4, or square 2
    */
    tally = 0;
    for (i = 0; i < 9; i++){
        if (board[i] != -1) tally = tally + 1;
    }

    if (tally < 2){
        if (board[4] == -1) square = 4;
        else square = 2;
    }

    /*same as noviceMove*/
    if (square == -1) square = ranf_next(9);

    while (board[square] != -1){
        square = ranf_next(9);
    }

    board[square] = player;

    return;
}
void humanMove(int board[], int player){
    /*input: array repr of board, player
     *output: Void
     *
     *prompt user to enter move, spec by single dec int [0,8]
     *input will be b/w 0-8, but if occupied square is chosen, prompt again
     *don't give up!
    */
    int square;

    printf("Please select a square:");
    scanf("%d", &square);
    printf("\n");

    /*keep prompting until valid input is received*/
    while (board[square] != -1){
        printf("Square was occupied, please select another square:");
        scanf("%d", &square);
    }

    board[square] = player;

    return;
}

void printBoard(const int board[]){
    /*input: array repr of the board
     *outpu: Void
     *
     *prints a visual representation of the board's current state to the screen
    */
    int i;
    int j;

    j = 0;
    /*print a newline after every 3 items in the array*/
    for (i = 0; i < 9; i++){
        if (((i % 3) == 0) || (i == 0)) printf("\n");
        printSquare(board[i]);
        if (j == 2) j = 0;
        else{
            printf("|");
            j++;
        }
    }
    printf("\n");
    printf("\n");
    return;
}

/*************************************************************************/
/*   DO NOT CHANGE ANYTHING PAST THIS LINE                               */
/*************************************************************************/


void ranf_test() {
    /* Here's some sample code that prints 10 random numbers less than 10. */
    int i;
    for (i=0; i<10; i++) {
        printf("%d ", ranf_next(10));
    }
    printf("\n");
    return;
}

/* ranf_next() added by gbyrd */

/*    This program by D E Knuth is in the public domain and freely copyable
 *    AS LONG AS YOU MAKE ABSOLUTELY NO CHANGES!
 *    It is explained in Seminumerical Algorithms, 3rd edition, Section 3.6
 *    (or in the errata to the 2nd edition --- see
 *        http://www-cs-faculty.stanford.edu/~knuth/taocp.html
 *    in the changes to Volume 2 on pages 171 and following).              */

/*    N.B. The MODIFICATIONS introduced in the 9th printing (2002) are
      included here; there's no backwards compatibility with the original. */

/*    This version also adopts Brendan McKay's suggestion to
      accommodate naive users who forget to call ranf_start(seed).         */

/*    If you find any bugs, please report them immediately to
 *                 taocp@cs.stanford.edu
 *    (and you will be rewarded if the bug is genuine). Thanks!            */

/************ see the book for explanations and caveats! *******************/
/************ in particular, you need two's complement arithmetic **********/

#define KK 100                     /* the long lag */
#define LL  37                     /* the short lag */
#define mod_sum(x,y) (((x)+(y))-(int)((x)+(y)))   /* (x+y) mod 1.0 */

double ran_u[KK];           /* the generator state */

#ifdef __STDC__
void ranf_array(double aa[], int n)
#else
void ranf_array(aa,n)    /* put n new random fractions in aa */
  double *aa;   /* destination */
  int n;      /* array length (must be at least KK) */
#endif
{
  register int i,j;
  for (j=0;j<KK;j++) aa[j]=ran_u[j];
  for (;j<n;j++) aa[j]=mod_sum(aa[j-KK],aa[j-LL]);
  for (i=0;i<LL;i++,j++) ran_u[i]=mod_sum(aa[j-KK],aa[j-LL]);
  for (;i<KK;i++,j++) ran_u[i]=mod_sum(aa[j-KK],ran_u[i-LL]);
}

/* the following routines are adapted from exercise 3.6--15 */
/* after calling ranf_start, get new randoms by, e.g., "x=ranf_arr_next()" */

#define QUALITY 1009 /* recommended quality level for high-res use */
double ranf_arr_buf[QUALITY];
double ranf_arr_dummy=-1.0, ranf_arr_started=-1.0;
double *ranf_arr_ptr=&ranf_arr_dummy; /* the next random fraction, or -1 */

#define TT  70   /* guaranteed separation between streams */
#define is_odd(s) ((s)&1)

#ifdef __STDC__
void ranf_start(long seed)
#else
void ranf_start(seed)    /* do this before using ranf_array */
  long seed;            /* selector for different streams */
#endif
{
  register int t,s,j;
  double u[KK+KK-1];
  double ulp=(1.0/(1L<<30))/(1L<<22);               /* 2 to the -52 */
  double ss=2.0*ulp*((seed&0x3fffffff)+2);

  for (j=0;j<KK;j++) {
    u[j]=ss;                                /* bootstrap the buffer */
    ss+=ss; if (ss>=1.0) ss-=1.0-2*ulp;  /* cyclic shift of 51 bits */
  }
  u[1]+=ulp;                     /* make u[1] (and only u[1]) "odd" */
  for (s=seed&0x3fffffff,t=TT-1; t; ) {
    for (j=KK-1;j>0;j--)
      u[j+j]=u[j],u[j+j-1]=0.0;                         /* "square" */
    for (j=KK+KK-2;j>=KK;j--) {
      u[j-(KK-LL)]=mod_sum(u[j-(KK-LL)],u[j]);
      u[j-KK]=mod_sum(u[j-KK],u[j]);
    }
    if (is_odd(s)) {                             /* "multiply by z" */
      for (j=KK;j>0;j--) u[j]=u[j-1];
      u[0]=u[KK];                    /* shift the buffer cyclically */
      u[LL]=mod_sum(u[LL],u[KK]);
    }
    if (s) s>>=1; else t--;
  }
  for (j=0;j<LL;j++) ran_u[j+KK-LL]=u[j];
  for (;j<KK;j++) ran_u[j-LL]=u[j];
  for (j=0;j<10;j++) ranf_array(u,KK+KK-1);  /* warm things up */
  ranf_arr_ptr=&ranf_arr_started;
}


#define ranf_arr_next() (*ranf_arr_ptr>=0? *ranf_arr_ptr++: ranf_arr_cycle()) 
double ranf_arr_cycle()
{
  if (ranf_arr_ptr==&ranf_arr_dummy)
    ranf_start(314159L); /* the user forgot to initialize */
  ranf_array(ranf_arr_buf,QUALITY);
  ranf_arr_buf[KK]=-1;
  ranf_arr_ptr=ranf_arr_buf+1;
  return ranf_arr_buf[0];
}

int ranf_next(int limit) {
    /* ranf_arr_next() returns a fraction less than 1 */
    /* multiply by a number slightly less than limit, to ensure result 
     * will be between 0 and limit-1, inclusive 
     */
    return (int) (ranf_arr_next() * ((double)limit) - 1.0e-6);
}
