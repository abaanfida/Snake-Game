#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <tchar.h>
using namespace std;
enum direction { UP, DOWN, LEFT, RIGHT };
struct position
{
	int ri;
	int ci;
};
struct snake
{
	position* ps;
	int size;
	int color;
	char sym;
	int score;
	direction dir;
	int uk;
	int dk;
	int lk;
	int rk;
	bool alive;
};
struct food
{
	position fp;
	int score;
	char sym;
};
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
bool is_touching_boundary(position p, int height, int width)
{
	for (int i = 0; i < height; i++)
	{
		if (p.ri == i && p.ci == 0 || p.ri == i && p.ci == width)
		{
			return true;
		}
	}
	for (int i = 0; i < width; i++)
	{
		if (p.ri == 0 && p.ci == i || p.ri == height && p.ci == i)
		{
			return true;
		}
	}
	return false;
}
bool is_legal_coordinate(position p, snake s)
{
	for (int i = 0; i < s.size; i++)
	{
		if (s.ps[i].ri == p.ri && s.ps[i].ci == p.ci)
		{
			return false;
		}
	}
	if (is_touching_boundary(p, 50, 50))
	{
		return false;
	}
	return true;
}
void generate_food(food& f, snake& s1, snake& s2, snake& s3, snake& s4, int height, int width)
{
	do
	{
		f.fp.ri = rand() % (height - 2);
		f.fp.ci = rand() % (width - 2);
	} while (!is_legal_coordinate(f.fp, s1) || !is_legal_coordinate(f.fp, s2) || !is_legal_coordinate(f.fp, s3) || !is_legal_coordinate(f.fp, s4));
	f.score = 1;
}
void init(snake& s1, snake& s2, snake& s3, snake& s4, food& f, int height, int width)
{
	s1.ps = new position[3];
	s1.ps[0].ri = height / 2;
	s1.ps[0].ci = width / 2;
	s1.ps[1].ri = height / 2;
	s1.ps[1].ci = (width / 2) - 1;
	s1.ps[2].ri = height / 2;
	s1.ps[2].ci = (width / 2) - 2;

	s2.ps = new position[3];
	s2.ps[0].ri = (height / 2) - 10;
	s2.ps[0].ci = width / 2;
	s2.ps[1].ri = (height / 2) - 10;
	s2.ps[1].ci = (width / 2) - 1;
	s2.ps[2].ri = (height / 2) - 10;
	s2.ps[2].ci = (width / 2) - 2;

	s3.ps = new position[3];
	s3.ps[0].ri = (height / 2) + 10;
	s3.ps[0].ci = width / 2;
	s3.ps[1].ri = (height / 2) + 10;
	s3.ps[1].ci = (width / 2) - 1;
	s3.ps[2].ri = (height / 2) + 10;
	s3.ps[2].ci = (width / 2) - 2;

	s4.ps = new position[3];
	s4.ps[0].ri = (height / 2) + 20;
	s4.ps[0].ci = width / 2;
	s4.ps[1].ri = (height / 2) + 20;
	s4.ps[1].ci = (width / 2) - 1;
	s4.ps[2].ri = (height / 2) + 20;
	s4.ps[2].ci = (width / 2) - 2;

	s1.size = 3;
	s1.score = 0;
	s1.sym = -37;
	s1.dir = RIGHT;
	s1.color = 5;

	s2.size = 3;
	s2.score = 0;
	s2.sym = -37;
	s2.dir = RIGHT;
	s2.color = 2;

	s3.size = 3;
	s3.score = 0;
	s3.sym = -37;
	s3.dir = RIGHT;
	s3.color = 6;

	s4.size = 3;
	s4.score = 0;
	s4.sym = -37;
	s4.dir = RIGHT;
	s4.color = 12;

	s1.uk = 72;
	s1.dk = 80;
	s1.lk = 75;
	s1.rk = 77;

	s2.uk = 119;
	s2.dk = 115;
	s2.lk = 97;
	s2.rk = 100;

	s3.uk = 105;
	s3.dk = 107;
	s3.lk = 106;
	s3.rk = 108;

	s4.uk = 116;
	s4.dk = 103;
	s4.lk = 102;
	s4.rk = 104;

	s1.alive = true;
	s2.alive = true;
	s3.alive = true;
	s4.alive = true;

	generate_food(f, s1, s2, s3, s4, height, width);
	f.sym = -37;
}
void display(const snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		SetClr(s.color);
		gotoRowCol(s.ps[i].ri, s.ps[i].ci);
		cout << s.sym;
		SetClr(15);
	}
}
void box(int r1, int c1, int r2, int c2)
{
	SetClr(10);
	for (int r = r1; r <= r2; r++)
	{
		for (int c = c1; c <= c2; c++)
		{
			gotoRowCol(r, c1);
			cout << char(-37);
			gotoRowCol(r, c2);
			cout << char(-37);
			gotoRowCol(r1, c);
			cout << char(-37);
			gotoRowCol(r2, c);
			cout << char(-37);

		}
	}
	SetClr(15);
}
void erase(const snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		gotoRowCol(s.ps[i].ri, s.ps[i].ci);
		cout << ' ';
	}
}
void change_direction(snake& s, char ch)
{
	if (ch == s.lk)
	{
		if (s.dir != RIGHT)
		{
			s.dir = LEFT;
		}
	}
	if (ch == s.rk)
	{
		if (s.dir != LEFT)
		{
			s.dir = RIGHT;
		}
	}
	if (ch == s.uk)
	{
		if (s.dir != DOWN)
		{
			s.dir = UP;
		}
	}
	if (ch == s.dk)
	{
		if (s.dir != UP)
		{
			s.dir = DOWN;
		}
	}

}
void move_snake(snake& s, int height, int width)
{
	for (int i = s.size - 1; i - 1 >= 0; i--)
	{
		s.ps[i] = s.ps[i - 1];
	}
	switch (s.dir)
	{
	case UP:
		s.ps[0].ri--;
		if (s.ps[0].ri == 0)
			s.ps[0].ri = height - 2;
		break;
	case DOWN:
		s.ps[0].ri++;
		if (s.ps[0].ri == height - 1)
			s.ps[0].ri = 1;
		break;
	case LEFT:
		s.ps[0].ci--;
		if (s.ps[0].ci == 0)
			s.ps[0].ci = width - 2;
		break;
	case RIGHT:
		s.ps[0].ci++;
		if (s.ps[0].ci == width - 1)
			s.ps[0].ci = 1;
		break;
	}
}
void move_snake_boundary(snake& s, int height, int width)
{
	for (int i = s.size - 1; i - 1 >= 0; i--)
	{
		s.ps[i] = s.ps[i - 1];
	}
	switch (s.dir)
	{
	case UP:
		s.ps[0].ri--;
		break;
	case DOWN:
		s.ps[0].ri++;
		break;
	case LEFT:
		s.ps[0].ci--;
		break;
	case RIGHT:
		s.ps[0].ci++;
		break;
	}
}
void display_food(food f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	cout << f.sym;
}
bool food_eaten(food f, snake s)
{
	return s.ps[0].ri == f.fp.ri && s.ps[0].ci == f.fp.ci;
}
void expand_snake(snake& s)
{
	position* hp = new position[s.size + 1];
	for (int i = 0; i < s.size; i++)
	{
		hp[i] = s.ps[i];
	}
	hp[s.size] = s.ps[s.size - 1];
	s.size++;
	delete[]s.ps;
	s.ps = hp;
}
void controls(string pn[])
{
	SetClr(5);
	gotoRowCol(10, 110);
	cout << pn[0];
	SetClr(15);
	gotoRowCol(11, 110);
	cout << "UP:" << "UP ARROW";
	gotoRowCol(12, 110);
	cout << "LEFT:" << "LEFT ARROW";
	gotoRowCol(13, 110);
	cout << "DOWN:" << "DOWN ARROW";
	gotoRowCol(14, 110);
	cout << "RIGHT:" << "RIGHT ARROW";

	SetClr(2);
	gotoRowCol(20, 110);
	cout << pn[1];
	SetClr(15);
	gotoRowCol(21, 110);
	cout << "UP:" << "W";
	gotoRowCol(22, 110);
	cout << "LEFT:" << "A";
	gotoRowCol(23, 110);
	cout << "DOWN:" << "S";
	gotoRowCol(24, 110);
	cout << "RIGHT:" << "D";

	SetClr(6);
	gotoRowCol(30, 110);
	cout << pn[2];
	SetClr(15);
	gotoRowCol(31, 110);
	cout << "UP:" << "I";
	gotoRowCol(32, 110);
	cout << "LEFT:" << "J";
	gotoRowCol(33, 110);
	cout << "DOWN:" << "K";
	gotoRowCol(34, 110);
	cout << "RIGHT:" << "L";

	SetClr(12);
	gotoRowCol(40, 110);
	cout << pn[3];
	SetClr(15);
	gotoRowCol(41, 110);
	cout << "UP:" << "T";
	gotoRowCol(42, 110);
	cout << "LEFT:" << "F";
	gotoRowCol(43, 110);
	cout << "DOWN:" << "G";
	gotoRowCol(44, 110);
	cout << "RIGHT:" << "H";
}
void names(string pn[])
{
	cout << "Player 1 Name:" << endl;
	cin >> pn[0];
	cout << "Player 2 Name:" << endl;
	cin >> pn[1];
	cout << "Player 3 Name:" << endl;
	cin >> pn[2];
	cout << "Player 4 Name:" << endl;
	cin >> pn[3];
}

int main()
{
	//CONSOLE RECOMMENDED SETTINGS:
	//HEIGHT=83
	//WIDTH=235
	//FONT:RASTER(8x12)


	//INCLUDE Winmm.lib in Project through: Properties->Linker->Additional Dependencies->Edit

	int gamemode;
	string pn[4];
	names(pn);
	cout << "CHOOSE GAME MODE" << endl;
	cout << "1 FOR NO BOUNDARY" << endl;
	cout << "2 FOR BOUNDARIES" << endl;
	do
	{
		cin >> gamemode;
	} while (gamemode != 1 && gamemode != 2);
	system("CLS");
	srand(time(0));
	int height = 50;
	int width = 100;
	controls(pn);
	Sleep(50);
	SetClr(10);
	box(0, 0, 50, 100);
	SetClr(15);
	snake s1;
	snake s2;
	snake s3;
	snake s4;

	food f;
	char ch;

	init(s1, s2, s3, s4, f, height, width);
	display_food(f);
	//gamemode 1 = no boundaries
	//gamemode 2 = boundaries
	if (gamemode == 1)
	{
		while (true)
		{
			if (_kbhit())
			{
				ch = _getch();
				if (ch == -32)
				{
					ch = _getch();
					change_direction(s1, ch);
				}
				else if (ch == s2.uk || ch == s2.dk || ch == s2.lk || ch == s2.rk)
				{
					change_direction(s2, ch);
				}
				else if (ch == s3.uk || ch == s3.dk || ch == s3.lk || ch == s3.rk)
				{
					change_direction(s3, ch);
				}
				else if (ch == s4.uk || ch == s4.dk || ch == s4.lk || ch == s4.rk)
				{
					change_direction(s4, ch);
				}
			}
			display(s1);
			display(s2);
			display(s3);
			display(s4);
			Sleep(30);
			erase(s1);
			erase(s2);
			erase(s3);
			erase(s4);
			move_snake(s1, height, width);
			move_snake(s2, height, width);
			move_snake(s3, height, width);
			move_snake(s4, height, width);
			if (food_eaten(f, s1) == true || food_eaten(f, s2) == true || food_eaten(f, s3) == true || food_eaten(f, s4) == true)
			{
				PlaySound(_T("Food.wav"), NULL, SND_ASYNC);
				if (food_eaten(f, s1) == true)
				{
					expand_snake(s1);
					s1.score += f.score;
				}
				else if (food_eaten(f, s2) == true)
				{
					expand_snake(s2);
					s2.score += f.score;
				}
				else if (food_eaten(f, s3) == true)
				{
					expand_snake(s3);
					s3.score += f.score;
				}
				else if (food_eaten(f, s4) == true)
				{
					expand_snake(s4);
					s4.score += f.score;
				}
				generate_food(f, s1, s2, s3, s4, height, width);
				display_food(f);
			}
			gotoRowCol(15, 110);
			cout << "SCORE:" << s1.score;
			gotoRowCol(25, 110);
			cout << "SCORE:" << s2.score;
			gotoRowCol(35, 110);
			cout << "SCORE:" << s3.score;
			gotoRowCol(45, 110);
			cout << "SCORE:" << s4.score;

		}

	}
	else if (gamemode == 2)
	{
		while (true)
		{

			if (_kbhit())
			{
				ch = _getch();
				if (ch == -32)
				{
					ch = _getch();
					change_direction(s1, ch);
				}
				else if (ch == s2.uk || ch == s2.dk || ch == s2.lk || ch == s2.rk)
				{
					change_direction(s2, ch);
				}
				else if (ch == s3.uk || ch == s3.dk || ch == s3.lk || ch == s3.rk)
				{
					change_direction(s3, ch);
				}
				else if (ch == s4.uk || ch == s4.dk || ch == s4.lk || ch == s4.rk)
				{
					change_direction(s4, ch);
				}
			}

			if (s1.alive == true)display(s1);
			if (s2.alive == true)display(s2);
			if (s3.alive == true)display(s3);
			if (s4.alive == true)display(s4);

			Sleep(50);
			if (s1.alive == true)erase(s1);
			if (s2.alive == true)erase(s2);
			if (s3.alive == true)erase(s3);
			if (s4.alive == true)erase(s4);
			if (s1.alive == true)move_snake_boundary(s1, height, width);
			if (s2.alive == true)move_snake_boundary(s2, height, width);
			if (s3.alive == true)move_snake_boundary(s3, height, width);
			if (s4.alive == true)move_snake_boundary(s4, height, width);
			if (is_touching_boundary(s1.ps[0], height, width) || is_touching_boundary(s2.ps[0], height, width) || is_touching_boundary(s3.ps[0], height, width) || is_touching_boundary(s4.ps[0], height, width))
			{
				if (is_touching_boundary(s1.ps[0], height, width)) s1.alive = false;
				if (is_touching_boundary(s2.ps[0], height, width)) s2.alive = false;
				if (is_touching_boundary(s3.ps[0], height, width)) s3.alive = false;
				if (is_touching_boundary(s4.ps[0], height, width)) s4.alive = false;
			}
			if (food_eaten(f, s1) || food_eaten(f, s2) || food_eaten(f, s3))
			{
				PlaySound(_T("bscs22004-Food.wav"), NULL, SND_ASYNC);
				if (food_eaten(f, s1))
				{
					expand_snake(s1);
					s1.score += f.score;
				}
				else if (food_eaten(f, s2))
				{
					s2.score += f.score;
					expand_snake(s2);
				}
				else if (food_eaten(f, s3))
				{
					expand_snake(s3);
					s3.score += f.score;
				}
				else if (food_eaten(f, s4))
				{
					expand_snake(s4);
					s4.score += f.score;
				}
				generate_food(f, s1, s2, s3, s4, height, width);
				display_food(f);
			}
			gotoRowCol(15, 110);
			if (s1.alive == true)
			{
				cout << "SCORE:" << s1.score;
			}
			else
			{
				cout << "DEAD LOL";
			}
			gotoRowCol(25, 110);
			if (s2.alive == true)
			{
				cout << "SCORE:" << s2.score;
			}
			else
			{
				cout << "DEAD LOL";
			}
			gotoRowCol(35, 110);
			if (s3.alive == true)
			{
				cout << "SCORE:" << s3.score;
			}
			else
			{
				cout << "DEAD LOL";
			}
			gotoRowCol(45, 110);
			if (s4.alive == true)
			{
				cout << "SCORE:" << s4.score;
			}
			else
			{
				cout << "DEAD LOL";
			}

			if ((s1.alive == true && s2.alive == false && s3.alive == false && s4.alive == false))
			{
				gotoRowCol(3, 110);
				cout << "Game Over... " << pn[0] << " Wins" << endl;
				return 0;
			}
			if ((s2.alive == true && s1.alive == false && s3.alive == false && s4.alive == false))
			{
				gotoRowCol(4, 110);
				cout << "Game Over... " << pn[1] << " Wins" << endl;
				return 0;
			}
			if ((s3.alive == true && s2.alive == false && s1.alive == false && s4.alive == false))
			{
				gotoRowCol(5, 110);
				cout << "Game Over... " << pn[2] << " Wins" << endl;
				return 0;
			}
			if ((s4.alive == true && s2.alive == false && s3.alive == false && s1.alive == false))
			{
				gotoRowCol(6, 110);
				cout << "Game Over... " << pn[3] << " Wins" << endl;
				return 0;
			}


		}
	}



}
