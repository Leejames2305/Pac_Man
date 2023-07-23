#include <iostream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

int main_menu(); // main menu function
void runGame();  // function to handle game logic and display
void level_editor(); // function to handle level editor
void SetFontSize();

const int ROWS = 20;    //Y-axis
const int COLS = 40;    //X-axis

// Custom struct to represent 2D coordinates
struct dots_coordinates 
{
    int dotsX;
    int dotsY;
    bool isDots;
};

dots_coordinates dots[COLS][ROWS];

int main()
{
    SetFontSize();
	int main_menu_selection = 0;

	while (main_menu_selection != 3)			// keep showing main menu
	{
		system("cls");
		main_menu_selection = main_menu(); 		// call main menu function

		if (main_menu_selection == 1) 			// if main menu selection is 1, start game
		{
			runGame(); 							// call the function to run the game
		}
		else if (main_menu_selection == 2) 		// if main menu selection is 2, level editor
		{
			level_editor(); 					// call the function to run the level editor
		}
		else if (main_menu_selection == 3) 		// if main menu selection is 3, exit
		{
			exit(0);
		}
	}

    return 0;
}

void SetFontSize()  //Supposed can set font size to 18x18, but not working in my console, please use Ctrl + MouseWheel to change font size
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(consoleHandle, 0, lpConsoleCurrentFontEx);
	lpConsoleCurrentFontEx->dwFontSize.X = 18;
	lpConsoleCurrentFontEx->dwFontSize.Y = 18;
	SetCurrentConsoleFontEx(consoleHandle, 0, lpConsoleCurrentFontEx);
}

int main_menu()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;                 // Next 4 lines set cursor visibility to true
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = true;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
	
	
	cout << "PacMan" << endl;
    cout << "Press 1 to start" << endl;
    cout << "Press 2 for level editor" << endl;
    cout << "Press 3/esc to exit" << endl;

    unsigned char CharInput;
    CharInput = _getch(); 	// get key stroke

    if (CharInput == 49) 	// 1 key
    {
        return 1;
    }
    else if (CharInput == 50) // 2 key
    {
        return 2;
    }
    else if (CharInput == 51) // 3 key
    {
        return 3;
    }
    else if (CharInput == 27) // esc key
	{
		return 3;
	}
	else
    {
        cout << "Invalid input, please try again" << endl;
		Sleep(200);
		return 0;
    }
}

void runGame()
{
    system("cls");                  // clear console screen, start from empty
    int x = 0, y = 0;               // record player's position
    int direction = 0;              // record player's direction; 1 up, 2 left, 3 right, 4 down        
    int score = -1;                 // record player's score, -1 so that it will be 0 when game start
    int timetaken = 0;              // record time taken
    
    for (int i = 0; i < COLS; ++i)      // initialize dots coordinates
    {
        for (int j = 0; j < ROWS; ++j) 
        {
            dots[i][j].dotsX = i;
            dots[i][j].dotsY = j;
            dots[i][j].isDots = true;
        }
    }
    
    for (;;) // infinite loop
    {
        
        if (dots[x][y].isDots == true)
        {
            dots[x][y].isDots = false; // remove dots when player pass through
            score++;                    // add score when player pass through
        }

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
            	break;
            }
        }
        else  //When key is not pressed, keep moving in the same direction
        {
			if (direction == 1 && y > 0) y--; 		// up momentum
			else if (direction == 2 && x > 0) x--; 	// left momentum
			else if (direction == 3 && x < 39) x++; // right momentum
			else if (direction == 4 && y < 19) y++; // down momentum
			else direction = 0; 					// stop momentum when hit wall
        }

        // Display routine
        COORD coord = {0, 0};
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleCursorPosition(consoleHandle, coord); // Set cursor position to (0,0)
        CONSOLE_CURSOR_INFO cursorInfo;                 // Next 4 lines set cursor visibility to FALSE
        GetConsoleCursorInfo(consoleHandle, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &cursorInfo);

        for (int row = 0; row < ROWS; row++) // draw 20x40 screen
        {
            for (int column = 0; column < COLS; column++)
            {
                if (row == y && column == x) // Print player when position matched
                {
                    switch (direction) // Print player direction, 0 up, 1 left, 2 right, 3 down
                    {
                    case 0:
                        cout << "X"; break;
                    case 1:
                        cout << "^"; break;
                    case 2:
                        cout << "<"; break;
                    case 3:
                        cout << ">"; break;
                    case 4:
                        cout << "v"; break;
                    }
                }
                else if (dots[column][row].isDots) 
                {
                    cout << "."; // Print dots in dots position
                }
                
                else
                {
                    cout << " "; // Print space in non-player position
                }
            }
            cout << endl;
        }
        cout << "Score: " << score << endl;
        cout << "Time taken: " << setw(4) << timetaken++ / 20 << "s" << endl;  //Speed is 50ms, there for 20 times = 1 second
        cout << "Press esc to quit" << endl;
        Sleep(50);  // Speed control
    }
}

void level_editor()
{
	unsigned char level_editor_selection = 0;
	
	while (level_editor_selection != 51)
	{
		system("cls");
		
		cout << "Level editor" << endl;
		cout << "Press 1 to create a new level" << endl;
		cout << "Press 2 to edit an existing level" << endl;
		cout << "Press 3/esc to return to main menu" << endl;
		level_editor_selection = _getch();

		if (level_editor_selection == 49)  // 1 key
		{
			// create new level
		}
		else if (level_editor_selection == 50)  // 2 key
		{
			// edit existing level
		}
		else if (level_editor_selection == 51)  // 3 key
		{
			break;
		}
		else if (level_editor_selection == 27)  // esc key
		{
			break;
		}
		else
		{
			cout << "Invalid input, please try again" << endl;
			Sleep(200);
		}
	}
}