#define Ydim 60
#define Xdim 30
#define SPACE 32
#define SnakeLen 1

struct SNAKE
{
    int ActualLeng;
    int PosX[SnakeLen];
    int PosY[SnakeLen];
    int MaxLeng;
    char SnakeHead;
    char SnakeBody;
};

void initSnake (struct SNAKE *);
void resetPlayGround (char [Xdim][Ydim]);
void printPlayGround (char [Xdim][Ydim]);
void moveSnake(char [Xdim][Ydim], struct SNAKE);
void readCommand(void);
