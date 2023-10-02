#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <fcntl.h>

#include "snake.h"

int main()
{
    struct SNAKE Snake;
    char PlayGround [Xdim][Ydim];

    initSnake (&Snake);
    resetPlayGround (PlayGround);
    printPlayGround (PlayGround);

    while(1)
    {
        moveSnake(PlayGround,Snake);
        printPlayGround (PlayGround);
        readCommand();
    }

    return 0;
}

void SetupIO(void)
{
    fcntl(0, F_SETFL, FNDELAY);
}

void ShutDownIO(void)
{
    fcntl(0, F_SETFL, 0);
}

void resetPlayGround (char PlayGround[Xdim][Ydim])
{ // Function to reset the PlayGround

    char val = SPACE;
    char Hborder = '#';
    char Vborder = '#';

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

void moveSnake(char PlayGround[Xdim][Ydim], struct SNAKE Snake)
{
    for (int iSnake=0; iSnake<Snake.ActualLeng; iSnake++)
    {
        if (iSnake<1)
        {
            PlayGround[Snake.PosX[iSnake]][Snake.PosY[iSnake]]=Snake.SnakeHead;
        }
        else
        {
            PlayGround[Snake.PosX[iSnake]][Snake.PosY[iSnake]]=Snake.SnakeBody;
        }
    }
}

void printPlayGround (char PlayGround[Xdim][Ydim])
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

void initSnake (struct SNAKE *Snake)
{// Initialize snake position, length, graphic.

    Snake->ActualLeng=1;
    Snake->MaxLeng = SnakeLen;
    Snake->PosX[0] = (int) Xdim/2;
    Snake->PosY[0] = (int) Ydim/2;
    Snake->SnakeHead = '0';
    Snake->SnakeBody = 'o';
}

void readCommand(void)
{
    if (getch() == '\033') 
    { // if the first value is esc
        getch(); // skip the [
        switch(getch()) 
        { // the real value
            case 'A':
                // code for arrow up
                break;
            case 'B':
                // code for arrow down
                break;
            case 'C':
                // code for arrow right
                break;
            case 'D':
                // code for arrow left
                break;
            default:
                break;
        }
    }
}
