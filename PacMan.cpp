#include <iostream>		//for cout,cin,endl
#include <conio.h>		//for _getch,_kbhit
#include <windows.h>	//for Sleep
using namespace std;

int main()
{
    system("cls"); 		//clear console screen, start from empty
	int x = 0, y = 0; 	//record player's position
	for (;;)  			//infinite loop
	{
		if (_kbhit()) 	//Check key stroke
		{
			unsigned char CharInput = _getch(); //arrow key is a 2-key combination, 244-72 for "up", this get first key
			if (CharInput == 224) 				//confirm it is a 2-key combination
			{
				CharInput = _getch(); 			//get second key
				switch (CharInput)
				{
				case 72: if (y > 0) y--; break; 	//up
				case 75: if (x > 0) x--; break; 	//left
				case 77: if (x < 39) x++; break; 	//right
				case 80: if (y < 19) y++; break;	//down
				}
			}
			else if (CharInput == 27) //esc key quit, other no respond
			{
				return 0;
			}
		}

		//Display routine
		COORD coord = { 0, 0 };
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleCursorPosition(consoleHandle, coord);			//Set cursor position to (0,0)
		CONSOLE_CURSOR_INFO cursorInfo;							//Next 4 lines set cursor visibility to FALSE
		GetConsoleCursorInfo(consoleHandle, &cursorInfo);
		cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(consoleHandle, &cursorInfo);

		for (int row = 0; row < 20; row++) //draw 20x40 screen
		{
			for (int column = 0; column < 40; column++) 
			{
				if (row == y && column == x) //Print player position when matched
					cout << 'X';
				else
					cout << " ";  //Print space in non player position 
			}
			cout << endl;
		}
		

		//Speed control
		Sleep(50);
	}
}
