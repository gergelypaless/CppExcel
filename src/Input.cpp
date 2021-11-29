#include "Input.h"

#include "utils.h"
#include "CommandProcessor.h"
#include <cassert>


void Input::Read(TableContainer& tableContainer, std::istream& is, std::ostream& os)
{
	tableContainer.GetCurrentTable().Print(os);
	tableContainer.PrintTableNames(os);
	os << ">>> ";

	std::string input;
	std::getline(is, input);
	auto tokens = SplitString(input, " ");
	try
	{
		CommandProcessor::ProcessCommand(tokens, tableContainer);
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

void Input::ReadFile(TableContainer& tableContainer, const std::string& filename, char sep)
{
	auto fileNameWithExtension = SplitString(filename, '/').back();
	auto fileNameTokens = SplitString(fileNameWithExtension, '.');
	std::string tableName;
	for (size_t i = 0; i < fileNameTokens.size() - 1; ++i)
	{
		tableName += fileNameTokens[i];
	}
	
	std::ifstream is(filename);
	if (!is.good())
		throw std::invalid_argument("Error: there is no such file as '" + filename + "'");
	
	tableContainer.emplace_back(std::make_unique<Table>(tableName));
	tableContainer.SetCurrentTable(tableContainer.size() - 1);
	auto& table = tableContainer.GetCurrentTable();
	
	std::string input;
	while (std::getline(is, input))
	{
		auto tokens = SplitString(input, sep);
		if (tokens.size() > table.GetRowLength())
			table.AddCols(tokens.size() - table.GetRowLength());

		for (size_t i = 0; i < tokens.size(); i++)
			table->LastRow()[i].SetContent(tokens[i]);

		table.AddRows(1);
	}
	table.DeleteRow(table.LastRowIndex());
}
