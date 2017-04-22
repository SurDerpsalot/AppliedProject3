#include <thread>
#include <string>
#include <interpreter.h>

// implement vtray main here

int main(int argv, char * argc[])
{
	int numThreads;
	int exit;
	interpreter interp;
	QString inFile;
	QString tds;
	exit = EXIT_SUCCESS;
	if (argv == 5)
	{
		std::cout << "multithreaded thingy" << std::endl;
		if (argc[1] == "-t")
		{
			
			inFile = argc[3];
			tds = argc[2];
			numThreads = tds.toInt();
			try {
				interp = interp.fromJSON(inFile);
			}
			catch (QJsonParseError & error)	{
				exit = EXIT_FAILURE;
			}
		}
		else
		{
			std::cerr << "Error : Incorrect input arguments" << std::endl;
			exit =  EXIT_FAILURE;
		}
	}
	else if (argv == 3)
	{
		std::cout << "just json and output file" << std::endl;
		inFile = argc[1];
		try {
			interp = interp.fromJSON(inFile);
		}
		catch (QJsonParseError & error)	{
			exit = EXIT_FAILURE;
		}
	}
	else
	{
		std::cerr << "Error : Incorrect number of input arguments" << std::endl;
		exit =  EXIT_FAILURE;
	}
	return exit;
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