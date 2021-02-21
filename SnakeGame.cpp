//Snake Game by Luke Balmer 2/20/21
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <conio.h>
using namespace std;

bool gameOver, gameStart, deathWallMode;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;
void setup(), draw(), input(), logic();


void setup()
{
	gameOver = false;
	cout << "Welcome to Snake! Use WASD to move and K to end game." << endl;
	
	cout << "	   /^\\/^\\ " << endl;
	cout <<	"         _|_O|  O|" << endl;
	cout <<	"\\/     /~     \\_/ \\ " << endl;
	cout <<	" \\____|__________/  \\ " << endl;
	cout <<	"        \\_______      \\ " << endl;
	cout <<	"                `\\     \\                  \\ " << endl;
	cout <<	"                  |     |                  \\ " << endl;
	cout <<	"                 /      /                    \\ " << endl;
	cout <<	"                /     /                       \\\\ " << endl;
	cout <<	"              /      /                         \\ \\ " << endl;
	cout <<	"             /     /                            \\  \\ " << endl;
	cout <<	"           /     /             _----_            \\   \\ " << endl;
	cout <<	"          /     /           _-~      ~-_         |   |" << endl;
	cout <<	"         (      (        _-~    _--_    ~-_     _/   |" << endl;
	cout <<	"          \\      ~-____-~    _-~    ~-_    ~-_-~    /" << endl;
	cout <<	"           ~-_           _-~          ~-_       _-~" << endl;
	cout <<	"               ~--______-~                ~-___-~ " << endl;
	cout << "Would you like to die when hitting a wall(enter 1), or play Pac-Man style(enter 0)?" << endl;
	cin >> deathWallMode;
	if(deathWallMode == true)
		cout << "Death wall mode confirmed." << endl;
	else
		cout << "Pac-Man mode confirmed." << endl;
	cout << "Enter 1 to start game: " << endl;
	cin >> gameStart;
	
	
	dir = STOP;
	x = 3;
	y = 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}
void draw()
{
	system("cls");
	for(int i = 0; i < width + 2; i++)//top on-screen boundary
		cout << "#";
	cout << endl;
	
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width + 1; j++)//row
		{
			if(j == 0 || j == width)
			 	cout << "#";
			 	
			if(i == y && j == x)
				cout << "O";
			else if(i == fruitY && j == fruitX)
				cout << "F";
			else
			{
				bool print = false;
				for(int k = 0; k < nTail; k++)//iterates through tail, draws
				{
					if(tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						print = true;
					}
				}
				if (!print)//prevents right wall from bulging by not printing blank if tail has been printed
					cout << " ";
			}	
		}
		cout << endl;
	}
	
	for(int i = 0; i < width + 2; i++)//bottom on-screen boundary
		cout << "#";
	cout << endl;
	cout << "Score: " << score << endl;
}
void input()
{
	if (_kbhit())
	{
		switch(_getch())
		{
			case 'w' :
				dir = UP;
				break;
			case 's' :
				dir = DOWN;
				break;
			case 'a' :
				dir = LEFT;
				break;
			case 'd' :
				dir = RIGHT;
				break;
			case 'k' : 
				gameOver = true;
				break;
		}
	}
}
void logic()
{
	for(int i = nTail; i > 0; i--)//iterates through 2 arrays, assigning new coords based on where the segment in front just was
	{
		tailX[i] = tailX[i-1];
		tailY[i] = tailY[i-1];
	}
	tailX[0] = x;
	tailY[0] = y;
	switch(dir)
	{
		case UP :
			y--;
			break;
		case DOWN :
			y++;
			break;
		case LEFT :
			x--;
			break;
		case RIGHT :
			x++;
			break;	
	}
	
	if(deathWallMode == true)
	{	
		if(x > width - 1 || x < 0 || y > height || y < 0)
			gameOver = true;
	}
	else
	{
		if(x >= width - 1)
			x = 0; 
		else if(x < 0)
			x = width - 1;
		if(y >= height)
			y = 0; 
		else if(y < 0)
			y = height - 1;  //code to the right will allow snake to survive going out of bounds, and appear on the other side pacman style.
	}
	
	for(int i = 0; i < nTail; i++)//collision detection: iterates through tail
	{
		if(tailX[i] == x && tailY[i] == y)//if tail coords match with head coords
			gameOver = true;//end game
	}
	
	if(x == fruitX && y == fruitY)
	{
		score += 100;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}

}

int main()
{
	setup();
	while(!gameOver && gameStart == true)
	{
		draw();
		if (dir == UP || dir == DOWN)
		{
			Sleep(25);
		}
		
		input();
		logic();
	//	unsigned sleep(1000);
	}
	system("cls");
	cout << "Game Over!" << endl << "Your score was: " << score << endl;
	
	return 0;
}
