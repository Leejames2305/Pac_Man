#include <iostream>		//for cout,cin,endl
#include <conio.h>		//for _getch,_kbhit
#include <windows.h>	//for Sleep
using namespace std;

int main()
{
    system("cls"); 		//clear console screen, start from empty

	int x = 0, y = 0; 	//record player's position

	for (;;)
	{
		//Check key stroke
		if (_kbhit())
		{
			//arrow key is a 2-key combination, 244-72 for "up"
			unsigned char c = _getch(); //get first key
			if (c == 224)
			{
				c = _getch(); //get second key
				switch (c)
				{
				case 72: if (y > 0) y--; break; //up
				case 75: if (x > 0) x--; break; //left
				case 77: if (x < 39) x++; break; //right
				case 80: if (y < 19) y++;        //down
				}
			}
			else if (c == 27) //esc key
			{
				return 0;
			}
		}

		//Display routine
		COORD coord = { 0, 0 };
		SetConsoleCursorPosition( //reset cursor at top-left corner
			GetStdHandle(STD_OUTPUT_HANDLE), coord);

		for (int r = 0; r < 20; r++)
		{
			for (int c = 0; c < 40; c++)
			{
				if (r == y && c == x)
					cout << "X";
				else
					cout << " ";
			}
			cout << endl;
		}

		//Speed control
		Sleep(100);
	}
}
