#include "Input.h"

#include "utils.h"
#include "CommandProcessor.h"
#include <cassert>


void Input::Read(std::istream& is, std::ostream& os, Table& table)
{
	table.Print(os);
	os << ">>> ";

	std::string input;
	std::getline(is, input);
	auto tokens = SplitString(input, " ");
	try
	{
		CommandProcessor::ProcessCommand(tokens, table);
	}
	catch (const std::invalid_argument& ex)
	{
		os << ex.what() << std::endl;
	}
	catch (const std::out_of_range& ex)
	{
		os << "Error: " << ex.what() << ". ";
		os << "You are overindexing the vector!" << std::endl;
	}
}

std::unique_ptr<Table> Input::ReadFile(const std::string& filename, char sep)
{
	std::unique_ptr<Table> table = std::make_unique<Table>(); // table with 1 row, 1 col

	std::ifstream is(filename);
	if (!is.good())
		throw std::invalid_argument("Error: there is no such file as '" + filename + "'");

	std::string input;
	while (std::getline(is, input))
	{
		auto tokens = SplitString(input, sep);
		if (tokens.size() > Row::GetRowLength())
			table->AddCols(tokens.size() - Row::GetRowLength());

		for (size_t i = 0; i < Row::GetRowLength(); i++)
			table->LastRow()[i].SetContent(tokens[i]);

		table->AddRows(1);
	}
	table->DeleteRow(table->LastRowIndex());

	return table;
}
