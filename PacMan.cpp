#include <iostream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>  //This is to get list of files in directory

using namespace std;
namespace fs = std::filesystem;

int main_menu(); // main menu function
void runGame();  // function to handle game logic and display
void level_editor_menu(); // function to handle level editor menu
void level_editor(); // function to handle level editor
void level_create(); // function to handle level creation
string loaded_level(bool previous, bool next); // string to store level name

const int COLS = 40;    //X-axis
const int ROWS = 20;    //Y-axis

int files_id = 0;

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
			level_editor_menu(); 				// call the function to run the level editor
		}
		else if (main_menu_selection == 3) 		// if main menu selection is 3, exit
		{
			exit(0);
		}
	}

    return 0;
}

int main_menu()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;                         // Next 4 lines set cursor visibility to true
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = true;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
	
	cout << "PacMan" << endl;
    cout << "Press 1 to start" << endl;
    cout << "Press 2 for level editor" << endl;
    cout << "Press 3/esc to exit" << endl;
    cout << endl;
    cout << "Current level: " << loaded_level(0, 0) << endl;
    cout << "Press a/d to change level" << endl;

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
	else if (CharInput == 97 || CharInput == 65) // a key
    {
        loaded_level(1, 0);
        return 0;
    }
    else if (CharInput == 100 || CharInput == 68) // d key
    {
        loaded_level(0, 1);
        return 0;
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
    int score = 0;                 // record player's score
    int timetaken = 0;              // record time taken
    int game_wall_coords [20][40] = { 0 }; // record wall coordinates, 0 = no wall, 1 = wall

    ifstream infile;
	infile.open(loaded_level(0, 0));
	if (!infile)
		infile.close(); //file error
    
    int xx, yy;
    while (infile >> xx >> yy)
    {
        game_wall_coords[yy][xx] = 1;
    }

    for (int i = 0; i < COLS; ++i)      // initialize dots coordinates
    {
        for (int j = 0; j < ROWS; ++j) 
        {
            dots[i][j].dotsX = i;
            dots[i][j].dotsY = j;
            dots[i][j].isDots = true;
            if (game_wall_coords[j][i] == 1) // if wall, set isDots to false
                dots[i][j].isDots = false;
        }
    }

    // dots[12][15].isDots = true;         // debuging: set a single dots position

    for (;;) // infinite loop
    {
        bool isDotsLeft = false; // used to determine any dots left

        if (_kbhit()) // Check key stroke
        {
            unsigned char CharInput = _getch(); // arrow key is a 2-key combination, 244-72 for "up", this get first key
            if (CharInput == 224)               // confirm it is a 2-key combination
            {
                CharInput = _getch(); // get second key
                switch (CharInput)
                {
                case 72:
                    if (y > 0 && (game_wall_coords[y-1][x] != 1)) y--; direction = 1; break; // up
                case 75:
                    if (x > 0 && (game_wall_coords[y][x-1] != 1)) x--; direction = 2; break; // left
                case 77:
                    if (x < 39 && (game_wall_coords[y][x+1] != 1)) x++; direction = 3; break; // right
                case 80:
                    if (y < 19 && (game_wall_coords[y+1][x] != 1)) y++; direction = 4; break; // down
                }
            }
            else if (CharInput == 27) // esc key quit, other no respond
            {
            	break;
            }
        }
        else  //When key is not pressed, keep moving in the same direction
        {
			if (direction == 1 && y > 0 && (game_wall_coords[y-1][x] != 1)) y--; 		// up momentum
			else if (direction == 2 && x > 0 && (game_wall_coords[y][x-1] != 1)) x--; 	// left momentum
			else if (direction == 3 && x < 39 && (game_wall_coords[y][x+1] != 1)) x++; // right momentum
			else if (direction == 4 && y < 19 && (game_wall_coords[y+1][x] != 1)) y++; // down momentum
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

        for (int row = 0; row < ROWS *2; row++) // draw 40x80 screen
        {
            for (int column = 0; column < COLS *2; column++)
            {
                if ((row == y*2 || row == y*2+1) && (column == x*2 || column == x*2+1)) // Print 2X of player when position matched
                {
                    switch (direction) // Print player direction, 1 up, 2 left, 3 right, 4 down
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
                    
                    if (dots[x][y].isDots == true)  // Check if there is any dots in player position
                    {
                        dots[x][y].isDots = false; // remove dots when player pass through
                        score++;                    // add score when player pass through
                    }
                }
                else if (game_wall_coords[row/2][column/2] == 1)
                {
					cout << '#';
                }
                else if (dots[column/2][row/2].isDots) // Print '.' if dots[coords][coords].isDots is true
                {
                    cout << "."; // Print dots in dots position
                    isDotsLeft = true; // Set isDotsLeft to true if there is any dots left
                }
                else
                {
                    cout << " "; // Print space in non-player position
                }
            }
            cout << "#"; // Print right wall
            cout << endl;
        }
        for (int lastrow = 0; lastrow < 80; lastrow++) //Print bottom wall
            cout << "#";
        cout << endl;
        cout << "Score: " << score << endl;
        cout << "Time taken: " << setw(4) << timetaken++ / 6 << "s" << endl;  //Speed is 50ms, therefor 20 rounds = 1 second; Currently set to 6 for debugging
        cout << "Press esc to quit" << endl;
        Sleep(50);  // Speed control

        if (isDotsLeft == false) // Check if there is any dots left
        {
            cout << endl;
            cout << "You win!" << endl;
            cout << "Score: " << score << endl;
            cout << "Time taken: " << setw(4) << timetaken / 6 << "s" << endl;
            cout << "Press any key to continue" << endl;
            _getch();
            break;
        }
    }
}

void level_editor_menu()
{
	unsigned char level_editor_selection = 0;
	
	while (level_editor_selection != 51)
	{
		system("cls");
		
		cout << "Level editor" << endl;
		cout << "Press 1 to create a new level" << endl;
		cout << "Press 2 to edit an existing level" << endl;
		cout << "Press 3/esc to return to main menu" << endl;
        cout << endl;
        cout << "Current level: " << loaded_level(0, 0) << endl;
        cout << "Press a/d to change level" << endl;


		level_editor_selection = _getch();

		if (level_editor_selection == 49)  // 1 key
		{
			level_create();
		}
		else if (level_editor_selection == 50)  // 2 key
		{
			level_editor();
		}
		else if (level_editor_selection == 51)  // 3 key
		{
			break;
		}
		else if (level_editor_selection == 27)  // esc key
		{
			break;
		}
        else if (level_editor_selection == 97 || level_editor_selection == 65) // a key
        {
            loaded_level(1, 0);
        }
        else if (level_editor_selection == 100 || level_editor_selection == 68) // d key
        {
            loaded_level(0, 1);
        }
        else
		{
			cout << "Invalid input, please try again" << endl;
			Sleep(200);
		}
	}
}

void level_editor()
{
    system("cls"); 		//clear console screen, start from empty

	int x = 0, y = 0; 	//record player's position
	int wall_coords [20][40] = { 0 }; //record wall coordinates, 0 = no wall, 1 = wall

	ifstream infile;
	infile.open(loaded_level(0, 0));
	if (!infile)
		infile.close(); //file error
    
    int xx, yy;
    while (infile >> xx >> yy)
    {
        wall_coords[yy][xx] = 1;
    }

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
			else if (c == 99 || c == 67)  // When press C, coordinates of wall is recorded
			{
                if ((x != 0 || y != 0) && (x != 39 || y != 19))  //P and G position cannot be a wall
                    wall_coords[y][x] = 1; 
				
                if (x < 39)
                    x++; // Move player to next position
			} 
            else if (c == 120 || c == 88)  // When press X, coordinates of wall is removed
			{
                if ((x != 0 || y != 0) && (x != 39 || y != 19))  // P and G position cannot be a removed
                    wall_coords[y][x] = 0; 
				
                if (x < 39)
                    x++; // Move player to next position
			} 
			else if (c == 27) //esc key
			{
				ofstream outfile;
				outfile.open(loaded_level(0, 0));
				if (!outfile)
					outfile.close(); //file error

				for (int r = 0; r < 20; r++)
				{
					for (int c = 0; c < 40; c++)
					{
						if (wall_coords[r][c] == 1)
							outfile << c << " " << r << endl;
					}
				}
                break;
			}
		}

		//Display routine
		COORD coord = { 0, 0 };
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		
		SetConsoleCursorPosition(consoleHandle, coord); // Set cursor position to (0,0)
		CONSOLE_CURSOR_INFO cursorInfo;                 // Next 4 lines set cursor visibility to FALSE
		GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	    cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(consoleHandle, &cursorInfo);

		for (int r = 0; r < 20; r++)
		{
			for (int c = 0; c < 40; c++)
			{
				if (r == y && c == x)
					cout << "X";
				else if (r == 0 && c == 0)
					cout << "P";
				else if (r == 19 && c == 39)
					cout << "G";
				else if (wall_coords[r][c] == 1)
					cout << '#';
				else
					cout << " ";
			}
            cout << "#";
			cout << endl;
		}
        for (int lastrow = 0; lastrow < 40; lastrow++) //Print bottom wall
            cout << "#";

        cout << endl;
        cout << "Press C to create wall, X to remove wall" << endl;
        cout << "Press esc to save and return to level editor menu" << endl;

		//Speed control
		Sleep(100);
	}
}

void level_create()
{
    string custom_level_name;
    system("cls"); 		//clear console screen, start from empty
    cout << "Please enter level name: ";
    cin >> custom_level_name;
    
    ofstream outfile;
    outfile.open(custom_level_name + ".txt");
    if (!outfile)
        outfile.close(); //file error

    cout << "Successfully created level: " << custom_level_name << endl;
    cout << "Please proceed to level editor to edit the level" << endl;
    cout << "Press any key to continue" << endl;
    _getch();
}

string loaded_level(bool previous, bool next)  //Read all txt name, all of the txt will be level
{
    vector<string> txt_files;  // array to store 100 txt files' name

    for (const auto & entry : fs::directory_iterator("."))  // get all txt files name in current directory
    {
        if (entry.path().extension() == ".txt")
        {
            txt_files.push_back(entry.path().filename().string());
        }
    }
    
    if (previous == true)  //If previous is true, go to previous txt file
    {
        if (files_id != 0)  //Cannot go below 0
            files_id--;
    }
    else if (next == true)  //If next is true, go to next txt file
    {
        if (files_id < txt_files.size() - 1)  //Cannot go above array size
            files_id++;
    }
    else
    {
        return txt_files[files_id];
    }

    return txt_files[files_id];  //Return current txt file name
}