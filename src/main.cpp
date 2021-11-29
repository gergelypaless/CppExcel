#include "utils.h"
#include "TableContainer.h"
#include "CommandProcessor.h"
#include "Input.h"
#include "BarchartDiagram.h"
#include "Color.h"

#include <iostream>
#include <string>
#include <memory>
#include <fstream>


// argv first argument is the full path of the executable
int main(int argc, char** argv)
{
	std::cout << argv[0] << std::endl;

	TableContainer tableContainer;
	if (argc > 1)
	{
		try
		{
			if (argc != 4)
			{
				Input::ReadFile(tableContainer, argv[1]);
			}
			else if (std::string(argv[2]) == "-sep")
			{
				Input::ReadFile(tableContainer, argv[1], argv[3][0]);
			}
			else
			{
				std::cout << "Error: Wrong parametrization" << std::endl;
				return 1;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what() << std::endl;
			return 1;
		}
	}
	else
	{
		tableContainer.emplace_back();
	}

	Input::Read(tableContainer, std::cin, std::cout);
	while (!CommandProcessor::ShouldExit())
		Input::Read(tableContainer, std::cin, std::cout);

	return 0;
}
