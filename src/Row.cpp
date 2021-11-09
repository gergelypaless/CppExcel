#include "Row.h"

#include <sstream>

size_t Row::rowLength = 1;
size_t Row::rowMaxLength = 26;


size_t Row::GetRowLength()
{
	return rowLength;
}

size_t Row::GetRowMaxLength()
{
	return rowMaxLength;
}

void Row::GrowRowLength(size_t N)
{
	if (rowLength + N > rowMaxLength)
		throw std::invalid_argument("Error: you cannot add that many columns to the table. The maximum is 26.");
	
	rowLength += N;
}

void Row::DecreaseRowLength(size_t N)
{
	if (rowLength - N < 1)
		throw std::invalid_argument("Error: you are trying to delete too many columns");

	rowLength -= N;
}

Row::Row()
{
	cells.reserve(rowMaxLength);
	for (size_t i = 0; i < rowLength; ++i)
		cells.emplace_back();
}

void Row::DeleteCell(size_t cellNumber)
{
	cells.erase(cells.begin() + cellNumber);
}

void Row::InsertColsBefore(size_t index, size_t N)
{
	cells.insert(cells.begin() + index, N, Cell());
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
		cells.emplace_back();
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
