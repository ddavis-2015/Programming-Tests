//
// Copyright (c) 2019 David Davis.  All rights reserved.
//
// Compiled on macOS with:
//
//	clang++ -O2 -std=c++14 -o add1 add1.cpp 
//

static const char copyright[] __attribute((used)) =
	"Copyright (c) 2019 David Davis.  All rights reserved.";


#include <iostream>
#include <string>
#include <stdexcept>


void usage(const char* programName)
{
	std::cout << "usage: " << programName << " base_10_number\n";
}

int add1(int val)
{
	int carry;
	int result;

	result = val ^ 1;
	carry = val & 1;

	while (carry)
	{
		result ^= (carry << 1);
		carry = val & (carry << 1);
	}

	return result;
}

int main(int argc, const char** argv)
{
	if (--argc == 1)
	{
		try
		{
			int val = std::stoi(std::string(argv[1]));
			std::cout << "result: " << add1(val) << std::endl;
		}
		catch (std::exception)
		{
			usage(argv[0]);
		}
	}
	else
		usage(argv[0]);

	return 0;
}

