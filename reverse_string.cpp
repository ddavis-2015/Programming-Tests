//
// Copyright (c) 2019 David Davis.  All rights reserved.
//
// Compiled on macOS with:
//
//	clang++ -O2 -std=c++14 -o reverse_string reverse_string.cpp 
//

static const char copyright[] __attribute((used)) =
	"Copyright (c) 2019 David Davis.  All rights reserved.";


#include <iostream>
#include <string>
#include <cstring>


void usage(const char* programName)
{
	std::cout << "usage: " << programName << " string\n";
}

void reverse(char* string)
{
	int stringLength;
	int i;

	stringLength = strlen(string);

	for (i = 0; i < stringLength / 2; i++)
	{
		char temp;

		temp = string[i];
		string[i] = string[(stringLength - 1) - i];
		string[(stringLength - 1) - i] = temp;
	}
}

int main(int argc, const char** argv)
{
	if (--argc == 1)
	{
		char* s = new char[strlen(argv[1]) + 1];

		(void)strcpy(s, argv[1]);
		reverse(s);
		std::cout << "result: " << s << std::endl;
		delete[] s;
	}
	else
		usage(argv[0]);

	return 0;
}

