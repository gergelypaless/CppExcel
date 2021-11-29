#include "Row.h"
#include "Table.h"

#include <sstream>

Row::Row(Table* table) : table(table)
{
	cells.reserve(Table::GetRowMaxLength());
	for (size_t i = 0; i < table->GetRowLength(); ++i)
		cells.emplace_back(*table);
}

void Row::DeleteCell(size_t cellNumber)
{
	cells.erase(cells.begin() + cellNumber);
}

void Row::InsertColsBefore(size_t index, size_t N)
{
	for (size_t i = 0; i < N; ++i)
		cells.insert(cells.begin() + index + i, Cell(*table));
}

void Row::InsertColsAfter(size_t index, size_t N)
{
	// inserting after is the same as inserting before the next one
	InsertColsBefore(index + 1, N);
}

void Row::Print(std::ostream& os, const std::vector<size_t>& colWidths) const
{
	for (size_t i = 0; i < cells.size(); ++i)
	{
		cells[i].Print(os, colWidths[i]);
		os << "|";
	}
	os << std::endl;
}

Cell& Row::operator[](size_t col)
{
	// .at() throws an exception if col is too big
	return cells.at(col);
}

const Cell& Row::operator[](size_t col) const
{
	return cells.at(col);
}

void Row::Grow(size_t N)
{
	for (size_t i = 0; i < N; ++i)
		cells.emplace_back(*table);
}


void Row::SaveToFile(std::ofstream& ofs, char sep) const
{
	for (size_t i = 0; i < cells.size() - 1; ++i)
	{
		cells[i].SaveToFile(ofs);
		ofs << sep;
	}
	cells.back().SaveToFile(ofs);
}

Row& Row::operator=(Row&& other) noexcept
{
	std::swap(cells, other.cells);
	std::swap(table, other.table);
	return *this;
}
