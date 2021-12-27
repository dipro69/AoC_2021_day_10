// AoC_2021_day_10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// #define DEFINE_TEST

#include <fstream>
#include <string>
#include <iostream>

using namespace std;

// global constants
#ifdef DEFINE_TEST
const int NUM_LINES = 10; // constant for number of lines in file to read, in test num_lines = 10, in production 98
const int MAX_LEN_LINE = 24; // constant for number of lines in file to read, in test max_len_line = 24, in production 109
const string INPUT_FILE = "syntax_test.txt"; // filename with the input data, in test input_file = "syntax_test.txt", in production "syntax.txt"
const int NUM_AUTO_COMPLETE = 5;
#else
const int NUM_LINES = 98; // constant for number of lines in file to read, in test num_lines = 10, in production 98
const int MAX_LEN_LINE = 109; // constant for number of lines in file to read, in test max_len_line = 24, in production 109
const string INPUT_FILE = "syntax.txt"; // filename with the input data, in test input_file = "syntax_test.txt", in production "syntax.txt"
const int NUM_AUTO_COMPLETE = 49;
#endif

/*

Part 1 score

Did you know that syntax checkers actually have contests to see who can get the high score for syntax errors in a file?
It's true! To calculate the syntax error score for a line, take the first illegal character on the line and look it up in the following table:

): 3 points.
]: 57 points.
}: 1197 points.
>: 25137 points.

Part 2 score

Did you know that autocomplete tools also have contests? It's true!
The score is determined by considering the completion string character-by-character.
Start with a total score of 0. Then, for each character,
multiply the total score by 5 and then increase the total score by the point value given for the character in the following table:

): 1 point.
]: 2 points.
}: 3 points.
>: 4 points.

*/

// global variables
string* pInputStrArr = new string[NUM_LINES]; // string array with data from the input file
int final_score[] = { 0,0,0,0 }; // final scores for ), ], }, > part 1
int max_string_length = 0; // store the max length of the lines from the input file
int stack_index = -1; // stack index for stack used in part 1
int num_auto_complete = 0;

#ifdef DEFINE_TEST

char CharStackArray[MAX_LEN_LINE] = {	'0','1','2','3','4','5','6','7','8','9', // 1
										'0','1','2','3','4','5','6','7','8','9', // 2
										'0','1','2','3' };	 // 24 items

unsigned long long int auto_complete_score[NUM_AUTO_COMPLETE] = {0,0,0,0,0};

#else
char CharStackArray[MAX_LEN_LINE] = {	'0','1','2','3','4','5','6','7','8','9', // 1
										'0','1','2','3','4','5','6','7','8','9', // 2
										'0','1','2','3','4','5','6','7','8','9', // 3
										'0','1','2','3','4','5','6','7','8','9', // 4
										'0','1','2','3','4','5','6','7','8','9', // 5
										'0','1','2','3','4','5','6','7','8','9', // 6
										'0','1','2','3','4','5','6','7','8','9', // 7
										'0','1','2','3','4','5','6','7','8','9', // 8
										'0','1','2','3','4','5','6','7','8','9', // 9
										'0','1','2','3','4','5','6','7','8','9', // 10
										'0','1','2','3','4','5','6','7','8' };	 // 109 items

unsigned long long int auto_complete_score[NUM_AUTO_COMPLETE] = { 0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0 }; // 49 items

#endif

int init();
int exit();
int getInputData();
int showInputData();
int checkStrCorrupted(int row);
int checkCharMatch(char& in_c, char check_c, int& index);
unsigned long long int doAutoComplete();
int bubbleSort(int num_elements);

int main()
{
	// initalize globals and get input data
	init();

	// now perform main algorithm
    cout << "Hello Advent of Code day 10!" << "\n";
	// test if input is read correctly
	showInputData();

	int check_result = -1;
	// for all the string lines check if corrupt
	for (int i = 0; i < NUM_LINES; i++)
	{
		check_result = -1;
		check_result = checkStrCorrupted(i);
		cout << "line index: " << i << " corruption result: " << check_result << "\n";
		if (check_result == 0)
		{
			auto_complete_score[num_auto_complete] = doAutoComplete();
			num_auto_complete++;
		}
	}

	bubbleSort(num_auto_complete);
	cout << " after sorting scores part 2:\n";
	for (int i = 0; i < num_auto_complete; i++)
	{
		cout << auto_complete_score[i] << " ";
	}
	cout << "\n";

	// calculate and echo the final score
	int sum_final_score = final_score[0] + final_score[1] + final_score[2] + final_score[3];
	cout << "sum final score part 1: " << sum_final_score << "\n";

	cout << "auto complete part 2: " << num_auto_complete << "\n";

	int index_middle_score = num_auto_complete / 2;
	cout << "middle score part 2: " << auto_complete_score[index_middle_score] << "\n";

	// pause and exit
	system("pause");
	exit(); // perform memory cleanups
}

int init()
{
	try
	{
		// read string data from file
		getInputData();
		cout << "max string length: " << max_string_length << "\n";
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
		cout << "line index: " << i << " " << *(pInputStrArr + i) << "\n";
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

		// store the string length and get the maximum length of a line
		int sl = line.length();
		if (max_string_length < sl)
		{
			max_string_length = sl;
		}

		// store the input data in a line
		*(pInputStrArr + i) = line;
		i++;
	}

	// close file and free resources
	in.close();

	// return function value
	return 0;
}

/*

Did you know that syntax checkers actually have contests to see who can get the high score for syntax errors in a file?
It's true! To calculate the syntax error score for a line, take the first illegal character on the line and look it up in the following table:

): 3 points.
]: 57 points.
}: 1197 points.
>: 25137 points.

*/

int checkStrCorrupted(int row)
{
	int output_result = 0;

	// get the chars in this string line
	string s = *(pInputStrArr + row);
	// how much chars do we have in this line?
	int string_length = s.length();
	// echo string length, just checking
	cout << " length " << string_length << " ";
	// let's capture the chars in the string in an array
	char* pCharArray; //pointer to a char array
	pCharArray = &s[0]; // store the adress of string s

	// let's get ourselves an index for our char stack
	// this is a global variable!
	stack_index = -1;

	// loop through all the elements in the string
	for (int i = 0; i < string_length; i++)
	{
		// decode the int value in the substring
		cout << *(pCharArray + i) << " ";

		// first push this item on the stack if it is a chunk starting char [, {, < or (
		if (*(pCharArray + i) == '(' || *(pCharArray + i) == '[' || *(pCharArray + i) == '{' || *(pCharArray + i) == '<')
		{
			stack_index++;
			CharStackArray[stack_index] = *(pCharArray + i);
		}
		// if not a chunk starting char then it must be a chunk ending char
		else if ( *(pCharArray + i) == ')' || *(pCharArray + i) == ']' || *(pCharArray + i) == '}' || *(pCharArray + i) == '>' )
		{
			// if stack is empty then we have a corrupt line!
			if (stack_index == -1)
			{
				return 1;
			}
			else
			{
				// in that case check with item on the stack, it must match otherwise this line is corrupt
				switch (*(pCharArray + i))
				{
				case (')'):
					output_result = checkCharMatch( CharStackArray[stack_index], '(', stack_index);
					if (output_result == 1)
					{
						final_score[0] += 3;
						return 1;
					}
					break;
				case (']'):
					output_result = checkCharMatch(CharStackArray[stack_index], '[', stack_index);
					if (output_result == 1)
					{
						final_score[1] += 57;
						return 1;
					}
					break;
				case ('}'):
					output_result = checkCharMatch(CharStackArray[stack_index], '{', stack_index);
					if (output_result == 1)
					{
						final_score[2] += 1197;
						return 1;
					}
					break;
				case ('>'):
					output_result = checkCharMatch(CharStackArray[stack_index], '<', stack_index);
					if (output_result == 1)
					{
						final_score[3] += 25137;
						return 1;
					}
					break;
				default:
					break;
				}
			}

		}
	}

	// if all items are checked and we have no corruption than this line is ok
	return 0;
}

// match char, if match than pop the stack top
int checkCharMatch(char &in_c, char check_c, int &index)
{
	if (in_c != check_c)
	{
		return 1;
	}
	else
	{
		//if we have a match, pop last item on the stack
		in_c = 'A';
		index--;
	}

	return 0;
}

unsigned long long int doAutoComplete()
{

	/*
		Did you know that autocomplete tools also have contests? It's true!
		The score is determined by considering the completion string character-by-character.
		Start with a total score of 0. Then, for each character,
		multiply the total score by 5 and then increase the total score by the point value given for the character in the following table:

		(: 1 point.
		[: 2 points.
		{: 3 points.
		<: 4 points.
	*/

	unsigned long long int part2_sub_score = 0;

	cout << "printing stack array: ";
	for (int i = stack_index; i > -1; i--)
	{
		cout << CharStackArray[i];

		switch (CharStackArray[i])
		{
		case('('):
			part2_sub_score = part2_sub_score * 5;
			part2_sub_score += 1;
			break;
		case('['):
			part2_sub_score = part2_sub_score * 5;
			part2_sub_score += 2;
			break;
		case('{'):
			part2_sub_score = part2_sub_score * 5;
			part2_sub_score += 3;
			break;
		case('<'):
			part2_sub_score = part2_sub_score * 5;
			part2_sub_score += 4;
			break;
		default:
			break;
		}
	}
	cout << "\n";
	cout << " part 2 sub score: " << part2_sub_score << "\n";

	return part2_sub_score;
}

int bubbleSort(int num_elements)
{
	int i, j, pass = 0;
	unsigned long long int temp = 0;

	for (i = 0; i < num_elements; i++)
	{
		for (j = i + 1; j < num_elements; j++)
		{
			if (auto_complete_score[j] < auto_complete_score[i])
			{
				temp = auto_complete_score[i];
				auto_complete_score[i] = auto_complete_score[j];
				auto_complete_score[j] = temp;
			}
		}
		pass++;
	}

	return 0;
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