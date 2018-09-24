#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum{NOUGHTS, CROSSES, BORDER, EMPTY};
enum{HUMAN_WIN, COMP_WIN, DRAW};
const int Directions[4] = {1, 4, 5, 6};
const int ConvertTo25[9] = {
 6,7,8,
 11,12,13,
 16,17,18
};
const int InMiddle = 4;
const int inCorners[4] = {0, 2, 6, 8};
void intializeBoard(int* board)
{
    int index;
    for(index = 0; index < 25; ++index)
    {
          board[index] = BORDER;
    }
    for(index = 0; index < 9; ++index)
    {
        board[ConvertTo25[index]] = EMPTY;
    }

}
int getNextBest(const int *board)
{
    int ourMove = ConvertTo25[InMiddle];
    if(ourMove == EMPTY)
    {
        return ourMove;
    }
    ourMove = -1;
    int index = 0;
    for(index = 0; index < 4; index++)
    {
        ourMove = ConvertTo25[inCorners[index]];
        if(ourMove == EMPTY)
        {
            break;
        }
        ourMove = -1;
    }
    return ourMove;
}
int getNumberForDirection(int startSq, const int direction, const int *board, const int us)
{
    int found = 0;
    while(board[startSq]!= BORDER)
    {
        if(board[startSq] !=us)
        {
            break;
        }
        found++;
        startSq+=direction;
    }
    return found;
}
int findThreeInARow(const *board, const int ourIndex, const int us)
{
    int DirIndex = 0;
    int Dir = 0;
    int threeCount = 1;
    for(DirIndex = 0; DirIndex < 4; ++DirIndex)
    {
        Dir = Directions[DirIndex];
        threeCount+=getNumberForDirection(ourIndex + Dir, Dir, board, us);
        threeCount+=getNumberForDirection(ourIndex + Dir *-1, Dir *-1, board, us);
        if(threeCount ==  3)
        {
            break;
        }
        threeCount = 1;

    }
    return threeCount;
}
void printBoard(const int* board)
{
    int index;
    char pieceChar[] = "OX|-";
    printf("\n\nBoard: \n\n");
    for(index = 0; index < 9; ++index)
    {
          if(index!=0 && index%3==0)
          {

              printf("\n\n");
          }
          printf("%4c", pieceChar[board[ConvertTo25[index]]]);
    }
   printf("\n");
}
int hasEmpty(const int *board)
{
    int index = 0;
    for(index = 0; index < 9; ++index)
    {

        if(board[ConvertTo25[index]]== EMPTY)
        {
            return 1;
        }
    }
    return 0;
}

void makeMove(int *board, const int sq, const side)
{
    board[sq] = side;
}
int getHumanMove(const int* board)
{
    char userInput[4];
    int moveOk = 0;
    int move = -1;
    while(moveOk == 0)
    {
        printf("Please enter a move from 1 to 9: ");
        fgets(userInput, 3, stdin);
        fflush(stdin);
        if(strlen(userInput)!= 2)
        {
            printf("Invalid strlen()\n");
            continue;

        }
        if(sscanf(userInput, "%d", &move) != 1)
        {
            move =-1;
            printf("Invalid sscanf()\n");
            continue;
        }
        if(move < 1 || move > 9)
        {
            move =-1;
            printf("Invalid range\n");
            continue;

        }
        move--; //Zero indexing
        if(board[ConvertTo25[move]]!= EMPTY)
        {
            move = -1;
            printf("Square not available\n");
            continue;
        }
        moveOk = 1;
    }
    printf("Making Move....%d\n",(move + 1));
    return ConvertTo25[move];

}
int getWinningMove(int* board, const int side)
{
    int ourMove = -1;
    int winMove = 0;
    int index = 0;
    for(index = 0; index < 9; ++index)
    {
        if(board[ConvertTo25[index]]==EMPTY)
        {
            ourMove = ConvertTo25[index];
            board[ourMove] = side;
            if(findThreeInARow(board, ourMove, side)== 3)
            {
                winMove = 1;
            }
            board[ourMove]= EMPTY;
            if(winMove == 1)
            {
                break;
            }
            ourMove = -1;
        }
    }
    return ourMove;
}
int getComputerMove(int* board, const int side)
{
    int index = 0, numFree =0, availableMoves[9], randMove =0;
    randMove = getWinningMove(board, side);
    if(randMove != -1)
    {
        return randMove;
    }
    randMove = getWinningMove(board, side^1);
    if(randMove != -1)
    {
        return randMove;
    }


    randMove = getNextBest(board);
    if(randMove != -1)
    {
        return randMove;
    }

     randMove = 0;
    for(index = 0; index < 9; ++index)
    {
        if(board[ConvertTo25[index]]== EMPTY)
        {
            availableMoves[numFree++] = ConvertTo25[index];

        }
    }
    randMove = (rand()% numFree);
    return availableMoves[randMove];

}
void runGame()
{
    int GameOver = 0;
    int Side = NOUGHTS;
    int LastMoveMade = 0;
    int board[25];
    intializeBoard(&board[0]);
    printBoard(&board[0]);
    while(!GameOver)
    {
        if(Side == NOUGHTS)//get move from human, make move on board, change side
        {
            LastMoveMade = getHumanMove(&board[0]);
            makeMove(&board[0], LastMoveMade, Side);
            Side = CROSSES;

        }
        else//get move from computer, make move on board, change side
        {
            LastMoveMade = getComputerMove(&board[0], Side);
            makeMove(&board[0], LastMoveMade, Side);
            Side = NOUGHTS;
            printBoard(&board[0]);
        }
        if(findThreeInARow(board,LastMoveMade, Side ^ 1)== 3)
        {
            printf("Game over!\n");
            GameOver = 1;
            if(Side == NOUGHTS)
            {
                printf("The computer wins\n");
            }
            else
            {
                printf("Human Wins\n");
            }
        }
        if(!hasEmpty(board))
        {
            printf("Game over!\n");
            GameOver = 1;
            printf("It's a draw");
        }
    }
   printBoard(&board[0]);
}
int main()
{
    srand(time(NULL));
    runGame();
    return 0;
}
