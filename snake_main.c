#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#include "snake.h"


int main()
{
    struct SNAKE Snake;
    struct FreeCells FreeCells;
    struct FRUIT Fruit;

    int PlayGround [Xdim][Ydim];

    int Movement[2];
    bool collision;
    bool isFruit;
    bool isEmpty;
    bool isContinue = true;

    int msec        = 0;
    int trigger     = 20; /* ms */
    int GameRefresh = 0;
    time_t difference_t, before_t;

    seedRandom();

    while (isContinue)
    {
        Movement[0] = -1;
        Movement[1] = 0;
        collision   = false;
        isFruit     = false;
        isEmpty     = true;

        initSnake (&Snake, Movement);
        initPlayGround  (PlayGround);
        printPlayGround (PlayGround, Snake, GameRefresh);

        before_t=clock();

        while(!collision && isEmpty)
        {
            GameRefresh = trigger*SNAKELEN/Snake.ActualLeng;
            if (!isFruit)
            {
                isEmpty= emptyElements(PlayGround, &FreeCells);
                genFruit(PlayGround, FreeCells, &Fruit);
                isFruit=true;
            }
            difference_t = clock()- before_t;
            msec = difference_t * 1000 / CLOCKS_PER_SEC;

            if ((msec>GameRefresh))
            {
                readCommand(Movement);
                collision=moveSnake(PlayGround,&Snake, Movement, &isFruit);
                printPlayGround (PlayGround, Snake, GameRefresh);
                before_t=clock();
            }
        }

        isContinue=askRetry();
        clearConsole();
    }
    return 0;
}

void seedRandom()
{
    srand((unsigned)time(NULL));
}

int getRandomNumber(int min, int max)
{
    return min + rand() % (max - min + 1);
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
    int num = getRandomNumber(lower, upper); 

    Fruit->X=FreeCells.X[num];
    Fruit->Y=FreeCells.Y[num];

    PlayGround[Fruit->X][Fruit->Y]=FOOD;
}

void initPlayGround (int PlayGround[Xdim][Ydim])
{   // Function to initialize the PlayGround

    for (int irow=1; irow<Xdim-1; irow++)
    {   // Initialize all element to SPACE
        for (int icol=1; icol< Ydim-1 ; icol++)
        {
            PlayGround[irow][icol]=SPACE;
        }
    }

    for (int icol=0; icol<Ydim; icol++)
    {   // Create horizontal borders
        PlayGround[0][icol]     =PLAYGROUND;
        PlayGround[Xdim-1][icol]=PLAYGROUND;
    }

    for (int irow=0; irow<Xdim-1; irow++)
    {    // Create vertical borders + string terminator
        PlayGround[irow][0]     =PLAYGROUND;
        PlayGround[irow][Ydim-1]=PLAYGROUND;
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

void printPlayGround (int PlayGround[Xdim][Ydim], struct SNAKE Snake, int GameRefresh)
{ // Print
    ResetConsole();
    for (int irow = 0; irow < Xdim; irow++) 
    {
        fwrite(PlayGround[irow], sizeof(int), Ydim, stdout);
        putchar('\n'); // Print a newline character to move to the next row
    }
    
    putchar('\n');
    putchar('\n');
    printf("Total Score: %d \n" , Snake.ActualLeng-SNAKELENSTART);
    printf("Refresh Rate: %d \n" , GameRefresh);

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
    key = _getch(); // The _getch and_getwch functions read a single character from the console without echoing the character. None of these functions can be used to read CTRL+C. 
                    // When reading a function key or an arrow key, each function must be called twice;
                    // the first call returns 0 or 0xE0, and the second call returns the actual key code.
    switch( key )
        {
            case ARROW_UP:
                xMov=-1;
                yMov=0;
                break;
            case ARROW_DOWN:
                xMov=1;
                yMov=0;
                break;
            case ARROW_LEFT:
                // counter-clockwise rotation
                xMov=0;
                yMov=-1;
                break;
            case ARROW_RIGHT:
                xMov=0;
                yMov=1;
                break;
            default:
                //code her...
                break;
        }
        if (Movement[0]!=-xMov && Movement[1]!=-yMov)
        {
            Movement[0]=xMov;
            Movement[1]=yMov;   
        }
    }
}

void clearConsole()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = (DWORD) csbi.dwSize.X * (DWORD) csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
}

void ResetConsole(void)
{// by Default (GPT 3.5)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0};

    // Set the cursor position to the top-left corner (first line)
    SetConsoleCursorPosition(hConsole, coordScreen);
}

bool askRetry()
{

    bool isOK 	= true;
	bool out 	= true;

    putchar('\n');
    printf("You dead :(");
    putchar('\n');

	while (isOK)
	{
		char dummy;
		printf("Do you want to play another game?");
		printf("\n");
		printf("y - Yes, I want to start again");
		printf("\n");
		printf("n - No, I prefer to go work");
		printf("\n");
		printf("%c",SPACE);
		printf("\n");
		
		fflush(stdin);
		scanf( "%c", &dummy);
		

		if (dummy=='y' || dummy=='n')
		{
			isOK=false;

			if(dummy=='n')
			{

				out = false;
			}
		}
		else
		{
			printf("The chosen symbol was not corret.");
			printf("\n");
		}
	}

    // For Armando: he likes press any key to continue
    printf("Press Any Key to Continue\n");
    fflush(stdin);
	getchar();
	return out;
}