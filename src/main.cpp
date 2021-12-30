#include "utils.h"
#include "Table.h"
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

	BarchartDiagramData data = {
			3,
			{
					{ "laptop", { 1, 2, 3 } },
					{ "monitor", { 10, 20, 30 } },
					{ "mouse", { 30, 10, 80 } },
					{ "pendrive", { 123, 40, 60 }}
			},
			{
					{ "Buda", Color(0) },
					{ "Pest", Color(1) },
					{ "Isaszeg", Color(2) },
					{ "Lisszabon", Color(3) },
			},
			{ 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130 }
	};
	
	std::cout << BarchartDiagram::CreateHtml(data) << std::endl;
	
	std::unique_ptr<Table> table;
	if (argc > 1)
	{
		try
		{
			if (argc != 4)
			{
				table = Input::ReadFile(argv[1]);
			}
			else if (std::string(argv[2]) == "-sep")
			{
				table = Input::ReadFile(argv[1], argv[3][0]);
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
		table = std::make_unique<Table>();	
	}

	Input::Read(std::cin, std::cout, *table);
	while (!CommandProcessor::ShouldExit())
		Input::Read(std::cin, std::cout, *table);

	return 0;
}
