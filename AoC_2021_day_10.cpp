// AoC_2021_day_10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "dipro_utils.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

// global constants
const int NUM_LINES = 10; // constant for number of lines in file to read
const int MAX_LEN_LINE = 24; // constant for number of lines in file to read

// const int LEN_LINE = 100; // constant for number of digits in an input line in file to read
const string INPUT_FILE = "syntax_test.txt"; // filename with the input data

// global variables
// string* pInputStrArr;
string* pInputStrArr = new string[NUM_LINES];

int init();
int exit();
int getInputData();
int showInputData();
bool decodeStr(int row);

int main()
{
	// initalize globals and get input data
	init();

	// now perform main algorithm
    cout << "Hello Advent of Code day 10!" << "\n";
	// test if input is read correctly
	showInputData();

	for (int i = 0; i < NUM_LINES; i++)
	{
		cout << decodeStr(i) << "\n";
	}

	// pause and exit
	system("pause");
	exit(); // perform memory cleanups
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

int init()
{
	try
	{
		// read string data from file
		getInputData();
	}
	catch (...)
	{
		cout << "Some error occured while getting input data.\n";
	}

    return 0;
}

int exit()
{
	delete[] pInputStrArr;

    return 0;
}

int showInputData()
{
	for (int i = 0; i < NUM_LINES; i++)
	{
		cout << *(pInputStrArr + i) << "\n";
	}

	return 0;
}

// this function opens a file and reads the lines into a string array
int getInputData()
{
	// input param: string* input_data: pointer to array that receives the input strings from file
	// input param: string file_name: the file to read from

	string line; // here we store a line of input data

	// define and open the filestream
	ifstream in;
	in.open(INPUT_FILE);

	// loop through all the lines in the input file
	int i = 0;
	while (!in.eof())
	{
		// read the line from the current position in the file
		getline(in, line);
		// store the input data in a line
		*(pInputStrArr + i) = line;
		i++;
	}

	// close file and free resources
	in.close();

	// return function value
	return 0;
}

bool decodeStr(int row)
{
	bool checkArray[MAX_LEN_LINE];
	for (int i = 0; i < MAX_LEN_LINE; i++)
	{
		checkArray[i] = true;
	}

	string s = *(pInputStrArr + row);
	int string_length = s.length();
	cout << "length " << string_length << "\n";

	char* pCharArray; //pointer to a char array
	pCharArray = &s[0]; // store the adress of string s

	// loop through all the elements in the string
	for (int i = 0; i < string_length; i++)
	{
		// decode the int value in the substring
		cout << *(pCharArray + i) << " ";

		if ( *(pCharArray + i) == ')' || *(pCharArray + i) == ']' || *(pCharArray + i) == '}' || *(pCharArray + i) == '>' )
		{
			// go back 1 item
			if (i-1 < 0)
			{
				// if first item is closing char than we have a corrupt line
				return false;
			}
			// if previous item in checkarray is true and previous item in string is inside range than check items
			else if ( checkArray[i-1] && i-1 >= 0 )
			{
				// if two check items belong together dan set checkarray to false, no inspection necessary anymore
				if ( *(pCharArray + i) == ')' && *(pCharArray + i-1) == '(' && checkArray[i - 1] )
				{
					checkArray[i-1] = false;
					checkArray[i] = false;
				}
				else
				{
					return false;
				}
			}
		}
	}
	cout << "\n";

	return true;
}
