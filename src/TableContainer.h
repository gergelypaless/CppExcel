#pragma once

#include "Table.h"

#include <vector>
#include <ostream>
#include <memory>

class TableContainer : public std::vector<std::unique_ptr<Table>>
{
public:
	TableContainer() = default;
	
	void SetCurrentTable(size_t newCurrentTableIndex);
	Table& GetCurrentTable();
	void PrintTableNames(std::ostream& os);
	
	void CorrectOutOfRangeCurrentTableIndex();

private:
	size_t currentTableIndex = 0;
};