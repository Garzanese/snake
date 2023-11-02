#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <fcntl.h>
#include <windows.h>

#include "snake.h"

int main()
{
    struct SNAKE Snake;
    struct FreeCells FreeCells;
    struct FRUIT Fruit;

    int PlayGround [Xdim][Ydim];

    //double t1=0;
    //double t2=0;
    //double deltaT=0;

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
        //t1 = get_time();
        printPlayGround (PlayGround);
        //t2 = get_time();
        //deltaT=t2-t1;
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
{   // Function to initialize the PlayGround

    for (int irow=0; irow<Xdim; ++irow)
    {   // Initialize all element to SPACE
        for (int icol=0; icol<= Ydim ; ++icol)
        {
            PlayGround[irow][icol]=SPACE;
        }
    }

    for (int icol=0; icol<Ydim-1; icol++)
    {   // Create horizontal borders
        PlayGround[0][icol]     =PLAYGROUND;
        PlayGround[Xdim-1][icol]=PLAYGROUND;
    }

    for (int irow=0; irow<Xdim-1; irow++)
    {    // Create vertical borders + string terminator
        PlayGround[irow][0]     =PLAYGROUND;
        PlayGround[irow][Ydim-2]=PLAYGROUND;
        PlayGround[irow][Ydim-1]=TERMINATOR;
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
{ // Print
    system("cls");
    fwrite(PlayGround, sizeof(PlayGround[0]),Xdim,stdout);
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

double get_time()
{
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&f);
    return (double)t.QuadPart/(double)f.QuadPart;
}