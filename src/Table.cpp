#include "Table.h"

#include "utils.h"

#include <unordered_map>
#include <set>


Table::Table()
{
	rows.emplace_back(this);
}

void Table::AddRows(size_t N)
{
	for (size_t i = 0; i < N; ++i)
		rows.emplace_back(this);
}

void Table::AddCols(size_t N)
{
	Row::GrowRowLength(N);
	for (Row& row : rows)
		row.Grow(N);
}

void Table::DeleteRow(size_t rowIndex)
{
	if (rows.size() == 1)
		throw std::invalid_argument("Error: you are trying to delete too many rows.");
	else if (rows.size() <= rowIndex)
		throw std::invalid_argument("Error: you are trying to delete a row which does not exists.");

	rows.erase(rows.begin() + rowIndex);
}

void Table::DeleteCol(size_t colIndex)
{
	if (Row::GetRowLength() == 1)
		throw std::invalid_argument("Error: you are trying to delete too many cols.");
	else if (Row::GetRowLength() <= colIndex)
		throw std::invalid_argument("Error: you are trying to delete a col which does not exists.");

	Row::DecreaseRowLength(1);
	for (Row& row : rows)
		row.DeleteCell(colIndex);
}

void Table::InsertRowsBefore(size_t index, size_t N)
{
	if (index > rows.size())
		throw std::invalid_argument("Error: you cannot insert rows there. Specified index is too big.");
	
	for (size_t i = 0; i < N; ++i)
		rows.insert(rows.begin() + index + i, Row(this));
}

void Table::InsertRowsAfter(size_t index, size_t N)
{
	// inserting after is the same as inserting before the next one
	InsertRowsBefore(index + 1, N);
}

void Table::InsertColsBefore(size_t index, size_t N)
{
	if (index >= Row::GetRowLength())
		throw std::invalid_argument("Error: you cannot insert columns there. Specified index is too big.");
	else if (Row::GetRowLength() + N > Row::GetRowMaxLength())
		throw std::invalid_argument("Error: you are trying to insert too many columns.");

	Row::GrowRowLength(N);
	for (Row& row : rows)
		row.InsertColsBefore(index, N);
}

void Table::InsertColsAfter(size_t index, size_t N)
{
	// inserting after is the same as inserting before the next one
	InsertColsBefore(index + 1, N);
}

std::pair<std::vector<size_t>, size_t> Table::CalculateColsWidth() const
{
	std::vector<size_t> colWidths;
	colWidths.reserve(Row::GetRowLength());

	size_t all = 0;
	for (size_t i = 0; i < Row::GetRowLength(); i++)
	{
		size_t longest = 0;
		for (const Row& row : rows)
		{
			if (row[i].GetContent().size() > longest)
				longest = row[i].GetContent().size();
		}
		if (longest == 0)
			colWidths.push_back(1);
		else
			colWidths.push_back(longest);
		all += longest;
	}
	return { colWidths, all };
}

void Table::EvaluateCells()
{
	std::set<CellContent*> functionMap;
	for (auto& row : rows)
		row.ResetEvaluated(functionMap);
	
	size_t updated = -1;
	while (!functionMap.empty() && updated > 0)
	{
		updated = 0;
		auto first = functionMap.begin();
		while (first != functionMap.end())
		{
			if ((**first).Evaluate())
			{
				first = functionMap.erase(first);
				updated++;
			}
			else
			{
				first++;
			}
		}
	}
	
	if (updated == 0 && !functionMap.empty())
		std::for_each(functionMap.begin(), functionMap.end(), [] (CellContent* cellContent) { static_cast<FunctionCellContent*>(cellContent)->SetInCycle(); });
}

// os: most of the time it is std::cout
void Table::Print(std::ostream& os)
{
	EvaluateCells();
	
	auto[colWidths, allSum] = CalculateColsWidth();
	size_t firstColWidth = StringLengthOf(rows.size());

	os << "Table:\n" << std::string(firstColWidth, ' ');
	for (size_t i = 0; i < Row::GetRowLength(); ++i)
		os << "|" << ConvertNumberToColLetter(i) << std::string(colWidths[i] - 1, ' ');
	os << "|\n";
	
	//		  spaces          the '|' characters   last '|' character
	allSum += firstColWidth + Row::GetRowLength() + 1;
	for (size_t i = 0; i < rows.size(); ++i)
	{
		os << std::string(allSum, '-') << '\n';
		os << i + 1 << std::string(firstColWidth - StringLengthOf(i + 1), ' ') << "|";
		rows[i].Print(os, colWidths);
	}
	os << std::string(allSum, '-') << '\n' << std::endl;
}

Row& Table::operator[](size_t idx)
{
	// .at() throws exception if the index is too big
	return rows.at(idx);
}

const Row& Table::operator[](size_t idx) const
{
	return rows.at(idx);
}

void Table::SetAlignmentInRange(const Range& range, Alignment alignment)
{
	Traverse(range, [=](Cell& cell) {
		cell.SetAlignment(alignment);
	});
}

void Table::ClearRange(const Range& range)
{
	auto [topLeftRow, topLeftCol] = range.GetTopLeftCorner();
	auto [bottomRightRow, bottomRightCol] = range.GetBottomRightCorner();
	for (size_t i = topLeftRow; i <= bottomRightRow; i++)
	{
		for (size_t j = topLeftCol; j <= bottomRightCol; j++)
		{
			rows[i][j].Clear();
		}
	}
}

Row& Table::LastRow()
{
	return rows[rows.size() - 1];
}

const Row& Table::LastRow() const
{
	return rows[rows.size() - 1];
}

size_t Table::LastRowIndex() const
{
	return rows.size() - 1;
}

void Table::SaveToFile(std::ofstream& ofs, char sep) const
{
	for (const Row& row : rows)
	{
		row.SaveToFile(ofs, sep);
		ofs << std::endl;
	}
}

void Table::SortColsASC(size_t rowNumber)
{
	const Row& rowToSortBy = rows[rowNumber];
	size_t n = Row::GetRowLength();
	size_t i, j;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (rowToSortBy[j + 1] < rowToSortBy[j])
			{
				for (Row& row : rows)
				{
					std::swap(row[j], row[j + 1]);
				}
			}
		}
	}
}

void Table::SortColsDESC(size_t rowNumber)
{
	const Row& rowToSortBy = rows[rowNumber];
	size_t n = Row::GetRowLength();
	size_t i, j;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (rowToSortBy[j] < rowToSortBy[j + 1])
			{
				for (Row& row : rows)
				{
					std::swap(row[j], row[j + 1]);
				}
			}
		}
	}
}

void Table::SortRowsASC(size_t colNumber)
{
	std::sort(rows.begin(), rows.end(), [colNumber](const Row& rowLeft, const Row& rowRight) {
		return rowLeft[colNumber] < rowRight[colNumber];
	});
}

void Table::SortRowsDESC(size_t colNumber)
{
	std::sort(rows.begin(), rows.end(), [colNumber](const Row& rowLeft, const Row& rowRight) {
		return rowRight[colNumber] < rowLeft[colNumber];
	});
}

