//Snake Game by Luke Balmer 2/20/21
#include <iostream>
#include <cstdlib>
#include <Windows.h> //system functions like "cls"
#include <conio.h> //input functions, _kbhit() and _getch()
#include <fstream> //used to read/write highscores to highscores.txt
#include <string>
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

int main()//calls setup, then loops through draw, input, and logic, then displays game over screen and reads/writes to highscores file if applicable
{
	setup();
	while(!gameOver && gameStart == true)
	{
		draw();
		if (dir == UP || dir == DOWN)//matches vertical speed to horizontal speed, because the spaces between lines(vertical) are greater than between side-by-side characters(horizontal)
		{
			Sleep(25);
		}
		
		input();
		logic();
	}
	system("cls");
	Sleep(100);//want to slightly pause after game end to allow player to stop pressing button, which could end program before they can read screen or input initials
	cout << "Game Over!" << endl;
	cout << "Your score was: " << score << endl;
	
	if(deathWallMode == true)//only reads and writes to snakehighscores.txt if player is using reg. ruleset, see line 102
	{
		int highScore;
		string bestPlayer;
		ifstream infile ("snakehighscores.txt");
		if(infile.is_open())
		{
			infile >> highScore;
			infile >> bestPlayer;
		}
		
		infile.close();
		cout << "High Score: " << highScore << " by " << bestPlayer << endl;
		
		if(score > highScore)
		{
			cout << "Congrats! You beat the high score by " << score - highScore << " points!" << endl;
			cout << "Please record your initials(ex: LB)(do not use spaces): " << endl;
			cin >> bestPlayer;
			highScore = score;
			cout << "New High Score: " << highScore << " by " << bestPlayer << endl;
			ofstream outfile ("snakehighscores.txt");
			outfile << highScore << endl << bestPlayer << endl;
			outfile.close();
		}
	}
	
	if(gameStart == false)
		cout << "Please restart and ensure you properly enter 1 to start the game." << endl;
	
	return 0;
}

void setup()
{
	gameOver = false;
	cout << "Welcome to Snake! Use WASD to move and K to end game." << endl;
//each of the backslashes in the ascii art below has to double up because of the backslash's use in strings(fun fact :) )
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
	cout << "If you would like to play with regular rules enter 1, or to play Pac-Man style, enter 0: " << endl;
	cin >> deathWallMode;
	if(deathWallMode == true)
		cout << "Regular snake ruleset confirmed." << endl;
	else
		cout << "Pac-Man mode confirmed. High scores are unavailable in this mode." << endl;//do not want highscores inflated by players on easier rules
	cout << "Enter 1 to start game: " << endl;
	cin >> gameStart;
	
	dir = STOP;
	x = 3;//two starting coords for snake head, can be changed to wherever in the matrix
	y = 2;
	fruitX = rand() % width;//fruit spawn
	fruitY = rand() % height;
	score = 0;
}

void draw()//outputs boundaries, fruit, and snake head/tail to console
{
	system("cls"); //clears screen of previous text
	
	for(int i = 0; i < width + 2; i++)//top on-screen boundary
		cout << "#";
	cout << endl;
	
	for(int i = 0; i < height; i++)//tells how many rows to draw 
	{
		for(int j = 0; j < width + 1; j++)//determines what each char in a row should be
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

void input()//this function looks for new input and gives any updates to logic() through dir enumerator, which allows logic to execute over and over based on that direction
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

void logic()//ensures tail follows head, executes changes to direction, provides collision detection for both tail and wall, and respawns fruit
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
		if(x > width - 1 || x < 0 || y > height || y < 0)//wall collision detection: if coords exceed matrix boundaries game over
			gameOver = true;
	}
	else//but, if player selects pac-man style,
	{//code below will allow snake to survive going out of bounds, and appear on the other side pacman style.
		if(x >= width - 1)
			x = 0; 
		else if(x < 0)
			x = width - 1;
		if(y >= height)
			y = 0; 
		else if(y < 0)
			y = height - 1;  
	}
	
	for(int i = 0; i < nTail; i++)//collision detection: iterates through tail
	{
		if(tailX[i] == x && tailY[i] == y)//if tail coords match with head coords
			gameOver = true;//end game
	}
	
	if(x == fruitX && y == fruitY)//detects fruit being eaten, ups score, and respawns fruit elsewhere
	{
		score += 100;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}
}
