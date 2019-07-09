// Project_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Node.h"
#include "encoder.h"
#include "decoder.h"
#include <iostream>


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		puts("Please enter a filename and choose encode or decode!");
		return 0;
	}

	std::string filePath;
	std::string selected;
	if (argv[1][0] == '-')
	{
		filePath = argv[2];
		selected = argv[1];
	}
	else if (argv[2][0] == '-')
	{
		filePath = argv[1];
		selected = argv[2];
	}

	if (selected[1] == 'e')
	{
		encoder e(filePath);
	}
	else if (selected[1] == 'd')
	{
		decoder d(filePath);
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
