#define Ydim 25
#define Xdim 25
#define SPACE 32
#define TERMINATOR 10
#define FOOD 184
#define SNAKELEN 100
#define SNAKELENSTART 3
#define SNAKEHEAD 220
#define SNAKEBODY 220
#define PLAYGROUND 43

struct SNAKE
{
    int ActualLeng;
    int PosX[SNAKELEN];
    int PosY[SNAKELEN];
    int MaxLeng;
    int SnakeHead;
    int SnakeBody;
};

struct FreeCells
{
    int X[Xdim*Ydim];
    int Y[Xdim*Ydim];
    int numFree;
};

struct FRUIT
{
    int X;
    int Y;
};

// define arrow key
#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77

void seedRandom(void);
int getRandomNumber(int, int);
bool emptyElements(int [Xdim][Ydim], struct FreeCells *);
void genFruit(int [Xdim][Ydim], struct FreeCells, struct FRUIT *);
void initSnake (struct SNAKE *, int[2]);
void initPlayGround (int [Xdim][Ydim]);
void printPlayGround (int [Xdim][Ydim]);
bool moveSnake(int [Xdim][Ydim], struct SNAKE*, int [2], bool*);
void readCommand(int *);
void clearConsole(void);
void ResetConsole(void);
bool askRetry();