#include "CommandProcessor.h"

#include <algorithm>
#include <fstream>

#include "utils.h"
#include "SortDirection.h"
#include "Alignment.h"
#include "Range.h"
#include "BarchartDiagram.h"
#include "Input.h"


bool CommandProcessor::shouldExit = false;


bool CommandProcessor::ShouldExit()
{
	return shouldExit;
}

void CommandProcessor::ProcessEditCommand(const std::vector<std::string>& commandTokens, Table& table)
{
	char colIndex = ConvertColLetterToNumber(commandTokens[1][0]);
	
	//                                this skips the first letter, which is the column's name
	std::string rowNumberString = commandTokens[1].substr(1, commandTokens[1].size() - 1);
	
	size_t row = ConvertStringToUInt(rowNumberString) - 1;

	std::string content;
	std::for_each(commandTokens.begin() + 2, commandTokens.end(), [&] (const std::string& element) {
		content += element + ' ';
	});

	if (!content.empty())
		content.pop_back(); // deletes the last space
	table[row][colIndex].SetContent(content);
}

void CommandProcessor::ProcessAddCommand(const std::vector<std::string>& commandTokens, Table& table)
{
	// check input for errors
	if (commandTokens[2] != "rows" && commandTokens[2] != "cols")
		throw std::invalid_argument("Error: unknown argument '" + commandTokens[3] + "'. Please specify 'rows' or 'cols'!");
	
	
	size_t numberOfRowsToAdd = ConvertStringToUInt(commandTokens[1]);
	if (commandTokens[2] == "rows")
		table.AddRows(numberOfRowsToAdd);
	else if (commandTokens[2] == "cols")
		table.AddCols(numberOfRowsToAdd);
}

void CommandProcessor::ProcessDeleteCommand(const std::vector<std::string>& commandTokens, Table& table)
{
	if (IsNumber(commandTokens[1]))
	{
		size_t nthRow = ConvertStringToUInt(commandTokens[1]);
		table.DeleteRow(nthRow - 1); // table indexing starts from 1, so we subtract 1, because arrays are indexed from 0
	}
	else
	{
		size_t colIndex = ConvertColLetterToNumber(commandTokens[1][0]);
		table.DeleteCol(colIndex);
	}
}

void CommandProcessor::ProcessInsertCommand(const std::vector<std::string>& commandTokens, Table& table)
{
	// check input for errors
	if (commandTokens[2] != "rows" && commandTokens[2] != "cols")
		throw std::invalid_argument("Error: unknown argument '" + commandTokens[3] + "'. Please specify 'rows' or 'cols'!");
	else if (commandTokens[3] != "before" && commandTokens[3] != "after")
		throw std::invalid_argument("Error: unknown argument '" + commandTokens[3] + "'. Please specify 'before' or 'after'!");
	
	
	size_t numberOfRowsOrColsToInsert = ConvertStringToUInt(commandTokens[1]);
	if (commandTokens[2] == "rows")
	{
		size_t rowNumber = ConvertStringToUInt(commandTokens[4]) - 1;
		if (commandTokens[3] == "before")
			table.InsertRowsBefore(rowNumber, numberOfRowsOrColsToInsert);
		else if (commandTokens[3] == "after")
			table.InsertRowsAfter(rowNumber, numberOfRowsOrColsToInsert);
	}
	else if (commandTokens[2] == "cols")
	{
		size_t colIndex = ConvertColLetterToNumber(commandTokens[4][0]);
		if (commandTokens[3] == "before")
			table.InsertColsBefore(colIndex, numberOfRowsOrColsToInsert);
		else if (commandTokens[3] == "after")
			table.InsertColsAfter(colIndex, numberOfRowsOrColsToInsert);
	}
}

void CommandProcessor::ProcessSaveCommand(const std::vector<std::string>& commandTokens, Table& table)
{
	std::ofstream ofs(commandTokens[1]);
	if (commandTokens.size() > 2)
	{
		if (commandTokens[2] == "-sep")
		{
			table.SaveToFile(ofs, commandTokens[3][0]);
		}
		else
		{
			throw std::invalid_argument("Error: unknown argument: " + commandTokens[2]);
		}
	}
	else
	{
		table.SaveToFile(ofs);
	}
}

void CommandProcessor::ProcessSortCommand(const std::vector<std::string>& commandTokens, Table& table)
{
	if (IsNumber(commandTokens[2]))
	{
		size_t rowNumber = ConvertStringToUInt(commandTokens[2]) - 1;
		if (commandTokens.size() < 4)
		{
			table.SortCols<SortDirection::ASC>(rowNumber);
			return;
		}

		if (commandTokens[3] == "asc")
		{
			table.SortCols<SortDirection::ASC>(rowNumber);
		}
		else if (commandTokens[3] == "desc")
		{
			table.SortCols<SortDirection::DESC>(rowNumber);
		}
	}
	else
	{
		size_t colNumber = ConvertColLetterToNumber(commandTokens[2][0]); // TOOD ??;
		if (commandTokens.size() < 4)
		{
			table.SortRows<SortDirection::ASC>(colNumber);
			return;
		}

		if (commandTokens[3] == "asc")
		{
			table.SortRows<SortDirection::ASC>(colNumber);
		}
		else if (commandTokens[3] == "desc")
		{
			table.SortRows<SortDirection::DESC>(colNumber);
		}
	}
}

void CommandProcessor::ProcessSwapCommand(const std::vector<std::string>& commandTokens, Table& table)
{
	const std::string& cell1 = commandTokens[1];
	const std::string& cell2 = commandTokens[2];

	size_t cell1Row = ConvertStringToUInt(cell1.substr(1, cell1.size() - 1)) - 1;
	size_t cell1Col = ConvertColLetterToNumber(cell1[0]);
	size_t cell2Row = ConvertStringToUInt(cell2.substr(1, cell1.size() - 1)) - 1;
	size_t cell2Col = ConvertColLetterToNumber(cell2[0]);
	std::swap(table[cell1Row][cell1Col], table[cell2Row][cell2Col]);
}

void CommandProcessor::ProcessAlignCommand(const std::vector<std::string>& commandTokens, Table& table)
{
	if (std::count(commandTokens[1].begin(), commandTokens[1].end(), ':') == 1)
	{
		auto cells = SplitString(commandTokens[1], ':');
		size_t cell1Row = ConvertStringToUInt(cells[0].substr(1, cells[0].size() - 1)) - 1;
		size_t cell1Col = ConvertColLetterToNumber(cells[0][0]);
		size_t cell2Row = ConvertStringToUInt(cells[1].substr(1, cells[1].size() - 1)) - 1;
		size_t cell2Col = ConvertColLetterToNumber(cells[1][0]);

		Range range(cell1Row, cell1Col, cell2Row, cell2Col);
		if (commandTokens[2] == "left")
		{
			table.SetAlignmentInRange(range, Alignment::LEFT);
		}
		else if (commandTokens[2] == "right")
		{
			table.SetAlignmentInRange(range, Alignment::RIGHT);
		}
	}
	else
	{
		size_t cellRow = ConvertStringToUInt(commandTokens[1].substr(1, commandTokens[1].size() - 1)) - 1;
		size_t cellCol = ConvertColLetterToNumber(commandTokens[1][0]);

		if (commandTokens[2] == "left")
		{
			table[cellRow][cellCol].SetAlignment(Alignment::LEFT);
		}
		else if (commandTokens[2] == "right")
		{
			table[cellRow][cellCol].SetAlignment(Alignment::RIGHT);
		}
	}
}

void CommandProcessor::ProcessClearCommand(const std::vector<std::string>& commandTokens, Table& table)
{
	auto cells = SplitString(commandTokens[1], ':');
	size_t cell1Row = ConvertStringToUInt(cells[0].substr(1, cells[0].size() - 1)) - 1;
	size_t cell1Col = ConvertColLetterToNumber(cells[0][0]);
	size_t cell2Row = ConvertStringToUInt(cells[1].substr(1, cells[1].size() - 1)) - 1;
	size_t cell2Col = ConvertColLetterToNumber(cells[1][0]);

	Range range(cell1Row, cell1Col, cell2Row, cell2Col);
	table.ClearRange(range);
}

void CommandProcessor::ProcessBarchartCommand(const std::vector<std::string>& commandTokens, Table& table)
{
	auto range = Range::Create(commandTokens[1]);
	BarchartDiagramData barchartDiagramData;
	barchartDiagramData.barCountInOneGroup = range.GetBottomRightCorner().second - range.GetTopLeftCorner().second;
	
	for (size_t i = range.GetTopLeftCorner().second + 1; i <= range.GetBottomRightCorner().second; ++i)
	{
		barchartDiagramData.groupMetaData.push_back({table[range.GetTopLeftCorner().first][i].GetContent(), Color(i % (int)Color::END) });
	}
	
	double maxInDiagram = 0;
	for (size_t i = range.GetTopLeftCorner().first + 1; i <= range.GetBottomRightCorner().first; ++i)
	{
		Group group;
		for (size_t j = range.GetTopLeftCorner().second + 1; j <= range.GetBottomRightCorner().second; ++j)
		{
			// TODO: what if GetContent == ""  ??
			auto value = ConvertStringToDouble(table[i][j].GetContent());
			if (value > maxInDiagram)
				maxInDiagram = value;
			group.values.push_back(value);
		}
		group.name = table[i][range.GetTopLeftCorner().second].GetContent();
		barchartDiagramData.groups.push_back(group);
	}
	
	size_t howManyNumbersOnAxisY = 14;
	double spaceBetweenNumbers = (maxInDiagram * 1.10) / howManyNumbersOnAxisY;
	for (size_t i = 0; i <= howManyNumbersOnAxisY; ++i)
		barchartDiagramData.yAxisNumbers.push_back(std::round(spaceBetweenNumbers * i * 1000) / 1000.0);
	
	std::ofstream htmlOfs(commandTokens[2] + ".html");
	htmlOfs << BarchartDiagram::CreateHtml(barchartDiagramData) << std::endl;
	
	std::ofstream svgOfs(commandTokens[2] + ".svg");
	svgOfs << BarchartDiagram::CreateSvg(barchartDiagramData) << std::endl;
}

void CommandProcessor::ProcessNewSheetCommand(const std::vector<std::string>& commandTokens, TableContainer& tableContainer)
{
	tableContainer.emplace_back(std::make_unique<Table>(commandTokens[2]));
	tableContainer.SetCurrentTable(tableContainer.size() - 1);
}

void CommandProcessor::ProcessOpenCommand(const std::vector<std::string>& commandTokens, TableContainer& tableContainer)
{
	if (commandTokens.size() > 2)
		Input::ReadFile(tableContainer, commandTokens[1], commandTokens[3][0]);
	else
		Input::ReadFile(tableContainer, commandTokens[1]);
}

void CommandProcessor::ProcessCloseCommand(const std::vector<std::string>& commandTokens, TableContainer& tableContainer)
{
	auto N = ConvertStringToUInt(commandTokens[1]);
	tableContainer.erase(tableContainer.begin() + N);
	
	if (tableContainer.empty())
		shouldExit = true;
	
	tableContainer.CorrectOutOfRangeCurrentTableIndex();
}

void CommandProcessor::ProcessTableRenameCommand(const std::vector<std::string>& commandTokens, TableContainer& tableContainer)
{
	auto N = ConvertStringToUInt(commandTokens[1]);
	tableContainer[N]->Rename(commandTokens[2]);
}

void CommandProcessor::ProcessSwitchCommand(const std::vector<std::string>& commandTokens, TableContainer& tableContainer)
{
	tableContainer.SetCurrentTable(ConvertStringToUInt(commandTokens[1]));
}

void CommandProcessor::ProcessCommand(const std::vector<std::string>& commandTokens, TableContainer& tableContainer)
{
	if (commandTokens[0] == "edit")
		ProcessEditCommand(commandTokens, tableContainer.GetCurrentTable());
	else if (commandTokens[0] == "add")
		ProcessAddCommand(commandTokens, tableContainer.GetCurrentTable());
	else if (commandTokens[0] == "delete")
		ProcessDeleteCommand(commandTokens, tableContainer.GetCurrentTable());
	else if (commandTokens[0] == "insert")
		ProcessInsertCommand(commandTokens, tableContainer.GetCurrentTable());
	else if (commandTokens[0] == "exit")
		shouldExit = true;
	else if (commandTokens[0] == "save")
		ProcessSaveCommand(commandTokens, tableContainer.GetCurrentTable());
	else if (commandTokens[0] == "sort" && commandTokens[1] == "by")
		ProcessSortCommand(commandTokens, tableContainer.GetCurrentTable());
	else if (commandTokens[0] == "swap")
		ProcessSwapCommand(commandTokens, tableContainer.GetCurrentTable());
	else if (commandTokens[0] == "align")
		ProcessAlignCommand(commandTokens, tableContainer.GetCurrentTable());
	else if (commandTokens[0] == "clear")
		ProcessClearCommand(commandTokens, tableContainer.GetCurrentTable());
	else if (commandTokens[0] == "barchart")
		ProcessBarchartCommand(commandTokens, table);
	else if (commandTokens[0] == "new" && commandTokens[1] == "sheet")
		ProcessNewSheetCommand(commandTokens, tableContainer);
	else if (commandTokens[0] == "open")
		ProcessOpenCommand(commandTokens, tableContainer);
	else if (commandTokens[0] == "close")
		ProcessCloseCommand(commandTokens, tableContainer);
	else if (commandTokens[0] == "rename")
		ProcessTableRenameCommand(commandTokens, tableContainer);
	else if (commandTokens[0] == "switch")
		ProcessSwitchCommand(commandTokens, tableContainer);
	else
	{
		std::string command;
		std::for_each(commandTokens.begin(), commandTokens.end(), [&](const auto& token) { command += token; });
		throw std::invalid_argument("Error: unknown command. Command was: " + command);
	}
}
