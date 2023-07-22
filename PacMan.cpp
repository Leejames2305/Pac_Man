#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

int main_menu(); // main menu function
void runGame();  // function to handle game logic and display

int main()
{
    system("cls");
	int main_menu_selection = 0; // record main menu selection

    main_menu_selection = main_menu(); // call main menu function

    if (main_menu_selection == 1) // if main menu selection is 1, start game
    {
        runGame(); // call the function to run the game
    }
    else if (main_menu_selection == 2) // if main menu selection is 2, level editor
    {
        cout << "Level editor" << endl;
    }
	else
	{
		return main();
	}

    return 0;
}

int main_menu()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;                 // Next 4 lines set cursor visibility to FALSE
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = true;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
	
	
	cout << "PacMan" << endl;
    cout << "Press 1 to start" << endl;
    cout << "Press 2 for level editor" << endl;
    cout << "Press 3 to exit" << endl;

    unsigned char CharInput;
	cout << "Enter your choice: ";
    cin >> CharInput;

    if (CharInput == 49) // 1 key
    {
        return 1;
    }
    else if (CharInput == 50) // 2 key
    {
        return 2;
    }
    else if (CharInput == 51) // 3 key
    {
        exit(0);
    }
    else
    {
        cout << "Invalid input, please try again" << endl;
        return main_menu(); // Return the value returned by the recursive call.
    }
}

void runGame()
{
    system("cls");                  // clear console screen, start from empty
    int x = 0, y = 0;               // record player's position
    int direction = 0;              // record player's direction; 1 up, 2 left, 3 right, 4 down

    for (;;) // infinite loop
    {
        if (_kbhit()) // Check key stroke
        {
            unsigned char CharInput = _getch(); // arrow key is a 2-key combination, 244-72 for "up", this get first key
            if (CharInput == 224)               // confirm it is a 2-key combination
            {
                CharInput = _getch(); // get second key
                switch (CharInput)
                {
                case 72:
                    if (y > 0) y--; direction = 1; break; // up
                case 75:
                    if (x > 0) x--; direction = 2; break; // left
                case 77:
                    if (x < 39) x++; direction = 3; break; // right
                case 80:
                    if (y < 19) y++; direction = 4; break; // down
                }
            }
            else if (CharInput == 27) // esc key quit, other no respond
            {
            	main();
            }
        }
        else
        {
            direction = 0;
        }

        // Display routine
        COORD coord = {0, 0};
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleCursorPosition(consoleHandle, coord); // Set cursor position to (0,0)
        CONSOLE_CURSOR_INFO cursorInfo;                 // Next 4 lines set cursor visibility to FALSE
        GetConsoleCursorInfo(consoleHandle, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &cursorInfo);

        for (int row = 0; row < 20; row++) // draw 20x40 screen
        {
            for (int column = 0; column < 40; column++)
            {
                if (row == y && column == x) // Print player when position matched
                {
                    switch (direction) // Print player direction, 0 up, 1 left, 2 right, 3 down
                    {
                    case 0:
                        cout << "X";
                        break;
                    case 1:
                        cout << "^";
                        break;
                    case 2:
                        cout << "<";
                        break;
                    case 3:
                        cout << ">";
                        break;
                    case 4:
                        cout << "v";
                        break;
                    }
                }
                else
                {
                    cout << " "; // Print space in non-player position
                }
            }
            cout << endl;
        }

        // Speed control
        Sleep(50);
    }
}