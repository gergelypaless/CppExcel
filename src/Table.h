#pragma once

#include "Row.h"
#include "SortDirection.h"
#include "Range.h"

#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>

/*
 * this class represents a table
 */
class Table
{
public:
	Table();
	
	/*
	 * adds N rows
	 * 
	 * @param N		amount of rows to add
	 */
	void AddRows(size_t N);

	/*
	 * adds N cols
	 *
	 * @param N		amount of columns to add
	 */
	void AddCols(size_t N);

	/*
	 * delete N rows
	 *
	 * @param N		amount of rows to delete
	 */
	void DeleteRow(size_t rowIndex);

	/*
	 * delete N columns
	 *
	 * @param N		amount of columns to delete
	 */
	void DeleteCol(size_t colIndex);
	
	/*
	 * inserts N rows before index
	 *
	 * @param index		the index to insert before
	 * @param N		amount of rows to insert
	 */
	void InsertRowsBefore(size_t index, size_t N);

	/*
	 * inserts N rows after index
	 *
	 * @param index		the index to insert after
	 * @param N		amount of rows to insert
	 */
	void InsertRowsAfter(size_t index, size_t N);

	/*
	 * inserts N columns before index
	 *
	 * @param index		the index to insert before
	 * @param N		amount of columns to insert
	 */
	void InsertColsBefore(size_t index, size_t N);

	/*
	 * inserts N columns after index
	 *
	 * @param index		the index to insert after
	 * @param N		amount of columns to insert
	 */
	void InsertColsAfter(size_t index, size_t N);
	
	/*
	 * template function to sort the columns by sortDirection
	 * 
	 * @param rowNumber		the index of the row to sort by
	 */
	template<SortDirection sortDirection>
	void SortCols(size_t rowNumber)
	{
		if constexpr (sortDirection == SortDirection::ASC)
			SortColsASC(rowNumber);
		else if constexpr (sortDirection == SortDirection::DESC)
			SortColsDESC(rowNumber);
	}

	/*
	 * template function to sort the rows by sortDirection
	 *
	 * @param rowNumber		the index of the column to sort by
	 */
	template<SortDirection sortDirection>
	void SortRows(size_t colNumber)
	{
		if constexpr (sortDirection == SortDirection::ASC)
			SortRowsASC(colNumber);
		else if constexpr (sortDirection == SortDirection::DESC)
			SortRowsDESC(colNumber);
	}

	/*
	 * sets all the cell alignment in a range
	 * 
	 * @param range		the range to set the alignment in
	 * @param alignment		the alignment to set
	 */
	void SetAlignmentInRange(const Range& range, Alignment alignment);

	/*
	 * clears all the cells in a range
	 * 
	 * @param range		the range to clear the cells in
	 */
	void ClearRange(const Range& range);

	/*
	 * returns the last row
	 */
	Row& LastRow();
	const Row& LastRow() const;

	/*
	 * retunrs the last row index
	 */
	size_t LastRowIndex() const;

	/*
	 * prints the table to an ostream
	 * 
	 * @param os	the ostream to print to
	 */
	void Print(std::ostream& os);

	/*
	 * saves the table to a file
	 * 
	 * @param ofs		the ofstream to save to
	 * @param sep		column separator
	 */
	void SaveToFile(std::ofstream& ofs, char sep = ';') const;
	
	/*
	 * returns the row at index idx
	 * 
	 * @param idx	the index to get the row at
	 */
	Row& operator[](size_t idx);
	const Row& operator[](size_t idx) const;
	
	/*
	 * this function goes through all the cells which are inside a range and calls a callback function
	 *
	 * @param range: the range to iterate over on
	 * @param someFunction: callback function
	 */
	void Traverse(const Range& range, const std::function<void(Cell&)>& someFunction)
	{
		auto [topLeftRow, topLeftCol] = range.GetTopLeftCorner();
		auto [bottomRightRow, bottomRightCol] = range.GetBottomRightCorner();
		for (size_t i = topLeftRow; i <= bottomRightRow; i++)
			for (size_t j = topLeftCol; j <= bottomRightCol; j++)
				someFunction(rows[i][j]);
	}
	
	void Traverse(const Range& range, const std::function<void(const Cell&)>& someFunction) const
	{
		auto [topLeftRow, topLeftCol] = range.GetTopLeftCorner();
		auto [bottomRightRow, bottomRightCol] = range.GetBottomRightCorner();
		for (size_t i = topLeftRow; i <= bottomRightRow; i++)
			for (size_t j = topLeftCol; j <= bottomRightCol; j++)
				someFunction(rows[i][j]);
	}
	
private:
	std::pair<std::vector<size_t>, size_t> CalculateColsWidth() const;
	
	void SortColsASC(size_t rowNumber);
	void SortColsDESC(size_t rowNumber);
	
	void SortRowsASC(size_t colNumber);
	void SortRowsDESC(size_t colNumber);
	
	/*
	 * evaluates all cells
	 */
	void EvaluateCells();
	
private:
	std::vector<Row> rows;
};