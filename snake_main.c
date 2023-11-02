#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <fcntl.h>
//#include <time.h>

#include "snake.h"

int main()
{
    struct SNAKE Snake;
    struct FreeCells FreeCells;
    struct FRUIT Fruit;

    int PlayGround [Xdim][Ydim];

    // time_t currenTime;
    // time_t starTime;

    int Movement[2]={-1,0};
    bool collision = false;
    bool isFruit = false;
    bool isEmpty = true;

    initSnake (&Snake, Movement);
    initPlayGround  (PlayGround);
    printPlayGround (PlayGround);

    while(!collision && isEmpty)
    {
        if (!isFruit)
        {
            isEmpty= emptyElements(PlayGround, &FreeCells);
            genFruit(PlayGround, FreeCells, &Fruit);
            isFruit=true;
        }
        readCommand(Movement);
        collision=moveSnake(PlayGround,&Snake, Movement, &isFruit);
        printPlayGround (PlayGround);
    }
    return 0;
}

bool emptyElements(int PlayGround[Xdim][Ydim], struct FreeCells *FreeCells)
{
    FreeCells->numFree=0;
    char val = SPACE;
    for (int irow=0; irow< Xdim; ++irow)
    {
        for (int icol=0; icol< Ydim ; ++icol)
        {
            if (PlayGround[irow][icol]==val)
            {
                FreeCells->X[FreeCells->numFree]=irow;
                FreeCells->Y[FreeCells->numFree]=icol;
                FreeCells->numFree++;

            }
        }
    }

    if(FreeCells->numFree==0)
    {
        return false;
    }
    return true;
}

void genFruit(int PlayGround[Xdim][Ydim], struct FreeCells FreeCells, struct FRUIT *Fruit )
{
    int lower = 0;
    int upper = FreeCells.numFree;
    int num = (rand() % (upper - lower + 1)) + lower; 

    Fruit->X=FreeCells.X[num];
    Fruit->Y=FreeCells.Y[num];

    PlayGround[Fruit->X][Fruit->Y]=FOOD;
}

void initPlayGround (int PlayGround[Xdim][Ydim])
{ // Function to reset the PlayGround

    char val = SPACE;
    int Hborder = PLAYGROUND;
    int Vborder = PLAYGROUND;

    for (int irow=0; irow< Xdim; ++irow)
    {
        for (int icol=0; icol< Ydim ; ++icol)
        {
            PlayGround[irow][icol]=val;
        }
    }
    // Create horizontal borders
    for (int icol=0; icol<Ydim; icol++)
    {
        PlayGround[0][icol]=Hborder;
        PlayGround[Xdim-1][icol]=Hborder;
    }

    // Create vertical borders
    for (int irow=1; irow<Xdim-1; irow++)
    {
        PlayGround[irow][0]=Vborder;
        PlayGround[irow][Ydim-1]=Vborder;
    }
}

bool moveSnake(int PlayGround[Xdim][Ydim], struct SNAKE *Snake, int Movement[2], bool *isFruit)
{
    PlayGround[Snake->PosX[Snake->ActualLeng-1]][Snake->PosY[Snake->ActualLeng-1]]=SPACE;
    for (int ilen=Snake->ActualLeng-1; ilen>0; ilen--)
    {
        Snake->PosX[ilen]=Snake->PosX[ilen-1];
        Snake->PosY[ilen]=Snake->PosY[ilen-1];
        PlayGround[Snake->PosX[ilen]][Snake->PosY[ilen]]=Snake->SnakeBody;
    }
    Snake->PosX[0]=Snake->PosX[0]+Movement[0];
    Snake->PosY[0]=Snake->PosY[0]+Movement[1];

    if (PlayGround[Snake->PosX[0]][Snake->PosY[0]]==SPACE || PlayGround[Snake->PosX[0]][Snake->PosY[0]]==FOOD)
    {
        if (PlayGround[Snake->PosX[0]][Snake->PosY[0]]==FOOD)
        {
            Snake->ActualLeng++;
            Snake->PosX[Snake->ActualLeng-1]=Snake->PosX[Snake->ActualLeng-2];
            Snake->PosY[Snake->ActualLeng-1]=Snake->PosY[Snake->ActualLeng-2];

            *isFruit = false;
        }
        PlayGround[Snake->PosX[0]][Snake->PosY[0]]=Snake->SnakeHead;
        return false;
    }
    
    return true;
}

void printPlayGround (int PlayGround[Xdim][Ydim])
{
    system("cls");
    for (int irow=0; irow<Xdim; irow++)
    {
        for (int icol=0; icol<Ydim; icol++)
        {
            printf("%c", PlayGround[irow][icol]);
        }
        printf("\n");
    }
}

void initSnake (struct SNAKE *Snake, int Movement[2])
{// Initialize snake position, length, graphic.

    Snake->ActualLeng=SNAKELENSTART;
    Snake->MaxLeng = SNAKELEN;
    Snake->PosX[0] = (int) Xdim/2;
    Snake->PosY[0] = (int) Ydim/2;

    Snake->SnakeHead = SNAKEHEAD;
    Snake->SnakeBody = SNAKEBODY;

    for (int ilen =1 ; ilen<Snake->ActualLeng; ilen++)
    {
        Snake->PosX[ilen]=Snake->PosX[ilen-1]-Movement[0];
        Snake->PosY[ilen]=Snake->PosY[ilen-1];
    }
}

void readCommand(int *Movement)
{
    int xMov=0;
    int yMov=0;

    if( _kbhit() )
    {     // If key is typed
    int key;
    key = _getch(); // Key variable get the ASCII code from _getch()
   
    switch( key )
        {
            case ARROW_UP:
                xMov=-1;
                yMov=0;
                if (Movement[0]!=-xMov && Movement[1]!=-yMov)
                {
                    Movement[0]=xMov;
                    Movement[1]=yMov;   
                }
                break;
            case ARROW_DOWN:
                xMov=1;
                yMov=0;
                if (Movement[0]!=-xMov && Movement[1]!=-yMov)
                {
                    Movement[0]=xMov;
                    Movement[1]=yMov;   
                }
                break;
            case ARROW_LEFT:
                // counter-clockwise rotation
                xMov=0;
                yMov=-1;
                if (Movement[0]!=-xMov && Movement[1]!=-yMov)
                {
                    Movement[0]=xMov;
                    Movement[1]=yMov;   
                }
                break;
            case ARROW_RIGHT:
                xMov=0;
                yMov=1;
                if (Movement[0]!=-xMov && Movement[1]!=-yMov)
                {
                    Movement[0]=xMov;
                    Movement[1]=yMov;   
                }
                break;
            default:
                //code her...
                break;
        }
    }
}
