#pragma once

#include "Cell.h"
#include "SortDirection.h"

#include <vector>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <set>

// forward declaration
class Table;


/*
 * this class  represents a row 
 */
class Row
{
public:
	Row(Table* table);
	Row(const Row& other) = delete;
	Row& operator=(const Row& other) = delete;
	
	Row(Row&& other) noexcept : cells(std::move(other.cells)), table(other.table) {}
	Row& operator=(Row&& other) noexcept;
	
	/*
	 * deletes a cell at cellNumber
	 * 
	 * @param cellNumber	the index of the cell to delete
	 */
	void DeleteCell(size_t cellNumber);
	
	/*
	 * inserts N cols before index
	 * 
	 * @param index		index to insert before
	 * @param N		amount of cells to insert
	 */
	void InsertColsBefore(size_t index, size_t N);

	/*
	 * inserts N cols after index
	 *
	 * @param index		index to insert after
	 * @param N		amount of cells to insert
	 */
	void InsertColsAfter(size_t index, size_t N);
	
	/*
	 * helper function to get a cell in a row
	 * 
	 * @param col	the index of the cell to get
	 */
	Cell& operator[](size_t col);
	const Cell& operator[](size_t col) const;
	
	/*
	 * prints the row to the ostream os
	 * 
	 * @param os	the ostream to print the row
	 * @param colWidths		describes how big each column is
	 */
	void Print(std::ostream& os, const std::vector<size_t>& colWidths) const;

	/*
	 * prints the row to the ofstream ofs
	 *
	 * @param ofs	the file to print the row
	 * @param sep	column separator
	 */
	void SaveToFile(std::ofstream& ofs, char sep = ';') const;

	/*
	 * grow this row by N
	 * 
	 * @param amount to grow with
	 */
	void Grow(size_t N);
	
	void ResetEvaluated(std::set<CellContent*>& functionMap)
	{
		for (auto& cell : cells)
			cell.ResetEvaluated(functionMap);
	}

private:
	std::vector<Cell> cells;
	Table* table;
};



