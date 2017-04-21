#include <thread>
#include <iostream>
#include <string>
#include <interpreter.h>


void parse(std::string jasonIN);

// implement vtray main here

int main(int argv, char * argc[])
{
	int numThreads;
	std::string inFile;
	if (argv == 4)
	{
		if (argc[0] == "-t")
		{
			parse(inFile);
		}
		else
		{
			std::cerr << "Error : Incorrect input arguments" << std::endl;
			return EXIT_FAILURE;
		}
	}
	else if (argv == 2)
	{
		parse(inFile);
	}
	else
	{
		std::cerr << "Error : Incorrect input arguments" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void parse(std::string jasonIN) {
	std::cout << "le parse" << std::endl;
}


// implement vtray main here
/*
for each pixel of the screen
{
	Final color = 0;
	Ray = { starting point, direction };
	Repeat
	{
		for each object in the scene
		{
			determine closest ray object / intersection;
		}
		if intersection exists
		{
			for each light in the scene
			{
				if the light is not in shadow of another object
				{
					add this light contribution to computed color;
				}
			}
		}
		Final color = Final color + computed color * previous reflection factor;
		reflection factor = reflection factor * surface reflection property;
		increment depth;
	} 
	until reflection factor is 0 or maximum depth is reached;
}
*/