#include "TableContainer.h"

void TableContainer::SetCurrentTable(size_t newCurrentTableIndex)
{
	if (newCurrentTableIndex >= size())
		throw std::invalid_argument("new current table index is out of bound");
	
	currentTableIndex = newCurrentTableIndex;
}

Table& TableContainer::GetCurrentTable()
{
	if (empty())
		throw std::invalid_argument("TableContainer is empty");
	
	return *at(currentTableIndex);
}

void TableContainer::PrintTableNames(std::ostream& os)
{
	for (size_t i = 0; i < size(); ++i)
	{
		os << i << (i == currentTableIndex ? "*" : "") << ": " << at(i)->GetName() << " ";
	}
	os << "\n";
}

void TableContainer::CorrectOutOfRangeCurrentTableIndex()
{
	currentTableIndex = std::min(size(), currentTableIndex - 1);
}
